/* USER CODE BEGIN Header */
/**
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usb_device.h"
#include "usbd_cdc_if.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "main.c"
#ifdef __cplusplus
}
#endif

#include <cassert>
#include <cstdio>
#include <cstring>

#include <array>
#include <numeric>
#include <vector>

#include "channel_data.hpp"

#include "swo_segger.h"

// initialize all hardware subsystem before doing any other logic
__attribute__((__constructor__)) void initHw() {
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();

  MX_ADC1_Init();
  MX_ADC2_Init();

  MX_USB_DEVICE_Init();

  MX_TIM2_Init();
  MX_TIM3_Init();

  HAL_Delay(1000);  // delay so that usb can initialize properly
}

class CriticalSectionLockGuard {
 public:
  CriticalSectionLockGuard() { __disable_irq(); }
  ~CriticalSectionLockGuard() { __enable_irq(); }
};

volatile bool usbDisconnected = true;

auto&    timestampTimer = htim2;
uint32_t getTimestamp() { return timestampTimer.Instance->CNT; }

template <typename T, int BufferSize>
class CircularBuffer {
 private:
  T*   _buf        = new T[BufferSize];
  int  _readIndex  = 0;
  int  _writeIndex = 0;
  bool _isFull     = false;

  int dropCnt = 0;

  int getNextIndex(const int index) volatile { return ((index + 1) % BufferSize); }

 public:
  bool isFull() volatile { return _isFull; }
  bool isEmpty() volatile { return !_isFull && _writeIndex == _readIndex; }
  T    read(bool& empty) volatile {
    if (isEmpty()) {
      empty = true;
      return T();
    }
    empty      = false;
    auto ret   = _buf[_readIndex];
    _readIndex = getNextIndex(_readIndex);
    if (_readIndex == _writeIndex) { _isFull = false; }
    return ret;
  }

  void write(const T& data) volatile {
    // is usb is disconnected, overwrite
    if (isFull()) {
      SWO_PrintStringLine("Dropped");
      // while (1) {}
      if (usbDisconnected) {
        _readIndex = getNextIndex(_readIndex);
      } else {
        // empty
      }
    }
    _buf[_writeIndex] = data;
    _writeIndex       = getNextIndex(_writeIndex);
    if (_writeIndex == _readIndex) { _isFull = true; }
  }

  int size() volatile {
    return _isFull ? BufferSize
                   : (_writeIndex < _readIndex) ? (_writeIndex + BufferSize) - _readIndex
                                                : _writeIndex - _readIndex;
  }

  int capacity() volatile { return BufferSize; }
};
typedef CircularBuffer<ChannelDataPkt, 15> UsbBuffer;
static UsbBuffer                           usbBuf;
static volatile UsbBuffer*                 volatileBuf = &usbBuf;

using AdcDataType = uint16_t;
class AdcWrapper {
 public:
  ADC_HandleTypeDef& adcHandle;
  AdcDataType*       data;
  int                _currTotalConversion;

  AdcWrapper(ADC_HandleTypeDef& hadc, const int maxTotalConversion, const int currTotalConversion)
      : adcHandle{hadc},
        data{new AdcDataType[maxTotalConversion]},
        _currTotalConversion{currTotalConversion} {
    HAL_ADC_Start_DMA(&adcHandle, (uint32_t*)(data), _currTotalConversion);
  }

  ~AdcWrapper() { delete[] data; }
};

class AdcChannel {
 private:
  int                _currSample = 0;
  ChannelDataPkt     _data;
  const AdcDataType* _buf;
  int                _startIndex;
  int                _endIndex;

 public:
  const ADC_HandleTypeDef& adcHandle;

  // endIndex inclusive
  AdcChannel(const uint8_t              channelID,
             const volatile AdcWrapper& adcWrapper,
             const int                  startIndex,
             const int                  endIndex)
      : _data{.channelID = channelID}, _buf{adcWrapper.data}, adcHandle{adcWrapper.adcHandle} {
    changeIndexRange(startIndex, endIndex);
  }

  void addSample() {
    AdcDataType sum = 0;
    for (auto i = _startIndex; i <= _endIndex; ++i) { sum += _buf[i]; }
    _data.samples[_currSample].adcData   = sum / (_endIndex - _startIndex + 1);
    _data.samples[_currSample].timestamp = getTimestamp();

    _currSample = (_currSample + 1) % kMaxSamplePerPkt;
    if (0 == _currSample) { volatileBuf->write(_data); }
  }

  void changeIndexRange(const int startIndex, const int endIndex) {
    assert(endIndex >= startIndex);
    _startIndex = startIndex;
    _endIndex   = endIndex;
  }
};

static constexpr auto kAdc1TotalConversion = 4;
static constexpr auto kAdc2TotalConversion = 4;

static std::array<volatile AdcWrapper, 1> adcs{
    AdcWrapper{hadc1, kAdc1TotalConversion, kAdc1TotalConversion}
    // ,AdcWrapper{hadc2, kAdc2TotalConversion, kAdc2TotalConversion}
};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  static std::array<AdcChannel, 4> adcChannels{AdcChannel{0, adcs[0], 0, 0},
                                               AdcChannel{1, adcs[0], 1, 1},
                                               AdcChannel{2, adcs[0], 2, 2},
                                               AdcChannel{3, adcs[0], 3, 3}};

  for (auto& adcChannel : adcChannels) {
    if (hadc == &(adcChannel.adcHandle)) { adcChannel.addSample(); }
  }
}

auto& samplingTimer = htim3;

int main(void) {
  auto           disconnectCnt    = 0;
  auto           isEmpty          = false;
  auto           prevTransferDone = true;
  ChannelDataPkt pkt;

  HAL_TIM_Base_Start_IT(&samplingTimer);
  HAL_TIM_Base_Start_IT(&timestampTimer);

  while (1) {
    if (prevTransferDone) {
      CriticalSectionLockGuard l();

      usbDisconnected = (disconnectCnt >= 5);
      pkt             = volatileBuf->read(isEmpty);
    }
    if (!isEmpty) {
      if (USBD_BUSY == CDC_Transmit_HS(reinterpret_cast<uint8_t*>(&pkt), sizeof(pkt))) {
        if (!usbDisconnected) { ++disconnectCnt; }
        prevTransferDone = false;
      } else {
        disconnectCnt    = 0;
        prevTransferDone = true;
      }
    }
  }
}
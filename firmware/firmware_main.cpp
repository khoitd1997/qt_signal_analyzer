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

class CriticalSectionLockGuard {
 public:
  CriticalSectionLockGuard() { __disable_irq(); }
  ~CriticalSectionLockGuard() { __enable_irq(); }
};

template <typename T, int BufferSize>
class CircularBuffer {
 private:
  T*   _buf        = new T[BufferSize];
  int  _readIndex  = 0;
  int  _writeIndex = 0;
  bool _isFull     = false;

  int getNextIndex(const int index) volatile { return ((index + 1) % BufferSize); }

 public:
  bool isFull() volatile { return _isFull; }
  bool isEmpty() volatile { return !_isFull && _writeIndex == _readIndex; }
  T    read() volatile {
    if (isEmpty()) {
      while (1) {}
    }
    auto ret   = _buf[_readIndex];
    _readIndex = getNextIndex(_readIndex);
    if (_readIndex == _writeIndex) { _isFull = false; }
    return ret;
  }

  void write(const T& data) volatile {
    if (isFull()) {
      _readIndex = getNextIndex(_readIndex);
      while (1) {}
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
typedef CircularBuffer<ChannelDataPkt, 50> UsbBuffer;
static UsbBuffer                           usbBuf;
static volatile UsbBuffer*                 volatileBuf = &usbBuf;

using AdcDataType = uint16_t;
class AdcWrapper {
 public:
  ADC_HandleTypeDef&       adcHandle;
  std::vector<AdcDataType> data;

  AdcWrapper(ADC_HandleTypeDef& hadc, const int maxTotalConversion, const int currTotalConversion)
      : adcHandle{hadc}, data(currTotalConversion) {
    data.reserve(maxTotalConversion);
  }

  void startSampling() {
    HAL_ADC_Start_DMA(&adcHandle, reinterpret_cast<uint32_t*>(data.data()), data.size());
  }
};

class AdcChannel {
 private:
  int                             _currSample = 0;
  ChannelDataPkt                  _data;
  const std::vector<AdcDataType>& _buf;
  int                             _startIndex;
  int                             _endIndex;

 public:
  const ADC_HandleTypeDef& adcHandle;

  // endIndex inclusive
  AdcChannel(const uint8_t     channelID,
             const AdcWrapper& adcWrapper,
             const int         startIndex,
             const int         endIndex)
      : _data{channelID, 0}, _buf{adcWrapper.data}, adcHandle{adcWrapper.adcHandle} {
    changeIndexRange(startIndex, endIndex);
  }

  void addSample() {
    AdcDataType sum = 0;
    for (auto i = _startIndex; i <= _endIndex; ++i) { sum += _buf[i]; }
    _data.samples[_currSample].adcData = sum / (_endIndex - _startIndex + 1);
    _data.samples[_currSample].tickCnt = HAL_GetTick();  // TODO(kd): Replace this

    _currSample = (_currSample + 1) % kSamplePerPkt;
    if (0 == _currSample) { volatileBuf->write(_data); }
  }

  void changeIndexRange(const int startIndex, const int endIndex) {
    assert(endIndex >= startIndex && endIndex < static_cast<int>(_buf.capacity()));
    _startIndex = startIndex;
    _endIndex   = endIndex;
  }
};

constexpr auto kAdc1TotalConversion = 4;
constexpr auto kAdc2TotalConversion = 4;

static std::array<AdcWrapper, 2> adcs{
    AdcWrapper{hadc1, kAdc1TotalConversion, kAdc1TotalConversion},
    AdcWrapper{hadc2, kAdc2TotalConversion, kAdc2TotalConversion}};

static std::array<AdcChannel, 1> adcChannels{AdcChannel{0, adcs[0], 0, kAdc1TotalConversion - 1}};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  for (auto& adcChannel : adcChannels) {
    if (hadc == &(adcChannel.adcHandle)) { adcChannel.addSample(); }
  }

  for (auto& adc : adcs) {
    if (hadc == &(adc.adcHandle)) { adc.startSampling(); }
  }
}

int main(void) {
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_USB_DEVICE_Init();

  HAL_Delay(1000);  // delay so that usb can initialize properly

  while (1) {
    auto           isEmpty = true;
    ChannelDataPkt pkt;
    {
      CriticalSectionLockGuard l();
      isEmpty = volatileBuf->isEmpty();
      if (!isEmpty) { pkt = volatileBuf->read(); }
    }
    if (!isEmpty) {
      while (USBD_BUSY == CDC_Transmit_HS(reinterpret_cast<uint8_t*>(&pkt), sizeof(pkt))) {
        // wait until usb is not busy and pkt is sent
      }
    }
  }
}
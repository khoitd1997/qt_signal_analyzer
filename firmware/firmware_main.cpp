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

#include "channel_data.h"

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
  MX_ADC3_Init();

  MX_USB_DEVICE_Init();

  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
}

class CriticalSectionLockGuard {
 public:
  CriticalSectionLockGuard() { __disable_irq(); }
  ~CriticalSectionLockGuard() { __enable_irq(); }
};

auto&    timestampTimer = htim2;
uint32_t getTimestamp() { return timestampTimer.Instance->CNT; }

template <typename T, int BufferSize>
class CircularBuffer {
 private:
  static_assert(BufferSize > 2, "Buffer size must be at least 3");

  T   _buf[BufferSize];
  int _readIndex  = 0;
  int _writeIndex = 0;

  int getNextIndex(const int index) volatile { return ((index + 1) % BufferSize); }

 public:
  T* read() volatile {
    auto tempIndex = getNextIndex(_readIndex);
    if (tempIndex == _writeIndex) { return nullptr; }

    _readIndex = tempIndex;
    return _buf + _readIndex;
  }

  T* front() volatile { return _buf; }

  T* write() volatile {
    auto tempIndex = getNextIndex(_writeIndex);
    if (tempIndex == _readIndex) {
      SWO_PrintStringLine("Dropped");
      return nullptr;
    }
    _writeIndex = tempIndex;
    return _buf + _writeIndex;
  }

  int capacity() volatile { return BufferSize; }
};
typedef CircularBuffer<volatile ChannelDataPkt, 3> UsbBuffer;
static UsbBuffer                                   usbBuf;
static volatile UsbBuffer*                         volatileBuf = &usbBuf;

using AdcDataType = uint16_t;
class AdcWrapper {
 public:
  ADC_HandleTypeDef& adcHandle;
  AdcDataType*       data;
  int                _currTotalConversion;

  AdcWrapper(ADC_HandleTypeDef& hadc, const int maxTotalConversion, const int currTotalConversion)
      : adcHandle{hadc},
        data{new AdcDataType[maxTotalConversion]},
        _currTotalConversion{currTotalConversion} {}

  ~AdcWrapper() { delete[] data; }
};

// class AdcChannel {
//  private:
//   int                _currSample = 0;
//   ChannelDataPkt     _data;
//   const AdcDataType* _buf;
//   int                _startIndex;
//   int                _endIndex;

//  public:
//   const ADC_HandleTypeDef& adcHandle;

//   // endIndex inclusive
//   AdcChannel(const uint8_t              channelID,
//              const volatile AdcWrapper& adcWrapper,
//              const int                  startIndex,
//              const int                  endIndex)
//       : _data{.channelID = channelID}, _buf{adcWrapper.data}, adcHandle{adcWrapper.adcHandle} {
//     changeIndexRange(startIndex, endIndex);
//   }

//   void addSample() {
//     AdcDataType sum = 0;
//     for (auto i = _startIndex; i <= _endIndex; ++i) { sum += _buf[i]; }
//     _data.samples[_currSample].adcData   = sum / (_endIndex - _startIndex + 1);
//     _data.samples[_currSample].timestamp = getTimestamp();

//     _currSample = (_currSample + 1) % kMaxSamplePerPkt;
//     if (0 == _currSample) { volatileBuf->write(_data); }
//   }

//   void changeIndexRange(const int startIndex, const int endIndex) {
//     assert(endIndex >= startIndex);
//     _startIndex = startIndex;
//     _endIndex   = endIndex;
//   }
// };

static constexpr auto kAdc1TotalConversion = 4;
static constexpr auto kAdc2TotalConversion = 4;

// static std::array<volatile AdcWrapper, 1> adcs{
//     AdcWrapper{hadc1, kAdc1TotalConversion, kAdc1TotalConversion}
//     // ,AdcWrapper{hadc2, kAdc2TotalConversion, kAdc2TotalConversion}
// };

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//   static std::array<AdcChannel, 4> adcChannels{AdcChannel{0, adcs[0], 0, 0},
//                                                AdcChannel{1, adcs[0], 1, 1},
//                                                AdcChannel{2, adcs[0], 2, 2},
//                                                AdcChannel{3, adcs[0], 3, 3}};

//   for (auto& adcChannel : adcChannels) {
//     if (hadc == &(adcChannel.adcHandle)) { adcChannel.addSample(); }
//   }
// }

void startAdcDMA(volatile ChannelDataPkt* pkt) {
  const auto ret0 = HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(pkt->adcData0), kMaxSamplePerPkt);
  const auto ret1 = HAL_ADC_Start_DMA(&hadc2, (uint32_t*)(pkt->adcData1), kMaxSamplePerPkt);
  const auto ret2 = HAL_ADC_Start_DMA(&hadc3, (uint32_t*)(pkt->adcData23), kMaxSamplePerPkt * 2);

  if (ret0 || ret1 || ret2) { SWO_PrintStringLine("adc problem"); }
}

auto& bufferSwapTimer = htim4;
class BufferSwapTimerLockGuard {
 public:
  BufferSwapTimerLockGuard() { __HAL_TIM_DISABLE_IT(&bufferSwapTimer, TIM_IT_UPDATE); }
  ~BufferSwapTimerLockGuard() { __HAL_TIM_ENABLE_IT(&bufferSwapTimer, TIM_IT_UPDATE); }
};
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (htim == &bufferSwapTimer) {
    // SWO_PrintStringLine("buffer");
    auto writeElem = volatileBuf->write();
    if (nullptr != writeElem) {
      startAdcDMA(writeElem);
      auto ret = HAL_TIM_IC_Start_DMA(
          &timestampTimer, TIM_CHANNEL_1, (uint32_t*)(writeElem->timestamp), kMaxSamplePerPkt);
      if (ret != HAL_OK) {
        SWO_PrintStringLine("dma problem");
        if (ret == HAL_ERROR) { SWO_PrintStringLine("dma busy"); }
      }
    }
  }
}

auto& samplingTimer = htim3;

int main(void) {
  auto writeElem = volatileBuf->front();
  startAdcDMA(writeElem);

  HAL_TIM_PWM_Start(&samplingTimer, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&bufferSwapTimer);
  HAL_TIM_IC_Start_DMA(
      &timestampTimer, TIM_CHANNEL_1, (uint32_t*)(writeElem->timestamp), kMaxSamplePerPkt);

  auto pkt              = volatileBuf->front();
  auto isEmpty          = false;
  auto prevTransferDone = true;
  HAL_Delay(100);  // wait till there are at least some packets and for USB to init properly

  while (1) {
    if (!isEmpty) {
      prevTransferDone = true;
      if (USBD_BUSY == CDC_Transmit_HS((uint8_t*)(pkt), sizeof(ChannelDataPkt))) {
        prevTransferDone = false;
      }
    }

    if (prevTransferDone) {
      BufferSwapTimerLockGuard lock();

      pkt     = volatileBuf->read();
      isEmpty = (pkt == nullptr);
    }
  }
}
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
    if (0 == _currSample) {
      CDC_Transmit_HS(reinterpret_cast<uint8_t*>(&_data), sizeof(ChannelDataPkt));
    }
  }

  void changeIndexRange(const int startIndex, const int endIndex) {
    assert(endIndex >= startIndex && endIndex < static_cast<int>(_buf.capacity()));
    _startIndex = startIndex;
    _endIndex   = endIndex;
  }
};

constexpr auto kAdc1TotalConversion = 4;
constexpr auto kAdc2TotalConversion = 4;

static std::array<AdcWrapper, 1> adcs{
    // AdcWrapper{hadc1, kAdc1TotalConversion, kAdc1TotalConversion}
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
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_Delay(2000);
  for (auto& adc : adcs) { adc.startSampling(); }
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
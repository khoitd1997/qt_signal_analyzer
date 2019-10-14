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

#include <cstdio>
#include <cstring>

#include <array>
#include <numeric>

#include "channel_data.hpp"

static ChannelData channel1Data{0, 0};

constexpr auto kChannel1Oversampling = 4;
using AdcDataType                    = uint16_t;
static std::array<volatile AdcDataType, kChannel1Oversampling> adcChannel1Data{0};

static constexpr auto kUsbBufLen         = 400;
char                  usbBuf[kUsbBufLen] = {0};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  static int totalTime = 0;
  const auto currTick  = HAL_GetTick();
  if (hadc == &hadc1) {
    static int currSample = 0;
    channel1Data.samples[currSample].adcData =
        std::accumulate(adcChannel1Data.begin(), adcChannel1Data.end(), 0) / kChannel1Oversampling;
    channel1Data.samples[currSample].tickCnt = currTick;
    if (kSampleCnt == currSample + 1) {
      // memcpy(reinterpret_cast<void*>(usbBuf),
      //        reinterpret_cast<void*>(&channel1Data),
      //        sizeof(ChannelData));
      CDC_Transmit_HS(reinterpret_cast<uint8_t*>(&channel1Data), sizeof(ChannelData));

      // const auto len = snprintf(usbBuf, kUsbBufLen, "Data: 1000\n");
      // if (totalTime < 5000) {
      // CDC_Transmit_HS(reinterpret_cast<uint8_t*>(usbBuf), len);
      //   ++totalTime;
      // }
    }
    currSample = (currSample + 1) % kSampleCnt;

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(adcChannel1Data.data()), kChannel1Oversampling);
  } else if (hadc == &hadc2) {
    // HAL_ADC_Start_DMA(&hadc2, (uint32_t*)(adcChannel1Data.data()), 1);
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
  // if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adcChannel1Data, 1) != HAL_OK) {
  //   /* Start Conversation Error */
  //   Error_Handler();
  // }

  // HAL_Delay(5000);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(adcChannel1Data.data()), kChannel1Oversampling);
  // HAL_ADC_Start_DMA(&hadc2, (uint32_t*)(adcChannel1Data.data()), 1);
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
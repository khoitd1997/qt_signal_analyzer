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
// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//   // while (1) {}
// }

#ifdef __cplusplus
}
#endif

__IO uint16_t uhADCxConvertedValue[16] = {0};

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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&uhADCxConvertedValue, 1) != HAL_OK) {
  //   /* Start Conversation Error */
  //   Error_Handler();
  // }

  char buf[] = "Hello";
  while (1) {
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(uhADCxConvertedValue), 3);
    // CDC_Transmit_FS(reinterpret_cast<uint8_t *>(buf), sizeof(buf));
    HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
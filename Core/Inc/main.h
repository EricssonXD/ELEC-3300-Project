/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "wifi.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Key2_Pin GPIO_PIN_13
#define Key2_GPIO_Port GPIOC
#define Key1_Pin GPIO_PIN_0
#define Key1_GPIO_Port GPIOA
#define LCD_Backlight_Pin GPIO_PIN_12
#define LCD_Backlight_GPIO_Port GPIOD
#define Col2_Pin GPIO_PIN_6
#define Col2_GPIO_Port GPIOC
#define Col1_Pin GPIO_PIN_7
#define Col1_GPIO_Port GPIOC
#define Row4_Pin GPIO_PIN_8
#define Row4_GPIO_Port GPIOC
#define Row3_Pin GPIO_PIN_9
#define Row3_GPIO_Port GPIOC
#define Row2_Pin GPIO_PIN_10
#define Row2_GPIO_Port GPIOC
#define Row1_Pin GPIO_PIN_11
#define Row1_GPIO_Port GPIOC
#define Col3_Pin GPIO_PIN_12
#define Col3_GPIO_Port GPIOC
#define Col4_Pin GPIO_PIN_2
#define Col4_GPIO_Port GPIOD
#define LCD_Reset_Pin GPIO_PIN_1
#define LCD_Reset_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define LED_RED_COL_Pin GPIO_PIN_5
#define LED_RED_COL_GPIO_Port GPIOA
#define LED_YELLOW_COL_Pin GPIO_PIN_6
#define LED_YELLOW_COL_GPIO_Port GPIOA
#define LED_GREEN_COL_Pin GPIO_PIN_7
#define LED_GREEN_COL_GPIO_Port GPIOA
#define LED_RED_ROW_Pin GPIO_PIN_0
#define LED_RED_ROW_GPIO_Port GPIOB
#define LED_YELLOW_ROW_Pin GPIO_PIN_1
#define LED_YELLOW_ROW_GPIO_Port GPIOB
#define LED_GREEN_ROW_Pin GPIO_PIN_2
#define LED_GREEN_ROW_GPIO_Port GPIOB
#define a1_Pin GPIO_PIN_8
#define a1_GPIO_Port GPIOA
#define b1_Pin GPIO_PIN_9
#define b1_GPIO_Port GPIOA
#define c1_Pin GPIO_PIN_10
#define c1_GPIO_Port GPIOA
#define d1_Pin GPIO_PIN_11
#define d1_GPIO_Port GPIOA
#define e1_Pin GPIO_PIN_12
#define e1_GPIO_Port GPIOA
#define f1_Pin GPIO_PIN_13
#define f1_GPIO_Port GPIOA
#define g1_Pin GPIO_PIN_14
#define g1_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

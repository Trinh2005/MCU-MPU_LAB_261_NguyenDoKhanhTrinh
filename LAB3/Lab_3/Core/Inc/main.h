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
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define B2_Pin GPIO_PIN_1
#define B2_GPIO_Port GPIOA
#define B3_Pin GPIO_PIN_2
#define B3_GPIO_Port GPIOA
#define RED_Pin GPIO_PIN_7
#define RED_GPIO_Port GPIOA
#define SA0_Pin GPIO_PIN_0
#define SA0_GPIO_Port GPIOB
#define SA1_Pin GPIO_PIN_1
#define SA1_GPIO_Port GPIOB
#define SA2_Pin GPIO_PIN_2
#define SA2_GPIO_Port GPIOB
#define SB3_Pin GPIO_PIN_10
#define SB3_GPIO_Port GPIOB
#define SB4_Pin GPIO_PIN_11
#define SB4_GPIO_Port GPIOB
#define SB5_Pin GPIO_PIN_12
#define SB5_GPIO_Port GPIOB
#define SB6_Pin GPIO_PIN_13
#define SB6_GPIO_Port GPIOB
#define EN2_Pin GPIO_PIN_14
#define EN2_GPIO_Port GPIOB
#define EN3_Pin GPIO_PIN_15
#define EN3_GPIO_Port GPIOB
#define YEL_Pin GPIO_PIN_8
#define YEL_GPIO_Port GPIOA
#define GRN_Pin GPIO_PIN_9
#define GRN_GPIO_Port GPIOA
#define RED1_Pin GPIO_PIN_10
#define RED1_GPIO_Port GPIOA
#define YEL1_Pin GPIO_PIN_11
#define YEL1_GPIO_Port GPIOA
#define GRN1_Pin GPIO_PIN_12
#define GRN1_GPIO_Port GPIOA
#define EN0_Pin GPIO_PIN_13
#define EN0_GPIO_Port GPIOA
#define EN1_Pin GPIO_PIN_14
#define EN1_GPIO_Port GPIOA
#define SA3_Pin GPIO_PIN_3
#define SA3_GPIO_Port GPIOB
#define SA4_Pin GPIO_PIN_4
#define SA4_GPIO_Port GPIOB
#define SA5_Pin GPIO_PIN_5
#define SA5_GPIO_Port GPIOB
#define SA6_Pin GPIO_PIN_6
#define SA6_GPIO_Port GPIOB
#define SB0_Pin GPIO_PIN_7
#define SB0_GPIO_Port GPIOB
#define SB1_Pin GPIO_PIN_8
#define SB1_GPIO_Port GPIOB
#define SB2_Pin GPIO_PIN_9
#define SB2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

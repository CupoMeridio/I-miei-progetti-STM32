/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define _0_gradi_Pin GPIO_PIN_5
#define _0_gradi_GPIO_Port GPIOA
#define _0_gradi_EXTI_IRQn EXTI9_5_IRQn
#define _45_gradi_Pin GPIO_PIN_6
#define _45_gradi_GPIO_Port GPIOA
#define _45_gradi_EXTI_IRQn EXTI9_5_IRQn
#define _90_gradi_Pin GPIO_PIN_7
#define _90_gradi_GPIO_Port GPIOA
#define _90_gradi_EXTI_IRQn EXTI9_5_IRQn
#define _135_gradi_Pin GPIO_PIN_8
#define _135_gradi_GPIO_Port GPIOA
#define _135_gradi_EXTI_IRQn EXTI9_5_IRQn
#define _180_gradi_Pin GPIO_PIN_9
#define _180_gradi_GPIO_Port GPIOA
#define _180_gradi_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

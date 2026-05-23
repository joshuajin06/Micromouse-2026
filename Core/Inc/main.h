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
#include "stm32f2xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IR_EMITTER_4_Pin GPIO_PIN_0
#define IR_EMITTER_4_GPIO_Port GPIOC
#define IR_RECEIVER_4_Pin GPIO_PIN_1
#define IR_RECEIVER_4_GPIO_Port GPIOC
#define IR_EMITTER_3_Pin GPIO_PIN_2
#define IR_EMITTER_3_GPIO_Port GPIOC
#define IR_RECEIVER_3_Pin GPIO_PIN_3
#define IR_RECEIVER_3_GPIO_Port GPIOC
#define RENC_1_Pin GPIO_PIN_0
#define RENC_1_GPIO_Port GPIOA
#define RENC_2_Pin GPIO_PIN_1
#define RENC_2_GPIO_Port GPIOA
#define IR_EMITTER_2_Pin GPIO_PIN_6
#define IR_EMITTER_2_GPIO_Port GPIOA
#define IR_RECEIVER_2_Pin GPIO_PIN_7
#define IR_RECEIVER_2_GPIO_Port GPIOA
#define IR_EMITTER_1_Pin GPIO_PIN_4
#define IR_EMITTER_1_GPIO_Port GPIOC
#define IR_RECEIVER_1_Pin GPIO_PIN_5
#define IR_RECEIVER_1_GPIO_Port GPIOC
#define USER_LED_1_Pin GPIO_PIN_6
#define USER_LED_1_GPIO_Port GPIOC
#define USER_LED_2_Pin GPIO_PIN_7
#define USER_LED_2_GPIO_Port GPIOC
#define LENC_1_Pin GPIO_PIN_8
#define LENC_1_GPIO_Port GPIOA
#define LENC_2_Pin GPIO_PIN_9
#define LENC_2_GPIO_Port GPIOA
#define ML_1_Pin GPIO_PIN_6
#define ML_1_GPIO_Port GPIOB
#define ML_2_Pin GPIO_PIN_7
#define ML_2_GPIO_Port GPIOB
#define MR_1_Pin GPIO_PIN_8
#define MR_1_GPIO_Port GPIOB
#define MR_2_Pin GPIO_PIN_9
#define MR_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

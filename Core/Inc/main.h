/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define LoRa_CS_Pin GPIO_PIN_4
#define LoRa_CS_GPIO_Port GPIOA
#define LoRa_RST_Pin GPIO_PIN_0
#define LoRa_RST_GPIO_Port GPIOB
#define LoRa_BUSY_Pin GPIO_PIN_1
#define LoRa_BUSY_GPIO_Port GPIOB
#define LoRa_TXEN_Pin GPIO_PIN_2
#define LoRa_TXEN_GPIO_Port GPIOB
#define NB_WK_Pin GPIO_PIN_13
#define NB_WK_GPIO_Port GPIOB
#define LoRa_RXEN_Pin GPIO_PIN_12
#define LoRa_RXEN_GPIO_Port GPIOA
#define GPS_EN_Pin GPIO_PIN_3
#define GPS_EN_GPIO_Port GPIOB
#define DS3553_CS_Pin GPIO_PIN_5
#define DS3553_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

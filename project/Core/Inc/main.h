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
#define OV7725_VSYNC_Pin GPIO_PIN_3
#define OV7725_VSYNC_GPIO_Port GPIOC
#define OV7725_VSYNC_EXTI_IRQn EXTI3_IRQn
#define OV7725_RRST_Pin GPIO_PIN_2
#define OV7725_RRST_GPIO_Port GPIOA
#define OV7725_OE_Pin GPIO_PIN_3
#define OV7725_OE_GPIO_Port GPIOA
#define VOICE_SCK_Pin GPIO_PIN_5
#define VOICE_SCK_GPIO_Port GPIOA
#define VOICE_MISO_Pin GPIO_PIN_6
#define VOICE_MISO_GPIO_Port GPIOA
#define VOICE_MOSI_Pin GPIO_PIN_7
#define VOICE_MOSI_GPIO_Port GPIOA
#define OV7725_WRST_Pin GPIO_PIN_4
#define OV7725_WRST_GPIO_Port GPIOC
#define OV7725_RCLK_Pin GPIO_PIN_5
#define OV7725_RCLK_GPIO_Port GPIOC
#define VOICE_CS_Pin GPIO_PIN_1
#define VOICE_CS_GPIO_Port GPIOB
#define VOICE_RST_Pin GPIO_PIN_2
#define VOICE_RST_GPIO_Port GPIOB
#define VOICE_IRQ_Pin GPIO_PIN_11
#define VOICE_IRQ_GPIO_Port GPIOF
#define OV7725_DATA0_Pin GPIO_PIN_8
#define OV7725_DATA0_GPIO_Port GPIOD
#define OV7725_DATA1_Pin GPIO_PIN_9
#define OV7725_DATA1_GPIO_Port GPIOD
#define OV7725_DATA2_Pin GPIO_PIN_10
#define OV7725_DATA2_GPIO_Port GPIOD
#define OV7725_DATA3_Pin GPIO_PIN_11
#define OV7725_DATA3_GPIO_Port GPIOD
#define OV7725_DATA4_Pin GPIO_PIN_12
#define OV7725_DATA4_GPIO_Port GPIOD
#define OV7725_DATA5_Pin GPIO_PIN_13
#define OV7725_DATA5_GPIO_Port GPIOD
#define OV7725_DATA6_Pin GPIO_PIN_14
#define OV7725_DATA6_GPIO_Port GPIOD
#define OV7725_DATA7_Pin GPIO_PIN_15
#define OV7725_DATA7_GPIO_Port GPIOD
#define SDIO_C_Pin GPIO_PIN_6
#define SDIO_C_GPIO_Port GPIOC
#define SDIO_D_Pin GPIO_PIN_7
#define SDIO_D_GPIO_Port GPIOC
#define OV7725_WEN_Pin GPIO_PIN_3
#define OV7725_WEN_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

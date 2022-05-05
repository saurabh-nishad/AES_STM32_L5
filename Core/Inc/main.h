/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32l5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdarg.h"
#include <stdio.h>
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
void Display_PlainData(uint8_t *plaintext128, uint32_t datalength);
void Display_key(uint8_t *key128,uint32_t datalength);
void Display_EncryptedData(uint32_t *encryptedtext, uint8_t mode, uint16_t keysize, uint32_t datalength);
void Display_DecryptedData(uint32_t *decryptedtext, uint8_t mode, uint16_t keysize, uint32_t datalength);
void Display_IV(uint8_t *IV128,uint32_t datalength);
void Display_AuthTAG(uint32_t *AuthTAG, uint8_t mode, uint16_t keysize, uint32_t datalength);
void Display_Header(uint8_t *Header128,uint32_t datalength);

void vprint(const char *fmt,va_list argp);
void debug_printf(const char *fmt, ...);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GREEN_LED_Pin GPIO_PIN_12
#define GREEN_LED_GPIO_Port GPIOG
#define USR_BUTTON_Pin GPIO_PIN_13
#define USR_BUTTON_GPIO_Port GPIOC
#define USR_BUTTON_EXTI_IRQn EXTI13_IRQn
#define VCOM_TX_Pin GPIO_PIN_9
#define VCOM_TX_GPIO_Port GPIOA
#define VCOM_RX_Pin GPIO_PIN_10
#define VCOM_RX_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
typedef enum {
	CM_NONE = 0,
	GCM = 1,
	ECB = 2,
	CTR = 3
} ChainingMode ;

typedef enum {
	BM_NONE = 0,
	key_128_Bit = 11,
	key_256_Bit = 12
} BitMode;

typedef enum {
	OM_NONE = 0,
	Encrypt = 21,
	Decrypt = 22
} OperationMode;

typedef enum {
	STATE_RESET = 0,
	USER_INPUT,
	GCM_128_Encrypt = 31,
	GCM_128_Decrypt = 28,
	GCM_256_Encrypt = 24,
	GCM_256_Decrypt = 27,
} OperationCase;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

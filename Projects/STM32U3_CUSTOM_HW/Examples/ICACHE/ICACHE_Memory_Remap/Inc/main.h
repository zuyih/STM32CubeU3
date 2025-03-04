/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "stm32u3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define QUAD_IO_READ_CMD            0xEB
#define QUAD_PAGE_PROG_CMD          0x38
#define SECTOR_ERASE_CMD            0x20
#define READ_STATUS_REG_CMD         0x05
#define READ_STATUS_SIZE            0x01
#define WRITE_STATUS_CFG_REG_CMD    0x01
#define STATUS_CFG_SIZE             0x03
#define WRITE_ENABLE_CMD            0x06

/* Dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ        8
#define DUMMY_CLOCK_CYCLES_READ_QUAD   6

/* Auto-polling values */
#define WRITE_ENABLE_MATCH_VALUE    0x02
#define WRITE_ENABLE_MASK_VALUE     0x02
#define MEMORY_READY_MATCH_VALUE    0x00
#define MEMORY_READY_MASK_VALUE     0x01
#define QUAD_ENABLE_MATCH_VALUE     0x40
#define QUAD_ENABLE_MASK_VALUE      0x40
#define HIGH_PERF_MATCH_VALUE       0x02
#define HIGH_PERF_MASK_VALUE        0x02
#define AUTO_POLLING_INTERVAL       0x10

#define QSPI_PAGE_SIZE              256
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_15
#define LED_GREEN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define IMAGE_SIZE_X          240
#define IMAGE_SIZE_Y          130

#define BUFFER_SIZE           160   /* 40 Cache Lines */

#define SRAM_BANK_ADDR    ((uint32_t)0x60000000)/*External SRAM start address*/

#define SRAM_MEMORY_WIDTH FMC_NORSRAM_MEM_BUS_WIDTH_16/*SRAM Bus width*/

#define SRAM_TIMEOUT      ((uint32_t)0xFFFF)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

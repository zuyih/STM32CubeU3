/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#include "stm32u3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "linked_list.h"
#include "stm32u3xx_nucleo.h"
#include "string.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern DMA_QListTypeDef Queue;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
#define SEGMENT_SIZES_COUNT                            3
#define HIGH_PRIO_INPUT_TAB_SIZE           ((uint32_t) 261)  /* The size of the input data "aInput_HighPrio" */
#define LOW_PRIO_LARGE_INPUT_TAB_SIZE      ((uint32_t) 3567) /* The size of the input data "aInput_LowPrio_Large" */
#define KEY_TAB_SIZE                       ((uint32_t) 261)  /* The size of the key */
#define HASHTimeout                                    0xFF
#define SHA1_DIGEST_LENGTH                             20
#define SHA224_DIGEST_LENGTH                           28
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

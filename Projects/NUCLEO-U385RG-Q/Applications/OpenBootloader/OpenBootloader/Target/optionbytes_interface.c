/**
  ******************************************************************************
  * @file    optionbytes_interface.c
  * @author  MCD Application Team
  * @brief   Contains Option Bytes access functions
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "platform.h"
#include "common_interface.h"

#include "openbl_mem.h"

#include "app_openbootloader.h"
#include "flash_interface.h"
#include "optionbytes_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef OB_Descriptor =
{
  OB_START_ADDRESS,
  OB_END_ADDRESS,
  OB_SIZE,
  OB_AREA,
  OPENBL_OB_Read,
  OPENBL_OB_Write,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Launch the option byte loading.
  * @retval None.
  */
void OPENBL_OB_Launch(void)
{
  /* Set the option start bit */
  HAL_FLASH_OB_Launch();
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_OB_Read(uint32_t address)
{
  return (*(uint8_t *)(address));
}

/**
  * @brief  This function is used to write data in Option bytes.
  * @param  address The address where that data will be written.
  * @param  p_data Pointer to the data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_OB_Write(uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(address);

  /* Unlock the FLASH & Option Bytes Registers access */
  OPENBL_FLASH_OB_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_SR_ERRORS);

  /* Wait for last operation to be completed */
  FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if ((data_length != 0U) && (*p_data != RDP_LEVEL_2))
  {
    /* Write the FLASH option bytes registers */

    /* Write OPTR option bytes */
    if (data_length > 0U)
    {
      FLASH->OPTR = *(uint32_t *)(uint32_t)&p_data[0U];
    }

    /* Write BOOT0R option bytes */
    if (data_length > 4U)
    {
      FLASH->BOOT0R = *(uint32_t *)(uint32_t)&p_data[4U];
    }

    /* Write BOOT01 option bytes */
    if (data_length > 8U)
    {
      FLASH->BOOT1R = *(uint32_t *)(uint32_t)&p_data[8U];
    }

    /* Write WRP1AR option bytes */
    if (data_length > 24U)
    {
      FLASH->WRP1AR = *(uint32_t *)(uint32_t)&p_data[24U];
    }

    /* Write WRP1BR option bytes */
    if (data_length > 28U)
    {
      FLASH->WRP1BR = *(uint32_t *)(uint32_t)&p_data[28U];
    }

    /* Write WRP2AR option bytes */
    if (data_length > 40U)
    {
      FLASH->WRP2AR = *(uint32_t *)(uint32_t)&p_data[40U];
    }

    /* Write WRP2BR option bytes */
    if (data_length > 44U)
    {
      FLASH->WRP2BR = *(uint32_t *)(uint32_t)&p_data[44U];
    }
  }

  SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT);

  /* Wait for last operation to be completed */
  FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  /* Register system reset callback */
  Common_SetPostProcessingCallback(OPENBL_OB_Launch);
}

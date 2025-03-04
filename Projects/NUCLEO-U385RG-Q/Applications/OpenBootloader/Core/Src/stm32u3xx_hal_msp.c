/**
  ******************************************************************************
  * @file         stm32u3xx_hal_msp.c
  * @author  MCD Application Team
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
#include "interfaces_conf.h"

/**
  * @brief  Initialize the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief PCD MSP Initialization
  *        This function configures the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
  GPIO_InitTypeDef gpio_initstruct             = {0U};
  RCC_PeriphCLKInitTypeDef periphclkinitstruct = {0U};

  if (hpcd->Instance == USB_DRD_FS)
  {
    /* Initializes the peripherals clock */
    periphclkinitstruct.PeriphClockSelection  = RCC_PERIPHCLK_USB1;
    periphclkinitstruct.Usb1ClockSelection    = RCC_USB1CLKSOURCE_ICLK;

    if (HAL_RCCEx_PeriphCLKConfig(&periphclkinitstruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**
       *USB GPIO Configuration
       *PA11     ------> USB_DM
       *PA12     ------> USB_DP
       */
    gpio_initstruct.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
    gpio_initstruct.Mode      = GPIO_MODE_AF_PP;
    gpio_initstruct.Pull      = GPIO_NOPULL;
    gpio_initstruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_initstruct.Alternate = GPIO_AF10_USB;
    HAL_GPIO_Init(GPIOA, &gpio_initstruct);

    /* Peripheral clock enable */
    __HAL_RCC_USB1_CLK_ENABLE();

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddUSB();

    /* USB_DRD_FS interrupt Init */
    HAL_NVIC_SetPriority(USB_FS_IRQn, 3U, 0U);
    HAL_NVIC_EnableIRQ(USB_FS_IRQn);
  }
}

/**
  * @brief PCD MSP De-Initialization
  *        This function freeze the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
  if (hpcd->Instance == USB_DRD_FS)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USB1_CLK_DISABLE();

    /**
       *USB GPIO Configuration
       *PA11     ------> USB_DM
       *PA12     ------> USB_DP
       */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

    /* USB_DRD_FS interrupt DeInit */
    HAL_NVIC_DisableIRQ(USB_FS_IRQn);
  }
}

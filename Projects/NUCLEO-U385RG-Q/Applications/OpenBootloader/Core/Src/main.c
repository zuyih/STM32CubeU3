/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "app_openbootloader.h"
#include "interfaces_conf.h"
#include "stm32u3xx_ll_crs.h"
#include "stm32u3xx_ll_bus.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/**
  * @brief  Main program
  * @retval None
  */
int main(void)
{
  /* STM32U3xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Low Level Initialization
  */
  HAL_Init();

  /* Configure the System clock to have a frequency of 48 MHz */
  SystemClock_Config();

  /* Initialize the OpenBootloader */
  OpenBootloader_Init();

  while (true)
  {
    OpenBootloader_ProtocolDetection();
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB3 Prescaler                 = 1
  *            Flash Latency(WS)              = 1
  *            Voltage range                  = 1
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef rcc_clk_init_struct = {0U};
  RCC_OscInitTypeDef rcc_osc_init_struct = {0U};

  /* Enable Epod Booster */
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1);

  HAL_PWREx_EnableEpodBooster();

  /* At reset, the regulator is the LDO, in range 2 : Need to move to range 1 to reach 96 MHz */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Activate MSIS, MSIK as source */
  rcc_osc_init_struct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
  rcc_osc_init_struct.MSISState      = RCC_MSI_ON;
  rcc_osc_init_struct.MSISSource     = RCC_MSI_RC0;
  rcc_osc_init_struct.MSISDiv        = RCC_MSI_DIV2;

  if (HAL_RCC_OscConfig(&rcc_osc_init_struct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select MSIS as system clock source and configure the HCLK, PCLK1, PCLK2 and PCLK3
     clocks dividers */
  rcc_clk_init_struct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
                                        | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);
  rcc_clk_init_struct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSIS;
  rcc_clk_init_struct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  rcc_clk_init_struct.APB1CLKDivider = RCC_HCLK_DIV1;
  rcc_clk_init_struct.APB2CLKDivider = RCC_HCLK_DIV1;
  rcc_clk_init_struct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&rcc_clk_init_struct, FLASH_LATENCY_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable HSI48 oscillator */
  LL_RCC_HSI48_Enable();
}

/**
  * @brief  This function is used to de-initialized the clock.
  * @retval None
  */
void System_DeInit(void)
{
  HAL_RCC_DeInit();

  /* Disable GPIOx */
  LL_GPIO_DeInit(GPIOA);
  LL_GPIO_DeInit(GPIOB);
  LL_GPIO_DeInit(GPIOC);

  /* Disable interfaces clocks */
  USARTx_CLK_DISABLE();
  I2Cx_CLK_DISABLE();
  I3Cx_CLK_DISABLE();
  SPIx_CLK_DISABLE();
  USB_CLK_DISABLE();

  /* Reset interfaces */
  USARTx_DEINIT();
  I2Cx_DEINIT();
  I3Cx_DEINIT();
  SPIx_DEINIT();
  USB_DEINIT();

  /* Disable SysTick interrupt */
  NVIC_DisableIRQ(SysTick_IRQn);

  /* Disable USB interrupt */
  HAL_NVIC_DisableIRQ(USB_FS_IRQn);

  /* Disable SPI interrupt */
  HAL_NVIC_DisableIRQ(SPIx_IRQ);

  /* Disable I3C interrupt */
  HAL_NVIC_DisableIRQ(I3C1_EV_IRQn);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  while (true)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (true)
  {
  }
}
#endif /* USE_FULL_ASSERT */

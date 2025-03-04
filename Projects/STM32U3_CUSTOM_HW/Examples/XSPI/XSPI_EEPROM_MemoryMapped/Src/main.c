/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This example describes how to erase part of the QSPI EEPROM memory,
  *          write data in IT mode and access to QSPI EEPROM memory
  *          in memory-mapped mode to check the data in a forever loop.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

XSPI_HandleTypeDef hxspi1;

/* USER CODE BEGIN PV */
/* Buffer used for transmission */
uint8_t aTxBuffer[] =
" ****Memory-mapped EEPROM QSPI communication****  ****Memory-mapped EEPROM QSPI communication****  ****Memory-mapped EEPROM QSPI communication****  ****Memory-mapped EEPROM QSPI communication****  ****Memory-mapped EEPROM QSPI communication****\
  ****Memory-mapped EEPROM QSPI communication****  ****Memory-mapped EEPROM QSPI communication****  ****Memory-mapped EEPROM QSPI communication****  ****Memory-mapped EEPROM QSPI communication****  ****Memory-mapped EEPROM QSPI communication****";
__IO uint8_t CmdCplt, TxCplt;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_OCTOSPI1_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
static void XSPI_WriteEnable(XSPI_HandleTypeDef *hxspi);
static void XSPI_AutoPollingMemReady(XSPI_HandleTypeDef *hxspi);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  XSPI_RegularCmdTypeDef sCommand = {0};
  XSPI_MemoryMappedTypeDef sMemMappedCfg = {0};

  uint16_t index = 0;
  uint16_t res = 0;
  uint32_t address = 0;
  __IO uint8_t step = 0;
  __IO uint8_t *mem_addr;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_OCTOSPI1_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    switch (step)
    {
    /* Step 0: Enable write operation (WE bit) and Erase Sector related
       to address: address */
    case 0:
      CmdCplt = 0;

      /* Enable write operations -------------------------------------------- */
      XSPI_WriteEnable(&hxspi1);

      /* Erasing Sequence --------------------------------------------------- */
      sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
      sCommand.Instruction        = SECTOR_ERASE_CMD;
      sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
      sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
      sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
      sCommand.AddressMode        = HAL_XSPI_ADDRESS_1_LINE;
      sCommand.AddressWidth       = HAL_XSPI_ADDRESS_24_BITS;
      sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
      sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
      sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
      sCommand.DataMode           = HAL_XSPI_DATA_NONE;
      sCommand.Address            = address;
      sCommand.DummyCycles        = 0;
      sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

      if (HAL_XSPI_Command_IT(&hxspi1, &sCommand) != HAL_OK)
      {
        Error_Handler();
      }

      step++;
      break;

      /* Step 1: Wait end of Write In Progress (WIP bit), enable write operation
      (WE bit) and Write data present at address: aTxBuffer */
    case 1:
      if (CmdCplt != 0)
      {
        CmdCplt = 0;

        /* Configure automatic polling mode to wait for end of erase -------- */
        XSPI_AutoPollingMemReady(&hxspi1);

        /* Enable write operations ------------------------------------------ */
        XSPI_WriteEnable(&hxspi1);

        /* Writing Sequence ------------------------------------------------- */
        sCommand.Instruction = PAGE_PROG_CMD;
        sCommand.DataMode    = HAL_XSPI_DATA_1_LINE;
        sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
        sCommand.DataLength  = BUFFERSIZE;
        sCommand.Address     = address;

        if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          Error_Handler();
        }

        if (HAL_XSPI_Transmit_IT(&hxspi1, aTxBuffer) != HAL_OK)
        {
          Error_Handler();
        }

        step++;
      }
      break;

    /* Step 2:  Wait end of Write In Progress (WIP bit), Switch memory in Mapped Mode,
       read data into memory and check validity */
    case 2:
      if (TxCplt != 0)
      {
        TxCplt = 0;

        /* Configure automatic polling mode to wait for end of write -------- */
        XSPI_AutoPollingMemReady(&hxspi1);

        /* Memory-mapped mode configuration --------------------------------- */
        sCommand.OperationType      = HAL_XSPI_OPTYPE_READ_CFG;
        sCommand.IOSelect           = HAL_XSPI_SELECT_IO_3_0;
        sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
        sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
        sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
        sCommand.Instruction        = QUAD_IO_READ_CMD;
        sCommand.AddressMode        = HAL_XSPI_ADDRESS_1_LINE;
        sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
        sCommand.AddressWidth       = HAL_XSPI_ADDRESS_24_BITS;
        sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
        sCommand.DataMode           = HAL_XSPI_DATA_4_LINES;
        sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
        sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_READ;
        sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;
        sCommand.Address            = 0;

        if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          Error_Handler();
        }

        sCommand.OperationType      = HAL_XSPI_OPTYPE_WRITE_CFG;
        sCommand.DataMode           = HAL_XSPI_DATA_1_LINE;
        sCommand.Instruction        = PAGE_PROG_CMD;
        sCommand.DummyCycles        = 0U;

        if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          Error_Handler();
        }

        sMemMappedCfg.TimeOutActivation  = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;
        sMemMappedCfg.TimeoutPeriodClock = 0x40;

        if (HAL_XSPI_MemoryMapped(&hxspi1, &sMemMappedCfg) != HAL_OK)
        {
          Error_Handler();
        }

        /* Reading Sequence ------------------------------------------------- */
        mem_addr = (uint8_t *)(OCTOSPI1_BASE + address);
        for (index = 0; index < BUFFERSIZE ; index++)
        {
          if (*mem_addr != aTxBuffer[index])
          {
            res++;
          }
          mem_addr++;
        }

        if (res != 0)
        {
          HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
          Error_Handler();
        }
        else
        {
          HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
          HAL_Delay(50);
        }

        address += QSPI_PAGE_SIZE;
        if (address >= QSPI_END_ADDR)
        {
          address = 0;
        }

        /* Abort OctoSPI driver to stop the memory-mapped mode -------------- */
        if (HAL_XSPI_Abort(&hxspi1) != HAL_OK)
        {
          Error_Handler();
        }

        step = 0;
      }
      break;

    default :
      Error_Handler();
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Enable Epod Booster
  */
  if (HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_PWREx_EnableEpodBooster() != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Set Flash latency before increasing MSIS
  */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS|RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  RCC_OscInitStruct.MSIKState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIKSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSIKDiv = RCC_MSI_DIV1;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief OCTOSPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_OCTOSPI1_Init(void)
{

  /* USER CODE BEGIN OCTOSPI1_Init 0 */

  /* USER CODE END OCTOSPI1_Init 0 */

  /* USER CODE BEGIN OCTOSPI1_Init 1 */

  /* USER CODE END OCTOSPI1_Init 1 */
  /* OCTOSPI1 parameter configuration*/
  hxspi1.Instance = OCTOSPI1;
  hxspi1.Init.FifoThresholdByte = 1;
  hxspi1.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
  hxspi1.Init.MemoryType = HAL_XSPI_MEMTYPE_MACRONIX;
  hxspi1.Init.MemorySize = HAL_XSPI_SIZE_32MB;
  hxspi1.Init.ChipSelectHighTimeCycle = 2;
  hxspi1.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
  hxspi1.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
  hxspi1.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
  hxspi1.Init.ClockPrescaler = 1;
  hxspi1.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_HALFCYCLE;
  hxspi1.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
  hxspi1.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_NONE;
  hxspi1.Init.DelayBlockBypass = HAL_XSPI_DELAY_BLOCK_BYPASS;
  hxspi1.Init.MaxTran = 0;
  hxspi1.Init.Refresh = 0;
  if (HAL_XSPI_Init(&hxspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OCTOSPI1_Init 2 */

  /* USER CODE END OCTOSPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED2_Pin|LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED2_Pin LED1_Pin */
  GPIO_InitStruct.Pin = LED2_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Command completed callbacks.
  * @param  hxspi: XSPI handle
  * @retval None
  */
void HAL_XSPI_CmdCpltCallback(XSPI_HandleTypeDef *hxspi)
{
  CmdCplt++;
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hqspi QSPI handle
  * @retval None
  */
void HAL_XSPI_TxCpltCallback(XSPI_HandleTypeDef *hqspi)
{
  TxCplt++;
}


/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hxspi: XSPI handle
  * @retval None
  */
static void XSPI_WriteEnable(XSPI_HandleTypeDef *hxspi)
{
  XSPI_RegularCmdTypeDef  sCommand = {0};
  XSPI_AutoPollingTypeDef s_config = {0};

  /* Enable write operations ------------------------------------------ */
  sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles        = 0;
  sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

  /* Enable write operations */
  sCommand.Instruction        = WRITE_ENABLE_CMD;
  sCommand.DataMode           = HAL_XSPI_DATA_NONE;
  sCommand.AddressMode        = HAL_XSPI_ADDRESS_NONE;

  if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Reconfigure XSPI to automatic polling mode to wait for write enabling */
  sCommand.Instruction        = READ_STATUS_REG_CMD;
  sCommand.DataMode           = HAL_XSPI_DATA_1_LINE;
  sCommand.DataLength         = READ_STATUS_SIZE;

  /* Send the command */
  if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
      Error_Handler();
  }

  s_config.MatchValue         = WRITE_ENABLE_MASK_VALUE;
  s_config.MatchMask          = WRITE_ENABLE_MATCH_VALUE;
  s_config.MatchMode          = HAL_XSPI_MATCH_MODE_AND;
  s_config.IntervalTime       = 0x10;
  s_config.AutomaticStop      = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_AutoPolling(hxspi, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
      Error_Handler();
  }
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hxspi: XSPI handle
  * @retval None
  */
static void XSPI_AutoPollingMemReady(XSPI_HandleTypeDef *hxspi)
{
  XSPI_RegularCmdTypeDef  sCommand = {0};
  XSPI_AutoPollingTypeDef s_config = {0};

  /* Enable write operations ------------------------------------------ */
  sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles        = 0;
  sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

  /* Configure XSPI to automatic polling mode to wait end of write in progress */
  sCommand.Instruction        = READ_STATUS_REG_CMD;
  sCommand.DataMode           = HAL_XSPI_DATA_1_LINE;
  sCommand.DataLength         = READ_STATUS_SIZE;

  /* Send the command */
  if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  s_config.MatchValue         = MEMORY_READY_MATCH_VALUE;
  s_config.MatchMask          = MEMORY_READY_MASK_VALUE;
  s_config.MatchMode          = HAL_XSPI_MATCH_MODE_AND;
  s_config.IntervalTime       = 0x10;
  s_config.AutomaticStop      = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_AutoPolling(hxspi, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

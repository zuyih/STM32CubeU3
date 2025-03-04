/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#define GPIO_NUMBER 16U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

XSPI_HandleTypeDef hxspi1;

/* USER CODE BEGIN PV */
uint8_t *flash_addr;
uint32_t extmem_size;
ICACHE_RegionConfigTypeDef region_config;
__IO uint8_t CmdCplt, TxCplt;

#if defined(__ARMCC_VERSION)||defined(__CC_ARM) 
extern uint32_t Load$$EXTMEM_REGION$$Base;
extern uint32_t Load$$EXTMEM_REGION$$Length;
#elif defined(__ICCARM__)
#pragma section =".extmem_region"
#pragma section =".extmem_region_init"
#elif defined(__GNUC__)
extern uint32_t _extmem_region_init_base;
extern uint32_t _extmem_region_init_length;
#endif

/* Function pointer declaration used to call remapped function */
typedef void (*funcptr)(void);

funcptr IO_Blink_extmem_Remapped;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_OCTOSPI1_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
static void IO_Blink_extmem(void);
static void XSPI_WriteMem(XSPI_HandleTypeDef *hxspi);
static void XSPI_NOR_QuadModeCfg(XSPI_HandleTypeDef *hxspi);
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
  /* STM32U3xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
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

  /* Configure the memory in quad mode -------------------------------------- */
  XSPI_NOR_QuadModeCfg(&hxspi1);

  /* Memory copy from ROM to external SRAM */
#if defined(__ARMCC_VERSION)||defined(__CC_ARM) 
  extmem_size = (uint32_t)(&Load$$EXTMEM_REGION$$Length);
#elif defined(__ICCARM__)
  extmem_size = __section_size(".extmem_region_init");
#elif defined(__GNUC__)
  extmem_size = (uint32_t)((uint8_t *)(&_extmem_region_init_length));
#endif

  XSPI_WriteMem(&hxspi1);

  /* Execute function from external RAM before remap */
  /* Blink the Green Led thrice, before remapping */
  IO_Blink_extmem();
  IO_Blink_extmem();
  IO_Blink_extmem();

  /* Remap SRAM memory */
  region_config.TrafficRoute    = ICACHE_MASTER2_PORT;
  region_config.OutputBurstType = ICACHE_OUTPUT_BURST_INCR;
  region_config.Size            = ICACHE_REGIONSIZE_2MB;
  region_config.BaseAddress     = 0x10000000;
  region_config.RemapAddress    = 0x90000000;

  /* Disable Instruction cache */
  HAL_ICACHE_Disable();

  /* Enable the remapped region */
  if(HAL_ICACHE_EnableRemapRegion(ICACHE_REGION_0, &region_config) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the Instruction and Data Cache */
  HAL_ICACHE_Enable();

   /* Set remap Blink function pointer */
  IO_Blink_extmem_Remapped = (funcptr)((uintptr_t)IO_Blink_extmem - 0x90000000 + 0x10000000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    IO_Blink_extmem_Remapped();
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
  HAL_GPIO_WritePin(GPIOB, LED_RED_Pin|LED_GREEN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED_RED_Pin LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin|LED_GREEN_Pin;
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
* @brief  This function configure the memory in Quad mode.
* @param  hxspi: XSPI handle
* @retval None
*/
static void XSPI_NOR_QuadModeCfg(XSPI_HandleTypeDef *hxspi)
{
  uint8_t reg[3] = {0};
  XSPI_RegularCmdTypeDef  sCommand = {0};
  XSPI_AutoPollingTypeDef sConfig  = {0};

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

  if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.MatchMask           = WRITE_ENABLE_MASK_VALUE;
  sConfig.MatchValue          = WRITE_ENABLE_MATCH_VALUE;
  sConfig.MatchMode           = HAL_XSPI_MATCH_MODE_AND;
  sConfig.AutomaticStop       = HAL_XSPI_AUTOMATIC_STOP_ENABLE;
  sConfig.IntervalTime        = 0x10;

  if (HAL_XSPI_AutoPolling(hxspi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Write Status register: with Quad I/O SPI protocol and High Perf (up to 80MHz) */
  sCommand.Instruction         = WRITE_STATUS_CFG_REG_CMD;
  sCommand.DataLength          = STATUS_CFG_SIZE;
  sCommand.Address             = 0;
  reg[0]                       = QUAD_ENABLE_MATCH_VALUE;
  reg[2]                       = HIGH_PERF_MATCH_VALUE;

  if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_XSPI_Transmit(hxspi, &reg[0], HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Reconfigure XSPI to automatic polling mode to wait for Quad enabling */
  sCommand.Instruction        = READ_STATUS_REG_CMD;
  sCommand.DataMode           = HAL_XSPI_DATA_1_LINE;
  sCommand.DataLength         = READ_STATUS_SIZE;

  if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.MatchMask           = QUAD_ENABLE_MASK_VALUE;
  sConfig.MatchValue          = QUAD_ENABLE_MATCH_VALUE;

  if (HAL_XSPI_AutoPolling(hxspi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}


static void XSPI_WriteMem(XSPI_HandleTypeDef *hxspi)
{
  XSPI_RegularCmdTypeDef sCommand = {0};
  XSPI_MemoryMappedTypeDef sMemMappedCfg = {0};

  __IO uint32_t qspi_addr = 0;
  uint8_t *flash_addr = NULL;
  __IO uint8_t step = 0;
  uint32_t max_size, size = 0;

#if defined(__ARMCC_VERSION)||defined(__CC_ARM) 
  max_size = (uint32_t)(&Load$$EXTMEM_REGION$$Length);
#elif defined(__ICCARM__)
  max_size = __section_size(".extmem_region_init");
#elif defined(__GNUC__)
  max_size = (uint32_t)((uint8_t *)(&_extmem_region_init_length));
#endif

  while(step < 4)
  {
    switch(step)
    {
      /* Step 0: Enable write operation (WE bit) and Erase Sector related
         to address: qspi_addr */
      case 0:
        CmdCplt = 0;
        TxCplt = 0;

        /* Enable write operations ------------------------------------------ */
        XSPI_WriteEnable(hxspi);

        /* Erasing Sequence ------------------------------------------------- */
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
        sCommand.Address            = qspi_addr;
        sCommand.DummyCycles        = 0;
        sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

        if (HAL_XSPI_Command_IT(hxspi, &sCommand) != HAL_OK)
        {
          Error_Handler();
        }

#if defined(__ARMCC_VERSION)||defined(__CC_ARM) 
        flash_addr = (uint8_t *)(&Load$$EXTMEM_REGION$$Base);
#elif defined(__ICCARM__)
        flash_addr = (uint8_t *)(__section_begin(".extmem_region_init"));
#elif defined(__GNUC__)
        flash_addr = (uint8_t *)(&_extmem_region_init_base);
#endif

        /* Copy only one page if the section is bigger */
        if (max_size > QSPI_PAGE_SIZE)
        {
          size = QSPI_PAGE_SIZE;
        }
        else
        {
          size = max_size;
        }

        step++;
        break;

      /* Step 1: Wait end of Write In Progress (WIP bit), enable write operation
         (WE bit) and Write data present at address: flash_addr */
      case 1:
        if(CmdCplt != 0)
        {
          CmdCplt = 0;

          /* Configure automatic polling mode to wait for end of erase ------ */
          XSPI_AutoPollingMemReady(hxspi);

          /* Enable write operations ---------------------------------------- */
          XSPI_WriteEnable(hxspi);

          /* Writing Sequence ----------------------------------------------- */
          sCommand.Instruction = QUAD_PAGE_PROG_CMD;
          sCommand.DataMode    = HAL_XSPI_DATA_4_LINES;
          sCommand.AddressMode = HAL_XSPI_ADDRESS_4_LINES;
          sCommand.DataLength  = size;
          sCommand.Address     = qspi_addr;

          if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
          {
            Error_Handler();
          }

          if (HAL_XSPI_Transmit_IT(hxspi, flash_addr) != HAL_OK)
          {
            Error_Handler();
          }

          step++;
        }
        break;

      /* Step 2: Check if another page is needed to flash data */
      case 2:
        if(TxCplt != 0)
        {
          TxCplt = 0;

          qspi_addr += size;
          flash_addr += size;

          /* Check if a new page writing is needed */
          if (qspi_addr < max_size)
          {
            /* Update the remaining size if it is less than the page size */
            if ((qspi_addr + size) > max_size)
            {
              size = (max_size - qspi_addr);
            }
            step = 1;
          }
          else
          {
            /* Configure automatic polling mode to wait for end of erase ---- */
            XSPI_AutoPollingMemReady(hxspi);

            step++;
          }
        }
        break;

      /* Step 3: Switch memory in Mapped Mode */
      case 3:
        /* Memory-mapped mode configuration --------------------------------- */
        sCommand.OperationType      = HAL_XSPI_OPTYPE_READ_CFG;
        sCommand.IOSelect           = HAL_XSPI_SELECT_IO_3_0;
        sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
        sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
        sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
        sCommand.Instruction        = QUAD_IO_READ_CMD;
        sCommand.AddressMode        = HAL_XSPI_ADDRESS_4_LINES;
        sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
        sCommand.AddressWidth       = HAL_XSPI_ADDRESS_24_BITS;
        sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
        sCommand.DataMode           = HAL_XSPI_DATA_4_LINES;
        sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
        sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_READ_QUAD;
        sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;
        sCommand.Address            = 0;

        if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          Error_Handler();
        }

        sCommand.OperationType      = HAL_XSPI_OPTYPE_WRITE_CFG;
        sCommand.Instruction        = QUAD_PAGE_PROG_CMD;
        sCommand.DummyCycles        = 0U;

        if (HAL_XSPI_Command(hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          Error_Handler();
        }

        sMemMappedCfg.TimeOutActivation  = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;
        sMemMappedCfg.TimeoutPeriodClock = 0x40;

        if (HAL_XSPI_MemoryMapped(hxspi, &sMemMappedCfg) != HAL_OK)
        {
          Error_Handler();
        }
        step++;
        break;
    }
  }
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

/**
  * @brief  Toggle led function in external SRAM executed after remap
  * @param  None
  * @retval None
  */
#if defined(__ICCARM__)
__root static void IO_Blink_extmem(void) @ ".extmem_region"

#elif defined(__ARMCC_VERSION)||defined(__CC_ARM) || defined(__GNUC__)
 static void __attribute__((section(".extmem_region"), noinline)) IO_Blink_extmem(void)
#endif
{
  /* Reset pin */
  LED_GREEN_GPIO_Port->BSRR = (((uint32_t)(LED_GREEN_Pin)) << GPIO_NUMBER);
  /* Delay */
  for(uint32_t i=0;i< 0x08FFFFU;i++ )
  {
    __asm("nop");
  }

  /* Set pin */
  LED_GREEN_GPIO_Port->BSRR = (LED_GREEN_Pin);
  /* Delay */
  for(uint32_t i=0;i< 0x08FFFFU;i++ )
  {
    __asm("nop");
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
    /* Turn LED Red on */
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

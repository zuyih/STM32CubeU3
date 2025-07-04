/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/CORTEX/CORTEX_MPU/Src/main.c
  * @author  MCD Application Team
  * @brief   This example presents the MPU features through
  *          the STM32U3xx CORTEX LL API.
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

/* Address of the privileged read-only area */
#define ARRAY_ADDRESS_START                      (0x20002000UL)
/* Size of privileged read-only area */
#define ARRAY_SIZE                               (0x00000100UL)
/* Attribute of privileged read-only area */
#define ARRAY_ATTRIBUTE_NUMBER                   LL_MPU_ATTRIBUTES_NUMBER3
#define REGION_NUMBER                            LL_MPU_REGION_NUMBER3
#define EXAMPLE_RAM_ADDRESS_START                (0x30000000UL)
#define EXAMPLE_RAM_SIZE                         SRAM1_SIZE
#define EXAMPLE_PERIPH_ADDRESS_START             (0x50000000)
#define EXAMPLE_PERIPH_SIZE                      (0x1F400000UL)
#define EXAMPLE_FLASH_ADDRESS_START              (0x0C000000)
#define EXAMPLE_FLASH_SIZE                       FLASH_SIZE
#define EXAMPLE_RAM_REGION_NUMBER                LL_MPU_REGION_NUMBER0
#define EXAMPLE_FLASH_REGION_NUMBER              LL_MPU_REGION_NUMBER1
#define EXAMPLE_PERIPH_REGION_NUMBER             LL_MPU_REGION_NUMBER2
#define EXAMPLE_RAM_ATTRIBUTE_NUMBER             LL_MPU_ATTRIBUTES_NUMBER0
#define EXAMPLE_FLASH_ATTRIBUTE_NUMBER           LL_MPU_ATTRIBUTES_NUMBER1
#define EXAMPLE_PERIPH_ATTRIBUTE_NUMBER          LL_MPU_ATTRIBUTES_NUMBER2
#define portMPU_REGION_READ_WRITE                LL_MPU_REGION_ALL_RW
#define portMPU_REGION_PRIVILEGED_READ_ONLY      LL_MPU_REGION_PRIV_RO
#define portMPU_REGION_READ_ONLY                 LL_MPU_REGION_PRIV_RO_URO
#define portMPU_REGION_PRIVILEGED_READ_WRITE     LL_MPU_REGION_PRIV_RW

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

__IO uint8_t ubButtonPress = 0;

#if defined ( __ICCARM__ )
#pragma location=ARRAY_ADDRESS_START
__no_init __IO uint8_t PrivilegedReadOnlyArray[ARRAY_SIZE];
#else
__IO uint8_t PrivilegedReadOnlyArray[ARRAY_SIZE] __attribute__((section(".ROarraySection")));
#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */

void     Configure_MPU(void);
void     MPU_AccessPermConfig(void);
void LED_On(void);
void LED_Off(void);
void WaitForUserButtonPress(void);

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* Set MPU regions */
  Configure_MPU();
  MPU_AccessPermConfig();

  /* Read from PrivilegedReadOnlyArray. This will not generate error */
  (void)PrivilegedReadOnlyArray[ARRAY_SIZE - 1];

  /* Toggle quickly LD2 while waiting for User-button press */
  WaitForUserButtonPress();

  /* Write into PrivilegedReadOnlyArray. This will generate memory fault error */
  PrivilegedReadOnlyArray[ARRAY_SIZE - 1] = 'e';

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  /** Enable Epod Booster
  */
  LL_AHB1_GRP2_EnableClock(LL_AHB1_GRP2_PERIPH_PWR);
  LL_RCC_SetEPODBoosterClkSource(LL_RCC_EPODBOOSTCLKSRCE_MSIS);
  LL_RCC_SetEPODBoosterClkPrescaler(LL_RCC_EPODBOOSTCLKPRESCAL_1);
  LL_PWR_EnableEPODBooster();
  while(LL_PWR_IsActiveFlag_BOOST() != 1U)
  {
  }

  /** Configure the main internal regulator output voltage
  */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while(LL_PWR_GetRegulCurrentVOS() != LL_PWR_REGU_VOLTAGE_SCALE1)
  {
  }

  /** Set Flash latency before switching to MSIS
  */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /** Activate MSIS as source
  */
  LL_RCC_MSIS_SetClockSource(LL_RCC_MSIS_CLOCK_SOURCE_RC0);
  LL_RCC_MSIS_SetClockDivision(LL_RCC_MSIS_CLOCK_SOURCE_RC_DIV_1);
  LL_RCC_MSI_SetMSIxClockRange();
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSIS);

  /** Wait till System clock is ready
  */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_MSIS)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_HCLK_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_HCLK_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_HCLK_DIV_1);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_HCLK_DIV_1);

  LL_Init1msTick(96000000);

  LL_SetSystemCoreClock(96000000);
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
  LL_ICACHE_SetMode(LL_ICACHE_1WAY);
  LL_ICACHE_Enable();
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);

  /**/
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTC, LL_EXTI_CONFIG_LINE13);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_13;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_PULL_DOWN);

  /**/
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_MODE_INPUT);

  /**/
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(EXTI13_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


/**
  * @brief  Configures the main MPU regions.
  * @param  None
  * @retval None
  */
void Configure_MPU(void)
{
  /* Disable MPU */
  LL_MPU_Disable();

  /* Configure RAM region as Region Number 0, 192KB of size and R/W region */
  /* Configure MPU attribute index */
  LL_MPU_ConfigAttributes(EXAMPLE_RAM_ATTRIBUTE_NUMBER, LL_MPU_NOT_CACHEABLE);

  /* Enable MPU region */
  LL_MPU_EnableRegion(EXAMPLE_RAM_REGION_NUMBER);

  LL_MPU_ConfigRegion(EXAMPLE_RAM_REGION_NUMBER, portMPU_REGION_READ_WRITE | LL_MPU_ACCESS_NOT_SHAREABLE |
                      LL_MPU_INSTRUCTION_ACCESS_ENABLE, EXAMPLE_RAM_ATTRIBUTE_NUMBER,EXAMPLE_RAM_ADDRESS_START,
                      (EXAMPLE_RAM_ADDRESS_START + EXAMPLE_RAM_SIZE - 1));

  /* Configure FLASH region as REGION Number 1, 1MB of size and R/W region */
  /* Configure MPU attribute index */
  LL_MPU_ConfigAttributes(EXAMPLE_FLASH_ATTRIBUTE_NUMBER, LL_MPU_NOT_CACHEABLE);

  /* Enable MPU region */
  LL_MPU_EnableRegion(EXAMPLE_FLASH_REGION_NUMBER);

  LL_MPU_ConfigRegion(EXAMPLE_FLASH_REGION_NUMBER, portMPU_REGION_READ_WRITE | LL_MPU_ACCESS_NOT_SHAREABLE |
                      LL_MPU_INSTRUCTION_ACCESS_ENABLE, EXAMPLE_FLASH_ATTRIBUTE_NUMBER, EXAMPLE_FLASH_ADDRESS_START,
                      (EXAMPLE_FLASH_ADDRESS_START + EXAMPLE_FLASH_SIZE - 1));

  /* Configure Peripheral region as REGION Number 2, 512MB of size, R/W and Execute
  Never region */
  /* Configure MPU attribute index */
  LL_MPU_ConfigAttributes(EXAMPLE_PERIPH_ATTRIBUTE_NUMBER, LL_MPU_NOT_CACHEABLE);

  /* Enable MPU region */
  LL_MPU_EnableRegion(EXAMPLE_PERIPH_REGION_NUMBER);

  LL_MPU_ConfigRegion(EXAMPLE_PERIPH_REGION_NUMBER, portMPU_REGION_READ_WRITE | LL_MPU_ACCESS_NOT_SHAREABLE |
                      LL_MPU_INSTRUCTION_ACCESS_ENABLE, EXAMPLE_PERIPH_ATTRIBUTE_NUMBER, EXAMPLE_PERIPH_ADDRESS_START,
                      (EXAMPLE_PERIPH_ADDRESS_START + EXAMPLE_PERIPH_SIZE - 1));

  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);
}

/**
  * @brief  This function configures the access right using Cortex-M33 MPU regions.
  * @param  None
  * @retval None
  */
void MPU_AccessPermConfig(void)
{
  /* Configure region for PrivilegedReadOnlyArray as REGION Number 3, 32byte and R
     only in privileged mode */
  /* Disable MPU */
  LL_MPU_Disable();

  /* Configure MPU attribute index */
  LL_MPU_ConfigAttributes(ARRAY_ATTRIBUTE_NUMBER, LL_MPU_NOT_CACHEABLE);

  /* Enable MPU region */
  LL_MPU_EnableRegion(REGION_NUMBER);

  LL_MPU_ConfigRegion(REGION_NUMBER, portMPU_REGION_PRIVILEGED_READ_ONLY | LL_MPU_ACCESS_NOT_SHAREABLE |
                      LL_MPU_INSTRUCTION_ACCESS_ENABLE, ARRAY_ATTRIBUTE_NUMBER, ARRAY_ADDRESS_START,
                      (ARRAY_ADDRESS_START + ARRAY_SIZE - 1));

  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);
}


/**
  * @brief  Turn on LD2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LD2 on */
  LL_GPIO_SetOutputPin(LD2_GPIO_Port, LD2_Pin);
}

/**
  * @brief  Turn off LD2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LD2 off */
  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);
}

/**
  * @brief  Wait for USER push-button press and toggle LD2.
  * @param  None
  * @retval None
  */
/*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Turn LD2 off */
  LED_Off();
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage User button IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update USER push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
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

  /* Infinite loop */
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

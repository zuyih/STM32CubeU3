/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/CRS/CRS_Synchronization_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure Clock Recovery System in
  *          IT mode through the STM32U3xx CRS LL API.
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

#define CRS_STATUS_INIT       0
#define CRS_STATUS_SYNCOK     1
#define CRS_STATUS_SYNCWARN   2
#define CRS_STATUS_SYNCMISS   3
#define CRS_STATUS_SYNCERR    4

/* Oscillator time-out values */
#define LSE_TIMEOUT_VALUE     ((uint32_t)5000)  /* 5 s */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO uint8_t ubButtonPress = 0;

/* Variable to save SYNC status*/
__IO uint32_t CRSSyncStatus = CRS_STATUS_INIT;

/* Variable used for Timeout management */
#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0;
#endif /* USE_TIMEOUT */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */


void     Enable_Sync_Reference_Clock(void);
void     Enable_CRS_Clocks(void);
void     Configure_CRS(uint32_t SyncSource);
void     MCO_ConfigGPIO(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
void     WaitForUserButtonPress(void);

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

  uint32_t felim = 0;

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

  /* Enable the reference clock used for synchronization (LSE) */
  Enable_Sync_Reference_Clock();

  /* Enable HSI48 and CRS */
  Enable_CRS_Clocks();

  /* For monitoring output HSI48 MCO pin(PA8) */
  MCO_ConfigGPIO();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* Configure CRS with LSE parameters                                */
  /* but set the source to external via GPIO (which is not connected) */
  Configure_CRS(LL_CRS_SYNC_SOURCE_GPIO);

  /* Simulate a SYNC EVENT which will raise a SYNC_MISS EVENT */
  /* because no external sync clock is connected to GPIO      */
  LL_CRS_GenerateEvent_SWSYNC();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    switch (CRSSyncStatus)
    {
      case CRS_STATUS_SYNCOK:
        /* Power on LD2 */
        LED_On();
        /* For visual comfort on LED2 apply delay */
        LL_mDelay(500);
        /* Enable CRS SYNCWARN interrupt (in case it goes out of sync) */
        LL_CRS_EnableIT_SYNCWARN();
        break;

      case CRS_STATUS_SYNCMISS:
        /* Power off LD2 */
        LED_Off();
        /* Perform a new configuration to LSE when SYNC MISS is detected. */
        /* For visual comfort on LD2 apply delay before new configuration */
        LL_mDelay(1000);

        /* Change source to LSE and restart synchronization if necessary */
        if (LL_CRS_GetSyncSignalSource() != LL_CRS_SYNC_SOURCE_LSE)
        {
          /* CRS configuration can be changed only if frequency error is disabled */
          LL_CRS_DisableFreqErrorCounter();

          /* Change CRS SYNC signal source to LSE */
          LL_CRS_SetSyncSignalSource(LL_CRS_SYNC_SOURCE_LSE);

          /* Enable Frequency error counter */
          LL_CRS_EnableFreqErrorCounter();
        }
        else
        {
          /* Enable RCC_CRS SYNCOK interrupt */
          LL_CRS_EnableIT_SYNCOK();
        }
        break;

      case CRS_STATUS_SYNCWARN:
        /* Toggle LD2 when SYNC WARN is detected. */
        LED_On();
        LL_mDelay(100);
        LED_Off();
        LL_mDelay(100);
        LED_On();
        LL_mDelay(100);
        LED_Off();
        LL_mDelay(100);
        LED_On();
        LL_mDelay(100);
        LED_Off();
        LL_mDelay(100);
        /* Increase tolerance (FELIM value) until no more warning */
        LL_CRS_DisableFreqErrorCounter();
        felim = LL_CRS_GetFreqErrorLimit();
        LL_CRS_SetFreqErrorLimit(felim + 1);
        LL_CRS_EnableFreqErrorCounter();
        /* Enable CRS SYNCWARN interrupt (to allow resync) */
        LL_CRS_EnableIT_SYNCWARN();
        /* Enable CRS SYNCOK interrupt (if it was in SYNCOK but then in WARN again) */
        LL_CRS_EnableIT_SYNCOK();
        break;

      case CRS_STATUS_SYNCERR:
        /* Power off LD2 */
        LED_Off();
        break;

      default:
        break;
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

  /* Enable Epod Booster */
  LL_AHB1_GRP2_EnableClock(LL_AHB1_GRP2_PERIPH_PWR);
  LL_RCC_SetEPODBoosterClkSource(LL_RCC_EPODBOOSTCLKSRCE_MSIS);
  LL_PWR_EnableEPODBooster();
  while(LL_PWR_IsActiveFlag_BOOST() != 1)
  {
  }

  /* Activate MSIS as source */
  LL_RCC_MSIS_SetClockSource(LL_RCC_MSIS_CLOCK_SOURCE_RC0);
  LL_RCC_MSIS_SetClockDivision(LL_RCC_MSIS_CLOCK_SOURCE_RC_DIV_1);
  LL_RCC_MSI_SetMSIxClockRange();
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSIS);

   /* Wait till System clock is ready */
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
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
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
  * @brief  This function enables the reference clock used for sync (LSE)
  * @param  None
  * @retval None
  */
void Enable_Sync_Reference_Clock(void)
{
  /* To enable LSE, before it is necessary to:
     - Enable the power clock
     - Reset the Back up Domain */
  LL_AHB1_GRP2_EnableClock(LL_AHB1_GRP2_PERIPH_PWR);
  LL_PWR_EnableBkUpAccess();

  /* Enable LSE only if disabled.*/
  if (LL_RCC_LSE_IsReady() == 0)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_LSE_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_LSE_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* LSE activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }
#endif /* USE_TIMEOUT */
    }
  }
  if(LL_RCC_LSE_IsPropagationReady() != 1)
  {
    /* Enable LSE propagation to System clock */
    LL_RCC_LSE_EnablePropagation();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */

    while (LL_RCC_LSE_IsPropagationReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
      {
        timeout --;
      }
      if (timeout == 0)
      {
        return;
      }
#endif /* USE_TIMEOUT */
    }
  }
}

/**
  * @brief  This function enables CRS and HSI48 clocks
  * @param  None
  * @retval None
  */
void Enable_CRS_Clocks(void)
{
  /* CRS initialization (enable HSI48 oscillator and enable CRS clock) */

  /* Enable CRS clock*/
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_CRS);


  /* Enable HSI48 only if disabled.*/
  if (LL_RCC_HSI48_IsReady() == 0)
  {
    LL_RCC_HSI48_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_HSI48_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* HSI48 activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }
#endif /* USE_TIMEOUT */
    }
  }
}

/**
  * @brief  This function configures CRS
  * @param  None
  * @retval None
  */
void Configure_CRS(uint32_t SyncSource)
{
  /* Enable and set RCC_CRS interrupt*/
  NVIC_SetPriority(CRS_IRQn, 0x03);
  NVIC_EnableIRQ(CRS_IRQn);

  /* HSI48 Synchronization:
     - HSI trimming value set to 0x00 to see the automatic calibration performed by CRS
     - Reload value calculated like this F(LSE)=32.768kHz then Reload=(f(Target)/f(LSE))-1= 0x5B7
     - Felim value calculated like this FELIM = (fTARGET / fSYNC) * STEP[%] / 100% / 2 with STEP=0.14% then FELIM = 2
  */
  LL_CRS_ConfigSynchronization(0x00,
                               2,
                               __LL_CRS_CALC_CALCULATE_RELOADVALUE(HSI48_VALUE, LSE_VALUE),
                               LL_CRS_SYNC_DIV_1 | SyncSource | LL_CRS_SYNC_POLARITY_FALLING
                              );

  /* Enable Automatic trimming */
  LL_CRS_EnableAutoTrimming();

  /* Enable Frequency error counter */
  LL_CRS_EnableFreqErrorCounter();

  /* Enable RCC_CRS interrupts */
  /* Each interruption can be enabled independently in using following functions:
  - LL_CRS_EnableIT_SYNCOK();
  - LL_CRS_EnableIT_SYNCWARN();
  - LL_CRS_EnableIT_ERR();
  - LL_CRS_EnableIT_ESYNC();
  */
  LL_CRS_WriteReg(CRS, CR, LL_CRS_ReadReg(CRS, CR) | LL_CRS_CR_SYNCOKIE | LL_CRS_CR_SYNCWARNIE | LL_CRS_CR_ERRIE | LL_CRS_CR_ESYNCIE);
}

/**
  * @brief  Configure MCO pin (PA8).
  * @param  None
  * @retval None
  */
void MCO_ConfigGPIO(void)
{
  /* MCO Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /* Configure the MCO pin in alternate function mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_8, LL_GPIO_AF_0);

  /* Select MCO clock source and prescaler */
  LL_RCC_SetSourceClockMCO(LL_RCC_MCOSOURCE_HSI48);
  LL_RCC_SetClockPrescalerMCO(LL_RCC_MCO_DIV_1);
}

/**
  * @brief  Turn-on LD2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LD2 on */
  LL_GPIO_SetOutputPin(LD2_GPIO_Port, LD2_Pin);

}

/**
  * @brief  Turn-off LD2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LD2 off */
  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);
}

/**
  * @brief  Set LD2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    LL_mDelay(Period);
  }
}

/**
  * @brief  Wait for User push-button press to start transfer.
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
  /* Ensure that LD2 is turned Off */
  LED_Off();
}


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage User button callback interruption
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}

/**
  * @brief  Handle CRS interruption
  * @param  none
  * @retval none
  */
void CRS_Handler_Callback(void)
{
  /* Check CRS SYNCOK flag  */
  if (LL_CRS_IsActiveFlag_SYNCOK() && LL_CRS_IsEnabledIT_SYNCOK())
  {
    CRSSyncStatus = CRS_STATUS_SYNCOK;

    /* Disable CRS SYNCOK interrupt since synchronization succeeded */
    LL_CRS_DisableIT_SYNCOK();

    /* Clear CRS SYNC event OK bit */
    LL_CRS_ClearFlag_SYNCOK();
  }

  /* Check CRS SYNCWARN flag  */
  if (LL_CRS_IsActiveFlag_SYNCWARN() && LL_CRS_IsEnabledIT_SYNCWARN())
  {
    CRSSyncStatus = CRS_STATUS_SYNCWARN;

    /* Disable temporary CRS SYNCWARN interrupt to let the main loop to be execute */
    LL_CRS_DisableIT_SYNCWARN();

    /* Clear CRS SYNCWARN bit */
    LL_CRS_ClearFlag_SYNCWARN();
  }

  /* Check CRS SYNCERR flag  */
  if (LL_CRS_IsActiveFlag_SYNCERR() && LL_CRS_IsEnabledIT_ERR())
  {
    CRSSyncStatus = CRS_STATUS_SYNCERR;

    /* Clear CRS Error bit */
    LL_CRS_ClearFlag_ERR();
  }

  /* Check CRS SYNC Missed flag  */
  if (LL_CRS_IsActiveFlag_SYNCMISS() && LL_CRS_IsEnabledIT_ERR())
  {
    CRSSyncStatus = CRS_STATUS_SYNCMISS;

    /* Clear CRS SYNC Missed bit */
    LL_CRS_ClearFlag_ERR();
  }

  /* Check CRS Expected SYNC flag  */
  if (LL_CRS_IsActiveFlag_ESYNC() && LL_CRS_IsEnabledIT_ESYNC())
  {
    /* frequency error counter reached a zero value */
    LL_CRS_ClearFlag_ESYNC();
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

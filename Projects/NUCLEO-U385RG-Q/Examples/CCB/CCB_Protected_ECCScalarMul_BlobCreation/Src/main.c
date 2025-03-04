/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "stm32u3xx_nucleo.h"
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
CCB_HandleTypeDef hccb;

/* USER CODE BEGIN PV */
CCB_WrappingKeyTypeDef wrappedKeyConf;
CCB_ECCMulCurveParamTypeDef ECCparam;
CCB_ECCMulKeyBlobTypeDef ECCKeyBlob;
CCB_ECCMulPointTypeDef Point_In;
CCB_ECCMulPointTypeDef Point_Out;

/* Input buffers */
uint32_t Symetric_AESKey256[8]           = {0xdecafe00, 0xdecafe01, 0xdecafe02, 0xdecafe03, 0xdecafe04, 0xdecafe05, 0xdecafe06, 0xdecafe07};

uint32_t AESIV[4]                        = {0x00010203 , 0x04050607 , 0x08090A0B , 0x0C0D0E0F};

uint32_t Wrapped_key_Buffer[8]           = {0x1ff4d7fb, 0xbb515f14, 0x676df6b4, 0xcb58c0c8, 0x2b48e65e, 0xf9bfcf32, 0xd9c0a66c, 0xeec362d7};

uint8_t eccMul192_Sclr_k_P[24]           = {0xD5, 0x6E, 0x0A, 0x82, 0xAF, 0x91, 0xEE, 0xDD, 0x62, 0x82, 0x00, 0x3B, 0x02, 0x55, 0x38, 0x35, 0x1F, 0x9B, 0x90, 0xB4, 0x00, 0x81, 0x2A, 0x43};

const uint32_t eccMul192_n_length        = 24;

const uint32_t eccMul192_p_length        = 24;

const uint32_t eccMul192_a_sign          = 0x00000001;

const uint8_t eccMul192_a_abs[24]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};

const uint8_t eccMul192_b[24]            = {0x64, 0x21, 0x05, 0x19, 0xE5, 0x9C, 0x80, 0xE7, 0x0F, 0xA7, 0xE9, 0xAB, 0x72, 0x24, 0x30, 0x49, 0xFE, 0xB8, 0xDE, 0xEC, 0xC1, 0x46, 0xB9, 0xB1};

const uint8_t eccMul192_p[24]            = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const uint8_t eccMul192_n[24]            = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0xDE, 0xF8, 0x36, 0x14, 0x6B, 0xC9, 0xB1, 0xB4, 0xD2, 0x28, 0x31};

uint8_t eccMul192_Custumized_x[24]       = {0xEA, 0x31, 0x34, 0xD4, 0xCC, 0x80, 0xF0, 0xEA, 0xAB, 0xEB, 0x1B, 0x08, 0x70, 0x12, 0x52, 0x26, 0x36, 0x0F, 0x08, 0xD7, 0xA9, 0x4B, 0xF6, 0x19};

uint8_t eccMul192_Custumized_y[24]       = {0x49, 0x7B, 0x27, 0x0A, 0x26, 0xEE, 0xC8, 0xBB, 0x6E, 0x58, 0x40, 0x9D, 0x8E, 0x8A, 0xDD, 0x09, 0x03, 0xD3, 0x30, 0x1D, 0x04, 0x0D, 0x3A, 0x03};

/* Output buffers */
uint32_t SclrK_encrypted_192[8]          = {0};
uint32_t IV_Buffer[4]                    = {0};
uint32_t TAG_Buffer[4]                   = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_CCB_Init(void);
/* USER CODE BEGIN PFP */

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  if (BSP_ERROR_NONE != BSP_LED_Init(LED_GREEN))
  {
    Error_Handler();
  }
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_CCB_Init();
  /* USER CODE BEGIN 2 */

  /* Configure Wrapped Key */
  wrappedKeyConf.WrappingKeyType         = HAL_CCB_USER_KEY_WRAPPED;
  wrappedKeyConf.pInitVect               = AESIV;
  wrappedKeyConf.AES_Algorithm           = CCB_AES_CBC;
  wrappedKeyConf.pUserWrappedWrappingKey = Wrapped_key_Buffer;
  if (HAL_CCB_ECC_WrapSymmetricKey(&hccb, Symetric_AESKey256, &wrappedKeyConf) != HAL_OK)
  {
    Error_Handler();
  }

  /* Fill ECCMul In parameters */
  ECCparam.primeOrderSizeByte            = eccMul192_n_length;
  ECCparam.modulusSizeByte               = eccMul192_p_length;
  ECCparam.coefSignA                     = eccMul192_a_sign;
  ECCparam.pAbsCoefA                     = eccMul192_a_abs;
  ECCparam.pCoefB                        = eccMul192_b;
  ECCparam.pModulus                      = eccMul192_p;
  ECCparam.pPrimeOrder                   = eccMul192_n;
  ECCparam.pPointX                       = eccMul192_Custumized_x;
  ECCparam.pPointY                       = eccMul192_Custumized_y;

  /* Prepare ECC Out parameters  */
  ECCKeyBlob.pIV                         = IV_Buffer;
  ECCKeyBlob.pTag                        = TAG_Buffer;
  ECCKeyBlob.pWrappedKey                 = SclrK_encrypted_192;

  if ((HAL_CCB_ECC_WrapPrivateKey(&hccb, &ECCparam, eccMul192_Sclr_k_P, &wrappedKeyConf, &ECCKeyBlob) != HAL_OK) || (hccb.State != HAL_CCB_STATE_READY))
  {
    Error_Handler();
  }

  if (HAL_CCB_DeInit(&hccb) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    BSP_LED_On(LED_GREEN);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
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
  * @brief CCB Initialization Function
  * @param None
  * @retval None
  */
static void MX_CCB_Init(void)
{

  /* USER CODE BEGIN CCB_Init 0 */

  /* USER CODE END CCB_Init 0 */

  /* USER CODE BEGIN CCB_Init 1 */

  /* USER CODE END CCB_Init 1 */
  hccb.Instance = CCB;
  if (HAL_CCB_Init(&hccb) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CCB_Init 2 */

  /* USER CODE END CCB_Init 2 */

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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
    /* Toggle green led for error */
    (void) BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(500);
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

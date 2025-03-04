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
CCB_ECDSACurveParamTypeDef ECDSAparam;
CCB_ECDSAKeyBlobTypeDef ECDSAKeyBlob;
CCB_ECDSASignTypeDef Signature;
CCB_ECCMulPointTypeDef publickeyOut;
RNG_HandleTypeDef hrng;
PKA_HandleTypeDef hpka;
PKA_ECDSAVerifInTypeDef inVerif;

/* Input buffers */
uint8_t ecdsa192_Prvt_keyP[24]         = {0x58, 0x9B, 0x8F, 0x39, 0x62, 0xD2, 0xF7, 0x40, 0x37, 0x8F, 0x68, 0x95, 0xDD, 0x42, 0xD5, 0x41, 0xB6, 0x3E, 0x01, 0x49, 0xD0, 0x81, 0x11, 0x8C};

const uint32_t ecdsa192_n_length       = 24;

const uint32_t ecdsa192_p_length       = 24;

const uint32_t ecdsa192_a_sign         = 0x00000001;

const uint8_t ecdsa192_a_abs[24]       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};

const uint8_t ecdsa192_b[24]           = {0x64, 0x21, 0x05, 0x19, 0xE5, 0x9C, 0x80, 0xE7, 0x0F, 0xA7, 0xE9, 0xAB, 0x72, 0x24, 0x30, 0x49, 0xFE, 0xB8, 0xDE, 0xEC, 0xC1, 0x46, 0xB9, 0xB1};

const uint8_t ecdsa192_p[24]           = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const uint8_t ecdsa192_n[24]           = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0xDE, 0xF8, 0x36, 0x14, 0x6B, 0xC9, 0xB1, 0xB4, 0xD2, 0x28, 0x31};

const uint8_t ecdsa192_Gx[24]          = {0x18, 0x8D, 0xA8, 0x0E, 0xB0, 0x30, 0x90, 0xF6, 0x7C, 0xBF, 0x20, 0xEB, 0x43, 0xA1, 0x88, 0x00, 0xF4, 0xFF, 0x0A, 0xFD, 0x82, 0xFF, 0x10, 0x12};

const uint8_t ecdsa192_Gy[24]          = {0x07, 0x19, 0x2B, 0x95, 0xFF, 0xC8, 0xDA, 0x78, 0x63, 0x10, 0x11, 0xED, 0x6B, 0x24, 0xCD, 0xD5, 0x73, 0xF9, 0x77, 0xA1, 0x1E, 0x79, 0x48, 0x11};

uint8_t ECDSAHash_192[24]              = {0xA9, 0x80, 0xB6, 0xBF, 0x67, 0x7A, 0xE3, 0x65, 0x27, 0x32, 0xCC, 0x78, 0x58, 0x54, 0x88, 0x9D, 0xCE, 0xE4, 0x69, 0x8F, 0xDF, 0x73, 0x8D, 0x08};
/* Output buffers */
uint32_t PrivateKey_encrypted_192[8]   = {0};
uint8_t ECDSASout_192[24]              = {0};
uint8_t ECDSARout_192[24]              = {0};
uint8_t PublicKeyXout_192[24]          = {0};
uint8_t PublicKeyYout_192[24]          = {0};
uint32_t Wrapped_key_Buffer[8]         = {0};
uint32_t IV_Buffer[4]                  = {0};
uint32_t TAG_Buffer[4]                 = {0};

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
  MX_CCB_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* Fill CRYPTON Instances */
  hccb.Instance                        = CCB;
  if (HAL_CCB_Init(&hccb) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure Wrapped Key */
  wrappedKeyConf.WrappingKeyType         = HAL_CCB_USER_KEY_HSW;

  /* Prepare ECDSA Out parameters  */
  ECDSAKeyBlob.pIV                        = IV_Buffer;
  ECDSAKeyBlob.pTag                       = TAG_Buffer;
  ECDSAKeyBlob.pWrappedKey                = PrivateKey_encrypted_192;

  /* Fill ECDSA parameters */
  ECDSAparam.primeOrderSizeByte           = ecdsa192_n_length;
  ECDSAparam.modulusSizeByte              = ecdsa192_p_length;
  ECDSAparam.coefSignA                    = ecdsa192_a_sign;
  ECDSAparam.pAbsCoefA                    = ecdsa192_a_abs;
  ECDSAparam.pCoefB                       = ecdsa192_b;
  ECDSAparam.pModulus                     = ecdsa192_p;
  ECDSAparam.pPrimeOrder                  = ecdsa192_n;
  ECDSAparam.pPointX                      = ecdsa192_Gx;
  ECDSAparam.pPointY                      = ecdsa192_Gy;

  /* Blob Creation and use for ECDSA Signature  */
  if ((HAL_CCB_ECDSA_WrapPrivateKey(&hccb, &ECDSAparam, ecdsa192_Prvt_keyP, &wrappedKeyConf, &ECDSAKeyBlob) != HAL_OK) || (hccb.State != HAL_CCB_STATE_READY))
  {
    Error_Handler();
  }

  /* Prepare Signature field */
  Signature.pSSign                   = ECDSASout_192;
  Signature.pRSign                   = ECDSARout_192;

  if (HAL_CCB_ECDSA_Sign(&hccb, &ECDSAparam, &wrappedKeyConf, &ECDSAKeyBlob, ECDSAHash_192, &Signature) != HAL_OK)
  {
    Error_Handler();
  }

  /* signing public key computation  */
  publickeyOut.pPointX                  = PublicKeyXout_192;
  publickeyOut.pPointY                  = PublicKeyYout_192;
  if (HAL_CCB_ECDSA_ComputePublicKey(&hccb, &ECDSAparam, &wrappedKeyConf, &ECDSAKeyBlob, &publickeyOut) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_CCB_DeInit(&hccb) != HAL_OK)
  {
    Error_Handler();
  }
  /******************************************************************************/
  /*                          Unprotected PKA operation                         */
  /*                Sign Verification    (Hash verification)                    */

  /* Initialise RNG */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initialise PKA in uprotected mode */
  hpka.Instance = PKA;
  if (HAL_PKA_Init(&hpka) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set input parameters  */
  inVerif.primeOrderSize               = ECDSAparam.primeOrderSizeByte;
  inVerif.modulusSize                  = ECDSAparam.modulusSizeByte;
  inVerif.coefSign                     = ECDSAparam.coefSignA;
  inVerif.coef                         = ECDSAparam.pAbsCoefA;
  inVerif.modulus                      = ECDSAparam.pModulus;
  inVerif.basePointX                   = ECDSAparam.pPointX;
  inVerif.basePointY                   = ECDSAparam.pPointY;
  inVerif.primeOrder                   = ECDSAparam.pPrimeOrder;
  inVerif.hash                         = ECDSAHash_192;

  inVerif.pPubKeyCurvePtX              = publickeyOut.pPointX  ;
  inVerif.pPubKeyCurvePtY              = publickeyOut.pPointY  ;

  inVerif.RSign                        = Signature.pRSign;
  inVerif.SSign                        = Signature.pSSign;

  /* Launch the verification */
  if (HAL_PKA_ECDSAVerif(&hpka, &inVerif, 1000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Retrieve computation result */
  if (HAL_PKA_ECDSAVerif_IsValidSignature(&hpka) != SET)
  {
    while (1)
    {
      /* Toggle green led for error */
      (void) BSP_LED_Toggle(LED_GREEN);
      HAL_Delay(500);
    }
  }

  /* Deinitialize the PKA */
  if (HAL_PKA_DeInit(&hpka) != HAL_OK)
  {
    Error_Handler();
  }

  /* Deinitialize the RNG */
  if (HAL_RNG_DeInit(&hrng) != HAL_OK)
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS|RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  RCC_OscInitStruct.MSIKState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIKSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSIKDiv = RCC_MSI_DIV2;

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

  /* USER CODE BEGIN ADF1_Init 0 */

  /* USER CODE END ADF1_Init 0 */

  /* USER CODE BEGIN ADF1_Init 1 */

  /* USER CODE END ADF1_Init 1 */

  hccb.Instance                        = CCB;
  if (HAL_CCB_Init(&hccb) != HAL_OK)
  {
    Error_Handler();
  }

  /**
    AdfFilterConfig0 structure initialization

    WARNING : only structure is filled, no specific init function call for filter
  */
  /* USER CODE BEGIN ADF1_Init 2 */

  /* USER CODE END ADF1_Init 2 */

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

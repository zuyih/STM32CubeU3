/**
  ******************************************************************************
  * @file    MbedTLS_HW_KWE/Initial_Attestation_KWE/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the
  *          Mbed TLS PSA opaque driver with STM32 Key Wrap Engine to sign a
  *          token for initial attestation with device attestation keys (DUA_FU,
  *          or DUA_LU). It explains how to export the public key and compare
  *          it with device certificate's public key without exposing the
  *          private key in transparent form.
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
#include <string.h>
#include "main.h"
#include "crypto.h"
#include "mbedtls/pem.h"
#include "kwe_psa_driver_interface.h"
#include "stm32_cert.h"

/* Global variables ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define PEM_BEGIN_CRT           "-----BEGIN CERTIFICATE-----\n"
#define PEM_END_CRT             "-----END CERTIFICATE-----\n"
#define PEM_BEGIN_PUBLIC_KEY    "-----BEGIN PUBLIC KEY-----\n"
#define PEM_END_PUBLIC_KEY      "-----END PUBLIC KEY-----\n"
#define PEM_BEGIN_TOKEN         "-----BEGIN TOKEN -----\n"
#define PEM_END_TOKEN           "-----END TOKEN -----\n"
    
#define ATTESTATION_CHALLENGE_SIZE_32 (32U)
#define ATTESTATION_CHALLENGE_SIZE_48 (48U)
#define ATTESTATION_CHALLENGE_SIZE_64 (64U)
#define ATTESTATION_CHALLENGE_SIZE ATTESTATION_CHALLENGE_SIZE_32

/* DUA key are wrapped using the Root Security Services, any
 * key wrapped using the Root Security Services should use
 * ID between PSA_KWE_KEY_ID_RSSE_MIN and PSA_KWE_KEY_ID_RSSE_MAX
 * 0x3ffffffE and 0x3fffffff IDs are reserved to DUA USER keys.
 */
#define PSA_KWE_ECC_KEY_ID_DUA_USER_FU   ((psa_key_id_t)0x3ffffffE) /* DUA user free use Certificate Key ID */
#define PSA_KWE_ECC_KEY_ID_DUA_USER_LU   ((psa_key_id_t)0x3fffffff) /* DUA user license use Certificate Key ID */
#define PSA_KWE_ECC_KEY_DUA_USER_BITS    (256U)                     /* DUA user size in bits */
#define CERT_OFFSET_PUBKEY_DUA_USER      (272U)

/* Private typedef -----------------------------------------------------------*/
/* Attestation token format is available at:
 * https://www.ietf.org/archive/id/draft-tschofenig-rats-psa-token-21.html
 */
typedef struct {
    uint8_t challenge[ATTESTATION_CHALLENGE_SIZE]; /* 32/48/64-byte challenge */
    uint8_t boot_seed[32];                         /* 32-byte boot seed */
    uint8_t instance_id[32+1];                     /* 0x1 + hash(initial attestation public key ANS1 format */
    uint8_t implementation_id[32];                 /* 32-byte of the ROM SW version */
    int32_t client_id;                             /* Negative number represents non-secure, positive numbers represents secure */
    uint32_t security_lifecycle;                   /* Security lifecycle state */
    uint32_t firmware_version;                     /* Firmware version */
    uint8_t measurements[32];                      /* 32-byte hash of firmware/configuration */
    uint8_t signed_id[32];                         /* Hash of the public authentication key of the firmware component */
    uint8_t device_id[16];                         /* 16-byte device ID */
    uint8_t signature[64];                         /* 64-byte signature (e.g., using ECDSA with P-256) */
} attestation_token_t;

typedef enum
{
  FAILED = 0,
  PASSED = 1
} TestStatus;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

const uint8_t Firmware_Data[] =
{
  0xff, 0x62, 0x4d, 0x0b, 0xa0, 0x2c, 0x7b, 0x63, 0x70, 0xc1, 0x62, 0x2e, 0xec, 0x3f, 0xa2, 0x18
};

/* Place DUA_USER_LU wrapped key or DUA_USER_FU wrapped key in a specific section
 * In this application, DUA_USER_LU is used as an example.
 * In prerequisite step the DUA_USER_LU key is exported in wrapped form using
 * STM32CubeProgrammer and the root security services (RSS).
 * Export configuration : Read protection level (RDP0) and security level (non secure).
 */
#if defined(__CC_ARM)|| defined(__ARMCC_VERSION)
__asm(
    ".section .wrapped_keys,\"a\"\n"
    "__wrapped_keys_start__:\n"
    ".incbin \"../Wrapped_Keys/DUA_USLU_DHUK_Sign_RDP0_NS_Priv.bin\"\n"
    "__wrapped_keys_end__:\n"
);
#elif defined(__GNUC__)
asm(".section .wrapped_keys,\"a\";"
".incbin \"../../Wrapped_Keys/DUA_USLU_DHUK_Sign_RDP0_NS_Priv.bin\";"
);
#elif defined(__ICCARM__)

#endif /* __CC_ARM || __ARMCC_VERSION */

extern const uint8_t __wrapped_keys_start__;
const uint8_t *Private_Key_Blob = &__wrapped_keys_start__;


/* Computed data buffer */
/* Computed data buffer */
uint8_t Computed_Pub_Key[2*32+1];
uint8_t Computed_Hash[32]; /* SHA256 length */

__IO TestStatus glob_status = FAILED;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void Error_Handler(void);
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  psa_status_t retval;
  size_t computed_size;
  /* Define key attributes */
  psa_key_attributes_t key_attributes;
  psa_key_handle_t key_handle_public;
  psa_key_handle_t key_handle_private;
  attestation_token_t token ={0};

  /* STM32U3xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock */
  SystemClock_Config();

  /* Configure LD2 */
  BSP_LED_Init(LD2);

  /* --------------------------------------------------------------------------
   *          PSA Crypto library Initialization
   * --------------------------------------------------------------------------
   */
  retval = psa_crypto_init();
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Generate a challenge and fill the attestation token
   * --------------------------------------------------------------------------
   */
  retval = psa_generate_random(token.challenge, sizeof(token.challenge));
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Collect device information and fill the attestation token
   * --------------------------------------------------------------------------
   */
  memcpy(token.device_id, (void *)UID_BASE , sizeof(token.device_id));
  token.firmware_version = 0x0000001234;
  token.security_lifecycle = 0x01;

  /* --------------------------------------------------------------------------
   *          Compute measurements
   *          Compute the hash of the Firmware Data and fill the
   *          attestation token
   *
   * --------------------------------------------------------------------------
   */
  retval = psa_hash_compute(PSA_ALG_SHA_256,
                            Firmware_Data,         /* Firmware Data to digest */
                            sizeof(Firmware_Data),
                            Computed_Hash,         /* Data buffer to receive digest data */
                            sizeof(Computed_Hash), /* Size of hash buffer */
                            &computed_size);       /* Size of computed digest */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Computed_Hash))
  {
    Error_Handler();
  }

  /* fill the attestation token with measurements */
  memcpy(token.measurements, Computed_Hash , sizeof(token.measurements));

  /* --------------------------------------------------------------------------
   *          Compute the hash of the token, excluding the signature field
   *
   * --------------------------------------------------------------------------
   */
  retval = psa_hash_compute(PSA_ALG_SHA_256,
                            (uint8_t*)&token,                     /* Token Data to digest */
                            sizeof(attestation_token_t) - sizeof(token.signature),
                            Computed_Hash,                        /* Data buffer to receive digest data */
                            sizeof(Computed_Hash),                /* Size of hash buffer */
                            &computed_size);                      /* Size of computed digest */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Computed_Hash))
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Sign the token using the STM32 Key Wrap Engine and the device
   *          user key (DUA)
   * --------------------------------------------------------------------------
   */

  /* --------------------------------------------------------------------------
   *          DUA user key import
   *          DUA user key is already wrapped using the Root Security Services
   *          (RSS). PSA_CRYPTO_KWE_DRIVER_LOCATION location should be used to
   *          import the key.
   *
   * --------------------------------------------------------------------------
   */

  /* Init the key attributes */
  key_attributes = psa_key_attributes_init();

  /* Setup the key policy for the private key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
  /* Set up key bits using PSA_KWE_ECC_KEY_DUA_USER_BITS */
  psa_set_key_bits(&key_attributes, PSA_KWE_ECC_KEY_DUA_USER_BITS);
  /* Set up the key location using PSA_CRYPTO_KWE_DRIVER_LOCATION to import the DUA User key using STM32 Key Wrap Engine (KWE) */
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, PSA_CRYPTO_KWE_DRIVER_LOCATION));
  /* Set up DUA user certificate key ID  */
  psa_set_key_id(&key_attributes, PSA_KWE_ECC_KEY_ID_DUA_USER_LU);
  /* Import the DUA user */
  retval = psa_import_key(&key_attributes, Private_Key_Blob, (PSA_KWE_ECC_KEY_DUA_USER_BITS/8U), &key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Reset the key attribute */
  psa_reset_key_attributes(&key_attributes);

  /* --------------------------------------------------------------------------
   *          Sign the token using imported Wrapped Private Key DUA
   * --------------------------------------------------------------------------
   */
  retval = psa_sign_hash(key_handle_private,
                         PSA_ALG_ECDSA(PSA_ALG_SHA_256),           /* Algorithm type */
                         Computed_Hash, sizeof(Computed_Hash),     /* Token to sign */
                         token.signature, sizeof(token.signature), /* Data buf to receive signature*/
                         &computed_size);                          /* Size of computed signature */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(token.signature))
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          DUA user public key export
   *          Export the public key from the wrapped private key to verify
   *          the token signature
   *          Optional : token verification should be done on the server side
   * --------------------------------------------------------------------------
   */
  retval = psa_export_public_key(key_handle_private, Computed_Pub_Key, sizeof(Computed_Pub_Key), &computed_size);
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Import the public key to verify the token signature
   *          Optional : token verification should be done on the server side
   * --------------------------------------------------------------------------
   */

  /* Setup the key policy for public key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));
  /* Import a public key */
  retval = psa_import_key(&key_attributes, Computed_Pub_Key, sizeof(Computed_Pub_Key), &key_handle_public);
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Verify the Token Signature
   *          Optional : token verification should be done on the server side
   * --------------------------------------------------------------------------
   */

  /* Reset the key attribute */
  psa_reset_key_attributes(&key_attributes);

  /* Verify directly the signature passing all the needed parameters */
  retval = psa_verify_hash(key_handle_public,
                           PSA_ALG_ECDSA(PSA_ALG_SHA_256),            /* Algorithm type */
                           Computed_Hash, sizeof(Computed_Hash),      /* Digest to verify */
                           token.signature, sizeof(token.signature)); /* Signature to verify */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Convert the public key to PEM format
   * --------------------------------------------------------------------------
   */
  size_t pem_pubk_len = 0;
  unsigned char pem_pubk[4*sizeof(Computed_Pub_Key)]={0};

  if ((mbedtls_pem_write_buffer(PEM_BEGIN_PUBLIC_KEY, PEM_END_PUBLIC_KEY,
                                Computed_Pub_Key, sizeof(Computed_Pub_Key),
                                pem_pubk, sizeof(pem_pubk), &pem_pubk_len)) != 0)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Convert the entity attestation token (ETA) to PEM format
   * --------------------------------------------------------------------------
   */
  size_t pem_token_len = 0;
  unsigned char pem_token[4*sizeof(token)]={0};

  if ((mbedtls_pem_write_buffer(PEM_BEGIN_TOKEN, PEM_END_TOKEN,
                                token.challenge, sizeof(token),
                                pem_token, sizeof(pem_token), &pem_token_len)) != 0)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Get the DUA USER Certificate :
   *          DUA_USER_FU for free usage
   *          DUA_USER_LU for licensed usage
   * --------------------------------------------------------------------------
   */
  CERT_Status_t cert_status = CERT_HW_ERROR;
  uint32_t certificate_size = 0U;
  uint8_t chip_certificate[DUA_PUBLIC_PART_MAX_SIZE_BYTES]={0};
  size_t pem_cert_len = 0;
  unsigned char pem_certificate[2*DUA_PUBLIC_PART_MAX_SIZE_BYTES]={0};

  /* --------------------------------------------------------------------------
   *          Get the certificate size using DUA_USER
   * --------------------------------------------------------------------------
   */
  cert_status = UTIL_CERT_GetCertificateSize(DUA_USER_LU, &certificate_size);
  /* Verify API returned value */
  if (cert_status != CERT_OK)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Get the certificate using DUA_USER
   * --------------------------------------------------------------------------
   */
  cert_status = UTIL_CERT_GetCertificate(DUA_USER_LU, chip_certificate);
  /* Verify API returned value */
  if (cert_status != CERT_OK)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Compare the public key from the certificate with
   *          the exported public key from the wrapped key
   *          Optional : the verification is useful to avoid using DUA_USER_LU
   *                    key with DUA_USER_FU certificate or DUA_USER_FU key
   *                    with DUA_USER_LU certificate.
   * --------------------------------------------------------------------------
   */
  if (memcmp(&chip_certificate[CERT_OFFSET_PUBKEY_DUA_USER - 1U], Computed_Pub_Key, sizeof(Computed_Pub_Key)) != 0)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   *          Convert the certificate to PEM format
   * --------------------------------------------------------------------------
   */
  if ((mbedtls_pem_write_buffer(PEM_BEGIN_CRT, PEM_END_CRT,
                                chip_certificate, certificate_size,
                                pem_certificate, sizeof(pem_certificate), &pem_cert_len)) != 0)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   * Destroy the PSA keys and clear all data
   * --------------------------------------------------------------------------
  */
  /* Destroy the public key */
  retval = psa_destroy_key(key_handle_public);
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Destroy the private key */
  retval = psa_destroy_key(key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Clear all data associated with the PSA layer */
  mbedtls_psa_crypto_free();

  /* Turn on LD2 in an infinite loop in case of Key Wrap operations are successful */
  BSP_LED_On(LD2);
  glob_status = PASSED;
  while (1)
  {}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = MSIS
  *            SYSCLK(Hz)                     = 96000000
  *            HCLK(Hz)                       = 96000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB3 Prescaler                 = 1
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t latency;

  /* Enable Epod Booster */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1);
  HAL_PWREx_EnableEpodBooster();

  /* At reset, the regulator is the LDO, in range 2 : Need to move to range 1 to reach 96 MHz */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Set Flash latency before increasing MSIS */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);

  /* Activate MSIS as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;

  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select MSIS as system clock source and configure the HCLK, PCLK1, PCLK2 and PCLK3
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  latency = FLASH_LATENCY_2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, latency) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


/**
  * @brief  This function is executed in case of error occurrence
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  /* Toggle LD2 @2Hz to notify error condition */
  while (1)
  {
    BSP_LED_Toggle(LD2);
    HAL_Delay(250);
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif /* USE_FULL_ASSERT */

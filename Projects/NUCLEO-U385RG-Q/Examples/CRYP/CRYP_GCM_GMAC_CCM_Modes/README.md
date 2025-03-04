## <b>CRYP_GCM_GMAC_CCM_Modes Description</b>

How to use the CRYP peripheral to encrypt data and generate authentication tags using GCM/GMAC/CCM
modes.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz for STM32U3xx Devices.

In this example, the following key sizes are used: 128 or 256.

Ciphering with a 128- or 256-bit long key is used in interrupt mode with data type set to 32-bit (no swapping).
Authentication tag is generated in polling mode.

This example successively carries out

1. GCM mode:
   - AES128 GCM encryption and authentication tag generation
2. GMAC mode:
   - AES256 GCM authentication tag generation (no payload)
3. CCM mode:
   - AES256 CCM encryption and authentication tag generation
   - payload size not a multiple of block to illustrate HAL and peripheral padding capabilities

When all ciphering and tag generation are successful, LED2 is turned on.
In case of ciphering or tag generation issue, LED2 is blinking with a 1s period.

#### <b>Notes</b>

 1. When the parity is enabled, the computed parity is inserted at the MSB
    position of the transmitted data.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Security, Cryptography, CRYPT, AESGCM, AESGMAC, AESCCM

### <b>Directory contents</b>

  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32u3xx_nucleo_conf.h BSP configuration file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/stm32u3xx_it.c          Interrupt handlers
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/main.c                  Main program
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385xx devices.

  - This example has been tested with one NUCLEO-U385RG board embedding
    a STM32U385RG device and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U385RG Set-up
     - CRYP Instance : AES

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


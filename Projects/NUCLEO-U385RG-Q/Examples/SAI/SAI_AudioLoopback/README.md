## <b>SAI_AudioLoopback Example Description</b>

  This project demonstrates how to configure SAI to perform audio loopback (record then playback).

  SAI1_BlockA is configured in master reception with PDM activated.

  Two digital microphones of X-NUCLEO-CCA02M2 (digital mems microphone expansion board) are used on this example.
  X-NUCLEO-CCA02M2 is connected but not plugged on NUCLEO-U385RG-Q board.
  Following wired connections are needed:
  - CN7 pin 37 of NUCLEO-U385RG-Q (SAI1_D1) needs to be connected to CN10 pin 26 of X-NUCLEO-CCA02M2 (MIC_PDM12).
  - CN10 pin 37 of NUCLEO-U385RG-Q (SAI1_CK1) needs to be connected to CN10 pin 29 of X-NUCLEO-CCA02M2 (MIC_CLK_NUCLEO).
  - CN6 pin 4 of NUCLEO-U385RG-Q (3V3) needs to be connected to CN6 pin 4 of X-NUCLEO-CCA02M2 (3V3).
  - CN6 pin 6 of NUCLEO-U385RG-Q (GND) needs to be connected to CN6 pin 6 of X-NUCLEO-CCA02M2 (GND).

  On this example, SAI1 kernel clock is set to MSIK and MSIK is configured at 48MHZ.
  In order to have a PDM frequency at arroung 2MHz, SAI master clock divider is set to 12 (MckDiv = Fsai/(Fpdmx2)).
  PDM samples will be stored on PdmBuffer with circular DMA.

  PDM to PCM library will be used to transform PDM samples to PCM samples.
  With a decimation of 64, we will obtain PCM samples at around 32KHz (2MHz/64).

  PCM samples will be stored in loop on PcmBuffer that contains 0.5 second of recording data.

  This PCM buffer will be played using another SAI block.

  SAI1_BlockB is configured in master transmission with a frequency of 32KHz.
  SAI is configured to transmit the content of PCM buffer using circular DMA.

  X-NUCLEO-CCA01M1 is used to output the PCM audio samples to a speaker pairs.
  Following wired connections are needed for SAI:
  - CN10 pin 27 of NUCLEO-U385RG-Q (SAI1_MCLK_B) needs to be connected to CN10 pin 4 of X-NUCLEO-CCA01M1 (MCLK).
  - CN7 pin 32 of NUCLEO-U385RG-Q (SAI1_FS_B) needs to be connected to CN10 pin 16 of X-NUCLEO-CCA01M1 (LRCKI).
  - CN10 pin 31 of NUCLEO-U385RG-Q (SAI1_SCK_B) needs to be connected to CN10 pin 30 of X-NUCLEO-CCA01M1 (BICKI).
  - CN10 pin 29 of NUCLEO-U385RG-Q (SAI1_SD_B) needs to be connected to CN10 pin 26 of X-NUCLEO-CCA01M1 (SDI1).
  Following wired connections are needed for I2C communication between the boards:
  - CN10 pin 3 of NUCLEO-U385RG-Q needs to be connected to CN10 pin 3 of X-NUCLEO-CCA01M1 (I2C_SCL).
  - CN10 pin 5 of NUCLEO-U385RG-Q needs to be connected to CN10 pin 5 of X-NUCLEO-CCA01M1 (I2C_SDA).
  Following wired connections are needed for X-NUCLEO-CCA01M1 power management:
  - CN6 pin 2 of NUCLEO-U385RG-Q needs to be connected to CN6 pin 2 of X-NUCLEO-CCA01M1 (IO_REF).
  - CN6 pin 4 of NUCLEO-U385RG-Q needs to be connected to CN6 pin 4 of X-NUCLEO-CCA01M1 (3V3).
  - CN6 pin 5 of NUCLEO-U385RG-Q needs to be connected to CN6 pin 5 of X-NUCLEO-CCA01M1 (5V).
  - CN6 pin 6 of NUCLEO-U385RG-Q needs to be connected to CN6 pin 6 of X-NUCLEO-CCA01M1 (GND).
  - CN6 pin 7 of NUCLEO-U385RG-Q needs to be connected to CN6 pin 7 of X-NUCLEO-CCA01M1 (GND).
  - CN10 pin 33 of NUCLEO-U385RG-Q needs to be connected to CN10 pin 33 of X-NUCLEO-CCA01M1 (RESET).
  Please have a look on UM1979 for additional information concerning X-NUCLEO-CCA01M1 board.

  If error occurs during previous steps, green led toggles.

  This project is targeted to run on STM32U385RG device on NUCLEO-U385RG-Q board from STMicroelectronics.

  The project configures the maximum system clock frequency at 96Mhz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.
 3. The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
    from Flash memory and external memories, and reach the maximum performance.

### <b>Keywords</b>

Audio, SAI, DMA, PCM

### <b>Directory contents</b>

  - SAI/SAI_AudioLoopback/Src/main.c                   Main program
  - SAI/SAI_AudioLoopback/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
  - SAI/SAI_AudioLoopback/Src/stm32u3xx_it.c           Interrupt handlers
  - SAI/SAI_AudioLoopback/Src/stm32u3xx_hal_msp.c      HAL MSP module
  - SAI/SAI_AudioLoopback/Inc/main.h                   Main program header file
  - SAI/SAI_AudioLoopback/Inc/stm32u3xx_nucleo_conf.h  BSP Configuration file
  - SAI/SAI_AudioLoopback/Inc/stm32u3xx_hal_conf.h     HAL Configuration file
  - SAI/SAI_AudioLoopback/Inc/stm32u3xx_it.h           Interrupt handlers header file
  - SAI/SAI_AudioLoopback/AudioFile/audio.bin          Audio PCM samples file
  - SAI/SAI_AudioLoopback/BSP/Components/sta350bw      BSP driver for X-NUCLEO-CCA01M1
  - SAI/SAI_AudioLoopback/BSP/STM32U3xx_Nucleo         BSP bus driver for I2C communication between NUCLEO-U385RG-Q and X-NUCLEO-CCA01M1

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RG devices without security enabled (TZEN=0).
  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q (MB1841)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


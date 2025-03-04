## <b>SAI_DigitalLoopback Example Description</b>

  This project demonstrates how to configure SAI to perform transmission/reception in DMA circular mode.

  SAI1_BlockA is configured in master transmission and SAI1_BlockB in slave reception.

  SAI1_BlockA signals (FS, SCK and SD) must be connected to SAI_BlockB signals.

  After initialization of SAI blocks, reception is launched on SAI1_BlockB and transmission is launched on SAI1_BlockA,
  both in circular DMA mode.

  After 20 receive complete occurrences, reception and transmission are stopped.

  Receive buffer is then compared with tranmsit buffer.

  If result is OK, green led of board turns on.
  If result is not OK or if error occurs during previous steps, green led toggles.

  Main SAI blocks configurations are as follows (I2S protocol) :
    - Frequency of 16KHz.
    - Frame length of 32 bits, 2 slots of 16 bits.
    - Active frame length of 16 bits.
    - Stereo.

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

Audio, SAI, DMA, audio protocol

### <b>Directory contents</b>

	- SAI/SAI_DigitalLoopback/Src/main.c                   Main program
	- SAI/SAI_DigitalLoopback/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
	- SAI/SAI_DigitalLoopback/Src/stm32u3xx_it.c           Interrupt handlers
	- SAI/SAI_DigitalLoopback/Src/stm32u3xx_hal_msp.c      HAL MSP module
	- SAI/SAI_DigitalLoopback/Inc/main.h                   Main program header file
	- SAI/SAI_DigitalLoopback/Inc/stm32u3xx_nucleo_conf.h  BSP Configuration file
	- SAI/SAI_DigitalLoopback/Inc/stm32u3xx_hal_conf.h     HAL Configuration file
	- SAI/SAI_DigitalLoopback/Inc/stm32u3xx_it.h           Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RG devices without security enabled (TZEN=0).
  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q (MB1841)
    board and can be easily tailored to any other supported device
    and development board.
  - NUCLEO-U385RG-Q set-up:
    - Connection of SAI1_BlockA with SAI1_BlockB FS signals,
      PA9 (pin 2 of CN10) with PB6 (pin 3 of CN10).
    - Connection of SAI1_BlockA with SAI1_BlockB SCK signals,
      PA8 (pin 23 of CN10) with PB3 (pin 31 of CN10).
    - Connection of SAI1_BlockA with SAI1_BlockB SD signals,
      PC1 (pin 36 of CN7) with PB5 (pin 29 of CN10).

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


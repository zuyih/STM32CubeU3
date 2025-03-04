## <b>SPI_FullDuplex_ComDMA_Slave Example Description</b>

How to handle a SPI as a Slave to perform a data buffer transmission/reception 
between two boards, using DMA.

	Board: NUCLEO-U385RG-Q (embeds a STM32U385RGTxQ device)
	CLK Pin: PA.01 (Pin 30 on CN7 connector)
	MISO Pin: PA.06 (Pin 13 on CN10 connector)
	MOSI Pin: PA.07 (Pin 15 on CN10 connector)

HAL architecture allows user to easily change code to move to Polling or IT
mode. To see others communication modes please check following examples:

- SPI_FullDuplex_ComPolling_Master and SPI_FullDuplex_ComPolling_Slave
- SPI_FullDuplex_ComIT_Master and SPI_FullDuplex_ComIT_Slave

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK,
GPIO, DMA and NVIC). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_DMA() function allows the reception and the
transmission of a predefined data buffer at the same time (Full Duplex Mode).

If the Master board is used, the project SPI_FullDuplex_ComDMA_Master must be used.
If the Slave board is used, the project SPI_FullDuplex_ComDMA_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive_DMA(), at the same time SPI Slave transmits aTxBuffer
and receives aRxBuffer through HAL_SPI_TransmitReceive_DMA().
The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update
the variable wTransferState used in the main function to check the transfer status.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:

 - LED2 toggles quickly on master board waiting User push-button to be pressed.
 - LED2 turns ON when the transmission/reception process is complete.
 - LED2 toggle when there is an error in transmission/reception process.

#### <b>Notes</b>

 1. You need to perform a reset on Slave board, then perform it on Master board
    to have the correct behaviour of this example.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI, DMA

### <b>Directory contents</b>

  - SPI/SPI_FullDuplex_ComDMA_Slave/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - SPI/SPI_FullDuplex_ComDMA_Slave/Inc/stm32u3xx_hal_conf.h   HAL configuration file
  - SPI/SPI_FullDuplex_ComDMA_Slave/Inc/stm32u3xx_it.h         Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComDMA_Slave/Inc/main.h                       Header for main.c module
  - SPI/SPI_FullDuplex_ComDMA_Slave/Src/stm32u3xx_it.c         Interrupt handlers
  - SPI/SPI_FullDuplex_ComDMA_Slave/Src/main.c                       Main program
  - SPI/SPI_FullDuplex_ComDMA_Slave/Src/system_stm32u3xx.c     stm32u3xx system source file
  - SPI/SPI_FullDuplex_ComDMA_Slave/Src/stm32u3xx_hal_msp.c    HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect Master board PA.01 (Pin 30 on CN7 connector) to Slave Board PA.01 (Pin 30 on CN7 connector)
    - Connect Master board PA.06 (Pin 13 on CN10 connector) to Slave Board PA.06 (Pin 13 on CN10 connector)
    - Connect Master board PA.07 (Pin 15 on CN10 connector) to Slave Board PA.07 (Pin 15 on CN10 connector)
    - Connect Master board GND  to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files (master project) and load your image into target memory
    - Load the project in Master Board
 - Rebuild all files (slave project) and load your image into target memory
    - Load the project in Slave Board
 - Run the example




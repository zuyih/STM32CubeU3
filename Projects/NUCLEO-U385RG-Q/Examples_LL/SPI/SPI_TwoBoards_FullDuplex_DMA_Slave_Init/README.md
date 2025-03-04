## <b>SPI_TwoBoards_FullDuplex_DMA_Slave_Init Example Description</b>

Data buffer transmission and reception via SPI using DMA mode. This example is
based on the STM32U3xx SPI LL API. The peripheral initialization uses
LL unitary service functions for optimization purposes (performance and size).

The communication is done with 2 boards through SPI.

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using DMA Transfer mode through the STM32U3xx SPI LL API.

This example is split in two projects, Master board and Slave board:

- Master Board
  SPI3 Peripheral is configured in Master mode.
  GPDMA1_Channel0 and GPDMA1_Channel1 configured to transfer Data via SPI peripheral
  GPIO associated to User push-button is linked with EXTI.

- Slave Board
  SPI3 Peripheral is configured in Slave mode.
  GPDMA1_Channel0 and GPDMA1_Channel1 configured to transfer Data via SPI peripheral

Example execution:

- On BOARD MASTER, LD2 is blinking Fast (200ms) and wait USER push-button action.
- Press User push-button on BOARD MASTER start a Full-Duplex communication through DMA.
- On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line) will be done at the same time.
- SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.
- LD2 is On on both boards if data is well received.
- In case of errors, LD2 is blinking Slowly (1s).

### <b>Note</b>

You need to perform a reset on Master board, then perform it on Slave board to have the correct behaviour of this example.

### <b>Keywords</b>

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI, DMA

### <b>Directory contents</b>

  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Inc/main.h                  Header for main.c module
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Src/stm32u3xx_it.c          Interrupt handlers
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Src/main.c                  Main program
  - SPI/SPI_TwoBoards_FullDuplex_DMA_Master_Init/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on NUCLEO-U385RG-Q devices.
  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.
  - NUCLEO-U385RG-Q Set-up :
    - Connect Master board PC10 to Slave Board PC10 (connected to pin 1 of CN7 connector)
    - Connect Master board PC11 to Slave Board PC11 (connected to pin 2 of CN7 connector)
    - Connect Master board PC12 to Slave Board PC12 (connected to pin 3 of CN7 connector)
    - Connect Master board GND to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    - Load the Master project in Master Board
    - Load the Slave project in Slave Board
 - Run the example


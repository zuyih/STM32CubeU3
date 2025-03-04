## <b>SPI_OneBoard_FullDuplex_IT Example Description</b>

This example shows how to perform SPI data buffer transmission/reception between two instances in the same board by using interruptions.  
This  example is based on the STM32U3xx SPI LL API. The peripheral initialization uses LL unitary service functions for optimization purposes (performance and size).  

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32U385xxQ Devices :  
The CPU at 96 MHz.  
The HCLK for CD Domain AXI and AHB peripherals at 96 MHz.  
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals to run at 96 MHz/2.  

This example shows how to configure GPIO and SPI peripherals to use a Full-Duplex communication using IT mode through the STM32U3xx SPI LL API.  
Configure_SPI1() function is called to configure and enable the SPI MASTER (SPI1) and also associated GPIO pins.  
Configure_SPI2() function is called to configure and enable the SPI SLAVE (SPI2) and also associated GPIO pins.  

The SPI communication is then initiated.  
The LL_SPI_StartMasterTransfer() function allows the transmission and the reception of a predefined data buffer at the same time (Full Duplex Mode).  
  
For this example the aTxBuffer is predefined and the SPI1_RxBuffer, SPI2_RxBuffer size is same as aTxBuffer.  
  
In a first step, SPI Master starts the communication by sending aTxBuffer and receiving SPI1_RxBuffer.  
At the same time, SPI Slave transmits aTxBuffer and receives SPI1_RxBuffer.  
  
The callback functions (SPI1_Tx_Callback, SPI1_Rx_Callback, SPI2_Tx_Callback, SPI2_Rx_Callback, SPI2_EOT_Callback, SPI1_EOT_Callback and SPI_TransferError_Callback) update  
the variables SPI1_ReceiveIndex, SPI1_TransmitIndex, SPI2_ReceiveIndex, SPI2_TransmitIndex and NbDataToTransmit used in the main function to check the transfer status.  
Finally, aTxBuffer with SPI1_RxBuffer and aTxBuffer with SPI2_RxBuffer are respectively compared through Buffercmp() in order to check buffers correctness.  
  
STM32 board's LED2 can be used to monitor the transfer status:  
 - LED2 blinks Fast (200ms) waiting User push-button to be pressed to start the transfer.  
 - LED2 turns ON when the transmission and reception process is complete with success.  
 - LED2 blinks Slowly (1s) when there is an error in transmission/reception process.  

### <b>Keywords</b>

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI

### <b>Directory contents</b>

  - SPI/SPI_OneBoard_FullDuplex_IT/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - SPI/SPI_OneBoard_FullDuplex_IT/Inc/main.h                  Header for main.c module
  - SPI/SPI_OneBoard_FullDuplex_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_OneBoard_FullDuplex_IT/Src/stm32u3xx_it.c          Interrupt handlers
  - SPI/SPI_OneBoard_FullDuplex_IT/Src/main.c                  Main program
  - SPI/SPI_OneBoard_FullDuplex_IT/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385xxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect Master CLK  PA1 : CN8.A1  (connected to pin A1  of CN8 connector) to Slave CLK  PB13 : CN10.30 (connected to pin 30 of CN10 connector)
    - Connect Master MOSI PA6 : CN5.D12 (connected to pin D12 of CN5 connector) to Slave MOSI PB14 : CN10.28 (connected to pin 28 of CN10 connector)
    - Connect Master MISO PA7 : CN5.D11 (connected to pin D11 of CN5 connector) to Slave MISO PB15 : CN10.26 (connected to pin 26 of CN10 connector)

The connection between the pins should use a short wires.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
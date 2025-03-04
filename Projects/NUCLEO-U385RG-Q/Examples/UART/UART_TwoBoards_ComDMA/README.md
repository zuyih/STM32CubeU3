## <b>UART_TwoBoards_ComDMA Example Description</b>

UART transmission (transmit/receive) in DMA mode between two boards.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

    Board: NUCLEO-U385RG-Q (embeds a STM32U385RGTxQ device)
    Tx Pin: PA.09 (Pin 2 in CN10)
    Rx Pin: PA.10 (Pin 4 in CN10)

The user presses the User push-button on board 1.
Then, board 1 sends in DMA mode a message to board 2 that sends it back to 
board 1 in DMA mode as well.
Finally, board 1 and 2 compare the received message to that sent.
If the messages are the same, the test passes.


WARNING: as both boards do not behave the same way, "TRANSMITTER_BOARD" compilation
switch is defined in /Src/main.c and must be enabled
at compilation time before loading the executable in the board that first transmits
then receives.
The receiving then transmitting board needs to be loaded with an executable
software obtained with TRANSMITTER_BOARD disabled. 

NUCLEO-U385RG-Q board LED is used to monitor the transfer status:

- While board 1 is waiting for the user to press the User push-button, its LD2 is
  blinking rapidly (100 ms period).
- While board 2 is waiting for the message from board 1, its LD2 is emitting
  a couple of flashes every half-second.
- When the test passes, LD2 on both boards is turned on, otherwise the test has failed. 
- If there is an initialization or transfer error, LD2 is slowly blinking (1 sec. period).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.


The UART is configured as follows:


    - BaudRate = 9600 baud  
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

### <b>Notes</b>

 1. When the parity is enabled, the computed parity is inserted at the MSB position of the transmitted data.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
   based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
   a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
   than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
   To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
   to have correct HAL operation.

### <b>Keywords</b>

Connectivity, UART/USART, baud rate, RS-232, full-duplex, DMA, parity, stop bit,
Transmitter, Receiver, Asynchronous

### <b>Directory contents</b>

  - UART/UART_TwoBoards_ComDMA/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - UART/UART_TwoBoards_ComDMA/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - UART/UART_TwoBoards_ComDMA/Inc/stm32u3xx_it.h          DMA interrupt handlers header file
  - UART/UART_TwoBoards_ComDMA/Inc/main.h                  Header for main.c module  
  - UART/UART_TwoBoards_ComDMA/Src/stm32u3xx_it.c          DMA interrupt handlers
  - UART/UART_TwoBoards_ComDMA/Src/main.c                  Main program
  - UART/UART_TwoBoards_ComDMA/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - UART/UART_TwoBoards_ComDMA/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.    
  - This example has been tested with two NUCLEO-U385RG-Q boards embedding
    a STM32U385RGTxQ device and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-U385RG-Q set-up
    - Connect a wire between 1st board PA9 pin (Uart Tx) and 2nd board PA10 pin (Uart Rx)
    - Connect a wire between 1st board PA10 pin (Uart Rx) and 2nd board PA9 pin (Uart Tx)
    - Connect 1st board GND to 2nd Board GND    

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

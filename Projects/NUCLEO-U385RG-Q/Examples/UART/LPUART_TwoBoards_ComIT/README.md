## <b>LPUART_TwoBoards_ComIT Example Description</b>

LPUART transmission (transmit/receive) in Interrupt mode
between two boards.

Board: NUCLEO-U385RG-Q (embeds a STM32U385RGTxQ device)

Tx Pin: PA2 - pin 35 of CN10 connector (pin 2 of CN9)

Rx Pin: PA3 - pin 37 of CN10 connector (pin 1 of CN9)

Two identical boards are connected as shown on the picture above.

Board 1: transmitting then receiving board

Board 2: receiving then transmitting board

NB: After plugging boards already loaded with the example, it is better to either:

      - reset them using the RESET button;
      - start again the example using your preferred IDE.

At the beginning of the main program, the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

The user presses the User push-button on board 1.
Then data are transmitted through LPUART1 in IT mode. 2nd board has configured LPUART1
to receive data in IT mode. Once data received is completed, 2nd board also
transmit same characters on LPUART1 which will be received by 1st board.
Both boards compare transmitted data to received ones. If they are same,
LD2 is turned on, else LD2 is turned off.

Warning:
As both boards do not behave same way, "TRANSMITTER_BOARD" switch
compilation exists to determine either software is for 1st transmitter board or
2nd receiver (then transmitter) board. In other words, 1st board has to be
flashed with software compiled with switch enable, 2nd board has to be flashed
with software compiled with switch disable.

LD2 can be used to monitor the transfer status:

 - LD2 turns ON on master board waiting User push-button to be pressed.
 - LD2 turns OFF on master board waiting the transmission process complete.
 - LD2 turns ON when the transmission/reception process is correct.
 - If there is an initialization or transfer error, LD2 is slowly blinking (1 sec. period).

The LPUART is configured as follows:

    - BaudRate = 115200 baud
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - None parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

**Note:** When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.

#### <b>Notes</b>
  1.  Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  2. The application needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

### <b>Keywords</b>

Connectivity, LPUART, baud rate, RS-232, full-duplex, Interrupt
Transmitter, Receiver, Asynchronous, Low Power

### <b>Directory contents</b>

  - UART/LPUART_TwoBoards_ComIT/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - UART/LPUART_TwoBoards_ComIT/Src/main.c                 Main program
  - UART/LPUART_TwoBoards_ComIT/Src/system_stm32u3xx.c     STM32U3xx system clock configuration file
  - UART/LPUART_TwoBoards_ComIT/Src/stm32u3xx_it.c         Interrupt handlers
  - UART/LPUART_TwoBoards_ComIT/Src/stm32u3xx_hal_msp.c    HAL MSP module
  - UART/LPUART_TwoBoards_ComIT/Inc/main.h                 Main program header file
  - UART/LPUART_TwoBoards_ComIT/Inc/stm32u3xx_hal_conf.h   HAL Configuration file
  - UART/LPUART_TwoBoards_ComIT/Inc/stm32u3xx_it.h         Interrupt handlers header file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - STM32U385RGTxQ Set-up
    - Connect a wire between 1st board PA2 - pin 35 of CN10 connector pin (pin 2 of CN9) (LPUART Tx) and 2nd board PA3 - pin 37 of CN10 connector pin (pin 1 of CN9) (LPUART Rx).
    - Connect a wire between 1st board PA3 - pin 37 of CN10 connector pin (pin 1 of CN9) (LPUART Rx) and 2nd board PA2 - pin 35 of CN10 connector pin (pin 2 of CN9) (LPUART Tx).
    - Connect 1st board GND to 2nd Board GND.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



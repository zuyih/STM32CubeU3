## <b>UART_Console Example Description</b>

How to use the HAL UART API for UART transmission (printf/getchar) via console with user interaction.

   - Board: NUCLEO-U385RG (embeds a STM32U385TG device)
   - UART Instance : USART1
   - Tx Pin: PA9
   - Rx Pin: PA10

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz for STM32U3xx Devices.

The UART peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit() function which core is implementing
the configuration of the needed UART resources according to the used hardware (CLOCK,
GPIO and NVIC). You may update this function to change UART configuration.

The UART/Hyperterminal communication is then initiated (Board is sending an information message using printf service,
inviting user to enter characters on Terminal). Sent messages are displayed on Hyperterminal console.
For getchar test, the user is prompted to enter any sequence of characters, followed by "ENTER" key.
Note that the getchar implementation ensures an echo of the received data.

The UART instance (USART1) is configured as follows:

    - BaudRate = 115200 baud
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled

LD2 turns ON when there is an error.

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

Connectivity, UART/USART, baud rate, RS-232, HyperTerminal, full-duplex, Interrupt
Transmitter, Receiver, Asynchronous

### <b>Directory contents</b>

  - UART/UART_Console/Inc/stm32u3xx_nucleo_conf.h BSP configuration file
  - UART/UART_Console/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - UART/UART_Console/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - UART/UART_Console/Inc/main.h                  Header for main.c module
  - UART/UART_Console/Src/stm32u3xx_it.c          Interrupt handlers
  - UART/UART_Console/Src/main.c                  Main program
  - UART/UART_Console/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - UART/UART_Console/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385xx devices.

  - This example has been tested with one NUCLEO-U385RG board embedding
    a STM32U385RG device and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U385RG Set-up
     - UART Instance : USART1 (Tx Pin: PA9, Rx Pin: PA10)
     - Connect ST-Link cable to the PC USB port to send/receive data between PC and board.
       A virtual COM port will then appear in the HyperTerminal.

  - Hyperterminal configuration:
    - Data Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - Flow control: None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


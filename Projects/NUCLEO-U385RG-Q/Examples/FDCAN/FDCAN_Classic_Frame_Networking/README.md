## <b>FDCAN_Classic_Frame_Networking Example Description</b>

This example describes how to configure the FDCAN peripheral to send and receive Classic CAN frames between two FDCAN units.

> **_NOTE:_**  This example require the same application to be run on two NUCLEO-U385RG-Q connected together.

> **_NOTE:_**  This example needs an external FDCAN transceiver: **MIKROE** - [**MCP2542 CLICK**](https://www.mikroe.com/mcp2542-click).

At the beginning of the main program, the HAL_Init() function is called to reset
all the peripherals and initialize the systick used as 1ms HAL timebase.

Then the *SystemClock_Config()* function is used to configure the system clock (SYSCLK) to run at 96 MHz.

The example is divided into several steps:<br>

  - In the first step, the *HAL_FDCAN_Init()* function is called to configure the module with a **Nominal Bit Rate of 1 MBit/s** and a **sampling point at 75%**, in classical CAN mode.<br>
    Reception filters are then configured with *HAL_FDCAN_ConfigFilter()*, and FDCAN module is started with *HAL_FDCAN_Start()*.<br>
  - In the second step, the application does nothing in the main loop, as received messages will be handled in IRQ handler.<br>

  - When User push-button is pressed on any board, its LD2 is turned OFF and its FDCAN module sends a message in classical CAN mode.
  - On the other board, after receiving the message, received payload is compared to expected data in the IRQ handler. If the result is OK, LD2 is turned ON.

  - If at any time of the process an error is encountered, LD2 starts blinking.

The FDCAN peripheral configuration is ensured by the *HAL_FDCAN_Init()* function.
This later is calling the *HAL_FDCAN_MspInit()* function which core is implementing
the configuration of the needed FDCAN resources according to the used hardware (CLOCK, GPIO, NVIC and DMA).
User may update this function to change FDCAN configuration.


NUCLEO-U385RG-Q's LEDs can be used to monitor the transfer status:

  - LD2 is OFF on one board when the message was successfully transmitted and LD2 is ON on the other board when it has successfully received the message.
  - LD2 toggle slowly on any of the boards when there is an error in transmission/reception process.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, CAN/FDCAN, IT, Bit rate switching, CAN FIFO, CAN Filter
, MIKROE, MCP2542


### <b>Directory contents</b>

File | Description
 --- | ---
FDCAN/FDCAN_Classic_Frame_Networking/Inc/stm32u3xx_nucleo_conf.h    | BSP configuration file
FDCAN/FDCAN_Classic_Frame_Networking/Inc/stm32u3xx_hal_conf.h   | HAL configuration file
FDCAN/FDCAN_Classic_Frame_Networking/Inc/stm32u3xx_it.h         | Header for stm32u3xx_it.c
FDCAN/FDCAN_Classic_Frame_Networking/Inc/main.h                 | Header for main.c module
FDCAN/FDCAN_Classic_Frame_Networking/Src/stm32u3xx_it.c         | Interrupt handlers
FDCAN/FDCAN_Classic_Frame_Networking/Src/main.c                 | Main program
FDCAN/FDCAN_Classic_Frame_Networking/Src/stm32u3xx_hal_msp.c    | HAL MSP module
FDCAN/FDCAN_Classic_Frame_Networking/Src/system_stm32u3xx.c     | stm32u3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with two NUCLEO-U385RG-Q boards and can be
    easily tailored to any other supported devices and development boards.

  - NUCLEO-U385RG-Q Set-up:
    - FDCAN RX/TX signals can be accessed through the Arduino or morpho connector.<br>
      To use this example, please connect an external FDCAN transceiver to these pins.<br>
      This example has been tested with [**MCP2542 CLICK**](https://www.mikroe.com/mcp2542-click) CAN FD transceiver module from MIKROE.<br>

      To connect the module, please follow these wiring indications:<br>
      - Connect GND, 5V **and** 3V3 on the MCP2545 CLICK board.
      - Connect MCP2545 CLICK **TX** pin to FDCAN **RX** pin PA11.
      - Connect MCP2545 CLICK **RX** pin to FDCAN **TX** pin PA12.

      This must also be done for the other board, running the same example.
      Both MCP2545 CLICK must be connected to each other using a CAN-FD cable.

    No FDCAN connector available on this board. FDCAN RX/TX signals are accessible through Morpho connector.
      - FDCAN RX : PA11
      - FDCAN TX : PA12

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

  - Open your preferred toolchain
  - Rebuild all files from this example and load your image into target memory of both boards.
  - Connect both CAN-FD connectors from each board setup, with a CAN-FD cable.
  - Run the examples on each board, and press User push-button on one of the boards.

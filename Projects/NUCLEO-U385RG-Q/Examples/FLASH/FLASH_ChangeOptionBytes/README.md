## <b>FLASH_ChangeOptionBytes Example Description</b>

How to configure and use the FLASH HAL API to change option bytes values.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK)
to run at 96 MHz.

After Reset, the Flash memory Program/Erase Controller is locked. A dedicated function
is used to enable the FLASH control register access  and the FLASH option bytes register access.

The Option Bytes modification procedure is done by filling the Option Bytes init structure giving
the option byte type and the option parameters. At this stage, the new option bytes will be programmed
through the HAL FLASH OB programming API. Then, an OBL_Launch command is executed to start option bytes loading.

Once this operation is launched, the STM32U3 device will reset and debugger connection will be lost.

In this example, the user can switch between SRAM2 parity enabled or disabled using the USER_BUTTON.
The NUCLEO-U385RG-Q board LEDs can be used to monitor the SRAM2 parity status :

 - LED2 is blinking with a frequency of 200ms when SRAM2 parity is disabled.
 - LED2 is blinking with a frequency of 1s when SRAM2 parity is enabled.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, FLASH, Erase, Program, Page, Mass Erase

### <b>Directory contents</b>

        - FLASH/FLASH_ChangeOptionBytes/Inc/stm32u3xx_nucleo_conf.h   BSP configuration file
        - FLASH/FLASH_ChangeOptionBytes/Inc/stm32u3xx_hal_conf.h      HAL Configuration file
        - FLASH/FLASH_ChangeOptionBytes/Inc/stm32u3xx_it.h            Header for stm32u3xx_it.c
        - FLASH/FLASH_ChangeOptionBytes/Inc/main.h                    Header for main.c module
        - FLASH/FLASH_ChangeOptionBytes/Src/stm32u3xx_it.c            Interrupt handlers
        - FLASH/FLASH_ChangeOptionBytes/Src/main.c                    Main program
        - FLASH/FLASH_ChangeOptionBytes/Src/stm32u3xx_hal_msp.c       MSP initialization and de-initialization
        - FLASH/FLASH_ChangeOptionBytes/Src/system_stm32u3xx.c        STM32U3xx system clock configuration file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGx devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

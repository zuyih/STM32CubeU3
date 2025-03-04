## <b>I3C_Target_ResetPattern_RSTACT Example Description</b>

How to handle an I3C as Target to perform the configuration of a chosen reset pattern after
a Direct RSTACT Command procedure between an I3C Controller and an I3C Target, using
interrupt.

      - Board: NUCLEO-U385RG's (embeds a STM32U385RG device)
      - SCL Pin: PB13 (Arduino D15/SCL CN5 pin 4, Morpho CN10 pin30)
      - SDA Pin: PB14 (Arduino D14/SDA CN5 pin 5, Morpho CN10 pin28)
      Connect GND between each board
      Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

The I3C peripheral configuration is ensured by the HAL_I3C_Init() function.
This later is calling the HAL_I3C_MspInit()function which core is implementing
the configuration of the needed I3C resources according to the used hardware (CLOCK, GPIO and NVIC).
User may update this function to change I3C configuration.

The I3C communication is then initiated.
The project is split in two workspaces:
the Controller Board (I3C_Controller_ResetPattern_RSTACT) and the Target Board (I3C_Target_ResetPattern_RSTACT)

- Target Board
  The HAL_I3C_ActivateNotification() function allow the Target application to be inform when the Controller
  assign a Dynamic Address and Get the reset pattern.

The HAL_I3C_Tgt_Config(), the HAL_I3C_SetConfigFifo(), and the HAL_I3C_ActivateNotification() functions
allow respectively the configuration of the Target descriptor,
the configuration of the internal hardware FIFOs and the enabling of Asynchronous catching event in Target mode using Interrupt.

The Controller send data at 12.5Mhz during Push-pull phase.

In a first step, the Target wait the assignation of a proper associated Dynamic Address from the Controller.

In a second step, the Target wait in a no activity loop, until reset pattern received from the Controller.

At reception of the reset pattern from the Controller, the Target retrieve the defining byte of the RSTACT CCC through HAL_I3C_GetCCCInfo().

This receiving of reset pattern and RSTACT CCC are totally manage by internal hardware state machine,
only the detect of reset pattern is treated on software part through the callback HAL_I3C_NotifyCallback().

Each time a Get reset pattern is confirmed treated through HAL_I3C_NotifyCallback(), the LD2 is toggle.

NUCLEO-U385RG's LEDs can be used to monitor the transfer status:
 - LD2 is toggle each time a reset pattern is treated on hardware side.
 - LD2 is toggle slowly when there is an error in Command Code process.

#### <b>Notes</b>

  1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, Communication, I3C, Interrupt, IT, Controller, Target, Dynamic Address Assignment, Direct Command,
Transmission, reset pattern.

### <b>Directory contents</b>

  - I3C/I3C_Target_ResetPattern_RSTACT/Inc/stm32u3xx_nucleo_conf.h   BSP configuration file
  - I3C/I3C_Target_ResetPattern_RSTACT/Inc/stm32u3xx_hal_conf.h      HAL configuration file
  - I3C/I3C_Target_ResetPattern_RSTACT/Inc/stm32u3xx_it.h            I3C interrupt handlers header file
  - I3C/I3C_Target_ResetPattern_RSTACT/Inc/main.h                    Header for main.c module
  - I3C/I3C_Target_ResetPattern_RSTACT/Src/stm32u3xx_it.c            I3C interrupt handlers
  - I3C/I3C_Target_ResetPattern_RSTACT/Src/main.c                    Main program
  - I3C/I3C_Target_ResetPattern_RSTACT/Src/system_stm32u3xx.c        STM32U3xx system source file
  - I3C/I3C_Target_ResetPattern_RSTACT/Src/stm32u3xx_hal_msp.c       HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RG devices.

  - This example has been tested with NUCLEO-U385RG board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG Set-up

    - Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.
    - Connect I3C_SCL line of Controller board (PB13 (Arduino D15/SCL CN5 pin 4, Morpho CN10 pin30)) to I3C_SCL line of Target Board (PB13 (Arduino D15/SCL CN5 pin 4, Morpho CN10 pin30)).
    - Connect I3C_SDA line of Controller board (PB14 (Arduino D14/SDA CN5 pin 5, Morpho CN10 pin28)) to I3C_SDA line of Target Board (PB14 (Arduino D14/SDA CN5 pin 5, Morpho CN10 pin28)).
    - Connect GND of Controller board to GND of Target Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into Controller memory (I3C_Controller_ResetPattern_RSTACT)
 - Then rebuild all files and load your image into Target memory (I3C_Target_ResetPattern_RSTACT)
 - Run the Controller in debug mode before run the Target in normal mode.
 This sequence will prevent a false startup phase on Target side
 as there is no high level on the bus, if the Target is started before the Controller.

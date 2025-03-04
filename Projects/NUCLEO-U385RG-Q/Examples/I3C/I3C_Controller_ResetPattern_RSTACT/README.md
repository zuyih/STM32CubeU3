## <b>I3C_Controller_ResetPattern_RSTACT Example Description</b>

How to handle an I3C as Controller to perform the configuration of a chosen reset pattern after
a Direct RSTACT Command procedure between an I3C Controller and an I3C Target, using
interrupt.

      - Board: NUCLEO-U385RG (embeds a STM32U385RG device)
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
the configuration of the needed I3C resources according to the used hardware (CLOCK, GPIO, NVIC and DMA).
User may update this function to change I3C configuration.
To have a better signal startup, the user must adapt the parameter BusFreeDuration
depends on its hardware constraint. The value of BusFreeDuration set on this example
is link to Nucleo hardware environment.

The I3C communication is then initiated.
The project is split in two workspaces:
the Controller Board (I3C_Controller_ResetPattern_RSTACT) and the Target Board (I3C_Target_ResetPattern_RSTACT)

- Controller Board
  The HAL_I3C_Ctrl_DynAddrAssign_IT() function allow the Controller to
  manage a Dynamic Address Assignment procedure to Target connected on the bus.
  This communication is done at 12.5Mhz based on I3C source clock which is at 96 MHz.

  The HAL_I3C_Ctrl_SetConfigResetPattern() and the HAL_I3C_Ctrl_TransmitCCC_IT() functions
  allow respectively the configuration of the inserted reset pattern at the end of a Frame and
  the transmission of the RSTACT CCC using IT mode at 12.5Mhz during Push-pull phase
  based on I3C source clock which is at 96 MHz.

For this example the RSTACT_DIRECT_Descriptor and aRSTACT_defining_byte are predefined related
to Common Command Code descriptor treated in this example.

In a first step, after the user press the USER push-button on the Controller Board,
I3C Controller starts the communication by initiate the Dynamic Address Assignment
procedure through HAL_I3C_Ctrl_DynAddrAssign_IT().
This procedure allows the Controller to associate Dynamic Address to the Target
connected on the Bus.
User can verify through debug the payload value by watch the content of TargetDesc1
in the field TARGET_BCR_DCR_PID.

In a second step, the HAL_I3C_Ctrl_SetConfigResetPattern() function set the configuration to insert
reset pattern at the end of a Frame.

Then controller waiting user action.

The user press the USER push-button on the Controller Board to start the communication by sending the first
the RSTACT CCC element of the RSTACT_DIRECT_Descriptor through HAL_I3C_Ctrl_TransmitCCC_IT() and at the end of
this frame the I3C controller send a reset pattern to I3C Targets which receive the RSTACT Command
and the reset pattern.

At the end, Controller is informed at fully transmission of RSTACT CCC element through HAL_I3C_CtrlTxCpltCallback().

NUCLEO-U385RG's LEDs can be used to monitor the transfer status:
 - LD2 is toggle at ENTDAA reception and each time the Command Code process is completed.
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
Transmission, RSTACT CCC, reset pattern.

### <b>Directory contents</b>

  - I3C/I3C_Controller_ResetPattern_RSTACT/Inc/desc_target1.h            Target Descriptor
  - I3C/I3C_Controller_ResetPattern_RSTACT/Inc/stm32u3xx_nucleo_conf.h   BSP configuration file
  - I3C/I3C_Controller_ResetPattern_RSTACT/Inc/stm32u3xx_hal_conf.h      HAL configuration file
  - I3C/I3C_Controller_ResetPattern_RSTACT/Inc/stm32u3xx_it.h            I3C interrupt handlers header file
  - I3C/I3C_Controller_ResetPattern_RSTACT/Inc/main.h                    Header for main.c module
  - I3C/I3C_Controller_ResetPattern_RSTACT/Src/stm32u3xx_it.c            I3C interrupt handlers
  - I3C/I3C_Controller_ResetPattern_RSTACT/Src/main.c                    Main program
  - I3C/I3C_Controller_ResetPattern_RSTACT/Src/system_stm32u3xx.c        STM32U3xx system source file
  - I3C/I3C_Controller_ResetPattern_RSTACT/Src/stm32u3xx_hal_msp.c       HAL MSP file

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

## <b>GTZC_TZSC_MPCBB_TrustZone Example Description</b>

How to use HAL GTZC MPCBB to build any example with illegal Access detection when TrustZone security is activated **(Option byte TZEN=0xB4)**.

The purpose of this example is to declare the SRAM1 (starting from 0x20018000) as non-secure
with HAL GTZC MPCBB services and to demonstrate that a security violation is detected when non-secure application accesses secure SRAM1
memory (starting from 0x20000000).

In non-secure application, LED GREEN is initially TOGGLING.
The security violation detection at MPCBB level is triggered by pressing the USER push-button key:

Any key press on USER push-button initiates an access to secure SRAM1 (SRAM1 base address).
The illegal Access is detected from the secure application.
After returning to non-secure application, the GREEN LED is ON to indicate that illegal access
is detected by the secure application.

This project is composed of two sub-projects:

- one for the secure application part (xxxxx_S)
- one for the non-secure application part (xxxxx_NS).

Please remember that on system with security enabled:

- the system always boots in secure and the secure application is responsible for
launching the non-secure application.
- the SystemInit() function in secure application sets up the SAU/IDAU, FPU and
secure/non-secure interrupts allocations defined in partition_STM32U385xx.h file.

This project shows how to switch from secure application to non-secure application
thanks to the system isolation performed to split the internal SRAM memories
into two halves:

 - the first half for the secure application and
 - the second half for the non-secure application.

The NUCLEO-U385RG-Q board LED GREEN can be used to monitor the transfer status:

 -  **LED_GREEN is TOGGLING** in non-secure (waiting for USER push-button key press),
 -  **LED_GREEN is ON** when the illegal access on internal SRAM1 is detected,

User Option Bytes configuration:

Please note the internal Flash is fully secure by default in TZEN=0xB4. If using Flash memory,
User Option Bytes SECWM1_PSTRT/SECWM1_PEND and SECWM2_PSTRT/SECWM2_PEND should be set according to
the application configuration. Here the proper User Option Bytes setup in line with the project
linker/scatter file is as follows:

  - TZEN=1

This project is targeted to run on STM32U3 device on boards from STMicroelectronics.

This example configures the maximum system clock frequency at 96Mhz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.
	
 3. When deleting MDK-ARM project folder and regenerating this application with STM32CubeMx tool, be sure to place the Veneer$$CMSE section into IROM2 “ROM_NSC_region”.
 4. When deleting the MDK-ARM project folder and regenerating the application with the STM32CubeMX tool, be sure to keep the setup.ini file 
 
### <b>Keywords</b>

Security, TrustZone, GTZC, MPCBB, Memory protection, Block-Based, Memory, internal SRAM, illegal access

### <b>Directory contents</b>

  - Secure/Src/main.c                            Secure Main program
  - Secure/Src/secure_nsc.c                      Secure Non-secure callable functions
  - Secure/Src/stm32u3xx_hal_msp.c               Secure HAL MSP module
  - Secure/Src/stm32u3xx_it.c                    Secure Interrupt handlers
  - Secure/Src/system_stm32u3xx_s.c              Secure STM32U3xx system clock configuration file
  - Secure/Inc/main.h                            Secure Main program header file
  - Secure/Inc/partition_STM32U385xx.h           STM32U3 Device System Configuration file
  - Secure/Inc/stm32u3xx_nucleo_conf.h           BSP Configuration file
  - Secure/Inc/stm32u3xx_hal_conf.h              Secure HAL Configuration file
  - Secure/Inc/stm32u3xx_it.h                    Secure Interrupt handlers header file
  - Secure_nsclib/secure_nsc.h                   Secure Non-Secure Callable (NSC) module header file
  - NonSecure/Src/main.c                         Non-secure Main program
  - NonSecure/Src/stm32u3xx_hal_msp.c            Non-secure HAL MSP module
  - NonSecure/Src/stm32u3xx_it.c                 Non-secure Interrupt handlers
  - NonSecure/Src/system_stm32u3xx_ns.c          Non-secure STM32U3xx system clock configuration file
  - NonSecure/Inc/main.h                         Non-secure Main program header file
  - NonSecure/Inc/stm32u3xx_hal_conf.h           Non-secure HAL Configuration file
  - NonSecure/Inc/stm32u3xx_it.h                 Non-secure Interrupt handlers header file
  - NonSecure/Inc/stm32u3xx_nucleo_conf.h        BSP Configuration file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385xx devices with security enabled (TZEN=0xB4).

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

  - User Option Bytes requirement (with STM32CubeProgrammer tool)

  	- TZEN=1                              System with TrustZone-M enabled

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

<b>IAR</b>

 - Open your toolchain
 - Open Multi-projects workspace file Project.eww
 - Set the "xxxxx_S" as active application (Set as Active)
 - Rebuild xxxxx_S project
 - Rebuild xxxxx_NS project
 - Load the secure and non-secures images into target memory (Ctrl + D)
 - Run the example
 
<b>MDK-ARM</b>

 - Open your toolchain
 - Open Multi-projects workspace file Project.uvmpw
 - Select the xxxxx_S project as Active Project (Set as Active Project)
 - Build xxxxx_S project
 - Select the xxxxx_NS project as Active Project (Set as Active Project)
 - Build xxxxx_NS project
 - Load the non-secure binary (F8)
   (this shall download the \MDK-ARM\xxxxx_ns\Exe\Project_ns.axf to flash memory)
 - Select the xxxxx_S project as Active Project (Set as Active Project)
 - Load the secure binary (F8)
   (this shall download the \MDK-ARM\xxxxx_s\Exe\Project_s.axf to flash memory)
 - Run the example

<b>STM32CubeIDE</b>

 - Open STM32CubeIDE
 - File > Import. Point to the STM32CubeIDE folder of the example project. Click Finish.
 - Select and build the xxxxx_NS project, this will automatically trigger build of xxxxx_S project
 - Select the xxxxx_S project and select “Debug configuration”
   - Double click on “STM32 Cortex-M C/C++ Application”
   - Click on "Debugger" and select "ST-LINK (OpenOCD)" as Debug probe
   - Select  “Startup” >  “Add” >
 - Select the xxxxx_NS project
 - Uncheck "Set breakpoint at: Reset_Handler"
 - Build configuration : Select Release/Debug
 - Click Debug to debug the example
 - When running the STM32CubeIDE project, the entry point should be "Reset_Handler" instead of "main" because when the project is running in RAM, CubeIDE will attach to "main" without performing system initialization.
 
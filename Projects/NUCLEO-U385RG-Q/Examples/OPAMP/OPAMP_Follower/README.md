## <b>OPAMP_Follower Example Description</b>

How to configure the OPAMP peripheral in follower mode interconnected
with DAC and COMP.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U385RGTxQ Devices :
The CPU at 96 MHz.

OPAMP1 is configured as following:

   - Follower mode.
   
   - OPAMP Non Inverting input is connected internally to DAC_OUT1 that perform a simple
     conversion in 8 bits right alignment of 0x7F value.
	 
   - OPAMP Output is connected to COMP1 inverting input internally (share same pin).

Test Description:

   - Push the user Push button to enter normal mode(LD2 On).
   
   - Push the user alternatively to switch from normal power mode to low power mode
     (LD2 Off)and vice versa.
	 
   - Inject voltage higher than 1.65V to COMP1 non inverting input(PA3 pin 1 in CN9).
   
   - check COMP1 output (PB10 pin 7 of CN9).
   
   - If COMP1 output is high test PASS otherwise test FAIL.

LD2 toggles slowly when an error occurred.

### <b>Notes</b>

1- Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

2- The application need to ensure that the SysTick time base is always set
      to 1 millisecond to have correct HAL operation.


### <b>Keywords</b>

Analog, OPAMP, Comparator, Voltage, Signal

### <b>Directory contents</b>

  -  OPAMP/OPAMP_InternalFollower/Src/main.c                   Main program
  -  OPAMP/OPAMP_InternalFollower/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
  -  OPAMP/OPAMP_InternalFollower/Src/stm32u3xx_it.c           Interrupt handlers
  -  OPAMP/OPAMP_InternalFollower/Src/stm32u3xx_hal_msp.c      HAL MSP module
  -  OPAMP/OPAMP_InternalFollower/Inc/main.h                   Main program header file
  -  OPAMP/OPAMP_InternalFollower/Inc/stm32u3xx_nucleo_conf.h  BSP Configuration file
  -  OPAMP/OPAMP_InternalFollower/Inc/stm32u3xx_hal_conf.h     HAL Configuration file
  -  OPAMP/OPAMP_InternalFollower/Inc/stm32u3xx_it.h           Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


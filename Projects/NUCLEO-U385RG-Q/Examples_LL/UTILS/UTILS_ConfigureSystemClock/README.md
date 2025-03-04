## <b>UTILS_ConfigureSystemClock Example Description</b>

Use of UTILS LL API to configure the system clock using MSIS as source clock. 

The user application just needs to configure the MSIS parameters using STM32CubeMX and call the UTILS LL API.

Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

System starts with clock used after reset.
Then, a system clock switch is done to MSIS as the system clock source. Automatically, FLASH latency
is tuned according to system constraints described in the reference manual.

User can easily set their own MSIS parameters by modifying the global variables used to store them.

A LD2 toggle of 1sec provides this information that system is well configured to requested frequency.  

Anyway, user can connect on oscilloscope to MCO pin (PA.8) to check requested frequency:
 
 - SYSCLK frequency with frequency value around 24MHz.

### <b>Keywords</b>

Utils, system, Clock, MSIS, flash latency, SYSCLK, frequency Oscilloscope



### <b>Directory contents</b>

  - UTILS/UTILS_ConfigureSystemClock/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - UTILS/UTILS_ConfigureSystemClock/Inc/main.h                  Header for main.c module
  - UTILS/UTILS_ConfigureSystemClock/Inc/stm32_assert.h          Template file to include assert_failed function
  - UTILS/UTILS_ConfigureSystemClock/Src/stm32u3xx_it.c          Interrupt handlers
  - UTILS/UTILS_ConfigureSystemClock/Src/main.c                  Main program
  - UTILS/UTILS_ConfigureSystemClock/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.
    
  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect the MCO pin to an oscilloscope to monitor the different waveforms:
      - PA.8: Arduino connector D7 CN9 pin 8 , Morpho connector CN10 pin 23 (MB1841)

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



## <b>DAC_SimpleConversion Example Description</b>

How to use the DAC peripheral to do a simple conversion.

      - The example uses the DAC for a simple conversion in 8 bit resolution, right alignment. 
        Voltage generated is set to maximum value (0xFF for 8 bit resolution)
        corresponding to Vdda=3.3V. 
        The result of conversion can be seen by connecting PA4(DAC channel1) to an oscilloscope.

NUCLEO-U385RG-Q board's LED can be used to monitor the process status:
  - LD2 is slowly blinking (1 sec. period) and example is stopped (using infinite loop)
  when there is an error during process.

### <b>Keywords</b>

Analog, DAC, Digital to Analog, Single conversion

### <b>Directory contents</b>

  - DAC/DAC_SimpleConversion/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - DAC/DAC_Simple_Conversion/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - DAC/DAC_Simple_Conversion/Inc/stm32u3xx_it.h          DMA interrupt handlers header file
  - DAC/DAC_Simple_Conversion/Inc/main.h                  Header for main.c module
  - DAC/DAC_Simple_Conversion/Src/stm32u3xx_it.c          DMA interrupt handlers
  - DAC/DAC_Simple_Conversion/Src/main.c                  Main program
  - DAC/DAC_Simple_Conversion/Src/stm32u3xx_hal_msp.c     HAL MSP file
  - DAC/DAC_Simple_Conversion/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    
      - Connect PA4 (DAC Channel1) (pin A2 in CN8) to an oscilloscope.
      

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

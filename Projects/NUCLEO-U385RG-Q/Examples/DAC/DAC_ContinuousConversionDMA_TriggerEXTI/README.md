## <b>DAC_ContinuousConversionDMA_TriggerEXTI Example Description</b>

How to use the DAC to generate a signal using the DMA and triggered by EXTI signal.

For each press on User push-button, a signal has been selected and can be monitored on the DAC channel one :

    - Escalator waveform (Channel 1) using DMA transfer, amplitute: ~3V, frequency: order of KHz.

NUCLEO-U385RG-Q board's LED can be used to monitor the process status:

  - LD2 is slowly blinking (1 sec. period) and example is stopped (using infinite loop) when there is an error during process.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Analog, DAC, Digital to Analog, Continuous conversion, DMA, Escalator generation, Timer Trigger

### <b>Directory contents</b>

  - DAC/DAC_ContinuousConversionDMA_TriggerEXTI/Inc/stm32u3xx_nucleo_conf.h BSP configuration file
  - DAC/DAC_ContinuousConversionDMA_TriggerEXTI/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - DAC/DAC_ContinuousConversionDMA_TriggerEXTI/Inc/stm32u3xx_it.h          DAC interrupt handlers header file
  - DAC/DAC_ContinuousConversionDMA_TriggerEXTI/Inc/main.h                  Header for main.c module
  - DAC/DAC_ContinuousConversionDMA_TriggerEXTI/Src/stm32u3xx_it.c          DAC interrupt handlers
  - DAC/DAC_ContinuousConversionDMA_TriggerEXTI/Src/main.c                  Main program
  - DAC/DAC_ContinuousConversionDMA_TriggerEXTI/Src/stm32u3xx_hal_msp.c     HAL MSP file
  - DAC/DAC_ContinuousConversionDMA_TriggerEXTI/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up :

      - Connect PA4 (DAC Channel1) (Arduino A2 CN8 pin 3, Morpho CN7 pin32) to an oscilloscope.

      - Connect DAC1_EXTI9 input (PC9, Arduino D10/CS/PMW CN5 pin 3, Morpho CN10 pin17) to
      User push-button output (PC13, Morpho CN7 pin23).

      - Use User push-button connected to PC13


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

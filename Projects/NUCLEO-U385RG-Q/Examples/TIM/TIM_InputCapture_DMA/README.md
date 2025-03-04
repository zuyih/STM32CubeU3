## <b>TIM_InputCapture_DMA Example Description</b>

This example demonstrates how to measure the LSI clock frequency thanks to the DMA interface of the TIM16 timer instance.

This project is targeted to run on STM32U385RGTxQ device on NUCLEO-U385RG-Q board from STMicroelectronics.
At the beginning of the main program, the HAL_Init() function is called to reset
all the peripherals and initialize the systick used as 1ms HAL timebase.

Then the *SystemClock_Config()* function is used to configure the system clock (SYSCLK) to run at 96 MHz.

The SystemCoreClockUpdate() function is used to update SystemCoreClock variable according to RCC registers values.
The ClockConfig() function is called to activate the LSI clock and redirect it to the microcontroller clock output 1 (MCO).
The TIM16 input clock (TIM16CLK) is set to APB2 clock (PCLK2).
SYSCLK = 96 MHz
CPU prescaler (CPRE) = DIV1
CPUCLK = SYSCLK/CPRE = 96 MHz
Bus Matrix prescaler (BMPRE) = DIV2
HCLK = CPUCLK/BMPRE = 96 MHz
APB2 prescaler (PPRE2) = DIV2
PCLK2 = HCLK/PPRE2 = 96 MHz
Timer prescaler (TIMPRE) = DIV1
Since PPRE2=DIV2 and TIMPRE=DIV1, APB2 timer clock is twice as fast as the APB2 peripheral clock (PCLK2).
APB2 timer clock = PCLK2 * 2 = 96 MHz
The MCO is internally connected to TIM16 input channel 1 thanks to TIM16 interconnection capabilities.
TIM16 channel 1 is started in input capture mode with DMA. From this point onward, a DMA request is generated
every time a rising edge is detected on the input channel 1 to transfer the captured value from the
capture 1 register to a buffer located in RAM. When the buffer is full (i.e., the DMA transfer is completed),
the minimum, maximum and average frequency values are computed and stored in uwMinFrequency, uwMaxFrequency,
uwAvgFrequency variables respectively.

NUCLEO-U385RG-Q's LED can be used to monitor the result:

 - LD2 is switched ON when the calculated average frequency value corresponds to the LSI frequency.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

TIM, DMA, Input Capture, Frequency

### <b>Directory contents</b>

File | Description
 --- | ---
TIM/TIM_InputCapture_DMA/Inc/stm32u3xx_nucleo_conf.h    | BSP configuration file
TIM/TIM_InputCapture_DMA/Inc/stm32u3xx_hal_conf.h       | HAL configuration file
TIM/TIM_InputCapture_DMA/Inc/stm32u3xx_it.h             | Header for stm32u3xx_it.c
TIM/TIM_InputCapture_DMA/Inc/main.h                       | Header for main.c module
TIM/TIM_InputCapture_DMA/Src/stm32u3xx_it.c             | Interrupt handlers
TIM/TIM_InputCapture_DMA/Src/main.c                       | Main program
TIM/TIM_InputCapture_DMA/Src/stm32u3xx_hal_msp.c        | HAL MSP module
TIM/TIM_InputCapture_DMA/Src/system_stm32u3xx.c         | stm32u3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

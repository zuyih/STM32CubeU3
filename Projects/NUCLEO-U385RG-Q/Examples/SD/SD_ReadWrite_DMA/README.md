### <b>SD_ReadWrite_DMA Example Description</b>

This example performs some write and read transfers to SD Card with SDMMC IP internal DMA mode.

NUCLEO-U385RG board's LED can be used to monitor the transfer status:
  - [**LED_GREEN **]{style="color: GREEN"}  is toggle when write and read transfer is completed and check buffers are OK.
  - [**LED_GREEN **]{style="color: GREEN"}  is on if any error occurs.

This example runs from the external Flash memory. It is launched from a first boot stage and inherits from this boot project
configuration (system clock at 96 MHz and external memory interface at the highest speed).

#### <b>Notes</b>.

1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

 3. Whenever the application is using ITCM/DTCM memories, there is no need for cache maintenance.

### <b>Keywords</b>

Storage, SD, SDMMC, Read Write,DMA

### <b>Directory contents</b>
                  File                                   |         Description
                  ---                                    |             ---
  SD/SD_ReadWrite_DMA/Inc/stm32u3xx_nucleo_conf.h        |     BSP configuration file
  SD/SD_ReadWrite_DMA/Inc/stm32u3xx_hal_conf.h    	 |     HAL configuration file
  SD/SD_ReadWrite_DMA/Inc/stm32u3xx_it.h          	 |     Interrupt handlers header file
  SD/SD_ReadWrite_DMA/Inc/main.h                  	 |     Header for main.c module
  SD/SD_ReadWrite_DMA/Src/stm32u3xx_it.c          	 |     Interrupt handlers
  SD/SD_ReadWrite_DMA/Src/main.c                  	 |     Main program
  SD/SD_ReadWrite_DMA/Src/stm32u3xx_hal_msp.c     	 |     HAL MSP module
  SD/SD_ReadWrite_DMA/Src/system_stm32u3xx.c      	 |     STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with an STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device 
    and development board.

 NUCLEO-U385RG Set-up :

  - Connect the uSD Card to the PmodMicroSD module.
  - Connect SCK(CLK) pin to PC12.
  - Connect MOSI(CMD) pin to PD2.
  - Connect D3(CS) pin  to PC11.
  - Connect MISO(D0) pin to PC8.
  - Connect DAT1(D1) pin to PC9.
  - Connect DAT2(D2) pin to PC10.
 

### <b>How to use it ?</b>

In order to make the program work, you must do the following :
In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
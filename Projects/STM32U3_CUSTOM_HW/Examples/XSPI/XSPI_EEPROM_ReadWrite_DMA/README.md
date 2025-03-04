
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}

## <b>XSPI_EEPROM_ReadWrite_DMA Example Description</b>

- This example describes how to erase part of a QSPI EEPROM memory, write data in DMA mode,
read data in DMA mode and compare the result in an infinite loop.

- At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U385xx Devices :
Example is design for a ST EEPROM memory M95P32. CPU at 96MHz. Memory clock at 48MHz.

- Step 0: At the startup, the QSPI EEPROM memory is erased
- Step 1: Data are copied from the initialization section of the flash to the QSPI memory
- Step 2: Data are read in QSPI mode from EEPROM Memory.
- Step 3: Rx Buffer is compared to Tx Buffer.
- In case of error, Error_Handler function is called (infinite loop).

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.
 3. The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
    from Flash memory and external memories, and reach the maximum performance.

### <b>Keywords</b>

EEPROM, QSPI, QUAD, XSPI, Erase, section, sector, Read, Write, DMA,  TrustZone disabled

### <b>Directory contents</b>

  - XSPI/XSPI_EEPROM_ReadWrite_DMA/Inc/stm32u3xx_hal_conf.h HAL configuration file
  - XSPI/XSPI_EEPROM_ReadWrite_DMA/Inc/stm32u3xx_it.h       Interrupt handlers header file
  - XSPI/XSPI_EEPROM_ReadWrite_DMA/Inc/main.h               Header for main.c module
  - XSPI/XSPI_EEPROM_ReadWrite_DMA/Src/stm32u3xx_it.c       Interrupt handlers
  - XSPI/XSPI_EEPROM_ReadWrite_DMA/Src/main.c               Main program
  - XSPI/XSPI_EEPROM_ReadWrite_DMA/Src/system_stm32u3xx.c   STM32U3xx system source file
  - XSPI/XSPI_EEPROM_ReadWrite_DMA/Src/stm32u3xx_hal_msp.c  HAL MSP file


### <b>Hardware and Software environment</b>

  - This template runs on STM32U385xx devices without security enabled (TZEN=0).
  - This example has been tested with STM32U3_CUSTOM_HW board and can be
    easily tailored to any other supported device and development board.
  - STM32U3_CUSTOM_HW Set-up :
      Board is configured by default to access XSPI memory

### <b>How to use it ?</b>

  In order to make the program work, you must do the following :

  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the example


:::
:::


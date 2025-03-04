## <b>HASH_HMAC_SHA224SHA1_DMA_Suspension Example Description</b>

This example describes how to suspend the HMAC digest computation when data are fed 
to the HASH IP by DMA.

The example first computes the multi-buffer HMAC SHA-224 digest of a low priority 
block in entering input data in DMA mode.

The processing is suspended in order to process a higher priority block to
compute its HMAC SHA1 digest. For the higher priority block, data are entered in
DMA mode as well.

It is shown how to save then how to restore later on the low priority block
context to resume its processing.
For both the low and high priority blocks, the computed digests are compared with
the expected ones.

For illustration purposes,

- the processing of each buffer of the low priority block is suspended
- suspension request is arbitrarily initiated by a flag set by software
(it could be triggered by an interruption in the application code).

The expected HASH digests are already computed using an online
HASH tool. Those values are compared to those computed by the HASH peripheral.
LD2 is turned on when correct digest values are calculated.
In case of digest computation or initialization issue, LD2 is blinking (200 ms period).

#### <b>Notes</b>

 1.  Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
     based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
     a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
     than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
     To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 2.  The application need to ensure that the SysTick time base is always set to 1 millisecond
     to have correct HAL operation.

### <b>Keywords</b>

System, Security, HASH, HMAC, SHA1, SHA224, DMA, digest

### <b>Directory contents</b>

  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/main.h						 Header for main.c module  
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Src/stm32u3xx_it.c          Interrupt handlers
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Src/main.c						 Main program
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

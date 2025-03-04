## <b>I2C_OneBoard_Communication_IT_Init Example Description</b>

How to handle the reception of one data byte from an I2C slave device
by an I2C master device. Both devices operate in interrupt mode. The peripheral is initialized
with LL initialization function to demonstrate LL init usage.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using only one NUCLEO-U385RG-Q.

I2C2 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C3 Peripheral is configured in Master mode with EXTI (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI.

LD2 blinks quickly to wait for user-button press.

Example execution:

Press the User push-button to initiate a read request by Master.

This action will generate an I2C start condition with the Slave address and a read bit condition.
When address Slave match code is received on I2C2, an ADDR interrupt occurs.
I2C2 IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXIS interrupt occurs.
When byte is received on I2C3, an RXNE interrupt occurs.
When RXDR register is read, Master auto-generate a NACK and STOP condition
to inform the Slave that the transfer is finished.
The NACK condition generate a NACK interrupt in Slave side treated in the I2C2 IRQ handler routine by a clear of NACK flag.
The STOP condition generate a STOP interrupt in both side (Slave and Master). I2C2 and I2C3 IRQ handler routine are then
clearing the STOP flag in both side.

LD2 is On if data is well received.

In case of errors, LD2 is blinking slowly (1s).

### <b>Keywords</b>

Connectivity, Communication, I2C, Master, Slave, Transmission, Reception, Fast mode


### <b>Directory contents</b>

    - I2C/I2C_OneBoard_Communication_IT_Init/Inc/stm32u3xx_it.h          Interrupt handlers header file
    - I2C/I2C_OneBoard_Communication_IT_Init/Inc/main.h                  Header for main.c module
    - I2C/I2C_OneBoard_Communication_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
    - I2C/I2C_OneBoard_Communication_IT_Init/Src/stm32u3xx_it.c          Interrupt handlers
    - I2C/I2C_OneBoard_Communication_IT_Init/Src/main.c                  Main program
    - I2C/I2C_OneBoard_Communication_IT_Init/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect GPIOs connected to I2C2 SCL/SDA (PB.10 and PA.6)
    to respectively SCL and SDA pins of I2C3 (PC.0 and PC.1).
      - Connect I2C2_SCL (PB10, Arduino D6 CN9 pin 7, Morpho CN10 pin 25) to I2C3_SCL (PC0, Arduino A5 CN8 pin 6, Morpho CN7 pin 38)
      - Connect I2C2_SDA (PA6, Arduino D12 CN5 pin 5, Morpho CN10 pin 13) to I2C3_SDA (PC1, Arduino A4 CN8 pin 5, Morpho CN7 pin 36)

  - Launch the program
  - Press User push-button to initiate a read request by Master
      then Slave send a byte.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


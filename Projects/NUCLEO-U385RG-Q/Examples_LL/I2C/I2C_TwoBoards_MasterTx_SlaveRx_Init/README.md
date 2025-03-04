## <b>I2C_TwoBoards_MasterTx_SlaveRx_Init Example Description</b>

How to transmit data bytes from an I2C master device using polling mode
to an I2C slave device using interrupt mode. The peripheral is initialized
with LL unitary service functions to optimize for performance and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using two NUCLEO-U385RG-Q.

       Board: NUCLEO-U385RG-Q (embeds a STM32U385RGTxQ device)
       - SCL Pin: PB10 (Arduino D6 CN9 pin 7, Morpho CN10 pin 25)
       - SDA Pin: PA6 (Arduino D12 CN5 pin 5, Morpho CN10 pin 13)

The project is split in two parts the Master Board and the Slave Board.

- Master Board: 
  I2C2 Peripheral is configured in Master mode (Clock 400Khz).
  And GPIO associated to User push-button is linked with EXTI.

- Slave Board: 
  I2C2 Peripheral is configured in Slave mode (Clock 400Khz, Own address 7-bit enabled).

The user can choose between Master and Slave through "#define SLAVE_BOARD"
in the "main.h" file:

- Comment "#define SLAVE_BOARD" to select Master board.
- Uncomment "#define SLAVE_BOARD" to select Slave board.

A first program launch, BOARD SLAVE waiting Address Match code through Handle_I2C_Slave() routine.
LD2 blinks quickly on BOARD MASTER to wait for User push-button press.

Example execution:

Press the User push-button on BOARD MASTER to initiate a write request by Master through Handle_I2C_Master() routine.
This action will generate an I2C start condition with the Slave address and a write bit condition.
When address Slave match code is received on I2C2 of BOARD SLAVE, an ADDR event occurs.
Handle_I2C_Slave() routine is then checking Address Match Code and direction Write.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the bytes.
When acknowledge is received on I2C2 (Master), a TXIS event occurs.
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C2 (Slave), an RXNE event occurs until a STOP condition.
And so each time the Slave acknowledge the byte received a TXIS event occurs on Master side.
Master auto-generate a Stop condition when size of data to transmit is achieved.

The STOP condition generate a STOP event and initiate the end of reception on Slave side.
Handle_I2C_Slave() and Handle_I2C_Master() routine are then clearing the STOP flag in both side.

LD2 is On :

- Slave side if data are well received.
- Master side if transfer sequence is completed.

In case of errors, LD2 is blinking slowly (1s).

### <b>Keywords</b>

Connectivity, Communication, I2C, Master Rx, Slave Tx, Transmission, Reception, Fast mode, SDA, SCL


### <b>Directory contents</b> 

    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/Inc/stm32u3xx_it.h          Interrupt handlers header file
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/Inc/main.h                  Header for main.c module
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/Inc/stm32_assert.h          Template file to include assert_failed function
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/Src/stm32u3xx_it.c          Interrupt handlers
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/Src/main.c                  Main program
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with STM32U385RGTxQ board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect I2C_SCL line of Master board (PB10, Arduino D6 CN9 pin 7, Morpho CN10 pin 25) to
    I2C_SCL line of Slave Board (PB10, Arduino D6 CN9 pin 7, Morpho CN10 pin 25).
    - Connect I2C_SDA line of Master board (PA6, Arduino D12 CN5 pin 5, Morpho CN10 pin 13) to
    I2C_SDA line of Slave Board (PA6, Arduino D12 CN5 pin 5, Morpho CN10 pin 13).
    - Connect GND of Master board to GND of Slave Board.

  - Launch the program.
  - Press User push-button to initiate a write request by Master
      then Slave receive bytes.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory (The user can choose between Master
   and Slave target through "#define SLAVE_BOARD" in the "main.h" file)
      - Comment "#define SLAVE_BOARD" and load the project in Master Board
      - Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
 - Run the example


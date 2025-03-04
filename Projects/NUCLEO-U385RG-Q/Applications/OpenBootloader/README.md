## <b>OpenBootloader Application Description</b>

This application exploits OpenBootloader Middleware to demonstrate how to develop an IAP application
and how use it.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock (SYSCLK)
to run at 48MHz.

Then, these protocols and memories interfaces are configured

  - USART1
  - I2C1
  - I3C1
  - SPI1
  - USB
  - FLASH interface
  - RAM interface
  - Option bytes interface
  - System memory interface
  - OTP interface
  - Engi bytes interface

Then, the OpenBootloader application will wait for incoming communication on one of the supported protocols.

Once a communication is established through one of the supported protocols,
the OpenBootloader application will wait for commands sent by the host.

#### <b>Notes</b>

 1. In case of Mass Erase operation, the OpenBootloader FLASH area must be protected otherwise the OpenBootloader
    will be erased.

 2. In the “OpenBootloader_Init()” function (app_openbootloader.c)), the user can:
       - Select the list of supported commands for a specific interface by defining its own list of commands.
       Here is an example of how to customize USART interface commands list, here only read/write commands are supported:

        OPENBL_CommandsTypeDef USART_Cmd =
        {
          NULL,
          NULL,
          NULL,
          OPENBL_USART_ReadMemory,
          OPENBL_USART_WriteMemory,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL
        };

        USART_Handle.p_Ops = &USART_Ops;
        USART_Handle.p_Cmd = &USART_Cmd;           /* Initialize the USART handle with the list of supported commands */
        OPENBL_USART_SetCommandsList(&USART_Cmd);  /* Register the list of supported commands in MW side */

       - Use the default list of supported commands for a specific interface by reusing the commands list defined in MW side.

    Here is an example of how to use USART interface default commands list:</br>

        /* Register USART interfaces */
        USART_Handle.p_Ops = &USART_Ops;
        USART_Handle.p_Cmd = OPENBL_USART_GetCommandsList();  /* Initialize the USART handle with the default list osupported commands */

### <b>Keywords</b>

OpenBootloader, USART, USB, I2C, I3C, SPI

### <b>Directory contents</b>

     - Core/Src/main.c                                    Main program file
     - Core/Src/stm32u3xx_hal_msp.c                       MSP Initialization file
     - Core/Src/stm32u3xx_it.c                            Interrupt handlers file
     - Core/Src/system_stm32u3xx.c                        stm32u3xx system clock configuration file
     - Core/Inc/main.h                                    Main program header file
     - Core/Inc/stm32u3xx_hal_conf.h                      HAL Library Configuration file
     - Core/Inc/stm32u3xx_it.h                            Interrupt handlers header file
     - OpenBootloader/App/app_openbootloader.c            OpenBootloader application entry point
     - OpenBootloader/App/app_openbootloader.h            Header for OpenBootloader application entry file
     - OpenBootloader/Target/common_interface.c           Contains common functions used by different interfaces
     - OpenBootloader/Target/common_interface.h           Header for common functions file
     - OpenBootloader/Target/flash_interface.c            Contains FLASH interface
     - OpenBootloader/Target/flash_interface.h            Header of FLASH interface file
     - OpenBootloader/Target/i2c_interface.c              Contains I2C interface
     - OpenBootloader/Target/i2c_interface.h              Header of I2C interface file
     - OpenBootloader/Target/i3c_interface.c              Contains I3C interface
     - OpenBootloader/Target/i3c_interface.h              Header of I3C interface file
     - OpenBootloader/Target/spi_interface.c              Contains spi interface
     - OpenBootloader/Target/spi_interface.c              Header of spi interface file
     - OpenBootloader/Target/iwdg_interface.c             Contains IWDG interface
     - OpenBootloader/Target/iwdg_interface.h             Header of IWDG interface file
     - OpenBootloader/Target/openbootloader_conf.h        Header file that contains OpenBootloader HW dependent configuration
     - OpenBootloader/Target/optionbytes_interface.c      Contains OptionBytes interface
     - OpenBootloader/Target/optionbytes_interface.h      Header of OptionBytes interface file
     - OpenBootloader/Target/otp_interface.c              Contains OTP interface
     - OpenBootloader/Target/otp_interface.h              Header of OTP interface file
     - OpenBootloader/Target/ram_interface.c              Contains RAM interface
     - OpenBootloader/Target/ram_interface.h              Header of RAM interface file
     - OpenBootloader/Target/systemmemory_interface.c     Contains ICP interface
     - OpenBootloader/Target/systemmemory_interface.h     Header of ICP interface file
     - OpenBootloader/Target/usart_interface.c            Contains USART interface
     - OpenBootloader/Target/usart_interface.h            Header of USART interface file
     - OpenBootloader/Target/usb_interface.c              Contains USB interface
     - OpenBootloader/Target/usb_interface.h              Header of USB interface file
     - USBX/App/app_usbx_device.c                         USBX Device applicative file
     - USBX/App/app_usbx_device.h                         USBX Device applicative header file
     - USBX/App/ux_device_descriptors.c                   USBX Device descriptor file
     - USBX/App/ux_device_descriptors.h                   USBX Device descriptor header file
     - USBX/App/ux_device_dfu_media.c                     USBX Device interface applicative file
     - USBX/App/ux_device_dfu_media.h                     USBX Device interface applicative header file
     - USBX/App/ux_user.h                                 Contains user defines for USBX configuration
     - USBX/Target/ux_stm32_config.h                      USBX STM32 configuration header file

### <b>Hardware and Software environment</b>

- This application runs on STM32u385xx devices.

  - This example has been tested with a STM32U385RGQ-NUCLEO board and can be
    easily tailored to any other supported device and development board.

  - STM32U385RGQ-NUCLEO set-up to use USART1:
    - To use the USART1 for communication you have to connect:
      - Tx pin of your host adapter to PA9 (CN10: 2)pin of the STM32U385RGQ-NUCLEO board
      - Rx pin of your host adapter to PA10 (CN10: 4)pin of the STM32U385RGQ-NUCLEO board

  - STM32U385RGQ-NUCLEO set-up to use I2C1:
    - In the host, set I2C address to 0x6C
    - To use the I2C1 for communication you have to connect:
      - SCL pin of your host adapter to PB6 (CN10: 3)pin of the STM32U385RGQ-NUCLEO board
      - SDA pin of your host adapter to PB7 (CN10: 5)pin of the STM32U385RGQ-NUCLEO board

  - STM32U385RGQ-NUCLEO set-up to use I3C1:
    - To use the I3C1 for communication you have to connect:
      - SCL pin of your host adapter to PB13 (CN10: 30)pin of the STM32U385RGQ-NUCLEO board
      - SDA pin of your host adapter to PA1 (CN7: 30)pin of the STM32U385RGQ-NUCLEO board

  - STM32U385RGQ-NUCLEO set-up to use SPI1:
      - To use the SPI1 for communication you have to connect:
        - SCK  pin of your host adapter to PA5 (CN10: 11)pin of the STM32U385RGQ-NUCLEO board
        - NSS  pin of your host adapter to PA4 (CN7: 32)pin of the STM32U385RGQ-NUCLEO board
        - MISO pin of your host adapter to PA6 (CN10: 13)pin of the STM32U385RGQ-NUCLEO board
        - MOSI pin of your host adapter to PA7 (CN10: 15)pin of the STM32U385RGQ-NUCLEO board

  - STM32U385RGQ-NUCLEO set-up to use USB:
    - USB FS CN3

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the application
  - Run STM32CubeProgrammer and connect to OpenBootloader using USART1</br>
    Or
  - Connect your I2C host adapter and connect to OpenBootloader using I2C1</br>
    Or
  - Connect your I3C host adapter and connect to OpenBootloader using I3C1</br>
    Or
  - Connect your SPI host adapter and connect to OpenBootloader using SPI1</br>
    Or
  - Connect your USB host adapter and connect to OpenBootloader using USB FS</br>
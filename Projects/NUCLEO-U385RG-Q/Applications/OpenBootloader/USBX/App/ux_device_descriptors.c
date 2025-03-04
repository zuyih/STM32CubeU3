/**
  ******************************************************************************
  * @file    ux_device_descriptors.c
  * @author  MCD Application Team
  * @brief   USBX Device descriptor header file
  ******************************************************************************
   * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ux_device_descriptors.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_DevClassHandleTypeDef  USBD_Device_FS, USBD_Device_HS;

uint8_t UserClassInstance[USBD_MAX_CLASS_INTERFACES] =
{
  CLASS_TYPE_DFU,
};

/* The generic device descriptor buffer that will be filled by builder
   Size of the buffer is the maximum possible device FS descriptor size. */
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN static uint8_t DevFrameWorkDesc_FS[USBD_FRAMEWORK_MAX_DESC_SZ] __ALIGN_END = {0};

/* The generic device descriptor buffer that will be filled by builder
   Size of the buffer is the maximum possible device HS descriptor size. */
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN static uint8_t DevFrameWorkDesc_HS[USBD_FRAMEWORK_MAX_DESC_SZ] __ALIGN_END = {0};

static uint8_t *pDevFrameWorkDesc_FS = DevFrameWorkDesc_FS;

static uint8_t *pDevFrameWorkDesc_HS = DevFrameWorkDesc_HS;

/* String Device Framework :
 Byte 0 and 1 : Word containing the language ID : 0x0904 for US
 Byte 2       : Byte containing the index of the descriptor
 Byte 3       : Byte containing the length of the descriptor string
*/
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN UCHAR USBD_string_framework[USBD_STRING_FRAMEWORK_MAX_LENGTH]
__ALIGN_END = {0};

/* Multiple languages are supported on the device, to add
   a language besides English, the Unicode language code must
   be appended to the language_id_framework array and the length
   adjusted accordingly. */

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
UCHAR USBD_language_id_framework[LANGUAGE_ID_MAX_LENGTH] = {0};

/* Private function prototypes -----------------------------------------------*/
static void USBD_Desc_GetString(uint8_t *desc, uint8_t *Buffer, uint16_t *len);
static uint8_t USBD_Desc_GetLen(uint8_t *buf);

static uint8_t *USBD_Device_Framework_Builder(USBD_DevClassHandleTypeDef *p_dev,
                                              uint8_t *p_DevFrameWorkDesc,
                                              uint8_t *p_UserClassInstance,
                                              uint8_t speed);

static uint8_t USBD_FrameWork_AddToConfDesc(USBD_DevClassHandleTypeDef *p_dev,
                                            uint8_t speed,
                                            uint8_t *p_CmpstConfDesc);

static uint8_t USBD_FrameWork_AddClass(USBD_DevClassHandleTypeDef *p_dev,
                                       USBD_CompositeClassTypeDef class,
                                       uint8_t cfgidx, uint8_t speed,
                                       uint8_t *p_CmpstConfDesc);

static uint8_t USBD_FrameWork_FindFreeIFNbr(USBD_DevClassHandleTypeDef *p_dev);

static void USBD_FrameWork_AddConfDesc(uint32_t conf, uint32_t *p_size);

static void USBD_FrameWork_AssignEp(USBD_DevClassHandleTypeDef *p_dev, uint8_t add,
                                    uint8_t type, uint32_t size);

#if USBD_DFU_CLASS_ACTIVATED == 1U
static void USBD_FrameWork_DFUDesc(USBD_DevClassHandleTypeDef *p_dev, uint32_t pconf,
                                   uint32_t *p_sze);
#endif /* USBD_DFU_CLASS_ACTIVATED */

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  USBD_Get_Device_Framework_Speed
  *         Return the device speed descriptor
  * @param  speed : HIGH or FULL SPEED flag
  * @param  p_length : Pointer to the length of HIGH or FULL SPEED array
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Get_Device_Framework_Speed(uint8_t speed, ULONG *p_length)
{
  uint8_t *p_frame_work = NULL;

  if (USBD_FULL_SPEED == speed)
  {
    USBD_Device_Framework_Builder(&USBD_Device_FS, pDevFrameWorkDesc_FS,
                                  UserClassInstance, speed);

    /* Get the length of USBD_device_framework_full_speed */
    *p_length = (ULONG)(USBD_Device_FS.CurrDevDescSz + USBD_Device_FS.CurrConfDescSz);

    p_frame_work = pDevFrameWorkDesc_FS;
  }
  else
  {
    USBD_Device_Framework_Builder(&USBD_Device_HS, pDevFrameWorkDesc_HS,
                                  UserClassInstance, speed);

    /* Get the length of USBD_device_framework_high_speed */
    *p_length = (ULONG)(USBD_Device_HS.CurrDevDescSz + USBD_Device_HS.CurrConfDescSz);

    p_frame_work = pDevFrameWorkDesc_HS;
  }

  return p_frame_work;
}

/**
  * @brief  USBD_Get_String_Framework
  *         Return the language_id_framework
  * @param  p_length Pointer to the length of String_Framework
  * @retval Pointer to language_id_framework buffer
  */
uint8_t *USBD_Get_String_Framework(ULONG *p_length)
{
  uint16_t len = 0U;
  uint8_t count = 0U;

  /* Set the Manufacturer language Id and index in USBD_string_framework */
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFF;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8;
  USBD_string_framework[count++] = USBD_IDX_MFC_STR;

  /* Set the Manufacturer string in string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_string_framework + count, &len);

  /* Set the Product language Id and index in USBD_string_framework */
  count += len + 1;
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFF;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8;
  USBD_string_framework[count++] = USBD_IDX_PRODUCT_STR;

  /* Set the Product string in USBD_string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_PRODUCT_STRING, USBD_string_framework + count, &len);

  /* Set Serial language Id and index in string_framework */
  count += len + 1;
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFF;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8;
  USBD_string_framework[count++] = USBD_IDX_SERIAL_STR;

  /* Set the Serial number in USBD_string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_SERIAL_NUMBER, USBD_string_framework + count, &len);

#if USBD_DFU_CLASS_ACTIVATED
  /* Set FLASH descriptor index in USBD_string_framework */
  count += len + 1;
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFFU;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8U;
  USBD_string_framework[count++] = USBD_DFU_STRING_FLASH_DESC_INDEX;

  /* Set FLASH descriptor string in USBD_string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_DFU_STRING_FLASH_DESC, USBD_string_framework + count, &len);

  /* Set Option Bytes area 1 descriptor index in USBD_string_framework */
  count += len + 1;
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFFU;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8U;
  USBD_string_framework[count++] = USBD_DFU_STRING_OB1_DESC_INDEX;

  /* Set Option Bytes area 1 descriptor string in USBD_string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_DFU_STRING_OB1_DESC, USBD_string_framework + count, &len);

  /* Set OTP descriptor index in USBD_string_framework */
  count += len + 1;
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFFU;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8U;
  USBD_string_framework[count++] = USBD_DFU_STRING_OTP_DESC_INDEX;

  /* Set OTP descriptor string in USBD_string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_DFU_STRING_OTP_DESC, USBD_string_framework + count, &len);

  /* Set device ID descriptor index in USBD_string_framework */
  count += len + 1;
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFFU;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8U;
  USBD_string_framework[count++] = USBD_DFU_STRING_DEVICE_ID_DESC_INDEX;

  /* Set device ID descriptor string in USBD_string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_DFU_STRING_DEVICE_ID_DESC, USBD_string_framework + count, &len);
#endif /* USBD_DFU_CLASS_ACTIVATED */

  /* Get the length of USBD_string_framework */
  *p_length = strlen((const char *)USBD_string_framework);

  return USBD_string_framework;
}

/**
  * @brief  USBD_Get_Language_Id_Framework
  *         Return the language_id_framework
  * @param  p_length Pointer to the length of Language_Id_Framework
  * @retval Pointer to language_id_framework buffer
  */
uint8_t *USBD_Get_Language_Id_Framework(ULONG *p_length)
{
  uint8_t count = 0U;

  /* Set the language Id in USBD_language_id_framework */
  USBD_language_id_framework[count++] = USBD_LANGID_STRING & 0xFF;
  USBD_language_id_framework[count++] = USBD_LANGID_STRING >> 8;

  /* Get the length of USBD_language_id_framework */
  *p_length = strlen((const char *)USBD_language_id_framework);

  return USBD_language_id_framework;
}

/**
  * @brief  USBD_Desc_GetString
  *         Convert ASCII string into Unicode one
  * @param  p_desc : Pointer to the descriptor buffer
  * @param  Unicode : Formatted string buffer (Unicode)
  * @param  len : descriptor length
  * @retval None
  */
static void USBD_Desc_GetString(uint8_t *p_desc, uint8_t *unicode, uint16_t *len)
{
  uint8_t idx = 0U;
  uint8_t *pdesc;

  if (p_desc == NULL)
  {
    return;
  }

  pdesc = p_desc;
  *len = (uint16_t)USBD_Desc_GetLen(pdesc);

  unicode[idx++] = *(uint8_t *)len;

  while (*pdesc != (uint8_t)'\0')
  {
    unicode[idx++] = *pdesc;
    pdesc++;
  }
}

/**
  * @brief  USBD_Desc_GetLen
  *         return the string length
  * @param  buf : pointer to the ASCII string buffer
  * @retval string length
  */
static uint8_t USBD_Desc_GetLen(uint8_t *buf)
{
  uint8_t  len = 0U;
  uint8_t *pbuff = buf;

  while (*pbuff != (uint8_t)'\0')
  {
    len++;
    pbuff++;
  }

  return len;
}

/**
  * @brief  USBD_Get_Configuration_Number
  *         Return configuration number
  * @param  class_type : Device class type
  * @param  interface_type : Device interface type
  * @retval configuration number
  */
uint16_t USBD_Get_Configuration_Number(uint8_t class_type, uint8_t interface_type)
{
  uint8_t cfg_num = 1U;

  return cfg_num;
}

/**
  * @brief  USBD_Get_Interface_Number
  *         Return interface number
  * @param  class_type : Device class type
  * @param  interface_type : Device interface type
  * @retval interface number
  */
uint16_t USBD_Get_Interface_Number(uint8_t class_type, uint8_t interface_type)
{
  uint8_t itf_num = 0U;
  uint8_t idx = 0U;

  for (idx = 0; idx < USBD_MAX_SUPPORTED_CLASS; idx++)
  {
    if ((USBD_Device_FS.tclasslist[idx].ClassType == class_type) &&
        (USBD_Device_FS.tclasslist[idx].InterfaceType == interface_type))
    {
      itf_num = USBD_Device_FS.tclasslist[idx].Ifs[0U];
    }
  }

  return itf_num;
}

/**
  * @brief  USBD_Device_Framework_Builder
  *         Device Framework builder
  * @param  p_dev: device instance
  * @param  p_DevFrameWorkDesc: Pointer to the device framework descriptor
  * @param  p_UserClassInstance: Pointer to the type of the class to be added
  * @param  speed: Speed parameter HS or FS
  * @retval status
  */
static uint8_t *USBD_Device_Framework_Builder(USBD_DevClassHandleTypeDef *p_dev,
                                              uint8_t *p_DevFrameWorkDesc,
                                              uint8_t *p_UserClassInstance,
                                              uint8_t speed)
{
  static USBD_DeviceDescTypedef *pDevDesc;
  static USBD_DevQualiDescTypedef *pDevQualDesc;
  uint8_t Idx_Instance = 0U;
  uint8_t NumberClass = 1U;

  /* Set Dev and conf descriptors size to 0 */
  p_dev->CurrConfDescSz = 0U;
  p_dev->CurrDevDescSz = 0U;

  /* Set the pointer to the device descriptor area*/
  pDevDesc = (USBD_DeviceDescTypedef *)p_DevFrameWorkDesc;

  /* Start building the generic device descriptor common part */
  pDevDesc->bLength = (uint8_t)sizeof(USBD_DeviceDescTypedef);
  pDevDesc->bDescriptorType = UX_DEVICE_DESCRIPTOR_ITEM;
  pDevDesc->bcdUSB = USB_BCDUSB;
  pDevDesc->bDeviceClass = 0x00;
  pDevDesc->bDeviceSubClass = 0x00;
  pDevDesc->bDeviceProtocol = 0x00;
  pDevDesc->bMaxPacketSize = USBD_MAX_EP0_SIZE;
  pDevDesc->idVendor = USBD_VID;
  pDevDesc->idProduct = USBD_PID;
  pDevDesc->bcdDevice = 0x0200;
  pDevDesc->iManufacturer = USBD_IDX_MFC_STR;
  pDevDesc->iProduct = USBD_IDX_PRODUCT_STR;
  pDevDesc->iSerialNumber = USBD_IDX_SERIAL_STR;
  pDevDesc->bNumConfigurations = USBD_MAX_NUM_CONFIGURATION;
  p_dev->CurrDevDescSz += (uint32_t)sizeof(USBD_DeviceDescTypedef);

  /* Check if USBx is in high speed mode to add qualifier descriptor */
  if (speed == USBD_HIGH_SPEED)
  {
    pDevQualDesc = (USBD_DevQualiDescTypedef *)(p_DevFrameWorkDesc + p_dev->CurrDevDescSz);
    pDevQualDesc->bLength = (uint8_t)sizeof(USBD_DevQualiDescTypedef);
    pDevQualDesc->bDescriptorType = UX_DEVICE_QUALIFIER_DESCRIPTOR_ITEM;
    pDevQualDesc->bcdDevice = 0x0200;
    pDevQualDesc->Class = 0x00;
    pDevQualDesc->SubClass = 0x00;
    pDevQualDesc->Protocol = 0x00;
    pDevQualDesc->bMaxPacketSize = 0x40;
    pDevQualDesc->bNumConfigurations = 0x01;
    pDevQualDesc->bReserved = 0x00;
    p_dev->CurrDevDescSz += (uint32_t)sizeof(USBD_DevQualiDescTypedef);
  }

  /* Build the device framework */
  while (Idx_Instance < USBD_MAX_SUPPORTED_CLASS)
  {
    if ((p_dev->classId < USBD_MAX_SUPPORTED_CLASS) &&
        (p_dev->NumClasses < USBD_MAX_SUPPORTED_CLASS))
    {
      /* Call the composite class builder */
      (void)USBD_FrameWork_AddClass(p_dev,
                                    (USBD_CompositeClassTypeDef)UserClassInstance[Idx_Instance],
                                    0, speed, (p_DevFrameWorkDesc + p_dev->CurrDevDescSz));

      /* Increment the ClassId for the next occurrence */
      p_dev->classId ++;
      p_dev->NumClasses ++;
    }
    Idx_Instance++;

    /* Count the number of Classes different of CLASS_TYPE_NONE */
    if (UserClassInstance[Idx_Instance] != CLASS_TYPE_NONE)
    {
      NumberClass++;
    }
  }
  /* Check if there is a composite class and update device class */
  if (NumberClass > 1)
  {
    pDevDesc->bDeviceClass = 0xEF;
    pDevDesc->bDeviceSubClass = 0x02;
    pDevDesc->bDeviceProtocol = 0x01;
  }
  else
  {
    /* Check if the CDC ACM class is set and update device class */
    if (UserClassInstance[0] == CLASS_TYPE_CDC_ACM)
    {
      pDevDesc->bDeviceClass = 0x02;
      pDevDesc->bDeviceSubClass = 0x02;
      pDevDesc->bDeviceProtocol = 0x00;
    }
  }

  return p_DevFrameWorkDesc;
}

/**
  * @brief  USBD_FrameWork_AddClass
  *         Register a class in the class builder
  * @param  p_dev: Pointer to the device instance
  * @param  pclass: pointer to the class structure to be added
  * @param  class: type of the class to be added (from USBD_CompositeClassTypeDef)
  * @param  cfgidx: configuration index
  * @retval status
  */
uint8_t  USBD_FrameWork_AddClass(USBD_DevClassHandleTypeDef *p_dev,
                                 USBD_CompositeClassTypeDef class,
                                 uint8_t cfgidx, uint8_t speed,
                                 uint8_t *p_CmpstConfDesc)
{
  if ((p_dev->classId < USBD_MAX_SUPPORTED_CLASS) &&
      (p_dev->tclasslist[p_dev->classId].Active == 0U))
  {
    /* Store the class parameters in the global tab */
    p_dev->tclasslist[p_dev->classId].ClassId = p_dev->classId;
    p_dev->tclasslist[p_dev->classId].Active = 1U;
    p_dev->tclasslist[p_dev->classId].ClassType = class;

    /* Call configuration descriptor builder and endpoint configuration builder */
    if (USBD_FrameWork_AddToConfDesc(p_dev, speed, p_CmpstConfDesc) != UX_SUCCESS)
    {
      return UX_ERROR;
    }
  }

  UNUSED(cfgidx);

  return UX_SUCCESS;
}

/**
  * @brief  USBD_FrameWork_AddToConfDesc
  *         Add a new class to the configuration descriptor
  * @param  pdev: device instance
  * @retval status
  */
uint8_t  USBD_FrameWork_AddToConfDesc(USBD_DevClassHandleTypeDef *p_dev, uint8_t speed,
                                      uint8_t *p_CmpstConfDesc)
{
  uint8_t interface = 0U;

  /* The USB drivers do not set the speed value, so set it here before starting */
  p_dev->Speed = speed;

  /* start building the config descriptor common part */
  if (p_dev->classId == 0U)
  {
    /* Add configuration and IAD descriptors */
    USBD_FrameWork_AddConfDesc((uint32_t)p_CmpstConfDesc, &p_dev->CurrConfDescSz);
  }

  switch (p_dev->tclasslist[p_dev->classId].ClassType)
  {

#if USBD_DFU_CLASS_ACTIVATED == 1
    case CLASS_TYPE_DFU:

      /* Find the first available interface slot and Assign number of interfaces */
      interface = USBD_FrameWork_FindFreeIFNbr(p_dev);
      p_dev->tclasslist[p_dev->classId].NumIf  = 1;
      p_dev->tclasslist[p_dev->classId].Ifs[0] = interface;

      /* Assign endpoint numbers */
      p_dev->tclasslist[p_dev->classId].NumEps = 0; /* only EP0 is used */

      /* Configure and Append the Descriptor */
      USBD_FrameWork_DFUDesc(p_dev, (uint32_t)p_CmpstConfDesc, &p_dev->CurrConfDescSz);

      break;
#endif /* USBD_DFU_CLASS_ACTIVATED */

    default:

      break;
  }

  return UX_SUCCESS;
}

/**
  * @brief  USBD_FrameWork_FindFreeIFNbr
  *         Find the first interface available slot
  * @param  pdev: device instance
  * @retval The interface number to be used
  */
static uint8_t USBD_FrameWork_FindFreeIFNbr(USBD_DevClassHandleTypeDef *p_dev)
{
  uint32_t idx = 0U;

  /* Unroll all already activated classes */
  for (uint32_t i = 0U; i < p_dev->NumClasses; i++)
  {
    /* Unroll each class interfaces */
    for (uint32_t j = 0U; j < p_dev->tclasslist[i].NumIf; j++)
    {
      /* Increment the interface counter index */
      idx++;
    }
  }

  /* Return the first available interface slot */
  return (uint8_t)idx;
}

/**
  * @brief  USBD_FrameWork_AddToConfDesc
  *         Add a new class to the configuration descriptor
  * @param  pdev: device instance
  * @retval none
  */
static void  USBD_FrameWork_AddConfDesc(uint32_t conf, uint32_t *p_size)
{
  /* Intermediate variable to comply with MISRA-C Rule 11.3 */
  USBD_ConfigDescTypedef *ptr = (USBD_ConfigDescTypedef *)conf;

  ptr->bLength                = (uint8_t)sizeof(USBD_ConfigDescTypedef);
  ptr->bDescriptorType        = USB_DESC_TYPE_CONFIGURATION;
  ptr->wDescriptorLength      = 0U;
  ptr->bNumInterfaces         = 0U;
  ptr->bConfigurationValue    = 1U;
  ptr->iConfiguration         = USBD_CONFIG_STR_DESC_IDX;
  ptr->bmAttributes           = USBD_CONFIG_BMATTRIBUTES;
  ptr->bMaxPower              = USBD_CONFIG_MAXPOWER;
  *p_size                       += sizeof(USBD_ConfigDescTypedef);
}

/**
  * @brief  USBD_FrameWork_AssignEp
  *         Assign and endpoint
  * @param  p_dev: Pointer to the device instance
  * @param  add: Endpoint address
  * @param  Type: Endpoint type
  * @param  size: Endpoint max packet size
  * @retval none
  */
static void  USBD_FrameWork_AssignEp(USBD_DevClassHandleTypeDef *p_dev,
                                     uint8_t add, uint8_t type, uint32_t size)
{
  uint32_t idx = 0U;

  /* Find the first available endpoint slot */
  while (((idx < (p_dev->tclasslist[p_dev->classId]).NumEps) && \
          ((p_dev->tclasslist[p_dev->classId].Eps[idx].is_used) != 0U)))
  {
    /* Increment the index */
    idx++;
  }

  /* Configure the endpoint */
  p_dev->tclasslist[p_dev->classId].Eps[idx].add = add;
  p_dev->tclasslist[p_dev->classId].Eps[idx].type = type;
  p_dev->tclasslist[p_dev->classId].Eps[idx].size = (uint16_t) size;
  p_dev->tclasslist[p_dev->classId].Eps[idx].is_used = 1U;
}

#if USBD_DFU_CLASS_ACTIVATED
/**
  * @brief  USBD_FrameWork_DFUDesc
  *         Configure and Append the DFU Descriptor
  * @param  p_dev: Pointer to the device instance
  * @param  pconf: Configuration descriptor pointer
  * @param  p_sze: pointer to the current configuration descriptor size
  * @retval None
  */
static void USBD_FrameWork_DFUDesc(USBD_DevClassHandleTypeDef *p_dev, uint32_t pconf,
                                   uint32_t *p_sze)
{
  static USBD_IfDescTypedef        *pIfDesc;
  static USBD_DFUFuncDescTypedef   *pDFUFuncDesc;

  /* Append DFU Interface descriptor to Configuration descriptor */
  __USBD_FRAMEWORK_SET_IF(p_dev->tclasslist[p_dev->classId].Ifs[0], 0U,
                          0U, 0xFEU, 0x01U, 0x02U, 0x06U);

  /* Append DFU Functional descriptor to Configuration descriptor */
  pDFUFuncDesc = ((USBD_DFUFuncDescTypedef *)(pconf + *p_sze));
  pDFUFuncDesc->bLength              = (uint8_t)sizeof(USBD_DFUFuncDescTypedef);
  pDFUFuncDesc->bDescriptorType      = DFU_DESCRIPTOR_TYPE;
  pDFUFuncDesc->bmAttributes         = USBD_DFU_BM_ATTRIBUTES;
  pDFUFuncDesc->wDetachTimeout       = USBD_DFU_DetachTimeout;
  pDFUFuncDesc->wTransferSze         = USBD_DFU_XFER_SIZE;
  pDFUFuncDesc->bcdDFUVersion        = 0x011AU;
  *p_sze                              += (uint32_t)sizeof(USBD_DFUFuncDescTypedef);

  /* Update Config Descriptor and IAD descriptor */
  ((USBD_ConfigDescTypedef *)pconf)->bNumInterfaces             += 1U;
  ((USBD_ConfigDescTypedef *)pconf)->wDescriptorLength           = *p_sze;

  UNUSED(USBD_FrameWork_AssignEp);
}
#endif /* USBD_DFU_CLASS_ACTIVATED */

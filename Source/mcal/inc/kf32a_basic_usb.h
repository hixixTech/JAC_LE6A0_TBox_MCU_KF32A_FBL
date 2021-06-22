/**
  ******************************************************************************
  * �ļ���  kf32a_basic_usb.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��USBģ��(USB)��ع��ܺ�����������غ궨�塣
  *********************************************************************
  */

#ifndef _KF32A_BASIC_USB_H_
#define _KF32A_BASIC_USB_H_

#include "KF32A_BASIC.h"


/**
  * ����  USB������Ϣ�ṹ��
  */
typedef struct
{
    uint32_t m_Speed;                 /* USB�ٶȣ�
                                         ȡֵ�ꡰUSB�ٶ�ѡ���е�һ���� */
    FunctionalState m_Pullup;         /* USBƬ������ʹ�ܣ�
                                         ȡֵΪTRUE��FALSE�� */
    uint32_t m_DoubleBufferCfg;       /* USB˫���������ã�
                                         ȡֵ�ꡰUSB˫���������á��е�һ���� */
    FunctionalState m_DoubleBufferRst;  /* USB˫��������λʹ�ܣ�
                                         ȡֵΪTRUE��FALSE�� */
    uint32_t m_EndPointMask;          /* USB�˵������룬
                                         ȡֵ�ꡰUSB�˵������롱�е�һ��������ϡ� */
    FunctionalState m_Handshake;      /* USB�˵�����ʹ�ܣ�
                                         ȡֵΪTRUE��FALSE�� */
    FunctionalState m_Bidirectional;  /* ˫��˵����ʹ�ܣ�
                                         ȡֵΪTRUE��FALSE�� */
    FunctionalState m_Output;         /* USB�˵����ʹ�ܣ�
                                         ȡֵΪTRUE��FALSE�� */
    FunctionalState m_Input;          /* USB�˵�����ʹ�ܣ�
                                         ȡֵΪTRUE��FALSE�� */
}USB_InitTypeDef;

/**
  * ����  USB��������Ϣ�ṹ��
  */
typedef struct
{
    uint16_t m_EndPoint;                 /* USB�˵��ţ�
                                            ȡֵ�ꡰUSB�˵��š��е�һ���� */
    uint16_t m_Direction;                /* USB�˵㷽��
                                            ȡֵ�ꡰUSB�˵㷽���е�һ���� */
    uint16_t m_BDPointer;                /* USB�˵���żBD�洢����
                                            ȡֵ�ꡰUSB��żBD�洢�����е�һ���� */
    uint16_t m_BufferAddr;               /* USB RAM�����ַ��
                                            ȡֵΪ16λ��Ч��ֵ�� */
    uint32_t m_ByteCount;                /* USB RAM�ֽڼ�����
                                            ȡֵΪ10λ��Ч��ֵ�� */
    uint32_t m_Own;                      /* USB ӵ��λ��
                                            ȡֵ�ꡰUSBӵ��λ���е�һ���� */
    uint32_t m_DataToggleSync;           /* USB���ݷ�תͬ����
                                            ȡֵ�ꡰUSB���ݷ�תͬ��λ���е�һ���� */
    FunctionalState m_DataToggleSyncEn;  /* USB���ݷ�תͬ��ʹ�ܣ�
                                            ȡֵΪTRUE��FALSE�� */
    FunctionalState m_BufferStall;       /* USB������ֹͣʹ�ܣ�
                                            ȡֵΪTRUE��FALSE�� */
}USB_BufferTypeDef;

/**
  * ����  USB״̬��Ϣ�ṹ��
  */
typedef struct
{
    uint8_t m_EndPoint;             /* ��һ����˵㣬
                                       ȡֵ�ꡰUSB�˵��š��е�һ���� */
    uint8_t m_BDDirection;          /* ��һ��BD����
                                       ȡֵ�ꡰUSB���ơ��е�һ���� */
    uint8_t m_BDPointer;            /* ƹ��BDָ�룬
                                       ȡֵ�ꡰUSB��żBD�洢�����е�һ���� */
}USB_StateTypeDef;

/**
  * USB��������ʼ��ַ
  */
#define USB_BUFFER_START_ADDR           ((uint32_t)0x1FFEFC00)
#define USB_BUFFER_END_ADDR             ((uint32_t)(0x1FFEFE00 - 1))
#define CHECK_USB_BUFFER_ADDR(ADDR)     (((uint32_t)(ADDR) - USB_BUFFER_START_ADDR) \
                                       <= (USB_BUFFER_END_ADDR - USB_BUFFER_START_ADDR))

/**
  * USB�ٶ�ѡ��
  */
#define USB_LOW_SPEED                   ((uint32_t)0<<USB_UCON_SPEEDEN_POS)
#define USB_FULL_SPEED                  ((uint32_t)1<<USB_UCON_SPEEDEN_POS)
#define CHECK_USB_SPEED_SELECT(SEL)     (((SEL) == USB_LOW_SPEED) \
                                      || ((SEL) == USB_FULL_SPEED))

/**
  * USB˫����������
  */
#define USB_DOUBLE_BUFFER_FORBID        ((uint32_t)0<<USB_UCON_DB0_POS)
#define USB_OUT_ENDPOINT_0              ((uint32_t)1<<USB_UCON_DB0_POS)
#define USB_ALL_ENDPOINTS               ((uint32_t)2<<USB_UCON_DB0_POS)
#define USB_ENDPOINT_1_TO_7             ((uint32_t)3<<USB_UCON_DB0_POS)
#define CHECK_USB_DOUBLE_BUFFER(EN)     (((EN) == USB_DOUBLE_BUFFER_FORBID) \
                                      || ((EN) == USB_OUT_ENDPOINT_0) \
                                      || ((EN) == USB_ALL_ENDPOINTS) \
                                      || ((EN) == USB_ENDPOINT_1_TO_7))

/**
  * USB�˵���
  */
#define USB_ENDPOINT_0                  ((uint32_t)0)
#define USB_ENDPOINT_1                  ((uint32_t)1)
#define USB_ENDPOINT_2                  ((uint32_t)2)
#define USB_ENDPOINT_3                  ((uint32_t)3)
#define USB_ENDPOINT_4                  ((uint32_t)4)
#define USB_ENDPOINT_5                  ((uint32_t)5)
#define USB_ENDPOINT_6                  ((uint32_t)6)
#define USB_ENDPOINT_7                  ((uint32_t)7)
#define CHECK_USB_ENDPOINT_NUM(NUM)     (((NUM) == USB_ENDPOINT_0) \
                                      || ((NUM) == USB_ENDPOINT_1) \
                                      || ((NUM) == USB_ENDPOINT_2) \
                                      || ((NUM) == USB_ENDPOINT_3) \
                                      || ((NUM) == USB_ENDPOINT_4) \
                                      || ((NUM) == USB_ENDPOINT_5) \
                                      || ((NUM) == USB_ENDPOINT_6) \
                                      || ((NUM) == USB_ENDPOINT_7))

/**
  * USB�˵�������
  */
#define USB_ENDPOINT_MASK_NONE          ((uint32_t)0)
#define USB_ENDPOINT_MASK_0             ((uint32_t)1<<USB_ENDPOINT_0)
#define USB_ENDPOINT_MASK_1             ((uint32_t)1<<USB_ENDPOINT_1)
#define USB_ENDPOINT_MASK_2             ((uint32_t)1<<USB_ENDPOINT_2)
#define USB_ENDPOINT_MASK_3             ((uint32_t)1<<USB_ENDPOINT_3)
#define USB_ENDPOINT_MASK_4             ((uint32_t)1<<USB_ENDPOINT_4)
#define USB_ENDPOINT_MASK_5             ((uint32_t)1<<USB_ENDPOINT_5)
#define USB_ENDPOINT_MASK_6             ((uint32_t)1<<USB_ENDPOINT_6)
#define USB_ENDPOINT_MASK_7             ((uint32_t)1<<USB_ENDPOINT_7)
#define CHECK_USB_ENDPOINT_MASK(NUM)    (((NUM) == USB_ENDPOINT_MASK_NONE) \
                                      || ((NUM) == USB_ENDPOINT_MASK_0) \
                                      || ((NUM) == USB_ENDPOINT_MASK_1) \
                                      || ((NUM) == USB_ENDPOINT_MASK_2) \
                                      || ((NUM) == USB_ENDPOINT_MASK_3) \
                                      || ((NUM) == USB_ENDPOINT_MASK_4) \
                                      || ((NUM) == USB_ENDPOINT_MASK_5) \
                                      || ((NUM) == USB_ENDPOINT_MASK_6) \
                                      || ((NUM) == USB_ENDPOINT_MASK_7))

/**
  * USB�˵㷽��
  */
#define USB_ENDPOINT_OUTPUT             ((uint32_t)0)
#define USB_ENDPOINT_INPUT              ((uint32_t)1)
#define CHECK_USB_ENDPOINT_DIRECT(DIR)  (((DIR) == USB_ENDPOINT_OUTPUT) \
                                      || ((DIR) == USB_ENDPOINT_INPUT))

/**
  * USB����
  */
#define USB_TOKEN_OUT_SETUP             ((uint32_t)0)
#define USB_TOKEN_IN                    ((uint32_t)1)
#define CHECK_USB_TOKEN(DIR)            (((DIR) == USB_TOKEN_OUT_SETUP) \
                                      || ((DIR) == USB_TOKEN_IN))

/**
  * USB��żBD�洢��
  */
#define USB_ODD_BUFFER_DESCRIPOR        ((uint32_t)0)
#define USB_EVEN_BUFFER_DESCRIPOR       ((uint32_t)1)
#define CHECK_USB_ODD_EVEN(SEL)         (((SEL) == USB_ODD_BUFFER_DESCRIPOR) \
                                      || ((SEL) == USB_EVEN_BUFFER_DESCRIPOR))

/**
  * USB��ַ
  */
#define CHECK_USB_ADDRESS(CFG)          (((uint32_t)(CFG) >> 7) == 0)

/**
  * USB�����������ֽڳ���
  */
#define CHECK_USB_DATA_LENGTH(CNT)      (((uint32_t)(CNT) >> 10) == 0)

/**
  * USBӵ��λ
  */
#define USB_KERNEL_OWN_BD               ((uint32_t)0<<USB_BDSTAT_UOWN_POS)
#define USB_SIE_OWN_BD                  ((uint32_t)1<<USB_BDSTAT_UOWN_POS)
#define CHECK_USB_OWN_BD(SEL)           (((SEL) == USB_KERNEL_OWN_BD) \
                                      || ((SEL) == USB_SIE_OWN_BD))

/**
  * USB���ݷ�תͬ��λ
  */
#define USB_DATA_0_TOGGLE_SYNC          ((uint32_t)0<<USB_BDSTAT_DTS_POS)
#define USB_DATA_1_TOGGLE_SYNC          ((uint32_t)1<<USB_BDSTAT_DTS_POS)
#define CHECK_USB_DATA_TOGGLE_SYNC(SEL) (((SEL) == USB_DATA_0_TOGGLE_SYNC) \
                                      || ((SEL) == USB_DATA_1_TOGGLE_SYNC))

/**
  * USB�ж��¼�
  */
#define USB_INT_ERROR                   (USB_UIR_ERRIF)
#define USB_INT_RESET                   (USB_UIR_RSTIF)
#define USB_INT_TRANSACTION_COMPLETED   (USB_UIR_TRNSIF)
#define USB_INT_BUS_ACTIVE              (USB_UIR_ACTVIF)
#define USB_INT_IDLE                    (USB_UIR_IDLEIF)
#define USB_INT_STALL_HANDSHAKE         (USB_UIR_STLIF)
#define USB_INT_SOF_TAKEN               (USB_UIR_SOFIF)
#define USB_INT_DATE_FIELD_NOT_BYTE     (USB_UIR_DFN8EIF)
#define USB_INT_CRC5_HOST_ERROR         (USB_UIR_CRC5EIF)
#define USB_INT_CRC16_FAILURE           (USB_UIR_CRC16EIF)
#define USB_INT_PID_CHECK_FAILURE       (USB_UIR_PIDEIF)
#define USB_INT_BUS_TURNAROUND_TIMEOUT  (USB_UIR_BTOEIF)
#define USB_INT_BIT_STUFF_ERROR         (USB_UIR_BTSEIF)
#define CHECK_USB_INT_ONE_EVENT(EVENT)  (((EVENT) == USB_INT_ERROR) \
                                      || ((EVENT) == USB_INT_RESET) \
                                      || ((EVENT) == USB_INT_TRANSACTION_COMPLETED) \
                                      || ((EVENT) == USB_INT_BUS_ACTIVE) \
                                      || ((EVENT) == USB_INT_IDLE) \
                                      || ((EVENT) == USB_INT_STALL_HANDSHAKE) \
                                      || ((EVENT) == USB_INT_SOF_TAKEN) \
                                      || ((EVENT) == USB_INT_DATE_FIELD_NOT_BYTE) \
                                      || ((EVENT) == USB_INT_CRC5_HOST_ERROR) \
                                      || ((EVENT) == USB_INT_CRC16_FAILURE) \
                                      || ((EVENT) == USB_INT_PID_CHECK_FAILURE) \
                                      || ((EVENT) == USB_INT_BUS_TURNAROUND_TIMEOUT) \
                                      || ((EVENT) == USB_INT_BIT_STUFF_ERROR))
#define CHECK_USB_INT_EVENT(EVENT)      (((EVENT) & (~(USB_INT_ERROR \
                                                    | USB_INT_RESET \
                                                    | USB_INT_TRANSACTION_COMPLETED \
                                                    | USB_INT_BUS_ACTIVE \
                                                    | USB_INT_IDLE \
                                                    | USB_INT_STALL_HANDSHAKE \
                                                    | USB_INT_SOF_TAKEN \
                                                    | USB_INT_DATE_FIELD_NOT_BYTE \
                                                    | USB_INT_CRC5_HOST_ERROR \
                                                    | USB_INT_CRC16_FAILURE \
                                                    | USB_INT_PID_CHECK_FAILURE \
                                                    | USB_INT_BUS_TURNAROUND_TIMEOUT \
                                                    | USB_INT_BIT_STUFF_ERROR))) == 0)

/**
  * USB Debug���ѡ��
  */
#define USB_DBG_DATA_OUTPUT_AND_CLOCK   ((uint32_t)0<<USB_UDB_DEBUG0_POS)
#define USB_DBG_RECEIVE_AND_CLOCK       ((uint32_t)1<<USB_UDB_DEBUG0_POS)
#define USB_DBG_DATA_INPUT_AND_CLOCK    ((uint32_t)2<<USB_UDB_DEBUG0_POS)
#define USB_DBG_RAM_DATA                ((uint32_t)3<<USB_UDB_DEBUG0_POS)
#define USB_DBG_USB_STATE_MACHINE       ((uint32_t)4<<USB_UDB_DEBUG0_POS)
#define USB_DBG_RAM_STATE_MACHINE       ((uint32_t)5<<USB_UDB_DEBUG0_POS)
#define USB_DBG_RAM_ADDRESS             ((uint32_t)6<<USB_UDB_DEBUG0_POS)
#define USB_DBG_OPERATIONAL_CONTROL     ((uint32_t)7<<USB_UDB_DEBUG0_POS)
#define CHECK_USB_DBG_CONFIG(CFG)       (((CFG) == USB_DBG_DATA_OUTPUT_AND_CLOCK) \
                                      || ((CFG) == USB_DBG_RECEIVE_AND_CLOCK) \
                                      || ((CFG) == USB_DBG_DATA_INPUT_AND_CLOCK) \
                                      || ((CFG) == USB_DBG_RAM_DATA) \
                                      || ((CFG) == USB_DBG_USB_STATE_MACHINE) \
                                      || ((CFG) == USB_DBG_RAM_STATE_MACHINE) \
                                      || ((CFG) == USB_DBG_RAM_ADDRESS) \
                                      || ((CFG) == USB_DBG_OPERATIONAL_CONTROL))


#ifdef KF32A_Periph_usb
/* USBģ��(USB)��ʼ����������**************************************/
void USB_Reset(void);
void USB_Configuration(USB_InitTypeDef* usbInitStruct);
void USB_Buffer_Configuration(USB_BufferTypeDef* usbInitStruct);
void USB_Struct_Init (USB_InitTypeDef* usbInitStruct);
void USB_Buffer_Struct_Init (USB_BufferTypeDef* usbInitStruct);
/* USBģ��(USB)�������ú�������************************************/
void USB_Double_Buffer_Pointer_Enable (FunctionalState NewState);
FlagStatus USB_Get_Single_Ended_0_Flag (void);
void USB_Clear_Single_Ended_0_Flag (void);
FlagStatus USB_Get_Package_Transmit_Flag (void);
void USB_Clear_Package_Transmit_Flag (void);
void USB_Suspend_Enable (FunctionalState NewState);
void USB_Resume_Signal_Transmit_Enable (FunctionalState NewState);
void USB_Cmd (FunctionalState NewState);
void USB_Eye_Test_Enable (FunctionalState NewState);
void USB_Speed_Config (uint32_t SpeedSelect);
void USB_On_Chip_Pull_Up_Enable (FunctionalState NewState);
void USB_Double_Buffer_Config (uint32_t DoubleBuffer);
void USB_Get_Arrangement_State (USB_StateTypeDef* usbStateStruct);
void USB_Address_Config (uint32_t Address);
uint32_t USB_Get_Address (void);
uint32_t USB_Get_Frame_Num (void);
void USB_Endpoint_Handshake_Enable (uint32_t EndPoint,
                    FunctionalState NewState);
void USB_Bidirectional_Endpoint_Enable (uint32_t EndPoint,
                    FunctionalState NewState);
void USB_Endpoint_Output_Enable (uint32_t EndPoint, FunctionalState NewState);
void USB_Endpoint_Input_Enable (uint32_t EndPoint, FunctionalState NewState);
FlagStatus USB_Get_Endpoint_STALL_Flag (uint32_t EndPoint);
void USB_Clear_Endpoint_STALL_Flag (uint32_t EndPoint);
void USB_Debug_Signal_Output_Enable (FunctionalState NewState);
void USB_Debug_Output_Config (uint32_t DebugCtl);
/* USBģ��(USB)��������������**************************************/
uint32_t USB_Get_Endpoint_Double_Buffer_Addr (uint32_t EndPoint,
                    uint32_t Direction, uint32_t BufferDescripor);
void USB_Buffer_Address_Config (uint32_t SfrAddr, uint16_t BufferAddr);
uint32_t USB_Get_Buffer_Address (uint32_t SfrAddr);
void USB_Buffer_Data_Length_Config (uint32_t SfrAddr, uint32_t ByteLength);
uint32_t USB_Get_Buffer_Data_Length (uint32_t SfrAddr);
void USB_Own_Config (uint32_t SfrAddr, uint32_t OwnSelect);
FlagStatus USB_Get_Own_Status (uint32_t SfrAddr);
void USB_Data_Toggle_Sync_Config (uint32_t SfrAddr, uint32_t DataPackage);
FlagStatus USB_Get_Data_Toggle_Sync_Status (uint32_t SfrAddr);
void USB_Data_Toggle_Sync_Enable (uint32_t SfrAddr, FunctionalState NewState);
void USB_Buffer_Stall_Enable (uint32_t SfrAddr, FunctionalState NewState);
uint32_t USB_Get_Last_Receive_PID (uint32_t SfrAddr);
void USB_Write_Buffer_Data (uint32_t WriteAddr,
                    uint32_t WriteDataAddr, uint32_t Length);
/* USBģ��(USB)�жϹ���������************************************/
FlagStatus USB_Get_INT_Flag (uint32_t InterruptType);
void USB_Clear_INT_Flag (uint32_t InterruptType);
void USB_Set_INT_Enable (uint32_t InterruptType, FunctionalState NewState);
FlagStatus USB_Get_INT_Enable (uint32_t InterruptType);

#endif //KF32A_Periph_usb
#endif /* _KF32A_BASIC_USB_H */

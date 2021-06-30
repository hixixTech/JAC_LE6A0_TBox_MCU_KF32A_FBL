/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    bl_config_PC.h                                              */
/* Description:  pre-compile parameters                                      */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** �޸���ʷ *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     �»�            ���ߴ���                          */
/*****************************************************************************/


#ifndef BL_CONFIG_PC_H
#define BL_CONFIG_PC_H


/*---------------------------------------------------------------------------*/
/* ͷ�ļ�                                                                    */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* ���Ͷ���                                                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* �궨��                                                                    */
/*---------------------------------------------------------------------------*/
/******************* CRC handling *****************************/
/* --- Choose betweenn CRC calculation algorithms: FAST but takes more memory or slow but less space */
/* undef FAST_CRC if you want the slower but smaller */
#define FAST_CRC
#define CRC32_FAST

/* Init value of CRC32 */
#define CRC32_INIT_VALUE 0x00u

/* TODO WDG_TO - should remove from bl_main and timer.h */
#define WDG_TO              ((UINT16)(100u))
#define MPU_TO              ((UINT16)(2000u))

#define SEND_WAIT_BEFORE_ERASE(addr, size)                      \
    do{                                                         \
        /* Max erase time: 3.6 sec for one sector */            \
        /* a wait needed */                                     \
        (void)ApiDlSendNegativeResponse((UINT16)responsePending);   \
                                                                \
    }while(0)

#define START_TABLE_ADDR_APP      (0x00038000uL)       /*APP����ʼ��ַ*/
#define START_TABLE_ADDR_EOL      (0x00018000uL)       /*EOL����ʼ��ַ*/
#define END_TABLE_ADDR_APP        (0x00100000uL)       /*APP�Ľ�����ַ*/

#define VAL_TABLE_ADDR_APP      (0x000FFFE4uL)       /*APP����Ч�ԣ��洢app��ʼ��ַ�ĵ�ַ*/
#define VAL_TABLE_ADDR_EOL      (0x00037FE4uL)       /*EOL����Ч�ԣ�eol��ʼ��ַ*/

#define COM_TABLE_ADDR_APP      (0x000FFFE0uL)       /*APP�ļ����ԣ��洢app��fbl�ļ��ݵ�ַ*/
#define COM_TABLE_ADDR_APP2     (0x000FFFDCuL)       /*�洢app��eol�ļ��ݵ�ַ*/
#define COM_TABLE_ADDR_EOL      (0x00037FE0uL)       /*eol��app�ļ����Ե�ַ*/

#define APP_FBL      (0xA100u)       /*app��fbl�ļ�����ֵ*/
#define APP_EOL      (0xA100u)       /*app��eol�ļ�����ֵ*/
#define EOL_APP      (0xA100u)       /*eol��app�ļ�����ֵ*/

#define CRC_OFFSET   (8u)      /*crcУ��ƫ�Ƶ�ַ*/

#define TABLE_NUM    (2U)      /*ˢ�����ݿ�*/

#define VAL_TABLE_MEMTYPE_COD   INTFLASH
#define VAL_TABLE_MEMTYPE_CFG   INTFLASH
#define VAL_TABLE_MEMTYPE_BT    4U            /*������ȡͨ��uart�����Ͳ�һ��*/

// TODO:�����Ժͼ�����״̬��������SIS ID��0x0014��Byte1����������Byte2����Ч��Byte3
#define APP_SIS_MEMTYPE         INT_EEPROM
#define APP_SIS_ADDR            (5u)  /*Index of EEL as defined in RFQ_SAIC_BL_042*/
#define APP_SCS_MEMTYPE         INT_EEPROM
#define APP_SCS_ADDR            (6u) /*Index of EEL as defined in RFQ_SAIC_BL_042*/

#define SVN_MEMTYPE             INT_EEPROM
#define SVN_ADDR                (0x2Au)
#define SVN_ERASED              0xFFFFU//(0xFFFFFFFFuL)

/* MCU is little endian */
#define ENDIAN_CONV32(value)     ((value) = (UINT32)(((0x000000FFuL & (value)) << 24) | ((0x0000FF00uL & (value)) << 8 )| \
                                                     ((0x00FF0000uL & (value)) >> 8 ) | ((0xFF000000uL & (value)) >> 24) ) )


/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �ⲿ������������                                                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* �ⲿ������������                                                          */
/*---------------------------------------------------------------------------*/



#endif /*BL_CONFIG_PC_H*/

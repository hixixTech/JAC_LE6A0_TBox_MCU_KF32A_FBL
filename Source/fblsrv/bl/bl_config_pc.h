/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    bl_config_PC.h                                              */
/* Description:  pre-compile parameters                                      */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠            基线创建                          */
/*****************************************************************************/


#ifndef BL_CONFIG_PC_H
#define BL_CONFIG_PC_H


/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
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

#define START_TABLE_ADDR_APP      (0x00038000uL)       /*APP的起始地址*/
#define START_TABLE_ADDR_EOL      (0x00018000uL)       /*EOL的起始地址*/
#define END_TABLE_ADDR_APP        (0x00100000uL)       /*APP的结束地址*/

#define VAL_TABLE_ADDR_APP      (0x000FFFE4uL)       /*APP的有效性，存储app起始地址的地址*/
#define VAL_TABLE_ADDR_EOL      (0x00037FE4uL)       /*EOL的有效性，eol起始地址*/

#define COM_TABLE_ADDR_APP      (0x000FFFE0uL)       /*APP的兼容性，存储app和fbl的兼容地址*/
#define COM_TABLE_ADDR_APP2     (0x000FFFDCuL)       /*存储app和eol的兼容地址*/
#define COM_TABLE_ADDR_EOL      (0x00037FE0uL)       /*eol和app的兼容性地址*/

#define APP_FBL      (0xA100u)       /*app和fbl的兼容性值*/
#define APP_EOL      (0xA100u)       /*app和eol的兼容性值*/
#define EOL_APP      (0xA100u)       /*eol和app的兼容性值*/

#define CRC_OFFSET   (8u)      /*crc校验偏移地址*/

#define TABLE_NUM    (2U)      /*刷新数据块*/

#define VAL_TABLE_MEMTYPE_COD   INTFLASH
#define VAL_TABLE_MEMTYPE_CFG   INTFLASH
#define VAL_TABLE_MEMTYPE_BT    4U            /*蓝牙读取通过uart，类型不一样*/

// TODO:完整性和兼容性状态，完整性SIS ID是0x0014，Byte1，兼容性是Byte2，有效性Byte3
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
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开变量申明                                                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 外部公开函数声明                                                          */
/*---------------------------------------------------------------------------*/



#endif /*BL_CONFIG_PC_H*/

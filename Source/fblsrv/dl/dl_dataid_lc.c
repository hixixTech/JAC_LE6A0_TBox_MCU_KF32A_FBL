/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    bl_dataid_LC.c                                              */
/* Description:  Linktime configuration for Memory Layer interface           */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠        基线创建                              */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "dl_dataid.h"
#include "dl_config_pc.h" /* get SA_CONST_ */
#include "bl_config_pc.h" /* get CAN_CFG_X */
#include "type.h"

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define REP_DID     INTFLASH
#define WRITE_ONCE_DID(n)               (write_once | ((n)<<4))   /* Highest 4 bit shows flag*/
#define WRITE_ONCE_AFTER_PROG_DID(n)    (write_once_after_prog | ((n)<<4)) /* uses RAM flag */

/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/
__attribute__((section (".F183")))
UINT8 au8F183FBLRefNum[10]  = {'T','B','O','X','S','8','1','1','0','2'};

/********************** Mandatory DID configuration ***************************/
/* Only memtype and address parameters of standard DIDs are configurable      */
static DidProp_t tStdId[] =
{
/*   did     memtype  address                   size readability  writability */
//{0xB301u, INT_EEPROM, CAN_CFG_ADDR,             8, true,   WRITE_ONCE_DID(0)},
//{0xB302u, INT_EEPROM, SA_CONST_ADDR,            4, false,  WRITE_ONCE_DID(1)},
//{0xB303u, INT_EEPROM, RESET_SOURCE_ADDR,        1, true,   read_only},
{0xF012u, INT_EEPROM, 0X0017,        2, true,   read_only},
{0xF183u, INTFLASH,   0x00017FF0,         10, true,   read_only},
{0xF186u, Invalid,   (UINT32)0x000000uL,      1u, true,   false},/*Active Diagnostic Session Data Identifier */
{0xF18Cu, INT_EEPROM, 0x0007,                      26, true,    read_only},
{0xF192u, INT_EEPROM, 0x0009,                      15, true,    read_only},
{0xF198u, INT_EEPROM, 0x000E,                      3, true,    ext_sec_level},
{0xF199u, INT_EEPROM, 0X000F,                       4, true,    ext_sec_level},
{0xF1FAu, INT_EEPROM, 0X0017,		   1, true,    read_only},
{0xF1FBu, INT_EEPROM, 0X0017,          1, true,    read_only},
{0xF1FCu, INT_EEPROM, 0X0017,          1, true,    read_only}
};


/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/
Id_t tStdIdStr =
{
/*  size                             id */
    sizeof(tStdId)/sizeof(DidProp_t), tStdId
};

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/



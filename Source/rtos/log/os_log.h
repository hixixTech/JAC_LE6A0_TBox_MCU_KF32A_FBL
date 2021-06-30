/*****************************************************************************/
/* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    os_log.h                                                    */
/* Description:  RTOS的LOG功能模块                                           */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author           Description                */
/* 2018-09-10   V11.01.01.01.00  吴辰雨              新建基线                */
/*****************************************************************************/

#ifndef __OS_LOG_H__
#define __OS_LOG_H__
/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "ecu_usart.h"
#include "type.h"

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define OS_LOG_ENABLE            1
#define OS_LOG_DISABLE             0

/* LOG种类定义 */
#define _LOG_CRITICAL         0
#define _LOG_ERROR            1
#define _LOG_WARNING        2
#define _LOG_DEBUG            3
#define _LOG_ENTRY            4
#define _LOG_EXIT            5
#define _LOG_TRACE            6


#define LOG_MAX_LEN         2048u
#define LOG_MPU_MAX_LEN     512u
#define LOG_BYTES_PER_CYCLE 120u
/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/
typedef struct{
    UINT8 u8LogState;
//    UART_BANDRATE eLogBaudrate;
    
}LogCfg_t;

typedef struct{
    CHAR data[LOG_MAX_LEN];
    UINT16 u16ReadPointer;
    UINT16 u16WritePointer;
    UINT16 u16DataCnt;
}LogBuff_t;
/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 全局函数定义                                                              */
/*---------------------------------------------------------------------------*/
extern void ApiLogInit(void);

extern void ApiLogEnable(void);

extern void ApiLogDisable(void);

extern void ApiLogPrint(UINT8 u8Logtype, const CHAR* pcFormat, ...);

extern void ApiLogPrintExt(UINT8 u8Print, UINT8 u8Logtype, const CHAR * pcFormat, ...);

extern void ApiLogProcess(void);

extern BOOL ApiLogIsEmpty(void);
#endif

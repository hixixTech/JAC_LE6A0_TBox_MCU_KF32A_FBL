/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    bl_app.h                                                    */
/* Description:  Header for Application validation                           */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠            基线创建                          */
/*****************************************************************************/

#ifndef BL_APP_H_
#define BL_APP_H_

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "type.h"


/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define DATA_0xFF_4bytes       0xFF, 0xFF, 0xFF, 0xFF                                                                   //0xFF      * 4
#define DATA_0xFF_8bytes       DATA_0xFF_4bytes , DATA_0xFF_4bytes                                                      //4bytes    * 2
#define DATA_0xFF_64bytes      DATA_0xFF_8bytes , DATA_0xFF_8bytes , DATA_0xFF_8bytes , DATA_0xFF_8bytes , DATA_0xFF_8bytes , DATA_0xFF_8bytes , DATA_0xFF_8bytes , DATA_0xFF_8bytes  //8bytes * 8
#define DATA_0xFF_256bytes     DATA_0xFF_64bytes , DATA_0xFF_64bytes , DATA_0xFF_64bytes , DATA_0xFF_64bytes

/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开变量申明                                                              */
/*---------------------------------------------------------------------------*/
extern UINT8 gu8InvalidateFlag;


/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开函数声明                                                          */
/*---------------------------------------------------------------------------*/
extern bool ApiApplIntegrityChk(void);
extern void ApiApplCompatibilityChk(void);
extern void ApiApplicationStart(void);
extern void ApiApplicationInvalidate(void);
extern void ApiSetInvalidationTarget(void);
extern UINT8 ApiGetSwCompatibilityStatus(void);
extern UINT8 ApiGetSwIntegrityStatus(void);
extern bool ApiIsProgrammingStarted(void);
extern bool ApiApplicationValidate(void);


#endif /* BL_APP_H_ */

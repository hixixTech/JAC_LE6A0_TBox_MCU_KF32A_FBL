/******************************************************************************/
/*                                                                            */
/*               Copyright Mentor Graphics Corporation 2008-2013              */
/*                         All Rights Reserved.                               */
/*                                                                            */
/* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS       */
/* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS        */
/* SUBJECT TO LICENSE TERMS.                                                  */
/*                                                                            */
/******************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.           */
/* File Name:    dl_services.h                                                */
/* Description:  Header for Diagnostic Services                               */
/*                                                                            */
/* Others:       other description                                            */
/*                                                                            */
/* Processor:    RH850F1L                                                     */
/* Compiler:     GHS_201517                                                   */
/********************************** 修改历史 **********************************/
/* Date            Version       Author      Description                      */
/* 2019-07-05   V11.01.01.00     陈惠            基线创建                           */
/******************************************************************************/

#ifndef DL_SERVICE_H_
#define DL_SERVICE_H_

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
#define SessionTypeMask         ((UINT8)0x7fu)
#define SubFunctionMask         ((UINT8)0x7fu)
#define ResRequiredMask         ((UINT8)0x80u)

#define hardReset               ((UINT8)0x01u)

#define requestSeed             ((UINT8)0x05u)
#define sendKey                 ((UINT8)0x06u)

#define startRoutine            ((UINT8)0x01u)
#define stopRoutine             ((UINT8)0x02u)
#define routineTypeResp         ((UINT8)0x03u)


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
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开函数声明                                                          */
/*---------------------------------------------------------------------------*/
extern void ApiDlTransferdatadl(UINT8 *pu8Data, UINT16 u16Length);
extern void ApiDlTransferdataMore(void);
extern void ApiDlTransferdatadlBg(void);
extern bool ApiDlTransferdataAbort(void);

extern void ApiDlRequestdl(const UINT8 *pu8Data);
extern void ApiDlRequestdlBg(void);
extern bool ApiDlRequestdlAbort(void);
extern void ApiDlGetRDLParam(UINT32* pu32MemoryAddress, UINT32* pu32MemorySize, MemType_e* peMemType);
extern RoutineStatus_e ApiDlGetTDFlowStatus(void);

extern void ApiDlTransferExit(UINT8 u8CrcType);
extern void ApiDlTransferExitBg(UINT8 *pu8data);
extern bool ApiDlTransferExitAbort(void);

extern void ApiDlRdbyid(const UINT8 *pu8Data, UINT16 u16Length);

extern void ApiDlWdbyid(UINT8 *pu8Data, UINT16 u16Length);
extern void ApiDlWdbyidBg(void);
extern bool ApiDlWdbyidAbort(void);

extern void ApiDlRoutineControl(const UINT8 *pu8Data,UINT16 u16Length);
extern void ApiDlRoutineControlEmBg(void);
extern void ApiDlRoutineControlCpdiBg(void);
extern void ApiDlRoutineControlCheckProgDepBg(void);
extern void ApiDlRoutineControlCheckProgIntBg(void);
extern bool ApiDlRoutineControlCheckProgIntAbort(void);
extern bool ApiDlRoutineControlCheckProgDepAbort(void);
extern bool ApiDlRoutineControlEmAbort(void);
extern bool ApiDlRoutineControlCpdiAbort(void);

extern void ApiDlTesterPresent(const UINT8 *pu8Data);

extern void ApiDlDsc(const UINT8 *pu8Data);
extern void ApiDlDscBg(void);
extern bool ApiDlDscAbort(void);
extern bool ApiDlDscEnterProgSession(UINT8 u8ResSuppress);

extern void ApiDlEcuReset(const UINT8 *pu8Data);
extern void ApiDlEcuResetBg(void);
extern bool ApiDlEcuResetAbort(void);

extern void ApiDlSecurityAccess(const UINT8 *pu8Data,UINT16 u16Length);

#endif /* DL_SERVICE_H_ */

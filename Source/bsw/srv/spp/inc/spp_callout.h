/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     spp_callout.h                                         */
/* Author:        Jianzhou.Zhang         Version:  v11.01.01.01.00        Data£º 2020-01-03*/
/* Description:   call back function define                                  */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:     RH850F1L                                                   */
/* Compiler:      GHS_201517                                                 */
/********************************** ÐÞ¸ÄÀúÊ· *********************************/
/* Date             Version    Author            Description                 */
/* 2020-01-03       v11.01.01.01.00         Jianzhou.Zhang              baseline    */
/*****************************************************************************/




#ifndef SPP_CALLOUT_H
#define SPP_CALLOUT_H

#include "spp_main.h"
#include "spp_type.h"

#define TIME_STEP   (10u)

#define PROCESS_TIMES_ONE_CYCLE (8)

UINT32 SppGetMpuBuffAddr(void);

extern SPPErrorCode_e SppHalInit(void);

extern SPPErrorCode_e SppHalDeInit(void);


extern SPPErrorCode_e SppHalOpen(void);


extern SPPErrorCode_e SppHalClose(void);

extern SPPErrorCode_e SppHalSendData(UINT8* pu8Tx, INT16 len);


extern SPPErrorCode_e SppSendReady(void);

UINT16 SppHalRecvDataNew(UINT8* pu8RecvData, UINT16 length);

/******************************************************************************
*  function name | SppHalRecvData
*  content       | receive data from mpu
*  parameter[1]  | recv_data :des address 
*  parameter[2]  | length :data length
*  notice        | 
*  return        | data received length
******************************************************************************/
extern INT16 SppHalRecvData(UINT8* pu8RecvData, UINT16 length);

//extern void SppEnterCritical(void);
/* by s811 */
//extern void SppLeaveCritical(void);

/*
extern INT16 SppHalRecvDataBB(SppConnectType_e eConnectType,UINT8* recv_data, UINT16 length); 
*/


#endif //MCU_SPP_H


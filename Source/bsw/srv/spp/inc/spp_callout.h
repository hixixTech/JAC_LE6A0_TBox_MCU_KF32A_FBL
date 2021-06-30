/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     dias_spp_callout.h                                         */
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




#ifndef DIAS_SPP_CALLOUT_H
#define DIAS_SPP_CALLOUT_H


#include "spp_type.h"

#define TIME_STEP   (1u)

extern DiasErrorCode_e DiasSppHalInit(void);

extern DiasErrorCode_e DiasSppHalDeInit(void);


extern DiasErrorCode_e DiasHalOpen(void);


extern DiasErrorCode_e DiasSppHalClose(void);

extern void DiasSppInitBuffer(void);


extern DiasErrorCode_e DiasSppHalSendData(UINT8* tx, INT16 len);

/******************************************************************************
*  function name | DiasSppHalRecvData
*  content       | receive data from mpu
*  parameter[1]  | recv_data :des address 
*  parameter[2]  | length :data length
*  notice        | 
*  return        | data received length
******************************************************************************/
extern INT16 DiasSppHalRecvData(UINT8* recv_data, uint16_t length);

extern void DiasSppEnterCritical(void);

extern void DiasSppLeaveCritical(void);



#endif //MCU_SPP_H


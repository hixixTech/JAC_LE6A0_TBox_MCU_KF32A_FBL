/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     spp_app.h                                             */
/* Author:        Jianzhou.Zhang         Version:  v11.01.01.01.00        Data£º 2020-01-06*/
/* Description:   spp_interface                                              */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:     RH850F1L                                                   */
/* Compiler:      GHS_201517                                                 */
/********************************** ÐÞ¸ÄÀúÊ· *********************************/
/* Date             Version    Author            Description                 */
/* 2020-01-06       v11.01.01.01.00         Jianzhou.Zhang              baseline    */
/*****************************************************************************/

#ifndef SPP_APP_H
#define SPP_APP_H



#include "spp_main.h"
#include "spp_generated.h"
#include "spp_type.h"
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif










#pragma pack()
typedef struct
{
    INT16 s16Major;  
    INT16 s16Minor;  
    INT16 s16Patch;  
}SppVersion_t;


typedef struct
{
    SppVersion_t tSysVersion;  
    SppVersion_t tUserVersion;
    INT8 as8md5[9];             
}SppVersionEx_t;




extern void ApiSPPSetListener(UINT16 u16EventId, void* pListener);
extern void ApiSPPSetAckListener(UINT16 u16EventId, void* pListener);

BOOL ApiSPPAddEvent(UINT16 u16EventId, UINT8 * pucData, UINT16 u16DataLen);
extern BOOL ApiSPPTestFlag(UINT16 u16EventId);
extern void ApiSPPClearFlag(UINT16 u16EventId);
extern void ApiSPPSetRecBuff(UINT16 u16_eventID,UINT8* tpu8_recBuff,UINT16 u16_len);
extern UINT8* ApiSPPGetRecvEventData(UINT16 u16EventId);




SPPErrorCode_e ApiSPPInit(void);
SPPErrorCode_e ApiSPPMainTask(void);

SPPErrorCode_e ApiSPPStart(void);

SPPErrorCode_e ApiSPPStop(void);

SPPErrorCode_e ApiSPPSleep(void);

SPPErrorCode_e ApiSPPWakeup(void);
SPPErrorCode_e ApiSPPDeInit(void);
SppState_e ApiSPPGetState(void);
void ApiSppGetVersionEx(SppVersionEx_t* tSppVersion);

#ifdef __cplusplus
}
#endif


#endif //MCU_SPP_H


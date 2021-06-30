/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     dias_spp_app.h                                             */
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

#ifndef DIAS_SPP_APP_H
#define DIAS_SPP_APP_H


#include "sppc_core.h"
#include "sppc_core_types.h"
#include "spp_generated.h"
#include "spp_type.h"
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

#if 0
typedef enum {
	SPP_USER_SEND_DCPD_MSG_TO_MPU_TCALL_CFG_RESP_EVT = SPP_USER_EVENT,
	SPP_USER_SEND_EVENT_MAX
} ESendEvent;

typedef enum {
	SPP_USER_RECV_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_EVT = SPP_USER_EVENT,
	SPP_USER_RECV_EVENT_MAX
} ERecvEvent;
#endif








#pragma pack()




/**
 *  Set log level
 */
//SPP_BOOL DIAS_SPP_SetLogLevel(SPP_LOGLEVEL ll);

/**
 *  Invoked when logging
 */
//SPP_BOOL DIAS_SPP_SetLoggingListener(SPP_Logging_Callback cb);

/**
 *  Invoked when an event is acknowledged
 */
//SPP_BOOL DIAS_SPP_Add_EventAck_Listener(SPP_UInt16 event_id, SPP_UserEventAck_Callback cb);

/**
 *  Invoked when an event is acknowledged error
 */
//SPP_BOOL DIAS_SPP_Add_EventAckError_Listener(SPP_UInt16 event_id, SPP_UserEventAckError_Callback cb);

/**
 *  Invoked when any event is received
 */
//SPP_BOOL DIAS_SPP_Add_EventRecv_Listener(SPP_EventRecv_Callback cb);

extern void sppc_SetListener(SPP_UInt16 event_id, void* listener);
SPP_STATUS sppc_AddEvent(SPP_UInt16 event_id, UINT8 * pucData, UINT16 u16DataLen);


///**
// *  Receive Event Interface
// */
//typedef void (*DIAS_SPP_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_Callback)(SPP_UInt16 event_id, DcpdPackageHeader_t* data);
//SPP_STATUS DIAS_SPP_Add_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_Event_Listener(DIAS_SPP_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_Callback cb);
//SPP_STATUS DIAS_SPP_Recv_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_Event(DcpdPackageHeader_t* data);
//
//typedef void (*DIAS_SPP_DCPD_MSG_TO_MCU_IS_SLEEP_READY_Callback)(SPP_UInt16 event_id, DcpdPackageToMcuSleepReady_t* data);
//SPP_STATUS DIAS_SPP_Add_DCPD_MSG_TO_MCU_IS_SLEEP_READY_Event_Listener(DIAS_SPP_DCPD_MSG_TO_MCU_IS_SLEEP_READY_Callback cb);
////SPP_STATUS DIAS_SPP_Recv_DCPD_MSG_TO_MCU_IS_SLEEP_READY_Event(DcpdPackageToMcuSleepReady_t* data);
//
//
//
//
//
//typedef void (*DIAS_SPP_DCPD_MSG_TO_MCU_AC_CFG_SAVE_REQ_Callback)(SPP_UInt16 event_id, DcpdPackageToMcuCfgSaveReq_t* data);
//SPP_STATUS DIAS_SPP_Add_DCPD_MSG_TO_MCU_AC_CFG_SAVE_REQ_Event_Listener(DIAS_SPP_DCPD_MSG_TO_MCU_AC_CFG_SAVE_REQ_Callback cb);
////SPP_STATUS DIAS_SPP_Recv_DCPD_MSG_TO_MCU_AC_CFG_SAVE_REQ_Event(DcpdPackageToMcuCfgSaveReq_t* data);
//
//SPP_BOOL DIAS_SPP_Send_DCPD_MSG_TO_MPU_AC_CFG_SAVE_RESP_Event(const DcpdPackageToMpuAcCfgSaveResult_t* data);
//
//SPP_BOOL DIAS_SPP_Send_DCPD_MSG_TO_MPU_AC_INI_CFG_RESP_Event(const DcpdPackageToMpuAcCfgIniData_t* data);
//
//
///**
// *  Send Event Interface
// */
//
//SPP_BOOL DIAS_SPP_Send_DCPD_MSG_TO_MPU_TCALL_CFG_RESP_Event(const DcpdPackageToMputCallCfg_t* data);
//


DiasErrorCode_e DIAS_SPP_Init(void);
DiasErrorCode_e DIAS_SPP_Main_Task(void);

DiasErrorCode_e DIAS_SPP_Start(void);

DiasErrorCode_e DIAS_SPP_Stop(void);

DiasErrorCode_e DIAS_SPP_Sleep(void);

DiasErrorCode_e DIAS_SPP_Wakeup(void);
DiasErrorCode_e DIAS_SPP_DeInit(void);
SppState_ENUM DIAS_SPP_GetState(void);

#ifdef __cplusplus
}
#endif


#endif //MCU_SPP_H


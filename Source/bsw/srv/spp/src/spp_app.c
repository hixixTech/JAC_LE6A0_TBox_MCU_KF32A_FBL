/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     dias_spp_app.c                                             */
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




#include "string.h"
#include "spp_main.h"
#include "spp_generated.h"
#include "spp_app.h"



void sppc_SetListener(SPP_UInt16 event_id, void* listener)
{
    (VOID)DiasSppSetListener(SPP_CONNECT_TYPE_MCU_4G,event_id,(pDiasSppListener)listener);    
}
SPP_STATUS sppc_AddEvent(SPP_UInt16 event_id, UINT8 * pucData, UINT16 u16DataLen)
{
    DiasSppSend(SPP_CONNECT_TYPE_MCU_4G,event_id,pucData,u16DataLen);
    return TRUE;
}


#if 0
SPP_BOOL DIAS_SPP_Add_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_Event_Listener(DIAS_SPP_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_Callback cb)
{
	//sppc_SetListener(SPP_USER_RECV_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_EVT, cb);
    pDiasSppListener p_fuc = (pDiasSppListener)(void*)cb;
    (VOID)DiasSppSetListener(SPP_CONNECT_TYPE_MCU_4G,SPP_USER_RECV_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_EVT,p_fuc);    
	return SPP_TRUE;
}




SPP_BOOL DIAS_SPP_Recv_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_Event(DcpdPackageHeader_t* data)
{
//	SPP_Int8* pbuf = sppc_GetRecvEventData(SPP_USER_RECV_DCPD_MSG_TO_MCU_TCALL_CFG_REQ_EVT);
    //(VOID)DiasSppSend(SPP_CONNECT_TYPE_MCU_4G, (UINT16)SPP_USER_SEND_DCPD_MSG_TO_MPU_TCALL_CFG_RESP_EVT, (UINT8 *)data, sizeof(DcpdPackageHeader_t));
	return SPP_TRUE;
}

SPP_BOOL DIAS_SPP_Send_DCPD_MSG_TO_MPU_TCALL_CFG_RESP_Event(const DcpdPackageToMputCallCfg_t* data)
{    
    (VOID)DiasSppSend(SPP_CONNECT_TYPE_MCU_4G, (UINT16)SPP_USER_SEND_DCPD_MSG_TO_MPU_TCALL_CFG_RESP_EVT, (UINT8 *)data, sizeof(DcpdPackageToMputCallCfg_t));
	return SPP_TRUE;
}



SPP_STATUS DIAS_SPP_Add_DCPD_MSG_TO_MCU_IS_SLEEP_READY_Event_Listener(DIAS_SPP_DCPD_MSG_TO_MCU_IS_SLEEP_READY_Callback cb)
{
    pDiasSppListener p_fuc = (pDiasSppListener)(void*)cb;
    (VOID)DiasSppSetListener(SPP_CONNECT_TYPE_MCU_4G,SPP_USER_RECV_DCPD_MSG_TO_MCU_IS_SLEEP_READY_EVT,p_fuc);    
}
SPP_STATUS DIAS_SPP_Recv_DCPD_MSG_TO_MCU_IS_SLEEP_READY_Event(DcpdPackageToMcuSleepReady_t* data)
{
	return SPP_TRUE;
}



SPP_STATUS DIAS_SPP_Add_DCPD_MSG_TO_MCU_AC_CFG_SAVE_REQ_Event_Listener(DIAS_SPP_DCPD_MSG_TO_MCU_AC_CFG_SAVE_REQ_Callback cb)
{
    pDiasSppListener p_fuc = (pDiasSppListener)(void*)cb;
    (VOID)DiasSppSetListener(SPP_CONNECT_TYPE_MCU_4G,SPP_USER_RECV_DCPD_MSG_TO_MCU_AC_CFG_SAVE_REQ_EVT,p_fuc);    
	return SPP_TRUE;
}
//SPP_STATUS DIAS_SPP_Recv_DCPD_MSG_TO_MCU_AC_CFG_SAVE_REQ_Event(DcpdPackageToMcuCfgSaveReq_t* data);

SPP_BOOL DIAS_SPP_Send_DCPD_MSG_TO_MPU_AC_CFG_SAVE_RESP_Event(const DcpdPackageToMpuAcCfgSaveResult_t* data)
{
    (VOID)DiasSppSend(SPP_CONNECT_TYPE_MCU_4G, (UINT16)SPP_USER_SEND_DCPD_MSG_TO_MPU_AC_CFG_SAVE_RESP_EVT, (UINT8 *)data, sizeof(DcpdPackageToMpuAcCfgSaveResult_t));
	return SPP_TRUE;
}

SPP_BOOL DIAS_SPP_Send_DCPD_MSG_TO_MPU_AC_INI_CFG_RESP_Event(const DcpdPackageToMpuAcCfgIniData_t* data)
{
    (VOID)DiasSppSend(SPP_CONNECT_TYPE_MCU_4G, (UINT16)SPP_USER_SEND_DCPD_MSG_TO_MPU_AC_INI_CFG_RESP_EVT, (UINT8 *)data, sizeof(DcpdPackageToMpuAcCfgIniData_t));
	return SPP_TRUE;
}

#endif



DiasErrorCode_e DIAS_SPP_Init(void)
{
	return DiasSppInitForAll();	
}

DiasErrorCode_e DIAS_SPP_Main_Task(void)
{
	return DiasSppMainTask(SPP_CONNECT_TYPE_MCU_4G);
}

DiasErrorCode_e DIAS_SPP_Start(void)
{
    return DiasSppInit(SPP_CONNECT_TYPE_MCU_4G) ;
}

DiasErrorCode_e DIAS_SPP_Stop(void)
{
    return DiasSppStop(SPP_CONNECT_TYPE_MCU_4G);
}

DiasErrorCode_e DIAS_SPP_Sleep(void)
{
	return DIAS_SUCCESS;
}

DiasErrorCode_e DIAS_SPP_Wakeup(void)
{
	return DIAS_SUCCESS;
}

#if 0

SPP_BOOL DIAS_SPP_SetLogLevel(SPP_LOGLEVEL ll)
{
	return sppc_SetLogLevel(ll);
}

SPP_BOOL DIAS_SPP_SetLoggingListener(SPP_Logging_Callback cb)
{
	return sppc_Add_Logging_Listener(cb);
}

SPP_BOOL DIAS_SPP_Add_EventAck_Listener(SPP_UInt16 event_id, SPP_UserEventAck_Callback cb)
{
	sppc_Add_EventAck_Listener(event_id, cb);
	return SPP_TRUE;
}

SPP_BOOL DIAS_SPP_Add_EventAckError_Listener(SPP_UInt16 event_id, SPP_UserEventAckError_Callback cb)
{
	sppc_Add_EventAckError_Listener(event_id, cb);
	return SPP_TRUE;
}

SPP_BOOL DIAS_SPP_Add_EventRecv_Listener(SPP_EventRecv_Callback cb)
{
	sppc_Add_EventRecv_Listener(cb);
	return SPP_TRUE;
}




SPP_BOOL DIAS_SPP_GetVersionEx(SppVersionEx* spp_version)
{
	const SppVersion* system_version = sppc_GetVersion();
	spp_version->sys_version = *system_version;
	spp_version->user_version = event_version;
	memcpy(spp_version->md5, spp_md5, 8);

	return SPP_TRUE;
}


SPP_BOOL DIAS_SPP_GetVersion(SppVersion* spp_version)
{
	const SppVersion* system_version = sppc_GetVersion();
	*spp_version = *system_version;

	return SPP_TRUE;
}
#endif

DiasErrorCode_e DIAS_SPP_DeInit(void)
{
    return DiasSppDeInit(SPP_CONNECT_TYPE_MCU_4G);
}

SppState_ENUM DIAS_SPP_GetState(void)
{
    SppState_ENUM e [6] = 
    {
        SPP_STATE_UNKNOW ,
        SPP_STATE_INIT,
        SPP_STATE_RUNNING,
        SPP_STATE_WARNING,
        SPP_STATE_ERROR,
        SPP_STATE_SLEEP
    };
    return e[(UINT8)DiasSppGetStatus(SPP_CONNECT_TYPE_MCU_4G)];
}


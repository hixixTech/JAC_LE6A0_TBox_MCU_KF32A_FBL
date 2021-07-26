/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     spp_app.c                                             */
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
#include "spp_app.h"



/******************************************************************************
*  function name | ApiSPPSetListener
*  content       | add receive event callback function
*  parameter[1]  | u16EventId :event id
*  parameter[2]  | pListener :receive callback function
*  notice        | 
*  return        | 
******************************************************************************/
void ApiSPPSetListener(UINT16 u16EventId, void* pListener)
{
    (VOID)SppSetListener(SPP_CONNECT_TYPE_MCU_4G,u16EventId,(pfSppListener)pListener);    
}


/******************************************************************************
*  function name | ApiSPPSetAckListener
*  content       | add receive ack callback function
*  parameter[1]  | u16EventId :send event id
*  parameter[2]  | pListener :receive ack
*  notice        | 
*  return        | 
******************************************************************************/
void ApiSPPSetAckListener(UINT16 u16EventId, void* pListener)
{
    SppSetAckListener(SPP_CONNECT_TYPE_MCU_4G,u16EventId,(pfSppAckListener)pListener);
}


/******************************************************************************
*  function name | ApiSPPAddEvent
*  content       | send event
*  parameter[1]  | u16EventId :event id
*  parameter[2]  | pucData :event data
*  parameter[3]  | u16DataLen :data length
*  notice        | 
*  return        | 
******************************************************************************/
BOOL ApiSPPAddEvent(UINT16 u16EventId, UINT8 * pucData, UINT16 u16DataLen)
{
    SPPErrorCode_e eSppRtv;
    eSppRtv = SppSend(SPP_CONNECT_TYPE_MCU_4G,u16EventId,pucData,u16DataLen);
    if (eSppRtv == SPP_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

SPP_STATUS sppc_AddEvent(SPP_UInt16 event_id, UINT8 * pucData, UINT16 u16DataLen)
{
    SppSend(SPP_CONNECT_TYPE_MCU_4G,event_id,pucData,u16DataLen);
    return TRUE;
}

SPP_BOOL SPP_Send_DCPD_MSG_TO_MPU_FOTA_FBL_COM_Event(const DcpdPackageFBLCom_t* data)
{
    return sppc_AddEvent(SPP_USER_SEND_DCPD_MSG_TO_MPU_FOTA_FBL_COM_EVT, (SPP_Int8 *)data, sizeof(DcpdPackageFBLCom_t));
}

/******************************************************************************
*  function name | ApiSPPInit
*  content       | initailise the spp
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e ApiSPPInit(void)
{
	return SppInitForAll(); 	
}


/******************************************************************************
*  function name | ApiSPPMainTask
*  content       | spp main task
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e ApiSPPMainTask(void)
{
	return SppMainTask(SPP_CONNECT_TYPE_MCU_4G);
}


/******************************************************************************
*  function name | ApiSPPStart
*  content       | start the spp protocal stack
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e ApiSPPStart(void)
{
    return SppInit(SPP_CONNECT_TYPE_MCU_4G) ;
}


/******************************************************************************
*  function name | ApiSPPStop
*  content       | stop the spp protocal stack
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e ApiSPPStop(void)
{
    return SppStop(SPP_CONNECT_TYPE_MCU_4G);
}


/******************************************************************************
*  function name | ApiSPPSleep
*  content       | spp sleep entering
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e ApiSPPSleep(void)
{
    SppSleep(SPP_CONNECT_TYPE_MCU_4G);
	return SPP_SUCCESS;
}


/******************************************************************************
*  function name | ApiSPPWakeup
*  content       | spp wake up
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e ApiSPPWakeup(void)
{
    SppWakeUp(SPP_CONNECT_TYPE_MCU_4G);
	return SPP_SUCCESS;
}


/******************************************************************************
*  function name | ApiSPPTestFlag
*  content       | new event receiving flag
*  parameter[1]  | u16EventId :
*  notice        | 
*  return        | 
******************************************************************************/
BOOL ApiSPPTestFlag(UINT16 u16EventId)
{
   return SppReceiveFlag(SPP_CONNECT_TYPE_MCU_4G, u16EventId);
}


/******************************************************************************
*  function name | ApiSPPClearFlag
*  content       | clear the flag
*  parameter[1]  | u16EventId :
*  notice        | 
*  return        | 
******************************************************************************/
void ApiSPPClearFlag(UINT16 u16EventId)
{
    SppClearRecFlg(SPP_CONNECT_TYPE_MCU_4G, u16EventId);
}


/******************************************************************************
*  function name | ApiSPPSetRecBuff
*  content       | set the message buffer
*  parameter[1]  | u16_eventID :
*  parameter[2]  | tpu8_recBuff :
*  parameter[3]  | u16_len :
*  notice        | 
*  return        | 
******************************************************************************/
void ApiSPPSetRecBuff(UINT16 u16_eventID,UINT8* tpu8_recBuff,UINT16 u16_len)
{
    (void)SppSetRecBuff(SPP_CONNECT_TYPE_MCU_4G,u16_eventID,tpu8_recBuff,u16_len);
}


/******************************************************************************
*  function name | ApiSPPGetRecvEventData
*  content       | get the event data
*  parameter[1]  | u16EventId :
*  notice        | 
*  return        | 
******************************************************************************/
UINT8* ApiSPPGetRecvEventData(UINT16 u16EventId)
{
    return SppGetReceiveData(SPP_CONNECT_TYPE_MCU_4G,u16EventId);
}


/******************************************************************************
*  function name | ApiSPPDeInit
*  content       | spp protocal stack deinit
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e ApiSPPDeInit(void)
{
    return SppDeInit(SPP_CONNECT_TYPE_MCU_4G);
}


/******************************************************************************
*  function name | ApiSPPGetState
*  content       | spp current state
*  notice        | 
*  return        | 
******************************************************************************/
SppState_e ApiSPPGetState(void)
{

    return SppGetStatus(SPP_CONNECT_TYPE_MCU_4G);
}


/******************************************************************************
*  function name | ApiSppGetVersionEx
*  content       | spp Version
*  parameter[1]  | tSppVersion :
*  notice        | 
*  return        | 
******************************************************************************/
void ApiSppGetVersionEx(SppVersionEx_t* tSppVersion)
{
	SppVersionEx_t tExVis ;
    tExVis.tSysVersion.s16Major = 0;
    tExVis.tSysVersion.s16Minor = 0;
    tExVis.tSysVersion.s16Patch = 0;
    
    tExVis.tUserVersion.s16Major = 0;
    tExVis.tUserVersion.s16Minor = 0;
    tExVis.tUserVersion.s16Patch = 0;

    memset(tExVis.as8md5,0x31,8);
    tExVis.as8md5[8] = 0;
    *tSppVersion = tExVis;
}



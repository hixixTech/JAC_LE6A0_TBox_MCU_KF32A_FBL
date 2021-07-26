/*****************************************************************************
|                      	       	Copyright
|-----------------------------------------------------------------------------
|    Copyright (c) 2018 SAIC Motor Co. LTD.  All rights reserved.
|
|    This software is furnished under a license and may be used and copied 
|    only in accordance with the terms of such license and with the inclusion
|    of the above copyright notice. This software or any other copies thereof 
|    may not be provided or otherwise made available to any other person.
|    No title to and ownership of the software is hereby transferred.
|
|    The information in this software is subject to change without notice 
|    and should not be constructed as a commitment by SAIC Motor Co. LTD.
|
|    SAIC Motor Co. LTD. assumes no responsibility for the use or reliability
|    of its Software on equipment which is not supported by SAIC Motor Co. LTD.
|-----------------------------------------------------------------------------
******************************************************************************/
#ifndef SPPCORE_H
#define SPPCORE_H

#include <stdio.h>
#include <string.h>
#include "sppc_core_types.h"
#include "type.h"

#define SPP_ON  (1)
#define SPP_OFF (0)


#if SPPC_UNITTEST_ENABLED == SPP_ON
#define SPPC_STATIC
#else
#define SPPC_STATIC static
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SPP_CYCLE_TIME        (10)


typedef enum
{
    SPP_SEND_STRATEGY_FORCE    = 0x01,
    SPP_SEND_STRATEGY_ONCHANGE = 0x02,
    SPP_SEND_STRATEGY_CYCLIC   = 0x04,
    SPP_SEND_ACK_ERROR_HANDLER = 0x08,
    SPP_SEND_ACK_HANDLER       = 0x10,
    SPP_SEND_HAVESEND_FLAG     = 0x20,
} SPP_SEND_EVENT_MASK;

typedef enum
{
    SPP_EVENT_START = 90,
    SPP_SYS_EVENT = SPP_EVENT_START,
    SPP_SYS_HANDSHAKE_EVENT = SPP_SYS_EVENT,
    SPP_KEEP_EVENT = 91,
    SPP_STOP_EVENT = 92,
    SPP_SEND_FRAME_ACK1_EVENT = 93,
    SPP_SEND_FRAME_ACK2_EVENT = 94,
    SPP_SEND_FRAME_ACK3_EVENT = 95,
    SPP_RECV_FRAME_ACK1_EVENT = 96,
    SPP_RECV_FRAME_ACK2_EVENT = 97,
    SPP_RECV_FRAME_ACK3_EVENT = 98,
    SPP_MULTIFRAME_EVENT = 99,
    SPP_USER_EVENT = 129,
} SPP_EVENT_ID;

typedef enum
{
    SPP_RECV_STATE_RECEIVED    = 0x01,
} SPP_RECV_EVENT_MASK;

typedef struct
{
    void* handler;
}SppcEventListener;

typedef struct
{
    SPP_UserEventAck_Callback      recv_ack_handle;
}SppcEventAckListener;

typedef struct
{
    SPP_UserEventAckError_Callback recv_ack_error_handle;
}SppcEventAckErrorListener;

typedef SPP_Int32  (*sppc_handle_event)(SPP_UInt16 event_id, void* data, SPP_Int32 data_length);

typedef struct
{
    SPP_Int16 start_offset;
    SPP_Int16 end_offset;
}SppcEventProperty;

typedef struct
{
    SPP_SerialPortInit_Callback  serialport_init_handle;
    SPP_SerialPortSend_Callback  serialport_send_handle;
    SPP_SerialPortRecv_Callback  serialport_recv_handle;
    SPP_SerialPortClose_Callback serialport_close_handle;
}sppc_SerialPortCallback;

typedef struct
{
    SppVersion*                   user_version;
    char                          *md5;
    SppcEventProperty*            user_event_props;
    SPP_Int16*                    user_queue_data;
    SPP_Int32                     user_queue_length;
    SPP_UInt8*                    user_databuf;
    SPP_Int32                     user_databuf_length;
    SppcEventProperty*            user_recv_event_props;
    SppcEventListener*            user_event_listener;
    SppcEventAckListener*         user_event_ack_listener;
    SppcEventAckErrorListener*    user_event_ack_error_listener;
    SPP_UInt8*                    user_recv_databuf;
    SPP_Int32                     user_recv_databuf_length;
    sppc_handle_event             user_recv_handler;
    sppc_SerialPortCallback         user_serialport_handle;
}SppcUserRuntime;

/** 
*  @brief spp module init function
*/
SPP_STATUS sppc_Init(SppcUserRuntime* user_runtime);

/** 
*  @brief spp module deinit function
*/
SPP_STATUS sppc_DeInit(void);

/** 
*  @brief spp module start function
*/
SPP_STATUS sppc_Start(void);

/** 
*  @brief spp module stop function
*/
SPP_STATUS sppc_Stop(void);

/** 
*  @brief get spp status function
*/
// SppState_e sppc_GetState(void);

/** 
*  @brief spp module main task function
*
*  This function is only working under the mcu environment, it should
*  be called periodically (typical value is 10ms)
*/
SPP_STATUS sppc_MainTask(void);

/** 
*  @brief spp module sleep function
*
*  Not support now
*/
SPP_STATUS sppc_Sleep(void);

/** 
*  @brief spp module wakeup function
*
*  Not support now
*/
SPP_STATUS sppc_Wakeup(void);

/** 
*  @brief set the log level if user want to capture spp's internal log
*/
SPP_STATUS sppc_SetLogLevel(SPP_LOGLEVEL ll);

/** 
*  @brief add the logging output callback
*/
SPP_STATUS sppc_Add_Logging_Listener(SPP_Logging_Callback cb);

/**
*  @brief whether the handshake is finish
*/
SPP_STATUS sppc_Is_Connected(void);

/** 
*  @brief add the connection state change callback
*/
SPP_STATUS sppc_Add_ConnectionStateChange_Listener(SPP_ConnectionStateChange_Callback cb);

/** 
*  @brief add the event acknowledge callback
*/
SPP_STATUS sppc_Add_EventAck_Listener(SPP_UInt16 event_id, SPP_UserEventAck_Callback cb);

/** 
*  @brief add the event acknowledge error callback
*/
SPP_STATUS sppc_Add_EventAckError_Listener(SPP_UInt16 event_id, SPP_UserEventAckError_Callback cb);

/** 
*  @brief add the event receive callback
*/
SPP_STATUS sppc_Add_EventRecv_Listener(SPP_EventRecv_Callback cb);

/** 
*  @brief add an event to event queue
*/
SPP_STATUS sppc_AddEvent(SPP_UInt16 event_id, UINT8 * pucData, UINT16 u16DataLen);

/** 
*  @brief get out-going event's data's length
*/
SPP_Int32 sppc_GetSendEventLength(SPP_UInt16 event_id);

/** 
*  @brief get out-going event's data's offset
*/
SPP_Int32 sppc_GetSendEventOffset(SPP_UInt16 event_id);

/** 
*  @brief get incoming event's data's length
*/
SPP_Int32 sppc_GetRecvEventLength(SPP_UInt16 event_id);

/** 
*  @brief get incoming event's data's offset
*/
SPP_Int32 sppc_GetRecvEventOffset(SPP_UInt16 event_id);

/** 
*  @brief get out-going event's data
*/
void* sppc_GetEventData(SPP_UInt16 event_id);

/** 
*  @brief get incoming event's data
*/
void* sppc_GetRecvEventData(SPP_UInt16 event_id);

/** 
*  @brief set handling callback of an event
*/
//void sppc_SetListener(SPP_UInt16 event_id, void* listener);

/** 
*  @brief get handling callback of an event
*/
void* sppc_GetListener(SPP_UInt16 event_id);

/** 
*  @brief set out-going event's strategy flag
*/
SPP_STATUS sppc_SetSendStrategyFlag(SPP_UInt16 event_id, SPP_UInt8 flag);

/** 
*  @brief set incoming event's strategy flag
*/
SPP_STATUS sppc_GetSendStrategyFlag(SPP_UInt16 event_id, SPP_UInt8* flag);

/** 
*  @brief set out-going event's strategy flag
*/
SPP_STATUS sppc_SetRecvStrategyFlag(SPP_UInt16 event_id, SPP_UInt8 flag);

/** 
*  @brief get out-going event's strategy flag
*/
SPP_STATUS sppc_GetRecvStrategyFlag(SPP_UInt16 event_id, SPP_UInt8* flag);

/** 
*  @brief get test flag of an event
*/
SPP_BOOL sppc_TestFlag(SPP_UInt16 event_id);

/** 
*  @brief clear test flag of an event
*/
void sppc_ClearFlag(SPP_UInt16 event_id);

/**
*  @brief get HaveSendFlag of an event
*/
SPP_BOOL sppc_TestHaveSendFlag(SPP_UInt16 event_id);

/**
*  @brief Set HaveSendFlag of an event
*/
void sppc_SetHaveSendFlag(SPP_UInt16 event_id);

/** 
*  @brief lock the out-going's event data in case of thread safety
*/
SPP_STATUS sppc_SendBufLock(void);

/** 
*  @brief unlock the out-going's event data in case of thread safety
*/
SPP_STATUS sppc_SendBufUnlock(void);

/** 
*  @brief lock the incoming's event data in case of thread safety
*/
SPP_STATUS sppc_RecvBufLock(void);

/** 
*  @brief unlock the incoming's event data in case of thread safety
*/
SPP_STATUS sppc_RecvBufUnlock(void);

/** 
*  @brief get the version informaiton
*/
const SppVersion* sppc_GetVersion(void);

/** 
*  @brief statistic functions
*/
SPP_UInt32 sppc_GetStatsTotalCount(void);
SPP_UInt32 sppc_GetStatsNormalCount(void);
SPP_UInt32 sppc_GetStatsRetryCount(void);
SPP_UInt32 sppc_GetStatsErrorCount(void);
SPP_UInt32 sppc_GetStatsSuccessCount(void);
SPP_UInt32 sppc_GetStatsFailureCount(void);
SPP_UInt16 sppc_GetStatsSuccessRate(void);
SPP_STATUS sppc_StatsCountInit(void);

/** 
*  @brief build-in types serialization and deserialization functions
*/
void sppc_serialize_SPP_Int64(SPP_Int8* pbuf, const SPP_Int64 *pdata);
void sppc_deserialize_SPP_Int64(const SPP_Int8* pbuf, SPP_Int64 *pdata);
void sppc_serialize_SPP_UInt64(SPP_Int8* pbuf, const SPP_UInt64 *pdata);
void sppc_deserialize_SPP_UInt64(const SPP_Int8* pbuf, SPP_UInt64 *pdata);
void sppc_serialize_SPP_Int32(SPP_Int8* pbuf, const SPP_Int32 *pdata);
void sppc_deserialize_SPP_Int32(const SPP_Int8* pbuf, SPP_Int32 *pdata);
void sppc_serialize_SPP_UInt32(SPP_Int8* pbuf, const SPP_UInt32 *pdata);
void sppc_deserialize_SPP_UInt32(const SPP_Int8* pbuf, SPP_UInt32 *pdata);
void sppc_serialize_SPP_Int16(SPP_Int8* pbuf, const SPP_Int16 *pdata);
void sppc_deserialize_SPP_Int16(const SPP_Int8* pbuf, SPP_Int16 *pdata);
void sppc_serialize_SPP_UInt16(SPP_Int8* pbuf, const SPP_UInt16 *pdata);
void sppc_deserialize_SPP_UInt16(const SPP_Int8* pbuf, SPP_UInt16 *pdata);
void sppc_serialize_SPP_Int8(SPP_Int8* pbuf, const SPP_Int8 *pdata);
void sppc_deserialize_SPP_Int8(const SPP_Int8* pbuf, SPP_Int8 *pdata);
void sppc_serialize_SPP_UInt8(SPP_Int8* pbuf, const SPP_UInt8 *pdata);
void sppc_deserialize_SPP_UInt8(const SPP_Int8* pbuf, SPP_UInt8 *pdata);

#ifdef __cplusplus
}
#endif

#endif //SPPCORE_H

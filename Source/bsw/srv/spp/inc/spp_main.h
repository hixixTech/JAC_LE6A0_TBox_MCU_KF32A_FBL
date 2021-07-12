#ifndef _SPP_MAIN
#define _SPP_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <pthread.h>
//#include "common_error_code.h"
//#include "lib_public.h"
#include "spp_code.h"

#define SPP
#define SPP_CONNECT_MAX_COUNT (4)
#define SPP_CALLBACK_MAX_COUNT (SPP_USER_DCPD_MSG_TOMCU_MAX)
#define SPP_ACK_MAX_COUT (256)
#define SPP_CMD_ROLL_MAX (64)
#define SPP_MUTEX_TIME_OUT 5 //5ms
#define SPP_SEND_LIST_MAX_NUM 64  //24->64 modify by lcl
#define SPP_SEND_LIST_NORMAL_NUM 50  //12->32->50 modify by lcl
#define SPP_ACK_LIST_MAX_NUM 64  //24->64 modify by lcl
#define SPP_OPEN_DEVICE_WAILT_TIME 1000 //1ms
#define SPP_SEND_DATA_WAILT_TIME 1000 //1ms
#define SPP_SEND_DATA_FAIL_WAILT_TIME 1000000 //1ms
#define SPP_RECV_DATA_WAILT_TIME 1000 //1ms
#define SPP_ACK_TIMEOUT_TIME 3*PROCESS_TIMES_ONE_CYCLE //30ms
#define SPP_SEND_TIMES_MAX 2
#define SPP_RESEND_MAX_TIMES 3 
#define SPP_CMD_MAX_TYPES 128
#define SPP_HEART_SEND_PERIOD 1000  //2000ms



typedef enum
{
    SPP_CON_UNKNOWN = 0,
    SPP_CON_INIT,
    SPP_CON_CONNECTING,
    SPP_CON_SYS_VERSION_MISMATCH_ERROR,
    SPP_CON_USER_VERSION_MISMATCH_ERROR,
    SPP_CON_SIGNATURE_VERSION_MISMATCH_ERROR,
    SPP_CON_CONNECTED,
    SPP_CON_DISCONNECTING,
    SPP_CON_DISCONNECTED,
}SppConnState_e;



/*dcpd中spp状态，同步spplib*/
typedef enum
{
    SPP_STATE_UNKNOW = 0,
    SPP_STATE_INIT,
    SPP_STATE_RUNNING,
    SPP_STATE_WARNING,
    SPP_STATE_ERROR,
    SPP_STATE_SLEEP
}SppState_e; 



typedef void (* pfSppConnectionStateChangeCallback)(SppConnState_e eConnState);
typedef void (* pfSppListener)( UINT16 u16EventId, SppPackage_t* ptData, UINT32 u32DataLen);
typedef void (* pfSppAckListener)(UINT16 u16EventId,UINT8 u8EventState);




typedef struct _SppListNode_t
{
    UINT8 u8Sn;
    UINT8 u8Type;
    UINT8 u8_eventNum;
    UINT8 u8SendTimes;
    UINT8 u8SplitedNumAll;   
    UINT8 u8SplitedNumCur;
    UINT16 u16_dataIndex;
    UINT16 u16_remainLen;
    UINT32 u32TimeCout;
    UINT16 u16_event[SPP_BIG_PACKET_EVENT_NUM_MAX];
    UINT8 pDcpdData[SPP_BIG_PACKET_PAYLOAG_LEN_MAX];
    struct _SppListNode_t *prev;
    struct _SppListNode_t *next;
}SppListNode_t;

typedef enum
{
	SPP_CONNECT_TYPE_MIN     =0, 
	SPP_CONNECT_TYPE_MCU_4G, 
	SPP_CONNECT_TYPE_MCU_MPU,
	SPP_CONNECT_TYPE_4G, 
	SPP_CONNECT_TYPE_MPU,
}SppConnectType_e; 

typedef struct
{
	SppConnectType_e eConnectType;
	SppState_e  eConnStatus;
}SppConnectStatus_t; 

typedef struct
{
	UINT16 u16EventId;
	UINT8 u8ResendTimes;
	UINT8 u8Prioty;
}SppCmdInfo_t; 
typedef struct
{
    UINT8 u8Sn;
    BOOL bBlock;
}SppStat_t;

typedef struct
{
    SppConnectType_e eConnectType;
    pfSppConnectionStateChangeCallback pFunConnCb;
}SppConnMsgProc_t;

typedef struct
{
    SppConnectType_e eConnectType;
	UINT16 u16EventId;
    pfSppListener pFunCb;
}SppMsgProc_t;  /*register pListener type*/

typedef struct
{
    SppConnectType_e eConnectType;
    UINT16 u16EventId;
    UINT8* pu8Buffer;
    UINT16 u16Len;
    BOOL bFlg;
}SppMsgBuffRec_t;


typedef struct
{
    SppConnectType_e eConnectType;
	UINT16 u16EventId;
    pfSppAckListener pFunCb;
}SppSendAckProc_t;  /*register pListener type*/




typedef struct
{
    UINT16 u16WaitTimes;   /*发送等待时间计数*/
    UINT8  u8SendFlg;      /*心跳发送标记*/
    UINT8  u8RecAckFlag;   /*心跳接收标记*/
    UINT8  u8SendTimes;    /*心跳发送次数*/
    UINT8  u8RecFirstAckFlg;  /*等待接收第一次Ack标记，0：未发送心跳包 
                                                        1：上电等待未接收第一个ACK,
                                                        2: 上电或者复位后已接收到第一个ACK*/
}SppHeartManageType;

SPPErrorCode_e SppInitForAll(VOID);
SPPErrorCode_e SppInit(SppConnectType_e eConnectType);
SPPErrorCode_e SppStart(SppConnectType_e eConnectType);
SPPErrorCode_e SppDeInit(SppConnectType_e eConnectType);
SPPErrorCode_e SppStop(SppConnectType_e eConnectType);
SPPErrorCode_e SppSetListener(SppConnectType_e eConnectType,UINT16 u16EventId,pfSppListener pSppcListener);	
SPPErrorCode_e SppSend(SppConnectType_e eConnectType,UINT16 u16EventId,UINT8 * pucData, UINT16 u16DataLen);	
SppState_e SppGetStatus(SppConnectType_e eConnectType);
SPPErrorCode_e SppSetConnStateChangeListener(SppConnectType_e eConnectType,void* pSppcConnStateChangeListener);	
SPPErrorCode_e SppSleep(SppConnectType_e eConnectType);
SPPErrorCode_e SppWakeUp(SppConnectType_e eConnectType);

SPPErrorCode_e SppSetAckListener(SppConnectType_e eConnectType,UINT16 u16EventId,pfSppAckListener pSppcListener);
SPPErrorCode_e SppMainTask(SppConnectType_e eConnectType);

UINT8* SppGetReceiveData(SppConnectType_e eConnectType,UINT16 u16EventID);
SPPErrorCode_e SppSetRecBuff(SppConnectType_e eConnectType,UINT16 u16EventID,UINT8* pu8RecBuff,UINT16 u16Len);
BOOL SppReceiveFlag(SppConnectType_e eConnectType,UINT16 u16EventID);
void SppClearRecFlg(SppConnectType_e eConnectType,UINT16 u16EventID);


#endif 

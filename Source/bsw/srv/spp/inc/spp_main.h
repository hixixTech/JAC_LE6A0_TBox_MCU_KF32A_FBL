#ifndef _DIAS_SPP_MAIN
#define _DIAS_SPP_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <pthread.h>
//#include "common_error_code.h"
//#include "lib_dias_public.h"
#include "spp_code.h"

#define DIAS_SPP
#define SPP_CONNECT_MAX_COUNT (4)
#define DIAS_SPP_CALLBACK_MAX_COUNT (256)
#define DIAS_SPP_CMD_ROLL_MAX (64)
#define DIAS_SPP_MUTEX_TIME_OUT 5 //5ms
#define DIAS_SPP_SEND_LIST_MAX_NUM 64  //24->64 modify by lcl
#define DIAS_SPP_SEND_LIST_NORMAL_NUM 50  //12->32->50 modify by lcl
#define DIAS_SPP_ACK_LIST_MAX_NUM 64  //24->64 modify by lcl
#define DIAS_SPP_OPEN_DEVICE_WAILT_TIME 1000 //1ms
#define DIAS_SPP_SEND_DATA_WAILT_TIME 1000 //1ms
#define DIAS_SPP_SEND_DATA_FAIL_WAILT_TIME 1000000 //1ms
#define DIAS_SPP_RECV_DATA_WAILT_TIME 1000 //1ms
#define DIAS_SPP_ACK_TIMEOUT_TIME 500 //500ms
#define DIAS_SPP_RESEND_MAX_TIMES 3 
#define DIAS_SPP_CMD_MAX_TYPES 128
#define DIAS_SPP_HEART_SEND_PERIOD 1000  //2000ms



typedef enum
{
    DIAS_SPP_CON_UNKNOWN = 0,
    DIAS_SPP_CON_INIT,
    DIAS_SPP_CON_CONNECTING,
    DIAS_SPP_CON_SYS_VERSION_MISMATCH_ERROR,
    DIAS_SPP_CON_USER_VERSION_MISMATCH_ERROR,
    DIAS_SPP_CON_SIGNATURE_VERSION_MISMATCH_ERROR,
    DIAS_SPP_CON_CONNECTED,
    DIAS_SPP_CON_DISCONNECTING,
    DIAS_SPP_CON_DISCONNECTED,
}DiasSppConnState_e;

/*dcpdÖÐspp×´Ì¬£¬Í¬²½spplib*/
typedef enum
{
    DIAS_SPP_STATE_UNKNOW = 0,
    DIAS_SPP_STATE_INIT,
    DIAS_SPP_STATE_RUNNING,
    DIAS_SPP_STATE_WARNING,
    DIAS_SPP_STATE_ERROR,
    DIAS_SPP_STATE_SLEEP
}DiasSppState_e; 


#ifdef _lint
    typedef long time_t;
#endif

struct timeval
{
    time_t tv_sec; // seconds
    long tv_usec;  // microseconds 
};

typedef void (* pDiasSppConnectionStateChangeCallback)(DiasSppConnState_e eConnState);
typedef void (* pDiasSppListener)( UINT16 u16EventId, DiasSppPackage_t* ptData, UINT32 u32DataLen);

typedef struct _DiasSppListNode_t{
    UINT16 u16EventId;  
    UINT16 u16CmdTimeout;
    UINT8 u8Priority;
    UINT8 u8SplitedNumAll;   
    UINT8 u8SplitedNumCur;
	UINT8 u8ResendTimes;
	UINT16 u16DataLen;
	UINT8 pDcpdData[DIAS_SPP_PACKET_LEN_MAX];  
    UINT32 u32TimeCout;
    struct timeval  currentTime;
    struct _DiasSppListNode_t *prev;
    struct _DiasSppListNode_t *next;	
}DiasSppListNode_t;

typedef enum
{
	SPP_CONNECT_TYPE_MIN     =0, 
	SPP_CONNECT_TYPE_MCU_4G, 
	SPP_CONNECT_TYPE_MCU_MPU,
	SPP_CONNECT_TYPE_4G, 
	SPP_CONNECT_TYPE_MPU,
}DiasSppConnectType_e; 

typedef struct
{
	DiasSppConnectType_e eConnectType;
	DiasSppState_e  eConnStatus;
}DiasSppConnectStatus_t; 

typedef struct
{
	UINT16 u16EventId;
	UINT8 u8ResendTimes;
	UINT8 u8Prioty;
}DiasSppCmdInfo_t; 

typedef struct
{
    DiasSppConnectType_e eConnectType;
    pDiasSppConnectionStateChangeCallback pFunConnCb;
}DiasSppConnMsgProc_t;

typedef struct
{
    DiasSppConnectType_e eConnectType;
	UINT16 u16EventId;
    pDiasSppListener pFunCb;
}DiasSppMsgProc_t;  /*register listener type*/
typedef struct
{
    UINT16 u16_waitTimes;
    UINT8  u8_sendFlg;
    UINT8  u8_recAckFlag;
    UINT8  u8_sendTimes;
    UINT8  u8_recFirstAckFlg;
}DiasSppHeartManageType;

DiasErrorCode_e DiasSppInitForAll(VOID);
DiasErrorCode_e DiasSppInit(DiasSppConnectType_e eConnectType);
DiasErrorCode_e DiasSppStart(DiasSppConnectType_e eConnectType);
DiasErrorCode_e DiasSppDeInit(DiasSppConnectType_e eConnectType);
DiasErrorCode_e DiasSppStop(DiasSppConnectType_e eConnectType);
DiasErrorCode_e DiasSppSetListener(DiasSppConnectType_e eConnectType,UINT16 u16EventId,pDiasSppListener pSppcListener);	
DiasErrorCode_e DiasSppSend(DiasSppConnectType_e eConnectType,UINT16 u16EventId,UINT8 * pucData, UINT16 u16DataLen);	
DiasSppState_e DiasSppGetStatus(DiasSppConnectType_e eConnectType);
DiasErrorCode_e DiasSppSetConnStateChangeListener(DiasSppConnectType_e eConnectType,void* pDiasSppcConnStateChangeListener);	
DiasErrorCode_e DiasSppMainTask(DiasSppConnectType_e eConnectType);


#endif 

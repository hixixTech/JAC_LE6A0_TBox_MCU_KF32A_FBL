/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    dl_engine.h                                                 */
/* Description:  Header for the diagnostic layer                             */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠            基线创建                          */
/*****************************************************************************/

#ifndef DL_ENGINE_H
#define DL_ENGINE_H

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "type.h"
#include "bl_mem.h"
#include "tl_engine.h"


/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    FunctionalAddressing,
	PhysicalAddressing,
	BothAddressing,
	FunctInDefaultBothInProg
}FrameAddressingType_e;

typedef enum
{
    deflt, 
	programming
}SessionType_e;

typedef enum
{
    sec_unlocked,
	sec_locked
}SecurityType_e;
typedef enum
{
    Completed=0,
	Failed=0x02,
	ActiveOrNotStarted=0xFF
}RoutineStatus_e;
	
typedef enum
{
    crcIgnored=0,
	crcRequested=1
}CrcType_e;
	
typedef enum 
{  
    NotValid,
    Valid,
    NotImportant
}MsgProperty_e;

typedef struct{
    bool bProgrammingSession;
    bool bDoEcuReset;
    bool bDoWdbyid;
    bool bDownload;
    bool bTdInProgress;
    bool bAutErase;
    bool bRcErase;
	bool bRcCpdi;
    bool bRcCheckProgDep;
    bool bRcCheckProgInt;
    bool bSaSeedSent;
    bool bSaFirstSeed;
    bool bLocked;
    bool bSblActive;
    bool bTexit;
    bool bAppIntegrityDone;
    bool bAppValidDone;
    bool bDoDscBg;
    bool bS3Expired;
}Semaphore_t;

typedef struct{
    UINT32  u32MemoryAddress;
    UINT32  u32MemorySize;
    MemType_e   eMemType;
}RcMemory_t;

typedef UINT8 * response_type;

typedef struct{
    UINT8* pu8TxData;
    UINT16 u16DataLength;
    UINT8 u8ServiceId;
} Resp_t;

typedef struct{
    UINT8           u8ServiceId;
    UINT16          u16Length;
    SessionType_e    eSessionType;
    SecurityType_e   eSecurityType;
    FrameAddressingType_e  eFrameAddressing;
}Msgprop_t;

typedef struct
{
    bool *pbFlag;
    bool (* bg_func)(void);
}BgTaskProp_t;

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#ifndef CRC_BUFFER_SIZE
    #define CRC_BUFFER_SIZE     (NumberOfBlockSize)
#endif

#define MAX_NUMBER_OF_WAITS     ((UINT16)5)

#define defaultSession          ((UINT8)0x01)
#define programmingSession      ((UINT8)0x02)

/* BL negative response codes (NRC)                                           */
#define generalReject                               ((UINT8)0x10u) /* not used */
#define serviceNotSupported                         ((UINT8)0x11u) /*服务不支持*/
#define subFunctionNotSupported                     ((UINT8)0x12u) /*子功能不支持*/
#define incorrectMessageLengthOrInvalidFormat       ((UINT8)0x13u)   /*报文长度错误*/
#define busyRepeatRequest                           ((UINT8)0x21u) /* not used */
#define conditionsNotCorrect                        ((UINT8)0x22u)  /*条件不正确*/
#define routineNotComplete                          ((UINT8)0x23u)
#define requestSequenceError                        ((UINT8)0x24u) /*请求次序错误*/
#define requestOutOfRange                           ((UINT8)0x31u) /*请求超出范围*/
#define securityAccessDenied                        ((UINT8)0x33u) /*安全访问未通过*/
#define invalidKey                                  ((UINT8)0x35u) /*秘钥无效*/
#define exceededNumberOfAttempts                    ((UINT8)0x36u) /*超过最大尝试次数*/
#define requiredTimeDelayNotExpired                 ((UINT8)0x37u) /*延时要求不满足*/
#define uploadDownloadNotAccepted                   ((UINT8)0x70u)
#define transferDataSuspended                       ((UINT8)0x71u) /*传输数据中止*/
#define generalProgrammingFailure                   ((UINT8)0x72u) /*一般性编程失败*/
#define wrongBlockSequenceCounter                   ((UINT8)0x73u) /*块序号错误*/
#define illegalByteCountInBlockTransfer             ((UINT8)0x75u) /* not used */
#define responsePending                             ((UINT8)0x78u)
#define incorrectByteCountDuringBlockTransfer       ((UINT8)0x79u) /* not used */
#define subFunctionNotSupportedInActiveSession      ((UINT8)0x7eu) /*子功能不支持*/
#define serviceNotSupportedInActiveSession          ((UINT8)0x7fu) /*在当前会话下服务不支持*/
#define serviceNotSupportedInActiveDiagnosticMode   ((UINT8)0x80u) /* not used */
#define voltageTooHigh                              ((UINT8)0x92u)
#define voltageTooLow                               ((UINT8)0x93u)

/* specificCauseCode for NRC conditionsNotCorrect, represented in the upper byte */
/* After changing specificCauseCode definition update release note and design */
#define specificCauseCode0  ((UINT16)(0u<<8)) /* general: addressing error */
#define specificCauseCode1  ((UINT16)(1u<<8)) /* PIF counter read was failed */
#define specificCauseCode2  ((UINT16)(2u<<8)) /* Pre-work failed. Write once ID is already written.*/
#define specificCauseCode3  ((UINT16)(3u<<8)) /* Post-write action failed (post_wr_act())*/
#define specificCauseCode4  ((UINT16)(4u<<8)) /* DID is only writeable in extended security level */
#define specificCauseCode5  ((UINT16)(5u<<8)) /* Entering programming session failed */
#define specificCauseCode6  ((UINT16)(6u<<8)) /* ID cannot be read (E.g. external EEPROM problem).*/
#define specificCauseCode7  ((UINT16)(7u<<8)) /* Download is ongoing, it cannot be interrupted.*/
#define specificCauseCode8  ((UINT16)(8u<<8)) /* Customer specific cause code for RoutineControl. */
#define specificCauseCode9  ((UINT16)(9u<<8)) /* DID write_once_after_prog: programming not started */

#define ResponseSID                     ((UINT8)0x40u)
/* It's not practical to use functions instead of this function-like macros*/
#define RESPONSE(x)                     ((x) + ResponseSID) /* PRQA S 3453 */

/* multilength identifies services with variable length                     */
#define multilength                     (0xffffu)

/* BL services */
#define SID_DSC                         ((UINT8)0x10u)
#define SID_EcuReset                    ((UINT8)0x11u)
#define SID_ReadDTCByStatus             ((UINT8)0x18u) /* not supported */
#define SID_ReadMemoryByAddress         ((UINT8)0x23u) /* not supported */
#define SID_SecurityAccess              ((UINT8)0x27u)
#define SID_TesterPresent               ((UINT8)0x3eu)
#define SID_ReadDataByIdentifier        ((UINT8)0x22u)
#define SID_ReadDataByCommonID          ((UINT8)0x22u) /* not supported */
#define SID_CommunicationControl        ((UINT8)0x28u) /* not supported */
#define SID_WriteDataByIdentifier       ((UINT8)0x2eu)
#define SID_RoutineControl              ((UINT8)0x31u)
#define SID_RequestDownload             ((UINT8)0x34u)
#define SID_TransferData                ((UINT8)0x36u)
#define SID_TransferExit                ((UINT8)0x37u)
#define SID_ControlDTCSetting           ((UINT8)0x85u) /* not supported */
#define SID_DiagnosticCommand           ((UINT8)0xb1u) /* not supported */

/* RoutineControl*/
#define rcEraseMemory                   ((UINT16)0xff00u)    /*擦除指令*/
#define rcCheckProgrammingDependencies  ((UINT16)0xff01u)    /*兼容性检查*/
#define rcCheckProgrammingIntegrity     ((UINT16)0xdfffu)    /*完整性检查*/

/*JAC Routines*/
#define rcEraseCode                     ((UINT16)0xff01u)
#define rcEraseCalibration              ((UINT16)0xff02u)
#define rcEraseConfig                   ((UINT16)0xff03u)
#define rcCheckProgramming              ((UINT16)0xff0Au) /* consistency / integrity */

/* bg_service_mask */
/* 'ALL' represents a request to abort all pending operations, it cannot force */
#define NONE            ((UINT16)0x0u)
#define ER              ((UINT16)0x1u)
#define WRDBI           ((UINT16)0x2u)
#define TE1              ((UINT16)0x4u)
#define TD              ((UINT16)0x8u)
#define RC_CRC          ((UINT16)0x10u)
#define RC_EM           ((UINT16)0x20u)
#define RC_CPD          ((UINT16)0x40u)
#define RC_CPI          ((UINT16)0x80u)
#define RD              ((UINT16)0x100u)
#define DSC             ((UINT16)0x200u)
#define ALL             ((UINT16)0x3FFu)


/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/
extern Semaphore_t tSemaphores;

/*---------------------------------------------------------------------------*/
/* 外部公开变量申明                                                              */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开函数声明                                                          */
/*---------------------------------------------------------------------------*/
extern void ApiDlDiagnosticBgTask(void);
extern void ApiDlDiagnosticFgTask(void);
extern void ApiDlAppSwitchToProgramming(void);

extern UINT8* ApiDlGetResponseBuffer(void);
extern bool ApiDlSendNegativeResponse(UINT16 u16Responsecode);
extern bool ApiDlSendPositiveResponse(const UINT8 *ptResp, UINT16 u16RespLength);

extern bool ApiDlAbortPendingOperations(UINT16 u16Mask);

extern bool ApiDlIsAnyOngoingService(UINT8 u8ServiceId);


#endif /* DL_ENGINE_H */

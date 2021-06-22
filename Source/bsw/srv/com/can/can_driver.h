/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     can_driver.h                                                */
/* Description:   DIAS CAN Stack的CAN驱动层头文件                            */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:     RH850F1L                                                   */
/* Compiler:      GHS_201517                                                 */
/********************************** 修改历史 *********************************/
/* Date             Version          Author            Description           */
/* 2019-08-01     V11.01.01.01.00   吴辰雨     新建基线                      */
/* 2019-11-06     V11.01.01.01.01   吴辰雨     增加发送中断停止接口          */
/*****************************************************************************/

#ifndef __CAN_DRIVER_H__
#define __CAN_DRIVER_H__


#include "type.h"
/*** public type definiton ***/

/*** Enum Definition ***/
/* Transmit message type */
typedef enum
{
    DCS_TXTYPE_PERIODIC = 0,
    DCS_TXTYPE_SPORADIC,
    DCS_TXTYPE_IMMEDIATE,
}DCSTxType_e;

/* Receive message type */
typedef enum
{
    DCS_RXTYPE_SIGNAL = 0,
    DCS_RXTYPE_IMMEDIATE,
}DCSRxType_e;

typedef enum
{
    DCS_TXFMD_APP = 0,
    DCS_TXFMD_NM,
    DCS_TXFMD_DIAG,
    DCS_RXFMD_APP,
    DCS_RXFMD_NM,
    DCS_RXFMD_DIAG,
}DCSFMDType_e;

typedef enum
{
    BYTE_ORDER_INTEL = 0,
    BYTE_ORDER_MOT,
}DCSBteOrder_e;

/*** Register Definition ***/
typedef struct
{
    volatile UINT32 CFG;
    volatile UINT32 CTR;
    volatile UINT32 STS;
    volatile UINT32 ERFL;
}DCSChannelRegAddr_t;

typedef struct
{
    volatile UINT32 GAFLID;
    volatile UINT32 GAFLM;
    volatile UINT32 GAFLP0;
    volatile UINT32 GAFLP1;
}DCSReceiveRuleRegAddr_t;

typedef struct
{
    volatile UINT32 RMID;
    volatile UINT32 RMPTR;
    volatile UINT32 RMDF0;
    volatile UINT32 RMDF1;
}DCSReceiveBufferRegAddr_t;

typedef struct
{
    volatile UINT32 TMID;
    volatile UINT32 TMPTR;
    volatile UINT32 TMDF0;
    volatile UINT32 TMDF1;
}DCSTransmitBufferRegAddr_t;


/*** type definition */
typedef struct
{
    volatile UINT32 CFID;
    volatile UINT32 CFPTR;
    volatile UINT32 CFDF0;
    volatile UINT32 CFDF1;
}DCSCFBufferRegAddr_t;

/*** type definition */
typedef struct
{
    UINT32    ID;
    UINT32    ID_Mask;
    UINT8     DLC;
    UINT8     RecBuffer;    
    UINT8*    MessageAddr;
    BOOL        received;
}DCSReceiveSigFrameConfig_t;

typedef struct
{
    UINT32    ID;
    UINT32    ID_Mask;
    UINT8     DLC;
    UINT8     RecBuffer;    
    UINT8*    MessageAddr;
    UINT8     FrameType;
}DCSReceiveImFrameConfig_t;


typedef struct
{
    UINT32    ID;
    UINT8     DLC;
    UINT8     TrmBuffer;
    UINT8*    MessageAddr;

    DCSTxType_e  TxType;
    BOOL        updated;
    UINT16    cycle;
    UINT16    counter;
}DCSTransmitSigFrameConfig_t;


typedef struct
{
    UINT32    ID;
    UINT8     DLC;
    UINT8     TrmBuffer;
    UINT8*    MessageAddr;
    UINT8     FrameType;
}DCSTransmitImFrameConfig_t;


typedef struct
{
    UINT32    ID;
    UINT32    ID_Mask;
    UINT8     DLC;
    UINT8     FIFOBuffer;
    UINT8     FIFORxSigNum;
}DCSOSEKNMRxFrameConfig_t;



/* Signal Structure */
typedef struct
{
    UINT16 signal_id;
    UINT8* frame_pointer;
    UINT8  start_bit;
    UINT8  length;
    DCSTransmitSigFrameConfig_t *txsig_config;
    DCSBteOrder_e sig_byte_order;
}DCSSignalConfig_t;

typedef UINT16    DCS_Signal;
typedef UINT8     DCS_RxSigf;
typedef UINT8     DCS_RxImf;
typedef UINT8     DCS_TxSigf;
typedef UINT8     DCS_TxImf;

typedef struct
{
    UINT8 channel;

    /* clock freq */
    UINT8 SWJ;
    UINT8 TSEG2;
    UINT8 TSEG1;
    UINT8 BRP;
    
    DCSBteOrder_e byte_order;

    UINT8 tx_sig_buffers;
    UINT8 tx_imf_buffers;
    UINT8 rx_sig_buffers;
    UINT8 rx_imf_buffers;

    /* receive frame */
    DCSReceiveSigFrameConfig_t* receiveSigFrameConfig;
    DCSReceiveImFrameConfig_t*  receiveImFrameConfig;
    DCSTransmitSigFrameConfig_t* transmitSigFrameConfig;
    DCSTransmitImFrameConfig_t* transmitImFrameConfig;

    DCSOSEKNMRxFrameConfig_t*   OSEKNMRxFrameConfig;

}DCS_controllerConfig;



/*** macro definiton ***/
#define DCS_OK      0x00
#define DCS_ERROR   0x01


#define DCS_IMF_TX_IDLE     0x00
#define DCS_IMF_TX_PENDING  0x01
#define DCS_IMF_TX_ERROR    0x02

/*** extern functions ***/
extern UINT8 ApiDcsSysInit(void);

extern UINT8 ApiDcsCtlInit(const DCS_controllerConfig* dcs_ctlcfg);
extern UINT8 ApiDcsCtlConn(const DCS_controllerConfig* dcs_ctlcfg);
extern UINT8 ApiDcsCtlDisConn(const DCS_controllerConfig* dcs_ctlcfg);
extern UINT8 ApiDcsImfTx(const DCS_controllerConfig* dcs_ctlcfg, const DCS_TxImf txframe);
extern BOOL ApiDcsImfRx(const DCS_controllerConfig* dcs_ctlcfg, const DCS_RxImf rxframe);
extern UINT8 ApiDcsImfTxPending(const DCS_controllerConfig* dcs_ctlcfg, const DCS_TxImf txframe);


extern void ApiDcsWr8(DCS_Signal d, UINT8 v);
extern void ApiDcsWr16(DCS_Signal d, UINT16 v);
extern void ApiDcsWr32(DCS_Signal d, uint32_t v);
extern void ApiDcsWrbool(DCS_Signal d, BOOL v);
extern void ApiDcsWrbytes(DCS_Signal d, UINT8 * src, UINT8 n, UINT8 o);
extern UINT8 ApiDcsRd8(DCS_Signal d);
extern UINT16 ApiDcsRd16(DCS_Signal d);
extern uint32_t ApiDcsRd32(DCS_Signal d);
extern UINT64 ApiDcsRd64(DCS_Signal d);
extern BOOL ApiDcsRdbool(DCS_Signal d);
extern void ApiDcsRdbytes(DCS_Signal d , UINT8* dest,  UINT8 n, UINT8 o);
extern BOOL ApiDcsTestRxFlag(const DCS_controllerConfig* dcs_ctlcfg, DCS_RxSigf rxframe);
extern void ApiDcsClearRxFlag(const DCS_controllerConfig* dcs_ctlcfg, DCS_RxSigf rxframe);
extern void ApiDcsInput(const DCS_controllerConfig* dcs_ctlconfig);
extern void ApiDcsOutput(const DCS_controllerConfig* dcs_ctlconfig);
extern void ApiDcsFmdSet(DCSFMDType_e dcs_fmd);
extern void ApiDcsFmdClr(DCSFMDType_e dcs_fmd);
extern BOOL ApiDcsFmdGet(DCSFMDType_e dcs_fmd);

extern void ApiDcsGetNMMessage(UINT32* id, UINT8* dlc, UINT8* data);
extern void ApiDcsDropNMMessage(void);
extern BOOL ApiDcsIsNMBufferEmpty(void);
extern BOOL ApiDcsBusoffState(const DCS_controllerConfig* dcs_ctlcfg);
extern void ApiDcsAbortSend(const DCS_controllerConfig* dcs_ctlcfg);

extern UINT8 ApiDcsCtlDisConn_Busoff(const DCS_controllerConfig* dcs_ctlcfg);
extern void ApiInputSigFrame(const DCS_controllerConfig* dcs_ctlconfig);
extern void ApiDiagTx(const DCS_controllerConfig* dcs_ctlconfig,uint32_t ID,uint8_t* data);
#endif

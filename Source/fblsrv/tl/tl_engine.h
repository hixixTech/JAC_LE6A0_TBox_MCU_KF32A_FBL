/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    tl_engine.h                                                 */
/* Description:  Header for Transport Layer                                  */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠            基线创建                          */
/*****************************************************************************/


#ifndef BL_TL_H_
#define BL_TL_H_


/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "type.h"
#include "ll_can.h"

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
/*** configuration ***/
#ifndef ADDON_LENGTH
    /* POSRESP_SIZE used to be defined to 32 byte ((((FRAME_COUNT-1u)*7u)+6u)-2u) */
    #define OLD_POSRESP_SIZE    (32u)   /* FRAME_COUNT was set to 5 */
    /* ADDON_LENGTH prevents to set POSRESP_SIZE too short when MAX_DID_NUM_RDBI is 1 */
    #define ADDON_LENGTH        (OLD_POSRESP_SIZE-22u)
#endif
/* Read data by ID uses the longest buffer if MAX_DID_NUM_RDBI is not 1.
    Longest DID is F1A8 (20 byte), DID length is additional 2: 22 byte / DID
    Longest Tx message is 22*MAX_DID_NUM_RDBI
*/

#define MAX_DID_NUM_RDBI    5

#define POSRESP_SIZE            (UINT16)((22u*MAX_DID_NUM_RDBI) + ADDON_LENGTH)

#define NumberOfBlockSize       (0x402u) /* dts0100897137 */
#define RX_MSG_SIZE             (NumberOfBlockSize + 2u)

#define CVTC

/* RFQ_SAIC_066: Padding Data(Byte) for diagnostic CAN frame */
#ifndef PADDING_PATTERN
#ifdef SAIC
#define PADDING_PATTERN         (0xAAU)   /*填充0xAA*/
#elif defined (CVTC)
#define PADDING_PATTERN         (0x00U)   /*填充0x00*/
#endif
#endif

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/
typedef enum 
{   
    TlNotReceived=0, 
	TlNewReceived, 
	TlMultipleFrameIsReceiving, 
	TlReceiveFinished, 
	TlReceiveAborted, 
	TlNotTransmitted, 
	TlTransmitted, 
	TlMultipleFrameTransmitInProgress, 
	TlTransmitAborted
}TlStateType_e;

typedef enum 
{
    SingleFrame = 0,
	FirstFrame,
	ConsecutiveFrame,
	FlowControl
}TlFrameType_e;

typedef enum
{
    ContinueToSend = 0, 
	Wait = 1, 
	Overflow = 2
}FsType_e;

typedef struct{
    UINT8  u8Data[RX_MSG_SIZE];
    UINT16 u16Inptr;
    UINT16 u16Outptr;
    UINT16 u16Length;
    TlFrameType_e eFrameType;
    AddressType_e eAtyp;
}Message_t;

typedef struct{
    UINT8   u8Data[POSRESP_SIZE];
    UINT16  u16DataLen;
    UINT8 u8Inptr;
    UINT8 u8Outptr;
    bool bGo;
} TxBufferType_t;

/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开变量申明                                                              */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开函数声明                                                          */
/*---------------------------------------------------------------------------*/
extern TlStateType_e ApiTlRxEngine(Message_t *ptMessage);
extern TlStateType_e ApiTlTxEngine(UINT8 *pu8TxData, UINT16 u16DataLength);
extern TlStateType_e ApiTlSegmentedTxEngine(void);
extern UINT8 *ApiTlGetTlBuffer(void);

#endif /* BL_TL_H_ */

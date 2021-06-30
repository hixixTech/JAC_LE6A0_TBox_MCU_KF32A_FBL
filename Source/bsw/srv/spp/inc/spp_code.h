#ifndef _DIAS_SPP_CODE
#define _DIAS_SPP_CODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spp_type.h"

#define DIAS_SPP_PACKET_LEN_MIN 16 //aucData is null

#define DIAS_SPP_PACKET_LEN_MAX 32   
#define DIAS_SPP_PACKET_BUF_LEN_MAX DIAS_SPP_PACKET_LEN_MAX*4 
#define DIAS_SPP_PACKET_DATA_LEN_MAX (DIAS_SPP_PACKET_LEN_MAX - DIAS_SPP_PACKET_LEN_MIN) 
#define DIAS_SPP_DATA_LEN_MAX (DIAS_SPP_PACKET_DATA_LEN_MAX - 4)   

#define DIAS_SPP_RECV_PACKET_LEN_MAX DIAS_SPP_PACKET_LEN_MAX*8
#define DIAS_SPP_RECV_PACKET_BUF_LEN_MAX DIAS_SPP_RECV_PACKET_LEN_MAX*4 
//#define DIAS_SPP_RECV_PACKET_DATA_LEN_MAX ()
//#define DIAS_SPP_RECV_DATA_LEN_MAX (444)
#define DIAS_SPP_RECV_PACKET_DATA_LEN_MAX (DIAS_SPP_PACKET_LEN_MAX*8-DIAS_SPP_PACKET_LEN_MIN*8)
#define DIAS_SPP_RECV_DATA_LEN_MAX (DIAS_SPP_RECV_PACKET_DATA_LEN_MAX-4)

#define DIAS_SPP_HEAD_DLE 0x0D 
#define DIAS_SPP_HEAD_SOM 0x05 
#define DIAS_SPP_TAIL_DLE 0x0D 
#define DIAS_SPP_TAIL_EOF 0xA0 
#pragma pack(1)

typedef enum
{
	DIAS_SPP_HANDSHAKE = 1,
	DIAS_SPP_HANDSHAKE_ACK,
	DIAS_SPP_HEART,
	DIAS_SPP_HEART_ACK,
	DIAS_SPP_REQ,
	DIAS_SPP_REQ_ACK,
	DIAS_SPP_NOTIFY,
	DIAS_SPP_NOTIFY_ACK,
	DIAS_SPP_REPORT,
}DiaSppControlType_e;

typedef enum
{
	DIAS_SPP_PRI_LOW = 1,
	DIAS_SPP_PRI_NORMAL,
	DIAS_SPP_PRI_MEDIUM,
	DIAS_SPP_PRI_HIGH,
}DiaSppPriorityType_e;

typedef struct
{
    UINT8 u8DleHeader;          /*Header  0x0D*/
    UINT8 u8SomHeader;          /*Header  0x05*/
	UINT8 u8ControlType;        /* ControlType HandShake: 0x01 HandShake ack: 0x02 Heart：0x03 Heart ack：0x04 Req: 0x05 Req ack:0x06 Notify：0x07 Notify ack：0x08 Report：0x09*/
	UINT8 u8Priority;           /* Priority  unknown: 0x00 low：0x01 nomal：0x02 medium：0x03 high：0x04 */
	UINT16 u16Cmd;              /* Command Id*/
	UINT16 u16DataLen;          /* PacketLength: length of aucData */
	UCHAR aucData[DIAS_SPP_PACKET_DATA_LEN_MAX];      /* Data: dcpd data MCU_DATA_MAX_LEN依据mcu能力定义 */	
	UINT8 u8AllNum;           /* Reserved1 all num of splited packet*/
	UINT8 u8CurNum;           /* Reserved2 cur num of splited packet, begin from 1*/
	UINT8 u8Reserved3;           /* Reserved3 */
	UINT8 u8Reserved4;           /* Reserved4 */
	UINT8 u8Roll;                /* Roll for Req and ack*/
	UINT8 u8Crc;                 /* Crc : */
	UINT8 u8DleTrail; 			 /* Tail 0x0D */
    UINT8 u8EofTail;			 /* Tail 0xA0 */
}DiasSppMcuMsg_t;

typedef struct
{
    UINT8 u8DleHeader;          /*Header  0x0D*/
    UINT8 u8SomHeader;          /*Header  0x05*/
	UINT8 u8ControlType;        /* ControlType HandShake: 0x01 HandShake ack: 0x02 Heart：0x03 Heart ack：0x04 Req: 0x05 Req ack:0x06 Notify：0x07 Notify ack：0x08 Report：0x09*/
	UINT8 u8Priority;           /* Priority  unknown: 0x00 low：0x01 nomal：0x02 medium：0x03 high：0x04 */
	UINT16 u16Cmd;              /* Command Id*/
	UINT16 u16DataLen;          /* PacketLength: length of aucData */
	UCHAR aucData[DIAS_SPP_RECV_PACKET_DATA_LEN_MAX];      /* Data: dcpd data MCU_DATA_MAX_LEN依据mcu能力定义 */	
	UINT8 u8AllNum;           /* Reserved1 all num of splited packet*/
	UINT8 u8CurNum;           /* Reserved2 cur num of splited packet, begin from 1*/
	UINT8 u8Reserved3;           /* Reserved3 */
	UINT8 u8Reserved4;           /* Reserved4 */
	UINT8 u8Roll;                /* Roll for Req and ack*/
	UINT8 u8Crc;                 /* Crc : */
	UINT8 u8DleTrail; 			 /* Tail 0x0D */
    UINT8 u8EofTail;			 /* Tail 0xA0 */
}DiasSppRecvMcuMsg_t;  


typedef struct
{
	UINT8 u8SrcSrvId;         
	UINT8 u8DstSrvId;         
	UINT16 u16EventId;     
}DiasSppPackageHeader_t; 

typedef struct
{
	DiasSppPackageHeader_t tHeader; 
	UCHAR ucBuf[DIAS_SPP_RECV_DATA_LEN_MAX];     
}DiasSppPackage_t; 
#pragma pack()

DiasErrorCode_e DiasSppMsgDecode(UCHAR* pucBuf, UINT32 u32len, DiasSppMcuMsg_t* ptDestMsg);
DiasErrorCode_e DiasSppDumpMsg(DiasSppMcuMsg_t* DiasSppMcuMsg_t);
DiasErrorCode_e DiasSppDumpRecvMsg(DiasSppRecvMcuMsg_t* ptMsg);
DiasErrorCode_e DiasSppMsgEncode(UCHAR* pucDestBuf, UINT32 u32Buflen, UINT32* u32len, DiasSppMcuMsg_t* ptMsg);
DiasErrorCode_e DiasSppGetCrc(DiasSppMcuMsg_t* ptMsg,UINT8* pu8Crc);
UINT8* DiasSppFindMsgHead(UCHAR* pucBegin, UCHAR* pucEnd);
DiasErrorCode_e DiasSppGetAbsTime(/*struct timeval *ptTime*/void *pvTime);
DiasErrorCode_e DiasSppMakeCompleteMsg(DiasSppMcuMsg_t* ptCurMsg,DiasSppRecvMcuMsg_t* ptAllMsg);

#endif //_DIAS_SPP_CODE

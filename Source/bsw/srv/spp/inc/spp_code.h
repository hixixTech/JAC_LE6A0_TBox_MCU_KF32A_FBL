#ifndef _SPP_CODE
#define _SPP_CODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spp_type.h"

#define SPP_PACKET_LEN_MIN 16 //EventLen




#define SPP_BIG_PACKET_LEN_MIN 8
#define SPP_BIG_PACKET_LEN_MAX 128
#define SPP_BIG_PACKET_PAYLOAG_LEN_MAX (SPP_BIG_PACKET_LEN_MAX - SPP_BIG_PACKET_LEN_MIN)
#define SPP_BIG_PACKET_DATA_LEN_MAX (SPP_BIG_PACKET_PAYLOAG_LEN_MAX - SPP_PACKET_LEN_MIN)
#define SPP_BIG_PACKET_EVENT_NUM_MAX 10


#define SPP_PACKET_LEN_MAX SPP_BIG_PACKET_PAYLOAG_LEN_MAX      /*256 - 7*/
#define SPP_PACKET_BUF_LEN_MAX (SPP_PACKET_LEN_MAX*4) 
#define SPP_PACKET_DATA_LEN_MAX (SPP_PACKET_LEN_MAX - SPP_PACKET_LEN_MIN)    /*257 - 7 - 16*/

#define SPP_RECV_PACKET_LEN_MAX (SPP_PACKET_LEN_MAX*8)
#define SPP_RECV_PACKET_BUF_LEN_MAX (SPP_RECV_PACKET_LEN_MAX*4) 


#define SPP_RECV_PACKET_DATA_LEN_MAX (SPP_BIG_PACKET_DATA_LEN_MAX*12)
#define SPP_RECV_DATA_LEN_MAX (SPP_RECV_PACKET_DATA_LEN_MAX-4)

#define SPP_MSG_MAX_LEN (1024)



#define SPP_HEAD_DLE 0x0D 
#define SPP_HEAD_SOM 0x05 
#define SPP_TAIL_DLE 0x0D 
#define SPP_TAIL_EOF 0xA0 


#define SPP_PACKET_DATA_FILL 0xff


#define SPP_PACKET_HEAD_DLE 0xA5
#define SPP_PACKET_HEAD_SOM 0x5A
#define SPP_PACKET_TAIL_DLE 0xD5
#define SPP_PACKET_TAIL_EOF 0x5D


#define SPP_PACKET_TYPE_BLOCK 0x01
#define SPP_PACKET_TYPE_ACK 0x02
#define SPP_PACKET_TYPE_NONBLOCK 0x03
#define SPP_PACKET_TYPE_ACK_NON 0x04
#define SPP_PACKET_TYPE_HB   0x05
#define SPP_PACKET_TYPE_ACK_HB 0x06
#pragma pack(1)

typedef enum
{
	SPP_HANDSHAKE = 1,
	SPP_HANDSHAKE_ACK,
	SPP_HEART,
	SPP_HEART_ACK,
	SPP_REQ,
	SPP_REQ_ACK,
	SPP_NOTIFY,
	SPP_NOTIFY_ACK,
	SPP_REPORT,
}SppControlType_e;

typedef enum
{
	SPP_PRI_LOW = 1,
	SPP_PRI_NORMAL,
	SPP_PRI_MEDIUM,
	SPP_PRI_HIGH,
}SppPriorityType_e;
typedef struct
{
    UINT8 u8Hdle;
    UINT8 u8Hsom;
    UINT8 u8Type;
    UINT8 u8Sn;
    UINT8 u8Sta;
    UINT8 au8PacketPayload[SPP_BIG_PACKET_PAYLOAG_LEN_MAX];
    UINT8 u8Crc;
    UINT8 u8Tdle;
    UINT8 u8Teof;
}SppPacketType;


typedef struct
{
    UINT8 u8DleHeader;          /*Header  0x0D*/
    UINT8 u8SomHeader;          /*Header  0x05*/
	UINT8 u8ControlType;        /* ControlType HandShake: 0x01 HandShake ack: 0x02 Heart：0x03 Heart ack：0x04 Req: 0x05 Req ack:0x06 Notify：0x07 Notify ack：0x08 Report：0x09*/
	UINT8 u8Priority;           /* Priority  unknown: 0x00 low：0x01 nomal：0x02 medium：0x03 high：0x04 */
	UINT16 u16Cmd;              /* Command Id*/
	UINT16 u16DataLen;          /* PacketLength: length of aucData */
	UCHAR aucData[SPP_PACKET_DATA_LEN_MAX];      /* Data: dcpd data MCU_DATA_MAX_LEN依据mcu能力定义 */	
	UINT8 u8AllNum;           /* Reserved1 all num of splited packet*/
	UINT8 u8CurNum;           /* Reserved2 cur num of splited packet, begin from 1*/
	UINT8 u8Reserved3;           /* Reserved3 */
	UINT8 u8Reserved4;           /* Reserved4 */
	UINT8 u8Roll;                /* Roll for Req and ack*/
	UINT8 u8Crc;                 /* Crc : */
	UINT8 u8DleTrail; 			 /* Tail 0x0D */
    UINT8 u8EofTail;			 /* Tail 0xA0 */
}SppMcuMsg_t;

typedef struct
{
    UINT8 u8DleHeader;          /*Header  0x0D*/
    UINT8 u8SomHeader;          /*Header  0x05*/
	UINT8 u8ControlType;        /* ControlType HandShake: 0x01 HandShake ack: 0x02 Heart：0x03 Heart ack：0x04 Req: 0x05 Req ack:0x06 Notify：0x07 Notify ack：0x08 Report：0x09*/
	UINT8 u8Priority;           /* Priority  unknown: 0x00 low：0x01 nomal：0x02 medium：0x03 high：0x04 */
	UINT16 u16Cmd;              /* Command Id*/
	UINT16 u16DataLen;          /* PacketLength: length of aucData */
	UCHAR aucData[SPP_RECV_PACKET_DATA_LEN_MAX];      /* Data: dcpd data MCU_DATA_MAX_LEN依据mcu能力定义 */	
	UINT8 u8AllNum;           /* Reserved1 all num of splited packet*/
	UINT8 u8CurNum;           /* Reserved2 cur num of splited packet, begin from 1*/
	UINT8 u8Reserved3;           /* Reserved3 */
	UINT8 u8Reserved4;           /* Reserved4 */
	UINT8 u8Roll;                /* Roll for Req and ack*/
	UINT8 u8Crc;                 /* Crc : */
	UINT8 u8DleTrail; 			 /* Tail 0x0D */
    UINT8 u8EofTail;			 /* Tail 0xA0 */
}SppRecvMcuMsg_t;  


typedef struct
{
	UINT8 u8SrcSrvId;         
	UINT8 u8DstSrvId;         
	UINT16 u16EventId;     
}SppPackageHeader_t; 

typedef struct
{
	SppPackageHeader_t tHeader; 
	UCHAR ucBuf[SPP_RECV_DATA_LEN_MAX];     
}SppPackage_t; 
#pragma pack()
SPPErrorCode_e SppPacketDecode(UCHAR* pucBuf, UINT32 u32len, SppPacketType* ptPacket);
SPPErrorCode_e SppMsgDecode(UCHAR* pucBuf, UINT32 u32len, SppMcuMsg_t* ptDestMsg);
SPPErrorCode_e SppDumpMsg(SppMcuMsg_t* SppMcuMsg_t);
SPPErrorCode_e SppDumpRecvMsg(SppRecvMcuMsg_t* ptMsg);
SPPErrorCode_e SppMsgEncode(UCHAR* pucDestBuf, UINT32 u32Buflen, UINT32* u32len, SppMcuMsg_t* ptMsg);
SPPErrorCode_e SppPacketEncode(UCHAR* pucDestBuf, UINT32 u32Buflen, UINT32* u32len, SppPacketType* ptPackt);
SPPErrorCode_e SppGetPacketCrc(SppPacketType* ptPacket,UINT8* pu8Crc);
SPPErrorCode_e SppGetCrc(SppMcuMsg_t* ptMsg,UINT8* pu8Crc);
UINT8* SppFindMsgHead(UCHAR* pucBegin, UCHAR* pucEnd);
SPPErrorCode_e SppMakeCompleteMsg(SppMcuMsg_t* ptCurMsg,SppRecvMcuMsg_t* ptAllMsg);

#endif //_SPP_CODE

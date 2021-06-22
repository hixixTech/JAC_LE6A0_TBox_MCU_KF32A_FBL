//#include <sys/syscall.h>
//#include <sys/time.h>
#include "time.h"
#include "spp_main.h"
#include "spp_memory.h"
#include "spp_code.h"
#include "spp_type.h"
#include "os_log.h"



/******************************************************************************
*  function name | SppPacketDecode
*  content       | decode the packet
*  parameter[1]  | pucBuf :message buffer
*  parameter[2]  | u32len :buffer len
*  parameter[3]  | ptPacket :packet address
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppPacketDecode(UCHAR* pucBuf, UINT32 u32len, SppPacketType* ptPacket)
{
    UINT8 u8Crc = 0;
    UINT32 u32Pos = 0;
	SPPErrorCode_e eRet = SPP_FAILURE;
    if ((pucBuf == NULL) || (ptPacket == NULL) || (u32len < SPP_BIG_PACKET_LEN_MIN))
	{
		return SPP_FAILURE;
	}	
    //ApiLogPrint(_LOG_ENTRY, "decode\n");
	//ApiLogPrint(_LOG_ENTRY, "type%2x sn %2x\n",pucBuf[2],pucBuf[3]);
    ptPacket->u8Hdle = pucBuf[u32Pos++];
    ptPacket->u8Hsom = pucBuf[u32Pos++];
	ptPacket->u8Type = pucBuf[u32Pos++];
    ptPacket->u8Sn = pucBuf[u32Pos++]; 
    ptPacket->u8Sta = pucBuf[u32Pos++];
    switch (ptPacket->u8Type)
    {
        case SPP_PACKET_TYPE_BLOCK:
        case SPP_PACKET_TYPE_NONBLOCK:
            memcpy(ptPacket->au8PacketPayload,pucBuf+u32Pos,SPP_BIG_PACKET_PAYLOAG_LEN_MAX);
            u32Pos += SPP_BIG_PACKET_PAYLOAG_LEN_MAX;
            break;
        case SPP_PACKET_TYPE_ACK:
        case SPP_PACKET_TYPE_ACK_NON:
        case SPP_PACKET_TYPE_HB:
        case SPP_PACKET_TYPE_ACK_HB:
            memset(ptPacket->au8PacketPayload,0xff,SPP_BIG_PACKET_PAYLOAG_LEN_MAX);
            break;
        default:
            return SPP_FAILURE;
            break;
    }
    ptPacket ->u8Crc = pucBuf[u32Pos++];
	eRet=SppGetPacketCrc(ptPacket, &u8Crc);
    if((SPP_FAILURE==eRet)||(u8Crc != ptPacket->u8Crc))
	{
        ApiLogPrint(_LOG_ENTRY, "CrcErr%d %d \n",ptPacket->u8Crc,u8Crc);
		return SPP_FAILURE;
	}
    	
    ptPacket->u8Tdle = pucBuf[u32Pos++];
	ptPacket->u8Teof = pucBuf[u32Pos++]; 

    
    if(ptPacket->u8Hdle != SPP_PACKET_HEAD_DLE 
        || ptPacket->u8Hsom != SPP_PACKET_HEAD_SOM
        || ptPacket->u8Tdle != SPP_PACKET_TAIL_DLE  
        || ptPacket->u8Teof != SPP_PACKET_TAIL_EOF 
        || u32Pos > u32len)
    {
        ApiLogPrint(_LOG_ENTRY, "decodErr\n");
		return SPP_FAILURE;
    }

    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppDecode
*  content       | 接收数据解码
*  parameter     | pucBuf 接收数据 u32len 接收数据长度 ptMsg解码目标结构体指针
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppDecode(UCHAR* pucBuf, UINT32 u32len, SppMcuMsg_t* ptMsg)
{
    UINT8 u8Crc = 0;
    UINT32 u32Pos = 0;
	SPPErrorCode_e eRet = SPP_FAILURE;
    if ((pucBuf == NULL) || (ptMsg == NULL) || (u32len < SPP_PACKET_LEN_MIN))
	{
		return SPP_FAILURE;
	}	
	
    ptMsg->u8DleHeader = pucBuf[u32Pos++];
    ptMsg->u8SomHeader = pucBuf[u32Pos++];
	ptMsg->u8ControlType = pucBuf[u32Pos++];
	ptMsg->u8Priority = pucBuf[u32Pos++];	
	
	memcpy(&ptMsg->u16Cmd, pucBuf+u32Pos, sizeof(ptMsg->u16Cmd));
    u32Pos += (INT32)sizeof(ptMsg->u16Cmd);	
    memcpy(&ptMsg->u16DataLen, pucBuf+u32Pos, sizeof(ptMsg->u16DataLen));
    u32Pos += (INT32)sizeof(ptMsg->u16DataLen);	
	
    if(ptMsg->u16DataLen > SPP_PACKET_DATA_LEN_MAX)
	{
		return SPP_FAILURE;
	}	
    memcpy(ptMsg->aucData, pucBuf+u32Pos, ptMsg->u16DataLen);
    u32Pos += ptMsg->u16DataLen;
	
	ptMsg->u8AllNum = pucBuf[u32Pos++]; 
	ptMsg->u8CurNum = pucBuf[u32Pos++]; 
	ptMsg->u8Reserved3 = pucBuf[u32Pos++]; 
	ptMsg->u8Reserved4 = pucBuf[u32Pos++]; 	
	ptMsg->u8Roll = pucBuf[u32Pos++];     	
    ptMsg->u8Crc = pucBuf[u32Pos++];
    
	eRet=SppGetCrc(ptMsg, &u8Crc);
    if((SPP_FAILURE==eRet)||(u8Crc != ptMsg->u8Crc))
	{
        ApiLogPrint(_LOG_ENTRY, "CrcErr%d %d \n",ptMsg->u8Crc,u8Crc);
		return SPP_FAILURE;
	}
	
    ptMsg->u8DleTrail = pucBuf[u32Pos++];
    ptMsg->u8EofTail = pucBuf[u32Pos++];	
    if(u32Pos > u32len)
	{
        ApiLogPrint(_LOG_ENTRY, "decodErr\n");
		return SPP_FAILURE;
	}

    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppDumpMsg
*  content       | Msg打印
*  parameter     | ptMsg Msg结构体指针
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppDumpMsg(SppMcuMsg_t* ptMsg)
{
    INT32 i = 0;	
    static CHAR acDataLog[SPP_PACKET_LEN_MAX*3 + 1] ={0};
	
    if ((ptMsg == NULL)&&(ptMsg->u16DataLen>SPP_PACKET_LEN_MAX))
	{
		return SPP_FAILURE;
	}
	

    memset(acDataLog,0,sizeof(acDataLog));
    for(i=0; i<ptMsg->u16DataLen; i++)
    {
        //snprintf(acDataLog, sizeof(acDataLog), "%s %02x", acDataLog, ptMsg->aucData[i]);
    }


	return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppDumpRecvMsg
*  content       | Msg打印
*  parameter     | ptMsg Msg结构体指针
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppDumpRecvMsg(SppRecvMcuMsg_t* ptMsg)
{
    INT32 i = 0;	
    static CHAR acDataLog[SPP_PACKET_LEN_MAX*3 + 1] ={0};
	
    if ((ptMsg == NULL)&&(ptMsg->u16DataLen>SPP_RECV_PACKET_LEN_MAX))
	{
		return SPP_FAILURE;
	}
	

    (void)memset(acDataLog, 0, sizeof(acDataLog));
    for(i=0; i<ptMsg->u16DataLen; i++)
    {        
    }


	return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppPacketEncode
*  content       | encode the packet
*  parameter[1]  | pucDestBuf :
*  parameter[2]  | u32Buflen :
*  parameter[3]  | u32len :
*  parameter[4]  | ptPackt :
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppPacketEncode(UCHAR* pucDestBuf, UINT32 u32Buflen, UINT32* u32len, SppPacketType* ptPackt)
{   
    INT32 i32Index = 0;	
    if ((pucDestBuf == NULL) || (u32len == NULL) ||(ptPackt == NULL))
	{
		return SPP_FAILURE;
	}	
    
    if( u32Buflen < SPP_BIG_PACKET_LEN_MAX )
    {
		return SPP_FAILURE;
	}
  
    pucDestBuf[i32Index++] = SPP_PACKET_HEAD_DLE;
	pucDestBuf[i32Index++] = SPP_PACKET_HEAD_SOM;
    pucDestBuf[i32Index++] = ptPackt->u8Type;
	pucDestBuf[i32Index++] = ptPackt->u8Sn;
	pucDestBuf[i32Index++] = ptPackt->u8Sta;
    if (ptPackt->u8Type == SPP_PACKET_TYPE_BLOCK
        ||ptPackt->u8Type == SPP_PACKET_TYPE_NONBLOCK)
    {
        memcpy(pucDestBuf+i32Index, ptPackt->au8PacketPayload, SPP_BIG_PACKET_PAYLOAG_LEN_MAX);
        i32Index += SPP_BIG_PACKET_PAYLOAG_LEN_MAX;
    }
    else
    {}    
	pucDestBuf[i32Index++] = ptPackt->u8Crc;
	pucDestBuf[i32Index++] = SPP_PACKET_TAIL_DLE;
	pucDestBuf[i32Index++] = SPP_PACKET_TAIL_EOF;
	
    *u32len = i32Index;
    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppMsgEncode
*  content       | 发送数据编码
*  parameter     | pucDestBuf 发送数据 u32Buflen pucDestBuf长度 u32len 发送数据长度 ptMsg 发送Msg结构体指针
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppMsgEncode(UCHAR* pucDestBuf, UINT32 u32Buflen, UINT32* u32len, SppMcuMsg_t* ptMsg)
{   
    INT32 i32Index = 0;	
    if ((pucDestBuf == NULL) || (u32len == NULL) ||(ptMsg == NULL))
	{
		return SPP_FAILURE;
	}	
    
    if( u32Buflen < (ptMsg->u16DataLen+SPP_PACKET_LEN_MIN) )
    {
		return SPP_FAILURE;
	}
  
    pucDestBuf[i32Index++] = SPP_HEAD_DLE;
	pucDestBuf[i32Index++] = SPP_HEAD_SOM;
    pucDestBuf[i32Index++] = ptMsg->u8ControlType;
	pucDestBuf[i32Index++] = ptMsg->u8Priority;
   
    memcpy(pucDestBuf+i32Index, &ptMsg->u16Cmd, sizeof(ptMsg->u16Cmd));
    i32Index += (INT32)sizeof(ptMsg->u16Cmd);
    memcpy(pucDestBuf+i32Index,  &ptMsg->u16DataLen, sizeof(ptMsg->u16DataLen));
    i32Index += (INT32)sizeof(ptMsg->u16DataLen);
    memcpy(pucDestBuf+i32Index, ptMsg->aucData, ptMsg->u16DataLen);	
    i32Index += ptMsg->u16DataLen;
	
	pucDestBuf[i32Index++] = ptMsg->u8AllNum;
	pucDestBuf[i32Index++] = ptMsg->u8CurNum;
	pucDestBuf[i32Index++] = ptMsg->u8Reserved3;
	pucDestBuf[i32Index++] = ptMsg->u8Reserved4;
	pucDestBuf[i32Index++] = ptMsg->u8Roll;
    pucDestBuf[i32Index++] = ptMsg->u8Crc;
    pucDestBuf[i32Index++] = SPP_TAIL_DLE;
	pucDestBuf[i32Index++] = SPP_TAIL_EOF;
	
    *u32len = i32Index;
    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppGetPacketCrc
*  content       | get crc value of packet
*  parameter[1]  | ptPacket :
*  parameter[2]  | pu8Crc :
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppGetPacketCrc(SppPacketType* ptPacket,UINT8* pu8Crc)
{   
	INT32 i = 0;
    UINT8 u8Crc = 0;
	
	if((NULL == ptPacket)||(NULL == pu8Crc))
	{
		return SPP_FAILURE;
	}
	
	u8Crc ^= ptPacket->u8Type;
    u8Crc ^= ptPacket->u8Sn;
    u8Crc ^= ptPacket->u8Sta;
    if (ptPacket->u8Type == SPP_PACKET_TYPE_BLOCK
        ||ptPacket->u8Type == SPP_PACKET_TYPE_NONBLOCK)
    {
        for(i=0; i<SPP_BIG_PACKET_PAYLOAG_LEN_MAX; i++)
        {
            u8Crc ^= ptPacket->au8PacketPayload[i];
        }
    }
	
	*pu8Crc = u8Crc;
	
    return SPP_SUCCESS;
}



/******************************************************************************
*  function name | SppGetCrc
*  content       | 获取Crc校验结果
*  parameter     | ptMsg Msg结构体指针 pu8Crc 获取的crc值
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppGetCrc(SppMcuMsg_t* ptMsg,UINT8* pu8Crc)
{   
	INT32 i = 0;
    UINT8 u8Crc = 0;
    UINT8* pu8Pos = NULL;
	
	if((NULL == ptMsg)||(NULL == pu8Crc))
	{
		return SPP_FAILURE;
	}
	
	pu8Pos = (UINT8*)&ptMsg->u16Cmd;
	u8Crc = pu8Pos[0] ^ pu8Pos[1];
    pu8Pos = (UINT8*)&ptMsg->u16DataLen;
    u8Crc ^= pu8Pos[0] ^ pu8Pos[1];
    for(i=0; i<ptMsg->u16DataLen; i++)
    {
        u8Crc ^= ptMsg->aucData[i];
    }
	
	*pu8Crc = u8Crc;
	
    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppFindMsgHead
*  content       | 查找Msg 头
*  parameter     | pucBegin 起始地址     pucEnd 结束地址
*  return        | UINT8* 查找到的地址
*  notice        | 
******************************************************************************/
UINT8* SppFindMsgHead(UCHAR* pucBegin, UCHAR* pucEnd)
{
	UCHAR* pucIndex = NULL;
	if(NULL == pucBegin || NULL == pucEnd || pucBegin >= pucEnd)
	{
		return NULL;
	}
	
	for(pucIndex = pucBegin; pucIndex < pucEnd; pucIndex++)
	{
		if((pucIndex[0] == SPP_HEAD_DLE) && (pucIndex[1] == SPP_HEAD_SOM))
		{
			return pucIndex;
		}
	}
	return NULL;
}


/******************************************************************************
*  function name | SppMsgDecode
*  content       | 接收数据解码对外接口
*  parameter     | pucBuf 接收数据 u32len 接收数据长度 ptMsg解码目标结构体指针
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppMsgDecode(UCHAR* pucBuf, UINT32 u32len, SppMcuMsg_t* ptDestMsg)
{
    INT32 i32ret = 0;
    if(0 == u32len)
    {
        return SPP_FAILURE;
    }

    i32ret = SppDecode(pucBuf, u32len, ptDestMsg);
    if(SPP_SUCCESS != i32ret)
    {
        return i32ret;
    }
	
    if(ptDestMsg->u8DleHeader != SPP_HEAD_DLE || ptDestMsg->u8SomHeader != SPP_HEAD_SOM)
    {
        return SPP_FAILURE;
    }
	
    if(ptDestMsg->u8DleTrail != SPP_TAIL_DLE || ptDestMsg->u8EofTail != SPP_TAIL_EOF)
    {
        return SPP_FAILURE;
    }

    return SPP_SUCCESS;
}



/******************************************************************************
*  function name | SppMakeCompleteMsg
*  content       | 接收数据组包
*  parameter     | ptCurMsg 当前包指针 ptAllMsg 全包指针
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppMakeCompleteMsg(SppMcuMsg_t* ptCurMsg,SppRecvMcuMsg_t* ptAllMsg)
{	
    if (ptCurMsg == NULL || ptAllMsg == NULL)
	{
		return SPP_FAILURE;
	}	
    if (ptCurMsg->u8CurNum == 1)
    {
        memset(ptAllMsg,0,sizeof(SppRecvMcuMsg_t));
    }
    //ApiLogPrint(_LOG_ENTRY, "MsgMake%d %d %d %d %d %d\n", ptCurMsg->u16Cmd,ptCurMsg->u8CurNum,ptAllMsg->u16Cmd,ptAllMsg->u8CurNum,u8_numPre,u16_cmdPre);
    if (ptCurMsg->u8AllNum >1)
    {
        if (ptCurMsg->u8CurNum>1)
        {
            if ((ptCurMsg->u16Cmd == ptAllMsg->u16Cmd)
                &&(ptCurMsg->u8CurNum == (ptAllMsg->u8CurNum+1)))
            {
            }
            else if((ptCurMsg->u16Cmd == ptAllMsg->u16Cmd)
                &&(ptCurMsg->u8CurNum == ptAllMsg->u8CurNum))  /*重复包处理*/
            {
                return SPP_SUCCESS;
            }
            else
            {                
                ApiLogPrint(_LOG_ENTRY, "MsgMakeErr%d", ptCurMsg->u16Cmd);
                return SPP_FAILURE;
            }
        }
        else if((ptCurMsg->u16Cmd == ptAllMsg->u16Cmd)
            &&(ptCurMsg->u8CurNum == ptAllMsg->u8CurNum))/*重复包处理*/
        {
            return SPP_SUCCESS;
        }
        else
        {
        }
        
    }
    else
    {
    }
	ptAllMsg->u8DleHeader = ptCurMsg->u8DleHeader;
	ptAllMsg->u8SomHeader = ptCurMsg->u8SomHeader;
	ptAllMsg->u8ControlType = ptCurMsg->u8ControlType;
	ptAllMsg->u8Priority = ptCurMsg->u8Priority;
	ptAllMsg->u16Cmd = ptCurMsg->u16Cmd;	
    if(1==ptCurMsg->u8AllNum)
	{
		memcpy(ptAllMsg->aucData, ptCurMsg->aucData, ptCurMsg->u16DataLen);
	}
	else
	{
		memcpy(ptAllMsg->aucData+ptAllMsg->u16DataLen, ptCurMsg->aucData, ptCurMsg->u16DataLen);
	}	
	ptAllMsg->u16DataLen = ptAllMsg->u16DataLen + ptCurMsg->u16DataLen;
	ptAllMsg->u8AllNum = ptCurMsg->u8AllNum; 
	ptAllMsg->u8CurNum = ptCurMsg->u8CurNum; 
	ptAllMsg->u8Reserved3 = ptCurMsg->u8Reserved3; 
	ptAllMsg->u8Reserved4 = ptCurMsg->u8Reserved4;	
	ptAllMsg->u8Roll = ptCurMsg->u8Roll;		
	ptAllMsg->u8Crc = ptCurMsg->u8Crc;
	ptAllMsg->u8DleTrail = ptCurMsg->u8DleTrail;
	ptAllMsg->u8EofTail = ptCurMsg->u8EofTail;
	return SPP_SUCCESS;

}


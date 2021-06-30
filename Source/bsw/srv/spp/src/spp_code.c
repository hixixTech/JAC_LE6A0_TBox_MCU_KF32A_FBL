//#include <sys/syscall.h>
//#include <sys/time.h>
#include "time.h"
#include "spp_main.h"
#include "spp_memory.h"
#include "spp_code.h"
#include "spp_type.h"
#include "os_log.h"

/******************************************************************************
*  function name | DiasSppDecode
*  content       | 接收数据解码
*  parameter     | pucBuf 接收数据 u32len 接收数据长度 ptMsg解码目标结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppDecode(UCHAR* pucBuf, UINT32 u32len, DiasSppMcuMsg_t* ptMsg)
{
    UINT8 u8Crc = 0;
    UINT32 u32Pos = 0;
	DiasErrorCode_e eRet = DIAS_FAILURE;
    if ((pucBuf == NULL) || (ptMsg == NULL) || (u32len < DIAS_SPP_PACKET_LEN_MIN))
	{
        DiasDcpdLog(LOG_ERR,"DiasSppDecode: wrong para");
		return DIAS_FAILURE;
	}	
	
    ptMsg->u8DleHeader = pucBuf[u32Pos++];
    ptMsg->u8SomHeader = pucBuf[u32Pos++];
	ptMsg->u8ControlType = pucBuf[u32Pos++];
	ptMsg->u8Priority = pucBuf[u32Pos++];	
	
	memcpy(&ptMsg->u16Cmd, pucBuf+u32Pos, sizeof(ptMsg->u16Cmd));
    u32Pos += (INT32)sizeof(ptMsg->u16Cmd);	
    memcpy(&ptMsg->u16DataLen, pucBuf+u32Pos, sizeof(ptMsg->u16DataLen));
    u32Pos += (INT32)sizeof(ptMsg->u16DataLen);	
	
    if(ptMsg->u16DataLen > DIAS_SPP_PACKET_LEN_MAX)
	{
        DiasDcpdLog(LOG_ERR,"DiasSppDecode error:u32len too big u32len:%d",u32len);
		return DIAS_FAILURE;
	}	
	
    memcpy(ptMsg->aucData, pucBuf+u32Pos, ptMsg->u16DataLen);
    u32Pos += ptMsg->u16DataLen;
	
	ptMsg->u8AllNum = pucBuf[u32Pos++]; 
	ptMsg->u8CurNum = pucBuf[u32Pos++]; 
	ptMsg->u8Reserved3 = pucBuf[u32Pos++]; 
	ptMsg->u8Reserved4 = pucBuf[u32Pos++]; 	
	ptMsg->u8Roll = pucBuf[u32Pos++];     	
    ptMsg->u8Crc = pucBuf[u32Pos++];
    
	eRet=DiasSppGetCrc(ptMsg, &u8Crc);
    if((DIAS_FAILURE==eRet)||(u8Crc != ptMsg->u8Crc))
	{
        DiasDcpdLog(LOG_INFO,"DiasSppDecode error:wrong crc, ptMsg->u8Crc=%d, calc_u8Crc=%d.", ptMsg->u8Crc, u8Crc);
        ApiLogPrint(_LOG_ENTRY, "CrcErr%d %d \n",ptMsg->u8Crc,u8Crc);
		return DIAS_FAILURE;
	}
	
    ptMsg->u8DleTrail = pucBuf[u32Pos++];
    ptMsg->u8EofTail = pucBuf[u32Pos++];	
    if(u32Pos > u32len)
	{
        DiasDcpdLog(LOG_ERR,"DiasSppDecode error:wrong u32len");
        
        ApiLogPrint(_LOG_ENTRY, "decodErr\n");
		return DIAS_FAILURE;
	}

    return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppDumpMsg
*  content       | Msg打印
*  parameter     | ptMsg Msg结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppDumpMsg(DiasSppMcuMsg_t* ptMsg)
{
    INT32 i = 0;	
    static CHAR acDataLog[DIAS_SPP_PACKET_LEN_MAX*3 + 1] ={0};
	
    if ((ptMsg == NULL)&&(ptMsg->u16DataLen>DIAS_SPP_PACKET_LEN_MAX))
	{
        DiasDcpdLog(LOG_ERR,"DiasSppDumpMsg wrong para");
		return DIAS_FAILURE;
	}
	
    DiasDcpdLog(LOG_INFO,"u8DleHeader=%02X;u8SomHeader=%02X;u8ControlType=%02X;u8Priority=%02X;u16Cmd=%d;u16DataLengthd=%d;aucData=", 
                ptMsg->u8DleHeader,ptMsg->u8SomHeader,ptMsg->u8ControlType,ptMsg->u8Priority,ptMsg->u16Cmd,ptMsg->u16DataLen); 

    memset(acDataLog,0,sizeof(acDataLog));
    for(i=0; i<ptMsg->u16DataLen; i++)
    {
        snprintf(acDataLog, sizeof(acDataLog), "%s %02x", acDataLog, ptMsg->aucData[i]);
    }

    DiasDcpdLog(LOG_INFO,acDataLog);
    DiasDcpdLog(LOG_INFO,"u8AllNum=%02X;u8CurNum=%02X;u8Roll=%02X;u8Crc=%02X;u8DleTrail=%02X;u8EofTail=%02X;", ptMsg->u8AllNum,ptMsg->u8CurNum,ptMsg->u8Roll,ptMsg->u8Crc,ptMsg->u8DleTrail,ptMsg->u8EofTail);  

	return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppDumpRecvMsg
*  content       | Msg打印
*  parameter     | ptMsg Msg结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppDumpRecvMsg(DiasSppRecvMcuMsg_t* ptMsg)
{
    INT32 i = 0;	
    static CHAR acDataLog[DIAS_SPP_PACKET_LEN_MAX*3 + 1] ={0};
	
    if ((ptMsg == NULL)&&(ptMsg->u16DataLen>DIAS_SPP_RECV_PACKET_LEN_MAX))
	{
        DiasDcpdLog(LOG_ERR,"DiasSppDumpMsg wrong para");
		return DIAS_FAILURE;
	}
	
    DiasDcpdLog(LOG_INFO,"u8DleHeader=%02X;u8SomHeader=%02X;u8ControlType=%02X;u8Priority=%02X;u16Cmd=%d;u16DataLengthd=%d;aucData=", 
                ptMsg->u8DleHeader,ptMsg->u8SomHeader,ptMsg->u8ControlType,ptMsg->u8Priority,ptMsg->u16Cmd,ptMsg->u16DataLen); 

    (void)memset(acDataLog, 0, sizeof(acDataLog));
    for(i=0; i<ptMsg->u16DataLen; i++)
    {        
		snprintf(acDataLog, sizeof(acDataLog), "%s %02x", acDataLog, ptMsg->aucData[i]);
    }

    DiasDcpdLog(LOG_INFO,acDataLog);
    DiasDcpdLog(LOG_INFO,"u8AllNum=%02X;u8CurNum=%02X;u8Roll=%02X;u8Crc=%02X;u8DleTrail=%02X;u8EofTail=%02X;", ptMsg->u8AllNum,ptMsg->u8CurNum,ptMsg->u8Roll,ptMsg->u8Crc,ptMsg->u8DleTrail,ptMsg->u8EofTail);  

	return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppMsgEncode
*  content       | 发送数据编码
*  parameter     | pucDestBuf 发送数据 u32Buflen pucDestBuf长度 u32len 发送数据长度 ptMsg 发送Msg结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppMsgEncode(UCHAR* pucDestBuf, UINT32 u32Buflen, UINT32* u32len, DiasSppMcuMsg_t* ptMsg)
{   
    INT32 i32Index = 0;	
    if ((pucDestBuf == NULL) || (u32len == NULL) ||(ptMsg == NULL))
	{
        DiasDcpdLog(LOG_ERR,"DiasSppMsgEncode error:wrong para\n");
		return DIAS_FAILURE;
	}	
    
    if( u32Buflen < (ptMsg->u16DataLen+DIAS_SPP_PACKET_LEN_MIN) )
    {
        DiasDcpdLog(LOG_ERR,"DiasSppMsgEncode error:u32Buflen too small\n");
		return DIAS_FAILURE;
	}
  
    pucDestBuf[i32Index++] = DIAS_SPP_HEAD_DLE;
	pucDestBuf[i32Index++] = DIAS_SPP_HEAD_SOM;
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
    pucDestBuf[i32Index++] = DIAS_SPP_TAIL_DLE;
	pucDestBuf[i32Index++] = DIAS_SPP_TAIL_EOF;
	
    *u32len = i32Index;
    return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppGetCrc
*  content       | 获取Crc校验结果
*  parameter     | ptMsg Msg结构体指针 pu8Crc 获取的crc值
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppGetCrc(DiasSppMcuMsg_t* ptMsg,UINT8* pu8Crc)
{   
	INT32 i = 0;
    UINT8 u8Crc = 0;
    UINT8* pu8Pos = NULL;
	
	if((NULL == ptMsg)||(NULL == pu8Crc))
	{
		return DIAS_FAILURE;
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
	
    return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppFindMsgHead
*  content       | 查找Msg 头
*  parameter     | pucBegin 起始地址     pucEnd 结束地址
*  return        | UINT8* 查找到的地址
*  notice        | 
******************************************************************************/
UINT8* DiasSppFindMsgHead(UCHAR* pucBegin, UCHAR* pucEnd)
{
	UCHAR* pucIndex = NULL;
	if(NULL == pucBegin || NULL == pucEnd || pucBegin >= pucEnd)
	{
		return NULL;
	}
	
	for(pucIndex = pucBegin; pucIndex < pucEnd; pucIndex++)
	{
		if((pucIndex[0] == DIAS_SPP_HEAD_DLE) && (pucIndex[1] == DIAS_SPP_HEAD_SOM))
		{
			return pucIndex;
		}
	}
	return NULL;
}

/******************************************************************************
*  function name | DiasSppGetAbsTime
*  content       | 获取时间
*  parameter     | ptTime 获取的时间结构体
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppGetAbsTime(/*struct timeval *ptTime*/void *pvTime)
{

    return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppMsgDecode
*  content       | 接收数据解码对外接口
*  parameter     | pucBuf 接收数据 u32len 接收数据长度 ptMsg解码目标结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppMsgDecode(UCHAR* pucBuf, UINT32 u32len, DiasSppMcuMsg_t* ptDestMsg)
{
    INT32 i32ret = 0;
    if(0 == u32len)
    {
        return DIAS_FAILURE;
    }

    i32ret = DiasSppDecode(pucBuf, u32len, ptDestMsg);
    if(DIAS_SUCCESS != i32ret)
    {
        return i32ret;
    }
	
    if(ptDestMsg->u8DleHeader != DIAS_SPP_HEAD_DLE || ptDestMsg->u8SomHeader != DIAS_SPP_HEAD_SOM)
    {
        return DIAS_FAILURE;
    }
	
    if(ptDestMsg->u8DleTrail != DIAS_SPP_TAIL_DLE || ptDestMsg->u8EofTail != DIAS_SPP_TAIL_EOF)
    {
        return DIAS_FAILURE;
    }

    DiasSppDumpMsg(ptDestMsg);
    return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppMakeCompleteMsg
*  content       | 接收数据组包
*  parameter     | ptCurMsg 当前包指针 ptAllMsg 全包指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppMakeCompleteMsg(DiasSppMcuMsg_t* ptCurMsg,DiasSppRecvMcuMsg_t* ptAllMsg)
{	
	if (ptCurMsg == NULL || ptAllMsg == NULL)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppMakeCompleteMsg: wrong para\n");
		return DIAS_FAILURE;
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
	DiasDcpdLog(LOG_DEBUG,"DiasSppMakeCompleteMsg finish.");	
	DiasSppDumpRecvMsg(ptAllMsg);
	return DIAS_SUCCESS;

}


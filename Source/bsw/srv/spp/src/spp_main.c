#include <stdio.h>
#include <stdlib.h>
//#include "Os.h"
#include "spi.h"
#include "spp_main.h"
#include "spp_memory.h"
#include "spp_code.h"
#include "spp_type.h"
#include "spp_com.h"
#include "spp_generated.h"
#include "mcu_com_uart.h"
#include "spp_callout.h"
#include "os_log.h"
#include "ll_can.h"

/**********************************************************************/
/*                        Global variable                             */
/**********************************************************************/
DiasSppConnectStatus_t gatDiasSppConnStatus[SPP_CONNECT_MAX_COUNT];
DIAS_SPP_MEM_TYPE *gptDiasSppSendListMcu4G = NULL;
DIAS_SPP_MEM_TYPE *gptDiasSppSendListMcuMpu = NULL;
DIAS_SPP_MEM_TYPE *gptDiasSppSendListMpu = NULL;
DIAS_SPP_MEM_TYPE *gptDiasSppCheckAckListMcu4G = NULL;
DIAS_SPP_MEM_TYPE *gptDiasSppCheckAckListMcuMpu = NULL;
DIAS_SPP_MEM_TYPE *gptDiasSppCheckAckListMpu = NULL;
DiasSppConnMsgProc_t gatDiasSppConnMsgProcList[SPP_CONNECT_MAX_COUNT]= {0};
DiasSppMsgProc_t gatDiasSppMsgProcList[DIAS_SPP_CALLBACK_MAX_COUNT]= {0};
UINT8 gucDiasSppRecvBuf[DIAS_SPP_PACKET_BUF_LEN_MAX] = {0};
UINT32 gu32DiasSppRecvBuf_len = 0;
DiasSppRecvMcuMsg_t gtAllMsg;
UINT8 gu8Temp[DIAS_SPP_PACKET_BUF_LEN_MAX] = {0};
DiasSppHeartManageType st_heartSend[SPP_CONNECT_MAX_COUNT] ;

DiasSppCmdInfo_t gatDiasSppCmdInfo[] = 
{ 
	{SPP_USER_RECV_DCPD_MSG_TO_MCU_VS_DATA_REQ_EVT,1,DIAS_SPP_PRI_NORMAL},
    //{0x1002,1,DIAS_SPP_PRI_MEDIUM},   
};

#define DIAS_DCPD_CMD_LIST_LEN (sizeof(gatDiasSppCmdInfo)/sizeof(DiasSppCmdInfo_t))


/**********************************************************************/
/*                            Functions                               */
/**********************************************************************/
static DiaSppPriorityType_e DiasSppGetPrioty(UINT16 u16EventId);
static DiasErrorCode_e DiasSppIsNeedtoCheckAck(DiasSppConnectType_e eConnectType,UINT16 u16EventId, UINT8 u8ResendTimes);
static DiasErrorCode_e DiasSppWhetherAddToSendList(DiasSppConnectType_e eConnectType,UINT16 u16EventId);
static DiasErrorCode_e DiasSppCheckConnectType(DiasSppConnectType_e eConnectType);
static DIAS_SPP_MEM_TYPE * DiasSppGetSendlistByConnectType(DiasSppConnectType_e eConnectType);
static void DiasSppSetSendlistByConnectType(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE * ptList);
static DIAS_SPP_MEM_TYPE * DiasSppGetCheckAcklistByConnectType(DiasSppConnectType_e eConnectType);
static DiasErrorCode_e DiasSppGetSendNode(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE** pptNode);
static DiasErrorCode_e DiasSppSendMsgToDevice(DiasSppConnectType_e eConnectType,DiasSppMcuMsg_t * ptMsg);
static DiasErrorCode_e DiasSppSendToDevice(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE *ptSendNode);
static DiasErrorCode_e DiasSppSendHeart(DiasSppConnectType_e eConnectType);
static DiasErrorCode_e DiasSppGetMsgAck(DiasSppRecvMcuMsg_t* ptRecvMsg, DiasSppMcuMsg_t* ptAckMsg);
static DiasErrorCode_e DiasSppAddToAckCheckList(DiasSppConnectType_e eConnectType, DIAS_SPP_MEM_TYPE *ptSendNode);
static DiasErrorCode_e DiasSppSendData(DiasSppConnectType_e eConnectType);
static DiasErrorCode_e DiasSppAckSend(DiasSppConnectType_e eConnectType,DiasSppRecvMcuMsg_t* ptRecvMsg);
static DiasErrorCode_e DiasSppIsAckMsg(DiasSppRecvMcuMsg_t* ptRecvMsg);
static DiasErrorCode_e DiasSppAckMsgProc(DiasSppConnectType_e eConnectType,DiasSppRecvMcuMsg_t* ptRecvMsg);
static DiasErrorCode_e DiasSppRecvCb(DiasSppConnectType_e eConnectType,DiasSppRecvMcuMsg_t* ptRecvMsg);
static DiasErrorCode_e DiasSppRecieveDataProc(DiasSppConnectType_e eConnectType);
static void DiasSppSetConnStatus(DiasSppConnectType_e eConnectType, DiasSppState_e eNewStatus);
static DiasErrorCode_e DiasSppInitSendMutex(DiasSppConnectType_e eConnectType);
static DiasErrorCode_e DiasSppUninitSendMutex(DiasSppConnectType_e eConnectType);
static DiasErrorCode_e DiasSppLockSendMutex(DiasSppConnectType_e eConnectType);
static DiasErrorCode_e DiasSppUnlockSendMutex(DiasSppConnectType_e eConnectType);
static long int DiasSppTimevalCompare(struct timeval *time1, struct timeval *time2);
static DiasErrorCode_e DiasSppAddSendListOneNode(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE * ptNodetoAdd);
static DiasErrorCode_e DiasSppAddSendList(DiasSppConnectType_e eConnectType,UINT16 u16EventId, UINT8 * pucData, UINT16 u16DataLen, UINT8 u8AllNum, UINT8 u8CurNum);
static DiasErrorCode_e DiasSppDelSendListbyCmdid(DiasSppConnectType_e eConnectType,UINT16 u16EventId);


/******************************************************************************
*  function name | DiasSppGetPrioty
*  content       | 判断是否高优先级
*  parameter     | u16EventId 业务id
*  return        | DiaSppPriorityType_e
*  notice        | 
******************************************************************************/
static DiaSppPriorityType_e DiasSppGetPrioty(UINT16 u16EventId)
{
	UINT16 u16Index=0;
	for(u16Index=0;u16Index<DIAS_DCPD_CMD_LIST_LEN;u16Index++)
	{
		if(u16EventId==gatDiasSppCmdInfo[u16Index].u16EventId)			
		{
			return (DiaSppPriorityType_e)gatDiasSppCmdInfo[u16Index].u8Prioty;
		}		
	}
	return DIAS_SPP_PRI_NORMAL;
}

/******************************************************************************
*  function name | DiasSppGetResendTimes
*  content       | get resend times by eventid
*  parameter     | u16EventId id
*  return        | UINT16
*  notice        | 
******************************************************************************/
static UINT8 DiasSppGetResendTimes(UINT16 u16EventId)
{
	UINT16 u16Index=0;
	for(u16Index=0;u16Index<DIAS_DCPD_CMD_LIST_LEN;u16Index++)
	{
		if(u16EventId==gatDiasSppCmdInfo[u16Index].u16EventId)
		{
			return gatDiasSppCmdInfo[u16Index].u8ResendTimes;
		}
	}
	return 0;	
}


/******************************************************************************
*  function name | DiasSppIsNeedtoCheckAck
*  content       | 判断是否需要判断ack
*  parameter     | u16EventId 业务id   u8ResendTimes 已重发次数
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppIsNeedtoCheckAck(DiasSppConnectType_e eConnectType,UINT16 u16EventId, UINT8 u8ResendTimes)
{
    UINT16 u16Index=0;
    DIAS_SPP_MEM_TYPE *ptAckCheckListCur=NULL;
    UINT32 u32ListNum=0;

    ptAckCheckListCur=DiasSppGetCheckAcklistByConnectType(eConnectType);  
    u32ListNum=DiasSppMemGetListNum(ptAckCheckListCur);
    if(u32ListNum>=DIAS_SPP_ACK_LIST_MAX_NUM)
    {
        return DIAS_FAILURE;
    }        

    for(u16Index=0;u16Index<DIAS_DCPD_CMD_LIST_LEN;u16Index++)
    {
        if((u16EventId==gatDiasSppCmdInfo[u16Index].u16EventId)
             &&(u8ResendTimes<gatDiasSppCmdInfo[u16Index].u8ResendTimes))
        {
            return DIAS_SUCCESS;
        }
    }
    return DIAS_FAILURE;	
}

/******************************************************************************
*  function name | DiasSppWhetherAddToSendList
*  content       | 判断是否添加到send list
*  parameter     | eConnectType连接类型     u16EventId业务id
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppWhetherAddToSendList(DiasSppConnectType_e eConnectType, UINT16 u16EventId)
{
    UINT32 u32ListCurNum=0;
	DiaSppPriorityType_e ePriority=DIAS_SPP_PRI_LOW;	
	UINT16 u16Cmdid=0;
	DIAS_SPP_MEM_TYPE * ptSendListCur=NULL;

	ptSendListCur=DiasSppGetSendlistByConnectType(eConnectType);
	u32ListCurNum=DiasSppMemGetListNum(ptSendListCur);
	ePriority=DiasSppGetPrioty(u16EventId);
	DiasDcpdLog(LOG_INFO,"DiasSppMemGetListNum %d EventId %d Priority %d",u32ListCurNum,u16EventId,ePriority);	

	if(DIAS_SPP_SEND_LIST_NORMAL_NUM<=u32ListCurNum)
	{		
        if(DIAS_SPP_SEND_LIST_MAX_NUM>u32ListCurNum)
        {
			if((DIAS_SPP_PRI_MEDIUM==ePriority)||(DIAS_SPP_PRI_HIGH==ePriority))
			{
				return DIAS_SUCCESS;
			}
        }
		else if(DIAS_SPP_SEND_LIST_MAX_NUM>=u32ListCurNum)
        {
			if(DIAS_SPP_PRI_HIGH==ePriority)
			{
				ptSendListCur=DiasSppMemRemoveNotHighNode(ptSendListCur);
				DiasSppSetSendlistByConnectType(eConnectType,ptSendListCur);
				return DIAS_SUCCESS;
			}
        }		
		else
		{
			return DIAS_FAILURE;
		}
	}
	else
	{
		return DIAS_SUCCESS;
	}
	return DIAS_FAILURE;	
}

/******************************************************************************
*  function name | DiasSppCheckConnectType
*  content       | 检查连接类型
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppCheckConnectType(DiasSppConnectType_e eConnectType)
{
	if((eConnectType < SPP_CONNECT_TYPE_MCU_4G) || (eConnectType > SPP_CONNECT_TYPE_MPU))
	{
		DiasDcpdLog(LOG_ERR,"wrong eConnectType");
		return DIAS_FAILURE;
	}
	return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppGetSendlistByConnectType
*  content       | 获取发送队列
*  parameter     | eConnectType 连接类型
*  return        | DIAS_SPP_MEM_TYPE *
*  notice        | 
******************************************************************************/
static DIAS_SPP_MEM_TYPE * DiasSppGetSendlistByConnectType(DiasSppConnectType_e eConnectType)
{
	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		return gptDiasSppSendListMcu4G;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		return gptDiasSppSendListMcuMpu;	
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		return gptDiasSppSendListMpu;
	}
}

/******************************************************************************
*  function name | DiasSppGetSendlistByConnectType
*  content       | 更新发送队列
*  parameter     | eConnectType 连接类型 ptList 队列指针
*  return        | 
*  notice        | 
******************************************************************************/
static void DiasSppSetSendlistByConnectType(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE * ptList)
{
	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		gptDiasSppSendListMcu4G=ptList;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		gptDiasSppSendListMcuMpu=ptList;	
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		gptDiasSppSendListMpu=ptList;
	}
}

/******************************************************************************
*  function name | DiasSppGetCheckAcklistByConnectType
*  content       | 获取校验ack队列
*  parameter     | eConnectType 连接类型
*  return        | DIAS_SPP_MEM_TYPE *
*  notice        | 
******************************************************************************/
static DIAS_SPP_MEM_TYPE * DiasSppGetCheckAcklistByConnectType(DiasSppConnectType_e eConnectType)
{
	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		return gptDiasSppCheckAckListMcu4G;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		return gptDiasSppCheckAckListMcuMpu;	
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		return gptDiasSppCheckAckListMpu;
	}		
}

static void DiasSppSetCheckAcklistByConnectType(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE * ptList)
{
	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		gptDiasSppCheckAckListMcu4G=ptList;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		gptDiasSppCheckAckListMcuMpu=ptList;	
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		gptDiasSppCheckAckListMpu=ptList;
	}
}


/******************************************************************************
*  function name | DiasSppGetSendNode
*  content       | 获取发送节点
*  parameter     | eConnectType 连接类型 pptNode 发送节点指针地址
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppGetSendNode(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE** pptNode)
{	
	DiasErrorCode_e eRet=DIAS_FAILURE;
	DIAS_SPP_MEM_TYPE *ptSendListCur=NULL;
	DIAS_SPP_MEM_TYPE *ptNode=NULL;	

	if(NULL == pptNode)
	{
		DiasDcpdLog(LOG_ERR,"pptNode is NULL");
		return DIAS_FAILURE;
	}
	
	eRet=DiasSppCheckConnectType(eConnectType);
	if(DIAS_FAILURE==eRet)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppCheckConnectType fail");
		return eRet;
	}


	ptSendListCur=DiasSppGetSendlistByConnectType(eConnectType);
	if(NULL==ptSendListCur)
	{
		//DiasDcpdLog(LOG_ERR,"ptSendListCur is NULL");
		return DIAS_FAILURE;
	}
	
	DiasSppLockSendMutex(eConnectType);
	ptNode=DiasSppMemGetFirstNode(ptSendListCur);

	if(NULL != ptNode)
	{
		ptSendListCur=ptNode->next;
		if(NULL!=ptSendListCur)
		{
			ptSendListCur->prev=NULL;
		}
		DiasSppSetSendlistByConnectType(eConnectType,ptSendListCur);
		DiasSppUnlockSendMutex(eConnectType);	
		DiasDcpdLog(LOG_INFO,"DiasSppMemGetFirstNode success EventId %d u16DataLen %d",ptNode->u16EventId, ptNode->u16DataLen);

		*pptNode = ptNode;
		return DIAS_SUCCESS;		
	}	
	DiasSppUnlockSendMutex(eConnectType);	
	return DIAS_FAILURE;	
}
/******************************************************************************
*  function name | DiasSppSendMsgToDevice
*  content       | 发送Msg
*  parameter     | eConnectType 连接类型 ptMsg 数据帧指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppSendMsgToDevice(DiasSppConnectType_e eConnectType,DiasSppMcuMsg_t * ptMsg)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	static UINT8 aucDestBuf[DIAS_SPP_PACKET_LEN_MAX]={0};
	UINT32 u32Destlen=DIAS_SPP_PACKET_LEN_MAX;
	UINT8 u8Crc=0;

	if(NULL==ptMsg)
	{
		return DIAS_FAILURE;
	}
	
	DiasSppDumpMsg(ptMsg);		
	(void)memset(aucDestBuf, 0xFF, sizeof(aucDestBuf));
	eRet = DiasSppMsgEncode(aucDestBuf,DIAS_SPP_PACKET_LEN_MAX,&u32Destlen,ptMsg);		
	if(DIAS_FAILURE==eRet)
	{
		return eRet;
	}	
		
	eRet =DiasSppComSend(eConnectType,aucDestBuf,DIAS_SPP_PACKET_LEN_MAX);		
	return eRet;

}
/******************************************************************************
*  function name | DiasSppSendData
*  content       | 发送节点
*  parameter     | eConnectType 连接类型 ptSendNode 发送节点指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppSendToDevice(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE *ptSendNode)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	DiasSppMcuMsg_t tMsg;
	UINT8 u8Crc=0;
	
	if(NULL==ptSendNode)
	{
		DiasDcpdLog(LOG_ERR,"ptSendNode is NULL");
		return DIAS_FAILURE;
	}

	memset(&tMsg, 0,sizeof(DiasSppMcuMsg_t)); 		
	tMsg.u8ControlType=DIAS_SPP_REQ;	
	tMsg.u8Priority=ptSendNode->u8Priority;
	tMsg.u16Cmd=ptSendNode->u16EventId;
	tMsg.u16DataLen=ptSendNode->u16DataLen;
	tMsg.u8AllNum=ptSendNode->u8SplitedNumAll;
	tMsg.u8CurNum=ptSendNode->u8SplitedNumCur;
	memcpy(tMsg.aucData, ptSendNode->pDcpdData, tMsg.u16DataLen); 	
	tMsg.u8Roll=0;
	DiasSppGetCrc(&tMsg,&u8Crc);
    if ((tMsg.u16Cmd == 157)
        ||(tMsg.u16Cmd == 158)
        ||(tMsg.u16Cmd == 159))
    {
        ApiLogPrint(_LOG_ENTRY, "%d: %2x %2x %2x %2x %2x \n",tMsg.u16Cmd,tMsg.aucData[0],tMsg.aucData[1],tMsg.aucData[2],tMsg.aucData[3],u8Crc );
    }
	tMsg.u8Crc=u8Crc;	

	eRet=DiasSppSendMsgToDevice(eConnectType,&tMsg);	
	return eRet;

}

/******************************************************************************
*  function name | DiasSppSendHeart
*  content       | 发送心跳
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppSendHeart(DiasSppConnectType_e eConnectType)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	DiasSppMcuMsg_t tMsg;
	static UINT8 aucDestBuf[DIAS_SPP_PACKET_LEN_MAX];
	UINT32 u32Destlen=0;
	UINT8 u8Crc=0;
	
	memset(&tMsg, 0,sizeof(DiasSppMcuMsg_t)); 		
	tMsg.u8ControlType=DIAS_SPP_HEART;
	tMsg.u8Priority=DIAS_SPP_PRI_MEDIUM;
	tMsg.u16Cmd=0;
	tMsg.u16DataLen=0;	
	tMsg.u8Roll=0;
	DiasSppGetCrc(&tMsg,&u8Crc);
	tMsg.u8Crc=u8Crc;	
	
	memset(aucDestBuf,0xFF,sizeof(aucDestBuf)); 
	eRet=DiasSppMsgEncode(aucDestBuf,DIAS_SPP_PACKET_LEN_MAX,&u32Destlen,&tMsg);
	if(DIAS_FAILURE==eRet)
	{
		return eRet;
	}	

	eRet=DiasSppComSend(eConnectType,aucDestBuf,DIAS_SPP_PACKET_LEN_MAX);		
	return eRet;

}

/******************************************************************************
*  function name | DiasSppGetMsgAck
*  content       | 发送
*  parameter     | ptRecvMsg 接收数据指针 ptAckMsg ack数据指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppGetMsgAck(DiasSppRecvMcuMsg_t* ptRecvMsg, DiasSppMcuMsg_t* ptAckMsg)
{
	UINT8 u8Crc=0;
	
	if((NULL==ptRecvMsg)||(NULL==ptAckMsg))
	{
		DiasDcpdLog(LOG_ERR,"DiasSppGetMsgAck wrong para");
		return DIAS_FAILURE;
	}

	if((DIAS_SPP_HANDSHAKE!=ptRecvMsg->u8ControlType)&&(DIAS_SPP_HEART!=ptRecvMsg->u8ControlType)
		&&(DIAS_SPP_REQ!=ptRecvMsg->u8ControlType)&&(DIAS_SPP_NOTIFY!=ptRecvMsg->u8ControlType))

	{
		DiasDcpdLog(LOG_DEBUG,"DiasSppGetMsgAck ControlType %d",ptRecvMsg->u8ControlType);
		return DIAS_FAILURE;
	}
		
	memset(ptAckMsg, 0,sizeof(DiasSppMcuMsg_t));  
	
	ptAckMsg->u8ControlType=ptRecvMsg->u8ControlType+1;
	ptAckMsg->u8Priority=DIAS_SPP_PRI_NORMAL;
	ptAckMsg->u16Cmd=ptRecvMsg->u16Cmd;
	ptAckMsg->u16DataLen=0;	
	ptAckMsg->u8Roll=ptRecvMsg->u8Roll;
	DiasSppGetCrc(ptAckMsg,&u8Crc);
	ptAckMsg->u8Crc=u8Crc;	
	
	return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppAddToAckCheckList
*  content       | 添加到检查ack list
*  parameter     | eConnectType 连接类型 ptSendNode 发送节点指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppAddToAckCheckList(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE *ptSendNode)
{	//add to send list
	DiasErrorCode_e eRet=DIAS_FAILURE;
	DIAS_SPP_MEM_TYPE *ptAckCheckListCur=NULL;
	
	if(NULL == ptSendNode)
	{
		DiasDcpdLog(LOG_ERR,"ptSendNode is NULL");
		return DIAS_FAILURE;
	}

    DiasDcpdLog(LOG_DEBUG,"DiasSppAddToAckCheckList EventId %d ResendTimes %d",ptSendNode->u16EventId,ptSendNode->u8ResendTimes);
	eRet=DiasSppCheckConnectType(eConnectType);
	if(DIAS_FAILURE==eRet)
	{
		DiasDcpdLog(LOG_INFO,"free Node cmdid %d",ptSendNode->u16EventId);
		eRet=DiasSppMemFreeOneNode(ptSendNode);	
		return eRet;
	}
	
	eRet=DiasSppIsNeedtoCheckAck(eConnectType,ptSendNode->u16EventId,ptSendNode->u8ResendTimes);
	if(DIAS_FAILURE != eRet)
	{		
		DiasDcpdLog(LOG_DEBUG,"EventId %d ResendTimes %d",ptSendNode->u16EventId,ptSendNode->u8ResendTimes);
		ptSendNode->u8ResendTimes=ptSendNode->u8ResendTimes+1;
		DiasSppLockSendMutex(eConnectType);
        ptAckCheckListCur=DiasSppGetCheckAcklistByConnectType(eConnectType);
		ptAckCheckListCur=DiasSppMemAddNode(ptAckCheckListCur, ptSendNode);
		if(NULL==ptAckCheckListCur)
		{			
			DiasDcpdLog(LOG_INFO,"free Node cmdid %d",ptSendNode->u16EventId);
			eRet=DiasSppMemFreeOneNode(ptSendNode);
		}
		else
		{
			DiasSppSetCheckAcklistByConnectType(eConnectType,ptAckCheckListCur);
		}
	    DiasSppUnlockSendMutex(eConnectType);	
		DiasDcpdLog(LOG_INFO,"Add Node to ACK check list cmdid %d",ptSendNode->u16EventId);
	}
	else
	{
		DiasDcpdLog(LOG_INFO,"free Node cmdid %d",ptSendNode->u16EventId);
		eRet=DiasSppMemFreeOneNode(ptSendNode);		
	}
	//DiasDcpdLog(LOG_DEBUG,"DiasSppAddToAckCheckList out");
	return eRet;
}

/******************************************************************************
*  function name | DiasSppSendData
*  content       | 线程中的发送处理
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppSendData(DiasSppConnectType_e eConnectType)
{	
	DiasErrorCode_e eRet = DIAS_FAILURE;
	DIAS_SPP_MEM_TYPE *ptSendNode=NULL;
	UINT8 au8Test[DIAS_SPP_PACKET_LEN_MAX] = {0};
	UINT8 u8Idx = 0;
	UINT8 *pu8IdxDelta = NULL;
	static UINT8 u8IdxdeltaMpu = 0;
	static UINT8 u8Idxdelta4G = 0;

	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		pu8IdxDelta = &u8Idxdelta4G;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		pu8IdxDelta = &u8IdxdeltaMpu;
	}
	else
	{
		return DIAS_FAILURE;
	}

    /*add heart*/
	/*
	if(gu32HeartWaitTimes<DIAS_SPP_HEART_SEND_PERIOD)
	{
		gu32HeartWaitTimes += 10;  //DCPD_TASK_PRD_MS=10ms
	}
	else
	{
		gu32HeartWaitTimes=0;
		if((1==gu8HeartSended)&&(0==gu8ReceivedHeartAck))  //not receive heart ack
		{
			DiasSppComClose(eConnectType);
			ApiDelayus(10);
			DiasSppComOpen(eConnectType);
			ApiDelayus(10);
			DiasSppSendHeart(eConnectType);
			gu8HeartSended=1;
			gu8ReceivedHeartAck=0;
		}
		else
		{
			DiasSppSendHeart(eConnectType);
			gu8HeartSended=1;
			gu8ReceivedHeartAck=0;
		}
	}
	*/

	eRet=DiasSppGetSendNode(eConnectType, &ptSendNode);
	if(DIAS_FAILURE == eRet)
	{
		//ApiLogPrintExt(1, _LOG_DEBUG, "NON-Valid data send...");
	#if 0
		for(u8Idx=0; u8Idx<sizeof(au8Test); u8Idx++)
		{
			au8Test[u8Idx] = u8Idx + *pu8IdxDelta;
		}
		(*pu8IdxDelta)++;
		if(sizeof(au8Test) == *pu8IdxDelta)
		{
			*pu8IdxDelta = 0;
		}
	#endif
		//SPI_Data_Send(eConnectType, au8Test, sizeof(au8Test));
		return DIAS_FAILURE;
	}

	//ApiLogPrintExt(1, _LOG_DEBUG, "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv Valid data send vvvvvvvvvvvvvvvvvvvvv");
	eRet= DiasSppSendToDevice(eConnectType,ptSendNode);
	if(DIAS_FAILURE == eRet)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppSendToDevice fail,EventId %d CurNum %d",ptSendNode->u16EventId,ptSendNode->u8SplitedNumCur);
	}
	
	DiasSppAddToAckCheckList(eConnectType,ptSendNode);		
	return DIAS_SUCCESS;	
}



/******************************************************************************
*  function name | DiasSppAckSend
*  content       | 返回ack
*  parameter     | eConnectType 连接类型 ptRecvMsg 收到的MSG结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppAckSend(DiasSppConnectType_e eConnectType,DiasSppRecvMcuMsg_t* ptRecvMsg)
{
	DiasSppMcuMsg_t tAckMsg;
	DiasErrorCode_e eRet=DIAS_FAILURE;
	
    memset(&tAckMsg,0,sizeof(DiasSppMcuMsg_t));
	if(NULL==ptRecvMsg)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppAckSend input is NULL");
		return DIAS_FAILURE;
	}	

	if((DIAS_SPP_PRI_HIGH==ptRecvMsg->u8Priority)||(DIAS_SPP_PRI_MEDIUM==ptRecvMsg->u8Priority))
	{
		eRet=DiasSppGetMsgAck(ptRecvMsg,&tAckMsg);
		if(DIAS_SUCCESS==eRet)
		{
			DiasSppSendMsgToDevice(eConnectType,&tAckMsg);
		}
		else
		{
			return DIAS_FAILURE;
		}
	}
	return DIAS_SUCCESS;	
}

/******************************************************************************
*  function name | DiasSppIsAckMsg
*  content       | 返回ack
*  parameter     |  ptRecvMsg 收到的MSG结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppIsAckMsg(DiasSppRecvMcuMsg_t* ptRecvMsg)
{	
	if(NULL==ptRecvMsg)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppIsAckMsg input is NULL");
		return DIAS_FAILURE;
	}	

	if((DIAS_SPP_HANDSHAKE_ACK==ptRecvMsg->u8ControlType)||(DIAS_SPP_HEART_ACK==ptRecvMsg->u8ControlType)
		||(DIAS_SPP_REQ_ACK==ptRecvMsg->u8ControlType)||(DIAS_SPP_NOTIFY_ACK==ptRecvMsg->u8ControlType))
	{
		return DIAS_SUCCESS;
	}	
	return DIAS_FAILURE;	
}


/******************************************************************************
*  function name | DiasSppTimevalCompare
*  content       | 时间比较
*  parameter     | time1 time2 输入参数
*  return        | long int ms
*  notice        | 
******************************************************************************/
static long int DiasSppTimevalCompare(struct timeval *time1, struct timeval *time2)
{ 
    long int ret = 0;
    ret = (time2->tv_sec - time1->tv_sec )*1000000 +(time2->tv_usec - time1->tv_usec);   
    return ret/1000;	
}

/******************************************************************************
*  function name | DiasSppAckMsgProc
*  content       | 处理ack
*  parameter     | eConnectType 连接类型     ptRecvMsg 收到的MSG结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppAckMsgProc(DiasSppConnectType_e eConnectType,DiasSppRecvMcuMsg_t* ptRecvMsg)
{	
	DIAS_SPP_MEM_TYPE *ptAckCheckListCur=NULL;		
	DIAS_SPP_MEM_TYPE * ptTmp=NULL;	
	DIAS_SPP_MEM_TYPE* ptPrev = NULL;
	DIAS_SPP_MEM_TYPE* ptNext = NULL;
	DIAS_SPP_MEM_TYPE* ptFree = NULL;  
	UINT32 u32ListNum=0;
	struct timeval tTime={0};

	ptAckCheckListCur=DiasSppGetCheckAcklistByConnectType(eConnectType);	
	if(NULL==ptAckCheckListCur)
	{
		return DIAS_FAILURE;
	}
	ptTmp=ptAckCheckListCur;
	DiasSppGetAbsTime(&tTime);		
	while((ptTmp) != NULL)
	{ 		
		//if(DiasSppTimevalCompare(&(ptTmp->currentTime),&tTime)>DIAS_SPP_ACK_TIMEOUT_TIME)
		if(ptTmp->u32TimeCout>DIAS_SPP_ACK_TIMEOUT_TIME)
	 	{ 	
			DiasDcpdLog(LOG_DEBUG,"DiasSppAckMsgProc find timeout id %d numcur %d",ptTmp->u16EventId,ptTmp->u8SplitedNumCur);
			ptPrev = ptTmp->prev;
			ptNext = ptTmp->next;				 
			if(ptPrev == NULL)
			{
				// first node
				ptAckCheckListCur = ptNext;
				if(ptAckCheckListCur != NULL)
				{
					ptAckCheckListCur ->prev = NULL;
				}
			}
			else
			{
				ptPrev->next= ptNext;
				if(ptNext != NULL)
				{
					ptNext->prev = ptPrev;
				}
			}						 
			ptFree = ptTmp;
			ptTmp = ptNext; 
		    //add to send list 	
			DiasSppAddSendListOneNode(eConnectType,ptFree);								
		}
		else
		{			
            if(NULL!=ptRecvMsg)
            {
				if(ptTmp->u16EventId==ptRecvMsg->u16Cmd)
	            {
					ptPrev = ptTmp->prev;
					ptNext = ptTmp->next;				 
					if(ptPrev == NULL)
					{
						// first node
						ptAckCheckListCur = ptNext;
						if(ptAckCheckListCur != NULL)
						{
							ptAckCheckListCur ->prev = NULL;
						}
					}
					else
					{
						ptPrev->next= ptNext;
						if(ptNext != NULL)
						{
							ptNext->prev = ptPrev;
						}
					}						 
					ptFree = ptTmp;
					ptTmp = ptNext; 
					DiasSppMemFreeOneNode(ptFree);				
	            }
            }
			ptTmp=ptTmp->next;
		}		
	}   
	DiasSppSetCheckAcklistByConnectType(eConnectType,ptAckCheckListCur);
	return DIAS_SUCCESS;	
}


/******************************************************************************
*  function name | DiasSppRecvCb
*  content       | 接收回调处理函数
*  parameter     | eConnectType 连接类型 ptRecvMsg 收到的MSG结构体指针
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppRecvCb(DiasSppConnectType_e eConnectType,DiasSppRecvMcuMsg_t* ptRecvMsg)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	UINT32 u32Index = 0; 	
	pDiasSppListener pRecvFunCb= NULL;	

	if(NULL==ptRecvMsg)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppRecvCb input is NULL");
		return DIAS_FAILURE;
	}		
	DiasSppSetConnStatus(eConnectType,DIAS_SPP_STATE_RUNNING);
    //ApiLogPrint(_LOG_ENTRY, "type:%2x,0x%2x%2x\n", ptRecvMsg->u8ControlType,(UINT8)ptRecvMsg->u16Cmd>>8,(UINT8)ptRecvMsg->u16Cmd);
	eRet=DiasSppIsAckMsg(ptRecvMsg);
	if(DIAS_SUCCESS==eRet)
	{		
		if(DIAS_SPP_HEART_ACK==ptRecvMsg->u8ControlType)
		{
			DiasDcpdLog(LOG_DEBUG,"recv heart ack");
            st_heartSend[(UINT8)eConnectType-1].u8_recAckFlag = 1;
            st_heartSend[(UINT8)eConnectType-1].u8_sendTimes = 0;
            st_heartSend[(UINT8)eConnectType-1].u8_recFirstAckFlg = 2;
            ApiLogPrint(_LOG_ENTRY, "==heartack==");
		}
		else
		{
			DiasDcpdLog(LOG_DEBUG,"recv cmd ack");
			DiasSppLockSendMutex(eConnectType);
			DiasSppAckMsgProc(eConnectType,ptRecvMsg);
			DiasSppUnlockSendMutex(eConnectType);
            st_heartSend[(UINT8)eConnectType-1].u16_waitTimes = 0;
            st_heartSend[(UINT8)eConnectType-1].u8_recAckFlag = 1;
            st_heartSend[(UINT8)eConnectType-1].u8_sendTimes = 0;
            st_heartSend[(UINT8)eConnectType-1].u8_recFirstAckFlg = 2;
            ApiLogPrint(_LOG_ENTRY, "==gack==");
		}
        gatDiasSppConnStatus[(UINT8)eConnectType-1].eConnStatus = DIAS_SPP_STATE_RUNNING;
	}
	else
	{
#if 0
        st_heartSend[(UINT8)eConnectType-1].u16_waitTimes = 0;
        st_heartSend[(UINT8)eConnectType-1].u8_recAckFlag = 1;
        st_heartSend[(UINT8)eConnectType-1].u8_sendTimes = 0;
        st_heartSend[(UINT8)eConnectType-1].u8_recFirstAckFlg = 2;        
        gatDiasSppConnStatus[(UINT8)eConnectType-1].eConnStatus = DIAS_SPP_STATE_RUNNING;
#endif
        if(ptRecvMsg->u8AllNum != ptRecvMsg->u8CurNum)
        {
        	DiasDcpdLog(LOG_INFO,"DiasSppRecvCb recv is not complete.");
			return DIAS_FAILURE;
        }
		else
		{			
			DiasDcpdLog(LOG_DEBUG,"DiasSppRecvCb recv is complete.");
		}
		
		DiasSppAckSend(eConnectType,ptRecvMsg);	
		DiasDcpdLog(LOG_DEBUG,"DiasSppAckSend end.");
		DiasDcpdLog(LOG_DEBUG,"gatDiasSppMsgProcList proc eConnectType %d  u16Cmd %d.",eConnectType,ptRecvMsg->u16Cmd);
		for(u32Index=0; u32Index<sizeof(gatDiasSppMsgProcList)/sizeof(DiasSppMsgProc_t); u32Index++)
		{			
			//DiasDcpdLog(LOG_DEBUG,"gatDiasSppMsgProcList proc u32Index %d eConnectType %d  u16Cmd %d.",u32Index,gatDiasSppMsgProcList[u32Index].eConnectType,gatDiasSppMsgProcList[u32Index].u16EventId);
			if((eConnectType == gatDiasSppMsgProcList[u32Index].eConnectType)
				&&(ptRecvMsg->u16Cmd == gatDiasSppMsgProcList[u32Index].u16EventId))
			{
				pRecvFunCb=gatDiasSppMsgProcList[u32Index].pFunCb;
				DiasDcpdLog(LOG_DEBUG,"find pRecvFunCb.");
				if(NULL!=pRecvFunCb)
				{
					pRecvFunCb(ptRecvMsg->u16Cmd, (DiasSppPackage_t*)ptRecvMsg->aucData, (UINT32)ptRecvMsg->u16DataLen);
				}			
			}
		}
		memset(&gtAllMsg, 0,sizeof(DiasSppRecvMcuMsg_t));
	}
	DiasDcpdLog(LOG_DEBUG,"DiasSppRecvCb out.");
	return DIAS_SUCCESS;
}
UINT32 u32_MSGCount;

/******************************************************************************
*  function name | DiasSppRecieveDataProc
*  content       | 接收处理函数
*  parameter     | eConnectType 连接类型 
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppRecieveDataProc(DiasSppConnectType_e eConnectType)
{
    UINT32 u32RecvLen =0;
	INT32 i32UnusedLen =0;
    UINT8 aucRecvBuf[DIAS_SPP_PACKET_LEN_MAX] = {0};
    UINT8* pucBeginOld = NULL;
    UINT8* pucEnd = NULL;
    UINT8* pucBeginNew = NULL;
    UINT8* pucSrc = NULL;
    //UINT8* pucTmp =NULL;
	DiasSppMcuMsg_t tCurMsg;    
    DiasErrorCode_e eRet =DIAS_FAILURE;

	// recv from spi
	u32RecvLen=DiasSppComRecv(eConnectType,aucRecvBuf,DIAS_SPP_PACKET_LEN_MAX);
#if 0
    if(u32RecvLen > 0)
    {   
		/*recieve success*/
		if(gu32DiasSppRecvBuf_len + u32RecvLen >= sizeof(gucDiasSppRecvBuf))
	    {
	        ApiLogPrint(_LOG_ENTRY, "receiveFull");
		    gu32DiasSppRecvBuf_len = 0;
	        memset(gucDiasSppRecvBuf, 0, sizeof(gucDiasSppRecvBuf));
	    }
		i32UnusedLen=DIAS_SPP_PACKET_BUF_LEN_MAX-gu32DiasSppRecvBuf_len;
		memcpy(gucDiasSppRecvBuf+gu32DiasSppRecvBuf_len,aucRecvBuf, (size_t)u32RecvLen);
		gu32DiasSppRecvBuf_len += u32RecvLen;
	    pucBeginOld = gucDiasSppRecvBuf;
	    while(1)
	    {
	    	pucEnd = gucDiasSppRecvBuf+gu32DiasSppRecvBuf_len;
	        pucBeginNew = DiasSppFindMsgHead(pucBeginOld, pucEnd);
	        if(NULL == pucBeginNew)
	        {
				DiasSppLockSendMutex(eConnectType);
				DiasSppAckMsgProc(eConnectType,NULL);
				DiasSppUnlockSendMutex(eConnectType);	
				break;				
	        }
	        else
	        {
				memset(&tCurMsg, 0,sizeof(DiasSppMcuMsg_t)); 
	            eRet = DiasSppMsgDecode(pucBeginNew, pucEnd-pucBeginNew, &tCurMsg);
	            if(DIAS_SUCCESS != eRet)
	            {
	                pucBeginOld = pucBeginNew + 2;
	            }
	            else
	            {
					UINT32 u32LenTmp =0;					
					DiasDcpdLog(LOG_INFO,"[info] on_recieve_data decode success");	
					DiasSppMakeCompleteMsg(&tCurMsg,&gtAllMsg);
					DiasSppRecvCb(eConnectType,&gtAllMsg);				
					
	                u32LenTmp = tCurMsg.u16DataLen + DIAS_SPP_PACKET_LEN_MIN;
	                pucSrc = pucBeginNew + u32LenTmp;
					pucEnd = gucDiasSppRecvBuf+gu32DiasSppRecvBuf_len;
	                if(pucSrc < pucEnd)
	                {
						//pucTmp=malloc(DIAS_SPP_PACKET_BUF_LEN_MAX);
                        memset(gu8Temp, 0, sizeof(gu8Temp));
						if(NULL!=gu8Temp)
						{						
							memcpy(gu8Temp, pucSrc, pucEnd-pucSrc);
							memset(gucDiasSppRecvBuf, 0, sizeof(gucDiasSppRecvBuf));
							gu32DiasSppRecvBuf_len = pucEnd-pucSrc;
							memcpy(gucDiasSppRecvBuf, gu8Temp, gu32DiasSppRecvBuf_len);
							//free(pucTmp);
        					//pucTmp = NULL;
						}						
	                }
	                else
	                {
	                    memset(gucDiasSppRecvBuf, 0, sizeof(gucDiasSppRecvBuf));
	                }	                
	                pucBeginOld = gucDiasSppRecvBuf;
	            }
	        }
	    }		
    }
    else
    {
        /*recieve error*/
		DiasSppLockSendMutex(eConnectType);
		DiasSppAckMsgProc(eConnectType,NULL);
		DiasSppUnlockSendMutex(eConnectType);		
        return  DIAS_FAILURE;
    }
#endif
    if(u32RecvLen > 0)
    {   
        if(DIAS_SPP_PACKET_LEN_MAX != u32RecvLen)
        {
            ApiLogPrint(_LOG_ENTRY, "receiveFull");
        }
                   /*recieve success*/
        if(gu32DiasSppRecvBuf_len + u32RecvLen >= sizeof(gucDiasSppRecvBuf))
        {
            ApiLogPrint(_LOG_ENTRY,"DiasSppRecv:RecvBuf is full");
            gu32DiasSppRecvBuf_len = 0;
            memset(gucDiasSppRecvBuf, 0, sizeof(gucDiasSppRecvBuf));
        }                 
        memcpy(gucDiasSppRecvBuf+gu32DiasSppRecvBuf_len,aucRecvBuf, (size_t)u32RecvLen);
        gu32DiasSppRecvBuf_len += u32RecvLen;
        pucBeginOld = gucDiasSppRecvBuf;
        pucEnd = gucDiasSppRecvBuf+gu32DiasSppRecvBuf_len;
        pucBeginNew = DiasSppFindMsgHead(pucBeginOld, pucEnd);
        if(NULL == pucBeginNew)
        {
         //   ApiLogPrint(_LOG_ENTRY,"DiasSppRecv:FindMsgHead Failed,CurrentLen(%d)",gu32DiasSppRecvBuf_len); 
            DiasSppInitBuffer();
        }
        else
        {
            memset(&tCurMsg, 0,sizeof(DiasSppMcuMsg_t)); 
            eRet = DiasSppMsgDecode(pucBeginNew, pucEnd-pucBeginNew, &tCurMsg);
            if(DIAS_SUCCESS != eRet)
            {
                ApiLogPrint(_LOG_ENTRY,"DiasSppRecv:MsgDecode Failed");
                DiasSppInitBuffer();
            }
            else
            {
                DiasSppMakeCompleteMsg(&tCurMsg,&gtAllMsg);
                DiasSppRecvCb(eConnectType,&gtAllMsg);  
#if 0
                u32_MSGCount ++;
                if (u32_MSGCount == 150)
                {
                    ApiLogPrint(_LOG_ENTRY,"==ke");
                    u32_MSGCount = 0;
                }
#endif
            } 
        }
        DiasSppLockSendMutex(eConnectType);
        DiasSppAckMsgProc(eConnectType,NULL);
        DiasSppUnlockSendMutex(eConnectType);
        memset(gucDiasSppRecvBuf, 0, sizeof(gucDiasSppRecvBuf));
        gu32DiasSppRecvBuf_len = 0x00;
    }
    else
    {
        /*recieve error*/
        DiasSppLockSendMutex(eConnectType);
        DiasSppAckMsgProc(eConnectType,NULL);
        DiasSppUnlockSendMutex(eConnectType);              
        return  DIAS_FAILURE;
    }
    return DIAS_SUCCESS;

}

/******************************************************************************
*  function name | DiasSppSetConnStatus
*  content       | 设置连接状态 
*  parameter     | eConnectType 连接类型     eNewStatus连接状态 
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static void DiasSppSetConnStatus(DiasSppConnectType_e eConnectType, DiasSppState_e           eNewStatus)
{
    UINT8 u8Index=0;
	for(u8Index =0; u8Index<SPP_CONNECT_MAX_COUNT; u8Index++)
    {
		if(eConnectType == gatDiasSppConnStatus[u8Index].eConnectType)
		{
			gatDiasSppConnStatus[u8Index].eConnStatus= eNewStatus;
            if (eNewStatus == DIAS_SPP_STATE_RUNNING)
            {
                //st_heartSend[u8Index].u16_waitTimes = 0;
                st_heartSend[u8Index].u8_recAckFlag = 1;
                st_heartSend[u8Index].u8_sendFlg = 0;
                st_heartSend[u8Index].u8_sendTimes = 0;
                st_heartSend[u8Index].u8_recFirstAckFlg = 2;
            }
//            st_heartSend[u8Index].u16_waitTimes = 0;
//            st_heartSend[u8Index].u8_recAckFlag = 0;
//            st_heartSend[u8Index].u8_sendFlg = 0;
//            st_heartSend[u8Index].u8_sendTimes = 0;
            //st_heartSend[u8Index].u8_recFirstAckFlg = 2;
       //     ApiLogPrint(_LOG_ENTRY, "=dddd%d\n", gatDiasSppConnStatus[SPP_CONNECT_TYPE_MCU_4G-1].eConnStatus);
			break;
		}
	}
}



/******************************************************************************
*  function name | DiasSppUninitSendMutex
*  content       | 互斥反初始化 
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppUninitSendMutex(DiasSppConnectType_e eConnectType)
{
	if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		//return (0==pthread_mutex_destroy(&gtDiasSppSendLockMpu))?DIAS_SUCCESS:DIAS_FAILURE; //del by lichenglin @20190614
		return DIAS_SUCCESS;
	}	
	else if((eConnectType == SPP_CONNECT_TYPE_MCU_4G)||(eConnectType == SPP_CONNECT_TYPE_MCU_MPU))
	{
	    return DIAS_SUCCESS;
	}	
	else
	{
		return DIAS_FAILURE;
	}
}

/******************************************************************************
*  function name | DiasSppLockSendMutex
*  content       | 互斥锁处理 
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppLockSendMutex(DiasSppConnectType_e eConnectType)
{
	if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
	    DiasSppEnterCritical();
		return DIAS_SUCCESS;
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MCU_4G)||(eConnectType == SPP_CONNECT_TYPE_MCU_MPU))
	{
	    return DIAS_SUCCESS;
	}	
	else
	{
		return DIAS_FAILURE;
	}
}

/******************************************************************************
*  function name | DiasSppUnlockSendMutex
*  content       | 互斥去锁处理 
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppUnlockSendMutex(DiasSppConnectType_e eConnectType)
{
	if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
        DiasSppLeaveCritical();
		return DIAS_SUCCESS;
	}	
	else if((eConnectType == SPP_CONNECT_TYPE_MCU_4G)||(eConnectType == SPP_CONNECT_TYPE_MCU_MPU))
	{
	    return DIAS_SUCCESS;
	}
	else
	{
		return DIAS_FAILURE;
	}
}
/******************************************************************************
*  function name | DiasSppAddSendListOneNode
*  content       | 添加到发送队列
*  parameter     | eConnectType 连接类型
*     		     | psNodetoAdd 节点指针		 
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppAddSendListOneNode(DiasSppConnectType_e eConnectType,DIAS_SPP_MEM_TYPE * ptNodetoAdd)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	DIAS_SPP_MEM_TYPE *ptSendListCur=NULL;

	if(NULL==ptNodetoAdd)
	{
		return DIAS_FAILURE;
	}

	eRet=DiasSppCheckConnectType(eConnectType);
	if(DIAS_FAILURE==eRet)
	{
		return DIAS_FAILURE;
	}

	eRet=DiasSppWhetherAddToSendList(eConnectType,ptNodetoAdd->u16EventId);
	if(DIAS_FAILURE==eRet)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppWhetherAddToSendList failed u16EventId %d eConnectType %d ",ptNodetoAdd->u16EventId,eConnectType);
		return DIAS_FAILURE;
	}	
	else
	{
		ptSendListCur=DiasSppGetSendlistByConnectType(eConnectType);
		ptSendListCur=DiasSppMemAddNode(ptSendListCur, ptNodetoAdd);			
		if(NULL==ptSendListCur)
		{
			DiasDcpdLog(LOG_INFO,"DiasSppMemAddNode fail.");
			return DIAS_FAILURE;
		}
		else
		{		
			DiasSppSetSendlistByConnectType(eConnectType,ptSendListCur);
			DiasDcpdLog(LOG_ERR,"DiasSppMemAddNode success.");
		}
	}
	return DIAS_SUCCESS;

}

/******************************************************************************
*  function name | DiasSppAddSendList
*  content       | 添加到发送队列
*  parameter     | eConnectType 连接类型
*     		     | u16EventId 事件类型
*                | pucData 发送数据指针 u16DataLen数据长度	u8AllNum分片总数      u8CurNum当前分片			 
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppAddSendList(DiasSppConnectType_e eConnectType,UINT16 u16EventId, UINT8 * pucData, UINT16 u16DataLen, UINT8 u8AllNum, UINT8 u8CurNum)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	DIAS_SPP_MEM_TYPE *ptSendListCur=NULL;
	DIAS_SPP_MEM_TYPE tNodeToAdd;
	void* pMem =NULL;	
	UINT8 u8ResendTimes=0;

	if(NULL == pucData)
	{
		DiasDcpdLog(LOG_ERR,"ptData or len wrong");
		return DIAS_FAILURE;
	}
	
	DiasDcpdLog(LOG_DEBUG,"EventId %d AllNum %d CurNum %d len %d",u16EventId,u8AllNum,u8CurNum,u16DataLen);

	memset(&tNodeToAdd, 0,sizeof(DIAS_SPP_MEM_TYPE));	
	tNodeToAdd.u16EventId=u16EventId;
	tNodeToAdd.u16DataLen=u16DataLen;
	tNodeToAdd.u8SplitedNumAll=u8AllNum;
	tNodeToAdd.u8SplitedNumCur=u8CurNum;
	tNodeToAdd.u8Priority=DiasSppGetPrioty(u16EventId);
	tNodeToAdd.u8ResendTimes=0;    
	tNodeToAdd.u16CmdTimeout=DIAS_SPP_ACK_TIMEOUT_TIME;
	//pMem = malloc(u16DataLen);	
//	if (NULL != pMem)
//	{
//	//	tNodeToAdd.pDcpdData=pMem;
//	//	memcpy(tNodeToAdd.pDcpdData, pucData, u16DataLen);				
//	}
//	else
//	{
//		DiasDcpdLog(LOG_ERR,"malloc error");
//		return DIAS_FAILURE;
//	}	
    memcpy(tNodeToAdd.pDcpdData, pucData, u16DataLen);	
	DiasSppLockSendMutex(eConnectType);
	eRet=DiasSppAddSendListOneNode(eConnectType,&tNodeToAdd);
	DiasSppUnlockSendMutex(eConnectType);
	if(DIAS_FAILURE==eRet)
	{
		//free(pMem);
		//pMem=NULL;        
	    //memset((UINT8*)(&tNodeToAdd->pDcpdData[0]),0,sizeof(tNodeToAdd->pDcpdData));
	}	
	return eRet;

}

/******************************************************************************
*  function name | DiasSppDelSendListbyCmdid
*  content       | 添加到发送队列
*  parameter     | eConnectType 连接类型
*     		     | u16EventId 事件类型
*                |
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppDelSendListbyCmdid(DiasSppConnectType_e eConnectType,UINT16 u16EventId)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	DIAS_SPP_MEM_TYPE *ptSendListCur=NULL;
	
	eRet=DiasSppCheckConnectType(eConnectType);	
	if(DIAS_FAILURE==eRet)
	{
		return DIAS_FAILURE;
	}

	ptSendListCur=DiasSppGetSendlistByConnectType(eConnectType);	
	if(NULL==ptSendListCur)
	{
		return DIAS_FAILURE;
	}	
	DiasSppLockSendMutex(eConnectType);
	ptSendListCur=DiasSppMemDeleteNodebyCmdid(ptSendListCur,u16EventId);
	DiasSppSetSendlistByConnectType(eConnectType,ptSendListCur);
	DiasSppUnlockSendMutex(eConnectType);
	return eRet;
}

DiasErrorCode_e DiasSppInitForAll(VOID)
{
	gatDiasSppConnStatus[0].eConnectType = SPP_CONNECT_TYPE_MCU_4G;
	gatDiasSppConnStatus[1].eConnectType = SPP_CONNECT_TYPE_MCU_MPU;
	gatDiasSppConnStatus[2].eConnectType = SPP_CONNECT_TYPE_4G;
	gatDiasSppConnStatus[3].eConnectType = SPP_CONNECT_TYPE_MPU;
	
	memset(gatDiasSppMsgProcList, 0, DIAS_SPP_CALLBACK_MAX_COUNT*sizeof(DiasSppMsgProc_t));
	memset(&gtAllMsg, 0, sizeof(DiasSppRecvMcuMsg_t));
	gptDiasSppSendListMpu = NULL;
	gptDiasSppCheckAckListMpu = NULL;

	//init sent list memory
	if(DIAS_FAILURE==DiasSppMemInit())
	{
		DiasDcpdLog(LOG_ERR,"DiasSppMemInit failed");
		return DIAS_FAILURE;
	}

	return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppInit
*  content       | 初始化
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppInit(DiasSppConnectType_e eConnectType)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
    UINT8 u8Index = eConnectType -1;

	//init conn status
	//DiasSppSetConnStatus(eConnectType,DIAS_SPP_STATE_INIT);
    gatDiasSppConnStatus[u8Index].eConnStatus= DIAS_SPP_STATE_INIT;
    st_heartSend[u8Index].u16_waitTimes = DIAS_SPP_HEART_SEND_PERIOD;
    st_heartSend[u8Index].u8_recAckFlag = 0;
    st_heartSend[u8Index].u8_sendFlg = 0;
    st_heartSend[u8Index].u8_sendTimes = 0;
    st_heartSend[u8Index].u8_recFirstAckFlg = 0;

	eRet = DiasSppComOpen(eConnectType);
	if(DIAS_FAILURE==eRet)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppComOpen failed");
		return eRet;
	}

	if(DIAS_FAILURE==eRet)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppcInitSendMutex failed");
		return eRet;
	}

	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		gptDiasSppSendListMcu4G = NULL;
		gptDiasSppCheckAckListMcu4G = NULL;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		gptDiasSppSendListMcuMpu = NULL;
		gptDiasSppCheckAckListMcuMpu = NULL;
	}



	return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppStart
*  content       | 启动
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppStart(DiasSppConnectType_e eConnectType)
{
	//open port
	//DiasErrorCode_e eRet=DIAS_FAILURE;
	//eRet=DiasSppComInit(eConnectType);

	//send handshake
}

/******************************************************************************
*  function name | DiasSppDeInit
*  content       | 反初始化
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppDeInit(DiasSppConnectType_e eConnectType)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;

	//Deinit conn status
	DiasSppSetConnStatus(eConnectType,DIAS_SPP_CON_UNKNOWN);

	//creat send mutex
	DiasSppUninitSendMutex(eConnectType);

	gptDiasSppSendListMcu4G = NULL;
	gptDiasSppSendListMcuMpu = NULL;
	gptDiasSppSendListMpu = NULL;
	gptDiasSppCheckAckListMcu4G = NULL;
	gptDiasSppCheckAckListMcuMpu = NULL;
	gptDiasSppCheckAckListMpu = NULL;

	//delete send list
	eRet=DiasSppMemUninit();
	if(DIAS_FAILURE==eRet)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppMemUninit failed");
	}
}

/******************************************************************************
*  function name | DiasSppStop
*  content       | 停止
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppStop(DiasSppConnectType_e eConnectType)
{
	//close port
	DiasSppSetConnStatus(eConnectType,DIAS_SPP_STATE_UNKNOW); 
	return DiasSppComClose(eConnectType);

}

/******************************************************************************
*  function name | DiasSppSetListener
*  content       | 设置回调
*  parameter     | eConnectType 连接类型
*     		     | u16EventId 事件类型
*                | pDiasSppcListener					
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppSetListener(DiasSppConnectType_e eConnectType,UINT16 u16EventId,pDiasSppListener pSppcListener)
{
      /*enter critical*/
  
	//add to cb list
	DiasErrorCode_e eRet=DIAS_FAILURE;
	UINT16 u16Index = 0; 

	eRet=DiasSppCheckConnectType(eConnectType);
	if(DIAS_FAILURE==eRet)
	{
		return eRet;
	}
    if(NULL == pSppcListener)
    {
		DiasDcpdLog(LOG_ERR,"pDiasSppcListener is NULL");        
		return DIAS_FAILURE;
    }
    DiasSppEnterCritical();
    for(u16Index=0; u16Index<sizeof(gatDiasSppMsgProcList)/sizeof(DiasSppMsgProc_t); u16Index++)
    {
        if((0 == gatDiasSppMsgProcList[u16Index].eConnectType)&&
			(0 == gatDiasSppMsgProcList[u16Index].u16EventId))
        {
			gatDiasSppMsgProcList[u16Index].eConnectType=eConnectType;
			gatDiasSppMsgProcList[u16Index].u16EventId=u16EventId;
			gatDiasSppMsgProcList[u16Index].pFunCb = pSppcListener;
	        DiasDcpdLog(LOG_INFO,"gatDiasSppMsgProcList set eConnectType %d u16EventId %d success",eConnectType,u16EventId);
            DiasSppLeaveCritical();
            return DIAS_SUCCESS;
        }
    }
	DiasDcpdLog(LOG_ERR,"gatDiasSppMsgProcList set u16EventId %d fail",u16EventId);
    DiasSppLeaveCritical();
	return DIAS_FAILURE;
}

/******************************************************************************
*  function name | DiasSppSend
*  content       | 发送
*  parameter     | eConnectType 连接类型
*     		     | u16EventId 事件类型
*                | pucData 发送数据指针 u16DataLen数据长度				
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppSend(DiasSppConnectType_e eConnectType,UINT16 u16EventId, UINT8 * pucData, UINT16 u16DataLen)
{
	//add to send list
	DiasErrorCode_e eRet=DIAS_FAILURE;
	DIAS_SPP_MEM_TYPE *ptSendListCur=NULL;
	DIAS_SPP_MEM_TYPE tNodeToAdd;
	UINT8 u8SplitedNum=0;
	UINT8 u8Temp=0;
	UINT8 u8Index=0;
	UINT16 u16SplitPackLen=0;
    if (u16EventId == 159)
    {
   //     ApiLogPrint(_LOG_ENTRY, "send:event0x%2x%2x\n",(UINT8)u16EventId>>8,(UINT8)u16EventId);
    }
	if((NULL == pucData)||(u16DataLen<sizeof(DiasSppPackageHeader_t)))
	{
		DiasDcpdLog(LOG_ERR,"ptData or len wrong");
		return DIAS_FAILURE;
	}
    if(gatDiasSppConnStatus[eConnectType-1].eConnStatus != DIAS_SPP_STATE_RUNNING)
    {
     //   return DIAS_SUCCESS;
    }
  //  ApiLogPrint(_LOG_ENTRY, "send:event0x%2x%2x\n",(UINT8)u16EventId>>8,(UINT8)u16EventId);
	u8SplitedNum=u16DataLen/DIAS_SPP_PACKET_DATA_LEN_MAX;
	u8Temp=u16DataLen%DIAS_SPP_PACKET_DATA_LEN_MAX;
	if(0 != u8Temp)
	{
		u8SplitedNum++;
	}

	DiasDcpdLog(LOG_DEBUG,"u8SplitedNum is %d",u8SplitedNum);
    DiasSppEnterCritical();

	if(1==u8SplitedNum)
	{
		eRet=DiasSppAddSendList(eConnectType,u16EventId,pucData,u16DataLen,u8SplitedNum,1);
        DiasSppLeaveCritical();
        return eRet;
	}
	else
	{
		for(u8Index=0; u8Index<u8SplitedNum; u8Index++)
	    {
			if(u8Index!=u8SplitedNum-1)
			{
				u16SplitPackLen = DIAS_SPP_PACKET_DATA_LEN_MAX;
			}
			else
			{
				u16SplitPackLen = u16DataLen - u8Index*DIAS_SPP_PACKET_DATA_LEN_MAX;
			}
			eRet=DiasSppAddSendList(eConnectType,u16EventId,pucData+u8Index*DIAS_SPP_PACKET_DATA_LEN_MAX,u16SplitPackLen,u8SplitedNum,u8Index+1);
			if(DIAS_FAILURE==eRet)
			{
				DiasSppDelSendListbyCmdid(eConnectType,u16EventId);
			}
		}	
	}
    DiasSppLeaveCritical();
	return eRet;
	
}

/******************************************************************************
*  function name | DiasSppGetStatus
*  content       | 获取连接状态
*  parameter     | eConnectType 连接类型				
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasSppState_e DiasSppGetStatus(DiasSppConnectType_e eConnectType)
{
	INT8 i8Index=0;
	DiasSppState_e eConnStatusResult=DIAS_SPP_STATE_UNKNOW;
	
	for(i8Index=0; i8Index<SPP_CONNECT_MAX_COUNT; i8Index++)
    {
		if(eConnectType==gatDiasSppConnStatus[i8Index].eConnectType)
		{
			eConnStatusResult=gatDiasSppConnStatus[i8Index].eConnStatus;
			break;
		}
	}	
	return eConnStatusResult;
}

/******************************************************************************
*  function name | DiasSppAddConnectionStateChangeListener
*  content       | 设置连接状态变化回调
*  parameter     | eConnectType 连接类型
*                | pDiasSppcConnStateChangeCb					
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppAddConnectionStateChangeListener(DiasSppConnectType_e eConnectType,void* pDiasSppcConnStateChangeListener)
{
	//add to conn cb list
	DiasErrorCode_e eRet=DIAS_FAILURE;
	UINT8 u8Index = 0; 

	eRet=DiasSppCheckConnectType(eConnectType);
	if(DIAS_FAILURE==eRet)
	{
		return eRet;
	}
	
    if(NULL == pDiasSppcConnStateChangeListener)
    {
		DiasDcpdLog(LOG_ERR,"pDiasSppcConnStateChangeCb is NULL");
		return DIAS_FAILURE;
    }

    for(u8Index=0; u8Index<sizeof(gatDiasSppConnMsgProcList)/sizeof(DiasSppConnMsgProc_t); u8Index++)
    {
        if(eConnectType == gatDiasSppConnMsgProcList[u8Index].eConnectType)
        {
            gatDiasSppConnMsgProcList[u8Index].pFunConnCb = pDiasSppcConnStateChangeListener;
            return DIAS_SUCCESS;
        }
    }
	return DIAS_FAILURE;
}


/******************************************************************************
*  function name | DiasSppTimeCount
*  content       | ack Timeout
*  parameter[1]  | eConnectType :
*  notice        | 
*  return        | 
******************************************************************************/
VOID DiasSppTimeCount(DiasSppConnectType_e eConnectType)
{
    DIAS_SPP_MEM_TYPE* ptAckCheckListCur;
    ptAckCheckListCur=DiasSppGetCheckAcklistByConnectType(eConnectType);    
    while(ptAckCheckListCur != NULL)
    {
        ptAckCheckListCur->u32TimeCout += TIME_STEP;
        ptAckCheckListCur = ptAckCheckListCur->next;
    }
    
}

BOOL bl_enableHeart = TRUE;

/******************************************************************************
*  function name | DiasSppHeartManage
*  content       | heart packet management
*  parameter[1]  | eConnectType : connectType
*  notice        | 
*  return        | 
******************************************************************************/
VOID DiasSppHeartManage(DiasSppConnectType_e eConnectType)
{
    UINT8 u8_indexcon = eConnectType -1;
    if ((UINT8)u8_indexcon>=sizeof(st_heartSend))
    {
        return;
    }
#if 1
    if (bl_enableHeart == FALSE)
    {
        return;
    }
    else
    {
        if (gatDiasSppConnStatus[(UINT8)u8_indexcon].eConnStatus == DIAS_SPP_STATE_RUNNING)
        {
            bl_enableHeart = FALSE;
            return;
        }
        else
        {
        }
    }
#endif
    if(st_heartSend[(UINT8)u8_indexcon].u16_waitTimes<DIAS_SPP_HEART_SEND_PERIOD)
	{
	    st_heartSend[(UINT8)u8_indexcon].u16_waitTimes += TIME_STEP;
	}
	else
	{
	  //  ApiLogPrint(_LOG_ENTRY, "==statte%d\n",gatDiasSppConnStatus[SPP_CONNECT_TYPE_MCU_4G-1].eConnStatus);
	    st_heartSend[(UINT8)u8_indexcon].u16_waitTimes = 0;
		if((1==st_heartSend[(UINT8)u8_indexcon].u8_sendFlg)
            &&(0==st_heartSend[(UINT8)u8_indexcon].u8_recAckFlag)
            &&(2==st_heartSend[(UINT8)u8_indexcon].u8_recFirstAckFlg ))  
		{
            if (st_heartSend[(UINT8)u8_indexcon].u8_sendTimes == 3)
            {
                gatDiasSppConnStatus[(UINT8)u8_indexcon].eConnStatus = DIAS_SPP_STATE_ERROR;
                st_heartSend[(UINT8)u8_indexcon].u8_sendTimes = 3;
                ApiLogPrint(_LOG_ENTRY, "-===error==\n");
            }
            else
            {
                gatDiasSppConnStatus[(UINT8)u8_indexcon].eConnStatus = DIAS_SPP_STATE_WARNING;
                st_heartSend[(UINT8)u8_indexcon].u8_sendTimes++;
                ApiLogPrint(_LOG_ENTRY, "-===warning==\n");
            }
		}
        else if(st_heartSend[(UINT8)u8_indexcon].u8_recFirstAckFlg == 1)
        {
            gatDiasSppConnStatus[(UINT8)u8_indexcon].eConnStatus = DIAS_SPP_STATE_INIT;
            
         //   ApiLogPrint(_LOG_ENTRY, "-===init==\n");
        }
		else 
		{
		    st_heartSend[(UINT8)u8_indexcon].u8_sendTimes = 0;
		}
        DiasSppSendHeart(eConnectType);        
        st_heartSend[(UINT8)u8_indexcon].u8_sendFlg = 1;
        st_heartSend[(UINT8)u8_indexcon].u8_recAckFlag = 0;
        if (st_heartSend[(UINT8)u8_indexcon].u8_recFirstAckFlg != 2)
        {
            st_heartSend[(UINT8)u8_indexcon].u8_recFirstAckFlg = 1;
        }
       // ApiLogPrint(_LOG_TRACE, "==heart===\n");
	}
}
/******************************************************************************
*  function name | DiasSppMainTask
*  content       | mcu 收发处理
*  parameter     | eConnectType 连接类型		
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppMainTask(DiasSppConnectType_e eConnectType)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	UINT8 u8Idx = 0;
	//UINT8 au8Test[DIAS_SPP_PACKET_LEN_MAX] = {0};

	if(1 == gu8CanRevFlag)
	{
		return;
	}

	if((eConnectType != SPP_CONNECT_TYPE_MCU_4G) && (eConnectType != SPP_CONNECT_TYPE_MCU_MPU))
	{
		return DIAS_FAILURE;
	}
    DiasSppTimeCount(eConnectType);
    DiasSppHeartManage(eConnectType);
//	for(u8Idx=0; u8Idx<5; u8Idx++)
//	{
		DiasSppRecieveDataProc(eConnectType);
//	}
	
	for(u8Idx=0; u8Idx<1; u8Idx++)
	{
		DiasSppSendData(eConnectType);
	}

}


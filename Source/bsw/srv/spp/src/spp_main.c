#include <stdio.h>
#include <stdlib.h>
/* #include "Os.h"
#include "Os_cfg.h" *//* by S811 */
#include "spp_main.h"
#include "spp_memory.h"
#include "spp_code.h"
#include "spp_type.h"
#include "spp_com.h"
#include "spp_generated.h"
/*#include "mcu_com_uart.h"*//*by s811*/
#include "spp_callout.h"
#include "os_log.h"

/**********************************************************************/
/*                        Global variable                             */
/**********************************************************************/
SppConnectStatus_t gatSppConnStatus[SPP_CONNECT_MAX_COUNT];
SppMemType_t *gptSppSendListMcu4G = NULL;
SppMemType_t *gptSppSendListMcuMpu = NULL;
SppMemType_t *gptSppSendListMpu = NULL;
SppMemType_t *gptSppCheckAckListMcu4G = NULL;
SppMemType_t *gptSppCheckAckListMcuMpu = NULL;
SppMemType_t *gptSppCheckAckListMpu = NULL;

SppConnMsgProc_t gatSppConnMsgProcList[SPP_CONNECT_MAX_COUNT];
SppMsgProc_t gatSppMsgProcList[SPP_CALLBACK_MAX_COUNT];
SppSendAckProc_t gatSppSendAckProcList[SPP_ACK_MAX_COUT];
//SppMsgBuffRec_t gatSppMsgRecList[SPP_CALLBACK_MAX_COUNT];



UINT8 gucSppRecvBuf[SPP_BIG_PACKET_LEN_MAX] = {0};
UINT32 gu32SppRecvBufLen = 0;
SppRecvMcuMsg_t gtAllMsg;
UINT8 gu8Temp[SPP_BIG_PACKET_LEN_MAX] = {0};
SppHeartManageType gatHeartSend[SPP_CONNECT_MAX_COUNT] ;

SppStat_t gatSppState1[SPP_CONNECT_MAX_COUNT];


/**********************************************************************/
/*                            Functions                               */
/**********************************************************************/
static SPPErrorCode_e SppCheckConnectType(SppConnectType_e eConnectType);
static SppMemType_t * SppGetSendlistByConnectType(SppConnectType_e eConnectType);
static void SppSetSendlistByConnectType(SppConnectType_e eConnectType,SppMemType_t * ptList);
static SPPErrorCode_e SppGetSendNode(SppConnectType_e eConnectType,SppMemType_t** pptNode);
static SPPErrorCode_e SppSendMsgToDevice(SppConnectType_e eConnectType,SppMcuMsg_t * ptMsg);
static SPPErrorCode_e SppSendHeart(SppConnectType_e eConnectType);
static SPPErrorCode_e SppAckMsgProc(SppConnectType_e eConnectType,SppRecvMcuMsg_t* ptRecvMsg);
static SPPErrorCode_e SppRecvCb(SppConnectType_e eConnectType,SppRecvMcuMsg_t* ptRecvMsg);
static void SppRecieveDataProc(SppConnectType_e eConnectType);
static void SppSetConnStatus(SppConnectType_e eConnectType, SppState_e eNewStatus);
static SPPErrorCode_e SppInitSendMutex(SppConnectType_e eConnectType);
static SPPErrorCode_e SppUninitSendMutex(SppConnectType_e eConnectType);
static SPPErrorCode_e SppLockSendMutex(SppConnectType_e eConnectType);
static SPPErrorCode_e SppUnlockSendMutex(SppConnectType_e eConnectType);
static SPPErrorCode_e SppAddSendListOneNode(SppConnectType_e eConnectType,SppMemType_t * ptNodetoAdd);
static SPPErrorCode_e SppDelSendListbyCmdid(SppConnectType_e eConnectType,UINT16 u16EventId);
static void SppEventSendResultCb(SppConnectType_e eConnectType,UINT16 pu16Event[],UINT8 u8SendResult);
static SPPErrorCode_e SppAddAckListOneNode(SppConnectType_e eConnectType,SppMemType_t * ptNodetoAdd);
static void SppSetRunningState(UINT8 u8ConnectIndex,UINT8 u8PacketType );


/******************************************************************************
*  function name | SppCheckConnectType
*  content       | 检查连接类型
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppCheckConnectType(SppConnectType_e eConnectType)
{
	if((eConnectType < SPP_CONNECT_TYPE_MCU_4G) || (eConnectType > SPP_CONNECT_TYPE_MPU))
	{
		return SPP_FAILURE;
	}
	return SPP_SUCCESS;
}

/******************************************************************************
*  function name | SppGetSendlistByConnectType
*  content       | 获取发送队列
*  parameter     | eConnectType 连接类型
*  return        | SppMemType_t *
*  notice        | 
******************************************************************************/
static SppMemType_t * SppGetSendlistByConnectType(SppConnectType_e eConnectType)
{
	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		return gptSppSendListMcu4G;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		return gptSppSendListMcuMpu;	
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		return gptSppSendListMpu;
	}
    else
    {
        return NULL;
    }
}
static SppMemType_t * SppGetAcklistByConnectType(SppConnectType_e eConnectType)
{
	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		return gptSppCheckAckListMcu4G;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		return gptSppCheckAckListMcuMpu;	
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		return gptSppCheckAckListMpu;
	}
    else
    {
        return NULL;
    }
}

/******************************************************************************
*  function name | SppGetSendlistByConnectType
*  content       | 更新发送队列
*  parameter     | eConnectType 连接类型 ptList 队列指针
*  return        | 
*  notice        | 
******************************************************************************/
static void SppSetSendlistByConnectType(SppConnectType_e eConnectType,SppMemType_t * ptList)
{
	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		gptSppSendListMcu4G=ptList;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		gptSppSendListMcuMpu=ptList;	
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		gptSppSendListMpu=ptList;
	}
    else
    {}
}

static void SppSetAcklistByConnectType(SppConnectType_e eConnectType,SppMemType_t * ptList)
{

	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		gptSppCheckAckListMcu4G=ptList;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		gptSppCheckAckListMcuMpu=ptList;	
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		gptSppCheckAckListMpu=ptList;
	}
    else
    {}
}





/******************************************************************************
*  function name | SppGetSendNode
*  content       | 获取发送节点
*  parameter     | eConnectType 连接类型 pptNode 发送节点指针地址
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppGetSendNode(SppConnectType_e eConnectType,SppMemType_t** pptNode)
{	
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMemType_t *ptSendListCur=NULL;
	SppMemType_t *ptNode=NULL;	

	if(NULL == pptNode)
	{
		return SPP_FAILURE;
	}
	
	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}


	ptSendListCur=SppGetSendlistByConnectType(eConnectType);
	if(NULL==ptSendListCur)
	{
		return SPP_FAILURE;
	}
	
	SppLockSendMutex(eConnectType);
	ptNode=SppMemGetFirstNode(ptSendListCur);

	if(NULL != ptNode)
	{
		ptSendListCur=ptNode->next;
		if(NULL!=ptSendListCur)
		{
			ptSendListCur->prev=NULL;
		}
		SppSetSendlistByConnectType(eConnectType,ptSendListCur);
		SppUnlockSendMutex(eConnectType);	

		*pptNode = ptNode;
		return SPP_SUCCESS;		
	}	
	SppUnlockSendMutex(eConnectType);	
	return SPP_FAILURE;	
}

static SPPErrorCode_e SppGetAckNode(SppConnectType_e eConnectType,SppMemType_t** pptNode)
{	
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMemType_t *ptSendListCur=NULL;
	SppMemType_t *ptNode=NULL;	

	if(NULL == pptNode)
	{
		return SPP_FAILURE;
	}
	
	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}


	ptSendListCur=SppGetAcklistByConnectType(eConnectType);
	if(NULL==ptSendListCur)
	{
		return SPP_FAILURE;
	}
	
	SppLockSendMutex(eConnectType);
	ptNode=SppMemGetFirstNode(ptSendListCur);

	if(NULL != ptNode)
	{
		ptSendListCur=ptNode->next;
		if(NULL!=ptSendListCur)
		{
			ptSendListCur->prev=NULL;
		}
		SppSetAcklistByConnectType(eConnectType,ptSendListCur);
		SppUnlockSendMutex(eConnectType);	

		*pptNode = ptNode;
		return SPP_SUCCESS;		
	}	
	SppUnlockSendMutex(eConnectType);	
	return SPP_FAILURE;	
}



static SPPErrorCode_e SppSendPacketToDevice(SppConnectType_e eConnectType,SppPacketType * ptPacket)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	static UINT8 aucDestBuf[SPP_BIG_PACKET_LEN_MAX]={0};
	UINT32 u32Destlen=SPP_BIG_PACKET_LEN_MAX;
	UINT8 u8Crc=0;
	if(NULL==ptPacket)
	{
		return SPP_FAILURE;
	}
	
 	(void)memset(aucDestBuf, 0xFF, sizeof(aucDestBuf));
	eRet = SppPacketEncode(aucDestBuf,SPP_BIG_PACKET_LEN_MAX,&u32Destlen,ptPacket);		
 	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}	    
    eRet = SppComSend(eConnectType, aucDestBuf, u32Destlen);
	return eRet;

}


static SPPErrorCode_e SppSendNodeToDevice(SppConnectType_e eConnectType,SppMemType_t *ptSendNode)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppPacketType tPacket;
	UINT8 u8Crc=0;
	
	if(NULL==ptSendNode)
	{
		return SPP_FAILURE;
	}
    ptSendNode->u8SendTimes++;
    ptSendNode->u32TimeCout = 0;
    memset(&tPacket,0,sizeof(tPacket));
    tPacket.u8Hdle = SPP_PACKET_HEAD_DLE;
    tPacket.u8Hsom = SPP_PACKET_HEAD_SOM;
    tPacket.u8Sn   = ptSendNode->u8Sn;  
    tPacket.u8Sta = (UINT8)(gatSppConnStatus[(UINT8)eConnectType - 1].eConnStatus);
    memcpy(tPacket.au8PacketPayload,ptSendNode->pDcpdData,sizeof(ptSendNode->pDcpdData));
    tPacket.u8Type = ptSendNode->u8Type;
    tPacket.u8Tdle = SPP_PACKET_TAIL_DLE;
    tPacket.u8Teof = SPP_PACKET_TAIL_EOF;
    
	SppGetPacketCrc(&tPacket,&u8Crc);
    tPacket.u8Crc = u8Crc;
    //ApiLogPrint(_LOG_ENTRY, "CRC:%d\n", u8Crc);
    eRet = SppSendPacketToDevice(eConnectType,&tPacket);
    return eRet;
}

/******************************************************************************
*  function name | SppSendHeart
*  content       | 发送心跳
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppSendHeart(SppConnectType_e eConnectType)
{
    SppPacketType tPacketAck;
    UINT8 u8Crc = 0;
    memset(&tPacketAck,0,sizeof(tPacketAck));
    
    tPacketAck.u8Hdle = SPP_PACKET_HEAD_DLE;
    tPacketAck.u8Hsom = SPP_PACKET_HEAD_SOM;
    tPacketAck.u8Sn = 0;
    tPacketAck.u8Type = SPP_PACKET_TYPE_HB;
    tPacketAck.u8Sta = (UINT8)(gatSppConnStatus[(UINT8)eConnectType - 1].eConnStatus);
    memset(tPacketAck.au8PacketPayload,0xff,SPP_BIG_PACKET_PAYLOAG_LEN_MAX);
    tPacketAck.u8Tdle = SPP_PACKET_TAIL_DLE;
    tPacketAck.u8Teof = SPP_PACKET_TAIL_EOF;
    SppGetPacketCrc(&tPacketAck,&u8Crc);
    tPacketAck.u8Crc = u8Crc;    
    SppSendPacketToDevice(eConnectType,&tPacketAck);
	return SPP_SUCCESS;
}






static SPPErrorCode_e SppDelSendNodeByNodes(SppConnectType_e eConnectType,SppMemType_t* ptNode)
{
    SPPErrorCode_e eRet=SPP_FAILURE;
    SppMemType_t *ptSendListCur=NULL;
    UINT8 u8Sn = 0;
    UINT8 u8_deleIdex = 0;
    
    eRet=SppCheckConnectType(eConnectType); 
    if(SPP_FAILURE==eRet)
    {
        return SPP_FAILURE;
    }

    ptSendListCur=SppGetSendlistByConnectType(eConnectType);    
    if(NULL==ptSendListCur)
    {
        return SPP_FAILURE;
    }
    u8Sn = ptNode->u8Sn;
	SppLockSendMutex(eConnectType);
    for (u8_deleIdex = 0; u8_deleIdex <= (ptNode->u8SplitedNumAll - ptNode->u8SplitedNumCur); u8_deleIdex++)
    {
        ptSendListCur=SppMemDeleteNodebySN(ptSendListCur,(u8Sn + u8_deleIdex)%0xff);
        SppSetSendlistByConnectType(eConnectType,ptSendListCur);
    }
	SppUnlockSendMutex(eConnectType);
    
    return eRet;
    
}








static SPPErrorCode_e SppDelSendNodeByNodesIncrease(SppConnectType_e eConnectType,SppMemType_t* ptNode)
{
    SPPErrorCode_e eRet=SPP_FAILURE;
    SppMemType_t *ptSendListCur=NULL;
    UINT8 u8Sn = 0;
    UINT8 u8_deleIdex = 0;
    
    eRet=SppCheckConnectType(eConnectType); 
    if(SPP_FAILURE==eRet)
    {
        return SPP_FAILURE;
    }

    ptSendListCur=SppGetSendlistByConnectType(eConnectType);    
    if(NULL==ptSendListCur)
    {
        return SPP_FAILURE;
    }
    u8Sn = ptNode->u8Sn;
	SppLockSendMutex(eConnectType);
    for (u8_deleIdex = 0; u8_deleIdex <= (ptNode->u8SplitedNumCur - 1); u8_deleIdex++)
    {
        ptSendListCur=SppMemDeleteNodebySN(ptSendListCur,(u8Sn + u8_deleIdex)%0xff);
        SppSetSendlistByConnectType(eConnectType,ptSendListCur);
    }
	SppUnlockSendMutex(eConnectType);
    
    eRet = SPP_SUCCESS;
    return eRet;
    
}





static SPPErrorCode_e SppDelSendNodeByOneNodes(SppConnectType_e eConnectType,SppMemType_t* ptNode)
{
    SPPErrorCode_e eRet=SPP_FAILURE;
    SppMemType_t *ptSendListCur=NULL;
    UINT8 u8Sn = 0;
    UINT8 u8_deleIdex = 0;
    
    eRet=SppCheckConnectType(eConnectType); 
    if(SPP_FAILURE==eRet)
    {
        return SPP_FAILURE;
    }

    ptSendListCur=SppGetSendlistByConnectType(eConnectType);    
    if(NULL==ptSendListCur)
    {
        return SPP_FAILURE;
    }
    u8Sn = ptNode->u8Sn;
	SppLockSendMutex(eConnectType);
    for (u8_deleIdex = 0; u8_deleIdex <= (ptNode->u8SplitedNumAll - ptNode->u8SplitedNumCur); u8_deleIdex++)
    {
        ptSendListCur=SppMemDeleteNodebySN(ptSendListCur,(u8Sn + u8_deleIdex)%0xff);
        SppSetSendlistByConnectType(eConnectType,ptSendListCur);
    }
	SppUnlockSendMutex(eConnectType);
    
    return eRet;
    
}


static SPPErrorCode_e SppDelAckNodeByNodes(SppConnectType_e eConnectType,SppMemType_t* ptNode)
{
    SPPErrorCode_e eRet=SPP_FAILURE;
    SppMemType_t *ptSendListCur=NULL;
    UINT8 u8Sn = 0;
    UINT8 u8_deleIdex = 0;
    
    eRet=SppCheckConnectType(eConnectType); 
    if(SPP_FAILURE==eRet)
    {
        return SPP_FAILURE;
    }

    ptSendListCur=SppGetAcklistByConnectType(eConnectType);    
    if(NULL==ptSendListCur)
    {
        return SPP_FAILURE;
    }
    u8Sn = ptNode->u8Sn;
	SppLockSendMutex(eConnectType);
    for (u8_deleIdex = 0; u8_deleIdex <= (ptNode->u8SplitedNumAll - ptNode->u8SplitedNumCur); u8_deleIdex++)
    {
        ptSendListCur=SppMemDeleteNodebySN(ptSendListCur,(u8Sn + u8_deleIdex)%0xff);
        SppSetAcklistByConnectType(eConnectType,ptSendListCur);
    }
	SppUnlockSendMutex(eConnectType);
    eRet = SPP_SUCCESS;
    return eRet;
    
}


static SPPErrorCode_e SppGetSendFirstNode(SppConnectType_e eConnectType,SppMemType_t** pptNode)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMemType_t *ptSendListCur=NULL;
	SppMemType_t *ptNode=NULL;	

	if(NULL == pptNode)
	{
		return SPP_FAILURE;
	}
	
	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}


	ptSendListCur=SppGetSendlistByConnectType(eConnectType);
	if(NULL==ptSendListCur)
	{
		return SPP_FAILURE;
	}
	
	SppLockSendMutex(eConnectType);
	ptNode=SppMemGetFirstNode(ptSendListCur);

	if(NULL != ptNode)
	{
        SppUnlockSendMutex(eConnectType);   
		*pptNode = ptNode;
		return SPP_SUCCESS;		
	}	
	SppUnlockSendMutex(eConnectType);	
	return SPP_FAILURE;	
}


static SPPErrorCode_e SppGetOutSendFirstNode(SppConnectType_e eConnectType,SppMemType_t** pptNode)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMemType_t *ptSendListCur=NULL;
	SppMemType_t *ptNode=NULL;	

	if(NULL == pptNode)
	{
		return SPP_FAILURE;
	}
	
	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}


	ptSendListCur=SppGetSendlistByConnectType(eConnectType);
	if(NULL==ptSendListCur)
	{
		return SPP_FAILURE;
	}
	
	SppLockSendMutex(eConnectType);
	ptNode=SppMemGetFirstNode(ptSendListCur);

	if(NULL != ptNode)
	{
		ptSendListCur=ptNode->next;
		if(NULL!=ptSendListCur)
		{
			ptSendListCur->prev=NULL;
		}
		SppSetSendlistByConnectType(eConnectType,ptSendListCur);
		SppUnlockSendMutex(eConnectType);	

		*pptNode = ptNode;
		return SPP_SUCCESS;		
	}		
	SppUnlockSendMutex(eConnectType);	
	return SPP_FAILURE;	
}


static SPPErrorCode_e SppGetAckFirstNode(SppConnectType_e eConnectType,SppMemType_t** pptNode)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMemType_t *ptSendListCur=NULL;
	SppMemType_t *ptNode=NULL;	

	if(NULL == pptNode)
	{
		return SPP_FAILURE;
	}
	
	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}


	ptSendListCur=SppGetAcklistByConnectType(eConnectType);
	if(NULL==ptSendListCur)
	{
		return SPP_FAILURE;
	}
	
	SppLockSendMutex(eConnectType);
	ptNode=SppMemGetFirstNode(ptSendListCur);

	if(NULL != ptNode)
	{
        SppUnlockSendMutex(eConnectType);   
		*pptNode = ptNode;
		return SPP_SUCCESS;		
	}	
	SppUnlockSendMutex(eConnectType);	
	return SPP_FAILURE;	
}

static SPPErrorCode_e SppSendProc(SppConnectType_e eConnectType)
{	
	SPPErrorCode_e eRet = SPP_FAILURE;
	SppMemType_t *ptSendNode=NULL;
    SppMemType_t *ptSendListCur = NULL;
	UINT8 u8Idx = 0;
	UINT8 *pu8IdxDelta = NULL;
	static UINT8 u8IdxdeltaMpu = 0;
	static UINT8 u8Idxdelta4G = 0;
    UINT8 u8Type = 0;
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
		return SPP_FAILURE;
	}
    eRet = SppSendReady();
    if (eRet == SPP_FAILURE)
    {
        return SPP_FAILURE;
    }
    if(gatSppState1[eConnectType-1].bBlock == FALSE)       
    {

    	eRet=SppGetOutSendFirstNode(eConnectType, &ptSendNode);
        
    	if(SPP_FAILURE == eRet)
    	{
    		return SPP_FAILURE;
    	}
    	eRet= SppSendNodeToDevice(eConnectType,ptSendNode);
    	if(SPP_FAILURE == eRet)
    	{
    	    SppEventSendResultCb(eConnectType, ptSendNode->u16_event, 0);  /*notify send result*/
            SppDelSendNodeByNodes(eConnectType, ptSendNode);
            SppMemFreeOneNode(ptSendNode);      
    	}
        else
        {
            eRet = SppAddAckListOneNode(eConnectType,ptSendNode);

            if (eRet == SPP_FAILURE)
            {
                return SPP_FAILURE;
            }
		    SppMemFreeOneNode(ptSendNode);		
            
            if (ptSendNode->u8Type == SPP_PACKET_TYPE_BLOCK)
            {
                gatSppState1[eConnectType - 1].bBlock = TRUE;
            }
            else
            {
                gatSppState1[eConnectType -1].bBlock = FALSE;
            }
        }
    }
    else
    {}
	
	return SPP_SUCCESS;	
}

















/******************************************************************************
*  function name | SppRecvCb
*  content       | 接收回调处理函数
*  parameter     | eConnectType 连接类型 ptRecvMsg 收到的MSG结构体指针
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppRecvCb(SppConnectType_e eConnectType,SppRecvMcuMsg_t* ptRecvMsg)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	UINT32 u32Index = 0; 	
	pfSppListener pRecvFunCb= NULL;	
    UINT16 u16Event;
	if(NULL==ptRecvMsg)
	{
		return SPP_FAILURE;
	}		
    if(ptRecvMsg->u8AllNum != ptRecvMsg->u8CurNum)
    {
		return SPP_FAILURE;
    }
	else
	{			
	}
    u16Event = ptRecvMsg->u16Cmd;
    if (u16Event >= SPP_CALLBACK_MAX_COUNT)
    {
        ApiLogPrint(_LOG_TRACE, "u16Event > err %d\n",ptRecvMsg->u16Cmd );
        return SPP_FAILURE;
    }
    pRecvFunCb = gatSppMsgProcList[u16Event].pFunCb;
    if (pRecvFunCb == NULL)
    {
        ApiLogPrint(_LOG_TRACE, "pRecvFunCb null %d\n",u16Event);
        return SPP_FAILURE;
    }    
    pRecvFunCb(ptRecvMsg->u16Cmd, (SppPackage_t*)ptRecvMsg->aucData, (UINT32)ptRecvMsg->u16DataLen);
	

#if 0
	for(u32Index=0; u32Index<sizeof(gatSppMsgProcList)/sizeof(SppMsgProc_t); u32Index++)
	{			
		if((eConnectType == gatSppMsgProcList[u32Index].eConnectType)
			&&(ptRecvMsg->u16Cmd == gatSppMsgProcList[u32Index].u16EventId))
		{
			pRecvFunCb=gatSppMsgProcList[u32Index].pFunCb;
			if(NULL!=pRecvFunCb)
			{
			    ApiLogPrint(_LOG_TRACE, "RecEvent%d\n",ptRecvMsg->u16Cmd );
				pRecvFunCb(ptRecvMsg->u16Cmd, (SppPackage_t*)ptRecvMsg->aucData, (UINT32)ptRecvMsg->u16DataLen);
			}			
		}
        if ((eConnectType == gatSppMsgRecList[u32Index].eConnectType)
			&&(ptRecvMsg->u16Cmd == gatSppMsgRecList[u32Index].u16EventId))
        {
            if(gatSppMsgRecList[u32Index].u16Len == ptRecvMsg->u16DataLen)
            {
                gatSppMsgRecList[u32Index].bFlg = TRUE;
                memcpy(gatSppMsgRecList[u32Index].pu8Buffer,ptRecvMsg->aucData,ptRecvMsg->u16DataLen);
            }
        }
	}

    if(u32Index == sizeof(gatSppMsgProcList)/sizeof(SppMsgProc_t))
    {
        ApiLogPrint(_LOG_TRACE, "LookERR %d\n",ptRecvMsg->u16Cmd );
    }
#endif

	memset(&gtAllMsg, 0,sizeof(gtAllMsg));
	return SPP_SUCCESS;
}









static void SppEventSendResultCb(SppConnectType_e eConnectType,UINT16 pu16Event[],UINT8 u8SendResult)
{
    UINT32 u32Index = 0;
    UINT8 u8_eventId = 0;
    pfSppAckListener pRecvFunCb = NULL;
    if (pu16Event == NULL)
    {
        return;
    }
    while(u8_eventId < 10)
    {
        if (pu16Event[u8_eventId] == 0)
        {
            return;
        }
        for(u32Index=0; u32Index<sizeof(gatSppSendAckProcList)/sizeof(SppMsgProc_t); u32Index++)
        {           
            if((eConnectType == gatSppSendAckProcList[u32Index].eConnectType)
                &&(pu16Event[u8_eventId] == gatSppSendAckProcList[u32Index].u16EventId))
            {
                pRecvFunCb=gatSppSendAckProcList[u32Index].pFunCb;
                if(NULL!=pRecvFunCb)
                {
                    pRecvFunCb(pu16Event[u8_eventId],u8SendResult);
                    if (u8SendResult == 0)
                    {
                        ApiLogPrint(_LOG_ENTRY, "==eventID%d STA%x\n", pu16Event[u8_eventId],u8SendResult);
                    }
                }           
            }
        }
        u8_eventId ++;
    }
}


static void SPPEventSendResulToIDAL(SppConnectType_e eConnectType,UINT16 u16Event,UINT8 u8SendResult)
{
    UINT32 u32Index = 0;
    pfSppAckListener pRecvFunCb = NULL;

    for(u32Index=0; u32Index<sizeof(gatSppSendAckProcList)/sizeof(SppMsgProc_t); u32Index++)
    {           
        if((eConnectType == gatSppSendAckProcList[u32Index].eConnectType) && (u16Event == gatSppSendAckProcList[u32Index].u16EventId))
        {
            pRecvFunCb=gatSppSendAckProcList[u32Index].pFunCb;
            if(NULL!=pRecvFunCb)
            {
                pRecvFunCb(u16Event,u8SendResult);
            }           
        }
    }
}


/******************************************************************************
*  function name | SppDataPacketProc
*  content       | 数据接收处理
*  parameter[1]  | eConnectType :
*  parameter[2]  | ptPacket :
*  notice        | 
*  return        | 
******************************************************************************/
static void SppDataPacketProc(SppConnectType_e eConnectType, SppPacketType* ptPacket)
{
    UINT8* pucBeginOld = NULL;
    UINT8* pucEnd = NULL;
    UINT8* pucBeginNew = NULL;
	SppMcuMsg_t tCurMsg;    
    SPPErrorCode_e eRet =SPP_FAILURE;
    if (ptPacket == NULL)
    {
        return;
    }
    pucBeginOld = ptPacket->au8PacketPayload;
    pucEnd = ptPacket->au8PacketPayload + SPP_BIG_PACKET_PAYLOAG_LEN_MAX;
    while(1)
    {
        pucBeginNew = SppFindMsgHead(pucBeginOld, pucEnd);
        if(NULL == pucBeginNew)
        {
            break;              
        }
        else
        {
            memset(&tCurMsg, 0,sizeof(SppMcuMsg_t)); 
            eRet = SppMsgDecode(pucBeginNew, pucEnd-pucBeginNew, &tCurMsg);
            if(SPP_SUCCESS != eRet)
            {
                pucBeginOld = pucBeginNew + 2;
            }
            else
            {
                eRet = SppMakeCompleteMsg(&tCurMsg,&gtAllMsg);
                if (eRet == SPP_SUCCESS)
                {
                    SppRecvCb(eConnectType,&gtAllMsg); 
                }
                else
                {
                    ApiLogPrint(_LOG_TRACE,"MakeMsgERR\n");
                    memset(&gtAllMsg,0,sizeof(gtAllMsg));
                }
                
                pucBeginOld = pucBeginOld + tCurMsg.u16DataLen + SPP_PACKET_LEN_MIN;
                if((pucBeginOld + SPP_PACKET_LEN_MIN) > pucEnd)
                {                        
                    break;
                }
                else
                {}                                       
            }
        }
    }           
}

/******************************************************************************
*  function name | SppAckProc
*  content       | ACK队列处理
*  parameter[1]  | eConnectType :连接类型
*  notice        | 
*  return        | 
******************************************************************************/
static void SppAckProc(SppConnectType_e eConnectType)
{
	SppMemType_t * ptTmp=NULL;	
    SppMemType_t tFree;

	ptTmp=SppGetAcklistByConnectType(eConnectType);	

	if(NULL==ptTmp)
	{
		return ;
	}
	while((ptTmp) != NULL)
	{ 	
        if (ptTmp->u8SendTimes < SPP_SEND_TIMES_MAX)
        {
            if (ptTmp->u32TimeCout>=SPP_ACK_TIMEOUT_TIME)
            {
                if (ptTmp->u8Type == SPP_PACKET_TYPE_BLOCK)
                {
                    SppSendNodeToDevice(eConnectType,ptTmp);                        
                }
                else
                {
                    SppDelAckNodeByNodes(eConnectType, ptTmp);
                    memcpy(&tFree,ptTmp,sizeof(tFree));
                    gatSppState1[eConnectType-1].u8Sn = (gatSppState1[eConnectType-1].u8Sn+1)%0xff;
                    tFree.u8Sn = gatSppState1[eConnectType-1].u8Sn;
                    SppAddSendListOneNode(eConnectType,&tFree);
                }  
                ApiLogPrint(_LOG_ENTRY, "TimeOut S:%d T:%d E:%d %d %d %d %d\n ",ptTmp->u8Sn,ptTmp->u8Type,ptTmp->u16_event[0],ptTmp->u16_event[1],ptTmp->u16_event[2],ptTmp->u16_event[3],ptTmp->u16_event[4]);
            }
            else
            {
                ptTmp->u32TimeCout ++;                    
            }
        }
        else
        {
            if (ptTmp->u32TimeCout>=SPP_ACK_TIMEOUT_TIME)
            {
#if 0
                if (ptTmp->u8Type == SPP_PACKET_TYPE_BLOCK)
                {
                    gatSppState1[eConnectType-1].bBlock = FALSE;
                }             
#endif
                gatSppState1[eConnectType-1].bBlock = FALSE;
                SppEventSendResultCb(eConnectType, ptTmp->u16_event, 0);
                SppDelAckNodeByNodes(eConnectType, ptTmp);
                SppDelSendNodeByNodes(eConnectType, ptTmp);
                ApiLogPrint(_LOG_ENTRY, "TimeOutFaluir S:%d T:%d E:%d %d %d %d %d\n ",ptTmp->u8Sn,ptTmp->u8Type,ptTmp->u16_event[0],ptTmp->u16_event[1],ptTmp->u16_event[2],ptTmp->u16_event[3],ptTmp->u16_event[4]);
            }
            else
            {
                ptTmp->u32TimeCout ++;                    
            }
        }
        ptTmp = ptTmp->next;
      
	}   
		
}

/******************************************************************************
*  function name | SppAckRecProc
*  content       | ACK处理消息
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | ptPacket :ACK消息包处理
*  notice        | 
*  return        | 
******************************************************************************/
static void SppAckRecProc(SppConnectType_e eConnectType, SppPacketType* ptPacket)
{
	SppMemType_t *ptAckCheckListCur=NULL;		
	SppMemType_t * ptTmp=NULL;	
	SppMemType_t* ptPrev = NULL;
	SppMemType_t* ptNext = NULL;
	SppMemType_t* ptFree = NULL;  
    if (ptPacket == NULL)
    {
    	return;
    }
	ptAckCheckListCur=SppGetAcklistByConnectType(eConnectType);	
    
	if(NULL==ptAckCheckListCur)
	{
		return ;
	}
	ptTmp=ptAckCheckListCur;
	while((ptTmp) != NULL)
	{ 		
		if(ptTmp->u8Sn == ptPacket->u8Sn)
        {
            if ((ptTmp->u8Type == SPP_PACKET_TYPE_BLOCK)
                  &&(ptTmp->u8SplitedNumCur == ptTmp->u8SplitedNumAll))
            {
                SppEventSendResultCb(eConnectType,ptTmp->u16_event,1);
                gatSppState1[eConnectType-1].bBlock = FALSE;
            }
            else if(ptTmp->u8Type == SPP_PACKET_TYPE_NONBLOCK)
            {
                SppEventSendResultCb(eConnectType,ptTmp->u16_event,1);                
            }
            else if((ptTmp->u8Type == SPP_PACKET_TYPE_BLOCK)
                &&(ptTmp->u8SplitedNumCur != ptTmp->u8SplitedNumAll))
            {
                gatSppState1[eConnectType-1].bBlock = FALSE;
            }
            else
            {}
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
			//ptTmp = ptNext; 
			SppMemFreeOneNode(ptFree);		
            break;
        }
		ptTmp=ptTmp->next;
	}   
	SppSetAcklistByConnectType(eConnectType,ptAckCheckListCur);
}
/******************************************************************************
*  function name | SppSendPacketAck
*  content       | 发送ACK响应
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | ptPacket :接收数据包
*  notice        | 
*  return        | 
******************************************************************************/
static void SppSendPacketAck(SppConnectType_e eConnectType,SppPacketType* ptPacket)
{
    SppPacketType tPacketAck;
    UINT8 u8Crc = 0;
    memset(&tPacketAck,0,sizeof(tPacketAck));
    if (ptPacket == NULL)
    {
        return;
    }
    tPacketAck.u8Hdle = SPP_PACKET_HEAD_DLE;
    tPacketAck.u8Hsom = SPP_PACKET_HEAD_SOM;
    tPacketAck.u8Sn = ptPacket->u8Sn;
    tPacketAck.u8Type = ptPacket->u8Type + 1;
    tPacketAck.u8Sta = (UINT8)(gatSppConnStatus[(UINT8)eConnectType - 1].eConnStatus);
    memset(tPacketAck.au8PacketPayload,0xff,SPP_BIG_PACKET_PAYLOAG_LEN_MAX);
    tPacketAck.u8Tdle = SPP_PACKET_TAIL_DLE;
    tPacketAck.u8Teof = SPP_PACKET_TAIL_EOF;
    SppGetPacketCrc(&tPacketAck,&u8Crc);
    tPacketAck.u8Crc = u8Crc;    
    SppSendPacketToDevice(eConnectType,&tPacketAck);
}



/******************************************************************************
*  function name | SppRecieveDataProc
*  content       | 接收处理函数
*  parameter     | eConnectType 连接类型 
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static void SppRecieveDataProc(SppConnectType_e eConnectType)
{
    UINT32 u32RecvLen =0;
    UINT8* pucBeginOld = NULL;
    UINT8* pucEnd = NULL;
    UINT8* pucBeginNew = NULL;
    SppPacketType tpacket;
    SPPErrorCode_e eRet =SPP_FAILURE;
    UINT8 u8PacketType = 0;
    UINT8 u8_packetSn =0;
    UINT8 i;
    memset(&tpacket,0,sizeof(tpacket));
	gu32SppRecvBufLen = SppComRecv(eConnectType,gucSppRecvBuf, SPP_BIG_PACKET_LEN_MAX);
    if ((gu32SppRecvBufLen >0)
        &&(gu32SppRecvBufLen <=sizeof(gucSppRecvBuf)))
    {
       // ApiLogPrint(_LOG_TRACE, "gu32SppRecvBufLen %d\n",gu32SppRecvBufLen );
//        for(i = 0; i<1; i++)
//        {
//            ApiLogPrint(_LOG_TRACE,"L%d %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",i
//            ,gucSppRecvBuf[i*8+0],gucSppRecvBuf[i*8+1],gucSppRecvBuf[i*8+2],gucSppRecvBuf[i*8+3]
//            ,gucSppRecvBuf[i*8+4],gucSppRecvBuf[i*8+5],gucSppRecvBuf[i*8+6],gucSppRecvBuf[i*8+7]
//            ,gucSppRecvBuf[i*8+8],gucSppRecvBuf[i*8+9],gucSppRecvBuf[i*8+10],gucSppRecvBuf[i*8+11]
//            ,gucSppRecvBuf[i*8+12],gucSppRecvBuf[i*8+13],gucSppRecvBuf[i*8+14],gucSppRecvBuf[i*8+15]);
//        }
//        for(i = 31; i<32; i++)
//        {
//            ApiLogPrint(_LOG_TRACE,"L%d %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",i
//            ,gucSppRecvBuf[i*8+0],gucSppRecvBuf[i*8+1],gucSppRecvBuf[i*8+2],gucSppRecvBuf[i*8+3]
//            ,gucSppRecvBuf[i*8+4],gucSppRecvBuf[i*8+5],gucSppRecvBuf[i*8+6],gucSppRecvBuf[i*8+7]
//            ,gucSppRecvBuf[i*8+8],gucSppRecvBuf[i*8+9],gucSppRecvBuf[i*8+10],gucSppRecvBuf[i*8+11]
//            ,gucSppRecvBuf[i*8+12],gucSppRecvBuf[i*8+13],gucSppRecvBuf[i*8+14],gucSppRecvBuf[i*8+15]
//            ,gucSppRecvBuf[i*8+16],gucSppRecvBuf[i*8+17],gucSppRecvBuf[i*8+18],gucSppRecvBuf[i*8+19]
//            ,gucSppRecvBuf[i*8+20],gucSppRecvBuf[i*8+21],gucSppRecvBuf[i*8+22],gucSppRecvBuf[i*8+23]
//            ,gucSppRecvBuf[i*8+24],gucSppRecvBuf[i*8+25],gucSppRecvBuf[i*8+26],gucSppRecvBuf[i*8+27]
//            ,gucSppRecvBuf[i*8+28],gucSppRecvBuf[i*8+29],gucSppRecvBuf[i*8+30],gucSppRecvBuf[i*8+31]);
//        }
        //ApiLogPrint(_LOG_ENTRY, "TB\n");
        eRet = SppPacketDecode(gucSppRecvBuf,SPP_BIG_PACKET_LEN_MAX , &tpacket);
        if (eRet == SPP_SUCCESS)
        {
            if (tpacket.u8Type == SPP_PACKET_TYPE_BLOCK
                ||tpacket.u8Type == SPP_PACKET_TYPE_NONBLOCK)
            {
                eRet = SppSendReady();
                if(eRet == SPP_SUCCESS)
                {
                    SppSendPacketAck(eConnectType,&tpacket);
                    SppDataPacketProc(eConnectType,&tpacket);
                }
                else
                {
                    ApiLogPrint(_LOG_ENTRY, "SendTimeOut%d\n",tpacket.u8Sn);
                }
            }
            else if(tpacket.u8Type == SPP_PACKET_TYPE_ACK
                    ||tpacket.u8Type == SPP_PACKET_TYPE_ACK_NON)
            {
                SppAckRecProc(eConnectType,&tpacket);
            }
            else if(tpacket.u8Type == SPP_PACKET_TYPE_ACK_HB)
            {                    
            }
            else if(tpacket.u8Type == SPP_PACKET_TYPE_HB)
            {

            }
            else
            {
            }
#if 0
            gatHeartSend[(UINT8)eConnectType-1].u16WaitTimes = 0;
            gatHeartSend[(UINT8)eConnectType-1].u8RecAckFlag = 1;
            gatHeartSend[(UINT8)eConnectType-1].u8SendTimes = 0;
            gatHeartSend[(UINT8)eConnectType-1].u8RecFirstAckFlg = 2;
            
            gatSppConnStatus[(UINT8)eConnectType-1].eConnStatus = SPP_STATE_RUNNING;
#else       /*增加建立连接过程*/
            SppSetRunningState((UINT8)eConnectType-1,tpacket.u8Type);
#endif

        }
        else
        {
            memset(gucSppRecvBuf, 0, sizeof(gucSppRecvBuf));
            gu32SppRecvBufLen = 0;
            ApiLogPrint(_LOG_ENTRY, "SPP:decodeErr\n");
        }        
    }
    else
    {
        memset(gucSppRecvBuf, 0, sizeof(gucSppRecvBuf));
        gu32SppRecvBufLen = 0;
    }    

}

/******************************************************************************
*  function name | SppSetConnStatus
*  content       | 设置连接状态 
*  parameter     | eConnectType 连接类型     eNewStatus连接状态 
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static void SppSetConnStatus(SppConnectType_e eConnectType, SppState_e           eNewStatus)
{
    UINT8 u8Index=0;
	for(u8Index =0; u8Index<SPP_CONNECT_MAX_COUNT; u8Index++)
    {
		if(eConnectType == gatSppConnStatus[u8Index].eConnectType)
		{
			gatSppConnStatus[u8Index].eConnStatus= eNewStatus;
            if (eNewStatus == SPP_STATE_RUNNING)
            {
                //gatHeartSend[u8Index].u16WaitTimes = 0;
                gatHeartSend[u8Index].u8RecAckFlag = 1;
                gatHeartSend[u8Index].u8SendFlg = 0;
                gatHeartSend[u8Index].u8SendTimes = 0;
                gatHeartSend[u8Index].u8RecFirstAckFlg = 2;
            }
			break;
		}
	}
}



/******************************************************************************
*  function name | SppUninitSendMutex
*  content       | 互斥反初始化 
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppUninitSendMutex(SppConnectType_e eConnectType)
{
	if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
		//return (0==pthread_mutex_destroy(&gtSppSendLockMpu))?SPP_SUCCESS:SPP_FAILURE; //del by lichenglin @20190614
		return SPP_SUCCESS;
	}	
	else if((eConnectType == SPP_CONNECT_TYPE_MCU_4G)||(eConnectType == SPP_CONNECT_TYPE_MCU_MPU))
	{
	    return SPP_SUCCESS;
	}	
	else
	{
		return SPP_FAILURE;
	}
}

/******************************************************************************
*  function name | SppLockSendMutex
*  content       | 互斥锁处理 
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppLockSendMutex(SppConnectType_e eConnectType)
{
	if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
	    //SppEnterCritical();
		return SPP_SUCCESS;
	}
	else if((eConnectType == SPP_CONNECT_TYPE_MCU_4G)||(eConnectType == SPP_CONNECT_TYPE_MCU_MPU))
	{
	    return SPP_SUCCESS;
	}	
	else
	{
		return SPP_FAILURE;
	}
}

/******************************************************************************
*  function name | SppUnlockSendMutex
*  content       | 互斥去锁处理 
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppUnlockSendMutex(SppConnectType_e eConnectType)
{
	if((eConnectType == SPP_CONNECT_TYPE_MPU)||(eConnectType == SPP_CONNECT_TYPE_4G))
	{
        //SppLeaveCritical();
		return SPP_SUCCESS;
	}	
	else if((eConnectType == SPP_CONNECT_TYPE_MCU_4G)||(eConnectType == SPP_CONNECT_TYPE_MCU_MPU))
	{
	    return SPP_SUCCESS;
	}
	else
	{
		return SPP_FAILURE;
	}
}
/******************************************************************************
*  function name | SppAddSendListOneNode
*  content       | 添加到发送队列
*  parameter     | eConnectType 连接类型
*     		     | psNodetoAdd 节点指针		 
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppAddSendListOneNode(SppConnectType_e eConnectType,SppMemType_t * ptNodetoAdd)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMemType_t *ptSendListCur=NULL;

	if(NULL==ptNodetoAdd)
	{
		return SPP_FAILURE;
	}

	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return SPP_FAILURE;
	}
	else
	{
        SppLockSendMutex(eConnectType);
		ptSendListCur=SppGetSendlistByConnectType(eConnectType);
		ptSendListCur=SppMemAddNode(ptSendListCur, ptNodetoAdd);			
		if(NULL==ptSendListCur)
		{
            SppUnlockSendMutex(eConnectType);
			return SPP_FAILURE;
		}
		else
		{		
			SppSetSendlistByConnectType(eConnectType,ptSendListCur);
		}
    	SppUnlockSendMutex(eConnectType);
	}
	return SPP_SUCCESS;

}


/******************************************************************************
*  function name | SppAddAckListOneNode
*  content       | 新增ACK节点
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | ptNodetoAdd :节点
*  notice        | 
*  return        | 
******************************************************************************/
static SPPErrorCode_e SppAddAckListOneNode(SppConnectType_e eConnectType,SppMemType_t * ptNodetoAdd)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMemType_t *ptSendListCur=NULL;

	if(NULL==ptNodetoAdd)
	{
		return SPP_FAILURE;
	}

	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return SPP_FAILURE;
	}
	else
	{
        SppLockSendMutex(eConnectType);
		ptSendListCur=SppGetAcklistByConnectType(eConnectType);
		ptSendListCur=SppMemAddNode(ptSendListCur, ptNodetoAdd);			
		if(NULL==ptSendListCur)
		{
            SppUnlockSendMutex(eConnectType);
			return SPP_FAILURE;
		}
		else
		{		
			SppSetAcklistByConnectType(eConnectType,ptSendListCur);
		}
    	SppUnlockSendMutex(eConnectType);
	}
	return SPP_SUCCESS;

}


SPPErrorCode_e SppInitForAll(VOID)
{
	gatSppConnStatus[0].eConnectType = SPP_CONNECT_TYPE_MCU_4G;
	gatSppConnStatus[1].eConnectType = SPP_CONNECT_TYPE_MCU_MPU;
	gatSppConnStatus[2].eConnectType = SPP_CONNECT_TYPE_4G;
	gatSppConnStatus[3].eConnectType = SPP_CONNECT_TYPE_MPU;
	
	//memset(gatSppMsgProcList, 0, SPP_CALLBACK_MAX_COUNT*sizeof(SppMsgProc_t));
	memset(&gtAllMsg, 0, sizeof(SppRecvMcuMsg_t));
	gptSppSendListMpu = NULL;


	//init sent list memory
	if(SPP_FAILURE==SppMemInit())
	{
		return SPP_FAILURE;
	}

	return SPP_SUCCESS;
}
/******************************************************************************
*  function name | SppInit
*  content       | 初始化
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppInit(SppConnectType_e eConnectType)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
    UINT8 u8Index = eConnectType -1;

	//init conn status
	//SppSetConnStatus(eConnectType,SPP_STATE_INIT);
    gatSppConnStatus[u8Index].eConnStatus= SPP_STATE_INIT;
    gatHeartSend[u8Index].u16WaitTimes = SPP_HEART_SEND_PERIOD;
    gatHeartSend[u8Index].u8RecAckFlag = 0;
    gatHeartSend[u8Index].u8SendFlg = 0;
    gatHeartSend[u8Index].u8SendTimes = 0;
    gatHeartSend[u8Index].u8RecFirstAckFlg = 0;
    gatSppState1[u8Index].bBlock = FALSE;
    gatSppState1[u8Index].u8Sn = 0;
	eRet = SppComOpen(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}

	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}

	if(eConnectType == SPP_CONNECT_TYPE_MCU_4G)
	{
		gptSppSendListMcu4G = NULL;
	}
	else if(eConnectType == SPP_CONNECT_TYPE_MCU_MPU)
	{
		gptSppSendListMcuMpu = NULL;
	}
    else
    {
    }



	return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppStart
*  content       | 启动
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppStart(SppConnectType_e eConnectType)
{
	//open port
	//SPPErrorCode_e eRet=SPP_FAILURE;
	//eRet=SppComInit(eConnectType);

	//send handshake
	return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppDeInit
*  content       | 反初始化
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppDeInit(SppConnectType_e eConnectType)
{
	SPPErrorCode_e eRet=SPP_FAILURE;

	//Deinit conn status
	SppSetConnStatus(eConnectType,SPP_STATE_UNKNOW);

	//creat send mutex
	SppUninitSendMutex(eConnectType);

	gptSppSendListMcu4G = NULL;
	gptSppSendListMcuMpu = NULL;
	gptSppSendListMpu = NULL;
	//delete send list
	eRet=SppMemUninit();
	if(SPP_FAILURE==eRet)
	{
	}
    return eRet;
}


/******************************************************************************
*  function name | SppStop
*  content       | 停止
*  parameter     | eConnectType 连接类型
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppStop(SppConnectType_e eConnectType)
{
	//close port
	SppSetConnStatus(eConnectType,SPP_STATE_UNKNOW); 
	return SppComClose(eConnectType);

}



/******************************************************************************
*  function name | SppSleep
*  content       | 睡眠
*  parameter[1]  | eConnectType :
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppSleep(SppConnectType_e eConnectType)
{
    UINT8 u8_indexcon = eConnectType -1;
    if ((UINT8)u8_indexcon>=SPP_CONNECT_MAX_COUNT)
    {
        return SPP_FAILURE;
    }
    gatSppConnStatus[(UINT8)u8_indexcon].eConnStatus = SPP_STATE_SLEEP;
    return SPP_SUCCESS;

}



/******************************************************************************
*  function name | SppWakeUp
*  content       | 唤醒
*  parameter[1]  | eConnectType :
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppWakeUp(SppConnectType_e eConnectType)
{
    UINT8 u8_indexcon = eConnectType -1;
    if ((UINT8)u8_indexcon>=SPP_CONNECT_MAX_COUNT)
    {
        return SPP_FAILURE;
    }
    gatSppConnStatus[(UINT8)u8_indexcon].eConnStatus = SPP_STATE_INIT;
    return SPP_SUCCESS;

}

/******************************************************************************
*  function name | SppSetListener
*  content       | 设置回调
*  parameter     | eConnectType 连接类型
*     		     | u16EventId 事件类型
*                | pSppcListener					
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppSetListener(SppConnectType_e eConnectType,UINT16 u16EventId,pfSppListener pSppcListener)
{
      /*enter critical*/
  
	//add to cb list
	SPPErrorCode_e eRet=SPP_FAILURE;
	UINT16 u16Index = 0; 

	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}
    if(NULL == pSppcListener)
    {
		return SPP_FAILURE;
    }
    
	SppLockSendMutex(eConnectType);
    if(u16EventId >= SPP_CALLBACK_MAX_COUNT)
    {
        ApiLogPrint(_LOG_TRACE, "SppSetListener u16EventId ERR \n");
        return SPP_FAILURE;
    }
    if (gatSppMsgProcList[u16EventId].pFunCb == NULL)
    {
        gatSppMsgProcList[u16EventId].eConnectType=eConnectType;
        gatSppMsgProcList[u16EventId].u16EventId=u16EventId;
        gatSppMsgProcList[u16EventId].pFunCb = pSppcListener;
    }
    else
    {
        ApiLogPrint(_LOG_TRACE, "SppSetListener already\n");
    }
    SppUnlockSendMutex(eConnectType);
    return SPP_SUCCESS;
    
#if 0
    for(u16Index=0; u16Index<sizeof(gatSppMsgProcList)/sizeof(SppMsgProc_t); u16Index++)
    {
        if(((SPP_CONNECT_TYPE_MIN == gatSppMsgProcList[u16Index].eConnectType)&&
			(0 == gatSppMsgProcList[u16Index].u16EventId))
			||((gatSppMsgProcList[u16Index].eConnectType == eConnectType)
            &&(gatSppMsgProcList[u16Index].u16EventId == u16EventId)))
        {
			gatSppMsgProcList[u16Index].eConnectType=eConnectType;
			gatSppMsgProcList[u16Index].u16EventId=u16EventId;
			gatSppMsgProcList[u16Index].pFunCb = pSppcListener;
            SppUnlockSendMutex(eConnectType);
            return SPP_SUCCESS;
        }
    }
	SppUnlockSendMutex(eConnectType);
	return SPP_FAILURE;
#endif
}


/******************************************************************************
*  function name | SppSetAckListener
*  content       | 注册发送ACK
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | u16EventId :事件ID
*  parameter[3]  | pSppcListener :注册函数
*  notice        | 
*  return        | 失败 成功
******************************************************************************/
SPPErrorCode_e SppSetAckListener(SppConnectType_e eConnectType,UINT16 u16EventId,pfSppAckListener pSppcListener)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	UINT16 u16Index = 0; 

	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}
    if(NULL == pSppcListener)
    {
		return SPP_FAILURE;
    }
	SppLockSendMutex(eConnectType);
    for(u16Index=0; u16Index<((sizeof(gatSppSendAckProcList))/(sizeof(pfSppAckListener))); u16Index++)
    {
        if(((SPP_CONNECT_TYPE_MIN == gatSppSendAckProcList[u16Index].eConnectType)&&
			(0 == gatSppSendAckProcList[u16Index].u16EventId))
			||((gatSppSendAckProcList[u16Index].eConnectType == eConnectType)
            &&(gatSppSendAckProcList[u16Index].u16EventId == u16EventId)))
        {
			gatSppSendAckProcList[u16Index].eConnectType=eConnectType;
			gatSppSendAckProcList[u16Index].u16EventId=u16EventId;
			gatSppSendAckProcList[u16Index].pFunCb = pSppcListener;
            SppUnlockSendMutex(eConnectType);
            return SPP_SUCCESS;
        }
    }
	SppUnlockSendMutex(eConnectType);
	return SPP_FAILURE;
}


/******************************************************************************
*  function name | SPPEventEncode
*  content       | 事件编码
*  parameter[1]  | u16EventId :事件id
*  parameter[2]  | pucData :数据
*  parameter[3]  | u16DataLen :数据长度
*  parameter[4]  | u8AllNum :数据包总个数
*  parameter[5]  | u8CurNum :数据包当前个数
*  parameter[6]  | outData[] :编码数据数据
*  parameter[7]  | u16outLen :编码输出数据长度
*  notice        | 
*  return        | 
******************************************************************************/
static SPPErrorCode_e SPPEventEncode(UINT16 u16EventId, UINT8 * pucData, UINT16 u16DataLen, UINT8 u8AllNum, UINT8 u8CurNum, UINT8 outData[], UINT16* u16outLen)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMcuMsg_t tMsg;
    UINT8 u8Crc = 0;

	if((NULL == pucData)||(NULL == outData))
	{
		return SPP_FAILURE;
	}
	
    
	memset(&tMsg, 0,sizeof(SppMcuMsg_t)); 		
	tMsg.u8ControlType=(UINT8)SPP_REQ;	
	tMsg.u8Priority=(UINT8)SPP_PRI_NORMAL;
	tMsg.u16Cmd=u16EventId;
	tMsg.u16DataLen=u16DataLen;
	tMsg.u8AllNum=u8AllNum;
	tMsg.u8CurNum=u8CurNum;
	memcpy(tMsg.aucData, pucData, tMsg.u16DataLen); 	
	tMsg.u8Roll=0;
	SppGetCrc(&tMsg,&u8Crc);
    tMsg.u8Crc = u8Crc;
    
	eRet = SppMsgEncode(outData,SPP_PACKET_LEN_MAX,(UINT32*)u16outLen,&tMsg);		
 
    
	return eRet;
}


/******************************************************************************
*  function name | SPPAddNewPacketToList
*  content       | 新增发送队列节点，并添加事件
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | u16EventId :事件id
*  parameter[3]  | u8_eventData[] ：事件数据
*  parameter[4]  | u16_dataLen :数据长度
*  parameter[5]  | u8AllNum :总包数
*  parameter[6]  | u8CurNum :当前包数
*  notice        | 
*  return        | 
******************************************************************************/
static SPPErrorCode_e SPPAddNewPacketToList(SppConnectType_e eConnectType, UINT16 u16EventId, UINT8 u8_eventData[], UINT16 u16_dataLen, UINT8 u8AllNum, UINT8 u8CurNum)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	SppMemType_t *ptSendListCur=NULL;    
    SppMemType_t tNodeToAdd;
    UINT8 u8Type = 0;
    if (u8_eventData == NULL)
    {
        return eRet;
    }
    memset(&tNodeToAdd,0,sizeof(tNodeToAdd));
    tNodeToAdd.u16_remainLen = SPP_BIG_PACKET_PAYLOAG_LEN_MAX;
    tNodeToAdd.u16_dataIndex = 0;
    if (tNodeToAdd.u16_remainLen < u16_dataLen)
    {
        return eRet;
    }
    if (u8AllNum >1)
    {
        u8Type = SPP_PACKET_TYPE_BLOCK;
    }
    else
    {
        u8Type = SPP_PACKET_TYPE_NONBLOCK;
    }
    tNodeToAdd.u8Type = u8Type;
    memset(tNodeToAdd.pDcpdData,SPP_PACKET_DATA_FILL,SPP_BIG_PACKET_PAYLOAG_LEN_MAX);
    memcpy(tNodeToAdd.pDcpdData,u8_eventData,u16_dataLen);
    tNodeToAdd.u16_remainLen = tNodeToAdd.u16_remainLen - u16_dataLen;
    tNodeToAdd.u16_dataIndex = tNodeToAdd.u16_dataIndex + u16_dataLen;
    gatSppState1[eConnectType-1].u8Sn = (UINT8)((gatSppState1[eConnectType-1].u8Sn + 1)%0xFF);
    tNodeToAdd.u8Sn = gatSppState1[eConnectType-1].u8Sn;
    tNodeToAdd.u16_event[0] = u16EventId;
    tNodeToAdd.u8_eventNum = 1;
    tNodeToAdd.u8SplitedNumAll = u8AllNum;
    tNodeToAdd.u8SplitedNumCur = u8CurNum;
    ptSendListCur=SppGetSendlistByConnectType(eConnectType);
    ptSendListCur=SppMemAddNode(ptSendListCur, &tNodeToAdd);      
    if (ptSendListCur == NULL)
    {
        ApiLogPrint(_LOG_ENTRY, "MemAddErr\n");
        SppDelSendNodeByNodesIncrease(eConnectType,&tNodeToAdd);
        eRet = SPP_FAILURE;
    }
    else
    {
        SppSetSendlistByConnectType(eConnectType,ptSendListCur);
        eRet = SPP_SUCCESS;
    }
    return eRet;
}


/******************************************************************************
*  function name | SPPAddEventList
*  content       | 新增事件至发送队列
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | u16EventId :事件id
*  parameter[3]  | u8_eventData[] ：事件数据
*  parameter[4]  | u16_dataLen :数据长度
*  parameter[5]  | u8AllNum :总包数
*  parameter[6]  | u8CurNum :当前包数
*  notice        | 
*  return        | 
******************************************************************************/
static SPPErrorCode_e SPPAddEventList(SppConnectType_e eConnectType,UINT16 u16EventId, UINT8 * pucData, UINT16 u16DataLen, UINT8 u8AllNum, UINT8 u8CurNum)
{
	SPPErrorCode_e eRet=SPP_SUCCESS;
	SppMemType_t *ptSendListCur=NULL;
    
    SppMemType_t *pt_tmp = NULL;

    static UINT8 aucDestBufPac[SPP_PACKET_LEN_MAX] = {0};
    UINT32 u32Destlen = 0;
    UINT8 u8Type = 0;
    UINT8 u8_eventIndex = 0;
	(void)memset(aucDestBufPac, 0xFF, sizeof(aucDestBufPac));
	eRet = SPPEventEncode(u16EventId,pucData,u16DataLen,u8AllNum,u8CurNum,aucDestBufPac,(UINT16 *)&u32Destlen);		
    
    ptSendListCur=SppGetSendlistByConnectType(eConnectType);
    pt_tmp = SppMemFindLastNode(ptSendListCur);
    
    if (u8AllNum >1)
    {
        u8Type = SPP_PACKET_TYPE_BLOCK;        
    }
    else
    {
        u8Type = SPP_PACKET_TYPE_NONBLOCK;
    }

    
    if ((pt_tmp == NULL)
        ||(pt_tmp->u8Type == SPP_PACKET_TYPE_BLOCK)
        ||(pt_tmp->u8SendTimes >0)
        ||(pt_tmp->u16_remainLen < u32Destlen)
        ||(pt_tmp->u8_eventNum >=SPP_BIG_PACKET_EVENT_NUM_MAX)
        ||(u8AllNum >1))  /*whan no packet or have block packet etc.*/
    {
        eRet = SPPAddNewPacketToList(eConnectType,u16EventId,aucDestBufPac,(UINT16)u32Destlen,u8AllNum,u8CurNum);
    }
    else
    {
        memcpy(&(pt_tmp->pDcpdData[pt_tmp->u16_dataIndex]),aucDestBufPac,u32Destlen);
        pt_tmp->u16_dataIndex = pt_tmp->u16_dataIndex + (UINT16)u32Destlen;
        pt_tmp->u16_remainLen = pt_tmp->u16_remainLen - (UINT16)u32Destlen;
        pt_tmp->u16_event[pt_tmp->u8_eventNum] = u16EventId;
        pt_tmp->u8_eventNum ++;
            
    }    
	return eRet;
    
}


/******************************************************************************
*  function name | SppSend
*  content       | 发送
*  parameter     | eConnectType 连接类型
*     		     | u16EventId 事件类型
*                | pucData 发送数据指针 u16DataLen数据长度				
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppSend(SppConnectType_e eConnectType,UINT16 u16EventId, UINT8 * pucData, UINT16 u16DataLen)
{
	//add to send list
	SPPErrorCode_e eRet=SPP_FAILURE;
	UINT8 u8SplitedNum=0;
	UINT8 u8Temp=0;
	UINT8 u8Index=0;
	UINT16 u16SplitPackLen=0;
    UINT32 u32SpaceReamain = 0;
	if((NULL == pucData)||(u16DataLen<sizeof(SppPackageHeader_t)))
	{
		return SPP_FAILURE;
	}
    if(gatSppConnStatus[eConnectType-1].eConnStatus != SPP_STATE_RUNNING)
    {
        ApiLogPrint(_LOG_ENTRY, "SPP:SppStateIsNotReady\n");
        return SPP_SUCCESS;
    }
    if (u16DataLen > SPP_MSG_MAX_LEN)
    {
        ApiLogPrint(_LOG_ENTRY, "SPP:MAXLen\n");
        SPPEventSendResulToIDAL(eConnectType,u16EventId,0);
        return SPP_SUCCESS;
    }
	u8SplitedNum=u16DataLen/SPP_BIG_PACKET_DATA_LEN_MAX;
	u8Temp=u16DataLen%SPP_BIG_PACKET_DATA_LEN_MAX;
	if(0 != u8Temp)
	{
		u8SplitedNum++;
	}
    u32SpaceReamain = SppMemGetFreeSpace();
    if (u32SpaceReamain<u8SplitedNum)
    {
        ApiLogPrint(_LOG_ENTRY, "SPP:RAMOverflower\n");
		ApiLogPrint(_LOG_ENTRY, "u16EventId is %d \n",u16EventId);
	   // printf("u16EventId is %d \n",u16EventId);
        SPPEventSendResulToIDAL(eConnectType,u16EventId,0);
        return SPP_FAILURE;
    }
    SppLockSendMutex(eConnectType);
	if(1==u8SplitedNum)
	{	    
		eRet=SPPAddEventList(eConnectType,u16EventId,pucData,u16DataLen,u8SplitedNum,1);
        if (eRet == SPP_FAILURE)
        {
            SPPEventSendResulToIDAL(eConnectType,u16EventId,0);
            ApiLogPrint(_LOG_ENTRY, "AddSendList\n");
        }
        SppUnlockSendMutex(eConnectType);
        return eRet;
	}
	else
	{
		for(u8Index=0; u8Index<u8SplitedNum; u8Index++)
	    {
			if(u8Index!=u8SplitedNum-1)
			{
				u16SplitPackLen = SPP_PACKET_DATA_LEN_MAX;
			}
			else
			{
				u16SplitPackLen = u16DataLen - u8Index*SPP_PACKET_DATA_LEN_MAX;
			}
			eRet=SPPAddEventList(eConnectType,u16EventId,pucData+u8Index*SPP_PACKET_DATA_LEN_MAX,u16SplitPackLen,u8SplitedNum,u8Index+1);
			if(SPP_FAILURE==eRet)
			{
                SPPEventSendResulToIDAL(eConnectType,u16EventId,0);
                ApiLogPrint(_LOG_ENTRY, "AddSendListBlock\n");
			}
		}	
	}
    
    SppUnlockSendMutex(eConnectType);
    return eRet;
	
}


/******************************************************************************
*  function name | SppGetStatus
*  content       | 获取连接状态
*  parameter     | eConnectType 连接类型				
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SppState_e SppGetStatus(SppConnectType_e eConnectType)
{
	INT8 i8Index=0;
	SppState_e eConnStatusResult=SPP_STATE_UNKNOW;
	
	for(i8Index=0; i8Index<SPP_CONNECT_MAX_COUNT; i8Index++)
    {
		if(eConnectType==gatSppConnStatus[i8Index].eConnectType)
		{
			eConnStatusResult=gatSppConnStatus[i8Index].eConnStatus;
			break;
		}
	}	
	return eConnStatusResult;
}

/******************************************************************************
*  function name | SppSetConnStateChangeListener
*  content       | 设置连接状态变化回调
*  parameter     | eConnectType 连接类型
*                | pSppcConnStateChangeCb					
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppSetConnStateChangeListener(SppConnectType_e eConnectType,void* pSppcConnStateChangeListener)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	UINT8 u8Index = 0; 

	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}
	
    if(NULL == pSppcConnStateChangeListener)
    {
		return SPP_FAILURE;
    }

    for(u8Index=0; u8Index<sizeof(gatSppConnMsgProcList)/sizeof(SppConnMsgProc_t); u8Index++)
    {
        if(eConnectType == gatSppConnMsgProcList[u8Index].eConnectType)
        {
            gatSppConnMsgProcList[u8Index].pFunConnCb = pSppcConnStateChangeListener;
            return SPP_SUCCESS;
        }
    }
	return SPP_FAILURE;
}


/******************************************************************************
*  function name | SppHeartManage
*  content       | heart packet management
*  parameter[1]  | eConnectType : connectType
*  notice        | 
*  return        | 
******************************************************************************/
VOID SppHeartManage(SppConnectType_e eConnectType)
{
    UINT8 u8_indexcon = eConnectType -1;
    if ((UINT8)u8_indexcon>=sizeof(gatHeartSend))
    {
        return;
    }
    if(gatHeartSend[(UINT8)u8_indexcon].u16WaitTimes<SPP_HEART_SEND_PERIOD)
	{
	    gatHeartSend[(UINT8)u8_indexcon].u16WaitTimes += TIME_STEP;
	}
	else
	{
	    /* ApiLogPrint(_LOG_ENTRY, "==statte%d\n",gatSppConnStatus[SPP_CONNECT_TYPE_MCU_4G-1].eConnStatus); */ /* by s811 */
	    gatHeartSend[(UINT8)u8_indexcon].u16WaitTimes = 0;
		if((1==gatHeartSend[(UINT8)u8_indexcon].u8SendFlg)
            &&(0==gatHeartSend[(UINT8)u8_indexcon].u8RecAckFlag)
            &&(2==gatHeartSend[(UINT8)u8_indexcon].u8RecFirstAckFlg ))  
		{
            if (gatHeartSend[(UINT8)u8_indexcon].u8SendTimes == 3)
            {
                gatSppConnStatus[(UINT8)u8_indexcon].eConnStatus = SPP_STATE_ERROR;
                gatHeartSend[(UINT8)u8_indexcon].u8SendTimes = 3;
                ApiLogPrint(_LOG_ENTRY, "-===error==\n");
            }
            else
            {
                gatSppConnStatus[(UINT8)u8_indexcon].eConnStatus = SPP_STATE_WARNING;
                gatHeartSend[(UINT8)u8_indexcon].u8SendTimes++;
                ApiLogPrint(_LOG_ENTRY, "-===warning==\n");
            }
		}
        else if(gatHeartSend[(UINT8)u8_indexcon].u8RecFirstAckFlg == 1)
        {
            gatSppConnStatus[(UINT8)u8_indexcon].eConnStatus = SPP_STATE_INIT;
            
            /* ApiLogPrint(_LOG_ENTRY, "-===init==\n"); */ /* by s811 */
        }
		else 
		{
		    gatHeartSend[(UINT8)u8_indexcon].u8SendTimes = 0;
		}
        SppSendHeart(eConnectType);        
        gatHeartSend[(UINT8)u8_indexcon].u8SendFlg = 1;
        gatHeartSend[(UINT8)u8_indexcon].u8RecAckFlag = 0;
        if (gatHeartSend[(UINT8)u8_indexcon].u8RecFirstAckFlg != 2)
        {
            gatHeartSend[(UINT8)u8_indexcon].u8RecFirstAckFlg = 1;
        }
        /* ApiLogPrint(_LOG_TRACE, "==heart===\n"); */ /* by s811 */
	}
}


/******************************************************************************
*  function name | SppMainTask
*  content       | mcu 收发处理
*  parameter     | eConnectType 连接类型		
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppMainTask(SppConnectType_e eConnectType)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	UINT8 u8Idx = 0;
    UINT8 u8_indexcon = eConnectType -1;
    if ((UINT8)u8_indexcon>=SPP_CONNECT_MAX_COUNT)
    {
        return SPP_FAILURE;
    }
	if((eConnectType != SPP_CONNECT_TYPE_MCU_4G) && (eConnectType != SPP_CONNECT_TYPE_MCU_MPU))
	{
		return SPP_FAILURE;
	}
    if(gatSppConnStatus[(UINT8)u8_indexcon].eConnStatus == SPP_STATE_SLEEP)
    {
        return SPP_SUCCESS;
    }

    SppHeartManage(eConnectType);
    for (u8Idx = 0; u8Idx < PROCESS_TIMES_ONE_CYCLE; u8Idx++)
    {
      SppRecieveDataProc(eConnectType);
      SppAckProc(eConnectType);
      SppSendProc(eConnectType);
    }
    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppSetRecBuff
*  content       | 设置事件消息接收空间，用于查询方式接收
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | u16EventID :事件id
*  parameter[3]  | pu8RecBuff :消息存放地址
*  parameter[4]  | u16Len :消息长度
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppSetRecBuff(SppConnectType_e eConnectType,UINT16 u16EventID,UINT8* pu8RecBuff,UINT16 u16Len)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	UINT16 u16Index = 0; 

	eRet=SppCheckConnectType(eConnectType);
	if(SPP_FAILURE==eRet)
	{
		return eRet;
	}
    if(NULL == pu8RecBuff)
    {
		return SPP_FAILURE;
    }
    return SPP_SUCCESS;
    
	SppLockSendMutex(eConnectType);
//    for(u16Index=0; u16Index<sizeof(gatSppMsgRecList)/sizeof(SppMsgProc_t); u16Index++)
//    {
//        if(((SPP_CONNECT_TYPE_MIN == gatSppMsgRecList[u16Index].eConnectType)&&
//			(0 == gatSppMsgRecList[u16Index].u16EventId))
//			||((gatSppMsgRecList[u16Index].eConnectType == eConnectType)
//            &&(gatSppMsgRecList[u16Index].u16EventId == u16EventID)))
//        {
//			gatSppMsgRecList[u16Index].eConnectType=eConnectType;
//			gatSppMsgRecList[u16Index].u16EventId=u16EventID;
//			gatSppMsgRecList[u16Index].u16Len = u16Len;
//            gatSppMsgRecList[u16Index].pu8Buffer = pu8RecBuff;
//            gatSppMsgRecList[u16Index].bFlg = FALSE;
//            SppUnlockSendMutex(eConnectType);
//            return SPP_SUCCESS;
//        }
//    }
	SppUnlockSendMutex(eConnectType);
	return SPP_FAILURE;
}


/******************************************************************************
*  function name | SppGetReceiveData
*  content       | 赋值消息至存放空间
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | u16EventID :事件ID
*  notice        | 
*  return        | 
******************************************************************************/
UINT8* SppGetReceiveData(SppConnectType_e eConnectType,UINT16 u16EventID)
{
	UINT16 u16Index = 0; 
//    for(u16Index=0; u16Index<sizeof(gatSppMsgRecList)/sizeof(SppMsgProc_t); u16Index++)
//    {
//        if(((eConnectType == gatSppMsgRecList[u16Index].eConnectType)&&
//			(u16EventID == gatSppMsgRecList[u16Index].u16EventId)))
//        {
//            return gatSppMsgRecList[u16Index].pu8Buffer;
//        }
//    }
    return NULL;
}


/******************************************************************************
*  function name | SppReceiveFlag
*  content       | 消息接收请求标记
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | u16EventID :事件ID
*  notice        | 
*  return        | 
******************************************************************************/
BOOL SppReceiveFlag(SppConnectType_e eConnectType,UINT16 u16EventID)
{
	UINT16 u16Index = 0; 
//    for(u16Index=0; u16Index<sizeof(gatSppMsgRecList)/sizeof(SppMsgProc_t); u16Index++)
//    {
//        if(((eConnectType == gatSppMsgRecList[u16Index].eConnectType)&&
//			(u16EventID == gatSppMsgRecList[u16Index].u16EventId)))
//        {
//            return gatSppMsgRecList[u16Index].bFlg;
//        }
//    }
    return FALSE;
}


/******************************************************************************
*  function name | SppClearRecFlg
*  content       | 清除消息接收标记
*  parameter[1]  | eConnectType :连接类型
*  parameter[2]  | u16EventID :事件ID
*  notice        | 
*  return        | 
******************************************************************************/
void SppClearRecFlg(SppConnectType_e eConnectType,UINT16 u16EventID)
{
	UINT16 u16Index = 0; 
//    for(u16Index=0; u16Index<sizeof(gatSppMsgRecList)/sizeof(SppMsgProc_t); u16Index++)
//    {
//        if(((eConnectType == gatSppMsgRecList[u16Index].eConnectType)&&
//			(u16EventID == gatSppMsgRecList[u16Index].u16EventId)))
//        {
//            gatSppMsgRecList[u16Index].bFlg = FALSE;
//        }
//    }
}



/******************************************************************************
*  function name | SppSetRunningState
*  content       | 设置running状态
*  parameter[1]  | u8ConnectIndex :连接类型索引
*  parameter[2]  | u8PacketType :接收到的packet类型
*  notice        | 
*  return        | 
******************************************************************************/
static void SppSetRunningState(UINT8 u8ConnectIndex,UINT8 u8PacketType )
{
    SppState_e eCurrentState;
    if (u8ConnectIndex >= SPP_CONNECT_MAX_COUNT)
    {
        return;
    }
    eCurrentState = gatSppConnStatus[u8ConnectIndex].eConnStatus;
    switch (u8PacketType)
    {
        case SPP_PACKET_TYPE_ACK_HB:
            if (eCurrentState != SPP_STATE_ERROR)
            {
                gatHeartSend[u8ConnectIndex].u16WaitTimes = 0;
                gatHeartSend[u8ConnectIndex].u8RecAckFlag = 1;
                gatHeartSend[u8ConnectIndex].u8SendTimes = 0;
                gatHeartSend[u8ConnectIndex].u8RecFirstAckFlg = 2;
                gatSppConnStatus[u8ConnectIndex].eConnStatus = SPP_STATE_RUNNING;
            }
            break;
        case SPP_PACKET_TYPE_ACK_NON:
        case SPP_PACKET_TYPE_ACK:
            if (eCurrentState == SPP_STATE_RUNNING
              ||eCurrentState == SPP_STATE_WARNING)
            {
                gatHeartSend[u8ConnectIndex].u16WaitTimes = 0;
                gatHeartSend[u8ConnectIndex].u8RecAckFlag = 1;
                gatHeartSend[u8ConnectIndex].u8SendTimes = 0;
                gatHeartSend[u8ConnectIndex].u8RecFirstAckFlg = 2;
                gatSppConnStatus[u8ConnectIndex].eConnStatus = SPP_STATE_RUNNING;
            }
            break;
        default:
            break;
    }
}
    


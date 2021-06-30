#include "spp_main.h"
#include "spp_memory.h"
#include "spp_code.h"
#include "spp_type.h"

/**********************************************************************/
/*                        Global variable                             */
/**********************************************************************/

UINT8 gaDiasSppMemMask[DIAS_SPP_MEM_MAX_COUNT] = {0};
DIAS_SPP_MEM_TYPE gptDiasSppMemBuf[DIAS_SPP_MEM_MAX_COUNT];


/**********************************************************************/
/*                            Functions                               */
/**********************************************************************/


/******************************************************************************
*  function name | DiasSppMemGetUsableMask
*  content       | 获取未使用的mask
*  parameter     | u32MaskIndex : 获取到的mask位置                   
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppMemGetUsableMask(UINT32* u32MaskIndex)
{
    UINT32 u32Index = 0;
    for(u32Index=0; u32Index<DIAS_SPP_MEM_MAX_COUNT; u32Index++)
    {
        if(gaDiasSppMemMask[u32Index] == DIAS_SPP_MEM_UNUSABLE)
        {
            *u32MaskIndex = u32Index;
            return DIAS_SUCCESS;
        }
    }
    return DIAS_FAILURE;
}

/******************************************************************************
*  function name | DiasSppMemSetMaskUsable
*  content       | 设置未使用的mask
*  parameter     | u32MaskIndex : 未使用的mask位置                   
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppMemSetMaskUsable(UINT32 u32MaskIndex)
{
    if(u32MaskIndex >= DIAS_SPP_MEM_MAX_COUNT)
    {
        return DIAS_FAILURE;
    }
    else
    {
        gaDiasSppMemMask[u32MaskIndex] = DIAS_SPP_MEM_USABLE;
        return DIAS_SUCCESS;
    }
}

/******************************************************************************
*  function name | DiasSppMemSetMaskUnusable
*  content       | 设置已使用的mask
*  parameter     | u32MaskIndex : 已使用的mask位置                   
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppMemSetMaskUnusable(UINT32 u32MaskIndex)
{
    if(u32MaskIndex >= DIAS_SPP_MEM_MAX_COUNT)
    {
        return DIAS_FAILURE;
    }
    else
    {
        gaDiasSppMemMask[u32MaskIndex] = DIAS_SPP_MEM_UNUSABLE;
        return DIAS_SUCCESS;
    }
}

/******************************************************************************
*  function name | DiasSppMemDoMalloc
*  content       | 申请空间
*  parameter     | u32Size : 空间大小                   
*  return        | void* 申请结果
*  notice        | 
******************************************************************************/
static void* DiasSppMemDoMalloc(UINT32 u32Size)
{
	DiasErrorCode_e eRet = DIAS_FAILURE;
    UINT32 u32MaskIndex = 0;
  
    eRet = DiasSppMemGetUsableMask(&u32MaskIndex);
    if(DIAS_SUCCESS == eRet)
    {
        DiasSppMemSetMaskUsable(u32MaskIndex);
        memset(gptDiasSppMemBuf + u32MaskIndex,0,sizeof(DIAS_SPP_MEM_TYPE));
        return (gptDiasSppMemBuf + u32MaskIndex);
    }
    else
    {      
        //return malloc(u32Size);
        return NULL;
    }
}

/******************************************************************************
*  function name | DiasSppMemDoFree
*  content       | 释放空间
*  parameter     | pcBuf : 空间地址                   
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppMemDoFree(CHAR* pcBuf)
{
    CHAR* pcBegin = (CHAR*)gptDiasSppMemBuf;
    UINT32 u32MaskIndex = 0;
    UINT32 u32len = pcBuf - pcBegin;
    if((u32len % DIAS_SPP_MEM_UINT_SIZE) != 0)
    {
		DiasDcpdLog(LOG_ERR,"DiasSppMemFree para wrong");		
        return DIAS_FAILURE;
    }
    u32MaskIndex = u32len / DIAS_SPP_MEM_UINT_SIZE;
    DiasSppMemSetMaskUnusable(u32MaskIndex);    
    return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppMemMalloc
*  content       | 申请空间
*  parameter     | u32Size : 空间大小                   
*  return        | void* 申请结果
*  notice        | 
******************************************************************************/
static void* DiasSppMemMalloc(UINT32 u32Size)
{
    if(NULL == gptDiasSppMemBuf)
    {
        return malloc(u32Size);
    }
    else
    {
        return DiasSppMemDoMalloc(u32Size);
    }
}

/******************************************************************************
*  function name | DiasSppMemFree
*  content       | 释放空间
*  parameter     | pcBuf : 空间地址                   
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
static DiasErrorCode_e DiasSppMemFree(void* pcBuf)
{
    CHAR* pcTmp = (CHAR*)pcBuf;
    CHAR* pcBegin = (CHAR*)gptDiasSppMemBuf;
    CHAR* pcEnd = (CHAR*)(gptDiasSppMemBuf+DIAS_SPP_MEM_MAX_COUNT);
    if(NULL == pcTmp)
    {
        return DIAS_FAILURE;
    }
    else if(pcTmp < pcBegin || pcTmp >= pcEnd)
    {
        free(pcTmp);
        pcTmp = NULL;
        return DIAS_SUCCESS;
    }
    else
    {
        return DiasSppMemDoFree(pcTmp);
    }
}

/******************************************************************************
*  function name | DiasSppMemInit
*  content       | spp存储初始化
*  parameter     |                   
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppMemInit(void)
{ 
    //modify
    /*
    gptDiasSppMemBuf = (DIAS_SPP_MEM_TYPE*)malloc(DIAS_SPP_MEM_BUF_SIZE); 
    if(gptDiasSppMemBuf)
    {
        return DIAS_SUCCESS;
    }
    else
    {
        return DIAS_FAILURE;
    }
    */
    //to

    UINT32 u32Index = 0;
    for(u32Index=0; u32Index<DIAS_SPP_MEM_MAX_COUNT; u32Index++)
    {
        //memset(gptDiasSppMemBuf[u32Index].pDcpdData,0,sizeof(gptDiasSppMemBuf[u32Index].pDcpdData));
//        if(gptDiasSppMemBuf[u32Index].pDcpdData != NULL)
//        {
//            free(gptDiasSppMemBuf[u32Index].pDcpdData);
//            gptDiasSppMemBuf[u32Index].pDcpdData=NULL;
//        }
    }   
    memset(gaDiasSppMemMask, 0, sizeof(gaDiasSppMemMask));
    memset(&gptDiasSppMemBuf, 0, sizeof(gptDiasSppMemBuf));
    return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppMemUninit
*  content       | spp存储反初始化
*  parameter     |                   
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppMemUninit(void)
{
    //modify
    /*
    if(gptDiasSppMemBuf)
    {
        free(gptDiasSppMemBuf);
        gptDiasSppMemBuf = NULL;
        memset(gaDiasSppMemMask, 0, sizeof(gaDiasSppMemMask));
    }
    */
    //to

    UINT32 u32Index = 0;
    for(u32Index=0; u32Index<DIAS_SPP_MEM_MAX_COUNT; u32Index++)
    {
//        if(gptDiasSppMemBuf[u32Index].pDcpdData != NULL)
//        {
//            free(gptDiasSppMemBuf[u32Index].pDcpdData);
//            gptDiasSppMemBuf[u32Index].pDcpdData=NULL;
//        }
    }   

    memset(gptDiasSppMemBuf, 0, sizeof(gptDiasSppMemBuf));
    memset(gaDiasSppMemMask, 0, sizeof(gaDiasSppMemMask));
    return DIAS_SUCCESS;
}

/******************************************************************************
*  function name | DiasSppMemAddNode
*  content       | 添加节点 
*  parameter     | ptMemList list指针 ptNodeToAdd 节点指针           
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DIAS_SPP_MEM_TYPE* DiasSppMemAddNode(DIAS_SPP_MEM_TYPE* ptMemList, DIAS_SPP_MEM_TYPE* ptNodeToAdd)
{
    DIAS_SPP_MEM_TYPE* ptList = NULL;
    DIAS_SPP_MEM_TYPE*  ptNode = NULL;

    if(ptMemList == NULL)
	{
        ptNode = (DIAS_SPP_MEM_TYPE*)DiasSppMemMalloc(DIAS_SPP_MEM_UINT_SIZE);
        if(ptNode == NULL)
		{
            DiasDcpdLog(LOG_ERR,"DiasSppMemAddNode DiasSppMemMalloc failed");
            return NULL;
        }
		
        memset(ptNode, 0,sizeof(DIAS_SPP_MEM_TYPE));		
        ptMemList = ptNode;	
		
		memcpy(ptMemList, ptNodeToAdd, sizeof(DIAS_SPP_MEM_TYPE));	       
        (ptMemList)->next = NULL;
        (ptMemList)->prev = NULL;
        DiasSppGetAbsTime(&((ptMemList)->currentTime));  	
        ptMemList->u32TimeCout = 0;
    }
	else
	{
        ptList = ptMemList;
        while((ptList->next) != NULL)
		{
            ptList = ptList->next;
        }//find last one		
        ptNode = (DIAS_SPP_MEM_TYPE*)DiasSppMemMalloc(DIAS_SPP_MEM_UINT_SIZE);
        if(ptNode == NULL)
		{
            //DiasDcpdLog(LOG_ERR,"malloc  node failed  cmdID = %d,",cmdid);            
            return NULL;
        }
        memset(ptNode, 0, sizeof(DIAS_SPP_MEM_TYPE));
		memcpy(ptNode, ptNodeToAdd, sizeof(DIAS_SPP_MEM_TYPE));
		
        ptNode->next = NULL;
        DiasSppGetAbsTime(&(ptNode->currentTime));  
        ptNode->u32TimeCout = 0;
        ptNode->prev = ptList;
        ptList->next = ptNode;
		
    }	
	return ptMemList;    
}

/******************************************************************************
*  function name | DiasSppMemFindNode
*  content       | 查找节点 
*  parameter     | ptMemList list指针 u16Cmdid 命令id      u8CmdSplitedNumCur roll值         
*  return        | DIAS_SPP_MEM_TYPE* 节点指针
*  notice        | 
******************************************************************************/
DIAS_SPP_MEM_TYPE* DiasSppMemRemoveNotHighNode(DIAS_SPP_MEM_TYPE* ptMemList)
{  
    DiasSppListNode_t* ptNode = ptMemList;
    while((ptNode) != NULL)
	{
        if(ptNode->u8Priority==DIAS_SPP_PRI_NORMAL)
		{            
            DiasDcpdLog(LOG_INFO,"DiasSppMemRemoveNotHighNode find normal node cmdid=%d ",ptNode->u16EventId);          
			return DiasSppMemDeleteNodebyCmdid(ptMemList, ptNode->u16EventId);
        }
        ptNode = ptNode->next;
    }

	ptNode = ptMemList;
	while((ptNode) != NULL)
	{
        if(ptNode->u8Priority==DIAS_SPP_PRI_MEDIUM)
		{            
            DiasDcpdLog(LOG_INFO,"DiasSppMemRemoveNotHighNode find medium node cmdid=%d ",ptNode->u16EventId);          
            return DiasSppMemDeleteNodebyCmdid(ptMemList, ptNode->u16EventId);
        }
        ptNode = ptNode->next;
    }
	
    DiasDcpdLog(LOG_INFO,"can not find normal medium node");  	
    return ptMemList;
}

/******************************************************************************
*  function name | DiasSppMemFindNode
*  content       | 查找节点 
*  parameter     | ptMemList list指针 u16Cmdid 命令id      u8CmdSplitedNumCur roll值         
*  return        | DIAS_SPP_MEM_TYPE* 节点指针
*  notice        | 
******************************************************************************/
DIAS_SPP_MEM_TYPE * DiasSppMemFindNode(DIAS_SPP_MEM_TYPE* ptMemList, UINT16 u16Cmdid, UINT8 u8CmdSplitedNumCur)
{  
    DiasSppListNode_t* ptNode = ptMemList;
    while((ptNode) != NULL)
	{
        if((u16Cmdid == ptNode->u16EventId ) && (u8CmdSplitedNumCur == ptNode->u8SplitedNumCur))
		{            
            DiasDcpdLog(LOG_INFO,"findCMD node cmdid=%d ",u16Cmdid);           
            return ptNode;
        }
        ptNode = ptNode->next;
    }//find last one
    DiasDcpdLog(LOG_INFO,"can not find cmdID = %d,",u16Cmdid);    
    return NULL;   
}

/******************************************************************************
*  function name | DiasSppMemDeleteNodebyCmdid
*  content       | 删除节点 
*  parameter     | ptMemList list指针 u16Cmdid 命令id      
*  return        | DIAS_SPP_MEM_TYPE* list 指针
*  notice        | 
******************************************************************************/
DIAS_SPP_MEM_TYPE* DiasSppMemDeleteNodebyCmdid(DIAS_SPP_MEM_TYPE* ptMemList, UINT16 u16Cmdid)
{   
	DIAS_SPP_MEM_TYPE* ptListCur=NULL;		
	DIAS_SPP_MEM_TYPE* ptTmp=NULL; 
	DIAS_SPP_MEM_TYPE* ptPrev = NULL;
	DIAS_SPP_MEM_TYPE* ptNext = NULL;
	DIAS_SPP_MEM_TYPE* ptFree = NULL;

	ptTmp = ptMemList;

	while((ptTmp) != NULL)
	{ 
		if(u16Cmdid == ptTmp->u16EventId)
		{	
			ptPrev = ptTmp->prev;
			ptNext = ptTmp->next;				 
			if(ptPrev == NULL)
			{
				// first node
				ptListCur = ptNext;
				if(ptListCur != NULL)
				{
					ptListCur ->prev = NULL;
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
			DiasSppMemFree(ptFree);   
		}
		else
		{			
			ptTmp=ptTmp->next;
		}
		continue;
	}  
	return ptListCur;    
}

/******************************************************************************
*  function name | DiasSppMemRemoveOneNode
*  content       | 删除节点 
*  parameter     | ptMemList list指针 ptNodetoRemove 待删除节点指针      
*  return        | DIAS_SPP_MEM_TYPE* list 指针
*  notice        | 
******************************************************************************/
DIAS_SPP_MEM_TYPE* DiasSppMemRemoveOneNode(DIAS_SPP_MEM_TYPE* ptMemList, DIAS_SPP_MEM_TYPE* ptNodetoRemove)
{
	DIAS_SPP_MEM_TYPE* ptListCur=NULL;		
	DIAS_SPP_MEM_TYPE* ptTmp=NULL; 
	DIAS_SPP_MEM_TYPE* ptPrev = NULL;
	DIAS_SPP_MEM_TYPE* ptNext = NULL;
	DIAS_SPP_MEM_TYPE* ptFree = NULL;

	ptTmp = ptMemList;

	while((ptTmp) != NULL)
	{ 
		if(ptTmp=ptNodetoRemove)
		{ 	
			ptPrev = ptTmp->prev;
			ptNext = ptTmp->next;				 
			if(ptPrev == NULL)
			{
				// first node
				ptListCur = ptNext;
				if(ptListCur != NULL)
				{
					ptListCur ->prev = NULL;
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
			DiasSppMemFree(ptFree);   
		}
		else
		{			
			ptTmp=ptTmp->next;
		}
		continue;
	}  
	return ptListCur;
}

/******************************************************************************
*  function name | DiasSppMemGetFirstNode
*  content       | 获取首个节点 
*  parameter     | ptMemList list指针        
*  return        | DIAS_SPP_MEM_TYPE * 节点指针
*  notice        | 
******************************************************************************/
DIAS_SPP_MEM_TYPE * DiasSppMemGetFirstNode(DIAS_SPP_MEM_TYPE* ptMemList)
{ 	
	DIAS_SPP_MEM_TYPE* ptNode = ptMemList;
	DIAS_SPP_MEM_TYPE* ptNodeNext =NULL;
		
	if(ptNode != NULL)
	{	  
		ptNodeNext = ptNode->next;
		if(ptNodeNext != NULL)
		{
		    ptNodeNext->prev = NULL;
		}
		ptMemList =ptNodeNext;
		return ptNode;	 
	}
	else
	{
		return NULL;
	}	
}
/******************************************************************************
*  function name | DiasSppMemFreeOneNode
*  content       | 释放节点 
*  parameter     | ptNode 节点指针        
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppMemFreeOneNode(DIAS_SPP_MEM_TYPE* ptNode)
{
	DiasErrorCode_e eRet=DIAS_FAILURE;
	if(ptNode != NULL)
	{
	    memset(ptNode->pDcpdData,0,sizeof(ptNode->pDcpdData));
//		if(ptNode->pDcpdData != NULL)
//		{
//			free(ptNode->pDcpdData);
//			ptNode->pDcpdData=NULL;			
//		}
		eRet=DiasSppMemFree(ptNode);
		if(DIAS_FAILURE==eRet)
		{
			DiasDcpdLog(LOG_ERR,"DiasSppMemFreeOneNode return fail");
		}
		return eRet;
	}
	else
	{
		DiasDcpdLog(LOG_ERR,"DiasSppMemFreeOneNode ptNode is NULL");
		return DIAS_FAILURE;
	}	
}
/******************************************************************************
*  function name | DiasSppMemGetListNum
*  content       | 获取list中节点个数
*  parameter     | ptMemList list指针         
*  return        | UINT32 个数
*  notice        | 
******************************************************************************/
UINT32 DiasSppMemGetListNum(DIAS_SPP_MEM_TYPE* ptMemList)
{
	DIAS_SPP_MEM_TYPE* ptNode = ptMemList;
	UINT32 u32Num=0;
		
	if(NULL==ptNode)
	{
		return 0;
	}

	while((ptNode) != NULL)
	{
		u32Num++;
        ptNode = ptNode->next;
    }
	return u32Num;	
}





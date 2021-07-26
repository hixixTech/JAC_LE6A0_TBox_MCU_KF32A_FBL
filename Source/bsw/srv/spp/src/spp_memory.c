#include "spp_main.h"
#include "spp_memory.h"
#include "spp_code.h"
#include "spp_type.h"

/**********************************************************************/
/*                        Global variable                             */
/**********************************************************************/

UINT8 gau8SppMemMask[SPP_MEM_MAX_COUNT] = {0};
SppMemType_t gptSppMemBuf[SPP_MEM_MAX_COUNT];


/**********************************************************************/
/*                            Functions                               */
/**********************************************************************/


/******************************************************************************
*  function name | SppMemGetUsableMask
*  content       | ��ȡδʹ�õ�mask
*  parameter     | pu32MaskIndex : ��ȡ����maskλ��
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppMemGetUsableMask(UINT32* pu32MaskIndex)
{
    UINT32 u32Index = 0;
    for(u32Index=0; u32Index<SPP_MEM_MAX_COUNT; u32Index++)
    {
        if(gau8SppMemMask[u32Index] == SPP_MEM_UNUSABLE)
        {
            *pu32MaskIndex = u32Index;
            return SPP_SUCCESS;
        }
    }
    return SPP_FAILURE;
}


/******************************************************************************
*  function name | SppMemSetMaskUsable
*  content       | ����δʹ�õ�mask
*  parameter     | u32MaskIndex : δʹ�õ�maskλ��
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppMemSetMaskUsable(UINT32 u32MaskIndex)
{
    if(u32MaskIndex >= SPP_MEM_MAX_COUNT)
    {
        return SPP_FAILURE;
    }
    else
    {
        gau8SppMemMask[u32MaskIndex] = SPP_MEM_USABLE;
        return SPP_SUCCESS;
    }
}


/******************************************************************************
*  function name | SppMemSetMaskUnusable
*  content       | ������ʹ�õ�mask
*  parameter     | u32MaskIndex : ��ʹ�õ�maskλ��
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppMemSetMaskUnusable(UINT32 u32MaskIndex)
{
    if(u32MaskIndex >= SPP_MEM_MAX_COUNT)
    {
        return SPP_FAILURE;
    }
    else
    {
        gau8SppMemMask[u32MaskIndex] = SPP_MEM_UNUSABLE;
        return SPP_SUCCESS;
    }
}


/******************************************************************************
*  function name | SppMemDoMalloc
*  content       | ����ռ�
*  parameter     | u32Size : �ռ��С
*  return        | void* ������
*  notice        | 
******************************************************************************/
static void* SppMemDoMalloc(UINT32 u32Size)
{
	SPPErrorCode_e eRet = SPP_FAILURE;
    UINT32 u32MaskIndex = 0;
  
    eRet = SppMemGetUsableMask(&u32MaskIndex);
    if(SPP_SUCCESS == eRet)
    {
        SppMemSetMaskUsable(u32MaskIndex);
        memset(gptSppMemBuf + u32MaskIndex,0,sizeof(SppMemType_t));
        return (gptSppMemBuf + u32MaskIndex);
    }
    else
    {      
        //return malloc(u32Size);
        return NULL;
    }
}


/******************************************************************************
*  function name | SppMemDoFree
*  content       | �ͷſռ�
*  parameter     | pcBuf : �ռ��ַ
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppMemDoFree(CHAR* pcBuf)
{
    CHAR* pcBegin = (CHAR*)gptSppMemBuf;
    UINT32 u32MaskIndex = 0;
    UINT32 u32len = pcBuf - pcBegin;
    if((u32len % SPP_MEM_UINT_SIZE) != 0)
    {
        return SPP_FAILURE;
    }
    u32MaskIndex = u32len / SPP_MEM_UINT_SIZE;
    SppMemSetMaskUnusable(u32MaskIndex);    
    return SPP_SUCCESS;
}

/******************************************************************************
*  function name | SppMemMalloc
*  content       | ����ռ�
*  parameter     | u32Size : �ռ��С
*  return        | void* ������
*  notice        | 
******************************************************************************/
static void* SppMemMalloc(UINT32 u32Size)
{
    if(NULL == gptSppMemBuf)
    {
        return malloc(u32Size);
    }
    else
    {
        return SppMemDoMalloc(u32Size);
    }
}

/******************************************************************************
*  function name | SppMemFree
*  content       | �ͷſռ�
*  parameter     | pcBuf : �ռ��ַ
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
static SPPErrorCode_e SppMemFree(void* pcBuf)
{
    CHAR* pcTmp = (CHAR*)pcBuf;
    CHAR* pcBegin = (CHAR*)gptSppMemBuf;
    CHAR* pcEnd = (CHAR*)(gptSppMemBuf+SPP_MEM_MAX_COUNT);
    if(NULL == pcTmp)
    {
        return SPP_FAILURE;
    }
    else if(pcTmp < pcBegin || pcTmp >= pcEnd)
    {
        free(pcTmp);
        pcTmp = NULL;
        return SPP_SUCCESS;
    }
    else
    {
        return SppMemDoFree(pcTmp);
    }
}

/******************************************************************************
*  function name | SppMemInit
*  content       | spp�洢��ʼ��
*  parameter     |                   
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppMemInit(void)
{ 
    memset(gau8SppMemMask, 0, sizeof(gau8SppMemMask));
    memset(&gptSppMemBuf, 0, sizeof(gptSppMemBuf));
    return SPP_SUCCESS;
}

/******************************************************************************
*  function name | SppMemUninit
*  content       | spp�洢����ʼ��
*  parameter     |                   
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppMemUninit(void)
{
    memset(gptSppMemBuf, 0, sizeof(gptSppMemBuf));
    memset(gau8SppMemMask, 0, sizeof(gau8SppMemMask));
    return SPP_SUCCESS;
}


SppMemType_t* SppMemFindLastNode(SppMemType_t* ptMemList)
{
    SppMemType_t* ptList = NULL;
    SppMemType_t*  ptNode = NULL;
    UINT32 u32_timeout = 100;

    if (ptMemList == NULL)
    {
        return NULL;
    }
    else
    {
        ptList = ptMemList;
        while(((ptList->next) != NULL) && (u32_timeout != 0))
        {
            ptList = ptList->next;
            u32_timeout--;
        }//find last one  
        return ptList;
    }
}

/******************************************************************************
*  function name | SppMemAddNode
*  content       | ��ӽڵ�
*  parameter     | ptMemList listָ�� ptNodeToAdd �ڵ�ָ��
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SppMemType_t* SppMemAddNode(SppMemType_t* ptMemList, SppMemType_t* ptNodeToAdd)
{
    SppMemType_t* ptList = NULL;
    SppMemType_t*  ptNode = NULL;
    UINT32 u32_timeout = 100;

    if(ptMemList == NULL)
	{
        ptNode = (SppMemType_t*)SppMemMalloc(SPP_MEM_UINT_SIZE);
        if(ptNode == NULL)
		{
            return NULL;
        }
		
        memset(ptNode, 0,sizeof(SppMemType_t));		
        ptMemList = ptNode;	
		
		memcpy(ptMemList, ptNodeToAdd, sizeof(SppMemType_t));	       
        (ptMemList)->next = NULL;
        (ptMemList)->prev = NULL;
        ptMemList->u32TimeCout = 0;
    }
	else
	{
        ptList = ptMemList;
        while(((ptList->next) != NULL) && (u32_timeout != 0))
		{
            ptList = ptList->next;
            u32_timeout--;
        }//find last one		
        ptNode = (SppMemType_t*)SppMemMalloc(SPP_MEM_UINT_SIZE);
        if(ptNode == NULL)
		{
            return NULL;
        }
        memset(ptNode, 0, sizeof(SppMemType_t));
		memcpy(ptNode, ptNodeToAdd, sizeof(SppMemType_t));
		
        ptNode->next = NULL;
        ptNode->u32TimeCout = 0;
        ptNode->prev = ptList;
        ptList->next = ptNode;
		
    }	
	return ptMemList;    
}


/******************************************************************************
*  function name | SppMemDeleteNodebySN
*  content       | ���SN��ɾ��ڵ�
*  parameter[1]  | ptMemList :����
*  parameter[2]  | u8Sn :sn
*  notice        | 
*  return        | 
******************************************************************************/
SppMemType_t* SppMemDeleteNodebySN(SppMemType_t* ptMemList, UINT8 u8Sn)
{   
	SppMemType_t* ptListCur=NULL;		
	SppMemType_t* ptTmp=NULL; 
	SppMemType_t* ptPrev = NULL;
	SppMemType_t* ptNext = NULL;
	SppMemType_t* ptFree = NULL;
    UINT32 u32_timeout = 100;

	ptTmp = ptMemList;
    ptListCur = ptMemList;

	while(((ptTmp) != NULL) && (u32_timeout != 0))
	{ 
        u32_timeout--;
		if(u8Sn == ptTmp->u8Sn)
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
			SppMemFree(ptFree);   
            break;
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
*  function name | SppMemRemoveOneNode
*  content       | ɾ��ڵ�
*  parameter     | ptMemList listָ�� ptNodetoRemove ��ɾ��ڵ�ָ��
*  return        | SppMemType_t* list ָ��
*  notice        | 
******************************************************************************/
SppMemType_t* SppMemRemoveOneNode(SppMemType_t* ptMemList, SppMemType_t* ptNodetoRemove)
{
	SppMemType_t* ptListCur=NULL;		
	SppMemType_t* ptTmp=NULL; 
	SppMemType_t* ptPrev = NULL;
	SppMemType_t* ptNext = NULL;
	SppMemType_t* ptFree = NULL;
    UINT32 u32_timeout = 100;

	ptTmp = ptMemList;

	while(((ptTmp) != NULL) && (u32_timeout != 0))
	{ 
        u32_timeout--;
		if(ptTmp==ptNodetoRemove)
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
			SppMemFree(ptFree);   
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
*  function name | SppMemGetFirstNode
*  content       | ��ȡ�׸��ڵ�
*  parameter     | ptMemList listָ��
*  return        | SppMemType_t * �ڵ�ָ��
*  notice        | 
******************************************************************************/
SppMemType_t * SppMemGetFirstNode(SppMemType_t* ptMemList)
{ 	
	SppMemType_t* ptNode = ptMemList;
	SppMemType_t* ptNodeNext =NULL;
		
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
*  function name | SppMemFreeOneNode
*  content       | �ͷŽڵ�
*  parameter     | ptNode �ڵ�ָ��
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppMemFreeOneNode(SppMemType_t* ptNode)
{
	SPPErrorCode_e eRet=SPP_FAILURE;
	if(ptNode != NULL)
	{
	    memset(ptNode->pDcpdData,0,sizeof(ptNode->pDcpdData));
		eRet=SppMemFree(ptNode);
		if(SPP_FAILURE==eRet)
		{
		}
		return eRet;
	}
	else
	{
		return SPP_FAILURE;
	}	
}


/******************************************************************************
*  function name | SppMemGetListNum
*  content       | ��ȡlist�нڵ����
*  parameter     | ptMemList listָ��
*  return        | UINT32 ����
*  notice        | 
******************************************************************************/
UINT32 SppMemGetListNum(SppMemType_t* ptMemList)
{
	SppMemType_t* ptNode = ptMemList;
	UINT32 u32Num=0;
    UINT32 u32_timeout = 100;
		
	if(NULL==ptNode)
	{
		return 0;
	}

	while(((ptNode) != NULL) && (u32_timeout != 0))
	{
        u32_timeout--;
		u32Num++;
        ptNode = ptNode->next;
    }
	return u32Num;	
}


/******************************************************************************
*  function name | SppMemGetFreeSpace
*  content       | ��ȡ�ڴ�ʣ��ռ�
*  return        | 
******************************************************************************/
UINT32 SppMemGetFreeSpace(void)
{
    UINT32 u32Index = 0;
    UINT32 u32Ret = 0;
    
    for(u32Index=0; u32Index<SPP_MEM_MAX_COUNT; u32Index++)
    {
        if(gau8SppMemMask[u32Index] == SPP_MEM_UNUSABLE)
        {
            u32Ret++;
        }
    }
    return u32Ret;
}




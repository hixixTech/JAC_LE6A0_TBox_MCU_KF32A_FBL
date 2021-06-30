/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    dl_dataid.c                                                 */
/* Description:  Data Identifier handling implementation                     */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠        基线创建                              */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/

#include "dl_dataid.h"
#include "tl_engine.h"

#include "dl_engine.h"
#include "dl_service.h"
#include "bl_mem.h"
#include "bl_app.h" /* for is_programming_started() */
// #include "mcu_drv_dflash.h"
#include "mem_ee.h"
#include "ll_target.h"


/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define WDBI_MSG_MIN_LENGTH     (3u)
#define RDBI_MSG_MIN_LENGTH     (2u)
#define SW_VALID_FLAG           (1u)
#define INVALID_DID_INDEX       (0xFFu) /* currenlty 1-254 DID supported */

#define LONG_WDBI


/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/
Id_t tCsrId =
{
/*  size     id     */
    0,  (DidProp_t*)0
};


static Id_t* ptIdType[2] =
{
    &tStdIdStr,
    &tCsrId
};


static DidProp_t* ptWrCurrentDidProp;
static UINT8* pu8WdbiData;


/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/
static UINT8 DlBinarySearchOnIDS(Id_t* ptId, UINT16 u16id);


/******************************************************************************
*  function name | DlBinarySearchOnIDS
*  content       | It searches the given ID in the ordered id array and gives back the index (up to 127) of it.
*  parameter     | id_str:存储id号的结构体 id:id号
*  return        | uint8:返回id号
*  notice        | 无
******************************************************************************/
static UINT8 DlBinarySearchOnIDS(Id_t* ptId, UINT16 u16id)
{
    UINT8 u8Low;
    UINT8 u8High;
    UINT8 u8Ret = INVALID_DID_INDEX;

    if((0u == ptId->u8Size) || (INVALID_DID_INDEX == ptId->u8Size)) /* 1-254 DID supported */
    {
        return u8Ret;
    }
    /* init low boundary to first index */
    u8Low = 0;
    /* init high boundary last index */
    u8High = ptId->u8Size-1;

    /*  */
    while(u8Low <= u8High)
    {
		UINT8 u8Mid;
		UINT16 u16MidDid;
      /* find the middle index */
		u8Mid = (u8Low+u8High)>>1;
      /* get the did of mix index */
		u16MidDid = ptId->ptId[u8Mid].u16Did;

      /* check id */
		if(u16id < u16MidDid)
      {
			if (u8Mid==0)
          {
               break;
          }
          else
          {
               /* set the high boundary below mid */
			    u8High = u8Mid-1;
          }
      }
		else if(u16id > u16MidDid)
      {
          /* set the low boundary above mid */
		    u8Low = u8Mid+1;
      }
      else
      {
		    u8Ret = u8Mid; /* found */
          /*PRQA S 771 1*/ /* break is needed*/
          break;
      }
    }
    /* boundaries reached each other without result */
    return u8Ret;
}


/*---------------------------------------------------------------------------*/
/* 外部函数声明                                                              */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | ApiDlRdbyid
*  content       | It is the main command handler to handle ReadDataByIdentifier.
*  parameter     | data:数据 length:长度
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlRdbyid(const UINT8 *pu8Data, UINT16 u16Length)
{
    UINT8 i;
    UINT8 u8DidReqCnt;      /* number of requested DIDs */
    UINT8 u8DidCnt;          /* number of processed DIDs */
    UINT8 u8DidOkCnt = 0u;  /* number of valid DIDs */
    UINT8 u8DataIdx = 0u;    /* positive response data array index */
    Id_t* ptCurrentIdType;
    UINT8 u8CurrentIdNo;
    UINT8* pu8Response;
	INT32 s32Ret;

    /* abort all operations and do actions or return without action */
    if (ApiDlAbortPendingOperations(ALL) == false)
    {
        return;
    }
    /* check minimum length - at least id and one byte needs */
    if ((u16Length < RDBI_MSG_MIN_LENGTH) || (1u == (u16Length & 1u)))
    {
        /* too short or odd message length */
        (void)ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);
        return;
    }
    /* check maximum number of requested DIDs: 1 byte (0x22) and 2 byte per DIDs */
    if ( u16Length > (MAX_DID_NUM_RDBI * 2) )
    {
        (void)ApiDlSendNegativeResponse((UINT16)requestOutOfRange);
        return;
    }

    /* 1 byte is the request, DIDs are 2 byte long */
    u8DidReqCnt = (UINT8)((u16Length) >> 1);

    for (u8DidCnt = 0u; u8DidCnt < u8DidReqCnt; u8DidCnt++)
    {
        /* step over STD and CSR ID structs */
        for(i=0u; i < 2u; i++)
        {
            ptCurrentIdType = ptIdType[i];
            /* search id in current id type struct */
            /* PRQA S 0489 1 */  /* Suppress 'Increment or decrement operation performed on pointer' message */
            u8CurrentIdNo = DlBinarySearchOnIDS(ptCurrentIdType, GET_UINT16(&pu8Data[0u+(u8DidCnt<<1)]));
            if (INVALID_DID_INDEX != u8CurrentIdNo)
            {
                /* we found the fruit */
                break;
            }
        }

        /* handle the did */
        if (INVALID_DID_INDEX == u8CurrentIdNo)
        {
            /* invalid DID */
        }
        else
        {
            DidProp_t* ptCurrentDidProp;

            /* get the pointer of property of current id */
            ptCurrentDidProp = &ptCurrentIdType->ptId[u8CurrentIdNo];

            if (ptCurrentDidProp->bReadability == false)
            {
                /* not readable DID */
            }
            else
            {
                /* read up the whole id in one shot */
                pu8Response = ApiDlGetResponseBuffer();

				if(ptCurrentDidProp->eMemtype == INT_EEPROM)
				{
					if (0xF186u == ptCurrentDidProp->u16Did)
			        {
			            /* Active Diagnostic Session Data Identifier RFQ_JAC_045 */
			            if (false == tSemaphores.bProgrammingSession)
			            {
			                pu8Response[2] = 1u; /* default session */
			            }
			            else
			            {
			                pu8Response[2] = 2u; /* programming session */
			            }
			        }
			        else if (0xF1FCu == ptCurrentDidProp->u16Did)
			        {
			            /* Software Valid Flag (SVF) - calculated from (SCS) */
						s32Ret = ApiNvramReadSyncInd(ptCurrentDidProp->u32Address, ptCurrentDidProp->u8Size, 3, &pu8Response[2+u8DataIdx]);
			        }
			        else if(0xF1FBu == ptCurrentDidProp->u16Did)
			        {
						s32Ret = ApiNvramReadSyncInd(ptCurrentDidProp->u32Address, ptCurrentDidProp->u8Size, 2, &pu8Response[2+u8DataIdx]);
			        }
					else if(0xF1FAu == ptCurrentDidProp->u16Did)
					{
						s32Ret = ApiNvramReadSyncInd(ptCurrentDidProp->u32Address, ptCurrentDidProp->u8Size, 1, &pu8Response[2+u8DataIdx]);
					}
					else if(0xF012u == ptCurrentDidProp->u16Did)
					{
						s32Ret = ApiNvramReadSyncInd(ptCurrentDidProp->u32Address, ptCurrentDidProp->u8Size, 4, &pu8Response[2+u8DataIdx]);
					}
					else
					{
							s32Ret = ApiNvramReadSyncInd(ptCurrentDidProp->u32Address,ptCurrentDidProp->u8Size,0,&pu8Response[2+u8DataIdx]);
					}
				}
				else
				{
						ApiLlCopyBuffer((UINT8 *)&pu8Response[2+u8DataIdx],(UINT8 *)ptCurrentDidProp->u32Address,ptCurrentDidProp->u8Size); /*读取app和fbl兼容性*/
				}
				
                    if(ERROR== s32Ret)
                {
                    /* handle read error */
                        (void)ApiDlSendNegativeResponse(specificCauseCode6|(UINT16)conditionsNotCorrect);
                    return;
                }

                /* positive response */
                /* init the rest of the positive response: current 2 byte DID */
                pu8Response[0+u8DataIdx] = pu8Data[0+(u8DidCnt<<1)];
                pu8Response[1+u8DataIdx] = pu8Data[1+(u8DidCnt<<1)];

                /* A readable valid DID is processed */
                u8DidOkCnt++;

                u8DataIdx += ptCurrentDidProp->u8Size + 2u; /* data and 2 byte ID */
            }
		}
    }
    if (0u != u8DidOkCnt)
    {
        /* pr length = resp_sid(1 byte) + DID(2 bytes) + ID_bytes */
        /* PRQA S 3353 1 */  /* Variable 'response' is possibly unset at this point. */
        (void)ApiDlSendPositiveResponse(pu8Response, u8DataIdx);
    }
    else
    {
        /* no valid DID */
        (void)ApiDlSendNegativeResponse((UINT16)requestOutOfRange);
    }
}


/******************************************************************************
*  function name | ApiDlWdbyid
*  content       | It is the main command handler to handle WriteDataByIdentifier.
*  parameter     | data:数据 length:长度
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlWdbyid(UINT8 *pu8Data, UINT16 u16Length)
{
    UINT8 i;
    UINT8 u8CurrentIdNo;
    static UINT8 u8FlagOnceWriteDidAfterProg = 0u;
    Id_t* ptCurrentIdType;

    /* abort all operations and do actions or return without action */
    if (ApiDlAbortPendingOperations(ALL) == false)
    {
        return;
    }

    /* check minimum length - at least id and one byte needs*/
    if (u16Length < WDBI_MSG_MIN_LENGTH)
    {
        (void)ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);
        return;
    }

    /* step over STD and CSR ID structs */
    for(i=0;i<2;i++)
    {
        ptCurrentIdType = ptIdType[i];
        /* search id in current id type struct */
        /* PRQA S 0489 1 */  /* Suppress 'Increment or decrement operation performed on pointer' message */
        u8CurrentIdNo = DlBinarySearchOnIDS(ptCurrentIdType, GET_UINT16(&pu8Data[0]));  /*二分法查DID*/
        if (INVALID_DID_INDEX != u8CurrentIdNo)
        {
            /* we found the fruit */
            break;
        }
    }

    /* handle the did */
    if (INVALID_DID_INDEX == u8CurrentIdNo)
    {
        /* negative response */
        (void)ApiDlSendNegativeResponse((UINT16)requestOutOfRange);
    }
    else
    {
        WritabilityType_e eCurrentWritability;

        /* get the pointer of property of current id */
        ptWrCurrentDidProp = &ptCurrentIdType->ptId[u8CurrentIdNo];

        /* check length */
        if ((u16Length-2u) != ptWrCurrentDidProp->u8Size)
        {
            (void)ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);
            return;
        }

        /*PRQA S 1482 1*/ /*Non-constant expression cast to enum type*/
        eCurrentWritability = (WritabilityType_e)(writability_mask & ptWrCurrentDidProp->u8Writability);

        if (read_only == eCurrentWritability)
        {
            (void)ApiDlSendNegativeResponse((UINT16)requestOutOfRange);
        }
        else if (ext_sec_level == eCurrentWritability)
        {
            /* DID is only writeable in extended security level */
            (void)ApiDlSendNegativeResponse(specificCauseCode4|(UINT16)requestOutOfRange);
        }
        else if ( write_once_after_prog == eCurrentWritability)
        {
            /* write-once after each re-programming */
            if (false == ApiIsProgrammingStarted())
            {
                /* Programming not started yet */
                (void)ApiDlSendNegativeResponse(specificCauseCode9|(UINT16)conditionsNotCorrect);
            }
            else /* programming started */
            {
                UINT8 u8Mask = 1u << (ptWrCurrentDidProp->u8Writability>>4);  /* higher 4 bit shows mask */

                if (0u != (u8Mask & u8FlagOnceWriteDidAfterProg))
                {
                    /* Already written */
                    (void)ApiDlSendNegativeResponse(specificCauseCode2|(UINT16)conditionsNotCorrect);
                }
                else
                {
                    u8FlagOnceWriteDidAfterProg |= u8Mask;

                    /* init bg action */
                    tSemaphores.bDoWdbyid = true;
                    /* init bg action params */
                    pu8WdbiData = pu8Data;
#ifdef LONG_WDBI
                    /* send wait before bg_task, bg_task will not be called while wait does not sent out */
                    (void)ApiDlSendNegativeResponse((UINT16)responsePending);
#endif
                }
            }
        }
        else /* BL can write it at least once */
        {
            /* init bg action */
            tSemaphores.bDoWdbyid = true;
            /* init bg action params */
            pu8WdbiData = pu8Data;
#ifdef LONG_WDBI
            /* send wait before bg_task, bg_task will not be called while wait does not sent out */
            (void)ApiDlSendNegativeResponse((UINT16)responsePending);
#endif
        }
    }
}


/******************************************************************************
*  function name | ApiDlWdbyidBg
*  content       | It is the background task of WriteDataByIdentifier handler
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlWdbyidBg(void)
{
    UINT8 *pu8Response;
	INT32 s32Ret;

    /* Disable further background operation write action () of this service should be unic */
    tSemaphores.bDoWdbyid = false;

    s32Ret = ApiNvramWritAsyncInd(ptWrCurrentDidProp->u32Address,ptWrCurrentDidProp->u8Size,&pu8WdbiData[2]);

    if( OK != s32Ret )
    {
        (void)ApiDlSendNegativeResponse((UINT16)generalProgrammingFailure);
        return;
    }
	else
	{
        /* Send positive response */
        pu8Response = ApiDlGetResponseBuffer();
        pu8Response[0] = pu8WdbiData[0];
        pu8Response[1] = pu8WdbiData[1];
        (void)ApiDlSendPositiveResponse(pu8Response,2);
	}
}


/******************************************************************************
*  function name | ApiDlWdbyidAbort
*  content       | It is abort function of WriteDataByIdentifier background task. 
*                  Its purpose is to abort and initiate the background memory activities.
*  parameter     | void
*  return        | void
*  notice        | Note that background task is handled as atomic action of activities so it can not be interrupted and aborted in diagnostic level.
******************************************************************************/
bool ApiDlWdbyidAbort(void)
{
    return true;
}



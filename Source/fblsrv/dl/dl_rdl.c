/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    dl_rdl.c                                                    */
/* Description:  Implementation of the download related services             */
/*               like RequestDownload and TransferData                       */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** �޸���ʷ *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     �»�        ���ߴ���                              */
/* 2019-10-24   V11.01.01.01     �»�        34��������ʱ��������ˢ��ģʽ                              */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* ͷ�ļ�                                                                    */
/*---------------------------------------------------------------------------*/

#include "dl_engine.h"
#include "dl_config_pc.h"
#include "dl_service.h"

#include "bl_app.h"
#include "bl_mem.h"
#include "bl_timer.h"

#include "bl_config_pc.h"
// #include "mcu_drv_wdg.h"
// #include "mcu_drv_dflash.h"
#include "mem_ee.h"
#include "ll_target.h"
// #include "mcu_drv_flash.h"
// #include "delay.h"

/*---------------------------------------------------------------------------*/
/* ���Ͷ���                                                                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    Validate = 0, 
	Verify, 
	Erase, 
	SucFinished
} AutEraseStatus_e;

typedef struct
{
    UINT32 u32StartAddress;
    UINT32 u32EndAddress;
    UINT32 u32WorkAddress;
    UINT32 u32AutErAddress;
    UINT32 u32AutErSize;
    UINT32 u32CurrAutErAddress;
    UINT32 u32CurrAutErSize;
    AutEraseStatus_e eAutEraseStat;
    UINT32 u32PreWorkAddress;
    UINT32 u32Length;
    UINT16 u16TdLength; /*the amount of bytes that need to be programmed in the actual TD */
    UINT8 *pu8TdMessage;/*pointer to the data in the message buffer*/
    UINT8 u8SequenceCounter;
    UINT8 u8PrevSequenceCounter;
    UINT16 u16Inptr;
    UINT16 u16Outptr;
    RoutineStatus_e eTdFlowResult;
    MemType_e eDlMemtype;
    UINT8 u8NegativeResponseCode;
    bool bNegativeResponseRequired;
} Udl_t;

/*---------------------------------------------------------------------------*/
/* �궨��                                                                    */
/*---------------------------------------------------------------------------*/
#define FIRSTFRAME_LENGTH       ((UINT8)6u)
#define SINGLEFRAME_LENGTH      ((UINT8)7u)
#define CONSECUTIVEFRAME_LENGTH ((UINT8)7u)

#define TD_MSG_MIN_LENGTH       (1u)

#ifndef SEND_WAIT_BEFORE_ERASE
/* This is a target specific macro.
   a. Normal case (any erase operation is shorter than 50ms)it is an empty macro
   b. If any erase operation takes longer than 50ms it can be replaced with
      a "(void)send_negative_response((uint16)responsePending)" call unconditionally
   c. If erase address or size determines the erase time of 1st chunk, target
      specific code can decide to send a wait or not
*/
#define SEND_WAIT_BEFORE_ERASE(rc_address,rc_size) /* do nothing */
#endif /* SEND_WAIT_BEFORE_ERASE */

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/
static bool bTdIgnore = false;
static bool bTdAborted = false;
static Udl_t tDl;

/*---------------------------------------------------------------------------*/
/* �ⲿ���ձ���                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �ڲ���������                                                              */
/*---------------------------------------------------------------------------*/



/******************************************************************************
*  function name | ApiDlRequestdl
*  content       | It is the main command handler to handle RequestDownload service.
*  parameter     | data:����
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlRequestdl(const UINT8 *pu8Data)
{
    MemType_e eMemoryType;

    /* reset download state, it will only enabled after a successful RD */
    tSemaphores.bDownload = false;

    if (false != tSemaphores.bTdInProgress)               /*�����������*/
    {
        /* RequestDownload cannot be served if a transfer is ongoing: NRC */
            /* Error occured, abort operations */
        (void)ApiDlAbortPendingOperations(ALL);
        (void)ApiDlSendNegativeResponse(specificCauseCode7|(UINT16)conditionsNotCorrect);
        return;
    }

    /* abort all operations and do actions or return without action */
    if (false == ApiDlAbortPendingOperations(ALL))
    {
        return;
    }

    if ((pu8Data[0] == 0u) && (pu8Data[1] == 0x44u))     /*34 00 44���������أ�����Խ��յ����������ж�*/
    {
        /* Suppress 'Increment or decrement operation performed on pointer' and 'Performing pointer arithmetic'
         * messages */
        tDl.u32StartAddress = GET_UINT32(&pu8Data[2]);          /*ת����������Ϊ��ַ*/
        tDl.u32WorkAddress = tDl.u32StartAddress;
        tDl.u32Length = GET_UINT32(&pu8Data[6]);                 /*ת����������Ϊд��mcu���ݵĳ���*/ 
        tDl.u32EndAddress = tDl.u32WorkAddress + tDl.u32Length;
		
		/*��ַ��Ч�Լ��*/
		if(tDl.u32StartAddress == START_TABLE_ADDR_EOL)
		{
			if((tDl.u32EndAddress != START_TABLE_ADDR_APP) && (tDl.u32EndAddress != END_TABLE_ADDR_APP))
			{
				eMemoryType = Invalid;
			}
			else
			{
				eMemoryType = INTFLASH;
			}
		}
		else if(tDl.u32StartAddress == START_TABLE_ADDR_APP)
		{
			if(tDl.u32EndAddress != END_TABLE_ADDR_APP)
			{
				eMemoryType = Invalid;
			}
			else
			{
				eMemoryType = INTFLASH;
			}
		}
		else
		{
			eMemoryType = Invalid;
		}

		/* if the  memoryAddress/memorySize is valid for RD */
        if (eMemoryType != Invalid)
        {
            tDl.eDlMemtype = eMemoryType;              /*�������ͣ���ֱ��дΪflash*/

            /* Battery voltage management */
            switch(ApiLlJudgeBatteryVoltage())      /*��ѹ�ж�*/
            {
                case battVoltageTooHigh:
                    (void)ApiDlSendNegativeResponse((UINT16)voltageTooHigh);
                    return;
                case battVoltageTooLow:
                    (void)ApiDlSendNegativeResponse((UINT16)voltageTooLow);
                    return;
                default:
                    break;
            }

            /* get memory parameters of automatic erase  */
            tDl.u32AutErAddress = tDl.u32StartAddress;    /*��ȡ������ʼ��ַ*/
            tDl.u32AutErSize = tDl.u32Length;    			 /*��ȡ��������*/

            /* init state machine of aut erase */
            tDl.eAutEraseStat = Validate;
            /* code, data or both can be erased. Invalidate which is touched */
            ApiSetInvalidationTarget();
            /* Send a wait if needed */
            /* in case of big endian MCU this is not implemented */
            SEND_WAIT_BEFORE_ERASE(tDl.u32AutErAddress, tDl.u32AutErSize);

            /* init automatic erase bg service */
            tSemaphores.bAutErase = true;
            /* init timer for response time */
            ApiInitTimer(erase_timer_handle,P2xCAN_TO);
        }
        else
        {
            /* if the specified specified memoryAddress/memorySize is not valid */
            (void)ApiDlSendNegativeResponse((UINT16)requestOutOfRange);
        }
    }
    else
    {
        /* if the specified dataFormatIdentifier
         * or addressAndLengthFormatIdentifier is not valid */
        (void)ApiDlSendNegativeResponse((UINT16)requestOutOfRange);
    }
    /*TODO: NRC: uploadDownloadNotAccepted is not implemented yet: If fault
    condition for genernalProgramming failure is the unique one, then NRC 0x70
    is not necessary. */
}


/******************************************************************************
*  function name | ApiDlRequestdlBg
*  content       | Working of the automatic erase.
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlRequestdlBg(void)
{
    INT32 s32EraseResult;
    UINT8* pu8Response;
	// r_fcl_request_t tReq; 
	UINT8 u8ProCnt[64];
	INT32 s32Ret;
	UINT16 u16ProgCtr;

	UINT8 i;
	UINT32 u32BtRet;

    switch (tDl.eAutEraseStat)
    {
    /* check that which range was erased till last rest */
    case Validate:
		// TODO:������Ϊ���
		ApiWdtSwtFeedDog();
		if(tDl.u32AutErAddress == START_TABLE_ADDR_EOL)   /*��ַ��block�ſ�ͨ����������ṹ�壬�����Ż�*/
		{
			if(tDl.u32AutErSize  == 0x00020000)
			{
				// for(i = 0; i < 4; i++)
				// {
				// 	ApiWdtSwtFeedDog();
				// 	s32EraseResult = ApiFlashErase(&tReq,(9+i),1);
                    
				// }
                s32EraseResult = ApiFlashErasePage(tDl.u32AutErAddress,tDl.u32AutErAddress+tDl.u32AutErSize);
			}
			else
			{
				// for(i = 0; i < 29; i++)
				// {
				// 	ApiWdtSwtFeedDog();
				// 	ApiWdtHwtFeedDog();
				// 	s32EraseResult = ApiFlashErase(&tReq,(9+i),1);
				// }
                s32EraseResult = ApiFlashErasePage(tDl.u32AutErAddress,0x80000);//erase all!
			}
		}
		else if(tDl.u32AutErAddress == START_TABLE_ADDR_APP)
		{
			// for(i = 0; i < 25; i++)
			// {
			// 	ApiWdtSwtFeedDog();
			// 	ApiWdtHwtFeedDog();
			// 	s32EraseResult = ApiFlashErase(&tReq,(13+i),1);
			// }
            s32EraseResult = ApiFlashErasePage(tDl.u32AutErAddress,0x80000);
		}
		else
		{
		}

		/*ˢ�¼�������*/
		s32Ret = ApiNvramReadSyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN,  &u8ProCnt[0]);
        if (OK == s32Ret)
        {
            u16ProgCtr = (((UINT16)u8ProCnt[4])<<8) | u8ProCnt[5];
            if (PROG_CTR_DEFAULT == u16ProgCtr)
            {
                u16ProgCtr = 0u; /* EEPROM was erased, after 1st erase ProgCtr will be 1 */
            }
            u16ProgCtr++;
            u8ProCnt[5] = (UINT8)(u16ProgCtr);     /* lower  */
            u8ProCnt[4] = (UINT8)(u16ProgCtr>>8);  /* higher */
			ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8ProCnt[0]);
        }

		if(s32EraseResult == ERROR)
        {
            tSemaphores.bAutErase = false;
            /* init aut erase state machine */
            tDl.eAutEraseStat = Validate;
            (void)ApiDlSendNegativeResponse((UINT16)generalProgrammingFailure);
        }
		else
		{
			tDl.eAutEraseStat = SucFinished;
		}
        break;

    /* execute the actions after automatic erase */
    case SucFinished:                        /*�������*/
        /* invalidate bg flag */
        tSemaphores.bAutErase = false;

        /* init aut erase state machine */
        tDl.eAutEraseStat= Validate;

        /*REQ: BL_SAIC094 - format of positive response to request download*/
        /* prepare positive response */
        pu8Response = ApiDlGetResponseBuffer();
        pu8Response[0] = 0x20u;
        /*REQ: BL_SAIC095 - maxNumberOfBlockLength*/
        /* receive buffer size in bytes, max. nr. of bytes sent by tester with transferdata */
        pu8Response[1] = MSB(NumberOfBlockSize);
        pu8Response[2] = LSB(NumberOfBlockSize);
        (void)ApiDlSendPositiveResponse(pu8Response,3);

        /* ECU is ready to download data */
        tSemaphores.bDownload = true;
        /* TD flow have not started yet */
        tDl.eTdFlowResult = ActiveOrNotStarted;

        tDl.u8SequenceCounter = 1u;
        tDl.u8PrevSequenceCounter = 1u;
        /*save work address in case of repeated TD*/
        tDl.u32PreWorkAddress = tDl.u32WorkAddress;

        bTdAborted = false;
        tSemaphores.bTdInProgress = false;

        bTdIgnore = false;

        break;

    default:
        /* never ever ever */
        break;
    }
}


/******************************************************************************
*  function name | ApiDlTransferdatadl
*  content       | process the start of the transfer data service.
*  parameter     | data:���� length:����
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlTransferdatadl(UINT8 *pu8Data, UINT16 u16Length)
{
    UINT32 u32RemainingLength;

    /* abort all operations and do actions or return without action */
    if (ApiDlAbortPendingOperations(ALL) == false)
    {
        return;
    }
    /* Error handling will be done in the background process
     * after the whole TD arrives*/
    if (u16Length < TD_MSG_MIN_LENGTH)
    {
        tDl.eTdFlowResult = Failed;
        (void) ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);
        return;
    }

    /* Battery voltage management */
    switch(ApiLlJudgeBatteryVoltage())
    {
        case battVoltageTooHigh:
            (void)ApiDlSendNegativeResponse((UINT16)voltageTooHigh);
            return;
        case battVoltageTooLow:
            (void)ApiDlSendNegativeResponse((UINT16)voltageTooLow);
            return;
        default:
            /* OK continue */
            break;
    }

    /* length of this transfer data payload */
    tDl.u16TdLength = u16Length - 1;

    /*set the input pointer to the end of the already received data*/
    if (tDl.u16TdLength <= (UINT32) (SINGLEFRAME_LENGTH - 2u))
    {
        tDl.u16Inptr = tDl.u16TdLength;
        /* deadline for the answer */
        ApiInitTimer(dl_timer_handle,P2CAN_TO);

    }
    else
    {
        tDl.u16Inptr = FIRSTFRAME_LENGTH-2u;
    }

    /*set up parameters for background processing*/
    tDl.u16Outptr = 0u;
    tSemaphores.bTdInProgress = true;


    /* unexpected: without request download */
    if(tSemaphores.bDownload == false)
    {
        /* Precondition (successful RequestDownload) not ok */
        tDl.u8NegativeResponseCode = requestSequenceError;
        tDl.bNegativeResponseRequired = true;
        return;
    }

    /*if the previous TD block was aborted*/
    if ((pu8Data[0] == tDl.u8SequenceCounter) && (false != bTdAborted))    /*�����һ�ε�blockû��ִ�У�����ִ����һ�ε�*/
    {
        /* roll-back work address */
        tDl.u32WorkAddress = tDl.u32PreWorkAddress;
        /* prev_sequencecounter should not be roll-back,because it differs */
        /* from sequencecounter only if TD download is finished and finished can not be aborted */
        /*new TD started -> initialize */
        bTdAborted = false;

        /*it's a normal working again*/
        tDl.eTdFlowResult = ActiveOrNotStarted;
    }
    /*tester re-transmitted the previously successfully programmed TD block*/
    else if((pu8Data[0] == tDl.u8PrevSequenceCounter) && (pu8Data[0] != tDl.u8SequenceCounter))    /*����ظ����ͣ����Դ˴η�������*/
    {
        /*ignore the re-transmitted block*/
        bTdIgnore = true;
        return;
    }
    /* if wrong sequence counter is received */
    else if(pu8Data[0] != tDl.u8SequenceCounter)   /*����յ���block��Ŵ����򷵻ط�Ӧ��*/
    {
        tDl.u8NegativeResponseCode = wrongBlockSequenceCounter;
        tDl.bNegativeResponseRequired = true;
        return;
    }
    else
    {
        /*correct sequence counter*/
    }

    /* check invalid length */
    u32RemainingLength = tDl.u32EndAddress - tDl.u32WorkAddress;    /*������ַ����ǰ��ַ���ж������Ƿ���*/

    /* SAIC 08 specific: if the data received is more than requested in RequestDownload */
    if (tDl.u16TdLength > u32RemainingLength)                    /*���������,���յ������ݴ���ʣ�೤��*/
    {
        tDl.u8NegativeResponseCode = transferDataSuspended;  /*���ͷ�Ӧ�𣬴���������ֹ*/   
        tDl.bNegativeResponseRequired = true;
        return;
    }

    /*if the TD is not the last, but it is shorter than the NumberOfBlockSize
     * OR the whole download can fit into one TD, but this TD don't contain all data
     * OR this TD is larger than the allowed NumberOfBlockSize*/
    if( ((u32RemainingLength> (UINT32)(NumberOfBlockSize-2u)) && (tDl.u16TdLength < (UINT16)(NumberOfBlockSize-2u))) ||
        ((u32RemainingLength < (UINT32)(NumberOfBlockSize-2u)) && (tDl.u16TdLength != u32RemainingLength)) ||
         (tDl.u16TdLength> (UINT16)(NumberOfBlockSize-2u)) )      /*�ж����ݳ����Ƿ���ȷ*/
    {
        tDl.u8NegativeResponseCode = requestOutOfRange;
        tDl.bNegativeResponseRequired = true;
        return;
    }

    /* cut down block counter */
    tDl.pu8TdMessage = &pu8Data[1];
    tDl.bNegativeResponseRequired = false;

    /* store TD params as previous for the next TD evaluation */
    tDl.u32PreWorkAddress = tDl.u32WorkAddress;
    tDl.u8PrevSequenceCounter = tDl.u8SequenceCounter;
    /* continue in the background*/
}


/******************************************************************************
*  function name | ApiDlTransferdataMore
*  content       | process the remaining parts of the request.
*  parameter     | data:���� length:����
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlTransferdataMore(void)
{
    tDl.u16Inptr += (UINT16) CONSECUTIVEFRAME_LENGTH;
    /*if the whole transfer data request has been received */
    if (tDl.u16Inptr >= tDl.u16TdLength)
    {
        tDl.u16Inptr = tDl.u16TdLength;
        /* deadline for the answer */
        ApiInitTimer(dl_timer_handle,P2CAN_TO);
    }
}


/******************************************************************************
*  function name | ApiDlTransferdatadlBg
*  content       | It is the background task of download handler.
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlTransferdatadlBg(void)
{
    UINT8 *pu8Response;
	// r_fcl_request_t tReq;
	INT32 s32Ret;
	UINT32 u32BtRet;

    /* if the whole transfer data request has arrived */
    if ((tDl.u16Inptr == tDl.u16TdLength))               /*����յ���ȫ������*/
    {
        /*if some problem happened during the transfer data
         * send the negative response when the whole TD arrived*/
        if (false != tDl.bNegativeResponseRequired)
        {
            tDl.u16Outptr = tDl.u16Inptr;
            /* TD flow have got error */
            /*stop background process */
            tSemaphores.bTdInProgress = false;
            tDl.eTdFlowResult = Failed;
            (void) ApiDlSendNegativeResponse((UINT16)tDl.u8NegativeResponseCode);
            return;
        }

        /*and we need to answer to the request within the deadline*/
        if (ApiCheckTimer(dl_timer_handle) == expired)
        {
            /* send a wait before start programming */
            (void) ApiDlSendNegativeResponse((UINT16)responsePending);
            ApiInitTimer(dl_timer_handle,P2xCAN_TO);
            return;
        }
    }

    /* if there is any data that need to be programmed and there was no error */
    if((tDl.u16Outptr < tDl.u16Inptr) && (false == tDl.bNegativeResponseRequired))
    {
        /*if the TD block has not been ignored*/
        if(false == bTdIgnore)
        {
            /* do the programming */
			if(((tDl.u16Inptr-tDl.u16Outptr)/256) == 4)

			{
				ApiWdtSwtFeedDog();
				if(tDl.eDlMemtype == INTFLASH)
				{
                    s32Ret=Ecu_Flash_ReadWriteNumBlockData(tDl.u32WorkAddress,(UINT32)&tDl.pu8TdMessage[tDl.u16Outptr],  1024,TRUE);
					// s32Ret = ApiFlashWrite(&tReq, (UINT32)&tDl.pu8TdMessage[tDl.u16Outptr], tDl.u32WorkAddress, 4);
				}
				
	            if(s32Ret == ERROR)
	            {
	                tDl.u8NegativeResponseCode = generalProgrammingFailure;
	                tDl.bNegativeResponseRequired = true;
	                /* Download interrupted, error occured */
	                tSemaphores.bDownload = false;
	                (void)ApiDlTransferdataAbort();
	                return;
	            }
	            /* if the programming was successful */
	            else if (OK == s32Ret)
	            {
	            	/*  casting prog_result to uint32 is intentional, and this branch is
	                 executed only if prog_result is positive    */
	                tDl.u32WorkAddress += (UINT32)(tDl.u16Inptr-tDl.u16Outptr);
				/*	if(dl.work_address == END_TABLE_ADDR_BT)
					{
						dl.work_address = START_TABLE_ADDR_BT2;
					}
					*/
	                /* step with the output pointer to the next part of the data */
	                tDl.u16Outptr+= (UINT16)(tDl.u16Inptr-tDl.u16Outptr);

	                /*if the whole download is finished */
	                if((tDl.u32WorkAddress == tDl.u32EndAddress) &&
	                        (tDl.u16Inptr == tDl.u16Outptr) &&
	                        (tSemaphores.bTdInProgress != false))
	                {
	                    tSemaphores.bTdInProgress = false;

	                    /*send the positive response */
	                    pu8Response = ApiDlGetResponseBuffer();
	                    pu8Response[0] = tDl.u8SequenceCounter;
	                    /*init the sequence counter in case of error handling of */
	                    /* further TDs without RDL and with not last sequence counter */
	                    tDl.u8SequenceCounter = 1u;

	                    tDl.eTdFlowResult = Completed;
	                    (void)ApiDlSendPositiveResponse(pu8Response,1);
	                }
	                /* if this TD block is finished, but not the whole download*/
	                else if((tDl.u16Outptr >= (tDl.u16TdLength)))
	                {
	                    /*send positive response*/
	                    pu8Response = ApiDlGetResponseBuffer();
	                    pu8Response[0] = tDl.u8SequenceCounter;
	                    (void)ApiDlSendPositiveResponse(pu8Response,1);

	                    /* wait for a new TD request*/
	                    tSemaphores.bTdInProgress = false;
	                    /*increase sequence counter*/
	                    tDl.u8SequenceCounter++;
	                }
	                else
	                {
	                    /*process remaining data in the background*/
	                }
			    }			

            }
            else
            {
                /*do nothing*/
            }
        }
        /*if this block is ignored - successfully programmed before
         * before the tester retransmitted it. */
        else
        {
            /*do like all data has been processed*/
            tDl.u16Outptr = tDl.u16Inptr;
            /*if at the end of the TD block*/
            if(tDl.u16Outptr == tDl.u16TdLength)
            {
                /*send the positive response*/
                pu8Response = ApiDlGetResponseBuffer();
                pu8Response[0] = tDl.u8PrevSequenceCounter;
                (void)ApiDlSendPositiveResponse(pu8Response,1);

                bTdIgnore = false;
                /* wait for a new TD request*/
                tSemaphores.bTdInProgress = false;
            }
        }
    }
    else
    {
        /* TD not finished yet, but it returned a negative response
         * so wait until the TD finishes */
    }
}


/******************************************************************************
*  function name | ApiDlTransferdataAbort
*  content       | abort the transferdata process if there was an error in the transport layer.
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
bool ApiDlTransferdataAbort(void)
{
    /* actual TD block is aborted*/
    bTdAborted = true;

    /*set result to Failed temporary in case the tester don't retransmit this block*/
    tDl.eTdFlowResult = Failed;
    /* abort the mem type we worked on or return true */
    return true;
}


/******************************************************************************
*  function name | ApiDlRequestdlAbort
*  content       | abort the transferdata process if there was an error in the transport layer.
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
bool ApiDlRequestdlAbort(void)
{
    /* init aut erase state machine */
    tDl.eAutEraseStat = Validate;

    /* abort the mem type we worked on or return true */
    return true;
}


/******************************************************************************
*  function name | ApiDlGetTDFlowStatus
*  content       | abort the transferdata process if there was an error in the transport layer.
*  parameter     | void
*  return        | routineStatus:
*  notice        | ��
******************************************************************************/
RoutineStatus_e ApiDlGetTDFlowStatus(void)
{
    return tDl.eTdFlowResult;
}


/******************************************************************************
*  function name | ApiDlGetRDLParam
*  content       | ��ȡ�������صĲ���.
*  parameter     | memory_address:��ַ; memory_size:��С; memtype:����
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlGetRDLParam(UINT32* pu32MemoryAddress, UINT32* pu32MemorySize, MemType_e* peMemType)
{
	if(tDl.eDlMemtype == INTFLASH)
	{
		*pu32MemoryAddress = START_TABLE_ADDR_APP;
		*pu32MemorySize = END_TABLE_ADDR_APP - START_TABLE_ADDR_APP;
		*peMemType = tDl.eDlMemtype;
	}
	else
	{
		*pu32MemoryAddress = tDl.u32StartAddress;
		*pu32MemorySize = tDl.u32Length;
		*peMemType = tDl.eDlMemtype;
	}
    
}

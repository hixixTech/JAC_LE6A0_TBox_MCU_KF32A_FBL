/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    dl_engine.c                                                 */
/* Description:  Diagnostic Layer implementation                             */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** �޸���ʷ *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     �»�        ���ߴ���                              */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* ͷ�ļ�                                                                    */
/*---------------------------------------------------------------------------*/
#include "dl_engine.h"
#include "dl_service.h"
#include "dl_config_pc.h"

#include "bl_timer.h"
#include "bl_app.h"
#include "bl_mem.h"
#include "bl_timer.h"

// #include "mcu_drv_dflash.h"
#include "mem_ee.h"
#include "ll_target.h"
#include "ll_can.h"
#include "os_log.h"
/*---------------------------------------------------------------------------*/
/* ���Ͷ���                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �궨��                                                                    */
/*---------------------------------------------------------------------------*/

#define DONT_SEND_POS_RESP_FOR_DSC_PS

#define NR_LENGTH_NORMAL        ((UINT8)3)
#define NR_LENGTH_22            ((UINT8)4) /* NRC 0x22 (conditionsNotCorrect) requests causeCode */
#define NR_SI                   ((UINT8)0x7f) /* Negative Response Service ID   */

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/
Semaphore_t tSemaphores;
static Message_t tMessage;
static Resp_t tMsgResp;
static bool bIsResponseFinished = true;

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/
/* declaraton is too big to store inside the function*/
static const BgTaskProp_t bg_service[] =
{
/*enum                                                                        */
/*bg_service_mask  *flag                                   bg_abort_func      */
/*ER*/           { &tSemaphores.bDoEcuReset,                &ApiDlEcuResetAbort },
/*WRDBI*/        { &tSemaphores.bDoWdbyid,                  &ApiDlWdbyidAbort },
/*TE*/           { &tSemaphores.bTexit,                     &ApiDlTransferExitAbort },
/*TD*/           { &tSemaphores.bTdInProgress,              &ApiDlTransferdataAbort },
/*RC_EM*/        { &tSemaphores.bRcErase,                   &ApiDlRoutineControlEmAbort },
/*RC_CPDI*/      { &tSemaphores.bRcCpdi,                    &ApiDlRoutineControlCpdiAbort },
/*RC_CPD*/       { &tSemaphores.bRcCheckProgDep,            &ApiDlRoutineControlCheckProgDepAbort },
/*RC_CPI*/       { &tSemaphores.bRcCheckProgInt,            &ApiDlRoutineControlCheckProgIntAbort },
/*RD*/           { &tSemaphores.bAutErase,                  &ApiDlRequestdlAbort},
/*DSC*/          { &tSemaphores.bDoDscBg,                   &ApiDlDscAbort }
};

/* declaraton is too big to store inside the function*/
static const Msgprop_t saicbl_prop_table[] =
{
/*ServiceId                  Length         SessionType  SecurityType  BlType */
{ SID_DSC,                   2u,            deflt,       sec_unlocked, BothAddressing },
{ SID_EcuReset,              2u,            deflt,       sec_unlocked, BothAddressing }, /* SAIC core 008 */
{ SID_SecurityAccess,        multilength,   programming, sec_unlocked, PhysicalAddressing},
{ SID_ReadDataByIdentifier,  multilength,   deflt,       sec_unlocked, BothAddressing},
{ SID_TesterPresent,         2u,            deflt,       sec_unlocked, BothAddressing},
{ SID_WriteDataByIdentifier, multilength,   programming, sec_locked,   PhysicalAddressing},
{ SID_RoutineControl,        multilength,   programming, sec_locked,   PhysicalAddressing},
{ SID_RequestDownload,       11u,           programming, sec_locked,   PhysicalAddressing},
{ SID_TransferData,          multilength,   programming, sec_locked,   PhysicalAddressing},
{ SID_TransferExit,          2u,            programming, sec_locked,   PhysicalAddressing},
{ 0u,                        1u,            deflt,       sec_unlocked, BothAddressing}};


/*---------------------------------------------------------------------------*/
/* �ⲿ���ձ���                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �ڲ���������                                                              */
/*---------------------------------------------------------------------------*/
static void DlDiagnosticServiceHandler(TlStateType_e eTlrxResult);
static MsgProperty_e DlLookupMessageProperties(UINT8* pu8Sid, UINT16* pu16Nac);
static bool DlSendResponse(UINT8 *pu8Resp, UINT16 u16RespLength, bool bRestartS3);
static void DlS3ExpiredBg(void);


/******************************************************************************
*  function name | DlDiagnosticServiceHandler
*  content       | Working of the Diagnostic Service Handler.
*  parameter     | tlrx_result:���ձ���״̬
*  return        | void
*  notice        | ��
******************************************************************************/
static void DlDiagnosticServiceHandler(TlStateType_e eTlrxResult)
{
    MsgProperty_e eMsgProp;
    UINT8   u8ServiceID;
    UINT16  u16NaCode; /* containing causeCode for NRC 0x22*/

    /* if new service request has arrived */
    if (TlReceiveFinished == eTlrxResult)     /*����յ����µķ���ָ��*/
    {
        /* store service ID */
        tMsgResp.u8ServiceId = tMessage.u8Data[0];    /*��ȡ����ID*/

        /* we received a single frame or the last consecutive frame in a
         * segmented receive, so stop the timer. */
        ApiStopTimer(S3_timer_handle);

        /* Check message_st properties */
        eMsgProp = DlLookupMessageProperties(&u8ServiceID,&u16NaCode);  /*У����յ��������Ƿ����ˢ�¹淶�������ط���ID��Ӧ��*/
        /* negative msg_resp handler */
        if (Valid == eMsgProp)            /*����յ��ķ���ID��Ч*/
        {
            /*if we received a valid service request*/
            /* Call Bootloader Services according to the received request */
            switch (u8ServiceID)
            {
            case SID_EcuReset:
                ApiDlEcuReset(&tMessage.u8Data[1]);
                break;
            case SID_DSC:
                ApiDlDsc(&tMessage.u8Data[1]);
                break;
            case SID_SecurityAccess:
                ApiDlSecurityAccess(&tMessage.u8Data[1], tMessage.u16Length - 1);
                break;
            case SID_ReadDataByIdentifier:
                ApiDlRdbyid(&tMessage.u8Data[1], tMessage.u16Length - 1);
                break;
            case SID_WriteDataByIdentifier:
                ApiDlWdbyid(&tMessage.u8Data[1], tMessage.u16Length - 1);
                break;
            case SID_RoutineControl:
                ApiDlRoutineControl(&tMessage.u8Data[1], tMessage.u16Length - 1);
                break;
            case SID_RequestDownload:
                /* call set_invalidation_target() first, it is needed */
                ApiDlRequestdl(&tMessage.u8Data[1]);
                ApiApplicationInvalidate();  /*�����Ч�Ե�*/
                break;
            case SID_TransferData:
                if (tMessage.eFrameType == SingleFrame)
                {
                    /*Process SingleFrame TransferData,���䵥֡���ȵ�����ʱ����ô˺���*/
                    ApiDlTransferdatadl(&tMessage.u8Data[1], tMessage.u16Length - 1);
                }
                else
                {
                    /*last frame of the TransferData */
                    ApiDlTransferdataMore();
                }
                break;
            case SID_TransferExit:
                ApiDlTransferExit(tMessage.u8Data[1]); /* pass crcType */
                break;
            case SID_TesterPresent:
                ApiDlTesterPresent(&tMessage.u8Data[1]);
                break;
            default:
                break;
            }
        }
        else if (NotValid == eMsgProp)         /*���������Чָ��ظ���Ӧ��*/
        {
            /* send negative msg_resp if TlReceiveFinished  */

            (void)ApiDlSendNegativeResponse(u16NaCode);
        }
        else    /*����Ҫ��ָ�����*/
        {
            /*invalid message_st*/
        }
    }
    /* First frame of a multi-frame request*/
    else if (TlNewReceived == eTlrxResult)
    {
        /*N_USDataFF.ind - S3Server must be stopped*/
        ApiStopTimer(S3_timer_handle);

        /*check if the new request is valid or not*/
        eMsgProp = DlLookupMessageProperties(&u8ServiceID,&u16NaCode);
        if (Valid == eMsgProp)
        {
            switch (u8ServiceID)
            {
            case SID_TransferData:
                ApiDlTransferdatadl(&tMessage.u8Data[1], tMessage.u16Length - 1);
                break;
            default:
                break;
            }
        }
        else
        {
            /* if the service is not valid or not important than it will be handled if its reception is finished */
            /*invalid message_st*/
        }
    }
    /* serve processes that can be started before the whole request arrives */
    else if (eTlrxResult == TlMultipleFrameIsReceiving)
    {
        if ((tSemaphores.bDownload != false) && (tMessage.u8Data[0]
                == SID_TransferData))
        {
            ApiDlTransferdataMore();
        }
    }
    else if (eTlrxResult == TlReceiveAborted)
    {
        if (tMessage.u8Data[0] == SID_TransferData)
        {
            /* abort operation can run in TlMultipleFrameIsReceiving mode */
            if (ApiDlAbortPendingOperations(TD) == false)
            {
                /* do nothing - return */
            }
            else
            {
                /* N_USData.ind  that  indicates  an  error  during
                 * the  reception  of  a  multi-frame  request */
                ApiResetTimer(S3_timer_handle);
            }
        }
        else
        {
            /* N_USData.ind  that  indicates  an  error  during
             * the  reception  of  a  multi-frame  request */
            ApiResetTimer(S3_timer_handle);
        }
    }
    else
    {
        /* TlTransmitAborted must be handled elsewhere*/
    }
}


/******************************************************************************
*  function name | DlLookupMessageProperties
*  content       | Services can acquire msg_resp buffer from the Diagnostic Layer.
*  parameter     | sid:����id; nac:ָ��
*  return        | void
*  notice        | ��
******************************************************************************/
static MsgProperty_e DlLookupMessageProperties(UINT8* pu8Sid, UINT16* pu16Nac)
{
    UINT8 u8Cnt;
    MsgProperty_e eMsgProp = NotValid;
    Msgprop_t const* ptCurrentService;
    bool bServiceFound = false;

    /* let's walk through the properties table */
    for (u8Cnt = 0u; saicbl_prop_table[u8Cnt].u8ServiceId != 0u; u8Cnt++)  /*���saic��Ϲ淶����*/
    {
        /* remember current properties table entry */
        ptCurrentService = &saicbl_prop_table[u8Cnt];

        /*check service ID match*/
        if (ptCurrentService->u8ServiceId == tMessage.u8Data[0])  /*�жϽ��յ���can����*/
        {
            /* check message_st length */
            if ((tMessage.u16Length != ptCurrentService->u16Length) &&
                    (ptCurrentService->u16Length != multilength))
            {
                eMsgProp = NotValid;
                *pu16Nac=incorrectMessageLengthOrInvalidFormat;
            }
            /* check addressing */
            else if ((tMessage.eAtyp == Functional)    /*���յ��ķ���ID��ʵ��ˢ����֧�ֵķ���ID��ƥ��*/
                    && (ptCurrentService->eFrameAddressing == PhysicalAddressing))
            {
                eMsgProp = NotValid;
                *pu16Nac=conditionsNotCorrect | specificCauseCode0;
            }
            else if ((tMessage.eAtyp == Physical)
                    && (ptCurrentService->eFrameAddressing == FunctionalAddressing))
            {
                eMsgProp = NotValid;
                *pu16Nac=conditionsNotCorrect | specificCauseCode0;
            }

            /* check correct session */
            /* if the request is available only in programming session and we are not in that */
            else if ((ptCurrentService->eSessionType == programming) /*�Ựģʽ��ƥ��*/
                    && (tSemaphores.bProgrammingSession == false))
            {
                eMsgProp = NotValid;
                *pu16Nac=serviceNotSupportedInActiveSession;
            }

            /* check for security state */
            else if ((ptCurrentService->eSecurityType == sec_locked) /*��ȫ����δͨ��*/
                    && (tSemaphores.bLocked != false))
            {
                eMsgProp = NotValid;
                *pu16Nac=securityAccessDenied;
            }
            /* This case needed by FunctInDefaultBothInProg */
            else if ((tMessage.eAtyp == Physical)   /*������Ĭ�ϻỰֻ֧�ֹ���Ѱַ������ȷ�յ��������ַ*/
                    && (ptCurrentService->eFrameAddressing == FunctInDefaultBothInProg)
                    && (tSemaphores.bProgrammingSession == false)) /*false��Ĭ�ϻỰ*/
            {
                eMsgProp = NotValid;
                *pu16Nac=conditionsNotCorrect | specificCauseCode0;
            }
            else                  /*���������ж�ͨ��*/
            {
                /* we've found a valid one! :-) */

                if (true == ApiDlIsAnyOngoingService(ptCurrentService->u8ServiceId))
                {
                    /* SR2541717607 fix: new request should not interrupt ongoing */
                    /* based on 7.5.5 of ISO_14229_2006(E) */
                    eMsgProp = NotImportant; /* ignored, not passed for upper layers */

                    /* PRQA S 715 2 */ /* Nesting of control structures exceeds 15 */
                    if((SID_TesterPresent == ptCurrentService->u8ServiceId) &&
                       ((tMessage.u8Data[1] & SubFunctionMask) == 0u))
                    {
                        /* Valid TesterPresent message, but download is ongoing */
                        /* suppress positive response */
                        ApiResetTimer(S3_timer_handle);
                    }
                }
                else /* no ongoing service */
                {
                    /* No further services other than TesterPresent are permitted
                       during the correct execution of the sequence RequestDownload,
                       TransferData and RequestTransferExit.
                       Chapter 9 Programming process, page 18,
                       SMTC 2 800 007 Technical requirement for ECU programming(101220)
                    */

                    /* If any of RD, TD, TE is active */
                    if( true == tSemaphores.bDownload) /* RD requested, TE not finished yet */
                    {
                        /* PRQA S 715 4 */ /* Nesting of control structures exceeds 15 */
                        if( (SID_RequestDownload == ptCurrentService->u8ServiceId) ||
                            (SID_TransferData == ptCurrentService->u8ServiceId)    ||
                            (SID_TransferExit == ptCurrentService->u8ServiceId)    ||
                            (SID_TesterPresent == ptCurrentService->u8ServiceId) )
                        {
                            eMsgProp = Valid;              /*������������ع����У��ж��Ƿ����������ָ��粻�Ǻ���*/
                        }
                        else /* not download related, ignore */
                        {
                            eMsgProp = NotImportant;
                            /* #01022425 */
                            ApiResetTimer(S3_timer_handle);
                        }
                    }
                    else /* no ongoing download */
                    {
                        eMsgProp = Valid;
                    }
                }
                /* no negative answer */
                *pu16Nac=0;
            }

            /* we found the service */
            bServiceFound = true;
            /* return the service id */
            *pu8Sid=ptCurrentService->u8ServiceId;   /*�����յ�����ȷ�ķ���ID*/
            break;
        }
        else
        {
            /* Step to the next message_st property record */
        }
    }
    /* the whole list has been processed*/

    /*if we haven't find the service in the list*/
    if (false == bServiceFound)
    {
        /* Do not send a negative msg_resp if the requested diagnostic service is not supported */
      //  if (Functional != message_st.atyp)   /*������ǹ���Ѱַ�����ͷ���֧��*/
      //  {
            eMsgProp = NotValid;
            *pu16Nac=serviceNotSupportedInActiveSession;
      //  }
      //  else                             /*����ǹ���Ѱַ�������ͷ�Ӧ��*/
      //  {
      //      msgprop = NotImportant;
            /* #01022425 */
       //     reset_timer(S3_timer_handle);
                /* no negative answer */
      //      *nac=0;
      //  }
    }
    return eMsgProp;
}


/******************************************************************************
*  function name | DlSendResponse
*  content       | Inform the client that something is wrong or timeout extension
*                  is necessary.
*  parameter     | resp:�϶�Ӧ��ָ��; respLength:Ӧ�𳤶�; restartS3:��λs3ʱ���־λ
*  return        | bool
*  notice        | ��
******************************************************************************/
static bool DlSendResponse(UINT8 *pu8Resp, UINT16 u16RespLength, bool bRestartS3)
{
    bool bRet = false;
    TlStateType_e eTxState;

    if (NULL != pu8Resp)
    {
        eTxState = ApiTlTxEngine(pu8Resp, u16RespLength);
        switch (eTxState)
        {
        case TlTransmitted:
            /*the msg_resp has been sent out*/
            bIsResponseFinished = true;
            if(false != bRestartS3)
            {
                ApiResetTimer(S3_timer_handle);
            }
            bRet = true;
            break;
        case TlMultipleFrameTransmitInProgress:
            /*the msg_resp hasn't been finished*/
            bIsResponseFinished = false;
            bRet = true;
            break;
        case TlNotTransmitted:
            ApiResetTimer(S3_timer_handle);
            bRet = false;
            break;
        case TlTransmitAborted:
            ApiResetTimer(S3_timer_handle);
            bRet = false;
            break;
        default:
            break;
        }
    }
    /* if the buffer is invalid */
    else
    {
        bRet = false;
    }
    return bRet;
}


/******************************************************************************
*  function name | s3_expired_bg
*  content       | Services can acquire msg_resp buffer from the Diagnostic Layer.
*  parameter     | void
*  return        | bool
*  notice        | ��
******************************************************************************/
static void DlS3ExpiredBg(void)
{
    /*if the msg_resp has been sent out*/
    if (ApilLlIsTxEmpty() != false)
    {
        (void)ApiLlReset();
    }
    else
    {/* Do nothing */
    }
}


/*---------------------------------------------------------------------------*/
/* �ⲿ��������                                                              */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | ApiDlIsAnyOngoingService
*  content       | Checking if there is ongoing service.
*  parameter     | ServiceId:����id
*  return        | bool��1-true; 0-false
*  notice        | ��
******************************************************************************/
bool ApiDlIsAnyOngoingService(UINT8 u8ServiceId)
{
    UINT8 i;

    /* walk through the bg services related semaphores */
    for (i = 0u; i < (sizeof(bg_service) / sizeof(BgTaskProp_t)); i++)
    {
        /* check if service is ongoing */
        if (true == (*bg_service[i].pbFlag))
        {
            /* this service is ongoing */
            if( (SID_RequestDownload == u8ServiceId) ||
                (SID_TransferData == u8ServiceId)    ||
                (SID_TransferExit == u8ServiceId) )
            {
                /* download related service: ignore td_in_progress, it is notmal to be set */
                if (bg_service[i].pbFlag != &tSemaphores.bTdInProgress)
                {
                    return true;    /* other service than download related */
                }
            }
            else
            {
                /* ongoing service */
                return true;
            }
        }
    }
    return false;
}


/******************************************************************************
*  function name | ApiDlDiagnosticBgTask
*  content       | It is the second thread. It handles all command processing
*                  issues which cannot be done on-the-fly.
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlDiagnosticBgTask(void)
{
    /*if the previous msg_resp finished*/
    if ((false != ApilLlIsTxEmpty()) &&(false != bIsResponseFinished))
    {
        if(tSemaphores.bS3Expired != false)                    /*�ж�S3�Ƿ�ʱ��5s*/
        {
            DlS3ExpiredBg();                          /*��ʱֱ�Ӹ�λ*/
        }
        if (tSemaphores.bAutErase != false)      /*fg�����л�Դ˱������д���������յ��˷��񣬻�����*/
        {
            tMsgResp.u8ServiceId = SID_RequestDownload;
            ApiDlRequestdlBg();
        }
        if (tSemaphores.bTdInProgress != false)
        {
            tMsgResp.u8ServiceId = SID_TransferData;
            ApiDlTransferdatadlBg();
        }
        if (tSemaphores.bDoWdbyid != false)
        {
            tMsgResp.u8ServiceId = SID_WriteDataByIdentifier;    
            ApiDlWdbyidBg();                                       /*дF100����Ϣ*/
        }
   //     if (semaphores.rc_crc != false)    /*����֧�ִ�31����*/
   /*     {
            msg_resp.serviceid = SID_RoutineControl;
            routinecontrol_crc_bg(&message_st.data[1]);
        }*/
        if (tSemaphores.bRcErase != false)    /*����֧�ִ�31����*/
        {
            tMsgResp.u8ServiceId = SID_RoutineControl;
            /*invalidate the application first*/
    //        application_invalidate();

            ApiDlRoutineControlEmBg();
        }
		if (tSemaphores.bRcCpdi != false)
        {
            /* application validation */
            tMsgResp.u8ServiceId = SID_RoutineControl;
            ApiDlRoutineControlCpdiBg();
        }
        if (tSemaphores.bRcCheckProgDep != false)    /*�����Լ��*/
        {
            tMsgResp.u8ServiceId = SID_RoutineControl;
            ApiDlRoutineControlCheckProgDepBg();      
        }
        if (tSemaphores.bRcCheckProgInt != false)    /*�����Լ��*/
        {
            tMsgResp.u8ServiceId = SID_RoutineControl;
            ApiDlRoutineControlCheckProgIntBg();
        }
        if (tSemaphores.bDoEcuReset != false)
        {
            tMsgResp.u8ServiceId = SID_EcuReset;
            ApiDlEcuResetBg();
        }
        if (tSemaphores.bTexit != false)
        {
            tMsgResp.u8ServiceId = SID_TransferExit;
            ApiDlTransferExitBg(&tMessage.u8Data[1]);
        }
        if (tSemaphores.bDoDscBg != false)
        {
            ApiDlDscBg();
        }
    }
    else
    {
        /* wait until the previous msg_resp has finished*/
    }

}


/******************************************************************************
*  function name | ApiDlSendNegativeResponse
*  content       | Inform the client that something is wrong or timeout extension
*                  is necessary.
*  parameter     | responsecode:��Ӧ��ָ��
*  return        | bool
*  notice        | ��
******************************************************************************/
bool ApiDlSendNegativeResponse(UINT16 u16Responsecode)
{
    bool bRet = false;
    UINT8* ptResp;
    /* Response have to be suppressed in some cases*/
    if ((tMessage.eAtyp == Functional) &&
            ((u16Responsecode == subFunctionNotSupported) ||
            (u16Responsecode == serviceNotSupported) ||
            (u16Responsecode == requestOutOfRange)))
    {
        /*restart S3Server when requested action finished and no msg_resp is allowed*/
        ApiResetTimer(S3_timer_handle);
    }
    /* msg_resp must be sent out */
    else
    {
        /*get pointer to the data part of msg_resp buffer and init msg_resp.txData*/
        ptResp = ApiDlGetResponseBuffer();
        /*initialize service ID part of msg_resp buffer*/
        tMsgResp.pu8TxData[0] = NR_SI;
        /*initialize the data part of msg_resp buffer*/
        ptResp[0] = tMsgResp.u8ServiceId;
        ptResp[1] = (UINT8)u16Responsecode;
        /*send the msg_resp*/
        /*NRC:responsePending does not restart S3Server timer*/
        if (conditionsNotCorrect != ((UINT8)u16Responsecode))
        {
            /*PRQA S 1441 1*/ /* both is from the same enum type*/
            bRet = DlSendResponse(&tMsgResp.pu8TxData[0], NR_LENGTH_NORMAL,(u16Responsecode != responsePending));
        }
        else
        {
            /* conditionsNotCorrect fills SpecificCauseCode */
            ptResp[2] = (UINT8)(u16Responsecode >> 8); /* SpecificCauseCode */
            /*PRQA S 1441 1*/ /* both is from the same enum type*/
            bRet = DlSendResponse(&tMsgResp.pu8TxData[0], NR_LENGTH_22,(u16Responsecode != responsePending));
        }
    }
    return bRet;
}


/******************************************************************************
*  function name | ApiDlSendPositiveResponse
*  content       | Inform the client that something is wrong or timeout extension
*                  is necessary.
*  parameter     | resp:�϶�Ӧ��ָ��; respLength:Ӧ�𳤶�
*  return        | bool
*  notice        | ��
******************************************************************************/
bool ApiDlSendPositiveResponse(const UINT8 *ptResp, UINT16 u16RespLength)
{
    bool bRet;
    /* check is the service returned a valid msg_resp buffer*/
    if (NULL != ptResp)
    {
        tMsgResp.pu8TxData[0] = RESPONSE(tMsgResp.u8ServiceId);
        bRet = DlSendResponse(&tMsgResp.pu8TxData[0], u16RespLength + 1,true);
    }
    else
    {
        bRet = false;
    }
    return bRet;
}


/******************************************************************************
*  function name | ApiDlDiagnosticFgTask
*  content       | It is the first thread. It handles all command processing
*                  issues which can done on-the-fly.
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlDiagnosticFgTask(void)
{
    static TlStateType_e eRxState = TlNotReceived;
    static bool bIsRxPending = false;
    TlStateType_e eState;

    /* if the transmission of the previous msg_resp not finished */
    if (false == bIsResponseFinished)
    {
        eState = ApiTlSegmentedTxEngine();
        if (TlMultipleFrameTransmitInProgress != eState)
        {
            /*transmit finished or aborted*/
            bIsResponseFinished = true;
            ApiResetTimer(S3_timer_handle);
        }
        else
        {
            /*remain in multiple frame transmit in progress state*/
        }
    }
    else
    {
    }

    /*if the previously received data has been processed */
    if(false == bIsRxPending)
    {
        /* check if we received a valid message_st */
        eState = ApiTlRxEngine(&tMessage);      /*��ѯ�Ƿ��յ�can����*/
        if (TlTransmitAborted == eState)
        {
            bIsResponseFinished = true;
            ApiResetTimer(S3_timer_handle);
        }
        /*if we received something important*/
        else if (TlNotReceived != eState)      /*����յ�����*/
        {
            /*if the msg_resp went out */
            if ((false != ApilLlIsTxEmpty()) && (false != bIsResponseFinished))
            {
                /*serve diagnostic requests*/
                DlDiagnosticServiceHandler(eState);    /*��Ϸ���*/
            }
            else
            {
                /* the diagnostic layer couldn't serve the received message_st, because
                 * the msg_resp hasn't been sent -> process the data next time*/
                bIsRxPending = true;
                eRxState = eState;
            }
        }
        else
        {
            /*do nothing*/
        }
    }
    /*process the previously received data*/
    else
    {
        /*if the msg_resp went out */
        if ((false != ApilLlIsTxEmpty()) && (false != bIsResponseFinished))
        {
            /*serve diagnostic requests*/
            DlDiagnosticServiceHandler(eRxState);
            bIsRxPending = false;
        }
        else
        {
            /*try to process the pending received data next time*/
        }
    }

    /* if we are not in the default session check S3 timer */
    if (false != tSemaphores.bProgrammingSession)
    {
        if (expired == ApiCheckTimer(S3_timer_handle))
        {
            tSemaphores.bS3Expired = true;
        }
    }
}


/******************************************************************************
*  function name | ApiDlGetResponseBuffer
*  content       | Services can acquire msg_resp buffer from the Diagnostic Layer.
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
UINT8* ApiDlGetResponseBuffer(void)
{
    tMsgResp.pu8TxData = ApiTlGetTlBuffer();
    /*return the address of the data buffer(-1) and let the service to fill it in*/
    return (UINT8* const ) &tMsgResp.pu8TxData[1];
}


/******************************************************************************
*  function name | ApiDlAbortPendingOperations
*  content       | Services can acquire msg_resp buffer from the Diagnostic Layer.
*  parameter     | mask
*  return        | bool
*  notice        | ��
******************************************************************************/
bool ApiDlAbortPendingOperations(UINT16 u16Mask)
{
    UINT8 i;
    bool bRet = true;

    /* walk through the bg services */
    for (i = 0; i < (sizeof(bg_service) / sizeof(BgTaskProp_t)); i++)
    {
        /* check if current service needs to be aborted (bit of mask is set or not) */
        /* and if it is ongoing */
        if ((0u !=(u16Mask & (UINT16)(1u << i))) && (0 != *(bg_service[i].pbFlag)))
        {
            /* call its abor func */
            bRet = bg_service[i].bg_func();
            if (false!= bRet)
            {
                /* disable background flag */
                *(bg_service[i].pbFlag) = false;
                /*return true*/
            }
            else
            {
                /*return false*/
            }
        }
    }

    /* there was nothing to abort */
    return bRet;
}


/******************************************************************************
*  function name | ApiDlAppSwitchToProgramming
*  content       | Services can acquire msg_resp buffer from the Diagnostic Layer.
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlAppSwitchToProgramming(void)
{
    UINT8 u8ReadFlag[64];
    INT32 s32Retval;

	UINT8 fill[64] = {DATA_0xFF_64bytes};

    /* called Programming Entry Flag (PEF) in  specification 008 */

    /* If the application is running and get a DSC enter programmingSession,
       it will update Programming Entry Flag (PEF), and applies ECU reset.
       The application usually sends a "wait", to avoid bus error during reset.
    */
	s32Retval = ApiNvramReadSyncInd(EEPID_FLHVLD,EEPID_FLHVLD_LEN,&u8ReadFlag[0]);  /*��ȡˢ����ڱ�־λ*/
    ApiLogPrint(_LOG_DEBUG,"s32Retval == %d!\n",s32Retval);
	if(s32Retval == FALSE)
	{
		ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &fill[0]);
	}
	
    if ((s32Retval == TRUE) && (u8ReadFlag[0] == TRUE))   /*���ˢ����ڱ�־λΪ1*/
    {
        ApiLogPrint(_LOG_DEBUG,"FBL u8ReadFlag[0] == true!\n");
        tMsgResp.u8ServiceId = SID_DSC; /* for response */
#ifndef DONT_SEND_POS_RESP_FOR_DSC_PS
        if(true == ApiDlDscEnterProgSession(0u))    /* do not suppress positive response */
        {
			u8ReadFlag[0] = 0;            /*�����Ч�Ա�־λ*/
			ApiNvramWritAsyncInd(EEPID_FLHVLD,EEPID_FLHVLD_LEN,&u8ReadFlag[0]);
        }
        /* else: SBL activization failed, so BL can not modify memory*/
#else
        if(true == ApiDlDscEnterProgSession(1u))    /* suppress positive response */
        {
			u8ReadFlag[0] = 0;
			ApiNvramWritAsyncInd(EEPID_FLHVLD,EEPID_FLHVLD_LEN,&u8ReadFlag[0]);
        }
#endif
    }
}

/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    tl_engine.c                                                 */
/* Description:  Transport Layer implementation                              */
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
#include "tl_engine.h"
#include "bl_timer.h"
#include "ll_target.h"
#include "dl_engine.h"


/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    Idle = 0,
    WaitForConsecutive,
    WaitForFlowControl,
    SegmentedTransmitInProgress
} TlState_e;

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define N_PCItype               ((UINT8)0xf0)
#define SF_DL                   ((UINT8)0x0f)
#define CF_SN                   ((UINT8)0x0f)
#define FC_FS                   ((UINT8)0x0f)

#define N_PCISingleFrame        (UINT8)(0u<<4)   /*单帧*/
#define N_PCIFirstFrame         (UINT8)(1u<<4)   /*首帧*/
#define N_PCIConsecutiveFrame   (UINT8)(2u<<4)   /*连续帧*/
#define N_PCIFlowControl        (UINT8)(3u<<4)   /*流控帧*/

#define CONSECUTIVEFRAME_LENGTH ((UINT8)7)
#define FIRSTFRAME_LENGTH       ((UINT8)6)
#define SINGLEFRAME_LENGTH      ((UINT8)7)

/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/
static TxBufferType_t tTxBuffer;
static TlState_e eTlState;
static UINT8 u8SequenceNumber;
static BOOL bFlowControlPending;
static FsType_e eFlowstatus;


/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/
static BOOL TlSendFlowControl(FsType_e eStatus);


/******************************************************************************
*  function name | TlSendFlowControl
*  content       | interfacing the device driver to the higher layer
*  parameter     | status:流控帧类型
*  return        | BOOL:
*  notice        | 无
******************************************************************************/
static BOOL TlSendFlowControl(FsType_e eStatus)
{
    BOOL bret;
    UINT8 u8FcFrame[8];

    u8FcFrame[0] = N_PCIFlowControl | (UINT8)eStatus;   /*发送流控帧首字节0x30*/
    /* BlockSize support */
    /* BlockSize parameter value zero shall be used to indicate that no more FlowControl frames shall be send during transmission */
    u8FcFrame[1] = 0u;   /* blocksize */
    u8FcFrame[2] = 0u;   /* stmin is not supported */
    /*tx_buff->frame[0].u8Data[2] = 0xF0 | MAX_CYCLE_TIME_100US;*/
    ApiLlFill(&u8FcFrame[3], PADDING_PATTERN, (UINT16)5u);    /*空余字节填充0xAA*/
    bret = ApilLlTx(&u8FcFrame[0]);                      /*can发送*/
    /* successfully sent*/
    if(FALSE != bret)
    {
        /* flow control isn't pending any more*/
        bFlowControlPending = FALSE;
        /*if a continue to send has been sent successfully */
        if(ContinueToSend == eStatus)
        {
            eTlState = WaitForConsecutive;
            /* timeout to next consecutive frame */
            ApiResetTimer(NCr_timer_handle);
        }
        else
        {
            /*do nothing*/
        }
    }
    /*send next time */
    else
    {
        /*store flow status */
        bFlowControlPending = TRUE;
        eFlowstatus = eStatus;
    }
    return bret;
}


/*---------------------------------------------------------------------------*/
/* 外部函数定义                                                              */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | ApiTlRxEngine
*  content       | interfacing the device driver to the higher layer
*  parameter     | message:接收的can报文
*  return        | tlState_type:报文类型
*  notice        | 无
******************************************************************************/
TlStateType_e ApiTlRxEngine(Message_t *ptMessage)   /*处理接收到的can数据*/
{
    static RxBuff_t tTlRxBuffer;
    static UINT8 u8TlSn;
    static UINT16 u16TlNumberOfWaits;
    BOOL bDiscardFrame = FALSE;
    BOOL BIsOngoing;
    UINT16 u16MsgLength;
    TlStateType_e eRetState = TlNotReceived;

    if(FALSE != bFlowControlPending)
    {
        (void)TlSendFlowControl(eFlowstatus);   /*发送流控帧*/
    }
    /*get received data from the device driver*/
    ApiLlRx(&tTlRxBuffer);            /*从驱动层获取数据*/
    BIsOngoing = ApiDlIsAnyOngoingService(tTlRxBuffer.tFrame.u8Data[1]);
    /*if a new frame has been received*/
    if(tTlRxBuffer.bReceived != FALSE)
    {
        /* reject anything that is Functionally addressed and FirstFrame, ConsecutiveFrame or FlowControl */
        /* ISO_15765-2_2004_PDF_version_(en)_033616_CPDF.pdf 6.7.3 Unexpected arrival of N_PDU */
        if((tTlRxBuffer.tFrame.eAtyp == Functional) && ((tTlRxBuffer.tFrame.u8Data[0]
                & N_PCItype) != N_PCISingleFrame))
        {
            bDiscardFrame = TRUE;
        }
        /* Bypass logic according to ISO14229-chapter 7.5.5 - Functional TP ignored during any activity (#01019395)*/
        else if((tTlRxBuffer.tFrame.u8Data[0] == (SingleFrame | 2u))
                && (tTlRxBuffer.tFrame.u8Data[1] == SID_TesterPresent)
                && (tTlRxBuffer.tFrame.eAtyp == Functional) && (TRUE == BIsOngoing))
        {
            /* if the timer is not stopped*/
            if(FALSE == ApiIsTimerStopped(S3_timer_handle))
            {
                /*restart it*/
                ApiResetTimer(S3_timer_handle);
            }
            else
            {

            }
            bDiscardFrame = TRUE;
        }

        /*if a new NPDU starts */
        else if(((tTlRxBuffer.tFrame.u8Data[0] & N_PCItype) == N_PCISingleFrame)
                || (((tTlRxBuffer.tFrame.u8Data[0] & N_PCItype) == N_PCIFirstFrame)))
        {
            /* Discard frame (SF,FF) if segmented transmit (multi frame) in progress or it is sending out the last (#01019395)*/
            if(((SegmentedTransmitInProgress == eTlState)
                     || (WaitForFlowControl == eTlState))||((TRUE == BIsOngoing)&&( tTlRxBuffer.tFrame.eAtyp == Functional)))
            {
                bDiscardFrame = TRUE;
            }
            /* We have a single frame */
            else if((tTlRxBuffer.tFrame.u8Data[0] & N_PCItype) == N_PCISingleFrame)
            {
                /* Check if the SF_DL is correct in case of single frame  */
                /* means 1<=SF_DL<=7                                      */
                u16MsgLength = tTlRxBuffer.tFrame.u8Data[0] & SF_DL;
                if((u16MsgLength == 0u) || (u16MsgLength > 7u))
                {
                    bDiscardFrame = TRUE;
                }
                else
                {
                    /*Valid Single Frame so copy its content */
                    ptMessage->u16Length = u16MsgLength;
                    ApiLlCopyBuffer(ptMessage->u8Data, &tTlRxBuffer.tFrame.u8Data[1],
                            (UINT8)ptMessage->u16Length);
                    ptMessage->eFrameType = SingleFrame;
                    ptMessage->eAtyp = tTlRxBuffer.tFrame.eAtyp;
                    eRetState = TlReceiveFinished;
                }
            }
            /* We have a first frame */
            else
            {
                /* Check the data length */
                u16MsgLength = (UINT16)(((UINT16)tTlRxBuffer.tFrame.u8Data[0]
                        & (UINT16)SF_DL) << 8)
                        | (UINT16)tTlRxBuffer.tFrame.u8Data[1];
                if(u16MsgLength < 8u)
                {
                    bDiscardFrame = TRUE;
                }
                /* Oversized segmented message */
                else if(u16MsgLength > (UINT16)NumberOfBlockSize)
                {
                    /* Notify sender to abort the transmission */
                    (void)TlSendFlowControl(Overflow);
                    bDiscardFrame = TRUE;
                }
                else
                {
                    /* Valid First Frame*/
                    /* init sequencenr to 1 */
                    u8TlSn = 1u;
                    /* fill the message */
                    ApiLlCopyBuffer(ptMessage->u8Data, &tTlRxBuffer.tFrame.u8Data[2],
                            FIRSTFRAME_LENGTH);
                    ptMessage->u16Length = u16MsgLength;
                    ptMessage->u16Inptr = FIRSTFRAME_LENGTH;
                    ptMessage->eFrameType = FirstFrame;
                    ptMessage->eAtyp = tTlRxBuffer.tFrame.eAtyp;
                    /* Initial response to a first frame ContinueToSend, sender will resume (start) transmit consecutive frames */
                    /*if continue to send has been sent*/
                    (void)TlSendFlowControl(ContinueToSend);
                    eRetState = TlNewReceived;
                }
            }
        }/*if( Single or First frame) */
        /*if we continue the receiving of an already started request */
        else if(((tTlRxBuffer.tFrame.u8Data[0] & N_PCItype) == N_PCIConsecutiveFrame)
                && (eTlState == WaitForConsecutive))
        {
            /* if the NCr timeout expired this frame must be ignored and the
             * receiving of the message must be interrupted */
            if(ApiCheckTimer(NCr_timer_handle) == expired)
            {
                eTlState = Idle;
                eRetState = TlReceiveAborted;
            }
            /* the consecutive frame can be received */
            else
            {
                /* if the sequence counter is correct */
                if((tTlRxBuffer.tFrame.u8Data[0] & CF_SN) == (u8TlSn++ & CF_SN))
                {
                    /*it's not the last CF */
                    if(ptMessage->u16Length > (ptMessage->u16Inptr
                            + (UINT16)CONSECUTIVEFRAME_LENGTH))
                    {
                        /* still receiving */
                        ApiLlCopyBuffer(&ptMessage->u8Data[ptMessage->u16Inptr],
                                &tTlRxBuffer.tFrame.u8Data[1],
                                CONSECUTIVEFRAME_LENGTH);
                        ptMessage->u16Inptr += (UINT16)CONSECUTIVEFRAME_LENGTH;
                        ApiResetTimer(NCr_timer_handle);
                        eRetState = TlMultipleFrameIsReceiving;
                    }
                    /*it is the last part of the segmented message */
                    else
                    {
                        ApiLlCopyBuffer(&ptMessage->u8Data[ptMessage->u16Inptr],
                                &tTlRxBuffer.tFrame.u8Data[1],
                                (UINT8)(ptMessage->u16Length - ptMessage->u16Inptr));
                        ptMessage->u16Inptr = ptMessage->u16Length;
                        ptMessage->eFrameType = ConsecutiveFrame;
                        ptMessage->eAtyp = tTlRxBuffer.tFrame.eAtyp;
                        /* multiple frame transmission is finished*/
                        eTlState = Idle;
                        eRetState = TlReceiveFinished;
                    }
                }
                /* wrong sequence counter */
                else
                {
                    /* abort reception */
                    eTlState = Idle;
                    eRetState = TlReceiveAborted;
                }
            }
        }/*if( Single or First frame) else if(Consecutive)*/
        /* It is a transmission and we are waiting for the FlowControl */
        else if(((tTlRxBuffer.tFrame.u8Data[0] & N_PCItype) == N_PCIFlowControl)
                && (eTlState == WaitForFlowControl))
        {
            /* Check if NBs counter hasn't expired until this moment */
            if(ApiCheckTimer(NBs_timer_handle) == expired)
            {
                /* if expired interrupt the actual transmission */
                eTlState = Idle;
                eRetState = TlTransmitAborted;
                ApiStopTimer(NBs_timer_handle);
            }
            /*FlowControl can be received*/
            else
            {
                /* It is a WAIT */
                if((tTlRxBuffer.tFrame.u8Data[0] & FC_FS) == (UINT8)Wait)
                {
                    /* receiver transmitted the maximum number os waits */
                    if(u16TlNumberOfWaits == MAX_NUMBER_OF_WAITS)
                    {
                        /* abort transmission*/
                        eTlState = Idle;
                        eRetState = TlTransmitAborted;
                    }
                    else
                    {
                        /*reset wait time*/
                        ApiResetTimer(NBs_timer_handle);
                        /*return default :TlNotReceived*/
                    }
                    u16TlNumberOfWaits++;
                }
                /*It is a ContinueToSend*/
                else if((tTlRxBuffer.tFrame.u8Data[0] & FC_FS)
                        == (UINT8)ContinueToSend)
                {
                    /* receiver is ready for a maximum of block size number of consecutive frames */
                    tTxBuffer.bGo = TRUE;
                    u16TlNumberOfWaits = 0u;
                    eTlState = SegmentedTransmitInProgress;

                    ApiStopTimer(NBs_timer_handle);
                    /*return default :TlNotReceived*/
                }
                else /* Overflow or invalid ISO 15765-2 - 6.5.5.3 FS error handling */
                {
                    /* abort transmission */
                    eTlState = Idle;
                    ApiStopTimer(NBs_timer_handle);
                    eRetState = TlTransmitAborted;

                }
            }
        }
        else
        {
            /*nothing important has come*/
        }
        /*tl_rx_buffer has been processed - make it free*/
        tTlRxBuffer.bReceived = FALSE;

        if(bDiscardFrame != FALSE)
        {
            /* discard frame with NO response  */
            eRetState = TlNotReceived;
        }
    }
    /*if nothing has come*/
    else
    {
        /*check NCr timer*/
        if((ApiCheckTimer(NCr_timer_handle) == expired) &&
           (eTlState == WaitForConsecutive) )
        {
            /*if expired interrupt actual receive flow*/
            eTlState = Idle;
            eRetState = TlReceiveAborted;

            if (SID_TransferData == ptMessage->u8Data[0])
            {   /* SAIC core 008 initiates transferDataSuspended for SID_TransferData */
                (void)ApiDlSendNegativeResponse((UINT16)transferDataSuspended);
            }
        }
        /* check NBs timer */
        else if((ApiCheckTimer(NBs_timer_handle) == expired) &&
                (eTlState == WaitForFlowControl))
        {
            eTlState = Idle;
            eRetState = TlTransmitAborted;
            ApiStopTimer(NBs_timer_handle);
        }
        else
        {
            /* Needed because QAC complains without a concluding else */
        }
    }
    return eRetState;
}


/******************************************************************************
*  function name | ApiTlTxEngine
*  content       | interfacing the device driver to the higher layer
*  parameter     | txData:发送数据; dataLength:数据长度
*  return        | tlState_type:报文类型
*  notice        | 无
******************************************************************************/
TlStateType_e ApiTlTxEngine(UINT8 *pu8TxData, UINT16 u16DataLength)
{
    TlStateType_e eRetState;

    /*if the message can fit into a SingleFrame */
    if(u16DataLength <= (UINT16)SINGLEFRAME_LENGTH)
    {
        /*Fill transport layer informations*/
        tTxBuffer.u8Data[1] = (N_PCISingleFrame | (UINT8)u16DataLength);
        /*Complete the frame to be 8 bytes long, fill with PADDING_PATTERN*/
        ApiLlFill(&tTxBuffer.u8Data[u16DataLength + 2u], PADDING_PATTERN, (UINT16)(8u - (u16DataLength
                + 1u)));
        /*Send the frame to the driver. The Frame starts at byte [1]*/
        if(FALSE != ApilLlTx(&tTxBuffer.u8Data[1]))
        {
            eRetState = TlTransmitted;
        }
        else
        {
            /*tx buffer is not empty*/
            eRetState = TlNotTransmitted;
        }
    }
    /* Multiple frame response */
    else
    {
        /*Fill transport layer informations*/
        tTxBuffer.u8Data[0] = N_PCIFirstFrame | MSB(u16DataLength);
        tTxBuffer.u8Data[1] = LSB(u16DataLength);
        tTxBuffer.u16DataLen = u16DataLength - (UINT16)FIRSTFRAME_LENGTH;

        u8SequenceNumber = 1;
        if(FALSE != ApilLlTx(&tTxBuffer.u8Data[0]))
        {
            eTlState = WaitForFlowControl;
            /* deadline until the FlowControl arrives*/
            ApiResetTimer(NBs_timer_handle);
            /*set output buffer to the last byte of the frame*/
            tTxBuffer.u8Outptr = SINGLEFRAME_LENGTH;
            eRetState = TlMultipleFrameTransmitInProgress;
        }
        else
        {
            /*tx buffer is not empty*/
            eRetState = TlNotTransmitted;
        }
    }
    return eRetState;
}


/******************************************************************************
*  function name | ApiTlSegmentedTxEngine
*  content       | interfacing the device driver to the higher layer
*  parameter     | void
*  return        | tlState_type:报文类型
*  notice        | 无
******************************************************************************/
TlStateType_e ApiTlSegmentedTxEngine(void)
{
    TlStateType_e eRetState;
    /*if multiple transmit in progress */
    if(SegmentedTransmitInProgress == eTlState)
    {
        /*Fill transport layer informations*/
        tTxBuffer.u8Data[tTxBuffer.u8Outptr] = (N_PCIConsecutiveFrame
                    | (u8SequenceNumber & 0x0f));

        /* if more date left than one consecutive frame */
        if(tTxBuffer.u16DataLen > (UINT16)CONSECUTIVEFRAME_LENGTH)
        {
            /*Send the frame to the driver.*/
            if(FALSE != ApilLlTx(&tTxBuffer.u8Data[tTxBuffer.u8Outptr]))
            {
                tTxBuffer.u16DataLen -= (UINT16)CONSECUTIVEFRAME_LENGTH;
                u8SequenceNumber++;

                /*step one frame*/
                tTxBuffer.u8Outptr += CONSECUTIVEFRAME_LENGTH;
                eRetState = TlMultipleFrameTransmitInProgress;

            }
            else
            {
                /*send next time*/
                eRetState = TlMultipleFrameTransmitInProgress;
            }
        }
        /*if this is the last frame */
        else
        {
            ApiLlFill(&tTxBuffer.u8Data[tTxBuffer.u8Outptr + tTxBuffer.u16DataLen + 1],
                    PADDING_PATTERN, (UINT16)(CONSECUTIVEFRAME_LENGTH
                            - (UINT8)tTxBuffer.u16DataLen));

            /*Send the frame to the driver.*/
            if(FALSE != ApilLlTx(&tTxBuffer.u8Data[tTxBuffer.u8Outptr]))
            {
                /*if finished */
                eTlState = Idle;
                eRetState = TlTransmitted;
            }
            else
            {
                /* try to retransmit next time */
                eRetState = TlMultipleFrameTransmitInProgress;
            }
        }
    }
    else if(WaitForFlowControl == eTlState)
    {
        /*if the deadline until the FlowControl expired */
        if(ApiCheckTimer(NBs_timer_handle) == expired)
        {
            eTlState = Idle;
            eRetState = TlTransmitAborted;
            ApiStopTimer(NBs_timer_handle);
        }
        else
        {
            /*still in segmented transmit*/
            eRetState = TlMultipleFrameTransmitInProgress;
        }
    }
    /* not correct Transport Layer state */
    else
    {
        eRetState = TlTransmitAborted;
    }
    return eRetState;
}


/******************************************************************************
*  function name | ApiTlGetTlBuffer
*  content       | Diagnostic layer can acquire response buffer from the Transpor Layer
*  parameter     | void
*  return        | UINT8*:
*  notice        | 无
******************************************************************************/
UINT8 *ApiTlGetTlBuffer(void)
{
    return &tTxBuffer.u8Data[2];
}

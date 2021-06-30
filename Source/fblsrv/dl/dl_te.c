/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.		  */
/* File Name:	  dl_te.c													  */
/* Description:  Request Transfer Exit service implementation			      */
/* Others: 	  other description 										  */
/* 																		  */
/* Processor:	  RH850F1L													  */
/* Compiler:	  GHS_201517												  */
/********************************** �޸���ʷ *********************************/
/* Date			Version 	  Author	  Description					  */
/* 2019-07-05	 V11.01.01.00	  �»�		���ߴ���							  */
/*****************************************************************************/
 
/*---------------------------------------------------------------------------*/
/* ͷ�ļ�																	 */
/*---------------------------------------------------------------------------*/
#include "dl_engine.h"
#include "dl_service.h"

#include "bl_mem.h"
#include "bl_timer.h"
#include "mem_ee.h"
// #include "mcu_drv_dflash.h"
#include "bl_crc32.h"
#include "tl_engine.h"

/*---------------------------------------------------------------------------*/
/* ���Ͷ���																  */
/*---------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/* �궨��																	 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* ��������																  */
/*---------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/* ��������																  */
/*---------------------------------------------------------------------------*/
static RcMemory_t tTe; 

/*---------------------------------------------------------------------------*/
/* �ⲿ���ձ���																*/
/*---------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/* �ڲ���������																*/
/*---------------------------------------------------------------------------*/


/******************************************************************************
*  function name | ApiDlTransferExit
*  content       | It is the main command handler to handle Transferexit.
*  parameter     | crcType:�Ƿ���crc
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlTransferExit(UINT8 u8CrcType)
{
    UINT8 *pu8Response;

    /* abort all operations and do actions or return without action */
    if(ApiDlAbortPendingOperations(ALL) == false)
    {
        /*do nothing - return*/
    }
    else
    {
        if(tSemaphores.bDownload != false)
        {
            /* We received RD */
            if (Completed == ApiDlGetTDFlowStatus())
            {
                /*the download process has been finished*/
                tSemaphores.bDownload = false;

                pu8Response = ApiDlGetResponseBuffer();
                (void)ApiDlSendPositiveResponse(pu8Response,0u);
            }
            else
            {
                /* TD flow have not finished (not started, not finished or error) */
                (void)ApiDlSendNegativeResponse((UINT16)requestSequenceError);
            }
        }
        else
        {
            /* The TransferData service has not been preceded by a valid RequestDownload service. */
            (void)ApiDlSendNegativeResponse((UINT16)requestSequenceError);
        }
    }
}


/******************************************************************************
*  function name | ApiDlTransferExitBg
*  content       | It is called after the transferExit request. It waits for the
*                  end of the CRC calculation and sends back the answer.
*  parameter     | data:����
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiDlTransferExitBg(UINT8 *pu8Data)
{
    tSemaphores.bTexit = false;
}


/******************************************************************************
*  function name | ApiDlTransferExitAbort
*  content       | It is abort fuction of background task. Its purpose is to abort
*                  and initiate the background transfere exit activities.
*  parameter     | crcType:�Ƿ���crc
*  return        | void
*  notice        | ��
******************************************************************************/
bool ApiDlTransferExitAbort(void)
{
    if(true == tSemaphores.bTexit)
    {
        return false; /* service is ongoing, do not interrupt it */
    }
    else
    {
        return true;  /* service is not active */
    }
}


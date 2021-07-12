/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    dl_dsc.c                                                    */
/* Description:  Diagnostic Session Control service request                  */
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
#include "dl_engine.h"
#include "dl_service.h"

#include "bl_timer.h"
#include "bl_app.h"
#include "ll_target.h"
// #include "mcu_drv_flash.h"

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/
static bool bIsDscResponseSupressed = false;


/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/


/******************************************************************************
*  function name | ApiDlDsc
*  content       | It is the main command handler to handle DSC.
*  parameter     | data:报文数据
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlDsc(const UINT8 *pu8Data)
{
    UINT8* pu8Response;

    if (ApiDlAbortPendingOperations(ALL) == false)
    {
    /*do nothing -return */
    }
    else
    {
        if(defaultSession == (pu8Data[0] & SessionTypeMask))
        {
            /*check if the positive response is suppressed*/
            if (0u != ((pu8Data[0] & ResRequiredMask)))
            {
                bIsDscResponseSupressed = true;
            }
            else
            {
                bIsDscResponseSupressed = false;
            }

            ApiInitTimer(app_valid_timer_handle,P2CAN_TO);
            tSemaphores.bDoDscBg = true;

        }
        else if (programmingSession == (pu8Data[0] & SessionTypeMask))
        {
        	if(0 == gu8FotaFlag)
        	{
        		tSemaphores.bLocked = true;
        	}

            if(false == tSemaphores.bProgrammingSession)
            {
                /* Activate SBL */
                (void)ApiDlDscEnterProgSession(pu8Data[0] & ResRequiredMask);
            }
            else
            {
                /* already in the programming session */
                if((pu8Data[0] & ResRequiredMask)== 0u)
                {
                    /* response nedded */
                    pu8Response = ApiDlGetResponseBuffer();
                    /*replay the session*/
                    pu8Response[0] = pu8Data[0];
                    /* P2CAN_SERVER_MAX and P2SCAN_SERVER_MAX should be shown */
                    pu8Response[1] = MSB(P2CAN_SERVER_MAX_MS);     /* 1ms resolution of 50ms */
                    pu8Response[2] = LSB(P2CAN_SERVER_MAX_MS);
                    pu8Response[3] = MSB(P2SCAN_SERVER_MAX_MS/10); /* 10ms resolution of 5000ms */
                    pu8Response[4] = LSB(P2SCAN_SERVER_MAX_MS/10);
                    (void)ApiDlSendPositiveResponse(pu8Response,5);
                }
                else
                {
                    /* no response nedded - no abort needed */
                }
            }
            /* REQ: BL_SAIC040 - Once in a Programming Session further requests case only to restart S3 timer */
            /* restart S3 timer in case of successful dsc-programming session */
            ApiResetTimer(S3_timer_handle);
        }
        else
        {
            (void)ApiDlSendNegativeResponse((UINT16)subFunctionNotSupportedInActiveSession);
        }
    }
}


/******************************************************************************
*  function name | ApiDlDscBg
*  content       | It is the main command handler to handle DSC.
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlDscBg(void)
{
    UINT8 *pu8Response;

    /*if positive response isn't suppressed*/
    if (false == bIsDscResponseSupressed)
    {
        pu8Response = ApiDlGetResponseBuffer();
        /*replay the session*/
        pu8Response[0] = defaultSession;
        /* P2CAN_SERVER_MAX and P2SCAN_SERVER_MAX should be shown */
        pu8Response[1] = MSB(P2CAN_SERVER_MAX_MS);     /* 1ms resolution of 50ms */
        pu8Response[2] = LSB(P2CAN_SERVER_MAX_MS);
        pu8Response[3] = MSB(P2SCAN_SERVER_MAX_MS/10);    /* 10ms resolution of 5000ms */
        pu8Response[4] = LSB(P2SCAN_SERVER_MAX_MS/10);
        (void)ApiDlSendPositiveResponse(pu8Response,5);
        bIsDscResponseSupressed = true;
    }
    else
    {/* Do nothing */
    }
    /*if the response has been sent out*/
    if (ApilLlIsTxEmpty() != false)
    {
        (void)ApiLlReset();
    }
    else
    {/* Do nothing */
    }
}


/******************************************************************************
*  function name | ApiDlDscAbort
*  content       | It is the main command handler to handle DSC.
*  parameter     | void
*  return        | bool
*  notice        | 无
******************************************************************************/
bool ApiDlDscAbort(void)
{
    return true;
}


/******************************************************************************
*  function name | ApiDlDscEnterProgSession
*  content       | It is the main command handler to handle DSC.
*  parameter     | ResSuppress:是否需要发送返回值标志位
*  return        | bool
*  notice        | 无
******************************************************************************/
bool ApiDlDscEnterProgSession(UINT8 u8ResSuppress)
{
    bool bRetValue = false;
	INT32 s32Ret;

    tSemaphores.bSblActive = true;
    if(u8ResSuppress == 0u)
    {
        UINT8 *pu8Response;

        /* response nedded */
        pu8Response = ApiDlGetResponseBuffer();
        /*replay the session*/
        pu8Response[0] = programmingSession;
        /* P2CAN_SERVER_MAX and P2SCAN_SERVER_MAX should be shown */
        pu8Response[1] = MSB(P2CAN_SERVER_MAX_MS);     /* 1ms resolution of 50ms */
        pu8Response[2] = LSB(P2CAN_SERVER_MAX_MS);
        pu8Response[3] = MSB(P2SCAN_SERVER_MAX_MS/10);    /* 10ms resolution of 5000ms */
        pu8Response[4] = LSB(P2SCAN_SERVER_MAX_MS/10);
        (void)ApiDlSendPositiveResponse(pu8Response,5);
    }
    tSemaphores.bProgrammingSession = true;
    tSemaphores.bSaSeedSent = false;

	// s32Ret = ApiFlashInit();              /*初始化flash驱动*/
	
	if (s32Ret == OK)
	{
		bRetValue = true;
	}
    
    return bRetValue;
}

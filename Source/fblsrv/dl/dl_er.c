/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    dl_er.c                                                     */
/* Description:  Implementation of the ECU Reset service the service         */
/*               do the application validation before                        */
/*               executing the reset                                         */
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
// #include "mcu_drv_dflash.h"
#include "ll_target.h"
#include "mem_ee.h"


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
static bool bIsErResponseSupressed = false;


/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 外部函数定义                                                              */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | ApiDlEcuReset
*  content       | It is the main command handler to handle the reset.
*  parameter     | data:报文数据
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlEcuReset(const UINT8 *pu8Data)
{
     /* abort all operations and do actions or return without action */
    if (ApiDlAbortPendingOperations(ALL) == false)   /*查表判断是否有其他服务正在执行*/
    {
        /*do nothing - return*/
    }
    else
    {
        if ((pu8Data[0] & SubFunctionMask) != hardReset)    /*如果不是11 01,返回否定应答12子功能不支持*/
        {
            (void)ApiDlSendNegativeResponse((UINT16)subFunctionNotSupported);
        }
        else
        {
            /*check if the positive response is suppressed*/
            if (0u != ((pu8Data[0] & ResRequiredMask)))
            {
                bIsErResponseSupressed = true;
            }
            else
            {
                bIsErResponseSupressed = false;
            }

            tSemaphores.bDoEcuReset = true;
        }
    }
}


/******************************************************************************
*  function name | ApiDlEcuResetBg
*  content       | It is a background task. Its purpose is to perform a reset.
*                  if the last frame is already transmitted successfully.
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlEcuResetBg(void)
{
    UINT8 *pu8Response;
	UINT8 u8AppValid[64] = {0};

	ApiNvramReadSyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, 0, &u8AppValid[0]);

	if((u8AppValid[1] == 0)&& (u8AppValid[2] == 0))
	{
		if(u8AppValid[3] != 1)
		{
			u8AppValid[3] = 1;
			ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8AppValid[0]);
		}
		else
		{
			;
		}
	}
	else if((u8AppValid[1] == 0xff)&& (u8AppValid[2] == 0xff))
	{
		;
	}
	else
	{
		if(u8AppValid[3] != 0)
		{
			u8AppValid[3] = 0;
			ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8AppValid[0]);
		}
		else
		{
			;
		}

	}

    /*if positive response isn't suppressed*/
    if (false == bIsErResponseSupressed)
    {
        pu8Response = ApiDlGetResponseBuffer();
        pu8Response[0] = hardReset;
        (void) ApiDlSendPositiveResponse(pu8Response, 1);
        bIsErResponseSupressed = true;
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
*  function name | ApiDlEcuResetAbort
*  content       | It is abort fuction of background task. Its purpose is to abort
*                  and initiate the background reset activities.
*  parameter     | void
*  return        | bool
*  notice        | 无
******************************************************************************/
bool ApiDlEcuResetAbort(void)
{
    return true;
}


/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    dl_rc.c                                                     */
/* Description:  Routine Control service                                     */
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
#include "dl_config_pc.h"
#include "dl_service.h"

#include "bl_app.h"
#include "bl_mem.h"
#include "bl_timer.h"

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define RC_MSG_MIN_LENGTH               (3u)
#define STARTROUTINE_LENGTH_SHORT       (3u)
#define STARTROUTINE_LENGTH_LONG        (12u)

#ifndef SEND_WAIT_BEFORE_ERASE
/* This is a target specific macro.
   a. Normal case (any erase operation is shorter than 50ms)it is an empty macro
   b. If any erase operation takes longer than 50ms it can be replaced with
      a "(void)send_negative_response((uint16)responsePending)" call unconditionally
   c. If erase address or size determines the erase time of 1st chunk, target
      specific code can decide to send a wait or not
*/
#define SEND_WAIT_BEFORE_ERASE(rc_address,rc_size) /* PRQA S 3138 1 */ /* do nothing */
#endif /* SEND_WAIT_BEFORE_ERASE */


/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/
static UINT16 u16RcId = 0x0u; /* invalid */


/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/
static bool DlIsPendingRc(void);


/******************************************************************************
*  function name | DlIsPendingRc
*  content       | return true if there is any pending routine control operation.
*  parameter     | void
*  return        | bool
*  notice        | 无
******************************************************************************/
static bool DlIsPendingRc(void)
{
    /*
    Note:
    Road vehicles - Unified diagnostic services (UDS) - Specification and requirements
    ISO 14229:2006(E) Second edition / 2006-12-01
    "A common server implementation has only one diagnostic protocol instance
    available in the server which can only handle one request at a time.
    The rule is that any received message (regardless of whether the addressing
    mode is physical or functional) occupies this resource until the request
    message is processed"
    */
    return ((false != tSemaphores.bRcCpdi) || (false != tSemaphores.bRcErase) ||
            (false != tSemaphores.bRcCheckProgInt) || (false != tSemaphores.bRcCheckProgDep));
}


/******************************************************************************
*  function name | ApiDlRoutineControl
*  content       | It is the main command handler to handle RoutineControl.
*  parameter     | data:数据; length:长度
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlRoutineControl(const UINT8 *pu8Data,UINT16 u16Length)
{
    if(false != DlIsPendingRc())
    {
        return; /* active routine cannot be interrupted */
    }

    if(startRoutine == pu8Data[0])
    {
        UINT32 u32RcAddress;
        UINT32 u32RcSize;
        /* PRQA S 0488, 0489 1 */ /* Suppress 'Performing pointer arithmetic' message */
        u16RcId  = GET_UINT16(&pu8Data[1]);

        /* check length*/
        switch (u16RcId)
        {
		    case rcEraseCode:
            case rcEraseCalibration:
            case rcEraseConfig:
            case rcCheckProgramming:
                if(u16Length != STARTROUTINE_LENGTH_SHORT)
                {
                    (void)ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);
                    return;
                }
                break;
            default:    /* invalid ID: not supported */
                (void)ApiDlSendNegativeResponse((UINT16)requestOutOfRange);
                break;
        }

        /* Suppress 'Performing pointer arithmetic' and 'Increment or decrement operation performed on
         * pointer ' messages */
        /* PRQA S 0488, 0489 2 */
        u32RcAddress = GET_UINT32(&pu8Data[4]);
        u32RcSize    = GET_UINT32(&pu8Data[8]);

        switch (u16RcId)
        {
		    case rcEraseCode:
		    case rcEraseCalibration:
		    case rcEraseConfig:
				{
				    tSemaphores.bRcErase = true;
			    }
			    break;
		    case rcCheckProgramming: /* integrity and dependencies */
			    tSemaphores.bRcCpdi = true;
			    /* init timer for response time */
			    ApiInitTimer(app_valid_timer_handle,P2CAN_TO);
			    break;
            default:    /* not supported */
               /* sends NRC above at length check requestOutOfRange */
                break;
        }
    }
    else /* invalid routineControl subfunction */
    {
        if (u16Length < RC_MSG_MIN_LENGTH)
        {
            (void)ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);
        }
        else
        {
            (void)ApiDlSendNegativeResponse((UINT16)subFunctionNotSupported);
        }
    }
}


/******************************************************************************
*  function name | ApiDlRoutineControlEmBg
*  content       | It is the background task of RoutineControl-eraseMemory handler.
*  parameter     | data:数据
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlRoutineControlEmBg(void)
{
	response_type pu8Response;

	pu8Response = ApiDlGetResponseBuffer();
	pu8Response[0] = startRoutine;
	pu8Response[1] = MSB(rcEraseMemory);
	pu8Response[2] = LSB(rcEraseMemory);
	(void)ApiDlSendPositiveResponse(pu8Response,3);

	tSemaphores.bRcErase = false;
}


/******************************************************************************
*  function name | ApiDlRoutineControlCpdiBg
*  content       | It is the background task of RoutineControl-eraseMemory handler.
*  parameter     | data:数据
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlRoutineControlCpdiBg(void)
{
	response_type	  response;
    /* if the application validation has't been done before let's do it */
    if (true != tSemaphores.bAppValidDone)
    {
        if (false != ApiApplicationValidate())
        {
            (void)ApiDlSendNegativeResponse(responsePending);  /* send a wait */
            ApiInitTimer(app_valid_timer_handle,P2xCAN_TO);
        }
    }
    else
    {
        /* positive response - validation is ready */
        tSemaphores.bRcCpdi = false;
        response = ApiDlGetResponseBuffer();
        response[0] = startRoutine;
        response[1] = MSB(rcCheckProgramming);
        response[2] = LSB(rcCheckProgramming);
        response[3] = ApiGetSwIntegrityStatus();     /* programming integrity   */
        response[4] = ApiGetSwCompatibilityStatus(); /* programming consistency */
        (void)ApiDlSendPositiveResponse(response,5u);
    }
}


/******************************************************************************
*  function name | ApiDlRoutineControlCheckProgDepBg
*  content       | It is the background task of RoutineControl-CheckProgrammingDependencies.
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlRoutineControlCheckProgDepBg(void)     					/*兼容性*/
{
    response_type pu8Response;

    if (true != tSemaphores.bAppValidDone)
    {
        (void)ApiApplCompatibilityChk();
    }
    else /* ready */
    {
        pu8Response = ApiDlGetResponseBuffer();
        pu8Response[0] = startRoutine;
        pu8Response[1] = MSB(rcCheckProgrammingDependencies);
        pu8Response[2] = LSB(rcCheckProgrammingDependencies);
        pu8Response[3] = ApiGetSwCompatibilityStatus();
        (void)ApiDlSendPositiveResponse(pu8Response,4);

        tSemaphores.bRcCheckProgDep = false;
    }
}


/******************************************************************************
*  function name | ApiDlRoutineControlCheckProgDepAbort
*  content       | It is abort fuction of CheckProgrammingDependencies background task.
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
bool ApiDlRoutineControlCheckProgDepAbort(void)
{
    if (false == tSemaphores.bRcCheckProgDep)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/******************************************************************************
*  function name | ApiDlRoutineControlCheckProgIntBg
*  content       | It is the background task of RoutineControl-CheckProgrammingIntegrity.
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlRoutineControlCheckProgIntBg(void)                        /*完整性检查*/
{
    response_type pu8Response;

    if (true != tSemaphores.bAppIntegrityDone)
    {
        (void)ApiApplIntegrityChk();
    }
    else
    {
        pu8Response = ApiDlGetResponseBuffer();
        pu8Response[0] = startRoutine;
        pu8Response[1] = MSB(rcCheckProgrammingIntegrity);
        pu8Response[2] = LSB(rcCheckProgrammingIntegrity);
        pu8Response[3] = ApiGetSwIntegrityStatus();
        (void)ApiDlSendPositiveResponse(pu8Response,4);

        tSemaphores.bRcCheckProgInt = false;
    }
}


/******************************************************************************
*  function name | ApiDlRoutineControlCheckProgIntAbort
*  content       | It is abort fuction of CheckProgrammingIntegrity background task.
*  parameter     | void
*  return        | bool
*  notice        | 无
******************************************************************************/
bool ApiDlRoutineControlCheckProgIntAbort(void)
{
    if (false == tSemaphores.bRcCheckProgInt)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/******************************************************************************
*  function name | ApiDlRoutineControlEmAbort
*  content       | It is abort fuction of eraseMemory background task. Its purpose is to abort
*                  and initiate the background memory erase activities.
*  parameter     | void
*  return        | bool
*  notice        | 无
******************************************************************************/
bool ApiDlRoutineControlEmAbort(void)
{
	return true;
    /* when sbl_active semaphore is set abort the mem type we worked on, otherwise return true */
    if (false == tSemaphores.bSblActive)
    {
        return true;    /* non-volatile memory cannot be busy */
    }
    else
    {
        tSemaphores.bRcErase = false;
        return true;
    }
}


/******************************************************************************
*  function name | ApiDlRoutineControlCpdiAbort
*  content       | It is abort fuction of checksum background task. Its purpose is to abort
*                  and initiate the background CRC calculation activities.
*  parameter     | void
*  return        | bool
*  notice        | 无
******************************************************************************/
bool ApiDlRoutineControlCpdiAbort(void)
{
    tSemaphores.bRcCpdi = false;
    return true;
}


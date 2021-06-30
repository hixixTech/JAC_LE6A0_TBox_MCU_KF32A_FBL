/*****************************************************************************/
/* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    os_log.c                                                    */
/* Description:  RTOS的LOG功能模块                                           */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author           Description                */
/* 2018-09-10   V11.01.01.01.00  吴辰雨              新建基线                       */
/* 2020-08-13   V11.01.01.01.01  张建舟            新增Log上传MPU                   */
/*****************************************************************************/


/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "ecu_usart.h"
#include "os_log.h"

#include "spp_generated.h"
//#include "rtos_dcpdtask.h"

#ifdef PRQA_TEST
    #pragma PRQA_MESSAGES_OFF 5124
    #pragma PRQA_MESSAGES_OFF 0432
    #pragma PRQA_MESSAGES_OFF 0179
#endif

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


static LogCfg_t tLogCfg;

/*---------------------------------------------------------------------------*/
/* 内部变量定义                                                            */
/*---------------------------------------------------------------------------*/
static LogBuff_t 	tLogBuff;
static UINT8 		au8MpuLogBuff[LOG_MPU_MAX_LEN];
static UINT16 		u16MpuLogLen;

static void LogTimestamp(void);
static void LogExec(UINT8 u8Logtype, const CHAR * pcFormat, va_list ptArg);
static void LogWritebuff(CHAR buff[], UINT16 len);
static void LOG_MPULOG_Process(void);
/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 全局函数定义                                                              */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | ApiLogInit
*  content       | 
*  timing        | 
*  parameter     | void
*  return        | void
*  notice        | 
******************************************************************************/
void ApiLogInit(void)
{
    tLogCfg.u8LogState =  OS_LOG_DISABLE;
    memset(tLogBuff.data, 0, LOG_MAX_LEN);
    memset(au8MpuLogBuff, 0 ,LOG_MPU_MAX_LEN);
    tLogBuff.u16DataCnt = 0;
    tLogBuff.u16ReadPointer = 0;
    tLogBuff.u16WritePointer = 0;
    u16MpuLogLen = 0;
}


/******************************************************************************
*  function name | ApiLogEnable
*  content       | 
*  timing        | 
*  parameter     | void
*  return        | void
*  notice        | 
******************************************************************************/
void ApiLogEnable(void)
{
    tLogCfg.u8LogState = OS_LOG_ENABLE;
    
}

/******************************************************************************
*  function name | ApiLogDisable
*  content       | 
*  timing        | 
*  parameter     | void
*  return        | void
*  notice        | 
******************************************************************************/
void ApiLogDisable(void)
{
    tLogCfg.u8LogState = OS_LOG_DISABLE;
}

 /******************************************************************************
 *    function name | ecu_iod_log_timestamp
 *    content       | 输出OS的时间戳
 *    timing          | 
 *    parameter      | void
 *    return          | void
 *    notice          | 
 ******************************************************************************/
 static void LogTimestamp(void)
{
    UINT32 u32Timer = 0;
    UINT32 pu32Temp[4];
    CHAR acBuf[20];

    u32Timer = (UINT32)xTaskGetTickCount();
    pu32Temp[0] = u32Timer % 1000;
    u32Timer =  u32Timer / 1000;
    pu32Temp[1] = u32Timer % 60;
    u32Timer = u32Timer / 60;
    pu32Temp[2] = u32Timer % 60;
    u32Timer = u32Timer / 60;
    pu32Temp[3] = u32Timer;

    (void)sprintf(acBuf, "%02d:%02d:%02d.%03d: ",pu32Temp[3],pu32Temp[2],  \
                            pu32Temp[1],pu32Temp[0]);
    
    LogWritebuff(acBuf, (UINT16)strlen(acBuf));

}
/******************************************************************************
*  function name | LogExec
*  content       | 
*  timing        | 
*  parameter     | void
*  return        | void
*  notice        |  
******************************************************************************/
static void LogExec(UINT8 u8Logtype, const CHAR * pcFormat, va_list ptArg)
{
    CHAR acBuf[128];
    if ( pcFormat == NULL )
    {
        return;
    }
    LogTimestamp();
        
    switch(u8Logtype)
    {
        case _LOG_CRITICAL:
            LogWritebuff((CHAR*)"CRI: ", (UINT16)strlen("CRI: "));
            break;
        case _LOG_ERROR:
            LogWritebuff((CHAR*)"ERR: ", (UINT16)strlen("ERR: "));
            break;
        case _LOG_WARNING:
            LogWritebuff((CHAR*)"WRN: ", (UINT16)strlen("WRN: "));
            break;
        case _LOG_DEBUG:
            LogWritebuff((CHAR*)"DBG: ", (UINT16)strlen("DBG: "));
            break;
        case _LOG_ENTRY:
            LogWritebuff((CHAR*)"ENT: ", (UINT16)strlen("ENT: "));
            break;
        case _LOG_EXIT:
            LogWritebuff((CHAR*)"EXT: ", (UINT16)strlen("EXT: "));
            break;
        case _LOG_TRACE:
            LogWritebuff((CHAR*)"TRC: ", (UINT16)strlen("TRC: "));
            break;
        default:
            break;
    }
        
    (void)vsprintf(acBuf, pcFormat, ptArg);
    LogWritebuff(acBuf, (UINT16)strlen(acBuf));
    
    
}

static void LogWritebuff(CHAR buff[], UINT16 len)
{
    if (buff == NULL)
    {
        return;
    }
    if ( (len + tLogBuff.u16DataCnt) > LOG_MAX_LEN )
    {
        return;
    }

    if (( tLogBuff.u16ReadPointer <= tLogBuff.u16WritePointer) &&   \
           ( len > (LOG_MAX_LEN - tLogBuff.u16WritePointer) ) )
    {
       
        memcpy(tLogBuff.data + tLogBuff.u16WritePointer, buff, LOG_MAX_LEN - tLogBuff.u16WritePointer);
        memcpy(tLogBuff.data, buff + LOG_MAX_LEN - tLogBuff.u16WritePointer, len + tLogBuff.u16WritePointer - LOG_MAX_LEN);
        tLogBuff.u16WritePointer = (UINT16)(len  + tLogBuff.u16WritePointer - LOG_MAX_LEN);
 
    }
    else
    {
        memcpy(tLogBuff.data + tLogBuff.u16WritePointer, buff, len);
        tLogBuff.u16WritePointer = (UINT16)((tLogBuff.u16WritePointer + len) % LOG_MAX_LEN);
    }
    tLogBuff.u16DataCnt = (UINT16)((tLogBuff.u16DataCnt + len) % LOG_MAX_LEN);
    /* MPU LOG */
    if ( (len + u16MpuLogLen) >= LOG_MPU_MAX_LEN )
    {
        len = LOG_MPU_MAX_LEN  - u16MpuLogLen;
    }
    memcpy(au8MpuLogBuff+u16MpuLogLen, buff, len);
    u16MpuLogLen = u16MpuLogLen + len;

}


/******************************************************************************
*  function name | ApiLogPrint
*  content       | 
*  timing        | 
*  parameter     | void
*  return        | void
*  notice        | 
******************************************************************************/
void ApiLogPrint(UINT8 u8Logtype, const CHAR * pcFormat, ...)
{
    va_list ptArg; 

    if (tLogCfg.u8LogState == OS_LOG_DISABLE)
    {
        return;
    }
    va_start(ptArg, pcFormat);
    
    LogExec(u8Logtype, pcFormat, ptArg);
    
    va_end(ptArg);
}

/******************************************************************************
*  function name | ApiLogPrintExt
*  content       | 
*  timing        | 
*  parameter     | void
*  return        | void
*  notice        | 
******************************************************************************/
void ApiLogPrintExt(UINT8 u8Print, UINT8 u8Logtype, const CHAR* pcFormat, ...)
{
    va_list ptArg;
//    return;
    if ((tLogCfg.u8LogState == OS_LOG_DISABLE) \
            || (u8Print == OFF))
    {
        return;
    }
    va_start(ptArg, pcFormat);
    
    LogExec(u8Logtype, pcFormat, ptArg);
    
    va_end(ptArg);
}

void ApiLogProcess(void)
{
    UINT16 u16PrintCnt = 0;
    INT32 s32Ret = OK; 
    if ( tLogBuff.u16DataCnt == 0)
    {
        return;
    }
    if ( tLogBuff.u16DataCnt > LOG_BYTES_PER_CYCLE )
    {
        u16PrintCnt = LOG_BYTES_PER_CYCLE;
    }
    else
    {
        u16PrintCnt = tLogBuff.u16DataCnt;
    }
    if ( ( tLogBuff.u16ReadPointer >= tLogBuff.u16WritePointer ) &&      \
            ( (tLogBuff.u16ReadPointer + u16PrintCnt) > LOG_MAX_LEN ) )
    {
        s32Ret = ApiUartDebugSend((UINT8*)(tLogBuff.data + tLogBuff.u16ReadPointer),    \
                                            LOG_MAX_LEN - tLogBuff.u16ReadPointer);
        if (s32Ret == OK)
        {
            tLogBuff.u16DataCnt = (UINT16)(tLogBuff.u16DataCnt + tLogBuff.u16ReadPointer - LOG_MAX_LEN);
            tLogBuff.u16ReadPointer = 0;
        }

    }
    else
    {
        s32Ret = ApiUartDebugSend((UINT8*)(tLogBuff.data + tLogBuff.u16ReadPointer),    \
                                                u16PrintCnt);
        if (s32Ret == OK)
        {
            tLogBuff.u16DataCnt =  (UINT16)(tLogBuff.u16DataCnt - u16PrintCnt);
            tLogBuff.u16ReadPointer = (UINT16)((tLogBuff.u16ReadPointer + u16PrintCnt) % LOG_MAX_LEN);
        }

    }
//   LOG_MPULOG_Process();

}

/******************************************************************************
*  function name | LOG_MPULOG_Process
*  content       | 
*  timing        | 
*  parameter     | void
*  return        | 
*  notice        | 
******************************************************************************/
// static void LOG_MPULOG_Process(void)
// {
//    UINT16 u16PrintCnt = 0;
//    DcpdPackageToMpuMcuLog_t tDcpdMcuLog;
//    UINT16 u16CurLen = 0;
//    static UINT16 u16SppCnt = 0;
//    static BOOL bSppStarted = FALSE;
//    static UINT8 u8Index = 0;



//    if ( u16MpuLogLen !=  LOG_MPU_MAX_LEN )
//    {
//        return;
//    }

//    if ( ApiDcpdComIsReady() == FALSE)
//    {
//        u16SppCnt = 0;
//        bSppStarted = FALSE;
//    }

//    // Wait 1.5 seconds after SPP connect
//    if ( bSppStarted == FALSE )
//    {
//        if ( ApiDcpdComIsReady() == FALSE )
//        {
//            return;
//        }
//        else
//        {
//            u16SppCnt++;
//            if ( u16SppCnt == 150)
//            {
//                bSppStarted = TRUE;
//                u16SppCnt = 0;
//            }
//            else
//            {
//                return;
//            }
           
//        }
       
//    }

//    tDcpdMcuLog.tHeader.u8SrcSrvId = 0x00;
//    tDcpdMcuLog.tHeader.u8DstSrvId = 0x00;
//    tDcpdMcuLog.tHeader.u16EventId = (UINT16)SPP_USER_DCPD_MSG_TO_MPU_MCULOG_EVT;
//    memcpy(tDcpdMcuLog.strLogBuf,au8MpuLogBuff, LOG_MPU_MAX_LEN );
//    ApiDcpdSendEvent(SPP_USER_DCPD_MSG_TO_MPU_MCULOG_EVT, (UINT8 * )(&tDcpdMcuLog), sizeof(tDcpdMcuLog));
//    u16MpuLogLen = 0;
//    memset(au8MpuLogBuff , 0 , LOG_MPU_MAX_LEN);

// }


/******************************************************************************
*  function name | ApiLogIsEmpty
*  content       | 
*  timing        | 
*  parameter     | void
*  return        | TRUE/FALSE
*  notice        | 
******************************************************************************/
//BOOL ApiLogIsEmpty(void)
//{
//    BOOL bSendCompleted = ApiUartDBUGSendComplete();
//    if ((tLogBuff.u16DataCnt == 0) && (bSendCompleted == TRUE))
//    {
//        return TRUE;
//    }
//    else
//    {
//        return FALSE;
//    }
//}



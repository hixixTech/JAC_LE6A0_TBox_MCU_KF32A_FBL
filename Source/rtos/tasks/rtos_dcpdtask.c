/******************************************************************************
 * (C) Copyright 2020 YuChiZhiNeng-Automotive
 * FILE NAME:    rtos_dcpdtask.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   
 * BY:           
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

/*****************************************************************************
** #include 
*****************************************************************************/
#include "bsw_init.h"

#include "rtos_taskinit.h"



/*****************************************************************************
** #define
*****************************************************************************/
#define DCPD_FEED_PERIOD_MULTI (500/DCPD_TASK_PRD_MS)
#define Tconnect1       (180*1000/DCPD_TASK_PRD_MS)
#define Treconnect1     (25*1000/DCPD_TASK_PRD_MS)

/*****************************************************************************
** typedef
*****************************************************************************/


/*****************************************************************************
** global variable
*****************************************************************************/


/*****************************************************************************
** static variables
*****************************************************************************/
static BOOL bSppStarted = FALSE;
static BOOL bSppWakeupReq = FALSE;
static BOOL bARMRunning = FALSE;  /* TODO: Need to add 0x000B and 0x0006 Recv callback */

/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/





/******************************************************************************
*  function name | DcpdSPPStatePrint
*  content       | 当SPP状态发生切换时打印SPP状态
*  parameter     | 
*  return        | 
*  notice        | 
******************************************************************************/
static void DcpdSPPStatePrint(SppState_e eSppStateCur, SppState_e eSppStateLast)
{
    if ( eSppStateCur != eSppStateLast)
    {
        switch (eSppStateCur)
        {
            case SPP_STATE_INIT:
                ApiLogPrint(_LOG_TRACE, "SPP State is INIT\n");
                break;
            case SPP_STATE_WARNING:
                ApiLogPrint(_LOG_TRACE, "SPP State is WARNING\n");
                break;     
            case SPP_STATE_ERROR:
                ApiLogPrint(_LOG_TRACE, "SPP State is ERROR\n");
                break;
            case SPP_STATE_RUNNING:
                ApiLogPrint(_LOG_TRACE, "SPP State is RUNNING\n");
                break;
            case SPP_STATE_SLEEP:
                ApiLogPrint(_LOG_TRACE, "SPP State is SLEEP\n");
                break;
            case SPP_STATE_UNKNOW:
                ApiLogPrint(_LOG_TRACE, "SPP State is UNKNOW\n");
                break;
            default:
                break;
        }
    }
}


/******************************************************************************
*  function name | DcpdSPPExceptionHandle
*  content       | SPP状态的异常处理
*  parameter     | 
*  return        | 
*  notice        | 
******************************************************************************/
static void DcpdSPPExceptionHandle(SppState_e eSppStateCur)
{
    static UINT16 u16Reconnect = 0;
    static UINT16 u16Connnect = 0;
    static UINT16 u16Sleeptimeout = 0;
   /*  PmPowerModeStatus_e eCurPmd = ApiPmPowerModeGet(); */ /* by s811 */

    /* Timeout process */
    if ( eSppStateCur == SPP_STATE_INIT)
    {
        u16Connnect++;
        if ( u16Connnect > Tconnect1 )
        {
            //ApiFmReportError(DTC_9561_47);
            //if (Detect_Struct[LTE_USB_VBUS_RESULT].u8_CurResult == USB_DETECT_OPEN)
            //{
                //ApiPmResetMPU();
            //}

            u16Connnect = 0;
        }
    }
    else if ( eSppStateCur == SPP_STATE_ERROR )
    {
        if ( u16Reconnect > Treconnect1 )
        {
            bSppStarted = FALSE;
            u16Reconnect = 0;
            
        }
        else
        {
            u16Reconnect++;
        }
        u16Connnect = 0;
    }
    /* Avoid spp in sleep mode and PM in RUNNING */
    else if ( eSppStateCur == SPP_STATE_SLEEP ) 
    {
        if (0/*eCurPmd == PM_POWER_MODE_RUNNING*/ /*by s811*/ )
        {
            if ( u16Sleeptimeout > ( 30000 / DCPD_TASK_PRD_MS ))
            {
                bSppStarted = FALSE;
                u16Sleeptimeout = 0;
            }
            else
            {
                u16Sleeptimeout++;
            }        
            if  (bSppWakeupReq == TRUE)
            {
                bSppStarted = FALSE;
                bSppWakeupReq = FALSE;
            }
        }
        u16Connnect = 0;
    }
    else
    {
        u16Connnect = 0;
        u16Reconnect = 0;
        u16Sleeptimeout = 0;
    }

    if ( eSppStateCur == SPP_STATE_RUNNING )
    {
        //ApiFmClearError(DTC_9561_47);
    }
    
}
/******************************************************************************
*  function name | DcpdTimeout
*  content       | Dcpd超时处理
*  parameter     | ptMsg
*  return        | void
*  notice        | 无
******************************************************************************/
static void DcpdTimeout(void)
{    
    SppVersionEx_t tSppVersion;
    static UINT8 u8WdgMultiPeriod = 1;
    static SppState_e eLastSppState = SPP_STATE_UNKNOW;
    SppState_e eSppState;



    /* SPP Start */
    if (bSppStarted == FALSE)
    {
                (void)ApiSppGetVersionEx(&tSppVersion);
                ApiLogPrint(_LOG_TRACE, "SPP: SYSVER=%d.%d.%d, USRVER=%d.%d.%d, MD5=%s\n", tSppVersion.tSysVersion.s16Major, \
                   tSppVersion.tSysVersion.s16Minor, tSppVersion.tSysVersion.s16Patch, tSppVersion.tUserVersion.s16Major, \
                   tSppVersion.tUserVersion.s16Minor, tSppVersion.tUserVersion.s16Patch, tSppVersion.as8md5);
                (void)ApiSPPInit();

                /*-----回调函数注册-----*/
                #if 0/* wait add function */
                ApiDIAGEventCbRegister();
                ApiXCALLEventCbRegister();
                ApiBleSrvEventCallbackRegister();
				ApiRvcEventCbRegister();
				ApiBTCEventCbRegister();
				ApiIctEventCbRegister();
				//ApiIctHVEventCbRegister();
				ApiReceWifiEventCbRegister();
                #endif 

				#if(LIB_PM_ADD == TRUE)
    			(void)ApiPmRegisterEventListener();
				#else
				#endif
                (void)ApiSPPStart();
                
                bSppStarted = TRUE; 
    }
    else
    {
        
        //(void)ApiSPPMainTask();

    }

    if(DCPD_FEED_PERIOD_MULTI == u8WdgMultiPeriod)
    {
        u8WdgMultiPeriod = 1;

        /* ApiSysMgrSrvFeedWtd(DCPD_FEED_WTD); */ /* wdg by s811 */
    }
    else
    {
        u8WdgMultiPeriod++;
    }

    /* SPP State Print */
    eSppState = ApiSPPGetState();

    DcpdSPPStatePrint(eSppState, eLastSppState);
/*     if ( ApiPmBatyVolConsGet(INDEX_BATY_CONS_OPERA) == STATUS_BATY_CONS_ST_NORMAL )
    {
        DcpdSPPExceptionHandle(eSppState);
    } *//* by s811 */

    eLastSppState = eSppState;


    ApiLogProcess();
}

/*****************************************************************************
** function prototypes
*****************************************************************************/
/******************************************************************************
*  function name | ApiDcpdTaskWakeup
*  content       | 唤醒
*  parameter     | 
*  return        | TRUE/FALSE
*  notice        | 无
******************************************************************************/
BOOL ApiDcpdTaskWakeup(void)
{
    ApiLogEnable();
    bSppWakeupReq = TRUE;
    ApiLogPrint(0, "**DCPD WAKEUP**\n");
    return TRUE;
}

/******************************************************************************
*  function name | ApiDcpdTaskSleep
*  content       | 睡眠
*  parameter     | u8Mode:  0 --- Sleep Mode
*                           1 --- Listen Mode
*  return        | TRUE/FALSE
*  notice        | 无
******************************************************************************/
BOOL ApiDcpdTaskSleep(UINT8 u8Mode)
{
    u8Mode = u8Mode;
//    ApiLogDisable();
    return TRUE;
}

/******************************************************************************
*  function name | ApiDcpdSrvCanSleep
*  content       | DCPDTASK能否休眠
*  parameter     | 
*  return        | TRUE/FALSE
*  notice        | 无
******************************************************************************/
BOOL ApiDcpdSrvCanSleep(void)
{
    return TRUE;
}


/******************************************************************************
*  function name | ApiDcpdSrvCanSleep
*  content       | DCPDTASK是否已经休眠
*  parameter     | 
*  return        | TRUE/FALSE
*  notice        | 无
******************************************************************************/
BOOL ApiDcpdSrvHasSlept(void)
{
    SppState_e eSppState;
 //   BOOL bLogEmpty = ApiLogIsEmpty();
 //   eSppState = ApiSPPGetState();
 //   if ( (( eSppState == SPP_STATE_SLEEP) || ( eSppState == SPP_STATE_INIT) ) && ( bLogEmpty == TRUE ) )
 //   {
        return TRUE;
//    }
//    else
//    {
 //      return FALSE;
//    }
}
/******************************************************************************
*  function name | ApiDcpdComIsReady
*  content       | DCPD是否正常工作
*  parameter     | 
*  return        | TRUE/FALSE
*  notice        | wan模块在请求MPU的数据的时候，需要DCPD处于正常工作状态
******************************************************************************/
BOOL ApiDcpdComIsReady(void)
{
    SppState_e eSppState;
    eSppState = ApiSPPGetState();
    if ( eSppState == SPP_STATE_RUNNING)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }  
}

BOOL ApiDcpdSendEvent(UINT16 u16EventId, UINT8 * pu8Data, UINT16 u16DataLen)
{
    return ApiSPPAddEvent(u16EventId,pu8Data,u16DataLen);
}
void ApiDcpdSetListener(UINT16 u16EventId, void* pListener)
{
    ApiSPPSetListener(u16EventId, pListener);
}

void ApiDcpdSetAckListener(UINT16 u16EventId, void* pListener)
{
    ApiSPPSetAckListener(u16EventId, pListener);
}

/****************************************************************************/
/**
 * Function Name: DCPD_task
 * Description: none
 *
 * :   none
 * Return:  none
 * Author:  create this function
 ****************************************************************************/
void DCPD_task(void)
{
	
	static portTickType xLastWakeTime_dcpd;
	xLastWakeTime_dcpd=xTaskGetTickCount();
	ApiLogPrint(_LOG_DEBUG,"This is DCPD_task\n");

	RUNTASK
	{
		vTaskDelayUntil(&xLastWakeTime_dcpd,10);//task周期
		
        DcpdTimeout();
		
		
		
	}
	
}

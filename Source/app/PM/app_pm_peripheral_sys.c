/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_peripheral_sys.c
* Description:  PM模块
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  徐高腾        新建基线
* 2020-08-04   V11.01.01.01.00  徐高腾        一级休眠增加模组短暂唤醒
* 2020-08-05   V11.01.01.01.00  徐高腾        增加总线长时间OFF休眠超时功能
* 2020-08-11   V11.01.01.01.00  徐高腾        增加判断是否支持休眠唤醒控制引脚检测功能
* 2020-08-14   V11.01.01.01.00  徐高腾        增加控制引脚状态检测功能
* 2020-08-15   V11.01.01.01.00  徐高腾        电源控制失败时，可以休眠
* 2020-09-11   V11.01.01.01.00  徐高腾        增加模组运行状态检测功能
******************************************************************************/


/******************************************************************************
* 头文件
******************************************************************************/

#include "os_log.h"
#include "app_pm_peripheral_sys.h"
#include "app_pm_peripheral_sys_cfg.h"
#include "app_pm_mv.h"
#include "app_pm_drv.h"
#include "app_pm_lte_dcpd_com.h"

#ifdef DIAS_PM_MPU_ENABLE
#include "app_pm_mpu_dcpd_com.h"
#endif



#include "ecu_gpio.h"

#ifndef DIAS_PM_EOL

#endif


/******************************************************************************
* 宏定义
******************************************************************************/
#define DIAS_PM_MIN_WAIT_VOL_STEADY_TIMER           (30u / DIAS_PM_PERIOD_TIMER)      /* 电压稳定等待时间30ms */
#define DIAS_PM_RECIEVE_SLEEP_READY_TIMEOUT         (120000u / DIAS_PM_PERIOD_TIMER)  /* 接收是否可以休眠消息超时 */


/******************************************************************************
* 变量定义
******************************************************************************/
static uint32_t gu32PmPowerOnTimeWaitCount = 0;
static BOOL   gbPmPowerStatusRecoveryEnable = FALSE;
static BOOL   gbPmHardPowerOffAfterCtrlFail = FALSE;
BOOL          gbPmMonitorDisable = TRUE;
BOOL          gbMonitorType = FALSE;


/******************************************************************************
* 内部函数声明
******************************************************************************/
static DiasPmPowerCtrlType_e DiasPmGetPowerCtrlType(DiasPmPowerStatus_e ePrePowerStatus, DiasPmPowerStatus_e eNextPowerStatus);
static DiasPmPowerCtrlType_e DiasPmGetSysPowerCtrlType(DiasPmPowerInfo_t *ptSysPowerInfo);
static void DiasPmSysPowerCtrl(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static DiasPmPowerStatus_e DiasPmGetPowerState(BOOL bIsSysPowerOn, BOOL bIsSysWakeup);
static void DiasPmRestoreSleepCtrlStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptSysPowerInfo);
static BOOL DiasPmGetSquWaveStatus(DiasPmFeedbackPara_t *ptFeedbackPara);
static BOOL DiasPmGetFeedbackStatus(DiasPmFeedbackInfo_t *ptPowerFeedbackInfo);
static void DiasPmRefreshCurrPowerStatus(DiasPmPowerInfo_t *ptCurrSysPowerInfo, DiasPmPowerStatus_e eFeedbackPowerStatus);
static void DiasPmRefreshFeedbackPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static BOOL DiasPmPowerCtrlEndAsPowerStatusChanged(DiasPmPowerCtrlType_e ePowerCtrlType, DiasPmPowerStatus_e eNewPowerStatus);
static void DiasPmProcFeedbackTimeout(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static void DiasPmProcPowerUpWhenChangingPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static void DiasPmSysPowerCheck(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptSysPowerInfo);

#ifndef DIAS_PM_EOL
static void DiasPmProcessSysRunAbnormal(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static void DiasPmCheckSysRunAbnormal(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
#endif

static void DiasPmProcessAfterPowerCtrl(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static void DiasPmProcessAfterPowerCtrlFailed(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);

static void DiasPmProcSleepReadyStateReceiveTimeout(DiasPmSysType_e eSysType);

static void DiasPmCheckSysHeartBeat(void);

static void DiasPmPowerCtrlSaveLog(DiasPmPowerInfo_t *ptCurrSysPowerInfo);


/******************************************************************************
*  function name | DiasPmPeripheralSysInit
*  content       | 初始化
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmPeripheralSysInit(void)
{
    DiasPmMcuDrvInit();
    DiasPmModDrvInit();
}

/******************************************************************************
*  function name | DiasPmClearSleepFailNotifyInfo
*  content       | 清除休眠失败通信信息
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmClearSleepFailNotifyInfo(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo != NULL)
        {
            gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->eNotifyStatus = DIAS_PM_WAIT_SEND;
            gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->u8FailureCount = 0;
        }
    }
}

/******************************************************************************
*  function name | DiasPmPeripheralSysInitAfterWakeup
*  content       | 唤醒后初始化
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmPeripheralSysInitAfterWakeup(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo != NULL)
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->u16ForceStayLeftTime = 0;
        }
    }

    DiasPmClearSleepFailNotifyInfo();
}

/******************************************************************************
*  function name | DiasPmPeripheralSysMonitorInit
*  content       | 周边系统监控功能初始化
*  parameter[1]  | void
*  notice        | 无
*  return        | int32_t
******************************************************************************/
int32_t DiasPmPeripheralSysMonitorInit(void)
{
    uint8_t u8Loop;

#ifdef XA
    if (ApiNvramReadSyncInd(INDEX_MCU_MONITOR, 1, (uint8_t *)&gbMonitorType) == FALSE)
    {
        return ERR;
    }
#endif

    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if ((gatPmSysInfo[u8Loop].ptPowerInfo != NULL) &&
            (gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg != NULL))
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg->bCurrMonitorState = gbMonitorType;
        }
    }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:MonitorType:%d\n", gbMonitorType);
    return OK;
}

/******************************************************************************
*  function name | DiasPmSetPowerStatusRecoveryEnable
*  content       | 设置电源状态自恢复功能启动/关闭
*  parameter[1]  | BOOL bSupport:1(支持自恢复)/0(不支持自恢复)
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSetPowerStatusRecoveryEnable(BOOL bSupport)
{
    gbPmPowerStatusRecoveryEnable = bSupport;
}

/******************************************************************************
*  function name | DiasPmSetRecoveryAfterPowerCtrlFailEnable
*  content       | 设置电源失败自恢复功能启动/关闭
*  parameter[1]  | BOOL bSupport:1(支持自恢复)/0(不支持自恢复)
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSetRecoveryAfterPowerCtrlFailEnable(BOOL bSupport)
{
    gbPmHardPowerOffAfterCtrlFail = bSupport;
}

/******************************************************************************
*  function name | DiasPmProcSleepReadyStateReceiveTimeout
*  content       | 接收是否允许休眠消息超时及处理
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmProcSleepReadyStateReceiveTimeout(DiasPmSysType_e eSysType)
{
    if (gatPmSysInfo[eSysType].bHasService == FALSE)
    {
        return;
    }

    if ((gatPmSysInfo[eSysType].ptPowerInfo != NULL) &&
        (gatPmSysInfo[eSysType].ptPowerInfo->eFeedbackPowerStatus != DIAS_PM_STATUS_WAKE_UP))
    {
        gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer = 0;
        return;
    }

    if (gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer < DIAS_PM_RECIEVE_SLEEP_READY_TIMEOUT)
    {
        ++gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer;
    }
    else
    {
        gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer = 0;
        gatPmSysInfo[eSysType].bIsSleepReady = TRUE;
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:System %d notify sleep ready timeout.\n", eSysType);
    }
}

/******************************************************************************
*  function name | DiasPmProcSleepFailNotify
*  content       | 休眠失败通知
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmProcSleepFailNotify(DiasPmSysType_e eSysType)
{
    DiasPmPowerStatus_e   ePowerStatus;
    DiasPmMsgNotifyInfo_t *ptNotifyInfo;
    if (gatPmSysInfo[eSysType].ptSleepFailNotifyInfo == NULL)
    {
        return;
    }

    ptNotifyInfo = gatPmSysInfo[eSysType].ptSleepFailNotifyInfo;
    switch (ptNotifyInfo->eNotifyStatus)
    {
        case DIAS_PM_SENDING:
        {
            if (gatPmSysInfo[eSysType].ptPowerInfo != NULL)
            {
                ePowerStatus = ApiPmGetCurrSysPowerStatus(eSysType);
                if (ePowerStatus != DIAS_PM_STATUS_WAKE_UP)
                {
                    /* 正在进行电源控制或系统未处于工作状态，休眠失败暂不通知 */
                    return;
                }
            }

            if ((ptNotifyInfo->msgNotifyFunc == NULL) ||
                (ptNotifyInfo->u8FailureCount >= ptNotifyInfo->u8MaxFailureCount))
            {
                ptNotifyInfo->eNotifyStatus = DIAS_PM_NOTIFY_FAILURE;
            }
            else
            {
                (void)ptNotifyInfo->msgNotifyFunc();
                ptNotifyInfo->u16WaitTime = 0;
                ptNotifyInfo->eNotifyStatus = DIAS_PM_WAIT_ACK;
            }
            break;
        }
        case DIAS_PM_WAIT_ACK:
        {
            if (ptNotifyInfo->u16WaitTime < ptNotifyInfo->u16SendTimeout)
            {
                ++ptNotifyInfo->u16WaitTime;
            }
            else
            {
                ++ptNotifyInfo->u8FailureCount;
                ptNotifyInfo->eNotifyStatus = DIAS_PM_SENDING;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

/******************************************************************************
*  function name | DiasPmPeripheralSysTimeout
*  content       | 周期处理函数
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmPeripheralSysTimeout(void)
{
    uint8_t u8Loop;
    DiasPmPowerInfo_t *ptCurrSysPowerInfo;
    DiasPmSysType_e eSysType;

    /* 刚上电，需电压稳定再控制开机，等待DIAS_PM_MIN_WAIT_VOL_STEADY_TIMER */
    if (gu32PmPowerOnTimeWaitCount < DIAS_PM_MIN_WAIT_VOL_STEADY_TIMER)
    {
        ++gu32PmPowerOnTimeWaitCount;
        return;
    }

    DiasPmCheckSysHeartBeat();

    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        eSysType = (DiasPmSysType_e)u8Loop;

        DiasPmProcSleepReadyStateReceiveTimeout(eSysType);
        DiasPmProcSleepFailNotify(eSysType);

        if (gatPmSysInfo[u8Loop].ptPowerInfo == NULL)
        {
            continue;
        }

        /* 刷新电源状态反馈(重启时电源状态反馈需等待控制超时后再检测) */
        ptCurrSysPowerInfo = gatPmSysInfo[u8Loop].ptPowerInfo;
        if (ptCurrSysPowerInfo->u16ForceStayLeftTime > 0)
        {
            --ptCurrSysPowerInfo->u16ForceStayLeftTime;
        }

        DiasPmRefreshFeedbackPowerStatus(eSysType, ptCurrSysPowerInfo);

        if ((ptCurrSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_NONE) &&
            (ptCurrSysPowerInfo->bPowerStatusConfirm == FALSE))
        {
            DiasPmSysPowerCheck(eSysType, ptCurrSysPowerInfo);
        }

        /* 电源状态机控制 */
        DiasPmSysPowerCtrl(eSysType, ptCurrSysPowerInfo);
    }
}

void DiasPmMonitorEnable(void)
{
    DiasPmSysMonitorCfg_t *ptMonitorCfg;
    uint8_t u8Loop;

    gbPmMonitorDisable = FALSE;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if ((gatPmSysInfo[u8Loop].ptPowerInfo != NULL) &&
            (gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg != NULL))
        {
            ptMonitorCfg = gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg;
            ptMonitorCfg->u32HeartbeatDeadTimer = ptMonitorCfg->u32InitMonitorPeriod;
            if (ptMonitorCfg->ptFeedbackInfo != NULL)
            {
                ptMonitorCfg->ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft = ptMonitorCfg->ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTime;
            }

            ptMonitorCfg->bCurrMonitorState = gbMonitorType;
        }
    }
}

void DiasPmMonitorDisable(void)
{
    gbPmMonitorDisable = TRUE;
}

/******************************************************************************
*  function name | DiasPmGetSysPowerCtrlType
*  content       | 获取指定电源的控制类型
*  parameter[1]  | DiasPmPowerInfo_t *ptSysPowerInfo：电源信息
*  notice        | 无
*  return        | DiasPmPowerCtrlType_e：DIAS_PM_CTRL_NONE(不控制)
*                |                        DIAS_PM_CTRL_POWER_ON(控制开机)
*                |                        DIAS_PM_CTRL_POWER_OFF(控制关机)
*                |                        DIAS_PM_CTRL_SLEEP(控制休眠)
*                |                        DIAS_PM_CTRL_WAKE_UP(控制唤醒)
*                |                        DIAS_PM_CTRL_RESET(控制重启)
******************************************************************************/
static DiasPmPowerCtrlType_e DiasPmGetSysPowerCtrlType(DiasPmPowerInfo_t *ptSysPowerInfo)
{
    if (ptSysPowerInfo->bPowerStatusConfirm == TRUE)
    {
        return DIAS_PM_CTRL_NONE;
    }

    if (ptSysPowerInfo->u16ForceStayLeftTime > 0)
    {
        return DIAS_PM_CTRL_NONE;
    }

    return DiasPmGetPowerCtrlType(ptSysPowerInfo->eCurrPowerStatus, ptSysPowerInfo->eExpectPowerStatus);
}

/******************************************************************************
*  function name | DiasPmSysPowerCheck
*  content       | 系统电源状态异常检测及修复
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*                | DiasPmPowerInfo_t *ptSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmSysPowerCheck(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptSysPowerInfo)
{
    if (gbPmPowerStatusRecoveryEnable == TRUE)
    {
        ptSysPowerInfo->ePowerCtrlType = DiasPmGetSysPowerCtrlType(ptSysPowerInfo);
    }

#ifndef DIAS_PM_EOL
    if (ApiPmBatyVolConsGet(INDEX_BATY_CONS_POWER_CTRL) != STATUS_BATY_CONS_ST_NORMAL)
    {
        return;
    }

    if (ptSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_NONE)
    {
        if (ptSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_WAKE_UP)
        {
            /* 只在DIAS_PM_STATUS_WAKE_UP下检测系统运行状况 */
            if ((gbPmMonitorDisable == FALSE) &&
                (ptSysPowerInfo->ptMonitorCfg != NULL))
            {
                DiasPmCheckSysRunAbnormal(eSysType, ptSysPowerInfo);
            }
        }
        else if (ptSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_RUN_ABMORMAL)
        {
            DiasPmProcessSysRunAbnormal(eSysType, ptSysPowerInfo);
        }
        else
        {
            /* do nothing */
        }
    }
#endif
}

#ifndef DIAS_PM_EOL
/******************************************************************************
*  function name | DiasPmCheckSysRunAbnormal
*  content       | 检查系统运行状况
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmCheckSysRunAbnormal(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    DiasPmSysMonitorCfg_t *ptCurrSysMonitorCfg;
    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    ptCurrSysMonitorCfg = ptCurrSysPowerInfo->ptMonitorCfg;
    if (ptCurrSysMonitorCfg->bCurrMonitorState != TRUE)
    {
        /* 系统不需要监控 */
        return;
    }

    if (ptCurrSysMonitorCfg->u32HeartbeatDeadTimer > 0u)
    {
        --ptCurrSysMonitorCfg->u32HeartbeatDeadTimer;
    }
    else
    {
        ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_RUN_ABMORMAL;
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:System %d is abnormal\n", eSysType);
    }
}


/******************************************************************************
*  function name | DiasPmProcessSysRunAbnormal
*  content       | 处理系统运行异常
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | VOID
******************************************************************************/
static void DiasPmProcessSysRunAbnormal(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    uint32_t u32CurrTime;
    DiasPmSysMonitorCfg_t *ptCurrSysMonitorCfg;
    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    if ((ptCurrSysPowerInfo->ptMonitorCfg == NULL) ||
        (ptCurrSysPowerInfo->ptMonitorCfg->procSysAbnormalFunc == NULL))
    {
        ptCurrSysPowerInfo->eCurrPowerStatus = ptCurrSysPowerInfo->eFeedbackPowerStatus;
        return;
    }

    ptCurrSysMonitorCfg = ptCurrSysPowerInfo->ptMonitorCfg;
    if (ptCurrSysMonitorCfg->bSysFault == TRUE)
    {
        /* 系统故障后，不再处理异常 */
        return;
    }

    if (ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex >= DIAS_PM_MAX_SYS_ABNORMAL_COUNT)
    {
        ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex = 0;
    }

    u32CurrTime = 0;//ApiAuxTaskGetSysTimeUtc();
    if ((ptCurrSysMonitorCfg->au32SysAbnormalTime[ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex] != 0) &&
        (u32CurrTime <= (ptCurrSysMonitorCfg->au32SysAbnormalTime[ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex] + ptCurrSysMonitorCfg->u32SysAbnormalIgnorePeriod)))
    {
        /* 系统异常处理无法修复，确认为系统故障 */
        ptCurrSysMonitorCfg->bSysFault = TRUE;
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:System %d is fault.\n", eSysType);
        return;
    }

    if (ptCurrSysMonitorCfg->procSysAbnormalFunc() == OK)
    {
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_DEBUG, "PM:Repair system %d as abnormal.\n", eSysType);
        ptCurrSysMonitorCfg->au32SysAbnormalTime[ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex] = u32CurrTime;
        ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex += 1u;
        ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex %= DIAS_PM_MAX_SYS_ABNORMAL_COUNT;
    }
}
#endif

/******************************************************************************
*  function name | DiasPmGetSquWaveStatus
*  content       | 方波状态反馈
*  parameter[1]  | DiasPmFeedbackPara_t tFeedbackPara：反馈参数
*  notice        | 无
*  return        | BOOL：TRUE(正常)/FALSE(异常)
******************************************************************************/
static BOOL DiasPmGetSquWaveStatus(DiasPmFeedbackPara_t *ptFeedbackPara)
{
    uint8_t u8PinStatus = (UINT8)ApiGpioInputRead(ptFeedbackPara->tSquWavePara.u8Pin);
    if (u8PinStatus != ptFeedbackPara->tSquWavePara.u8PinStatus)
    {
        ptFeedbackPara->tSquWavePara.u8PinStatus = u8PinStatus;
        ptFeedbackPara->tSquWavePara.u16KeepTime = 0;
        return TRUE;
    }

    if ((u8PinStatus == 0) ||
        (u8PinStatus == 1))
    {
        if (ptFeedbackPara->tSquWavePara.u16KeepTime < ptFeedbackPara->tSquWavePara.u16MaxKeepTime)
        {
            ++ptFeedbackPara->tSquWavePara.u16KeepTime;
            return TRUE;
        }
    }
    else
    {
        /* do nothing */
    }

    return FALSE;
}

/******************************************************************************
*  function name | DiasPmGetFeedbackStatus
*  content       | 获取状态反馈
*  parameter[1]  | DiasPmFeedbackInfo_t *ptPowerFeedbackInfo：电源反馈信息
*  notice        | 无
*  return        | BOOL：TRUE(工作)/FALSE(不工作)
******************************************************************************/
static BOOL DiasPmGetFeedbackStatus(DiasPmFeedbackInfo_t *ptPowerFeedbackInfo)
{
    switch (ptPowerFeedbackInfo->eFeedbackMode)
    {
        case DIAS_PM_FEEDBACK_MODE_PIN_HIGH:
        {
            if (ApiGpioInputRead(ptPowerFeedbackInfo->tFeedbackPara.tPinPara.u8Pin) == 1)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        case DIAS_PM_FEEDBACK_MODE_PIN_LOW:
        {
            if (ApiGpioInputRead(ptPowerFeedbackInfo->tFeedbackPara.tPinPara.u8Pin) == 0)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        case DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_HIGH:
        {
            if (ApiGpioOutputRead(ptPowerFeedbackInfo->tFeedbackPara.tPinPara.u8Pin) == 1)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        case DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_LOW:
        {
            if (ApiGpioOutputRead(ptPowerFeedbackInfo->tFeedbackPara.tPinPara.u8Pin) == 0)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        case DIAS_PM_FEEDBACK_MODE_ADC:
        {
	    /*
            if (Detect_Struct[ptPowerFeedbackInfo->tFeedbackPara.tChannelPara.u8Channel].u8_CurResult == ON)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }*/
	    return TRUE;
        }
        case DIAS_PM_FEEDBACK_MODE_SQU_WAKE:
        {
            return DiasPmGetSquWaveStatus(&(ptPowerFeedbackInfo->tFeedbackPara));
        }
        default:
        {
            break;
        }
    }

    return ptPowerFeedbackInfo->bFeedbackStatus;
}

/******************************************************************************
*  function name | DiasPmRefreshFeedbackPowerStatus
*  content       | 修复休眠唤醒控制状态
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*                | DiasPmPowerInfo_t *ptSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmRestoreSleepCtrlStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptSysPowerInfo)
{
    if (ptSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_CTRL].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
    {
        return;
    }

    if ((ptSysPowerInfo->u16ForceStayLeftTime > 0) ||
        (ptSysPowerInfo->bPowerStatusConfirm == TRUE))
    {
        return;
    }

    if (DiasPmIsPeripheralSysPowerCtrlFinished(eSysType) != TRUE)
    {
        return;
    }

    if (ptSysPowerInfo->eExpectPowerStatus == DIAS_PM_STATUS_WAKE_UP)
    {
        if (ptSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_CTRL].bFeedbackStatus != TRUE)
        {
            ptSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_WAKE_UP;
            ptSysPowerInfo->u8CtrlFailCount = 0;  /* 电源控制失败次数清零 */
        }
    }
    else if (ptSysPowerInfo->eExpectPowerStatus == DIAS_PM_STATUS_SLEEP)
    {
        if (ptSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_CTRL].bFeedbackStatus == TRUE)
        {
            ptSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_SLEEP;
            ptSysPowerInfo->u8CtrlFailCount = 0;  /* 电源控制失败次数清零 */
        }
    }
    else
    {
        /* do nothing */
    }
}

/******************************************************************************
*  function name | DiasPmRefreshCurrPowerStatus
*  content       | 刷新当前电源状态
*  parameter[1]  | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*                | DiasPmPowerStatus_e eFeedbackPowerStatus：新电源状态
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmRefreshCurrPowerStatus(DiasPmPowerInfo_t *ptCurrSysPowerInfo, DiasPmPowerStatus_e eFeedbackPowerStatus)
{
    if ((ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_WAKE_UP) &&
        (ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_UNSTABLE) &&
        (ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_SLEEP) &&
        (ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_POWER_OFF))
    {
        /* 异常电源状态需要经过电源控制修复，电源控制过程中电源状态为DIAS_PM_STATUS_UNSTABLE */
        return;
    }

    if (ptCurrSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_RESET)
    {
        /* 重启控制电源状态等待重启完成再更新 */
        return;
    }

    if (DiasPmPowerCtrlEndAsPowerStatusChanged(ptCurrSysPowerInfo->ePowerCtrlType, eFeedbackPowerStatus) == TRUE)
    {
        ptCurrSysPowerInfo->eCurrPowerStatus = eFeedbackPowerStatus;
    }
}

/******************************************************************************
*  function name | DiasPmRefreshFeedbackPowerStatus
*  content       | 刷新电源状态反馈
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmRefreshFeedbackPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    DiasPmPowerStatus_e eNewFeedbackPowerStatus;
    DiasPmFeedbackInfo_t *ptPowerFeedbackInfo;
    uint8_t u8FeedbackTypeNum = (uint8_t)DIAS_PM_FEEDBACK_TYPE_END;
    uint8_t u8Loop = 0;
    BOOL bPowerStatusConfirm = FALSE;
    BOOL bFeedbackStatus;
    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    for (; u8Loop < u8FeedbackTypeNum; ++u8Loop)
    {
        ptPowerFeedbackInfo = ptCurrSysPowerInfo->atPowerFeedbackInfo + u8Loop;
        bFeedbackStatus = DiasPmGetFeedbackStatus(ptPowerFeedbackInfo);
        if (bFeedbackStatus == ptPowerFeedbackInfo->bFeedbackStatus)
        {
            ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft = ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTime;
            continue;
        }

        if (ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft > 0)
        {
            --ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft;
            bPowerStatusConfirm = TRUE;
            continue;
        }

        if (bFeedbackStatus == TRUE)
        {
            if (ptCurrSysPowerInfo->u16ForceStayLeftTime < ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16WorkMinStay)
            {
                ptCurrSysPowerInfo->u16ForceStayLeftTime = ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16WorkMinStay;
            }
        }
        else
        {
            if (ptCurrSysPowerInfo->u16ForceStayLeftTime < ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16OutWorkMinStay)
            {
                ptCurrSysPowerInfo->u16ForceStayLeftTime = ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16OutWorkMinStay;
            }
        }

        ptPowerFeedbackInfo->bFeedbackStatus = bFeedbackStatus;
    }

    ptCurrSysPowerInfo->bPowerStatusConfirm = bPowerStatusConfirm;

    /* 只有电源状态反馈发生变化，才刷新当前电源状态 */
    eNewFeedbackPowerStatus = DiasPmGetPowerState(ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].bFeedbackStatus,
                                               ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].bFeedbackStatus);
    if (eNewFeedbackPowerStatus != ptCurrSysPowerInfo->eFeedbackPowerStatus)
    {
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Power status changed.sys:%d,type:%d,sta:%d\n", eSysType, ptCurrSysPowerInfo->ePowerCtrlType, eNewFeedbackPowerStatus);
        ptCurrSysPowerInfo->eFeedbackPowerStatus = eNewFeedbackPowerStatus;
    }

    DiasPmRefreshCurrPowerStatus(ptCurrSysPowerInfo, eNewFeedbackPowerStatus);

    DiasPmRestoreSleepCtrlStatus(eSysType, ptCurrSysPowerInfo);
}

/******************************************************************************
*  function name | DiasPmPowerCtrlEndAsPowerStatusChanged
*  content       | 电源状态变化反馈电源控制成功
*                | DiasPmPowerCtrlType_e ePowerCtrlType：电源控制类型
*                | DiasPmPowerStatus_e eNewPowerStatus：新电源状态
*  notice        | 无
*  return        | BOOL
******************************************************************************/
static BOOL DiasPmPowerCtrlEndAsPowerStatusChanged(DiasPmPowerCtrlType_e ePowerCtrlType, DiasPmPowerStatus_e eNewPowerStatus)
{
    switch (ePowerCtrlType)
    {
        case DIAS_PM_CTRL_NONE:
        {
            return TRUE;
        }
        case DIAS_PM_CTRL_POWER_OFF:
        case DIAS_PM_CTRL_HARD_POWER_OFF:
        {
            if (eNewPowerStatus == DIAS_PM_STATUS_POWER_OFF)
            {
                return TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_POWER_ON:
        {
            if (eNewPowerStatus == DIAS_PM_STATUS_WAKE_UP)
            {
                return TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_WAKE_UP:
        {
            if (eNewPowerStatus == DIAS_PM_STATUS_WAKE_UP)
            {
                return TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_SLEEP:
        {
            if (eNewPowerStatus == DIAS_PM_STATUS_SLEEP)
            {
                return TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_RESET:
        {
            return TRUE;
        }
        default:
            break;
    }

    return FALSE;
}

/******************************************************************************
*  function name | DiasPmProcFeedbackTimeout
*  content       | 电源状态反馈超时处理
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmProcFeedbackTimeout(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    DiasPmPowerStatus_e eNewFeedbackPowerStatus;

    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    switch (ptCurrSysPowerInfo->ePowerCtrlType)
    {
        case DIAS_PM_CTRL_POWER_OFF:
        {
            if (ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
            {
                /* 不支持开关机状态反馈，则认为关机成功 */
                ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].bFeedbackStatus = FALSE;
            }
            break;
        }
        case DIAS_PM_CTRL_POWER_ON:
        {
            if (ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
            {
                /* 不支持开关机状态反馈，则认为开机成功 */
                ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].bFeedbackStatus = TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_WAKE_UP:
        {
            if (ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
            {
                /* 不支持休眠唤醒状态反馈，则认为唤醒成功 */
                ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].bFeedbackStatus = TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_SLEEP:
        {
            if (ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
            {
                /* 不支持休眠唤醒状态反馈，则认为休眠成功 */
                ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].bFeedbackStatus = FALSE;
            }
            break;
        }
        default:
            break;
    }

    eNewFeedbackPowerStatus = DiasPmGetPowerState(ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].bFeedbackStatus,
                                               ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].bFeedbackStatus);
    ApiLogPrint(_LOG_DEBUG,"PM:TIMEOUT eSysType:%d  %d %d",eSysType,eNewFeedbackPowerStatus,ptCurrSysPowerInfo->eFeedbackPowerStatus);
    ptCurrSysPowerInfo->eFeedbackPowerStatus = eNewFeedbackPowerStatus;
    ptCurrSysPowerInfo->eCurrPowerStatus = ptCurrSysPowerInfo->eFeedbackPowerStatus;
}

/******************************************************************************
*  function name | DiasPmGetPowerState
*  content       | 根据开关机状态和休眠状态获取电源状态
*  parameter[1]  | BOOL bIsSysPowerOn：是否处于开机状态
*                | BOOL bIsSysWakeup ：是否处于唤醒状态
*  notice        | 无
*  return        | DiasPmPowerStatus_e：DIAS_PM_STATUS_POWER_OFF(关机)
*                |                      DIAS_PM_STATUS_SLEEP(休眠)
*                |                      DIAS_PM_STATUS_WAKE_UP(唤醒)
******************************************************************************/
static DiasPmPowerStatus_e DiasPmGetPowerState(BOOL bIsSysPowerOn, BOOL bIsSysWakeup)
{
    if (bIsSysPowerOn == FALSE)
    {
        return DIAS_PM_STATUS_POWER_OFF;
    }

    if (bIsSysWakeup != TRUE)
    {
        return DIAS_PM_STATUS_SLEEP;
    }

    return DIAS_PM_STATUS_WAKE_UP;
}

/******************************************************************************
*  function name | DiasPmPowerCtrlSaveLog
*  content       | 存储电源控制Log
*  parameter[1]  | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmPowerCtrlSaveLog(DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
}

/******************************************************************************
*  function name | DiasPmSysPowerCtrl
*  content       | 电源控制处理
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmSysPowerCtrl(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    const DiasPmPowerCtrlCfg_t *ptPowerCtrlCfg;

    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    ptPowerCtrlCfg = ptCurrSysPowerInfo->atPowerCtrlCfg + (uint8_t)ptCurrSysPowerInfo->ePowerCtrlType;

    do
    {
        ptCurrSysPowerInfo->ePrePowerCtrlStatus = ptCurrSysPowerInfo->ePowerCtrlStatus;
        switch (ptCurrSysPowerInfo->ePowerCtrlStatus)
        {
            case DIAS_PM_CTRL_IDLE:
            {
                if (ptCurrSysPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
                {
                    if (ptPowerCtrlCfg->ptCtrlStepCfg == NULL)
                    {
                        ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_NONE;
                        ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_POWER_CTRL_FAULT;
                        return;
                    }
                    else
                    {
                        if (ptCurrSysPowerInfo->bPowerStatusConfirm == TRUE)
                        {
                            ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_NONE;
                            return;
                        }
                    }

                    ptCurrSysPowerInfo->u8CurrStep = 0;
                    ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_STRAT;
                    ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_UNSTABLE;   /* 控制开始后电源状态为DIAS_PM_STATUS_UNSTABLE，直到电源控制成功或超时才再次更新 */
                    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:PowerCtrl start.sys:%d,type:%d.\n", eSysType, ptCurrSysPowerInfo->ePowerCtrlType);
                }
                break;
            }
            case DIAS_PM_CTRL_STRAT:
            {
                if (ptPowerCtrlCfg->ptCtrlStepCfg[ptCurrSysPowerInfo->u8CurrStep].ctrlFunc != NULL)
                {
                    if (ptPowerCtrlCfg->ptCtrlStepCfg[ptCurrSysPowerInfo->u8CurrStep].ctrlFunc(ptPowerCtrlCfg->ptCtrlStepCfg[ptCurrSysPowerInfo->u8CurrStep].u8Para) != OK)
                    {
                        return;
                    }

                    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_DEBUG, "PM:PowerCtrl.sys:%d,type:%d,step:%d.\n", eSysType, ptCurrSysPowerInfo->ePowerCtrlType, ptCurrSysPowerInfo->u8CurrStep);
                }
                ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_KEEP;
                break;
            }
            case DIAS_PM_CTRL_KEEP:
            {
                if (ptCurrSysPowerInfo->u32CtrlTimerCount < ptPowerCtrlCfg->ptCtrlStepCfg[ptCurrSysPowerInfo->u8CurrStep].u32CtrlTime)
                {
                    ++ptCurrSysPowerInfo->u32CtrlTimerCount;
                }
                else
                {
                    ptCurrSysPowerInfo->u32CtrlTimerCount = 0u;
                    ++ptCurrSysPowerInfo->u8CurrStep;
                    if (ptCurrSysPowerInfo->u8CurrStep < ptPowerCtrlCfg->u8CtrlStepNum)
                    {
                        ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_STRAT;
                    }
                    else
                    {
                        ptCurrSysPowerInfo->u8CurrStep = 0;
                        ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_FEEDBACK;
                        ptCurrSysPowerInfo->bPowerStatusConfirm = TRUE;
                    }
                }
                break;
            }
            case DIAS_PM_CTRL_FEEDBACK:
            {
                if (ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_UNSTABLE)
                {
                    /* 收到反馈结果，控制结束 */
                    ptCurrSysPowerInfo->u32FeedbackTimerCount = 0u;
                    DiasPmProcessAfterPowerCtrl(eSysType, ptCurrSysPowerInfo);
                    ptCurrSysPowerInfo->ePrePowerCtrlStatus = DIAS_PM_CTRL_IDLE;
                    ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_IDLE;
                    ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_NONE;
                }
                else if (ptCurrSysPowerInfo->u32FeedbackTimerCount < ptPowerCtrlCfg->u16MaxFeedbackTime)
                {
                    ++ptCurrSysPowerInfo->u32FeedbackTimerCount;
                }
                else
                {
                    /* 不支持电源状态反馈或电源控制失败 */
                    ptCurrSysPowerInfo->u32FeedbackTimerCount = 0u;
                    ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_TIMEOUT;
                    ApiLogPrint(_LOG_DEBUG,"PM:FeedBack Timeout %d\n",eSysType);
                }
                break;
            }
            default:
            {
                DiasPmProcFeedbackTimeout(eSysType, ptCurrSysPowerInfo);
                DiasPmProcessAfterPowerCtrl(eSysType, ptCurrSysPowerInfo);
                ptCurrSysPowerInfo->ePrePowerCtrlStatus = DIAS_PM_CTRL_IDLE;
                ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_IDLE;
                ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_NONE;
                break;
            }
        }
    } while(ptCurrSysPowerInfo->ePrePowerCtrlStatus != ptCurrSysPowerInfo->ePowerCtrlStatus);
}

/******************************************************************************
*  function name | DiasPmProcessAfterPowerCtrlFailed
*  content       | 电源控制失败后处理
*  parameter[1]  | DiasPmSysType_e eSysType        ：系统类型
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmProcessAfterPowerCtrlFailed(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:PowerCtrl fail.sys:%d,type:%d.\n", eSysType, ptCurrSysPowerInfo->ePowerCtrlType);
    if (ptCurrSysPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_HARD_POWER_OFF)
    {
        ++ptCurrSysPowerInfo->u8CtrlFailCount;
        if (ptCurrSysPowerInfo->u8CtrlFailCount < DIAS_PM_MAX_POWER_CTRL_TIMES)
        {
            return;
        }

        ptCurrSysPowerInfo->u8CtrlFailCount = 0;
        if ((gbPmHardPowerOffAfterCtrlFail == TRUE) &&
            (ptCurrSysPowerInfo->atPowerCtrlCfg[DIAS_PM_CTRL_HARD_POWER_OFF].ptCtrlStepCfg != NULL))
        {
            /* 电源控制失败次数超过上限，系统可能死机，执行硬关机处理 */
            ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_POWER_CTRL_FAILURE;
            if (ptCurrSysPowerInfo->eExpectPowerStatus == DIAS_PM_STATUS_SLEEP)
            {
                /* 休眠失败后改为控制硬关机，若关机成功，无需再控制为休眠状态 */
                ptCurrSysPowerInfo->eExpectPowerStatus = DIAS_PM_STATUS_POWER_OFF;
            }

            return;
        }
    }

    ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_POWER_CTRL_FAULT;
}

/******************************************************************************
*  function name | DiasPmProcessAfterPowerCtrl
*  content       | 电源控制完成后处理
*  parameter[1]  | DiasPmSysType_e eSysType        ：系统类型
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmProcessAfterPowerCtrl(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    if (DiasPmPowerCtrlEndAsPowerStatusChanged(ptCurrSysPowerInfo->ePowerCtrlType, ptCurrSysPowerInfo->eCurrPowerStatus) == TRUE)
    {
        ptCurrSysPowerInfo->u8CtrlFailCount = 0;
        if (ptCurrSysPowerInfo->ptMonitorCfg != NULL)
        {
            ptCurrSysPowerInfo->ptMonitorCfg->u32HeartbeatDeadTimer = ptCurrSysPowerInfo->ptMonitorCfg->u32InitMonitorPeriod;
            if (ptCurrSysPowerInfo->ptMonitorCfg->ptFeedbackInfo != NULL)
            {
                ptCurrSysPowerInfo->ptMonitorCfg->ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft =
                    ptCurrSysPowerInfo->ptMonitorCfg->ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTime;
            }
            
            if (ptCurrSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_POWER_OFF)
            {
                ptCurrSysPowerInfo->ptMonitorCfg->bSysFault = FALSE;
                (void)memset(ptCurrSysPowerInfo->ptMonitorCfg->au32SysAbnormalTime, 0, sizeof(ptCurrSysPowerInfo->ptMonitorCfg->au32SysAbnormalTime));
                ptCurrSysPowerInfo->ptMonitorCfg->u8NextAbnormalTimeIndex = 0;
            }
        }

        gatPmSysInfo[eSysType].bIsSleepReady = gatPmSysInfo[eSysType].bIsSleepReadyDefault;
        gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer = 0;
        gatPmSysInfo[eSysType].u32NoServiceStartTime = 0;
    }
    else
    {
        DiasPmProcessAfterPowerCtrlFailed(eSysType, ptCurrSysPowerInfo);
    }

    DiasPmPowerCtrlSaveLog(ptCurrSysPowerInfo);
}

/******************************************************************************
*  function name | DiasPmChangeSysPowerStatus
*  content       | 修改系统最终的电源状态
*  parameter[1]  | DiasPmSysType_e eSysType        ：系统类型
*                | DiasPmPowerStatus_e ePowerStatus：电源状态
*  notice        | 无
*  return        | int32_t
******************************************************************************/
int32_t DiasPmChangeSysPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerStatus_e ePowerStatus)
{
    DiasPmPowerInfo_t *ptCurrSysPowerInfo;
    if (eSysType >= DIAS_PM_SYS_TYPE_END)
    {
        return ERR;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo == NULL)
    {
        return OK;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->aPeripheralUintCtrlFunc[ePowerStatus] != NULL)
    {
        (gatPmSysInfo[eSysType].ptPowerInfo->aPeripheralUintCtrlFunc[ePowerStatus])();
    }

    ptCurrSysPowerInfo = gatPmSysInfo[eSysType].ptPowerInfo;

    ptCurrSysPowerInfo->u8CtrlFailCount = 0;  /* 电源控制失败次数清零 */
    if ((ptCurrSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_POWER_CTRL_FAULT) ||
        (ptCurrSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_POWER_CTRL_FAILURE))
    {
        /* 收到新电源控制请求后，如果之前电源控制失败，则重新启动电源控制 */
        ptCurrSysPowerInfo->eCurrPowerStatus = ptCurrSysPowerInfo->eFeedbackPowerStatus;
    }

#ifndef DIAS_PM_EOL
    ptCurrSysPowerInfo->eLastExpectPowerStatus = ptCurrSysPowerInfo->eExpectPowerStatus;
    ptCurrSysPowerInfo->eExpectPowerStatus = ePowerStatus;

    if (ptCurrSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_NONE)
    {
        ptCurrSysPowerInfo->ePowerCtrlType = DiasPmGetSysPowerCtrlType(ptCurrSysPowerInfo);
        DiasPmProcPowerUpWhenChangingPowerStatus(eSysType, ptCurrSysPowerInfo);
    }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:PowerCtrl req sys:%d,exp:%d,cur:%d.\n", eSysType, ePowerStatus, ptCurrSysPowerInfo->eCurrPowerStatus);
#else
    if (gbPmResetMcu == TRUE)
    {
        return ERR;
    }

    if ((ptCurrSysPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE) ||
        (ptCurrSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_UNSTABLE) ||
        (ptCurrSysPowerInfo->bPowerStatusConfirm != FALSE))
    {
        return ERR;
    }

    ptCurrSysPowerInfo->eExpectPowerStatus = ePowerStatus;
    ptCurrSysPowerInfo->ePowerCtrlType = DiasPmGetSysPowerCtrlType(ptCurrSysPowerInfo);
    DiasPmProcPowerUpWhenChangingPowerStatus(eSysType, ptCurrSysPowerInfo);

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:PowerCtrl req sys:%d,exp:%d,cur:%d\n", eSysType, ePowerStatus, ptCurrSysPowerInfo->eCurrPowerStatus);
#endif

    return OK;
}

/******************************************************************************
*  function name | DiasPmGetPowerCtrlType
*  content       | 获取控制类型
*  parameter[1]  | DiasPmPowerStatus_e ePrePowerMode：当前电源状态
*                | DiasPmPowerStatus_e eNextPowerStatus：目标电源状态
*  notice        | 无
*  return        | DiasPmPowerCtrlType_e：DIAS_PM_CTRL_NONE(不控制)
*                |                        DIAS_PM_CTRL_POWER_ON(控制开机)
*                |                        DIAS_PM_CTRL_POWER_OFF(控制关机)
*                |                        DIAS_PM_CTRL_SLEEP(控制休眠)
*                |                        DIAS_PM_CTRL_WAKE_UP(控制唤醒)
*                |                        DIAS_PM_CTRL_RESET(控制重启)
******************************************************************************/
static DiasPmPowerCtrlType_e DiasPmGetPowerCtrlType(DiasPmPowerStatus_e ePrePowerStatus, DiasPmPowerStatus_e eNextPowerStatus)
{
    switch (ePrePowerStatus)
    {
        case DIAS_PM_STATUS_POWER_OFF:
        {
            if (eNextPowerStatus != DIAS_PM_STATUS_POWER_OFF)
            {
                /* 无论是WAKEUP状态还是SLEEP状态，均需先开机 */
                return DIAS_PM_CTRL_POWER_ON;
            }
            break;
        }
        case DIAS_PM_STATUS_WAKE_UP:
        {
            if (eNextPowerStatus == DIAS_PM_STATUS_POWER_OFF)
            {
                /* 关机 */
                return DIAS_PM_CTRL_POWER_OFF;
            }

            if (eNextPowerStatus == DIAS_PM_STATUS_SLEEP)
            {
                /* 休眠 */
                return DIAS_PM_CTRL_SLEEP;
            }
            break;
        }
        case DIAS_PM_STATUS_SLEEP:
        {
            if (eNextPowerStatus == DIAS_PM_STATUS_POWER_OFF)
            {
                /* 休眠情况下不能直接关机，需要先唤醒 */
                return DIAS_PM_CTRL_WAKE_UP;
            }

            if (eNextPowerStatus != DIAS_PM_STATUS_SLEEP)
            {
                /* 无论是WAKEUP状态还是OFF状态，均需先唤醒 */
                return DIAS_PM_CTRL_WAKE_UP;
            }
            break;
        }
        case DIAS_PM_STATUS_RUN_ABMORMAL:
        {
            if (eNextPowerStatus == DIAS_PM_STATUS_POWER_OFF)
            {
                /* 系统运行异常，只处理关机命令和重启命令(此处只处理关机) */
                return DIAS_PM_CTRL_POWER_OFF;
            }
            break;
        }
        case DIAS_PM_STATUS_POWER_CTRL_FAILURE:
        {
            if (eNextPowerStatus != DIAS_PM_STATUS_WAKE_UP)
            {
                /* 控制关机/休眠失败，系统可能已死机，使用硬关机 */
                return DIAS_PM_CTRL_HARD_POWER_OFF;
            }
            break;
        }
        default:
            break;
    }

    return DIAS_PM_CTRL_NONE;
}

/******************************************************************************
*  function name | DiasPmIsPeripheralSysPowerCtrlFinished
*  content       | 判断周边系统电源控制是否完成
*  parameter[1]  | DiasPmSysType_e eSysType:系统类型
*  notice        | 无
*  return        | BOOL：TRUE(电源控制完成)/FALSE(电源控制未完成)
******************************************************************************/
BOOL DiasPmIsPeripheralSysPowerCtrlFinished(DiasPmSysType_e eSysType)
{
    if (gatPmSysInfo[eSysType].ptPowerInfo == NULL)
    {
        return TRUE;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
    {
        return FALSE;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_POWER_CTRL_FAULT)
    {
        /* 控制失败后不再控制，可以认为控制已经结束 */
        return TRUE;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->eCurrPowerStatus != gatPmSysInfo[eSysType].ptPowerInfo->eExpectPowerStatus)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************
*  function name | DiasPmIsAllPeripheralSysPowerCtrlFinished
*  content       | 判断所有周边系统电源控制是否完成
*  parameter[1]  | void
*  notice        | 无
*  return        | BOOL：TRUE(电源控制完成)/FALSE(电源控制未完成)
******************************************************************************/
BOOL DiasPmIsAllPeripheralSysPowerCtrlFinished(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (DiasPmIsPeripheralSysPowerCtrlFinished((DiasPmSysType_e)u8Loop) != TRUE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************
*  function name | ApiPmGetCurrSysPowerStatus
*  content       | 获取当前电源状态
*  parameter[1]  | DiasPmSysType_e eSysType
*  notice        | 需要通过该函数判断电源控制是否完成，故不再控制电源，需有实际
*                | 的电源状态，不支持电源控制返回关机状态
*  return        | DiasPmPowerStatus_e：DIAS_PM_STATUS_UNSTABLE(不确定状态)
*                |                      DIAS_PM_STATUS_POWER_OFF(关机)
*                |                      DIAS_PM_STATUS_SLEEP(休眠)
*                |                      DIAS_PM_STATUS_WAKE_UP(唤醒)
*                |                      DIAS_PM_STATUS_RUN_ABMORMAL(异常运行状态)
******************************************************************************/
DiasPmPowerStatus_e ApiPmGetCurrSysPowerStatus(DiasPmSysType_e eSysType)
{
    if (eSysType >= DIAS_PM_SYS_TYPE_END)
    {
        return DIAS_PM_STATUS_POWER_OFF;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo == NULL)
    {
        return DIAS_PM_STATUS_UNSTABLE;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
    {
        return DIAS_PM_STATUS_UNSTABLE;
    }

    return gatPmSysInfo[eSysType].ptPowerInfo->eFeedbackPowerStatus;
}

/******************************************************************************
*  function name | DiasPmNotifyPowerModeToPeripheralSys
*  content       | 电源模式广播通知
*  parameter[1]  | PmPowerModeStatus_e ePowerMode：电源模式
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmNotifyPowerModeToPeripheralSys(PmPowerModeStatus_e ePowerMode)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].notifyPowerModeFunc != NULL)
        {
            /* 通知电源模式切换 */
            gatPmSysInfo[u8Loop].notifyPowerModeFunc(ePowerMode);
        }
        else
        {
            gatPmSysInfo[u8Loop].ePowerModeStatus = ePowerMode;
        }
    }
}

/******************************************************************************
*  function name | DiasPmNotifySleepFailToPeripheralSys
*  content       | 通知MCU周边系统休眠失败
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmNotifySleepFailToPeripheralSys(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo != NULL)
        {
            if (gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->eNotifyStatus == DIAS_PM_WAIT_SEND)
            {
                gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->eNotifyStatus = DIAS_PM_SENDING;
                gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->u8FailureCount = 0;
            }
        }
    }
}

/******************************************************************************
*  function name | DiasPmIsPowerModeChangeEnd
*  content       | 判断所有周边系统电源模式是否都已变更完成
*  parameter[1]  | PmPowerModeStatus_e eCurrPowerMode：当前电源模式
*  notice        | 无
*  return        | BOOL：TRUE(电源模式变更完成)/FALSE(电源模式未变更完成)
******************************************************************************/
BOOL DiasPmIsPowerModeChangeEnd(PmPowerModeStatus_e eCurrPowerMode)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo != NULL)
        {
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
            {
                /* 正在进行电源控制，认为电源模式切换未完成 */
                return FALSE;
            }

            if (gatPmSysInfo[u8Loop].ptPowerInfo->eFeedbackPowerStatus != DIAS_PM_STATUS_WAKE_UP)
            {
                /* 系统软件已不再运行，认为电源模式切换完成 */
                continue;
            }
        }

        if (gatPmSysInfo[u8Loop].ePowerModeStatus != eCurrPowerMode)
        {
            return FALSE;
        }
    }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Power mode change end.PowerMode:%d\n", eCurrPowerMode);
    return TRUE;
}

/******************************************************************************
*  function name | DiasPmIsSysSleepReady
*  content       | 判断是否可以进入休眠
*  parameter[1]  | DiasPmSysType_e eSysType
*                | BOOL bAllowIgnore：允许忽略
*  notice        | 无
*  return        | BOOL：TRUE(可以进入休眠)/FALSE(不可以进入休眠)
******************************************************************************/
BOOL DiasPmIsSysSleepReady(DiasPmSysType_e eSysType, BOOL bAllowIgnore)
{
    if (gatPmSysInfo[eSysType].ptPowerInfo != NULL)
    {
        if (DiasPmIsPeripheralSysPowerCtrlFinished(eSysType) != TRUE)
        {
            /* 电源控制未完成，休眠条件满足 */
            return TRUE;
        }

        if (gatPmSysInfo[eSysType].ptPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_WAKE_UP)
        {
            /* 系统软件已不再运行或运行异常，休眠条件满足 */
            return TRUE;
        }
    }

    if ((bAllowIgnore == TRUE) &&
        (gatPmSysInfo[eSysType].bIsSleepReadyIgnore == TRUE))
    {
        /* 不允许休眠被忽略，无论是否允许休眠都认为可以休眠 */
        return TRUE;
    }

    return gatPmSysInfo[eSysType].bIsSleepReady;
}

/******************************************************************************
*  function name | DiasPmSetSysSleepReadyIgnoreFlag
*  content       | 设置系统不允许休眠是否可以忽略标记
*  parameter[1]  | DiasPmSysType_e eSysType
*                | BOOL bIgnore
*  notice        | 无
*  return        | VOID
******************************************************************************/
void DiasPmSetSysSleepReadyIgnoreFlag(DiasPmSysType_e eSysType, BOOL bIgnore)
{
    gatPmSysInfo[eSysType].bIsSleepReadyIgnore = bIgnore;
}

#ifndef DIAS_PM_EOL
/******************************************************************************
*  function name | DiasPmGetNoServiceTime
*  content       | 获取无远程业务持续时间
*  parameter[1]  | void
*  notice        | 无
*  return        | U32：无远程业务持续时间
******************************************************************************/
uint32_t DiasPmGetNoServiceTime(void)
{
    uint8_t  u8Loop;
    uint32_t u32CurrTime = 0;
    uint32_t u32TotalNoServiceStartTime = 0u;

    u32CurrTime = 0;//ApiAuxTaskGetSysTimeUtc();
    
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].bHasService == FALSE)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo != NULL)
        {
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
            {
                /* 正在进行电源控制，认为存在远程业务 */
                return 0;
            }

            if (gatPmSysInfo[u8Loop].ptPowerInfo->eFeedbackPowerStatus != DIAS_PM_STATUS_WAKE_UP)
            {
                /* 系统软件已不再运行，不需要判断是否有远程业务 */
                continue;
            }
        }

        if (gatPmSysInfo[u8Loop].u32NoServiceStartTime > u32TotalNoServiceStartTime)
        {
            u32TotalNoServiceStartTime = gatPmSysInfo[u8Loop].u32NoServiceStartTime;
        }
    }

    if ((u32TotalNoServiceStartTime == 0) ||
        (u32CurrTime <= u32TotalNoServiceStartTime))
    {
        return 0;
    }

    return u32CurrTime - u32TotalNoServiceStartTime;
}
#endif

/******************************************************************************
*  function name | DiasPmCheckSysHeartBeat
*  content       | 发送心跳给外设模块
*  parameter     | void
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmCheckSysHeartBeat(void)
{
    DiasPmSysMonitorCfg_t *ptMonitorCfg;
    DiasPmFeedbackInfo_t *ptFeedbackInfo;
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if ((gatPmSysInfo[u8Loop].ptPowerInfo == NULL) ||
            (gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg == NULL))
        {
            continue;
        }

        ptMonitorCfg = gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg;

        /* 发送心跳消息方式监控系统 */
        if (ptMonitorCfg->tSendHeartBeatInfo.sendHeartBeatFunc != NULL)
        {
            if (ptMonitorCfg->tSendHeartBeatInfo.u16TimeSinceLastSend < ptMonitorCfg->tSendHeartBeatInfo.u16SendPeriod)
            {
                ++ptMonitorCfg->tSendHeartBeatInfo.u16TimeSinceLastSend;
            }
            else
            {
                ptMonitorCfg->tSendHeartBeatInfo.sendHeartBeatFunc();
                ptMonitorCfg->tSendHeartBeatInfo.u16TimeSinceLastSend = 0;
            }
        }

        /* 状态反馈形式监控系统 */
        if (ptMonitorCfg->ptFeedbackInfo != NULL)
        {
            ptFeedbackInfo = ptMonitorCfg->ptFeedbackInfo;
            if (ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft > 0)
            {
                --ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft;
            }
            else
            {
                ptFeedbackInfo->bFeedbackStatus = DiasPmGetFeedbackStatus(ptFeedbackInfo);
                if (ptFeedbackInfo->bFeedbackStatus == TRUE)
                {
                    ptMonitorCfg->u32HeartbeatDeadTimer = ptMonitorCfg->u32AbnormalConfirmPeriod;
                }
            }
        }
    }
}

/******************************************************************************
*  function name | ApiPmRegisterEventListener
*  content       | pm app与DCPD相关回调函数注册
*  parameter[1]  | eConnectType-connect type
*  notice        | 无
*  return        | void
******************************************************************************/
void ApiPmRegisterEventListener(void)
{
    DiasPmLteRegisterEventListener();
}

/******************************************************************************
*  function name | DiasPmSysPowerDownBeforeLowPower
*  content       | 进入低功耗前系统断电处理
*  parameter     | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSysPowerDownBeforeLowPower(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo == NULL)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo == NULL)
        {
            continue;
        }

        /* 电源状态强制停留时间清零，防止唤醒后还需电源控制等待 */
        gatPmSysInfo[u8Loop].ptPowerInfo->u16ForceStayLeftTime = 0;

        if ((gatPmSysInfo[u8Loop].ptPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_POWER_OFF) &&
            (gatPmSysInfo[u8Loop].ptPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_POWER_CTRL_FAULT))
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerDownInLowPower == TRUE)
        {
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerDownFunc != NULL)
            {
                gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerDownFunc();
            }

            gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUp = FALSE;
        }
    }
}

/******************************************************************************
*  function name | DiasPmRefreshPowerUpStatusAfterSysPowerUp
*  content       | 整机上电后系统上电状态刷新
*  parameter     | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmRefreshPowerUpStatusAfterSysPowerUp(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo == NULL)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo == NULL)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUpDefault == FALSE)
        {
            continue;
        }

        gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUp = TRUE;

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerUpFunc != NULL)
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerUpFunc();
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerOnAfterPowerUp == TRUE)
        {
            /* 上电默认开机的系统在上电后等待开机结果后才可进行其他电源控制 */
            gatPmSysInfo[u8Loop].ptPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_POWER_ON;
            gatPmSysInfo[u8Loop].ptPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_UNSTABLE;
            gatPmSysInfo[u8Loop].ptPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_FEEDBACK;
            gatPmSysInfo[u8Loop].ptPowerInfo->u32FeedbackTimerCount = 0;
            ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Sys poweron after power up.Sys:%d\n", u8Loop);
        }
    }
}

/******************************************************************************
*  function name | DiasPmRefreshPowerUpStatusWhenWakeupFromDeepstop
*  content       | 从深度休眠模式唤醒后系统电源状态刷新
*  parameter     | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmRefreshPowerUpStatusWhenWakeupFromDeepstop(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo == NULL)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo == NULL)
        {
            continue;
        }

#if 0   /* 深度休眠保持GPIO状态 */
        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUpDefault == FALSE)
        {
            continue;
        }
#endif

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerDownInLowPower == TRUE)
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUp = FALSE;
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerDownFunc != NULL)
            {
                gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerDownFunc();
            }
        }
        else
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUp = TRUE;
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerUpFunc != NULL)
            {
                gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerUpFunc();
            }
        }
    }
}

/******************************************************************************
*  function name | DiasPmProcPowerUpWhenChangingPowerStatus
*  content       | 期待电源状态改变时上电处理
*  parameter[1]  | DiasPmSysType_e eSysType：系统类型
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo：电源信息
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmProcPowerUpWhenChangingPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    if ((ptCurrSysPowerInfo == NULL) ||
        (ptCurrSysPowerInfo->ptPowerUpCtrlInfo == NULL))
    {
        return;
    }

    switch (ptCurrSysPowerInfo->ePowerCtrlType)
    {
        case DIAS_PM_CTRL_NONE:
        case DIAS_PM_CTRL_POWER_ON:
        {
            if (ptCurrSysPowerInfo->ptPowerUpCtrlInfo->bPowerUp == FALSE)
            {
                if (ptCurrSysPowerInfo->ptPowerUpCtrlInfo->powerUpFunc != NULL)
                {
                    ptCurrSysPowerInfo->ptPowerUpCtrlInfo->powerUpFunc();
                    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Sys power up Sys:%d\n", eSysType);
                }

                ptCurrSysPowerInfo->ptPowerUpCtrlInfo->bPowerUp = TRUE;

                if (ptCurrSysPowerInfo->ptPowerUpCtrlInfo->bPowerOnAfterPowerUp == TRUE)
                {
                    /* 上电后直接开机，可以直接等待系统开机结果 */
                    ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_POWER_ON;
                    ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_UNSTABLE;
                    ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_FEEDBACK;
                    ptCurrSysPowerInfo->u32FeedbackTimerCount = 0;
                }
            }
            break;
        }
        default:
        {
            /* 系统不是OFF状态，已经上电，这里为确保万无一失，再次控制上电引脚 */
            /* 应用场景：升级MCU前，网口电源被MPU接管，升级后需MCU重新接管 */
            if (ptCurrSysPowerInfo->ptPowerUpCtrlInfo->powerUpFunc != NULL)
            {
                ptCurrSysPowerInfo->ptPowerUpCtrlInfo->powerUpFunc();
            }

            ptCurrSysPowerInfo->ptPowerUpCtrlInfo->bPowerUp = TRUE;
            break;
        }
    }
}



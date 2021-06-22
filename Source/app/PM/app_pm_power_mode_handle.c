/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_power_mode_handle.c
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
* 2020-08-06   V11.01.01.01.00  徐高腾        编译警告修改
* 2020-08-06   V11.01.01.01.00  徐高腾        进入低功耗前将已关机且需要掉电的系统下电
* 2020-08-14   V11.01.01.01.00  徐高腾        非RTC唤醒时gu32PmWakeupSource不设置RTC唤醒标记
* 2020-08-28   V11.01.01.01.00  徐高腾        增加12V/24V系统识别功能
******************************************************************************/


/******************************************************************************
* 头文件
******************************************************************************/
#include "os_log.h"
#include "pm_public.h"
#include "app_pm_power_mode_cfg.h"
#include "app_pm_power_mode.h"
#include "app_pm_peripheral_sys.h"
#include "app_pm_mv.h"
#include "app_pm_vol.h"
#include "app_pm_public_define.h"
#include "app_pm_drv.h"

#include "spp_generated.h"

#include "ecu_gpio.h"

/******************************************************************************
* 宏定义
******************************************************************************/
#define DIAS_PM_NOTIFY_POWER_MODE_TIMER             (1000u / DIAS_PM_PERIOD_TIMER)     /* 周期性同步电源模式的间隔时间 */
#define DIAS_PM_POWER_MODE_CHANGE_TIMEOUT           (3200u / DIAS_PM_PERIOD_TIMER)     /* Lte模组电源模式变更超时时间 */
#define DIAS_PM_MAX_RENOTIFY_POWER_MODE_COUNT       (5u)                               /* 电源模式改变时同步电源模式重试次数 */
#define DIAS_PM_SLEEP_TIMEOUT_AFTER_NW_OFF          (1800000u / DIAS_PM_PERIOD_TIMER)  /* 车辆总线状态OFF后判定休眠失败的时间 */

#define DIAS_PM_SLEEP_TIMER_2                       (11*24*60U)                                /* TBOX Sleep2:11天 休眠时间 单位:mins */
/******************************************************************************
* 类型定义
******************************************************************************/
typedef struct
{
    uint8_t   u8PowerMode;
    uint8_t   u8PowerModeRelateType;
    uint16_t  bPowerTimer12Expired     : 1;
    uint16_t  bPowerTimer2Expired      : 1;
    uint16_t  u16Reserved              : 14;
    uint32_t  u32Sleep1StartTime;
    uint32_t  u32Sleep2StartTime;
}DiasPmPowerModeInfoSaved_t;


/******************************************************************************
* 变量定义
******************************************************************************/
PmPowerModeStatus_e gePmPowerModeStatus = PM_POWER_MODE_OFF;
DiasPmPowerModeHandleState_e gePmPowerModeHandleState = DIAS_PM_HANDLE_MCU_START;
static uint32_t gu32PmWakeupSource = 0;                 /* MCU休眠前设置的唤醒源 */
static DiasPmPowerModeInfoSaved_t gtPmPowerModeInfoSaved = {PM_POWER_MODE_OFF, DIAS_PM_MODE_RELATE_TYPE_NORMAL, FALSE, FALSE, 0, 0, 0};

static uint32_t gu32PmNotifyPowerModeTimerCount = 0;


/******************************************************************************
* 休眠唤醒处理
******************************************************************************/
uint8_t       gu8CurrHandlePriority = 0;

BOOL      gbPmPowerTimer12Expired = FALSE;                     /* Power_Timer_12_expired满足 */
BOOL      gbPmPowerTimer2Expired = FALSE;                      /* Power_Timer_2_expired满足 */
uint32_t  gu32PmSleep1StartTime = 0;                           /* 开始进入PM_POWER_MODE_SLEEP1时刻时间 */
uint32_t  gu32PmSleep2StartTime = 0;                           /* 开始进入PM_POWER_MODE_SLEEP2时刻时间 */

static uint32_t gu32WaitPowerModeChangeEndCount = 0;           /* 等待电源模式切换结束计数器 */
static uint32_t gu32RetryPowerModeChangeCount = 0;             /* 电源模式广播通知重试次数 */

static uint32_t gu32TimeCountAfterNwOff = 0;                   /* 车辆总线状态OFF后时间累计 */


/******************************************************************************
* 模式迁移处理标记
******************************************************************************/
BOOL gbPmNewStateHandleStartFlag = TRUE;                     /* 新状态处理开始Flag */
BOOL gbPmNotifyPowerStatusFlag = FALSE;                      /* 同步电源模式Flag */


/******************************************************************************
* 内部电源模式管理
******************************************************************************/
static int32_t DiasPmProcDeepReset(PmPowerModeStatus_e ePowerMode);
static BOOL    DiasPmProcMcuTaskSleepByPriority(uint8_t u8Priority);
static void    DiasPmSetWakeupSourceInSleep3Mode(void);
static int32_t DiasPmSetWakeupSourceInSleep2Mode(uint32_t u32PowerTime2);
static void    DiasPmSetWakeupSourceInSleep1Mode(uint32_t u32PowerTime11);
static BOOL    DiasPmIsPowerTimer12Expired(uint32_t u32PowerTime12);
static BOOL    DiasPmIsPowerTimer2Expired(uint32_t u32PowerTime2);
static int32_t DiasPmSaveDefaultPowerModeInfo(void);
static int32_t DiasPmSaveCurrPowerModeInfo(void);
static void    DiasPmProcessNetworkStatusChanging(void);
static void    DiasPmOutputLowPowerCondition(void);

/******************************************************************************
*  function name | DiasPmPowerModeTimeout
*  content       | 电源模式迁移周期处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmPowerModeTimeout(void)
{
    int32_t i32Ret = ERR;

    if (gePmPowerModeStatus >= PM_POWER_MODE_END)
    {
        gePmPowerModeStatus = DIAS_PM_POWER_MODE_INIT;
    }

    // (void)GetResource(gtPmLock);

    if (gatPmPowerModeCfg[gePmPowerModeStatus].handleFunc != NULL)
    {
        (gatPmPowerModeCfg[gePmPowerModeStatus].handleFunc)();
    }

    if (gePmPowerModeHandleState == DIAS_PM_HANDLE_NONE)
    {
        if (gatPmPowerModeCfg[gePmPowerModeStatus].aeRelatePowerMode[gePmPowerModeRelateType] != PM_POWER_MODE_INVALID)
        {
            DiasPmChangePowerMode(gePmPowerModeStatus, gatPmPowerModeCfg[gePmPowerModeStatus].aeRelatePowerMode[gePmPowerModeRelateType]);
            i32Ret = DiasPmSaveCurrPowerModeInfo();
            ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:ToSpecMode %d,Ret:%d\n", gePmPowerModeRelateType, i32Ret);
        }
    }

    DiasPmProcessNetworkStatusChanging();

    // (void)ReleaseResource(gtPmLock);

    /* 每隔DIAS_PM_NOTIFY_POWER_MODE_TIMER时间同步电源模式 */
    if (gu32PmNotifyPowerModeTimerCount < DIAS_PM_NOTIFY_POWER_MODE_TIMER)
    {
        ++gu32PmNotifyPowerModeTimerCount;
        return;
    }

    gu32PmNotifyPowerModeTimerCount = 0u;
    DiasPmNotifyPowerMode(NULL, NULL);
    if (gePmPowerModeStatus == PM_POWER_MODE_RUNNING)
    {
        DiasPmOutputLowPowerCondition();
    }
}

/******************************************************************************
*  function name | DiasPmChangeSysPowerStatusByPowerMode
*  content       | 根据电源模式修改周边系统电源控制
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmChangeSysPowerStatusByPowerMode(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    uint8_t u8Loop = 0;
    uint8_t u8SysNum = (uint8_t)DIAS_PM_SYS_TYPE_END;
    if(pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    if (gePmPowerModeStatus >= PM_POWER_MODE_END)
    {
        gePmPowerModeStatus = PM_POWER_MODE_OFF;
    }

    for (; u8Loop < u8SysNum; ++u8Loop)
    {
        DiasPmChangeSysPowerStatus((DiasPmSysType_e)u8Loop, gatPmPowerModeCfg[gePmPowerModeStatus].aePowerStatus[u8Loop]);
    }

    /* 状态切换 */
    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmWaitAllPeripheralSysOff
*  content       | 等待周边系统电源关机
*  parameter[1]  | DiasPmSysType_e eSysType        ：系统类型
*                | DiasPmPowerStatus_e ePowerStatus：电源状态
*                | BOOL *pbCurrFuncFlag            ：当前处理Flag
*                | BOOL *pbNextFuncFlag            ：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmChangeSysPowerStatusHandle(DiasPmSysType_e eSysType, DiasPmPowerStatus_e ePowerStatus, BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    if(pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    DiasPmChangeSysPowerStatus(eSysType, ePowerStatus);

    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmChangePowerMode
*  content       | 电源模式变更
*  parameter[1]  | PmPowerModeStatus_e ePreStatus：变更前电源模式
*                | PmPowerModeStatus_e eNextStatus：变更后电源模式
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmChangePowerMode(PmPowerModeStatus_e ePreStatus, PmPowerModeStatus_e eNextStatus)
{
    uint8_t u8Loop = 0;

    gu32WaitPowerModeChangeEndCount = 0u;   /* 等待电源模式切换结束计数器 */
    gu32RetryPowerModeChangeCount = 0u;     /* 电源模式广播通知重试次数 */

    /* 清除所有的处理Flag */
    DiasPmInitAllHandleFlag();

    for (; u8Loop < gu8PmPowerModeChangeTypeNum; ++u8Loop)
    {
        if ((gatPmPowerModeChangeCfg[u8Loop].ePreStatus == ePreStatus) &&
            (gatPmPowerModeChangeCfg[u8Loop].eNextStatus == eNextStatus))
        {
            gePmPowerModeHandleState = gatPmPowerModeChangeCfg[u8Loop].ePowerModeHandleState;
            gbPmNewStateHandleStartFlag = TRUE;
            gePmPowerModeStatus = eNextStatus;
            ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Mode %d to %d\n", ePreStatus, eNextStatus);
            return;
        }
    }

    gePmPowerModeHandleState = DIAS_PM_HANDLE_NONE;
    gbPmNewStateHandleStartFlag = TRUE;
    gePmPowerModeStatus = eNextStatus;
    DiasPmChangeSysPowerStatusByPowerMode(NULL, NULL);
    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Mode %d to %d\n", ePreStatus, eNextStatus);
}

/******************************************************************************
*  function name | DiasPmForceReset
*  content       | 系统强制重启
*  parameter[1]  | VOID
*  notice        | 无
*  return        | INT32:OK(处理成功)/其他(处理失败)
******************************************************************************/
int32_t ApiPmResetSystem(BOOL bSavePowerMode)
{
    INT32 i32Ret = ERR;

    if (bSavePowerMode == TRUE)
    {
        i32Ret = DiasPmSaveCurrPowerModeInfo();
    }
    else
    {
        i32Ret = DiasPmSaveDefaultPowerModeInfo();
    }

    if (i32Ret != OK)
    {
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:SaveFail\n");
    }

    gbPmResetMcu = TRUE;

    gu32WaitPowerModeChangeEndCount = 0u;   /* 等待电源模式切换结束计数器 */
    gu32RetryPowerModeChangeCount = 0u;     /* 电源模式广播通知重试次数 */

    /* 清除所有的处理Flag */
    DiasPmInitAllHandleFlag();

    gePmPowerModeHandleState = DIAS_PM_ALL_SYSTEM_FORCE_RESET;
    gbPmNewStateHandleStartFlag = TRUE;
    gePmPowerModeStatus = PM_POWER_MODE_OFF;
    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:SysForceReset\n");

    return OK;
}

/******************************************************************************
*  function name | DiasPmReadAfterMcuStart
*  content       | MCU启动后读取数据处理
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmReadAfterMcuStart(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    if ((pbCurrFuncFlag == NULL) ||
        (*pbCurrFuncFlag != TRUE))
    {
        return;
    }

    if (DiasPmVolInit() != OK)
    {
        return;
    }

    // if (ApiNvramReadSyncInd(INDEX_NVID_0xE012, sizeof(gtPmPowerModeInfoSaved), (uint8_t *)&gtPmPowerModeInfoSaved) != TRUE)
    // {
    //     return;
    // }

    if (DiasPmPeripheralSysMonitorInit() != OK)
    {
        return;
    }

    *pbCurrFuncFlag = FALSE;
    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmNotifyPowerMode
*  content       | 电源模式广播通知
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmNotifyPowerMode(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    /* 状态切换 */
    if(pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }
        else
        {
            *pbCurrFuncFlag = FALSE;
        }
    }

    DiasPmNotifyPowerModeToPeripheralSys(gePmPowerModeStatus);

    if(pbNextFuncFlag != NULL)
    {
        /* 无论是否发送成功，都切到下一个状态，若发送失败，MPU电源模式校验不通过会重新发送 */
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmSwitchFlag
*  content       | Flag切换
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSwitchFlag(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    /* 状态切换 */
    if(pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }
        else
        {
            *pbCurrFuncFlag = FALSE;
        }
    }

    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmFinishPowerModeSwitch
*  content       | 电源模式切换结束
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmFinishPowerModeSwitch(BOOL *pbCurrFuncFlag)
{
    if(pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }
        else
        {
            *pbCurrFuncFlag = FALSE;
        }
    }

    gePmPowerModeHandleState = DIAS_PM_HANDLE_NONE;
}

/******************************************************************************
*  function name | DiasPmHandleBeforeMcuTaskStateSwitch
*  content       | 切换MCU其他模块状态前处理
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmHandleBeforeMcuTaskStateSwitch(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    if (pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    gu8CurrHandlePriority = 0;

    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmLeaveLowPower
*  content       | MCU退出低功耗
*  parameter[1]  | PmPowerModeStatus_e eCurrPowerMode:当前电源模式
*                | PmPowerModeStatus_e eTargetPowerMode:目标电源模式
*                | BOOL *pbCurrFuncFlag：当前处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmLeaveLowPower(PmPowerModeStatus_e eCurrPowerMode, PmPowerModeStatus_e eTargetPowerMode, BOOL *pbCurrFuncFlag)
{
    if (pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    gu32PmWakeupSource = 0u;
    gu32PmWakeupSourceInSleeping = 0u;
    gbPmResetMcu = FALSE;
    gbPmPowerTimer12Expired = FALSE;
    gbPmPowerTimer2Expired = FALSE;
    gu32PmSleep1StartTime = 0;
    gu32PmSleep2StartTime = 0;
    gu32TimeCountAfterNwOff = 0;

    /* MCU正常工作时，即使系统电源控制失败，也不硬关机，防止硬关机后再开机还失败，一直控制电源 */
    DiasPmSetRecoveryAfterPowerCtrlFailEnable(FALSE);

    DiasPmClearSleepFailNotifyInfo();

    DiasPmSetPowerStatusRecoveryEnable(TRUE);

    /* 电源模式迁移到TELEMATIC模式 */
    DiasPmChangePowerMode(eCurrPowerMode, eTargetPowerMode);
}

/******************************************************************************
*  function name | DiasPmWaitMcuTaskWakeup
*  content       | 等待MCU其他模块唤醒
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmWaitMcuTaskWakeup(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    uint16_t u16Loop = 0;
    if ((pbCurrFuncFlag == NULL) || (*pbCurrFuncFlag != TRUE))
    {
        return;
    }

    for (; u16Loop < gu16PmMcuModuleNum; ++u16Loop)
    {
        if ((gatPmMcuModuleInfo[u16Loop].u8WakeupPriority != gu8CurrHandlePriority) ||
            (gatPmMcuModuleInfo[u16Loop].eModuleState == DIAS_PM_WAKEUP_OK))
        {
            continue;
        }

        if (gatPmMcuModuleInfo[u16Loop].wakeupFunc != NULL)
        {
            if (gatPmMcuModuleInfo[u16Loop].wakeupFunc() != TRUE)
            {
                gatPmMcuModuleInfo[u16Loop].eModuleState = DIAS_PM_WAKEUP_ERR;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_DEBUG, "WakeupFail.i:%d\n", u16Loop);
                return;
            }
            else
            {
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "WakeupOk.i:%d\n", u16Loop);
            }
        }

        gatPmMcuModuleInfo[u16Loop].eModuleState = DIAS_PM_WAKEUP_OK;
    }

    ++gu8CurrHandlePriority;
    if (gu8CurrHandlePriority <= gu8PmMaxWakeupPriority)
    {
        return;
    }

    /* MCU其他模块已完成唤醒，可以进入下一步处理 */
    *pbCurrFuncFlag = FALSE;
    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }

}

/******************************************************************************
*  function name | DiasPmHandleAfterMcuWakeup
*  content       | Mcu唤醒后电源模式处理
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmHandleAfterMcuWakeup(BOOL *pbCurrFuncFlag)
{
    uint32_t u32WakeupTime;
    uint32_t u32CurrTime;
    uint32_t u32WakeupSource;
    uint32_t u32ValidWakeupSource;
    BOOL     bPmOnlyRtcWakeup = FALSE;

    if (pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    // ApiStbcGetResetFactor();

    // u32WakeupSource = ApiStbcGetWakeupFactor();
    u32WakeupSource |= gu32PmWakeupSourceInSleeping;
    u32ValidWakeupSource = u32WakeupSource & gu32PmWakeupSource;

//     if ((gu32PmWakeupSource & DIAS_PM_RTC_WAKEUP_SOURCE) != 0)
//     {
//         u32WakeupTime = ApiAuxTaskGetAlarmTimeUtc();
//         if (u32WakeupTime != 0)
//         {
//             u32CurrTime = ApiAuxTaskGetSysTimeUtc();

//             /* 休眠模式剩余停留时间太短时，可以认为Power_Timer已满足，避免来不及进入休眠模式就需唤醒 */
//             if (u32WakeupTime <= (u32CurrTime + DIAS_PM_ENTER_LOW_POWER_MAX_TIME))
//             {
//                 u32ValidWakeupSource |= DIAS_PM_RTC_WAKEUP_SOURCE;
//             }

// #ifdef DIAS_PM_RTC_WKAEUP_SOURCE_NOT_ALONE
//             if (u32WakeupTime <= (u32CurrTime + DIAS_PM_ENTER_LOW_POWER_MAX_TIME))
// #else
//             if ((u32ValidWakeupSource & DIAS_PM_RTC_WAKEUP_SOURCE) != 0)
// #endif
//             {
//                 /* 仅被RTC唤醒 */
//                 if ((u32ValidWakeupSource & (~DIAS_PM_RTC_WAKEUP_SOURCE)) == 0)
//                 {
//                     bPmOnlyRtcWakeup = TRUE;
//                 }
//             }
//         }
//     }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "ValidWakeupSource:%08X\n", u32ValidWakeupSource);
    if (u32ValidWakeupSource == 0)
    {
        gbPmNewStateHandleStartFlag = TRUE;
        gePmPowerModeHandleState = DIAS_PM_MCU_WAKEUP_INVALID_SOURCE;
        return;
    }

    if (bPmOnlyRtcWakeup == TRUE)
    {
        gbPmNewStateHandleStartFlag = TRUE;
        gePmPowerModeHandleState = DIAS_PM_MCU_WAKEUP_BY_RTC;
    }
    else
    {
        gbPmNewStateHandleStartFlag = TRUE;
        gePmPowerModeHandleState = DIAS_PM_MCU_WAKEUP_BY_SERVICE;
    }
}

/******************************************************************************
*  function name | DiasPmWaitPowerModeChangeEnd
*  content       | 等待其他系统电源模式切换完成
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmWaitPowerModeChangeEnd(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    if ((pbCurrFuncFlag == NULL) || (*pbCurrFuncFlag != TRUE))
    {
        return;
    }

    if (DiasPmIsPowerModeChangeEnd(gePmPowerModeStatus) == TRUE)
    {
        *pbCurrFuncFlag = FALSE;
        if(pbNextFuncFlag != NULL)
        {
            *pbNextFuncFlag = TRUE;
        }

        gu32WaitPowerModeChangeEndCount = 0u;
        gu32RetryPowerModeChangeCount = 0u;
        return;
    }

    if (gu32WaitPowerModeChangeEndCount < DIAS_PM_POWER_MODE_CHANGE_TIMEOUT)
    {
        ++gu32WaitPowerModeChangeEndCount;
        return;
    }

    gu32WaitPowerModeChangeEndCount = 0u;
    *pbCurrFuncFlag = FALSE;

    if (gu32RetryPowerModeChangeCount < DIAS_PM_MAX_RENOTIFY_POWER_MODE_COUNT)
    {
        /* 重新同步电源模式 */
        ++gu32RetryPowerModeChangeCount;
        gbPmNotifyPowerStatusFlag = TRUE;
    }
    else
    {
        gu32RetryPowerModeChangeCount = 0u;

        /* 电源模式同步失败，继续后续动作 */
        if(pbNextFuncFlag != NULL)
        {
            *pbNextFuncFlag = TRUE;
        }
    }
}

/******************************************************************************
*  function name | DiasPmSetWakeupSourceHandle
*  content       | 设置唤醒源处理
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSetWakeupSourceHandle(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    /* 状态切换 */
    if(pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }
        else
        {
            *pbCurrFuncFlag = FALSE;
        }
    }

    /* 设置唤醒源 */
    // ApiStbcSetWakeupSource(gu32PmWakeupSource);
    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:WakeupSource = 0x%x\n",gu32PmWakeupSource);

    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmMcuEnterDeepstopHandle
*  content       | MCU进入深度休眠状态
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmMcuEnterDeepstopHandle(BOOL *pbCurrFuncFlag)
{
    static uint16_t u16t_deepstop_delay = 0;
    static BOOL B_enter_deepstop = TRUE;
    if ((pbCurrFuncFlag == NULL) || (*pbCurrFuncFlag != TRUE))
    {
        return;
    }

    if (DiasPmProcMcuTaskSleepByPriority(DIAS_PM_PRIORITY_FINAL) != TRUE)
    {
        return;
    }

    u16t_deepstop_delay++;
    if ( B_enter_deepstop == TRUE )
    {
        ApiLogPrint(_LOG_TRACE, "PM: Enter Deepstop!\n");
        ApiLogDisable();
        B_enter_deepstop = FALSE;
    }
    if ( u16t_deepstop_delay < 100 )
    {
        return;
    }

    *pbCurrFuncFlag = FALSE;

#ifndef PM_TEST_MODE
    /* 进入低功耗前将已关机且需要掉电的系统下电 */
    DiasPmSysPowerDownBeforeLowPower();
#endif

    /* 设置唤醒源 */
    // ApiStbcSetWakeupSource(gu32PmWakeupSource);
    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:WakeupSource = 0x%x\n",gu32PmWakeupSource);

    DiasPmDisableHwt();

    DiasPmPeripheralCtrlMcuDeepstop();

    DiasPmSetPowerStatusRecoveryEnable(FALSE);

    /* MCU设置为Deepstop模式 */
    DiasPmMcuEnterDeepstop();

    ApiPmPeripheralCtrlMcuRun();

    DiasPmEnableHwt();

    DiasPmPeripheralSysInitAfterWakeup();

    gePmPowerModeHandleState = DIAS_PM_MCU_WAKEUP;
    gbPmNewStateHandleStartFlag = TRUE;
}

/******************************************************************************
*  function name | DiasPmMcuPowerDownHandle
*  content       | MCU下电处理
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmMcuPowerDownHandle(BOOL *pbCurrFuncFlag)
{
    if ((pbCurrFuncFlag == NULL) || (*pbCurrFuncFlag != TRUE))
    {
        return;
    }

    if (DiasPmProcMcuTaskSleepByPriority(DIAS_PM_PRIORITY_FINAL) != TRUE)
    {
        return;
    }

    *pbCurrFuncFlag = FALSE;

    /* MCU下电 */

    gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
    gbPmNewStateHandleStartFlag = TRUE;
}

/******************************************************************************
*  function name | DiasPmMcuResetHandle
*  content       | MCU重启处理
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmMcuResetHandle(BOOL *pbCurrFuncFlag)
{
    if (pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    (void)DiasPmResetMCU();

    /* 重启成功后PC指针会指向地址0，此处为重启失败处理 */
    gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
    gbPmNewStateHandleStartFlag = TRUE;
    gbPmResetMcu = FALSE;
    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: McuResetFailed\n");
}

/******************************************************************************
*  function name | DiasPmWaitSysPowerStatusCtrlEnd
*  content       | 等待周边系统电源控制结束
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmWaitSysPowerStatusCtrlEnd(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    if ((pbCurrFuncFlag == NULL) || (*pbCurrFuncFlag != TRUE))
    {
        return;
    }

    if (DiasPmIsAllPeripheralSysPowerCtrlFinished() == FALSE)
    {
        return;
    }

    /* 状态切换 */
    *pbCurrFuncFlag = FALSE;
    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmWaitAllPeripheralSysOff
*  content       | 等待周边系统电源关机
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmWaitAllPeripheralSysOff(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    if ((pbCurrFuncFlag == NULL) || (*pbCurrFuncFlag != TRUE))
    {
        return;
    }

    if (DiasPmIsAllPeripheralSysPowerCtrlFinished() == FALSE)
    {
        return;
    }

    /* 状态切换 */
    *pbCurrFuncFlag = FALSE;
    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmIsPowerTimer12Expired
*  content       | 判断Power_Timer_12是否期满
*  parameter[1]  | uint32_t u32PowerTime12：Power_Timer_12_expired值(单位分钟)
*  notice        | 无
*  return        | BOOL
******************************************************************************/
static BOOL DiasPmIsPowerTimer12Expired(uint32_t u32PowerTime12)
{
    uint32_t u32CurrTime;
    uint32_t u32MaxTime;

    u32CurrTime = 0;//ApiAuxTaskGetSysTimeUtc();
    if (gu32PmSleep1StartTime == 0)
    {
        gu32PmSleep1StartTime = u32CurrTime;
    }
    else
    {
        if (u32PowerTime12 != DIAS_U32_INVALID_VALUE)
        {
            /* DIAS_PM_POWER_TIMER_12单位为分钟 */
            u32MaxTime = (uint32_t)u32PowerTime12 * 60u;
            if (u32CurrTime >= (u32MaxTime + gu32PmSleep1StartTime))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/******************************************************************************
*  function name | DiasPmIsPowerTimer2Expired
*  content       | 判断Power_Timer_2是否期满
*  parameter[1]  | uint32_t u32PowerTime2：Power_Timer_2_expired值(单位分钟)
*  notice        | 无
*  return        | BOOL
******************************************************************************/
static BOOL DiasPmIsPowerTimer2Expired(uint32_t u32PowerTime2)
{
    uint32_t u32CurrTime;
    uint32_t u32MaxTime;

    u32CurrTime = 0;//ApiAuxTaskGetSysTimeUtc();
    // if (gu32PmSleep2StartTime == 0)
    // {
    //     gu32PmSleep2StartTime = u32CurrTime;
    // }
    // else
    // {
    // if ( ApiStbcGetWakeupFactor() == DIAS_PM_RTC_WAKEUP_SOURCE )
    // {
    //     if (u32PowerTime2 != DIAS_U32_INVALID_VALUE)
    //     {
    //         /* DIAS_PM_POWER_TIMER_2单位为分钟 */
    //         // u32MaxTime = u32PowerTime2 * 60u;
    //         // if ((u32CurrTime + DIAS_PM_ENTER_LOW_POWER_MAX_TIME) >= (u32MaxTime + gu32PmSleep2StartTime))
    //         // {
    //             ApiLogPrint(_LOG_DEBUG, "Valid RTC wakeup\n");
    //             return TRUE;
    //         // }
    //     }
    // }
    // }

    return FALSE;
}

/******************************************************************************
*  function name | DiasPmSwitchToLowPowerMode
*  content       | 进入低功耗模式
*  parameter[1]  | PmPowerModeStatus_e eCurrStatus:当前电源模式
*                | BOOL *pbCurrFuncFlag：当前处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSwitchToLowPowerMode(PmPowerModeStatus_e eCurrStatus, BOOL *pbCurrFuncFlag)
{
    uint32_t au32PowerTime[DIAS_PM_POWER_TIMER_END] = {0};
    BOOL bPowerTimer12Expired = FALSE;
    BOOL bPowerTimer2Expired = FALSE;

    if (pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }
    }

    /* 进入低功耗时，电源控制失败后进行硬关机处理，防止电流太大 */
    DiasPmSetRecoveryAfterPowerCtrlFailEnable(TRUE);

    // (void)ApiNvramReadSyncInd(INDEX_NVID_0xE022, sizeof(au32PowerTime[DIAS_PM_POWER_TIMER_11]), (uint8_t *)(&au32PowerTime[DIAS_PM_POWER_TIMER_11]));
    // (void)ApiNvramReadSyncInd(INDEX_NVID_0xE023, sizeof(au32PowerTime[DIAS_PM_POWER_TIMER_12]), (uint8_t *)(&au32PowerTime[DIAS_PM_POWER_TIMER_12]));
    // (void)ApiNvramReadSyncInd(INDEX_NVID_0xE024,  sizeof(au32PowerTime[DIAS_PM_POWER_TIMER_2]), (uint8_t *)(&au32PowerTime[DIAS_PM_POWER_TIMER_2]));
    
    au32PowerTime[DIAS_PM_POWER_TIMER_11] = DIAS_PM_LISTEN_NOT_SUPPORT;
    au32PowerTime[DIAS_PM_POWER_TIMER_12] = DIAS_PM_LISTEN_NOT_SUPPORT;
    au32PowerTime[DIAS_PM_POWER_TIMER_2] = DIAS_PM_SLEEP_TIMER_2;


    bPowerTimer12Expired = DiasPmIsPowerTimer12Expired(au32PowerTime[DIAS_PM_POWER_TIMER_12]);
    if ((gbPmPowerTimer12Expired != TRUE) &&
        (bPowerTimer12Expired != TRUE) &&
        (au32PowerTime[DIAS_PM_POWER_TIMER_11] != DIAS_PM_LISTEN_NOT_SUPPORT) &&
        (au32PowerTime[DIAS_PM_POWER_TIMER_12] != DIAS_PM_LISTEN_NOT_SUPPORT))
    {
        DiasPmSetWakeupSourceInSleep1Mode(au32PowerTime[DIAS_PM_POWER_TIMER_11]);
        DiasPmChangePowerMode(eCurrStatus, PM_POWER_MODE_SLEEP1);
    }
    else
    {
        gbPmPowerTimer12Expired = TRUE;

        bPowerTimer2Expired = DiasPmIsPowerTimer2Expired(au32PowerTime[DIAS_PM_POWER_TIMER_2]);
        if ((gbPmPowerTimer2Expired != TRUE) &&
            (bPowerTimer2Expired != TRUE) &&
            (au32PowerTime[DIAS_PM_POWER_TIMER_2] != DIAS_PM_LISTEN_NOT_SUPPORT))
        {
            if (DiasPmSetWakeupSourceInSleep2Mode(au32PowerTime[DIAS_PM_POWER_TIMER_2]) != OK)
            {
                return;
            }

            DiasPmChangePowerMode(eCurrStatus, PM_POWER_MODE_SLEEP2);
        }
        else
        {
            gbPmPowerTimer2Expired = TRUE;

            DiasPmSetWakeupSourceInSleep3Mode();
            DiasPmChangePowerMode(eCurrStatus, PM_POWER_MODE_SLEEP3);
        }
    }

    DiasPmSetPowerStatusRecoveryEnable(TRUE);

    if (pbCurrFuncFlag != NULL)
    {
        *pbCurrFuncFlag = FALSE;
    }
}

/******************************************************************************
*  function name | DiasPmSetWakeupSourceInSleep1Mode
*  content       | 进入Listen模式前的处理
*  parameter[1]  | uint32_t u32PowerTime11：Power_Timer_11_expired值
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmSetWakeupSourceInSleep1Mode(uint32_t u32PowerTime11)
{
    uint32_t u32ListenTime;

    // ApiStbcClrWakeupFactor();

    // gu32PmWakeupSource = DIAS_PM_SLEEP1_MODE_WAKEUP_SOURCE;

    // if (u32PowerTime11 == DIAS_U32_INVALID_VALUE)
    // {
    //     return;
    // }

    // gu32PmWakeupSource |= DIAS_PM_RTC_WAKEUP_SOURCE;

    /* DIAS_PM_POWER_TIMER_11单位为分钟 */
    u32ListenTime = (uint32_t)u32PowerTime11 * 60u;
    // ApiAuxTaskSetTimerSleep(u32ListenTime);
}

/******************************************************************************
*  function name | DiasPmSetWakeupSourceInSleep2Mode
*  content       | 进入Listen模式前的处理
*  parameter[1]  | uint32_t u32PowerTime11：Power_Timer_2_expired值
*  notice        | 无
*  return        | int32_t
******************************************************************************/
static int32_t DiasPmSetWakeupSourceInSleep2Mode(uint32_t u32PowerTime2)
{
    uint32_t u32ListenTime;
    uint32_t u32MaxTime;
    uint32_t u32CurrTime = 0;

    u32CurrTime = 0;//ApiAuxTaskGetSysTimeUtc();

    // ApiStbcClrWakeupFactor();

    // gu32PmWakeupSource = DIAS_PM_SLEEP2_MODE_WAKEUP_SOURCE;

    if (u32PowerTime2 == DIAS_U32_INVALID_VALUE)
    {
        /* 二级休眠不支持RTC唤醒 */
        return OK;
    }

    /* DIAS_PM_POWER_TIMER_2单位为分钟 */
    u32MaxTime = (uint32_t)u32PowerTime2 * 60u;

    // if ((u32CurrTime + DIAS_PM_ENTER_LOW_POWER_MAX_TIME) >= (u32MaxTime + gu32PmSleep2StartTime))
    // {
    //     /* PowerTimer2期满 */
    //     return ERR;
    // }

    // gu32PmWakeupSource |= DIAS_PM_RTC_WAKEUP_SOURCE;

    u32ListenTime = gu32PmSleep2StartTime + u32MaxTime - u32CurrTime;
    // ApiAuxTaskSetTimerSleep(u32MaxTime);
    return OK;
}

/******************************************************************************
*  function name | DiasPmSetWakeupSourceInSleep3Mode
*  content       | 进入Listen模式前的处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmSetWakeupSourceInSleep3Mode(void)
{
    // ApiStbcClrWakeupFactor();

    // gu32PmWakeupSource = DIAS_PM_SLEEP3_MODE_WAKEUP_SOURCE;
}

/******************************************************************************
*  function name | DiasPmSaveDefaultPowerModeInfo
*  content       | 存储默认电源管理状态
*  parameter[1]  | VOID
*  notice        | 无
*  return        | int32_t：OK(存储成功)/ERR(存储失败)
******************************************************************************/
static int32_t DiasPmSaveDefaultPowerModeInfo(void)
{
    gtPmPowerModeInfoSaved.u8PowerMode = (uint8_t)PM_POWER_MODE_OFF;
    gtPmPowerModeInfoSaved.u8PowerModeRelateType = (uint8_t)DIAS_PM_MODE_RELATE_TYPE_NORMAL;
    gtPmPowerModeInfoSaved.u16Reserved = 0;
    gtPmPowerModeInfoSaved.bPowerTimer12Expired = FALSE;
    gtPmPowerModeInfoSaved.bPowerTimer2Expired = FALSE;
    gtPmPowerModeInfoSaved.u32Sleep1StartTime = 0;
    gtPmPowerModeInfoSaved.u32Sleep2StartTime = 0;

    // if (ApiNvramWritAsyncInd(INDEX_NVID_0xE012, sizeof(gtPmPowerModeInfoSaved), (UINT8 *)&gtPmPowerModeInfoSaved) != TRUE)
    // {
    //     return ERR;
    // }

    return OK;
}

/******************************************************************************
*  function name | DiasPmSaveCurrPowerModeInfo
*  content       | 存储当前电源管理状态
*  parameter[1]  | void
*  notice        | 无
*  return        | int32_t：OK(存储成功)/ERR(存储失败)
******************************************************************************/
static int32_t DiasPmSaveCurrPowerModeInfo(void)
{
    gtPmPowerModeInfoSaved.u8PowerMode = (uint8_t)gePmPowerModeStatus;
    gtPmPowerModeInfoSaved.u8PowerModeRelateType = (uint8_t)gePmPowerModeRelateType;
    gtPmPowerModeInfoSaved.u16Reserved = 0;
    gtPmPowerModeInfoSaved.bPowerTimer12Expired = gbPmPowerTimer12Expired;
    gtPmPowerModeInfoSaved.bPowerTimer2Expired = gbPmPowerTimer2Expired;
    gtPmPowerModeInfoSaved.u32Sleep1StartTime = gu32PmSleep1StartTime;
    gtPmPowerModeInfoSaved.u32Sleep2StartTime = gu32PmSleep2StartTime;

    // if (ApiNvramWritAsyncInd(INDEX_NVID_0xE012, sizeof(gtPmPowerModeInfoSaved), (uint8_t *)&gtPmPowerModeInfoSaved) != TRUE)
    // {
    //     return ERR;
    // }

    return OK;
}

/******************************************************************************
*  function name | DiasPmSaveDefaultPowerModeInfoHandle
*  content       | 存储默认电源管理状态
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSaveDefaultPowerModeInfoHandle(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    if (pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    (void)DiasPmSaveDefaultPowerModeInfo();

    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmSaveCurrPowerModeInfoHandle
*  content       | 存储当前电源管理状态
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSaveCurrPowerModeInfoHandle(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{
    if (pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    (void)DiasPmSaveCurrPowerModeInfo();

    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmProcMcuTaskSleepByPriority
*  content       | 处理指定优先级的MCU模块进入休眠
*  parameter[1]  | uint8_t u8Priority：优先级
*  notice        | 无
*  return        | BOOL：是否休眠完成
******************************************************************************/
static BOOL DiasPmProcMcuTaskSleepByPriority(uint8_t u8Priority)
{
    uint16_t u16Loop = 0;
    uint8_t u8PowerModeStatus = DIAS_PM_MODULE_MODE_LISTEN;
    BOOL bHasSlept = TRUE;

    if ((gePmPowerModeStatus == PM_POWER_MODE_SLEEP3) ||
        (gePmPowerModeStatus == PM_POWER_MODE_OFF))
    {
        u8PowerModeStatus = DIAS_PM_MODULE_MODE_SLEEP;
    }

    for (; u16Loop < gu16PmMcuModuleNum; ++u16Loop)
    {
        if (gatPmMcuModuleInfo[u16Loop].u8SleepPriority != u8Priority)
        {
            continue;
        }

        if (gatPmMcuModuleInfo[u16Loop].eModuleState == DIAS_PM_SLEEP_OK)
        {
            if (gatPmMcuModuleInfo[u16Loop].hasSleptFunc != NULL)
            {
                if (gatPmMcuModuleInfo[u16Loop].hasSleptFunc() != TRUE)
                {
                    bHasSlept = FALSE;
                }
            }
        }
        else
        {
            if (gatPmMcuModuleInfo[u16Loop].changePowerModeFunc != NULL)
            {
                if (gatPmMcuModuleInfo[u16Loop].changePowerModeFunc(u8PowerModeStatus) != TRUE)
                {
                    gatPmMcuModuleInfo[u16Loop].eModuleState = DIAS_PM_SLEEP_ERR;
                    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "SleepFail:index:%d\n", u16Loop);
                    return FALSE;
                }
            }

            gatPmMcuModuleInfo[u16Loop].eModuleState = DIAS_PM_SLEEP_OK;
            if (gatPmMcuModuleInfo[u16Loop].hasSleptFunc != NULL)
            {
                if (gatPmMcuModuleInfo[u16Loop].hasSleptFunc() != TRUE)
                {
                    bHasSlept = FALSE;
                }
            }
        }
    }

    return bHasSlept;
}

/******************************************************************************
*  function name | DiasPmWaitMcuTaskSleep
*  content       | 等待MCU其他模块进入休眠
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*                | BOOL *pbNextFuncFlag：下一个处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmWaitMcuTaskSleep(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag)
{

    if ((pbCurrFuncFlag == NULL) || (*pbCurrFuncFlag != TRUE))
    {
        return;
    }

    if (DiasPmProcMcuTaskSleepByPriority(gu8CurrHandlePriority) != TRUE)
    {
        return;
    }

    ++gu8CurrHandlePriority;
    if (gu8CurrHandlePriority <= gu8PmMaxSleepPriority)
    {
        return;
    }

    /* MCU其他模块已完成休眠，可以进入下一步处理 */
    *pbCurrFuncFlag = FALSE;
    if(pbNextFuncFlag != NULL)
    {
        *pbNextFuncFlag = TRUE;
    }
}

/******************************************************************************
*  function name | DiasPmProcDeepReset
*  content       | MCU从Deepstop模式唤醒，电源模式迁移处理
*  parameter[1]  | PmPowerModeStatus_e ePowerMode：休眠时电源模式
*  notice        | 无
*  return        | INT32：OK(处理成功)/ERR(处理失败)
******************************************************************************/
static int32_t DiasPmProcDeepReset(PmPowerModeStatus_e ePowerMode)
{
    DiasPmRefreshPowerUpStatusWhenWakeupFromDeepstop();

    if (ePowerMode == PM_POWER_MODE_SLEEP1)
    {
        // gu32PmWakeupSource = DIAS_PM_SLEEP1_MODE_WAKEUP_SOURCE | DIAS_PM_RTC_WAKEUP_SOURCE;
        // DiasPmChangePowerMode(PM_POWER_MODE_OFF, ePowerMode);
        return OK;
    }
    else if (ePowerMode == PM_POWER_MODE_SLEEP2)
    {
        // gu32PmWakeupSource = DIAS_PM_SLEEP2_MODE_WAKEUP_SOURCE | DIAS_PM_RTC_WAKEUP_SOURCE;
        // DiasPmChangePowerMode(PM_POWER_MODE_OFF, ePowerMode);
        return OK;
    }
    else if (ePowerMode == PM_POWER_MODE_SLEEP3)
    {
        // gu32PmWakeupSource = DIAS_PM_SLEEP3_MODE_WAKEUP_SOURCE;
        // DiasPmChangePowerMode(PM_POWER_MODE_OFF, ePowerMode);
        return OK;
    }
    else
    {
        return ERR;
    }
}

/******************************************************************************
*  function name | DiasPmCheckMcuStartReason
*  content       | 检查MCU启动原因
*  parameter[1]  | BOOL *pbCurrFuncFlag：当前处理Flag
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmCheckMcuStartReason(BOOL *pbCurrFuncFlag)
{
    uint32_t u32ResetFactor;
    PmPowerModeStatus_e ePowerMode = DIAS_PM_POWER_MODE_INIT;
    if(pbCurrFuncFlag != NULL)
    {
        if (*pbCurrFuncFlag != TRUE)
        {
            return;
        }

        *pbCurrFuncFlag = FALSE;
    }

    DiasPmSetPowerStatusRecoveryEnable(TRUE);

    if ((gtPmPowerModeInfoSaved.u8PowerMode > PM_POWER_MODE_OFF) &&
        (gtPmPowerModeInfoSaved.u8PowerMode < PM_POWER_MODE_END))
    {
        ePowerMode = (PmPowerModeStatus_e)gtPmPowerModeInfoSaved.u8PowerMode;
    }

    if (gtPmPowerModeInfoSaved.u8PowerModeRelateType < DIAS_PM_MODE_RELATE_TYPE_NUM)
    {
        gePmPowerModeRelateType = (DiasPmPowerModeRelateType_e)gtPmPowerModeInfoSaved.u8PowerModeRelateType;
    }

    gbPmPowerTimer12Expired = gtPmPowerModeInfoSaved.bPowerTimer12Expired;
    gbPmPowerTimer2Expired = gtPmPowerModeInfoSaved.bPowerTimer2Expired;
    gu32PmSleep1StartTime = gtPmPowerModeInfoSaved.u32Sleep1StartTime;
    gu32PmSleep2StartTime = gtPmPowerModeInfoSaved.u32Sleep2StartTime;

    // u32ResetFactor = ApiStbcGetResetFactor();
    // ApiStbcClrResetFactor();
    
    // if ((u32ResetFactor & RESF_POWER_ON) != 0)
    // {
    //     /* 上电后电源控制状态刷新 */
    //     DiasPmRefreshPowerUpStatusAfterSysPowerUp();
    //     gePmPowerModeHandleState = DIAS_PM_MCU_POWER_UP;
    //     gbPmNewStateHandleStartFlag = TRUE;
    //     return;
    // }
    // else if ((u32ResetFactor & RESF_DEEP_RESET) != 0)
    // {
    //     // if (DiasPmProcDeepReset(ePowerMode) == OK)
    //     // {
    //     //     return;
    //     // }
    //     /* 上电后电源控制状态刷新 */
    //     DiasPmRefreshPowerUpStatusAfterSysPowerUp();
    //     gePmPowerModeHandleState = DIAS_PM_MCU_POWER_UP;
    //     gbPmNewStateHandleStartFlag = TRUE;
    //     return;
    // }
    // else
    // {
    //     if ((u32ResetFactor & RESF_SOFT_RESET) != 0)
    //     {
    //         DiasPmChangePowerMode(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT);
    //         return;
    //     }
    // }

    /* MCU异常重启后，电源控制关机失败后进行硬关机处理，防止系统死机导致工作异常 */
    DiasPmSetRecoveryAfterPowerCtrlFailEnable(TRUE);

    gePmPowerModeHandleState = DIAS_PM_MCU_UNEXPECT_RESET;
    gbPmNewStateHandleStartFlag = TRUE;
}

/******************************************************************************
*  function name | ApiPmPowerModeGet
*  content       | 获取当前电源模式
*  parameter[1]  | void
*  notice        | 无
*  return        | PmPowerModeStatus_e :当前电源模式
******************************************************************************/
PmPowerModeStatus_e ApiPmPowerModeGet(void)
{
    return gePmPowerModeStatus;
}

/******************************************************************************
*  function name | ApiPmResetMCU
*  content       | 复位MCU
*  parameter[1]  | void
*  notice        | 无
*  return        | int32_t:OK(处理成功)/其他(处理失败)
******************************************************************************/
int32_t ApiPmResetMCU(void)
{
    if (DiasPmSaveCurrPowerModeInfo() == OK)
    {
        delayms(5);
    }

    // ApiStbcSoftReset();

    return OK;
}

/******************************************************************************
*  function name | DiasPmProcessNetworkStatusChanging
*  content       | 处理车辆总线状态变化
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmProcessNetworkStatusChanging(void)
{
    // uint8_t u8Status = ApiComGetNwSt(CAN_CHANNEL1);
    // if (u8Status != CAN_NW_ST_SLEEP)
    // {
    //     gu32TimeCountAfterNwOff = 0;
    //     DiasPmClearSleepFailNotifyInfo();
    // }
    // else
    // {
    //     if (gu32TimeCountAfterNwOff < DIAS_PM_SLEEP_TIMEOUT_AFTER_NW_OFF)
    //     {
    //         ++gu32TimeCountAfterNwOff;
    //     }
    //     else
    //     {
    //         DiasPmNotifySleepFailToPeripheralSys();
    //     }
    // }
}

/******************************************************************************
*  function name | DiasPmOutputLowPowerCondition
*  content       | 输出低功耗进入条件
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmOutputLowPowerCondition(void)
{
    uint32_t u16t_lowpower_status = 0x0000;
    DiasPmPowerStatus_e ePMState;

    // if (app_bat_CanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 16);
    // }
    u16t_lowpower_status = u16t_lowpower_status | (1 << 16);


    if (ApiDcpdSrvCanSleep() == TRUE )
    {
        u16t_lowpower_status = u16t_lowpower_status | (1 << 15);
    }
    // if (ApiSysMgrSrvCanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 14);
    // } 

    if (ApiPmGetCurrSysPowerStatus(DIAS_PM_BLE) == DIAS_PM_STATUS_WAKE_UP )
    {
        u16t_lowpower_status = u16t_lowpower_status | ( 1 << 13 );
    }

    // if (Detect_Struct[LTE_USB_VBUS_RESULT].u8_CurResult == USB_DETECT_OPEN)
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 12);
    // }
    u16t_lowpower_status = u16t_lowpower_status | (1 << 12);

    ePMState = ApiPmGetCurrSysPowerStatus(DIAS_PM_LTE);
    if ((DiasPmIsSleepReady() == TRUE ) && ( ePMState == DIAS_PM_STATUS_WAKE_UP))
    {
        u16t_lowpower_status = u16t_lowpower_status | (1 << 11);
    }
    // if (app_btc_CanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 10);
    // }
    u16t_lowpower_status = u16t_lowpower_status | (1 << 10);

    // if (app_vcp_CanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 9 );
    // }
    u16t_lowpower_status = u16t_lowpower_status | (1 << 9 );

    // if (app_ac_CanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 8 );
    // }
    u16t_lowpower_status = u16t_lowpower_status | (1 << 8 );

    // if (app_rc_CanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 7 );
    // }
    u16t_lowpower_status = u16t_lowpower_status | (1 << 7 );

    // if (app_diag_CanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 6 );
    // }
    u16t_lowpower_status = u16t_lowpower_status | (1 << 6 );

    // if (ApiComTaskCanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 5 );
    // }
    // if (ApiDTCCanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 4 );
    // }
    // if (ApiGsenSrvCanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 3 );
    // }
    // if (ApiAuxTaskCanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 2 );
    // }
    // if (ApiWanCanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 1);
    // }
    // if (ApiNvramCanSleep() == TRUE )
    // {
    //     u16t_lowpower_status = u16t_lowpower_status | (1 << 0);
    // }

    ApiLogPrint(_LOG_DEBUG, "PM: PowerMode condition Status:0x%x\n", u16t_lowpower_status);
}
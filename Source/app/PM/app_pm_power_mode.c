/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_power_mode.c
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
* 2020-08-04   V11.01.01.01.00  徐高腾        修改ACC检测方法
* 2020-08-25   V11.01.01.01.00  徐高腾        增加休眠过程中唤醒中断处理
******************************************************************************/


/******************************************************************************
* 头文件
******************************************************************************/
#include "app_pm_power_mode.h"
#include "app_pm_mv.h"
#include "pm_public.h"
#include "app_pm_power_mode_cfg.h"
#include "app_pm_peripheral_sys.h"
#include "app_pm_vol.h"
#include "app_pm_drv.h"
#include "os_log.h"
#include "ecu_gpio.h"


/******************************************************************************
* 类型定义
******************************************************************************/
typedef void (*DiasPmRtcWakeupHandleFunc)(PmPowerModeStatus_e eCurrPowerMode);


/******************************************************************************
* 模式迁移处理标记
******************************************************************************/
static BOOL gbPmHandleBeforeMcuTaskStateSwitchFlag = FALSE;  /* 切换MCU其他模块状态前处理Flag */
static BOOL gbPmWaitMcuTaskSleepFlag = FALSE;                /* 等待MCU其他模块休眠Flag */
static BOOL gbPmWaitMcuTaskWakeupFlag = FALSE;               /* 等待MCU其他模块唤醒Flag */
static BOOL gbPmWaitPowerStatusSyncFlag = FALSE;             /* 等待电源模式同步Flag */
static BOOL gbPmChangeSysPowerStatus = FALSE;                /* 周边系统电源控制Flag */
static BOOL gbPmWaitSysPowerStatusCtrlEnd = FALSE;           /* 等待周边系统电源控制结束Flag */
static BOOL gbPmSysPowerStatusCtrlEndFlag = FALSE;           /* 周边系统电源控制结束后处理Flag */
static BOOL gbPmHandleMcuEnterLowPowerFlag = FALSE;          /* Mcu进入低功耗处理Flag */
static BOOL gbPmSaveBeforeEnterLowerPowerFlag = FALSE;       /* Mcu进入低功耗前数据存储处理Flag */
static BOOL gbPmHandleAfterMcuWakeupFlag = FALSE;            /* Mcu唤醒后电源模式处理Flag */
static BOOL gbPmLeaveLowPowerFlag = FALSE;                   /* Mcu退出低功耗模式处理Flag */
static BOOL gbPmCheckMcuStartReasonFlag = FALSE;             /* 检查Mcu启动原因处理Flag */


/******************************************************************************
* 特殊模式相关变量定义
******************************************************************************/
DiasPmPowerModeRelateType_e gePmPowerModeRelateType = DIAS_PM_MODE_RELATE_TYPE_NORMAL;


/******************************************************************************
* 其他变量定义
******************************************************************************/
uint32_t gu32PmWakeupSourceInSleeping = 0;   /* 休眠过程中唤醒中断触发标记 */


/******************************************************************************
* 内部函数声明
******************************************************************************/
static BOOL DiasPmNeedToEnterLowPower(void);
static void DiasPmHandleBeforeMcuEnterDeepstopMode(void);
static void DiasPmHandleBeforeMcuReset(void);
static void DiasPmSleepModeHandle(PmPowerModeStatus_e eCurrSleepMode, DiasPmRtcWakeupHandleFunc rtcWakeupFunc);
static void DiasPmWakeupLteShortTimeHandle(PmPowerModeStatus_e eCurrPowerMode);


/******************************************************************************
*  function name | DiasPmInitAllHandleFlag
*  content       | 清除所有的处理Flag
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmInitAllHandleFlag(void)
{
    gbPmNewStateHandleStartFlag = TRUE;              /* 新状态处理开始Flag */
    gbPmNotifyPowerStatusFlag = FALSE;               /* 同步电源模式Flag */
    gbPmHandleBeforeMcuTaskStateSwitchFlag = FALSE;  /* 切换MCU其他模块状态前处理Flag */
    gbPmWaitMcuTaskSleepFlag = FALSE;                /* 等待MCU其他模块休眠Flag */
    gbPmWaitMcuTaskWakeupFlag = FALSE;               /* 等待MCU其他模块唤醒Flag */
    gbPmWaitPowerStatusSyncFlag = FALSE;             /* 等待电源模式同步Flag */
    gbPmChangeSysPowerStatus = FALSE;                /* 周边系统电源控制Flag */
    gbPmWaitSysPowerStatusCtrlEnd = FALSE;           /* 等待周边系统电源控制结束Flag */
    gbPmSysPowerStatusCtrlEndFlag = FALSE;           /* 周边系统电源控制结束后处理Flag */
    gbPmHandleMcuEnterLowPowerFlag = FALSE;          /* Mcu进入低功耗处理Flag */
    gbPmSaveBeforeEnterLowerPowerFlag = FALSE;       /* Mcu进入低功耗前数据存储处理Flag */
    gbPmHandleAfterMcuWakeupFlag = FALSE;            /* Mcu唤醒后电源模式处理Flag */
    gbPmLeaveLowPowerFlag = FALSE;                   /* Mcu退出低功耗模式 */
    gbPmCheckMcuStartReasonFlag = FALSE;             /* 检查Mcu启动原因处理Flag */
}

/******************************************************************************
*  function name | DiasPmRunningModeChangeHandle
*  content       | PM_POWER_MODE_RUNNING模式模式迁移处理
*  parameter[1]  | void
*  notice        | C-V2X模式： 车辆点火
*                | Listen模式：车辆熄火 & KL15 OFF & CAN休眠 &
*                |             无业务消息持续了2分钟 & 以太网OFF
*                | Sleep模式： 车辆熄火 & KL15 OFF & CAN休眠 &
*                |             无业务消息持续了1分钟 & 以太网OFF &
*                |             (Listen模式超时 || 周期休眠超时)
*  return        | void
******************************************************************************/
void DiasPmRunningModeChangeHandle(void)
{
    BOOL bNeedLowPower;
    BOOL bKl30Off;
    BOOL bCrash;

    if (DiasPmIsAllPeripheralSysPowerCtrlFinished() == FALSE)
    {
        return;
    }

    /* 判断是否为无业务状态，等待进入休眠 */
    bNeedLowPower = DiasPmNeedToEnterLowPower();
    if (bNeedLowPower == TRUE)
    {
        DiasPmSwitchToLowPowerMode(PM_POWER_MODE_RUNNING, NULL);
        return;
    }

    bKl30Off = DiasPmIsKl30Off();
    if (bKl30Off != TRUE)
    {
        return;
    }

    // bCrash = DiasPmIsCrashed();
    // if (bCrash == TRUE)
    // {
    //     DiasPmChangePowerMode(PM_POWER_MODE_RUNNING, PM_POWER_MODE_BAKBAT);
    // }
    // else
    // {
    //     DiasPmChangePowerMode(PM_POWER_MODE_RUNNING, PM_POWER_MODE_POWERDOWN);
    // }
    DiasPmChangePowerMode(PM_POWER_MODE_RUNNING, PM_POWER_MODE_BAKBAT);

}

/******************************************************************************
*  function name | ApiPmIsKeyStateOff
*  content       | 获取KL15状态
*  parameter[1]  | void
*  notice        | 无
*  return        | BOOL：TRUE(熄火)/FALSE(点火)
******************************************************************************/
BOOL ApiPmIsKeyStateOff(void)
{
    if (ApiGpioInputRead(GPIO_ACC_DET) == 0)
    {
        return FALSE;
    }

    return TRUE;
}


/******************************************************************************
*  function name | ApiPmKL15IsOn
*  content       | 获取KL15状态
*  parameter[1]  | void
*  notice        | 无
*  return        | BOOL：TRUE(点火)/FALSE(熄火)
******************************************************************************/
BOOL ApiPmKL15IsOn(void)
{
    if ( ApiPmIsKeyStateOff() == TRUE )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
    
}

/******************************************************************************
*  function name | DiasPmNeedToEnterLowPower
*  content       | 判断MCU是否需要进入低功耗
*  parameter[1]  | void
*  notice        | 无
*  return        | BOOL：TRUE(进入低功耗)/FALSE(不进入低功耗)
******************************************************************************/
static BOOL DiasPmNeedToEnterLowPower(void)
{
    uint16_t u16Loop = 0;

    if (ApiPmIsKeyStateOff() == FALSE)
    {
        return FALSE;
    }

    for (; u16Loop < gu16PmMcuModuleNum; ++u16Loop)
    {
        if (gatPmMcuModuleInfo[u16Loop].sleepReadyFunc != NULL)
        {
            if (gatPmMcuModuleInfo[u16Loop].sleepReadyFunc() != TRUE)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/******************************************************************************
*  function name | DiasPmOffModeHandle
*  content       | MCU Off模式处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmOffModeHandle(void)
{
    switch (gePmPowerModeHandleState)
    {
        case DIAS_PM_HANDLE_MCU_START:
        {
            /* MCU启动后NV数据读取 */
            DiasPmReadAfterMcuStart(&gbPmNewStateHandleStartFlag, &gbPmCheckMcuStartReasonFlag);
            /* MCU启动原因检查及处理 */
            DiasPmCheckMcuStartReason(&gbPmCheckMcuStartReasonFlag);
            break;
        }
        case DIAS_PM_MCU_POWER_UP:
        {
            /* MCU退出低功耗 */
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, &gbPmNewStateHandleStartFlag);
            break;
        }
        case DIAS_PM_MCU_UNEXPECT_RESET:
        {
            /* 等待周边系统电源状态恢复默认 */
            DiasPmWaitSysPowerStatusCtrlEnd(&gbPmNewStateHandleStartFlag, &gbPmLeaveLowPowerFlag);
            /* MCU退出低功耗 */
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, &gbPmLeaveLowPowerFlag);
            break;
        }
        case DIAS_PM_MCU_STANDBY:
        {
            if (DiasPmNeedToEnterLowPower() != TRUE)
            {
                gbPmNewStateHandleStartFlag = TRUE;
                gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Stop off mode\n");
                return;
            }

            /* MCU进入低功耗前处理 */
            DiasPmHandleBeforeMcuEnterDeepstopMode();

            /* MCU进入休眠 */
            DiasPmMcuEnterDeepstopHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_ALL_SYSTEM_RESET:
        {
            if (DiasPmNeedToEnterLowPower() != TRUE)
            {
                gbPmNewStateHandleStartFlag = TRUE;
                gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Stop reset all system\n");
                return;
            }

            /* MCU重启前处理 */
            DiasPmHandleBeforeMcuReset();

            /* MCU进入休眠 */
            DiasPmMcuResetHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_ALL_SYSTEM_FORCE_RESET:
        {
            /* MCU重启前处理 */
            DiasPmHandleBeforeMcuReset();
            /* MCU进入休眠 */
            DiasPmMcuResetHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_MCU_WAKEUP:  /* 保留Wakeup状态，防止进入Deepstop，MCU未重启，导致电源模式切换功能失效 */
        {
            /* 切换MCU其他模块状态前处理 */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* 等待MCU其他模块唤醒 */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmHandleAfterMcuWakeupFlag);
            /* Mcu唤醒后电源模式处理 */
            DiasPmHandleAfterMcuWakeup(&gbPmHandleAfterMcuWakeupFlag);
            break;
        }
        case DIAS_PM_MCU_WAKEUP_BY_RTC:
        case DIAS_PM_MCU_WAKEUP_BY_SERVICE:
        {
            /* MCU退出低功耗 */
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
        case DIAS_PM_MCU_WAKEUP_INVALID_SOURCE:
        {
            /* 重新进入休眠 */
            gbPmNewStateHandleStartFlag = TRUE;
            gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY;
            DiasPmSetPowerStatusRecoveryEnable(TRUE);
            break;
        }
        case DIAS_PM_MCU_STANDBY_FAILED:
        {
            /* 切换MCU其他模块状态前处理 */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* 等待MCU其他模块唤醒 */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmLeaveLowPowerFlag);
            /* MCU退出低功耗 */
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, &gbPmLeaveLowPowerFlag);
            break;
        }
        default:
        {
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
    }
}

/******************************************************************************
*  function name | DiasPmSleepModeHandle
*  content       | MCU深度休眠处理
*  parameter[1]  | PmPowerModeStatus_e eCurrSleepMode：当前休眠模式
*                | DiasPmRtcWakeupHandleFunc wakeupFunc：唤醒处理函数
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmSleepModeHandle(PmPowerModeStatus_e eCurrSleepMode, DiasPmRtcWakeupHandleFunc rtcWakeupFunc)
{
    switch (gePmPowerModeHandleState)
    {
        case DIAS_PM_MCU_STANDBY:
        {
            if (DiasPmNeedToEnterLowPower() != TRUE)
            {
                gbPmNewStateHandleStartFlag = TRUE;
                gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Stop sleep mode\n");
                return;
            }

            /* MCU进入低功耗前处理 */
            DiasPmHandleBeforeMcuEnterDeepstopMode();

            /* MCU进入休眠 */
            DiasPmMcuEnterDeepstopHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_MCU_WAKEUP:
        {
            /* 切换MCU其他模块状态前处理 */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* 等待MCU其他模块唤醒 */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmHandleAfterMcuWakeupFlag);
            /* Mcu唤醒后电源模式处理 */
            DiasPmHandleAfterMcuWakeup(&gbPmHandleAfterMcuWakeupFlag);
            break;
        }
        case DIAS_PM_MCU_WAKEUP_BY_RTC:
        {
            if (rtcWakeupFunc != NULL)
            {
                rtcWakeupFunc(eCurrSleepMode);
            }
            else
            {
                DiasPmSwitchToLowPowerMode(eCurrSleepMode, NULL);
            }
            break;
        }
        case DIAS_PM_MCU_WAKEUP_BY_SERVICE:
        {
            /* MCU退出低功耗 */
            DiasPmLeaveLowPower(eCurrSleepMode, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
        case DIAS_PM_MCU_WAKEUP_INVALID_SOURCE:
        {
            /* 重新进入休眠 */
            gbPmNewStateHandleStartFlag = TRUE;
            gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY;
            DiasPmSetPowerStatusRecoveryEnable(TRUE);
            break;
        }
        case DIAS_PM_MCU_STANDBY_FAILED:
        {
            /* 切换MCU其他模块状态前处理 */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* 等待MCU其他模块唤醒 */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmLeaveLowPowerFlag);
            /* MCU退出低功耗 */
            DiasPmLeaveLowPower(eCurrSleepMode, DIAS_PM_POWER_MODE_INIT, &gbPmLeaveLowPowerFlag);
            break;
        }
        default:
        {
            DiasPmLeaveLowPower(eCurrSleepMode, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
    }
}

/******************************************************************************
*  function name | DiasPmWakeupLteShortTimeHandle
*  content       | 模组短时间唤醒处理
*  parameter[1]  | PmPowerModeStatus_e eCurrPowerMode
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmWakeupLteShortTimeHandle(PmPowerModeStatus_e eCurrPowerMode)
{
    DiasPmSetSysSleepReadyIgnoreFlag(DIAS_PM_LTE, TRUE);
    if (DiasPmNeedToEnterLowPower() == FALSE)
    {
        /* MCU退出低功耗 */
        DiasPmSetSysSleepReadyIgnoreFlag(DIAS_PM_LTE, FALSE);
        DiasPmLeaveLowPower(eCurrPowerMode, DIAS_PM_POWER_MODE_INIT, NULL);
        return;
    }

    /* 唤醒模组 */
    DiasPmChangeSysPowerStatusHandle(DIAS_PM_LTE, DIAS_PM_STATUS_WAKE_UP, &gbPmNewStateHandleStartFlag, &gbPmWaitPowerStatusSyncFlag);

    /* 等待电源模式切换完成 */
    DiasPmWaitPowerModeChangeEnd(&gbPmWaitPowerStatusSyncFlag, &gbPmSysPowerStatusCtrlEndFlag);

    if (gbPmSysPowerStatusCtrlEndFlag == TRUE)
    {
        if (DiasPmIsSysSleepReady(DIAS_PM_LTE, FALSE) == FALSE)
        {
            return;
        }

        gbPmSysPowerStatusCtrlEndFlag = FALSE;
        DiasPmSetSysSleepReadyIgnoreFlag(DIAS_PM_LTE, FALSE);
        DiasPmSwitchToLowPowerMode(eCurrPowerMode, NULL);
    }
}

/******************************************************************************
*  function name | DiasPmSleep1ModeHandle
*  content       | PM_POWER_MODE_SLEEP1处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSleep1ModeHandle(void)
{
    DiasPmSleepModeHandle(PM_POWER_MODE_SLEEP1, DiasPmWakeupLteShortTimeHandle);
}

/******************************************************************************
*  function name | DiasPmSleep2ModeHandle
*  content       | PM_POWER_MODE_SLEEP2处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSleep2ModeHandle(void)
{
    DiasPmSleepModeHandle(PM_POWER_MODE_SLEEP2, DiasPmWakeupLteShortTimeHandle);
}

/******************************************************************************
*  function name | DiasPmSleep3ModeHandle
*  content       | PM_POWER_MODE_SLEEP3处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmSleep3ModeHandle(void)
{
    DiasPmSleepModeHandle(PM_POWER_MODE_SLEEP3, NULL);
}

/******************************************************************************
*  function name | DiasPmBakBatModeHandle
*  content       | PM_POWER_MODE_BAKBAT处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmBakBatModeHandle(void)
{
    static BOOL bBakBatteryIsUsed = FALSE;
    BOOL bKl30Off = DiasPmIsKl30Off();
    // if ( bBakBatteryIsUsed == FALSE )
    // {
    //     bBakBatteryIsUsed = TRUE;
    //     ApiGpioOutputHigh(GPIO_NMBAT_SW);
    // }

    if (bKl30Off != TRUE)
    {        
        // if ( bBakBatteryIsUsed == TRUE)
        // {
        //     bBakBatteryIsUsed = FALSE;
        //     ApiGpioOutputHigh(GPIO_NMBAT_SW);
        // }

        DiasPmChangePowerMode(PM_POWER_MODE_BAKBAT, DIAS_PM_POWER_MODE_INIT);
    }
}

/******************************************************************************
*  function name | DiasPmPowerDownModeHandle
*  content       | PM_POWER_MODE_POWERDOWN处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmPowerDownModeHandle(void)
{
    BOOL bKl30Off = DiasPmIsKl30Off();

    switch (gePmPowerModeHandleState)
    {
        case DIAS_PM_MCU_STANDBY:
        {
            if (bKl30Off != TRUE)
            {
                gbPmNewStateHandleStartFlag = TRUE;
                gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Stop powerdown mode\n");
                return;
            }

            /* MCU进入低功耗前处理 */
            DiasPmHandleBeforeMcuEnterDeepstopMode();

            /* MCU下电 */
            DiasPmMcuPowerDownHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_MCU_STANDBY_FAILED:
        {
            /* 切换MCU其他模块状态前处理 */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* 等待MCU其他模块唤醒 */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmLeaveLowPowerFlag);
            /* MCU退出低功耗 */
            DiasPmLeaveLowPower(PM_POWER_MODE_POWERDOWN, DIAS_PM_POWER_MODE_INIT, &gbPmLeaveLowPowerFlag);
            break;
        }
        default:
        {
            DiasPmLeaveLowPower(PM_POWER_MODE_POWERDOWN, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
    }
}

/******************************************************************************
*  function name | DiasPmIsSleepReady
*  content       | 判断是否可以进入休眠
*  parameter[1]  | void
*  notice        | 无
*  return        | BOOL：TRUE(可以进入休眠)/FALSE(不可以进入休眠)
******************************************************************************/
BOOL DiasPmIsSleepReady(void)
{
    uint8_t u8Loop = 0u;
    if (gePmPowerModeRelateType != DIAS_PM_MODE_RELATE_TYPE_NORMAL)
    {
        return FALSE;
    }

    for (; u8Loop < DIAS_PM_SYS_TYPE_END; ++u8Loop)
    {
        if (DiasPmIsSysSleepReady((DiasPmSysType_e)u8Loop, TRUE) == FALSE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************
*  function name | DiasPmHandleBeforeMcuEnterDeepstopMode
*  content       | MCU深度休眠前处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmHandleBeforeMcuEnterDeepstopMode(void)
{
    /* 切换Flag到电源模式通知 */
    DiasPmSwitchFlag(&gbPmNewStateHandleStartFlag, &gbPmNotifyPowerStatusFlag);

    /* 通知LTE电源模式切换 */
    DiasPmNotifyPowerMode(&gbPmNotifyPowerStatusFlag, &gbPmWaitPowerStatusSyncFlag);

    /* 等待电源模式切换完成 */
    DiasPmWaitPowerModeChangeEnd(&gbPmWaitPowerStatusSyncFlag, &gbPmSaveBeforeEnterLowerPowerFlag);

    /* MCU进入低功耗前数据存储 */
    DiasPmSaveCurrPowerModeInfoHandle(&gbPmSaveBeforeEnterLowerPowerFlag, &gbPmHandleBeforeMcuTaskStateSwitchFlag);

    /* MCU其他模块休眠前处理 */
    DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmHandleBeforeMcuTaskStateSwitchFlag, &gbPmWaitMcuTaskSleepFlag);

    /* 等待MCU其他模块休眠 */
    DiasPmWaitMcuTaskSleep(&gbPmWaitMcuTaskSleepFlag, &gbPmChangeSysPowerStatus);

    /* 根据电源模式切换周边系统电源状态控制 */
    DiasPmChangeSysPowerStatusByPowerMode(&gbPmChangeSysPowerStatus, &gbPmWaitSysPowerStatusCtrlEnd);

    /* 等待周边系统电源控制完成 */
    DiasPmWaitSysPowerStatusCtrlEnd(&gbPmWaitSysPowerStatusCtrlEnd, &gbPmHandleMcuEnterLowPowerFlag);
}

/******************************************************************************
*  function name | DiasPmHandleBeforeMcuEnterDeepstopMode
*  content       | MCU重启前处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmHandleBeforeMcuReset(void)
{
    /* 切换Flag到电源模式通知 */
    DiasPmSwitchFlag(&gbPmNewStateHandleStartFlag, &gbPmNotifyPowerStatusFlag);

    /* 通知LTE电源模式切换 */
    DiasPmNotifyPowerMode(&gbPmNotifyPowerStatusFlag, &gbPmWaitPowerStatusSyncFlag);

    /* 等待电源模式切换完成 */
    DiasPmWaitPowerModeChangeEnd(&gbPmWaitPowerStatusSyncFlag, &gbPmSaveBeforeEnterLowerPowerFlag);

    /* MCU进入低功耗前数据存储 */
    DiasPmSaveDefaultPowerModeInfoHandle(&gbPmSaveBeforeEnterLowerPowerFlag, &gbPmHandleBeforeMcuTaskStateSwitchFlag);

    /* MCU其他模块休眠前处理 */
    DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmHandleBeforeMcuTaskStateSwitchFlag, &gbPmWaitMcuTaskSleepFlag);

    /* 等待MCU其他模块休眠 */
    DiasPmWaitMcuTaskSleep(&gbPmWaitMcuTaskSleepFlag, &gbPmChangeSysPowerStatus);

    /* 根据电源模式切换周边系统电源状态控制 */
    DiasPmChangeSysPowerStatusByPowerMode(&gbPmChangeSysPowerStatus, &gbPmWaitSysPowerStatusCtrlEnd);

    /* 等待周边系统电源全部关机 */
    DiasPmWaitAllPeripheralSysOff(&gbPmWaitSysPowerStatusCtrlEnd, &gbPmHandleMcuEnterLowPowerFlag);
}

/******************************************************************************
*  function name | Mcm63mcWakeup中断处理
*  content       | 
*  return        | 
******************************************************************************/
// ISR(Mcm63mcWakeup)
// {
//     // (VOID)GetResource(gtPmLock);
//     if (gePmPowerModeHandleState == DIAS_PM_MCU_STANDBY)
//     {
//         /* 清除所有的处理Flag */
//         DiasPmInitAllHandleFlag();

//         gbPmNewStateHandleStartFlag = TRUE;
//         gePmPowerModeHandleState = DIAS_PM_MCU_WAKEUP;
//         // gu32PmWakeupSourceInSleeping |= DIAS_PM_LTE_WAKEUP_SOURCE;
//     }
//     // (void)ReleaseResource(gtPmLock);

//     ApiLogPrintExt(PM_TASK_DEBUG, _LOG_TRACE, "Wakeup by Lte.\n");
//     Os_Clear_Mcm63mcWakeup();
// }

/******************************************************************************
*  function name | RtcWakeup中断处理
*  content       | 
*  return        | 
******************************************************************************/
// ISR(AccWakeup)
// {
//     // (VOID)GetResource(gtPmLock);
//     if (gePmPowerModeHandleState == DIAS_PM_MCU_STANDBY)
//     {
//         /* 清除所有的处理Flag */
//         DiasPmInitAllHandleFlag();

//         gbPmNewStateHandleStartFlag = TRUE;
//         gePmPowerModeHandleState = DIAS_PM_MCU_WAKEUP;
//         // gu32PmWakeupSourceInSleeping |= DIAS_PM_RTC_WAKEUP_SOURCE;
//     }
//     // (void)ReleaseResource(gtPmLock);

//     ApiLogPrintExt(PM_TASK_DEBUG, _LOG_TRACE, "Wakeup by Rtc.\n");
//     Os_Clear_AccWakeup();
// }


/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_power_mode.h
* Author:
* Description:  PM模块
* 
* Others:       other description
*
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 **********************************
* Date            Version       Author     Description
* 2020-08-03   V11.01.01.01.00   徐高腾       新建基线
* 2020-08-04   V11.01.01.01.00  徐高腾        一级休眠增加模组短暂唤醒
******************************************************************************/

#ifndef _APP_PM_POWER_MODE_H_
#define _APP_PM_POWER_MODE_H_
#include "app_pm_public_define.h"


/******************************************************************************
* 类型定义
******************************************************************************/
#define DIAS_PM_LISTEN_NOT_SUPPORT                  0u                              /* Power_Timer_1_expired为0时，不支持Listen模式 */
#define DIAS_PM_PERIOD_WAKEUP_NOT_SUPPORT           0u                              /* Power_Timer_2_expired为0时，不支持周期Wakeup */
#define DIAS_PM_ENTER_LOW_POWER_MAX_TIME            60u                             /* 进入休眠过程中最大耗时时间(s) */


/******************************************************************************
* 电源模式处理
******************************************************************************/
extern void DiasPmOffModeHandle(void);
extern void DiasPmSleep1ModeHandle(void);
extern void DiasPmSleep2ModeHandle(void);
extern void DiasPmSleep3ModeHandle(void);
extern void DiasPmRunningModeChangeHandle(void);
extern void DiasPmBakBatModeHandle(void);
extern void DiasPmPowerDownModeHandle(void);


/******************************************************************************
* 电源模式切换处理
******************************************************************************/
extern void DiasPmInitAllHandleFlag(void);
extern void DiasPmChangeSysPowerStatusByPowerMode(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmChangeSysPowerStatusHandle(DiasPmSysType_e eSysType, DiasPmPowerStatus_e ePowerStatus, BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmNotifyPowerMode(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmSwitchFlag(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmHandleBeforeMcuTaskStateSwitch(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmWaitMcuTaskWakeup(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmLeaveLowPower(PmPowerModeStatus_e eCurrPowerMode, PmPowerModeStatus_e eTargetPowerMode, BOOL *pbCurrFuncFlag);
extern void DiasPmWaitPowerModeChangeEnd(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmSetWakeupSourceHandle(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmMcuEnterDeepstopHandle(BOOL *pbCurrFuncFlag);
extern void DiasPmMcuPowerDownHandle(BOOL *pbCurrFuncFlag);
extern void DiasPmMcuResetHandle(BOOL *pbCurrFuncFlag);
extern void DiasPmChangePowerMode(PmPowerModeStatus_e ePreStatus, PmPowerModeStatus_e eNextStatus);
extern void DiasPmReadAfterMcuStart(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmFinishPowerModeSwitch(BOOL *pbCurrFuncFlag);
extern void DiasPmHandleAfterMcuWakeup(BOOL *pbCurrFuncFlag);
extern void DiasPmSaveCurrPowerModeInfoHandle(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmSaveDefaultPowerModeInfoHandle(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmWaitSysPowerStatusCtrlEnd(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmWaitAllPeripheralSysOff(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmWaitMcuTaskSleep(BOOL *pbCurrFuncFlag, BOOL *pbNextFuncFlag);
extern void DiasPmCheckMcuStartReason(BOOL *pbCurrFuncFlag);
extern void DiasPmSwitchToLowPowerMode(PmPowerModeStatus_e eCurrStatus, BOOL *pbCurrFuncFlag);
extern BOOL DiasPmIsSleepReady(void);
extern BOOL ApiPmIsKeyStateOff(void);
extern BOOL ApiPmKL15IsOn(void);


/******************************************************************************
* 外部接口声明
******************************************************************************/
extern PmPowerModeStatus_e ApiPmPowerModeGet(void);
extern void DiasPmPowerModeTimeout(void);


/******************************************************************************
* 模式迁移处理标记
******************************************************************************/
extern BOOL gbPmNewStateHandleStartFlag;             /* 新状态处理开始Flag */
extern BOOL gbPmNotifyPowerStatusFlag;               /* 同步电源模式Flag */


/******************************************************************************
* 外部变量声明
******************************************************************************/
extern DiasPmPowerModeHandleState_e gePmPowerModeHandleState;
extern DiasPmPowerModeRelateType_e gePmPowerModeRelateType;

extern BOOL      gbPmPowerTimer12Expired;      /* Power_Timer_12_expired满足 */
extern BOOL      gbPmPowerTimer2Expired;       /* Power_Timer_2_expired满足 */
extern uint32_t  gu32PmSleep1StartTime;        /* 开始进入PM_POWER_MODE_SLEEP1时刻时间 */
extern uint32_t  gu32PmWakeupSourceInSleeping; /* 休眠过程中唤醒源 */


#endif /* _APP_PM_POWER_MODE_H_ */


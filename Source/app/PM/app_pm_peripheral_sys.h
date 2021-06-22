/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_peripheral_sys.h
* Author:
* Description:  PM模块
* 
* Others:       other description
*
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 **********************************
* Date            Version       Author     Description
* 2020-08-03   V11.01.01.01.00  徐高腾        新建基线
* 2020-08-04   V11.01.01.01.00  徐高腾        一级休眠增加模组短暂唤醒
* 2020-08-05   V11.01.01.01.00  徐高腾        增加总线长时间OFF休眠超时功能
******************************************************************************/

#ifndef _APP_PM_PERIPHERAL_SYS_H_
#define _APP_PM_PERIPHERAL_SYS_H_
#include "type.h"
#include "pm_public.h"
#include "app_pm_public_define.h"
#include "app_pm_peripheral_sys_reset.h"
// #include "dias_spp_main.h"


extern void     DiasPmPeripheralSysInit(void);
extern int32_t  DiasPmPeripheralSysMonitorInit(void);
extern void     DiasPmClearSleepFailNotifyInfo(void);
extern void     DiasPmPeripheralSysInitAfterWakeup(void);
extern void     DiasPmPeripheralSysTimeout(void);
extern void     DiasPmMonitorEnable(void);
extern void     DiasPmMonitorDisable(void);
extern int32_t  DiasPmChangeSysPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerStatus_e ePowerStatus);
extern BOOL     DiasPmIsPeripheralSysPowerCtrlFinished(DiasPmSysType_e eSysType);
extern BOOL     DiasPmIsAllPeripheralSysPowerCtrlFinished(void);
extern DiasPmPowerStatus_e ApiPmGetCurrSysPowerStatus(DiasPmSysType_e eSysType);
extern void     DiasPmNotifyPowerModeToPeripheralSys(PmPowerModeStatus_e ePowerMode);
extern void     DiasPmNotifySleepFailToPeripheralSys(void);
extern BOOL     DiasPmIsPowerModeChangeEnd(PmPowerModeStatus_e eCurrPowerMode);
extern BOOL     DiasPmIsSysSleepReady(DiasPmSysType_e eSysType, BOOL bAllowIgnore);
extern void     DiasPmSetSysSleepReadyIgnoreFlag(DiasPmSysType_e eSysType, BOOL bIgnore);
extern uint32_t DiasPmGetNoServiceTime(void);
extern void     ApiPmRegisterEventListener(void);
extern void     DiasPmSetPowerStatusRecoveryEnable(BOOL bSupport);
extern void     DiasPmSetRecoveryAfterPowerCtrlFailEnable(BOOL bSupport);
extern void     DiasPmSysPowerDownBeforeLowPower(void);
extern void     DiasPmRefreshPowerUpStatusAfterSysPowerUp(void);
extern void     DiasPmRefreshPowerUpStatusWhenWakeupFromDeepstop(void);


#endif /* _APP_PM_PERIPHERAL_SYS_H_ */


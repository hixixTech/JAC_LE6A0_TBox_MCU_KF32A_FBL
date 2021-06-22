/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_peripheral_sys_cfg.h
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
* 2020-08-14   V11.01.01.01.00  徐高腾        唤醒源修改
******************************************************************************/

#ifndef _APP_PM_DRV_H_
#define _APP_PM_DRV_H_
#include "type.h"


// #define   DIAS_PM_RTC_WAKEUP_SOURCE            MCU_WAKEUP_SOURCE_RTC
// #define   DIAS_PM_LTE_WAKEUP_SOURCE            MCU_WAKEUP_SOURCE_LTE

// #define DIAS_PM_SLEEP3_MODE_WAKEUP_SOURCE  (MCU_WAKEUP_SOURCE_CAN1 | MCU_WAKEUP_SOURCE_CAN2 | MCU_WAKEUP_SOURCE_LIS1 | MCU_WAKEUP_SOURCE_LIS2 |\
//                                      MCU_WAKEUP_SOURCE_SOS | MCU_WAKEUP_SOURCE_ACC | MCU_WAKEUP_SOURCE_IGN | MCU_WAKEUP_SOURCE_BT)
// #define DIAS_PM_SLEEP2_MODE_WAKEUP_SOURCE  ( DIAS_PM_SLEEP3_MODE_WAKEUP_SOURCE | DIAS_PM_LTE_WAKEUP_SOURCE | MCU_WAKEUP_SOURCE_RTC )
// #define DIAS_PM_SLEEP1_MODE_WAKEUP_SOURCE  DIAS_PM_SLEEP2_MODE_WAKEUP_SOURCE


extern void DiasPmDrvTimeout(void);
extern void DiasPmWakeupEnable(void);
extern void DiasPmMcuDrvInit(void);
extern void DiasPmMcuEnterStop(void);
extern void DiasPmMcuEnterDeepstop(void);
extern void ApiPmPeripheralCtrlMcuRun(void);
extern void DiasPmPeripheralCtrlMcuStop(void);
extern void DiasPmPeripheralCtrlMcuDeepstop(void);
extern void DiasPmModDrvInit(void);
extern VOID DiasPmModPowerUp(VOID);
extern VOID DiasPmModPowerDown(VOID);
extern void DiasPmPeripheralCtrlModRun(void);
extern void DiasPmPeripheralCtrlModOff(void);
extern void DiasPmPeripheralCtrlModSleep(void);
extern void DiasPmEnableHwt(void);
extern void DiasPmDisableHwt(void);
extern BOOL DiasPmIsCrashed(void);


#endif /* _APP_PM_DRV_H_ */


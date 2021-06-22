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
******************************************************************************/

#ifndef _APP_PM_POWER_MODE_CFG_H_
#define _APP_PM_POWER_MODE_CFG_H_
#include "app_pm_public_define.h"
#include "pm_public.h"


/******************************************************************************
* 类型定义
******************************************************************************/
#define DIAS_PM_PRIORITY_FINAL                      0xFF                            /* MCU停止运行前才执行(休眠优先级) */
#define DIAS_PM_POWER_MODE_INIT                     PM_POWER_MODE_RUNNING           /* 初始电源模式 */


/******************************************************************************
* 电源模式切换配置声明
******************************************************************************/
extern const uint8_t gu8PmMaxSleepPriority;
extern const uint8_t gu8PmMaxWakeupPriority;
extern DiasPmMcuModuleInfo_t gatPmMcuModuleInfo[];
extern uint16_t gu16PmMcuModuleNum;
extern DiasPmPowerModeCfg_t gatPmPowerModeCfg[PM_POWER_MODE_END];
extern DiasPmPowerModeChangeCfg_t gatPmPowerModeChangeCfg[];
extern uint8_t gu8PmPowerModeChangeTypeNum;


#endif /* _APP_PM_POWER_MODE_CFG_H_ */


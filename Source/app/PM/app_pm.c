/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm.c
* Description:  PM模块
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  徐高腾        新建基线
* 2020-08-11   V11.01.01.01.00  徐高腾        将电源管理功能宏开关名改为LIB_PM_ADD
******************************************************************************/


/******************************************************************************
* 头文件
******************************************************************************/

#include "os_log.h"
#include "pm_public.h"
#include "app_pm_peripheral_sys.h"
#include "app_pm_power_mode.h"
#include "app_pm_mv.h"
#include "app_pm_drv.h"
#include "app_pm_vol.h"
#include "app_pm_public_define.h"


/******************************************************************************
* 宏定义
******************************************************************************/
#define DIAS_PM_MIN_BATY_OPERB      6500      /* B级功能电压最小值 */

#ifndef LIB_PM_ADD
#define LIB_PM_ADD TRUE
#endif

/******************************************************************************
*  function name | ApiPmInit
*  content       | PM模块初始化
*  parameter[1]  | void
*  notice        | 无
*  return        | BOOL：TRUE(初始化成功)/FALSE(初始化失败)
******************************************************************************/
BOOL ApiPmInit(void)
{
    DiasPmWakeupEnable();

    DiasPmPeripheralSysInit();

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Mcu start.\n");
    return TRUE;
}

/******************************************************************************
*  function name | ApiPmProcess10ms
*  content       | PM模块周期处理
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void ApiPmProcess10ms(void)
{
#if LIB_PM_ADD == TRUE
    DiasPmDrvTimeout();

    DiasPmRefreshBatyVolInfo();

    DiasPmPowerModeTimeout();

    /* 周边系统电源控制 */
    // (void)GetResource(gtPmLock);
    DiasPmPeripheralSysTimeout();
    // (void)ReleaseResource(gtPmLock);
#endif
}


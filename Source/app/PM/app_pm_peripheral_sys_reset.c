/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_peripheral_sys_reset.c
* Description:  PM模块
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  徐高腾        新建基线
******************************************************************************/


/******************************************************************************
* 头文件
******************************************************************************/

#include "os_log.h"
#include "app_pm_mv.h"
#include "app_pm_peripheral_sys_reset.h"
#include "app_pm_peripheral_sys_cfg.h"
#include "app_pm_peripheral_sys.h"
/******************************************************************************
* 宏定义
******************************************************************************/


/******************************************************************************
* 变量定义
******************************************************************************/
BOOL gbPmResetMcu = FALSE;


/******************************************************************************
* 内部函数声明
******************************************************************************/
static void DiasPmSysReset(DiasPmSysType_e eSysType);




/******************************************************************************
*  function name | DiasPmSysReset
*  content       | 系统重启请求
*  parameter[1]  | DiasPmSysType_e eSysType        ：系统类型
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmSysReset(DiasPmSysType_e eSysType)
{
    if (eSysType >= DIAS_PM_SYS_TYPE_END)
    {
        return;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo == NULL)
    {
        return;
    }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Reset req. sys:%d.\n", eSysType);
    gatPmSysInfo[eSysType].ptPowerInfo->u8CtrlFailCount = 0;
    gatPmSysInfo[eSysType].ptPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_RESET;
}

/******************************************************************************
*  function name | ApiPmResetBLE
*  content       | 复位BLE
*  parameter[1]  | void
*  notice        | 无
*  return        | int32_t:OK(处理成功)/其他(处理失败)
******************************************************************************/
int32_t ApiPmResetBLE(void)
{
    if (gatPmSysInfo[DIAS_PM_BLE].ptPowerInfo == NULL)
    {
        return OK;
    }

    if ((gatPmSysInfo[DIAS_PM_BLE].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE) ||
        (gatPmSysInfo[DIAS_PM_BLE].ptPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_POWER_OFF) ||
        (gatPmSysInfo[DIAS_PM_BLE].ptPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_SLEEP))
    {
        return ERR;
    }

    if (gbPmResetMcu == TRUE)
    {
        return ERR;
    }

    DiasPmSysReset(DIAS_PM_BLE);
    return OK;
}

/******************************************************************************
*  function name | ApiPmResetMPU
*  content       | 复位MPU
*  parameter[1]  | void
*  notice        | 无
*  return        | int32_t:OK(处理成功)/其他(处理失败)
******************************************************************************/
int32_t ApiPmResetMPU(void)
{
    if ((gatPmSysInfo[DIAS_PM_LTE].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE) ||
        (gatPmSysInfo[DIAS_PM_LTE].ptPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_POWER_OFF) ||
        (gatPmSysInfo[DIAS_PM_LTE].ptPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_SLEEP))
    {
        return ERR;
    }

    if (gbPmResetMcu == TRUE)
    {
        return ERR;
    }
    ApiLogPrint(_LOG_DEBUG,"PM:ApiPmResetMPU\n");
    DiasPmSysReset(DIAS_PM_LTE);

    return OK;
}


/******************************************************************************
*  function name | ApiPmGetResetMPUStatus
*  content       | 获取复位MPU状态
*  parameter[1]  | void
*  notice        | 无
*  return        | BOOL :TRUE(重启处理结束)/FALSE(正在重启)
******************************************************************************/
BOOL ApiPmGetResetMPUStatus(void)
{
    if ((gatPmSysInfo[DIAS_PM_LTE].ptPowerInfo != NULL) &&
        (gatPmSysInfo[DIAS_PM_LTE].ptPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_RESET))
    {
        return FALSE;
    }

    if (gbPmResetMcu == TRUE)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************
*  function name | DiasPmResetMCU
*  content       | 复位MCU
*  parameter[1]  | void
*  notice        | 无
*  return        | int32_t:OK(处理成功)/其他(处理失败)
******************************************************************************/
int32_t DiasPmResetMCU(void)
{
    // ApiStbcSoftReset();

    return OK;
}


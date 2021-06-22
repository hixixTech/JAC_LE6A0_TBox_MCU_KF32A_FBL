/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_peripheral_sys_reset.h
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

#ifndef _APP_PM_PERIPHERAL_SYS_RESET_H_
#define _APP_PM_PERIPHERAL_SYS_RESET_H_
#include "type.h"
#include "pm_public.h"
#include "app_pm_public_define.h"


extern int32_t  DiasPmResetUblox(VOID);
extern int32_t  ApiPmResetBLE(VOID);
extern int32_t  ApiPmResetMPU(VOID);
extern BOOL     ApiPmGetResetMPUStatus(VOID);
extern int32_t  DiasPmResetMCU(VOID);


#endif /* _APP_PM_PERIPHERAL_SYS_RESET_H_ */



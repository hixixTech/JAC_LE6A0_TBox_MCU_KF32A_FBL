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
******************************************************************************/

#ifndef _APP_PM_PERIPHERAL_SYS_CFG_H_
#define _APP_PM_PERIPHERAL_SYS_CFG_H_
#include "app_pm_public_define.h"


extern int32_t ApiPmResetBLE(VOID);

extern DiasPmSysInfo_t gatPmSysInfo[];
extern const uint8_t gu8PmSysNum;


#endif /* _APP_PM_PERIPHERAL_SYS_CFG_H_ */


/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_vol.h
* Author:
* Description:  PM模块电压管理
* 
* Others:       other description
*
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 **********************************
* Date            Version       Author     Description
* 2019-11-27   V11.01.01.01.00   徐高腾       新建基线
******************************************************************************/

#ifndef _APP_PM_VOL_H_
#define _APP_PM_VOL_H_
#include "type.h"


extern int32_t DiasPmVolInit(void);
extern void    DiasPmRefreshBatyVolInfo(void);
extern BOOL    DiasPmIsKl30Off(void);


#endif /* _APP_PM_VOL_H_ */



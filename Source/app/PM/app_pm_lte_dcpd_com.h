/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_lte_dcpd_com.h
* Author:
* Description:  PMģ��
* 
* Others:       other description
*
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** �޸���ʷ **********************************
* Date            Version       Author     Description
* 2020-08-03   V11.01.01.01.00   �����       �½�����
* 2020-08-05   V11.01.01.01.00  �����        �������߳�ʱ��OFF���߳�ʱ����
******************************************************************************/

#ifndef _APP_PM_LTE_DCPD_COM_H_
#define _APP_PM_LTE_DCPD_COM_H_
#include "type.h"
#include "app_pm_mv.h"
#include "pm_public.h"


extern void     DiasPmLteNotifyPowerMode(PmPowerModeStatus_e ePowerMode);
extern void     DiasPmLteRegisterEventListener(void);
extern int32_t  DiasPmLteNotifySleepFailure(void);


#endif /* _APP_PM_LTE_DCPD_COM_H_ */


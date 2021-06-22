/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_power_mode.h
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
* 2020-08-04   V11.01.01.01.00  �����        һ����������ģ����ݻ���
******************************************************************************/

#ifndef _APP_PM_POWER_MODE_H_
#define _APP_PM_POWER_MODE_H_
#include "app_pm_public_define.h"


/******************************************************************************
* ���Ͷ���
******************************************************************************/
#define DIAS_PM_LISTEN_NOT_SUPPORT                  0u                              /* Power_Timer_1_expiredΪ0ʱ����֧��Listenģʽ */
#define DIAS_PM_PERIOD_WAKEUP_NOT_SUPPORT           0u                              /* Power_Timer_2_expiredΪ0ʱ����֧������Wakeup */
#define DIAS_PM_ENTER_LOW_POWER_MAX_TIME            60u                             /* �������߹���������ʱʱ��(s) */


/******************************************************************************
* ��Դģʽ����
******************************************************************************/
extern void DiasPmOffModeHandle(void);
extern void DiasPmSleep1ModeHandle(void);
extern void DiasPmSleep2ModeHandle(void);
extern void DiasPmSleep3ModeHandle(void);
extern void DiasPmRunningModeChangeHandle(void);
extern void DiasPmBakBatModeHandle(void);
extern void DiasPmPowerDownModeHandle(void);


/******************************************************************************
* ��Դģʽ�л�����
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
* �ⲿ�ӿ�����
******************************************************************************/
extern PmPowerModeStatus_e ApiPmPowerModeGet(void);
extern void DiasPmPowerModeTimeout(void);


/******************************************************************************
* ģʽǨ�ƴ�����
******************************************************************************/
extern BOOL gbPmNewStateHandleStartFlag;             /* ��״̬����ʼFlag */
extern BOOL gbPmNotifyPowerStatusFlag;               /* ͬ����ԴģʽFlag */


/******************************************************************************
* �ⲿ��������
******************************************************************************/
extern DiasPmPowerModeHandleState_e gePmPowerModeHandleState;
extern DiasPmPowerModeRelateType_e gePmPowerModeRelateType;

extern BOOL      gbPmPowerTimer12Expired;      /* Power_Timer_12_expired���� */
extern BOOL      gbPmPowerTimer2Expired;       /* Power_Timer_2_expired���� */
extern uint32_t  gu32PmSleep1StartTime;        /* ��ʼ����PM_POWER_MODE_SLEEP1ʱ��ʱ�� */
extern uint32_t  gu32PmWakeupSourceInSleeping; /* ���߹����л���Դ */


#endif /* _APP_PM_POWER_MODE_H_ */


/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_power_mode.c
* Description:  PMģ��
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** �޸���ʷ ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  �����        �½�����
* 2020-08-04   V11.01.01.01.00  �����        һ����������ģ����ݻ���
* 2020-08-04   V11.01.01.01.00  �����        �޸�ACC��ⷽ��
* 2020-08-25   V11.01.01.01.00  �����        �������߹����л����жϴ���
******************************************************************************/


/******************************************************************************
* ͷ�ļ�
******************************************************************************/
#include "app_pm_power_mode.h"
#include "app_pm_mv.h"
#include "pm_public.h"
#include "app_pm_power_mode_cfg.h"
#include "app_pm_peripheral_sys.h"
#include "app_pm_vol.h"
#include "app_pm_drv.h"
#include "os_log.h"
#include "ecu_gpio.h"


/******************************************************************************
* ���Ͷ���
******************************************************************************/
typedef void (*DiasPmRtcWakeupHandleFunc)(PmPowerModeStatus_e eCurrPowerMode);


/******************************************************************************
* ģʽǨ�ƴ�����
******************************************************************************/
static BOOL gbPmHandleBeforeMcuTaskStateSwitchFlag = FALSE;  /* �л�MCU����ģ��״̬ǰ����Flag */
static BOOL gbPmWaitMcuTaskSleepFlag = FALSE;                /* �ȴ�MCU����ģ������Flag */
static BOOL gbPmWaitMcuTaskWakeupFlag = FALSE;               /* �ȴ�MCU����ģ�黽��Flag */
static BOOL gbPmWaitPowerStatusSyncFlag = FALSE;             /* �ȴ���Դģʽͬ��Flag */
static BOOL gbPmChangeSysPowerStatus = FALSE;                /* �ܱ�ϵͳ��Դ����Flag */
static BOOL gbPmWaitSysPowerStatusCtrlEnd = FALSE;           /* �ȴ��ܱ�ϵͳ��Դ���ƽ���Flag */
static BOOL gbPmSysPowerStatusCtrlEndFlag = FALSE;           /* �ܱ�ϵͳ��Դ���ƽ�������Flag */
static BOOL gbPmHandleMcuEnterLowPowerFlag = FALSE;          /* Mcu����͹��Ĵ���Flag */
static BOOL gbPmSaveBeforeEnterLowerPowerFlag = FALSE;       /* Mcu����͹���ǰ���ݴ洢����Flag */
static BOOL gbPmHandleAfterMcuWakeupFlag = FALSE;            /* Mcu���Ѻ��Դģʽ����Flag */
static BOOL gbPmLeaveLowPowerFlag = FALSE;                   /* Mcu�˳��͹���ģʽ����Flag */
static BOOL gbPmCheckMcuStartReasonFlag = FALSE;             /* ���Mcu����ԭ����Flag */


/******************************************************************************
* ����ģʽ��ر�������
******************************************************************************/
DiasPmPowerModeRelateType_e gePmPowerModeRelateType = DIAS_PM_MODE_RELATE_TYPE_NORMAL;


/******************************************************************************
* ������������
******************************************************************************/
uint32_t gu32PmWakeupSourceInSleeping = 0;   /* ���߹����л����жϴ������ */


/******************************************************************************
* �ڲ���������
******************************************************************************/
static BOOL DiasPmNeedToEnterLowPower(void);
static void DiasPmHandleBeforeMcuEnterDeepstopMode(void);
static void DiasPmHandleBeforeMcuReset(void);
static void DiasPmSleepModeHandle(PmPowerModeStatus_e eCurrSleepMode, DiasPmRtcWakeupHandleFunc rtcWakeupFunc);
static void DiasPmWakeupLteShortTimeHandle(PmPowerModeStatus_e eCurrPowerMode);


/******************************************************************************
*  function name | DiasPmInitAllHandleFlag
*  content       | ������еĴ���Flag
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmInitAllHandleFlag(void)
{
    gbPmNewStateHandleStartFlag = TRUE;              /* ��״̬����ʼFlag */
    gbPmNotifyPowerStatusFlag = FALSE;               /* ͬ����ԴģʽFlag */
    gbPmHandleBeforeMcuTaskStateSwitchFlag = FALSE;  /* �л�MCU����ģ��״̬ǰ����Flag */
    gbPmWaitMcuTaskSleepFlag = FALSE;                /* �ȴ�MCU����ģ������Flag */
    gbPmWaitMcuTaskWakeupFlag = FALSE;               /* �ȴ�MCU����ģ�黽��Flag */
    gbPmWaitPowerStatusSyncFlag = FALSE;             /* �ȴ���Դģʽͬ��Flag */
    gbPmChangeSysPowerStatus = FALSE;                /* �ܱ�ϵͳ��Դ����Flag */
    gbPmWaitSysPowerStatusCtrlEnd = FALSE;           /* �ȴ��ܱ�ϵͳ��Դ���ƽ���Flag */
    gbPmSysPowerStatusCtrlEndFlag = FALSE;           /* �ܱ�ϵͳ��Դ���ƽ�������Flag */
    gbPmHandleMcuEnterLowPowerFlag = FALSE;          /* Mcu����͹��Ĵ���Flag */
    gbPmSaveBeforeEnterLowerPowerFlag = FALSE;       /* Mcu����͹���ǰ���ݴ洢����Flag */
    gbPmHandleAfterMcuWakeupFlag = FALSE;            /* Mcu���Ѻ��Դģʽ����Flag */
    gbPmLeaveLowPowerFlag = FALSE;                   /* Mcu�˳��͹���ģʽ */
    gbPmCheckMcuStartReasonFlag = FALSE;             /* ���Mcu����ԭ����Flag */
}

/******************************************************************************
*  function name | DiasPmRunningModeChangeHandle
*  content       | PM_POWER_MODE_RUNNINGģʽģʽǨ�ƴ���
*  parameter[1]  | void
*  notice        | C-V2Xģʽ�� �������
*                | Listenģʽ������Ϩ�� & KL15 OFF & CAN���� &
*                |             ��ҵ����Ϣ������2���� & ��̫��OFF
*                | Sleepģʽ�� ����Ϩ�� & KL15 OFF & CAN���� &
*                |             ��ҵ����Ϣ������1���� & ��̫��OFF &
*                |             (Listenģʽ��ʱ || �������߳�ʱ)
*  return        | void
******************************************************************************/
void DiasPmRunningModeChangeHandle(void)
{
    BOOL bNeedLowPower;
    BOOL bKl30Off;
    BOOL bCrash;

    if (DiasPmIsAllPeripheralSysPowerCtrlFinished() == FALSE)
    {
        return;
    }

    /* �ж��Ƿ�Ϊ��ҵ��״̬���ȴ��������� */
    bNeedLowPower = DiasPmNeedToEnterLowPower();
    if (bNeedLowPower == TRUE)
    {
        DiasPmSwitchToLowPowerMode(PM_POWER_MODE_RUNNING, NULL);
        return;
    }

    bKl30Off = DiasPmIsKl30Off();
    if (bKl30Off != TRUE)
    {
        return;
    }

    // bCrash = DiasPmIsCrashed();
    // if (bCrash == TRUE)
    // {
    //     DiasPmChangePowerMode(PM_POWER_MODE_RUNNING, PM_POWER_MODE_BAKBAT);
    // }
    // else
    // {
    //     DiasPmChangePowerMode(PM_POWER_MODE_RUNNING, PM_POWER_MODE_POWERDOWN);
    // }
    DiasPmChangePowerMode(PM_POWER_MODE_RUNNING, PM_POWER_MODE_BAKBAT);

}

/******************************************************************************
*  function name | ApiPmIsKeyStateOff
*  content       | ��ȡKL15״̬
*  parameter[1]  | void
*  notice        | ��
*  return        | BOOL��TRUE(Ϩ��)/FALSE(���)
******************************************************************************/
BOOL ApiPmIsKeyStateOff(void)
{
    if (ApiGpioInputRead(GPIO_ACC_DET) == 0)
    {
        return FALSE;
    }

    return TRUE;
}


/******************************************************************************
*  function name | ApiPmKL15IsOn
*  content       | ��ȡKL15״̬
*  parameter[1]  | void
*  notice        | ��
*  return        | BOOL��TRUE(���)/FALSE(Ϩ��)
******************************************************************************/
BOOL ApiPmKL15IsOn(void)
{
    if ( ApiPmIsKeyStateOff() == TRUE )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
    
}

/******************************************************************************
*  function name | DiasPmNeedToEnterLowPower
*  content       | �ж�MCU�Ƿ���Ҫ����͹���
*  parameter[1]  | void
*  notice        | ��
*  return        | BOOL��TRUE(����͹���)/FALSE(������͹���)
******************************************************************************/
static BOOL DiasPmNeedToEnterLowPower(void)
{
    uint16_t u16Loop = 0;

    if (ApiPmIsKeyStateOff() == FALSE)
    {
        return FALSE;
    }

    for (; u16Loop < gu16PmMcuModuleNum; ++u16Loop)
    {
        if (gatPmMcuModuleInfo[u16Loop].sleepReadyFunc != NULL)
        {
            if (gatPmMcuModuleInfo[u16Loop].sleepReadyFunc() != TRUE)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/******************************************************************************
*  function name | DiasPmOffModeHandle
*  content       | MCU Offģʽ����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmOffModeHandle(void)
{
    switch (gePmPowerModeHandleState)
    {
        case DIAS_PM_HANDLE_MCU_START:
        {
            /* MCU������NV���ݶ�ȡ */
            DiasPmReadAfterMcuStart(&gbPmNewStateHandleStartFlag, &gbPmCheckMcuStartReasonFlag);
            /* MCU����ԭ���鼰���� */
            DiasPmCheckMcuStartReason(&gbPmCheckMcuStartReasonFlag);
            break;
        }
        case DIAS_PM_MCU_POWER_UP:
        {
            /* MCU�˳��͹��� */
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, &gbPmNewStateHandleStartFlag);
            break;
        }
        case DIAS_PM_MCU_UNEXPECT_RESET:
        {
            /* �ȴ��ܱ�ϵͳ��Դ״̬�ָ�Ĭ�� */
            DiasPmWaitSysPowerStatusCtrlEnd(&gbPmNewStateHandleStartFlag, &gbPmLeaveLowPowerFlag);
            /* MCU�˳��͹��� */
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, &gbPmLeaveLowPowerFlag);
            break;
        }
        case DIAS_PM_MCU_STANDBY:
        {
            if (DiasPmNeedToEnterLowPower() != TRUE)
            {
                gbPmNewStateHandleStartFlag = TRUE;
                gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Stop off mode\n");
                return;
            }

            /* MCU����͹���ǰ���� */
            DiasPmHandleBeforeMcuEnterDeepstopMode();

            /* MCU�������� */
            DiasPmMcuEnterDeepstopHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_ALL_SYSTEM_RESET:
        {
            if (DiasPmNeedToEnterLowPower() != TRUE)
            {
                gbPmNewStateHandleStartFlag = TRUE;
                gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Stop reset all system\n");
                return;
            }

            /* MCU����ǰ���� */
            DiasPmHandleBeforeMcuReset();

            /* MCU�������� */
            DiasPmMcuResetHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_ALL_SYSTEM_FORCE_RESET:
        {
            /* MCU����ǰ���� */
            DiasPmHandleBeforeMcuReset();
            /* MCU�������� */
            DiasPmMcuResetHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_MCU_WAKEUP:  /* ����Wakeup״̬����ֹ����Deepstop��MCUδ���������µ�Դģʽ�л�����ʧЧ */
        {
            /* �л�MCU����ģ��״̬ǰ���� */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* �ȴ�MCU����ģ�黽�� */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmHandleAfterMcuWakeupFlag);
            /* Mcu���Ѻ��Դģʽ���� */
            DiasPmHandleAfterMcuWakeup(&gbPmHandleAfterMcuWakeupFlag);
            break;
        }
        case DIAS_PM_MCU_WAKEUP_BY_RTC:
        case DIAS_PM_MCU_WAKEUP_BY_SERVICE:
        {
            /* MCU�˳��͹��� */
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
        case DIAS_PM_MCU_WAKEUP_INVALID_SOURCE:
        {
            /* ���½������� */
            gbPmNewStateHandleStartFlag = TRUE;
            gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY;
            DiasPmSetPowerStatusRecoveryEnable(TRUE);
            break;
        }
        case DIAS_PM_MCU_STANDBY_FAILED:
        {
            /* �л�MCU����ģ��״̬ǰ���� */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* �ȴ�MCU����ģ�黽�� */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmLeaveLowPowerFlag);
            /* MCU�˳��͹��� */
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, &gbPmLeaveLowPowerFlag);
            break;
        }
        default:
        {
            DiasPmLeaveLowPower(PM_POWER_MODE_OFF, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
    }
}

/******************************************************************************
*  function name | DiasPmSleepModeHandle
*  content       | MCU������ߴ���
*  parameter[1]  | PmPowerModeStatus_e eCurrSleepMode����ǰ����ģʽ
*                | DiasPmRtcWakeupHandleFunc wakeupFunc�����Ѵ�����
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmSleepModeHandle(PmPowerModeStatus_e eCurrSleepMode, DiasPmRtcWakeupHandleFunc rtcWakeupFunc)
{
    switch (gePmPowerModeHandleState)
    {
        case DIAS_PM_MCU_STANDBY:
        {
            if (DiasPmNeedToEnterLowPower() != TRUE)
            {
                gbPmNewStateHandleStartFlag = TRUE;
                gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Stop sleep mode\n");
                return;
            }

            /* MCU����͹���ǰ���� */
            DiasPmHandleBeforeMcuEnterDeepstopMode();

            /* MCU�������� */
            DiasPmMcuEnterDeepstopHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_MCU_WAKEUP:
        {
            /* �л�MCU����ģ��״̬ǰ���� */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* �ȴ�MCU����ģ�黽�� */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmHandleAfterMcuWakeupFlag);
            /* Mcu���Ѻ��Դģʽ���� */
            DiasPmHandleAfterMcuWakeup(&gbPmHandleAfterMcuWakeupFlag);
            break;
        }
        case DIAS_PM_MCU_WAKEUP_BY_RTC:
        {
            if (rtcWakeupFunc != NULL)
            {
                rtcWakeupFunc(eCurrSleepMode);
            }
            else
            {
                DiasPmSwitchToLowPowerMode(eCurrSleepMode, NULL);
            }
            break;
        }
        case DIAS_PM_MCU_WAKEUP_BY_SERVICE:
        {
            /* MCU�˳��͹��� */
            DiasPmLeaveLowPower(eCurrSleepMode, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
        case DIAS_PM_MCU_WAKEUP_INVALID_SOURCE:
        {
            /* ���½������� */
            gbPmNewStateHandleStartFlag = TRUE;
            gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY;
            DiasPmSetPowerStatusRecoveryEnable(TRUE);
            break;
        }
        case DIAS_PM_MCU_STANDBY_FAILED:
        {
            /* �л�MCU����ģ��״̬ǰ���� */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* �ȴ�MCU����ģ�黽�� */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmLeaveLowPowerFlag);
            /* MCU�˳��͹��� */
            DiasPmLeaveLowPower(eCurrSleepMode, DIAS_PM_POWER_MODE_INIT, &gbPmLeaveLowPowerFlag);
            break;
        }
        default:
        {
            DiasPmLeaveLowPower(eCurrSleepMode, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
    }
}

/******************************************************************************
*  function name | DiasPmWakeupLteShortTimeHandle
*  content       | ģ���ʱ�份�Ѵ���
*  parameter[1]  | PmPowerModeStatus_e eCurrPowerMode
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmWakeupLteShortTimeHandle(PmPowerModeStatus_e eCurrPowerMode)
{
    DiasPmSetSysSleepReadyIgnoreFlag(DIAS_PM_LTE, TRUE);
    if (DiasPmNeedToEnterLowPower() == FALSE)
    {
        /* MCU�˳��͹��� */
        DiasPmSetSysSleepReadyIgnoreFlag(DIAS_PM_LTE, FALSE);
        DiasPmLeaveLowPower(eCurrPowerMode, DIAS_PM_POWER_MODE_INIT, NULL);
        return;
    }

    /* ����ģ�� */
    DiasPmChangeSysPowerStatusHandle(DIAS_PM_LTE, DIAS_PM_STATUS_WAKE_UP, &gbPmNewStateHandleStartFlag, &gbPmWaitPowerStatusSyncFlag);

    /* �ȴ���Դģʽ�л���� */
    DiasPmWaitPowerModeChangeEnd(&gbPmWaitPowerStatusSyncFlag, &gbPmSysPowerStatusCtrlEndFlag);

    if (gbPmSysPowerStatusCtrlEndFlag == TRUE)
    {
        if (DiasPmIsSysSleepReady(DIAS_PM_LTE, FALSE) == FALSE)
        {
            return;
        }

        gbPmSysPowerStatusCtrlEndFlag = FALSE;
        DiasPmSetSysSleepReadyIgnoreFlag(DIAS_PM_LTE, FALSE);
        DiasPmSwitchToLowPowerMode(eCurrPowerMode, NULL);
    }
}

/******************************************************************************
*  function name | DiasPmSleep1ModeHandle
*  content       | PM_POWER_MODE_SLEEP1����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmSleep1ModeHandle(void)
{
    DiasPmSleepModeHandle(PM_POWER_MODE_SLEEP1, DiasPmWakeupLteShortTimeHandle);
}

/******************************************************************************
*  function name | DiasPmSleep2ModeHandle
*  content       | PM_POWER_MODE_SLEEP2����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmSleep2ModeHandle(void)
{
    DiasPmSleepModeHandle(PM_POWER_MODE_SLEEP2, DiasPmWakeupLteShortTimeHandle);
}

/******************************************************************************
*  function name | DiasPmSleep3ModeHandle
*  content       | PM_POWER_MODE_SLEEP3����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmSleep3ModeHandle(void)
{
    DiasPmSleepModeHandle(PM_POWER_MODE_SLEEP3, NULL);
}

/******************************************************************************
*  function name | DiasPmBakBatModeHandle
*  content       | PM_POWER_MODE_BAKBAT����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmBakBatModeHandle(void)
{
    static BOOL bBakBatteryIsUsed = FALSE;
    BOOL bKl30Off = DiasPmIsKl30Off();
    // if ( bBakBatteryIsUsed == FALSE )
    // {
    //     bBakBatteryIsUsed = TRUE;
    //     ApiGpioOutputHigh(GPIO_NMBAT_SW);
    // }

    if (bKl30Off != TRUE)
    {        
        // if ( bBakBatteryIsUsed == TRUE)
        // {
        //     bBakBatteryIsUsed = FALSE;
        //     ApiGpioOutputHigh(GPIO_NMBAT_SW);
        // }

        DiasPmChangePowerMode(PM_POWER_MODE_BAKBAT, DIAS_PM_POWER_MODE_INIT);
    }
}

/******************************************************************************
*  function name | DiasPmPowerDownModeHandle
*  content       | PM_POWER_MODE_POWERDOWN����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmPowerDownModeHandle(void)
{
    BOOL bKl30Off = DiasPmIsKl30Off();

    switch (gePmPowerModeHandleState)
    {
        case DIAS_PM_MCU_STANDBY:
        {
            if (bKl30Off != TRUE)
            {
                gbPmNewStateHandleStartFlag = TRUE;
                gePmPowerModeHandleState = DIAS_PM_MCU_STANDBY_FAILED;
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Stop powerdown mode\n");
                return;
            }

            /* MCU����͹���ǰ���� */
            DiasPmHandleBeforeMcuEnterDeepstopMode();

            /* MCU�µ� */
            DiasPmMcuPowerDownHandle(&gbPmHandleMcuEnterLowPowerFlag);
            break;
        }
        case DIAS_PM_MCU_STANDBY_FAILED:
        {
            /* �л�MCU����ģ��״̬ǰ���� */
            DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmNewStateHandleStartFlag, &gbPmWaitMcuTaskWakeupFlag);
            /* �ȴ�MCU����ģ�黽�� */
            DiasPmWaitMcuTaskWakeup(&gbPmWaitMcuTaskWakeupFlag, &gbPmLeaveLowPowerFlag);
            /* MCU�˳��͹��� */
            DiasPmLeaveLowPower(PM_POWER_MODE_POWERDOWN, DIAS_PM_POWER_MODE_INIT, &gbPmLeaveLowPowerFlag);
            break;
        }
        default:
        {
            DiasPmLeaveLowPower(PM_POWER_MODE_POWERDOWN, DIAS_PM_POWER_MODE_INIT, NULL);
            break;
        }
    }
}

/******************************************************************************
*  function name | DiasPmIsSleepReady
*  content       | �ж��Ƿ���Խ�������
*  parameter[1]  | void
*  notice        | ��
*  return        | BOOL��TRUE(���Խ�������)/FALSE(�����Խ�������)
******************************************************************************/
BOOL DiasPmIsSleepReady(void)
{
    uint8_t u8Loop = 0u;
    if (gePmPowerModeRelateType != DIAS_PM_MODE_RELATE_TYPE_NORMAL)
    {
        return FALSE;
    }

    for (; u8Loop < DIAS_PM_SYS_TYPE_END; ++u8Loop)
    {
        if (DiasPmIsSysSleepReady((DiasPmSysType_e)u8Loop, TRUE) == FALSE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************
*  function name | DiasPmHandleBeforeMcuEnterDeepstopMode
*  content       | MCU�������ǰ����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmHandleBeforeMcuEnterDeepstopMode(void)
{
    /* �л�Flag����Դģʽ֪ͨ */
    DiasPmSwitchFlag(&gbPmNewStateHandleStartFlag, &gbPmNotifyPowerStatusFlag);

    /* ֪ͨLTE��Դģʽ�л� */
    DiasPmNotifyPowerMode(&gbPmNotifyPowerStatusFlag, &gbPmWaitPowerStatusSyncFlag);

    /* �ȴ���Դģʽ�л���� */
    DiasPmWaitPowerModeChangeEnd(&gbPmWaitPowerStatusSyncFlag, &gbPmSaveBeforeEnterLowerPowerFlag);

    /* MCU����͹���ǰ���ݴ洢 */
    DiasPmSaveCurrPowerModeInfoHandle(&gbPmSaveBeforeEnterLowerPowerFlag, &gbPmHandleBeforeMcuTaskStateSwitchFlag);

    /* MCU����ģ������ǰ���� */
    DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmHandleBeforeMcuTaskStateSwitchFlag, &gbPmWaitMcuTaskSleepFlag);

    /* �ȴ�MCU����ģ������ */
    DiasPmWaitMcuTaskSleep(&gbPmWaitMcuTaskSleepFlag, &gbPmChangeSysPowerStatus);

    /* ���ݵ�Դģʽ�л��ܱ�ϵͳ��Դ״̬���� */
    DiasPmChangeSysPowerStatusByPowerMode(&gbPmChangeSysPowerStatus, &gbPmWaitSysPowerStatusCtrlEnd);

    /* �ȴ��ܱ�ϵͳ��Դ������� */
    DiasPmWaitSysPowerStatusCtrlEnd(&gbPmWaitSysPowerStatusCtrlEnd, &gbPmHandleMcuEnterLowPowerFlag);
}

/******************************************************************************
*  function name | DiasPmHandleBeforeMcuEnterDeepstopMode
*  content       | MCU����ǰ����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmHandleBeforeMcuReset(void)
{
    /* �л�Flag����Դģʽ֪ͨ */
    DiasPmSwitchFlag(&gbPmNewStateHandleStartFlag, &gbPmNotifyPowerStatusFlag);

    /* ֪ͨLTE��Դģʽ�л� */
    DiasPmNotifyPowerMode(&gbPmNotifyPowerStatusFlag, &gbPmWaitPowerStatusSyncFlag);

    /* �ȴ���Դģʽ�л���� */
    DiasPmWaitPowerModeChangeEnd(&gbPmWaitPowerStatusSyncFlag, &gbPmSaveBeforeEnterLowerPowerFlag);

    /* MCU����͹���ǰ���ݴ洢 */
    DiasPmSaveDefaultPowerModeInfoHandle(&gbPmSaveBeforeEnterLowerPowerFlag, &gbPmHandleBeforeMcuTaskStateSwitchFlag);

    /* MCU����ģ������ǰ���� */
    DiasPmHandleBeforeMcuTaskStateSwitch(&gbPmHandleBeforeMcuTaskStateSwitchFlag, &gbPmWaitMcuTaskSleepFlag);

    /* �ȴ�MCU����ģ������ */
    DiasPmWaitMcuTaskSleep(&gbPmWaitMcuTaskSleepFlag, &gbPmChangeSysPowerStatus);

    /* ���ݵ�Դģʽ�л��ܱ�ϵͳ��Դ״̬���� */
    DiasPmChangeSysPowerStatusByPowerMode(&gbPmChangeSysPowerStatus, &gbPmWaitSysPowerStatusCtrlEnd);

    /* �ȴ��ܱ�ϵͳ��Դȫ���ػ� */
    DiasPmWaitAllPeripheralSysOff(&gbPmWaitSysPowerStatusCtrlEnd, &gbPmHandleMcuEnterLowPowerFlag);
}

/******************************************************************************
*  function name | Mcm63mcWakeup�жϴ���
*  content       | 
*  return        | 
******************************************************************************/
// ISR(Mcm63mcWakeup)
// {
//     // (VOID)GetResource(gtPmLock);
//     if (gePmPowerModeHandleState == DIAS_PM_MCU_STANDBY)
//     {
//         /* ������еĴ���Flag */
//         DiasPmInitAllHandleFlag();

//         gbPmNewStateHandleStartFlag = TRUE;
//         gePmPowerModeHandleState = DIAS_PM_MCU_WAKEUP;
//         // gu32PmWakeupSourceInSleeping |= DIAS_PM_LTE_WAKEUP_SOURCE;
//     }
//     // (void)ReleaseResource(gtPmLock);

//     ApiLogPrintExt(PM_TASK_DEBUG, _LOG_TRACE, "Wakeup by Lte.\n");
//     Os_Clear_Mcm63mcWakeup();
// }

/******************************************************************************
*  function name | RtcWakeup�жϴ���
*  content       | 
*  return        | 
******************************************************************************/
// ISR(AccWakeup)
// {
//     // (VOID)GetResource(gtPmLock);
//     if (gePmPowerModeHandleState == DIAS_PM_MCU_STANDBY)
//     {
//         /* ������еĴ���Flag */
//         DiasPmInitAllHandleFlag();

//         gbPmNewStateHandleStartFlag = TRUE;
//         gePmPowerModeHandleState = DIAS_PM_MCU_WAKEUP;
//         // gu32PmWakeupSourceInSleeping |= DIAS_PM_RTC_WAKEUP_SOURCE;
//     }
//     // (void)ReleaseResource(gtPmLock);

//     ApiLogPrintExt(PM_TASK_DEBUG, _LOG_TRACE, "Wakeup by Rtc.\n");
//     Os_Clear_AccWakeup();
// }


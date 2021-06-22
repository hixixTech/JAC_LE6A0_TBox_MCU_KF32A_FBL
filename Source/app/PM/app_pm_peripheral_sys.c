/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_peripheral_sys.c
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
* 2020-08-05   V11.01.01.01.00  �����        �������߳�ʱ��OFF���߳�ʱ����
* 2020-08-11   V11.01.01.01.00  �����        �����ж��Ƿ�֧�����߻��ѿ������ż�⹦��
* 2020-08-14   V11.01.01.01.00  �����        ���ӿ�������״̬��⹦��
* 2020-08-15   V11.01.01.01.00  �����        ��Դ����ʧ��ʱ����������
* 2020-09-11   V11.01.01.01.00  �����        ����ģ������״̬��⹦��
******************************************************************************/


/******************************************************************************
* ͷ�ļ�
******************************************************************************/

#include "os_log.h"
#include "app_pm_peripheral_sys.h"
#include "app_pm_peripheral_sys_cfg.h"
#include "app_pm_mv.h"
#include "app_pm_drv.h"
#include "app_pm_lte_dcpd_com.h"

#ifdef DIAS_PM_MPU_ENABLE
#include "app_pm_mpu_dcpd_com.h"
#endif



#include "ecu_gpio.h"

#ifndef DIAS_PM_EOL

#endif


/******************************************************************************
* �궨��
******************************************************************************/
#define DIAS_PM_MIN_WAIT_VOL_STEADY_TIMER           (30u / DIAS_PM_PERIOD_TIMER)      /* ��ѹ�ȶ��ȴ�ʱ��30ms */
#define DIAS_PM_RECIEVE_SLEEP_READY_TIMEOUT         (120000u / DIAS_PM_PERIOD_TIMER)  /* �����Ƿ����������Ϣ��ʱ */


/******************************************************************************
* ��������
******************************************************************************/
static uint32_t gu32PmPowerOnTimeWaitCount = 0;
static BOOL   gbPmPowerStatusRecoveryEnable = FALSE;
static BOOL   gbPmHardPowerOffAfterCtrlFail = FALSE;
BOOL          gbPmMonitorDisable = TRUE;
BOOL          gbMonitorType = FALSE;


/******************************************************************************
* �ڲ���������
******************************************************************************/
static DiasPmPowerCtrlType_e DiasPmGetPowerCtrlType(DiasPmPowerStatus_e ePrePowerStatus, DiasPmPowerStatus_e eNextPowerStatus);
static DiasPmPowerCtrlType_e DiasPmGetSysPowerCtrlType(DiasPmPowerInfo_t *ptSysPowerInfo);
static void DiasPmSysPowerCtrl(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static DiasPmPowerStatus_e DiasPmGetPowerState(BOOL bIsSysPowerOn, BOOL bIsSysWakeup);
static void DiasPmRestoreSleepCtrlStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptSysPowerInfo);
static BOOL DiasPmGetSquWaveStatus(DiasPmFeedbackPara_t *ptFeedbackPara);
static BOOL DiasPmGetFeedbackStatus(DiasPmFeedbackInfo_t *ptPowerFeedbackInfo);
static void DiasPmRefreshCurrPowerStatus(DiasPmPowerInfo_t *ptCurrSysPowerInfo, DiasPmPowerStatus_e eFeedbackPowerStatus);
static void DiasPmRefreshFeedbackPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static BOOL DiasPmPowerCtrlEndAsPowerStatusChanged(DiasPmPowerCtrlType_e ePowerCtrlType, DiasPmPowerStatus_e eNewPowerStatus);
static void DiasPmProcFeedbackTimeout(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static void DiasPmProcPowerUpWhenChangingPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static void DiasPmSysPowerCheck(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptSysPowerInfo);

#ifndef DIAS_PM_EOL
static void DiasPmProcessSysRunAbnormal(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static void DiasPmCheckSysRunAbnormal(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
#endif

static void DiasPmProcessAfterPowerCtrl(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);
static void DiasPmProcessAfterPowerCtrlFailed(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo);

static void DiasPmProcSleepReadyStateReceiveTimeout(DiasPmSysType_e eSysType);

static void DiasPmCheckSysHeartBeat(void);

static void DiasPmPowerCtrlSaveLog(DiasPmPowerInfo_t *ptCurrSysPowerInfo);


/******************************************************************************
*  function name | DiasPmPeripheralSysInit
*  content       | ��ʼ��
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmPeripheralSysInit(void)
{
    DiasPmMcuDrvInit();
    DiasPmModDrvInit();
}

/******************************************************************************
*  function name | DiasPmClearSleepFailNotifyInfo
*  content       | �������ʧ��ͨ����Ϣ
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmClearSleepFailNotifyInfo(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo != NULL)
        {
            gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->eNotifyStatus = DIAS_PM_WAIT_SEND;
            gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->u8FailureCount = 0;
        }
    }
}

/******************************************************************************
*  function name | DiasPmPeripheralSysInitAfterWakeup
*  content       | ���Ѻ��ʼ��
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmPeripheralSysInitAfterWakeup(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo != NULL)
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->u16ForceStayLeftTime = 0;
        }
    }

    DiasPmClearSleepFailNotifyInfo();
}

/******************************************************************************
*  function name | DiasPmPeripheralSysMonitorInit
*  content       | �ܱ�ϵͳ��ع��ܳ�ʼ��
*  parameter[1]  | void
*  notice        | ��
*  return        | int32_t
******************************************************************************/
int32_t DiasPmPeripheralSysMonitorInit(void)
{
    uint8_t u8Loop;

#ifdef XA
    if (ApiNvramReadSyncInd(INDEX_MCU_MONITOR, 1, (uint8_t *)&gbMonitorType) == FALSE)
    {
        return ERR;
    }
#endif

    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if ((gatPmSysInfo[u8Loop].ptPowerInfo != NULL) &&
            (gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg != NULL))
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg->bCurrMonitorState = gbMonitorType;
        }
    }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:MonitorType:%d\n", gbMonitorType);
    return OK;
}

/******************************************************************************
*  function name | DiasPmSetPowerStatusRecoveryEnable
*  content       | ���õ�Դ״̬�Իָ���������/�ر�
*  parameter[1]  | BOOL bSupport:1(֧���Իָ�)/0(��֧���Իָ�)
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmSetPowerStatusRecoveryEnable(BOOL bSupport)
{
    gbPmPowerStatusRecoveryEnable = bSupport;
}

/******************************************************************************
*  function name | DiasPmSetRecoveryAfterPowerCtrlFailEnable
*  content       | ���õ�Դʧ���Իָ���������/�ر�
*  parameter[1]  | BOOL bSupport:1(֧���Իָ�)/0(��֧���Իָ�)
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmSetRecoveryAfterPowerCtrlFailEnable(BOOL bSupport)
{
    gbPmHardPowerOffAfterCtrlFail = bSupport;
}

/******************************************************************************
*  function name | DiasPmProcSleepReadyStateReceiveTimeout
*  content       | �����Ƿ�����������Ϣ��ʱ������
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmProcSleepReadyStateReceiveTimeout(DiasPmSysType_e eSysType)
{
    if (gatPmSysInfo[eSysType].bHasService == FALSE)
    {
        return;
    }

    if ((gatPmSysInfo[eSysType].ptPowerInfo != NULL) &&
        (gatPmSysInfo[eSysType].ptPowerInfo->eFeedbackPowerStatus != DIAS_PM_STATUS_WAKE_UP))
    {
        gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer = 0;
        return;
    }

    if (gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer < DIAS_PM_RECIEVE_SLEEP_READY_TIMEOUT)
    {
        ++gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer;
    }
    else
    {
        gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer = 0;
        gatPmSysInfo[eSysType].bIsSleepReady = TRUE;
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:System %d notify sleep ready timeout.\n", eSysType);
    }
}

/******************************************************************************
*  function name | DiasPmProcSleepFailNotify
*  content       | ����ʧ��֪ͨ
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmProcSleepFailNotify(DiasPmSysType_e eSysType)
{
    DiasPmPowerStatus_e   ePowerStatus;
    DiasPmMsgNotifyInfo_t *ptNotifyInfo;
    if (gatPmSysInfo[eSysType].ptSleepFailNotifyInfo == NULL)
    {
        return;
    }

    ptNotifyInfo = gatPmSysInfo[eSysType].ptSleepFailNotifyInfo;
    switch (ptNotifyInfo->eNotifyStatus)
    {
        case DIAS_PM_SENDING:
        {
            if (gatPmSysInfo[eSysType].ptPowerInfo != NULL)
            {
                ePowerStatus = ApiPmGetCurrSysPowerStatus(eSysType);
                if (ePowerStatus != DIAS_PM_STATUS_WAKE_UP)
                {
                    /* ���ڽ��е�Դ���ƻ�ϵͳδ���ڹ���״̬������ʧ���ݲ�֪ͨ */
                    return;
                }
            }

            if ((ptNotifyInfo->msgNotifyFunc == NULL) ||
                (ptNotifyInfo->u8FailureCount >= ptNotifyInfo->u8MaxFailureCount))
            {
                ptNotifyInfo->eNotifyStatus = DIAS_PM_NOTIFY_FAILURE;
            }
            else
            {
                (void)ptNotifyInfo->msgNotifyFunc();
                ptNotifyInfo->u16WaitTime = 0;
                ptNotifyInfo->eNotifyStatus = DIAS_PM_WAIT_ACK;
            }
            break;
        }
        case DIAS_PM_WAIT_ACK:
        {
            if (ptNotifyInfo->u16WaitTime < ptNotifyInfo->u16SendTimeout)
            {
                ++ptNotifyInfo->u16WaitTime;
            }
            else
            {
                ++ptNotifyInfo->u8FailureCount;
                ptNotifyInfo->eNotifyStatus = DIAS_PM_SENDING;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

/******************************************************************************
*  function name | DiasPmPeripheralSysTimeout
*  content       | ���ڴ�����
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmPeripheralSysTimeout(void)
{
    uint8_t u8Loop;
    DiasPmPowerInfo_t *ptCurrSysPowerInfo;
    DiasPmSysType_e eSysType;

    /* ���ϵ磬���ѹ�ȶ��ٿ��ƿ������ȴ�DIAS_PM_MIN_WAIT_VOL_STEADY_TIMER */
    if (gu32PmPowerOnTimeWaitCount < DIAS_PM_MIN_WAIT_VOL_STEADY_TIMER)
    {
        ++gu32PmPowerOnTimeWaitCount;
        return;
    }

    DiasPmCheckSysHeartBeat();

    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        eSysType = (DiasPmSysType_e)u8Loop;

        DiasPmProcSleepReadyStateReceiveTimeout(eSysType);
        DiasPmProcSleepFailNotify(eSysType);

        if (gatPmSysInfo[u8Loop].ptPowerInfo == NULL)
        {
            continue;
        }

        /* ˢ�µ�Դ״̬����(����ʱ��Դ״̬������ȴ����Ƴ�ʱ���ټ��) */
        ptCurrSysPowerInfo = gatPmSysInfo[u8Loop].ptPowerInfo;
        if (ptCurrSysPowerInfo->u16ForceStayLeftTime > 0)
        {
            --ptCurrSysPowerInfo->u16ForceStayLeftTime;
        }

        DiasPmRefreshFeedbackPowerStatus(eSysType, ptCurrSysPowerInfo);

        if ((ptCurrSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_NONE) &&
            (ptCurrSysPowerInfo->bPowerStatusConfirm == FALSE))
        {
            DiasPmSysPowerCheck(eSysType, ptCurrSysPowerInfo);
        }

        /* ��Դ״̬������ */
        DiasPmSysPowerCtrl(eSysType, ptCurrSysPowerInfo);
    }
}

void DiasPmMonitorEnable(void)
{
    DiasPmSysMonitorCfg_t *ptMonitorCfg;
    uint8_t u8Loop;

    gbPmMonitorDisable = FALSE;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if ((gatPmSysInfo[u8Loop].ptPowerInfo != NULL) &&
            (gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg != NULL))
        {
            ptMonitorCfg = gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg;
            ptMonitorCfg->u32HeartbeatDeadTimer = ptMonitorCfg->u32InitMonitorPeriod;
            if (ptMonitorCfg->ptFeedbackInfo != NULL)
            {
                ptMonitorCfg->ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft = ptMonitorCfg->ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTime;
            }

            ptMonitorCfg->bCurrMonitorState = gbMonitorType;
        }
    }
}

void DiasPmMonitorDisable(void)
{
    gbPmMonitorDisable = TRUE;
}

/******************************************************************************
*  function name | DiasPmGetSysPowerCtrlType
*  content       | ��ȡָ����Դ�Ŀ�������
*  parameter[1]  | DiasPmPowerInfo_t *ptSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | DiasPmPowerCtrlType_e��DIAS_PM_CTRL_NONE(������)
*                |                        DIAS_PM_CTRL_POWER_ON(���ƿ���)
*                |                        DIAS_PM_CTRL_POWER_OFF(���ƹػ�)
*                |                        DIAS_PM_CTRL_SLEEP(��������)
*                |                        DIAS_PM_CTRL_WAKE_UP(���ƻ���)
*                |                        DIAS_PM_CTRL_RESET(��������)
******************************************************************************/
static DiasPmPowerCtrlType_e DiasPmGetSysPowerCtrlType(DiasPmPowerInfo_t *ptSysPowerInfo)
{
    if (ptSysPowerInfo->bPowerStatusConfirm == TRUE)
    {
        return DIAS_PM_CTRL_NONE;
    }

    if (ptSysPowerInfo->u16ForceStayLeftTime > 0)
    {
        return DIAS_PM_CTRL_NONE;
    }

    return DiasPmGetPowerCtrlType(ptSysPowerInfo->eCurrPowerStatus, ptSysPowerInfo->eExpectPowerStatus);
}

/******************************************************************************
*  function name | DiasPmSysPowerCheck
*  content       | ϵͳ��Դ״̬�쳣��⼰�޸�
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*                | DiasPmPowerInfo_t *ptSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmSysPowerCheck(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptSysPowerInfo)
{
    if (gbPmPowerStatusRecoveryEnable == TRUE)
    {
        ptSysPowerInfo->ePowerCtrlType = DiasPmGetSysPowerCtrlType(ptSysPowerInfo);
    }

#ifndef DIAS_PM_EOL
    if (ApiPmBatyVolConsGet(INDEX_BATY_CONS_POWER_CTRL) != STATUS_BATY_CONS_ST_NORMAL)
    {
        return;
    }

    if (ptSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_NONE)
    {
        if (ptSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_WAKE_UP)
        {
            /* ֻ��DIAS_PM_STATUS_WAKE_UP�¼��ϵͳ����״�� */
            if ((gbPmMonitorDisable == FALSE) &&
                (ptSysPowerInfo->ptMonitorCfg != NULL))
            {
                DiasPmCheckSysRunAbnormal(eSysType, ptSysPowerInfo);
            }
        }
        else if (ptSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_RUN_ABMORMAL)
        {
            DiasPmProcessSysRunAbnormal(eSysType, ptSysPowerInfo);
        }
        else
        {
            /* do nothing */
        }
    }
#endif
}

#ifndef DIAS_PM_EOL
/******************************************************************************
*  function name | DiasPmCheckSysRunAbnormal
*  content       | ���ϵͳ����״��
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmCheckSysRunAbnormal(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    DiasPmSysMonitorCfg_t *ptCurrSysMonitorCfg;
    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    ptCurrSysMonitorCfg = ptCurrSysPowerInfo->ptMonitorCfg;
    if (ptCurrSysMonitorCfg->bCurrMonitorState != TRUE)
    {
        /* ϵͳ����Ҫ��� */
        return;
    }

    if (ptCurrSysMonitorCfg->u32HeartbeatDeadTimer > 0u)
    {
        --ptCurrSysMonitorCfg->u32HeartbeatDeadTimer;
    }
    else
    {
        ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_RUN_ABMORMAL;
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:System %d is abnormal\n", eSysType);
    }
}


/******************************************************************************
*  function name | DiasPmProcessSysRunAbnormal
*  content       | ����ϵͳ�����쳣
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | VOID
******************************************************************************/
static void DiasPmProcessSysRunAbnormal(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    uint32_t u32CurrTime;
    DiasPmSysMonitorCfg_t *ptCurrSysMonitorCfg;
    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    if ((ptCurrSysPowerInfo->ptMonitorCfg == NULL) ||
        (ptCurrSysPowerInfo->ptMonitorCfg->procSysAbnormalFunc == NULL))
    {
        ptCurrSysPowerInfo->eCurrPowerStatus = ptCurrSysPowerInfo->eFeedbackPowerStatus;
        return;
    }

    ptCurrSysMonitorCfg = ptCurrSysPowerInfo->ptMonitorCfg;
    if (ptCurrSysMonitorCfg->bSysFault == TRUE)
    {
        /* ϵͳ���Ϻ󣬲��ٴ����쳣 */
        return;
    }

    if (ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex >= DIAS_PM_MAX_SYS_ABNORMAL_COUNT)
    {
        ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex = 0;
    }

    u32CurrTime = 0;//ApiAuxTaskGetSysTimeUtc();
    if ((ptCurrSysMonitorCfg->au32SysAbnormalTime[ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex] != 0) &&
        (u32CurrTime <= (ptCurrSysMonitorCfg->au32SysAbnormalTime[ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex] + ptCurrSysMonitorCfg->u32SysAbnormalIgnorePeriod)))
    {
        /* ϵͳ�쳣�����޷��޸���ȷ��Ϊϵͳ���� */
        ptCurrSysMonitorCfg->bSysFault = TRUE;
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:System %d is fault.\n", eSysType);
        return;
    }

    if (ptCurrSysMonitorCfg->procSysAbnormalFunc() == OK)
    {
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_DEBUG, "PM:Repair system %d as abnormal.\n", eSysType);
        ptCurrSysMonitorCfg->au32SysAbnormalTime[ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex] = u32CurrTime;
        ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex += 1u;
        ptCurrSysMonitorCfg->u8NextAbnormalTimeIndex %= DIAS_PM_MAX_SYS_ABNORMAL_COUNT;
    }
}
#endif

/******************************************************************************
*  function name | DiasPmGetSquWaveStatus
*  content       | ����״̬����
*  parameter[1]  | DiasPmFeedbackPara_t tFeedbackPara����������
*  notice        | ��
*  return        | BOOL��TRUE(����)/FALSE(�쳣)
******************************************************************************/
static BOOL DiasPmGetSquWaveStatus(DiasPmFeedbackPara_t *ptFeedbackPara)
{
    uint8_t u8PinStatus = (UINT8)ApiGpioInputRead(ptFeedbackPara->tSquWavePara.u8Pin);
    if (u8PinStatus != ptFeedbackPara->tSquWavePara.u8PinStatus)
    {
        ptFeedbackPara->tSquWavePara.u8PinStatus = u8PinStatus;
        ptFeedbackPara->tSquWavePara.u16KeepTime = 0;
        return TRUE;
    }

    if ((u8PinStatus == 0) ||
        (u8PinStatus == 1))
    {
        if (ptFeedbackPara->tSquWavePara.u16KeepTime < ptFeedbackPara->tSquWavePara.u16MaxKeepTime)
        {
            ++ptFeedbackPara->tSquWavePara.u16KeepTime;
            return TRUE;
        }
    }
    else
    {
        /* do nothing */
    }

    return FALSE;
}

/******************************************************************************
*  function name | DiasPmGetFeedbackStatus
*  content       | ��ȡ״̬����
*  parameter[1]  | DiasPmFeedbackInfo_t *ptPowerFeedbackInfo����Դ������Ϣ
*  notice        | ��
*  return        | BOOL��TRUE(����)/FALSE(������)
******************************************************************************/
static BOOL DiasPmGetFeedbackStatus(DiasPmFeedbackInfo_t *ptPowerFeedbackInfo)
{
    switch (ptPowerFeedbackInfo->eFeedbackMode)
    {
        case DIAS_PM_FEEDBACK_MODE_PIN_HIGH:
        {
            if (ApiGpioInputRead(ptPowerFeedbackInfo->tFeedbackPara.tPinPara.u8Pin) == 1)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        case DIAS_PM_FEEDBACK_MODE_PIN_LOW:
        {
            if (ApiGpioInputRead(ptPowerFeedbackInfo->tFeedbackPara.tPinPara.u8Pin) == 0)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        case DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_HIGH:
        {
            if (ApiGpioOutputRead(ptPowerFeedbackInfo->tFeedbackPara.tPinPara.u8Pin) == 1)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        case DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_LOW:
        {
            if (ApiGpioOutputRead(ptPowerFeedbackInfo->tFeedbackPara.tPinPara.u8Pin) == 0)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        case DIAS_PM_FEEDBACK_MODE_ADC:
        {
	    /*
            if (Detect_Struct[ptPowerFeedbackInfo->tFeedbackPara.tChannelPara.u8Channel].u8_CurResult == ON)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }*/
	    return TRUE;
        }
        case DIAS_PM_FEEDBACK_MODE_SQU_WAKE:
        {
            return DiasPmGetSquWaveStatus(&(ptPowerFeedbackInfo->tFeedbackPara));
        }
        default:
        {
            break;
        }
    }

    return ptPowerFeedbackInfo->bFeedbackStatus;
}

/******************************************************************************
*  function name | DiasPmRefreshFeedbackPowerStatus
*  content       | �޸����߻��ѿ���״̬
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*                | DiasPmPowerInfo_t *ptSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmRestoreSleepCtrlStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptSysPowerInfo)
{
    if (ptSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_CTRL].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
    {
        return;
    }

    if ((ptSysPowerInfo->u16ForceStayLeftTime > 0) ||
        (ptSysPowerInfo->bPowerStatusConfirm == TRUE))
    {
        return;
    }

    if (DiasPmIsPeripheralSysPowerCtrlFinished(eSysType) != TRUE)
    {
        return;
    }

    if (ptSysPowerInfo->eExpectPowerStatus == DIAS_PM_STATUS_WAKE_UP)
    {
        if (ptSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_CTRL].bFeedbackStatus != TRUE)
        {
            ptSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_WAKE_UP;
            ptSysPowerInfo->u8CtrlFailCount = 0;  /* ��Դ����ʧ�ܴ������� */
        }
    }
    else if (ptSysPowerInfo->eExpectPowerStatus == DIAS_PM_STATUS_SLEEP)
    {
        if (ptSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_CTRL].bFeedbackStatus == TRUE)
        {
            ptSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_SLEEP;
            ptSysPowerInfo->u8CtrlFailCount = 0;  /* ��Դ����ʧ�ܴ������� */
        }
    }
    else
    {
        /* do nothing */
    }
}

/******************************************************************************
*  function name | DiasPmRefreshCurrPowerStatus
*  content       | ˢ�µ�ǰ��Դ״̬
*  parameter[1]  | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*                | DiasPmPowerStatus_e eFeedbackPowerStatus���µ�Դ״̬
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmRefreshCurrPowerStatus(DiasPmPowerInfo_t *ptCurrSysPowerInfo, DiasPmPowerStatus_e eFeedbackPowerStatus)
{
    if ((ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_WAKE_UP) &&
        (ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_UNSTABLE) &&
        (ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_SLEEP) &&
        (ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_POWER_OFF))
    {
        /* �쳣��Դ״̬��Ҫ������Դ�����޸�����Դ���ƹ����е�Դ״̬ΪDIAS_PM_STATUS_UNSTABLE */
        return;
    }

    if (ptCurrSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_RESET)
    {
        /* �������Ƶ�Դ״̬�ȴ���������ٸ��� */
        return;
    }

    if (DiasPmPowerCtrlEndAsPowerStatusChanged(ptCurrSysPowerInfo->ePowerCtrlType, eFeedbackPowerStatus) == TRUE)
    {
        ptCurrSysPowerInfo->eCurrPowerStatus = eFeedbackPowerStatus;
    }
}

/******************************************************************************
*  function name | DiasPmRefreshFeedbackPowerStatus
*  content       | ˢ�µ�Դ״̬����
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmRefreshFeedbackPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    DiasPmPowerStatus_e eNewFeedbackPowerStatus;
    DiasPmFeedbackInfo_t *ptPowerFeedbackInfo;
    uint8_t u8FeedbackTypeNum = (uint8_t)DIAS_PM_FEEDBACK_TYPE_END;
    uint8_t u8Loop = 0;
    BOOL bPowerStatusConfirm = FALSE;
    BOOL bFeedbackStatus;
    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    for (; u8Loop < u8FeedbackTypeNum; ++u8Loop)
    {
        ptPowerFeedbackInfo = ptCurrSysPowerInfo->atPowerFeedbackInfo + u8Loop;
        bFeedbackStatus = DiasPmGetFeedbackStatus(ptPowerFeedbackInfo);
        if (bFeedbackStatus == ptPowerFeedbackInfo->bFeedbackStatus)
        {
            ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft = ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTime;
            continue;
        }

        if (ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft > 0)
        {
            --ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft;
            bPowerStatusConfirm = TRUE;
            continue;
        }

        if (bFeedbackStatus == TRUE)
        {
            if (ptCurrSysPowerInfo->u16ForceStayLeftTime < ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16WorkMinStay)
            {
                ptCurrSysPowerInfo->u16ForceStayLeftTime = ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16WorkMinStay;
            }
        }
        else
        {
            if (ptCurrSysPowerInfo->u16ForceStayLeftTime < ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16OutWorkMinStay)
            {
                ptCurrSysPowerInfo->u16ForceStayLeftTime = ptPowerFeedbackInfo->tFeedbackConfirmInfo.u16OutWorkMinStay;
            }
        }

        ptPowerFeedbackInfo->bFeedbackStatus = bFeedbackStatus;
    }

    ptCurrSysPowerInfo->bPowerStatusConfirm = bPowerStatusConfirm;

    /* ֻ�е�Դ״̬���������仯����ˢ�µ�ǰ��Դ״̬ */
    eNewFeedbackPowerStatus = DiasPmGetPowerState(ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].bFeedbackStatus,
                                               ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].bFeedbackStatus);
    if (eNewFeedbackPowerStatus != ptCurrSysPowerInfo->eFeedbackPowerStatus)
    {
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Power status changed.sys:%d,type:%d,sta:%d\n", eSysType, ptCurrSysPowerInfo->ePowerCtrlType, eNewFeedbackPowerStatus);
        ptCurrSysPowerInfo->eFeedbackPowerStatus = eNewFeedbackPowerStatus;
    }

    DiasPmRefreshCurrPowerStatus(ptCurrSysPowerInfo, eNewFeedbackPowerStatus);

    DiasPmRestoreSleepCtrlStatus(eSysType, ptCurrSysPowerInfo);
}

/******************************************************************************
*  function name | DiasPmPowerCtrlEndAsPowerStatusChanged
*  content       | ��Դ״̬�仯������Դ���Ƴɹ�
*                | DiasPmPowerCtrlType_e ePowerCtrlType����Դ��������
*                | DiasPmPowerStatus_e eNewPowerStatus���µ�Դ״̬
*  notice        | ��
*  return        | BOOL
******************************************************************************/
static BOOL DiasPmPowerCtrlEndAsPowerStatusChanged(DiasPmPowerCtrlType_e ePowerCtrlType, DiasPmPowerStatus_e eNewPowerStatus)
{
    switch (ePowerCtrlType)
    {
        case DIAS_PM_CTRL_NONE:
        {
            return TRUE;
        }
        case DIAS_PM_CTRL_POWER_OFF:
        case DIAS_PM_CTRL_HARD_POWER_OFF:
        {
            if (eNewPowerStatus == DIAS_PM_STATUS_POWER_OFF)
            {
                return TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_POWER_ON:
        {
            if (eNewPowerStatus == DIAS_PM_STATUS_WAKE_UP)
            {
                return TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_WAKE_UP:
        {
            if (eNewPowerStatus == DIAS_PM_STATUS_WAKE_UP)
            {
                return TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_SLEEP:
        {
            if (eNewPowerStatus == DIAS_PM_STATUS_SLEEP)
            {
                return TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_RESET:
        {
            return TRUE;
        }
        default:
            break;
    }

    return FALSE;
}

/******************************************************************************
*  function name | DiasPmProcFeedbackTimeout
*  content       | ��Դ״̬������ʱ����
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmProcFeedbackTimeout(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    DiasPmPowerStatus_e eNewFeedbackPowerStatus;

    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    switch (ptCurrSysPowerInfo->ePowerCtrlType)
    {
        case DIAS_PM_CTRL_POWER_OFF:
        {
            if (ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
            {
                /* ��֧�ֿ��ػ�״̬����������Ϊ�ػ��ɹ� */
                ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].bFeedbackStatus = FALSE;
            }
            break;
        }
        case DIAS_PM_CTRL_POWER_ON:
        {
            if (ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
            {
                /* ��֧�ֿ��ػ�״̬����������Ϊ�����ɹ� */
                ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].bFeedbackStatus = TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_WAKE_UP:
        {
            if (ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
            {
                /* ��֧�����߻���״̬����������Ϊ���ѳɹ� */
                ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].bFeedbackStatus = TRUE;
            }
            break;
        }
        case DIAS_PM_CTRL_SLEEP:
        {
            if (ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].eFeedbackMode == DIAS_PM_FEEDBACK_MODE_NONE)
            {
                /* ��֧�����߻���״̬����������Ϊ���߳ɹ� */
                ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].bFeedbackStatus = FALSE;
            }
            break;
        }
        default:
            break;
    }

    eNewFeedbackPowerStatus = DiasPmGetPowerState(ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_POWERON].bFeedbackStatus,
                                               ptCurrSysPowerInfo->atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS].bFeedbackStatus);
    ApiLogPrint(_LOG_DEBUG,"PM:TIMEOUT eSysType:%d  %d %d",eSysType,eNewFeedbackPowerStatus,ptCurrSysPowerInfo->eFeedbackPowerStatus);
    ptCurrSysPowerInfo->eFeedbackPowerStatus = eNewFeedbackPowerStatus;
    ptCurrSysPowerInfo->eCurrPowerStatus = ptCurrSysPowerInfo->eFeedbackPowerStatus;
}

/******************************************************************************
*  function name | DiasPmGetPowerState
*  content       | ���ݿ��ػ�״̬������״̬��ȡ��Դ״̬
*  parameter[1]  | BOOL bIsSysPowerOn���Ƿ��ڿ���״̬
*                | BOOL bIsSysWakeup ���Ƿ��ڻ���״̬
*  notice        | ��
*  return        | DiasPmPowerStatus_e��DIAS_PM_STATUS_POWER_OFF(�ػ�)
*                |                      DIAS_PM_STATUS_SLEEP(����)
*                |                      DIAS_PM_STATUS_WAKE_UP(����)
******************************************************************************/
static DiasPmPowerStatus_e DiasPmGetPowerState(BOOL bIsSysPowerOn, BOOL bIsSysWakeup)
{
    if (bIsSysPowerOn == FALSE)
    {
        return DIAS_PM_STATUS_POWER_OFF;
    }

    if (bIsSysWakeup != TRUE)
    {
        return DIAS_PM_STATUS_SLEEP;
    }

    return DIAS_PM_STATUS_WAKE_UP;
}

/******************************************************************************
*  function name | DiasPmPowerCtrlSaveLog
*  content       | �洢��Դ����Log
*  parameter[1]  | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmPowerCtrlSaveLog(DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
}

/******************************************************************************
*  function name | DiasPmSysPowerCtrl
*  content       | ��Դ���ƴ���
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmSysPowerCtrl(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    const DiasPmPowerCtrlCfg_t *ptPowerCtrlCfg;

    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    ptPowerCtrlCfg = ptCurrSysPowerInfo->atPowerCtrlCfg + (uint8_t)ptCurrSysPowerInfo->ePowerCtrlType;

    do
    {
        ptCurrSysPowerInfo->ePrePowerCtrlStatus = ptCurrSysPowerInfo->ePowerCtrlStatus;
        switch (ptCurrSysPowerInfo->ePowerCtrlStatus)
        {
            case DIAS_PM_CTRL_IDLE:
            {
                if (ptCurrSysPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
                {
                    if (ptPowerCtrlCfg->ptCtrlStepCfg == NULL)
                    {
                        ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_NONE;
                        ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_POWER_CTRL_FAULT;
                        return;
                    }
                    else
                    {
                        if (ptCurrSysPowerInfo->bPowerStatusConfirm == TRUE)
                        {
                            ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_NONE;
                            return;
                        }
                    }

                    ptCurrSysPowerInfo->u8CurrStep = 0;
                    ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_STRAT;
                    ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_UNSTABLE;   /* ���ƿ�ʼ���Դ״̬ΪDIAS_PM_STATUS_UNSTABLE��ֱ����Դ���Ƴɹ���ʱ���ٴθ��� */
                    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:PowerCtrl start.sys:%d,type:%d.\n", eSysType, ptCurrSysPowerInfo->ePowerCtrlType);
                }
                break;
            }
            case DIAS_PM_CTRL_STRAT:
            {
                if (ptPowerCtrlCfg->ptCtrlStepCfg[ptCurrSysPowerInfo->u8CurrStep].ctrlFunc != NULL)
                {
                    if (ptPowerCtrlCfg->ptCtrlStepCfg[ptCurrSysPowerInfo->u8CurrStep].ctrlFunc(ptPowerCtrlCfg->ptCtrlStepCfg[ptCurrSysPowerInfo->u8CurrStep].u8Para) != OK)
                    {
                        return;
                    }

                    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_DEBUG, "PM:PowerCtrl.sys:%d,type:%d,step:%d.\n", eSysType, ptCurrSysPowerInfo->ePowerCtrlType, ptCurrSysPowerInfo->u8CurrStep);
                }
                ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_KEEP;
                break;
            }
            case DIAS_PM_CTRL_KEEP:
            {
                if (ptCurrSysPowerInfo->u32CtrlTimerCount < ptPowerCtrlCfg->ptCtrlStepCfg[ptCurrSysPowerInfo->u8CurrStep].u32CtrlTime)
                {
                    ++ptCurrSysPowerInfo->u32CtrlTimerCount;
                }
                else
                {
                    ptCurrSysPowerInfo->u32CtrlTimerCount = 0u;
                    ++ptCurrSysPowerInfo->u8CurrStep;
                    if (ptCurrSysPowerInfo->u8CurrStep < ptPowerCtrlCfg->u8CtrlStepNum)
                    {
                        ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_STRAT;
                    }
                    else
                    {
                        ptCurrSysPowerInfo->u8CurrStep = 0;
                        ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_FEEDBACK;
                        ptCurrSysPowerInfo->bPowerStatusConfirm = TRUE;
                    }
                }
                break;
            }
            case DIAS_PM_CTRL_FEEDBACK:
            {
                if (ptCurrSysPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_UNSTABLE)
                {
                    /* �յ�������������ƽ��� */
                    ptCurrSysPowerInfo->u32FeedbackTimerCount = 0u;
                    DiasPmProcessAfterPowerCtrl(eSysType, ptCurrSysPowerInfo);
                    ptCurrSysPowerInfo->ePrePowerCtrlStatus = DIAS_PM_CTRL_IDLE;
                    ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_IDLE;
                    ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_NONE;
                }
                else if (ptCurrSysPowerInfo->u32FeedbackTimerCount < ptPowerCtrlCfg->u16MaxFeedbackTime)
                {
                    ++ptCurrSysPowerInfo->u32FeedbackTimerCount;
                }
                else
                {
                    /* ��֧�ֵ�Դ״̬�������Դ����ʧ�� */
                    ptCurrSysPowerInfo->u32FeedbackTimerCount = 0u;
                    ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_TIMEOUT;
                    ApiLogPrint(_LOG_DEBUG,"PM:FeedBack Timeout %d\n",eSysType);
                }
                break;
            }
            default:
            {
                DiasPmProcFeedbackTimeout(eSysType, ptCurrSysPowerInfo);
                DiasPmProcessAfterPowerCtrl(eSysType, ptCurrSysPowerInfo);
                ptCurrSysPowerInfo->ePrePowerCtrlStatus = DIAS_PM_CTRL_IDLE;
                ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_IDLE;
                ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_NONE;
                break;
            }
        }
    } while(ptCurrSysPowerInfo->ePrePowerCtrlStatus != ptCurrSysPowerInfo->ePowerCtrlStatus);
}

/******************************************************************************
*  function name | DiasPmProcessAfterPowerCtrlFailed
*  content       | ��Դ����ʧ�ܺ���
*  parameter[1]  | DiasPmSysType_e eSysType        ��ϵͳ����
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmProcessAfterPowerCtrlFailed(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:PowerCtrl fail.sys:%d,type:%d.\n", eSysType, ptCurrSysPowerInfo->ePowerCtrlType);
    if (ptCurrSysPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_HARD_POWER_OFF)
    {
        ++ptCurrSysPowerInfo->u8CtrlFailCount;
        if (ptCurrSysPowerInfo->u8CtrlFailCount < DIAS_PM_MAX_POWER_CTRL_TIMES)
        {
            return;
        }

        ptCurrSysPowerInfo->u8CtrlFailCount = 0;
        if ((gbPmHardPowerOffAfterCtrlFail == TRUE) &&
            (ptCurrSysPowerInfo->atPowerCtrlCfg[DIAS_PM_CTRL_HARD_POWER_OFF].ptCtrlStepCfg != NULL))
        {
            /* ��Դ����ʧ�ܴ����������ޣ�ϵͳ����������ִ��Ӳ�ػ����� */
            ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_POWER_CTRL_FAILURE;
            if (ptCurrSysPowerInfo->eExpectPowerStatus == DIAS_PM_STATUS_SLEEP)
            {
                /* ����ʧ�ܺ��Ϊ����Ӳ�ػ������ػ��ɹ��������ٿ���Ϊ����״̬ */
                ptCurrSysPowerInfo->eExpectPowerStatus = DIAS_PM_STATUS_POWER_OFF;
            }

            return;
        }
    }

    ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_POWER_CTRL_FAULT;
}

/******************************************************************************
*  function name | DiasPmProcessAfterPowerCtrl
*  content       | ��Դ������ɺ���
*  parameter[1]  | DiasPmSysType_e eSysType        ��ϵͳ����
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmProcessAfterPowerCtrl(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    if (ptCurrSysPowerInfo == NULL)
    {
        return;
    }

    if (DiasPmPowerCtrlEndAsPowerStatusChanged(ptCurrSysPowerInfo->ePowerCtrlType, ptCurrSysPowerInfo->eCurrPowerStatus) == TRUE)
    {
        ptCurrSysPowerInfo->u8CtrlFailCount = 0;
        if (ptCurrSysPowerInfo->ptMonitorCfg != NULL)
        {
            ptCurrSysPowerInfo->ptMonitorCfg->u32HeartbeatDeadTimer = ptCurrSysPowerInfo->ptMonitorCfg->u32InitMonitorPeriod;
            if (ptCurrSysPowerInfo->ptMonitorCfg->ptFeedbackInfo != NULL)
            {
                ptCurrSysPowerInfo->ptMonitorCfg->ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft =
                    ptCurrSysPowerInfo->ptMonitorCfg->ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTime;
            }
            
            if (ptCurrSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_POWER_OFF)
            {
                ptCurrSysPowerInfo->ptMonitorCfg->bSysFault = FALSE;
                (void)memset(ptCurrSysPowerInfo->ptMonitorCfg->au32SysAbnormalTime, 0, sizeof(ptCurrSysPowerInfo->ptMonitorCfg->au32SysAbnormalTime));
                ptCurrSysPowerInfo->ptMonitorCfg->u8NextAbnormalTimeIndex = 0;
            }
        }

        gatPmSysInfo[eSysType].bIsSleepReady = gatPmSysInfo[eSysType].bIsSleepReadyDefault;
        gatPmSysInfo[eSysType].u16NoSleepReadyMsgTimer = 0;
        gatPmSysInfo[eSysType].u32NoServiceStartTime = 0;
    }
    else
    {
        DiasPmProcessAfterPowerCtrlFailed(eSysType, ptCurrSysPowerInfo);
    }

    DiasPmPowerCtrlSaveLog(ptCurrSysPowerInfo);
}

/******************************************************************************
*  function name | DiasPmChangeSysPowerStatus
*  content       | �޸�ϵͳ���յĵ�Դ״̬
*  parameter[1]  | DiasPmSysType_e eSysType        ��ϵͳ����
*                | DiasPmPowerStatus_e ePowerStatus����Դ״̬
*  notice        | ��
*  return        | int32_t
******************************************************************************/
int32_t DiasPmChangeSysPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerStatus_e ePowerStatus)
{
    DiasPmPowerInfo_t *ptCurrSysPowerInfo;
    if (eSysType >= DIAS_PM_SYS_TYPE_END)
    {
        return ERR;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo == NULL)
    {
        return OK;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->aPeripheralUintCtrlFunc[ePowerStatus] != NULL)
    {
        (gatPmSysInfo[eSysType].ptPowerInfo->aPeripheralUintCtrlFunc[ePowerStatus])();
    }

    ptCurrSysPowerInfo = gatPmSysInfo[eSysType].ptPowerInfo;

    ptCurrSysPowerInfo->u8CtrlFailCount = 0;  /* ��Դ����ʧ�ܴ������� */
    if ((ptCurrSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_POWER_CTRL_FAULT) ||
        (ptCurrSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_POWER_CTRL_FAILURE))
    {
        /* �յ��µ�Դ������������֮ǰ��Դ����ʧ�ܣ�������������Դ���� */
        ptCurrSysPowerInfo->eCurrPowerStatus = ptCurrSysPowerInfo->eFeedbackPowerStatus;
    }

#ifndef DIAS_PM_EOL
    ptCurrSysPowerInfo->eLastExpectPowerStatus = ptCurrSysPowerInfo->eExpectPowerStatus;
    ptCurrSysPowerInfo->eExpectPowerStatus = ePowerStatus;

    if (ptCurrSysPowerInfo->ePowerCtrlType == DIAS_PM_CTRL_NONE)
    {
        ptCurrSysPowerInfo->ePowerCtrlType = DiasPmGetSysPowerCtrlType(ptCurrSysPowerInfo);
        DiasPmProcPowerUpWhenChangingPowerStatus(eSysType, ptCurrSysPowerInfo);
    }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:PowerCtrl req sys:%d,exp:%d,cur:%d.\n", eSysType, ePowerStatus, ptCurrSysPowerInfo->eCurrPowerStatus);
#else
    if (gbPmResetMcu == TRUE)
    {
        return ERR;
    }

    if ((ptCurrSysPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE) ||
        (ptCurrSysPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_UNSTABLE) ||
        (ptCurrSysPowerInfo->bPowerStatusConfirm != FALSE))
    {
        return ERR;
    }

    ptCurrSysPowerInfo->eExpectPowerStatus = ePowerStatus;
    ptCurrSysPowerInfo->ePowerCtrlType = DiasPmGetSysPowerCtrlType(ptCurrSysPowerInfo);
    DiasPmProcPowerUpWhenChangingPowerStatus(eSysType, ptCurrSysPowerInfo);

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:PowerCtrl req sys:%d,exp:%d,cur:%d\n", eSysType, ePowerStatus, ptCurrSysPowerInfo->eCurrPowerStatus);
#endif

    return OK;
}

/******************************************************************************
*  function name | DiasPmGetPowerCtrlType
*  content       | ��ȡ��������
*  parameter[1]  | DiasPmPowerStatus_e ePrePowerMode����ǰ��Դ״̬
*                | DiasPmPowerStatus_e eNextPowerStatus��Ŀ���Դ״̬
*  notice        | ��
*  return        | DiasPmPowerCtrlType_e��DIAS_PM_CTRL_NONE(������)
*                |                        DIAS_PM_CTRL_POWER_ON(���ƿ���)
*                |                        DIAS_PM_CTRL_POWER_OFF(���ƹػ�)
*                |                        DIAS_PM_CTRL_SLEEP(��������)
*                |                        DIAS_PM_CTRL_WAKE_UP(���ƻ���)
*                |                        DIAS_PM_CTRL_RESET(��������)
******************************************************************************/
static DiasPmPowerCtrlType_e DiasPmGetPowerCtrlType(DiasPmPowerStatus_e ePrePowerStatus, DiasPmPowerStatus_e eNextPowerStatus)
{
    switch (ePrePowerStatus)
    {
        case DIAS_PM_STATUS_POWER_OFF:
        {
            if (eNextPowerStatus != DIAS_PM_STATUS_POWER_OFF)
            {
                /* ������WAKEUP״̬����SLEEP״̬�������ȿ��� */
                return DIAS_PM_CTRL_POWER_ON;
            }
            break;
        }
        case DIAS_PM_STATUS_WAKE_UP:
        {
            if (eNextPowerStatus == DIAS_PM_STATUS_POWER_OFF)
            {
                /* �ػ� */
                return DIAS_PM_CTRL_POWER_OFF;
            }

            if (eNextPowerStatus == DIAS_PM_STATUS_SLEEP)
            {
                /* ���� */
                return DIAS_PM_CTRL_SLEEP;
            }
            break;
        }
        case DIAS_PM_STATUS_SLEEP:
        {
            if (eNextPowerStatus == DIAS_PM_STATUS_POWER_OFF)
            {
                /* ��������²���ֱ�ӹػ�����Ҫ�Ȼ��� */
                return DIAS_PM_CTRL_WAKE_UP;
            }

            if (eNextPowerStatus != DIAS_PM_STATUS_SLEEP)
            {
                /* ������WAKEUP״̬����OFF״̬�������Ȼ��� */
                return DIAS_PM_CTRL_WAKE_UP;
            }
            break;
        }
        case DIAS_PM_STATUS_RUN_ABMORMAL:
        {
            if (eNextPowerStatus == DIAS_PM_STATUS_POWER_OFF)
            {
                /* ϵͳ�����쳣��ֻ����ػ��������������(�˴�ֻ����ػ�) */
                return DIAS_PM_CTRL_POWER_OFF;
            }
            break;
        }
        case DIAS_PM_STATUS_POWER_CTRL_FAILURE:
        {
            if (eNextPowerStatus != DIAS_PM_STATUS_WAKE_UP)
            {
                /* ���ƹػ�/����ʧ�ܣ�ϵͳ������������ʹ��Ӳ�ػ� */
                return DIAS_PM_CTRL_HARD_POWER_OFF;
            }
            break;
        }
        default:
            break;
    }

    return DIAS_PM_CTRL_NONE;
}

/******************************************************************************
*  function name | DiasPmIsPeripheralSysPowerCtrlFinished
*  content       | �ж��ܱ�ϵͳ��Դ�����Ƿ����
*  parameter[1]  | DiasPmSysType_e eSysType:ϵͳ����
*  notice        | ��
*  return        | BOOL��TRUE(��Դ�������)/FALSE(��Դ����δ���)
******************************************************************************/
BOOL DiasPmIsPeripheralSysPowerCtrlFinished(DiasPmSysType_e eSysType)
{
    if (gatPmSysInfo[eSysType].ptPowerInfo == NULL)
    {
        return TRUE;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
    {
        return FALSE;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->eCurrPowerStatus == DIAS_PM_STATUS_POWER_CTRL_FAULT)
    {
        /* ����ʧ�ܺ��ٿ��ƣ�������Ϊ�����Ѿ����� */
        return TRUE;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->eCurrPowerStatus != gatPmSysInfo[eSysType].ptPowerInfo->eExpectPowerStatus)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************
*  function name | DiasPmIsAllPeripheralSysPowerCtrlFinished
*  content       | �ж������ܱ�ϵͳ��Դ�����Ƿ����
*  parameter[1]  | void
*  notice        | ��
*  return        | BOOL��TRUE(��Դ�������)/FALSE(��Դ����δ���)
******************************************************************************/
BOOL DiasPmIsAllPeripheralSysPowerCtrlFinished(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (DiasPmIsPeripheralSysPowerCtrlFinished((DiasPmSysType_e)u8Loop) != TRUE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************
*  function name | ApiPmGetCurrSysPowerStatus
*  content       | ��ȡ��ǰ��Դ״̬
*  parameter[1]  | DiasPmSysType_e eSysType
*  notice        | ��Ҫͨ���ú����жϵ�Դ�����Ƿ���ɣ��ʲ��ٿ��Ƶ�Դ������ʵ��
*                | �ĵ�Դ״̬����֧�ֵ�Դ���Ʒ��عػ�״̬
*  return        | DiasPmPowerStatus_e��DIAS_PM_STATUS_UNSTABLE(��ȷ��״̬)
*                |                      DIAS_PM_STATUS_POWER_OFF(�ػ�)
*                |                      DIAS_PM_STATUS_SLEEP(����)
*                |                      DIAS_PM_STATUS_WAKE_UP(����)
*                |                      DIAS_PM_STATUS_RUN_ABMORMAL(�쳣����״̬)
******************************************************************************/
DiasPmPowerStatus_e ApiPmGetCurrSysPowerStatus(DiasPmSysType_e eSysType)
{
    if (eSysType >= DIAS_PM_SYS_TYPE_END)
    {
        return DIAS_PM_STATUS_POWER_OFF;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo == NULL)
    {
        return DIAS_PM_STATUS_UNSTABLE;
    }

    if (gatPmSysInfo[eSysType].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
    {
        return DIAS_PM_STATUS_UNSTABLE;
    }

    return gatPmSysInfo[eSysType].ptPowerInfo->eFeedbackPowerStatus;
}

/******************************************************************************
*  function name | DiasPmNotifyPowerModeToPeripheralSys
*  content       | ��Դģʽ�㲥֪ͨ
*  parameter[1]  | PmPowerModeStatus_e ePowerMode����Դģʽ
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmNotifyPowerModeToPeripheralSys(PmPowerModeStatus_e ePowerMode)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].notifyPowerModeFunc != NULL)
        {
            /* ֪ͨ��Դģʽ�л� */
            gatPmSysInfo[u8Loop].notifyPowerModeFunc(ePowerMode);
        }
        else
        {
            gatPmSysInfo[u8Loop].ePowerModeStatus = ePowerMode;
        }
    }
}

/******************************************************************************
*  function name | DiasPmNotifySleepFailToPeripheralSys
*  content       | ֪ͨMCU�ܱ�ϵͳ����ʧ��
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmNotifySleepFailToPeripheralSys(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo != NULL)
        {
            if (gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->eNotifyStatus == DIAS_PM_WAIT_SEND)
            {
                gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->eNotifyStatus = DIAS_PM_SENDING;
                gatPmSysInfo[u8Loop].ptSleepFailNotifyInfo->u8FailureCount = 0;
            }
        }
    }
}

/******************************************************************************
*  function name | DiasPmIsPowerModeChangeEnd
*  content       | �ж������ܱ�ϵͳ��Դģʽ�Ƿ��ѱ�����
*  parameter[1]  | PmPowerModeStatus_e eCurrPowerMode����ǰ��Դģʽ
*  notice        | ��
*  return        | BOOL��TRUE(��Դģʽ������)/FALSE(��Դģʽδ������)
******************************************************************************/
BOOL DiasPmIsPowerModeChangeEnd(PmPowerModeStatus_e eCurrPowerMode)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo != NULL)
        {
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
            {
                /* ���ڽ��е�Դ���ƣ���Ϊ��Դģʽ�л�δ��� */
                return FALSE;
            }

            if (gatPmSysInfo[u8Loop].ptPowerInfo->eFeedbackPowerStatus != DIAS_PM_STATUS_WAKE_UP)
            {
                /* ϵͳ����Ѳ������У���Ϊ��Դģʽ�л���� */
                continue;
            }
        }

        if (gatPmSysInfo[u8Loop].ePowerModeStatus != eCurrPowerMode)
        {
            return FALSE;
        }
    }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Power mode change end.PowerMode:%d\n", eCurrPowerMode);
    return TRUE;
}

/******************************************************************************
*  function name | DiasPmIsSysSleepReady
*  content       | �ж��Ƿ���Խ�������
*  parameter[1]  | DiasPmSysType_e eSysType
*                | BOOL bAllowIgnore���������
*  notice        | ��
*  return        | BOOL��TRUE(���Խ�������)/FALSE(�����Խ�������)
******************************************************************************/
BOOL DiasPmIsSysSleepReady(DiasPmSysType_e eSysType, BOOL bAllowIgnore)
{
    if (gatPmSysInfo[eSysType].ptPowerInfo != NULL)
    {
        if (DiasPmIsPeripheralSysPowerCtrlFinished(eSysType) != TRUE)
        {
            /* ��Դ����δ��ɣ������������� */
            return TRUE;
        }

        if (gatPmSysInfo[eSysType].ptPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_WAKE_UP)
        {
            /* ϵͳ����Ѳ������л������쳣�������������� */
            return TRUE;
        }
    }

    if ((bAllowIgnore == TRUE) &&
        (gatPmSysInfo[eSysType].bIsSleepReadyIgnore == TRUE))
    {
        /* ���������߱����ԣ������Ƿ��������߶���Ϊ�������� */
        return TRUE;
    }

    return gatPmSysInfo[eSysType].bIsSleepReady;
}

/******************************************************************************
*  function name | DiasPmSetSysSleepReadyIgnoreFlag
*  content       | ����ϵͳ�����������Ƿ���Ժ��Ա��
*  parameter[1]  | DiasPmSysType_e eSysType
*                | BOOL bIgnore
*  notice        | ��
*  return        | VOID
******************************************************************************/
void DiasPmSetSysSleepReadyIgnoreFlag(DiasPmSysType_e eSysType, BOOL bIgnore)
{
    gatPmSysInfo[eSysType].bIsSleepReadyIgnore = bIgnore;
}

#ifndef DIAS_PM_EOL
/******************************************************************************
*  function name | DiasPmGetNoServiceTime
*  content       | ��ȡ��Զ��ҵ�����ʱ��
*  parameter[1]  | void
*  notice        | ��
*  return        | U32����Զ��ҵ�����ʱ��
******************************************************************************/
uint32_t DiasPmGetNoServiceTime(void)
{
    uint8_t  u8Loop;
    uint32_t u32CurrTime = 0;
    uint32_t u32TotalNoServiceStartTime = 0u;

    u32CurrTime = 0;//ApiAuxTaskGetSysTimeUtc();
    
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].bHasService == FALSE)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo != NULL)
        {
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ePowerCtrlType != DIAS_PM_CTRL_NONE)
            {
                /* ���ڽ��е�Դ���ƣ���Ϊ����Զ��ҵ�� */
                return 0;
            }

            if (gatPmSysInfo[u8Loop].ptPowerInfo->eFeedbackPowerStatus != DIAS_PM_STATUS_WAKE_UP)
            {
                /* ϵͳ����Ѳ������У�����Ҫ�ж��Ƿ���Զ��ҵ�� */
                continue;
            }
        }

        if (gatPmSysInfo[u8Loop].u32NoServiceStartTime > u32TotalNoServiceStartTime)
        {
            u32TotalNoServiceStartTime = gatPmSysInfo[u8Loop].u32NoServiceStartTime;
        }
    }

    if ((u32TotalNoServiceStartTime == 0) ||
        (u32CurrTime <= u32TotalNoServiceStartTime))
    {
        return 0;
    }

    return u32CurrTime - u32TotalNoServiceStartTime;
}
#endif

/******************************************************************************
*  function name | DiasPmCheckSysHeartBeat
*  content       | ��������������ģ��
*  parameter     | void
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmCheckSysHeartBeat(void)
{
    DiasPmSysMonitorCfg_t *ptMonitorCfg;
    DiasPmFeedbackInfo_t *ptFeedbackInfo;
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if ((gatPmSysInfo[u8Loop].ptPowerInfo == NULL) ||
            (gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg == NULL))
        {
            continue;
        }

        ptMonitorCfg = gatPmSysInfo[u8Loop].ptPowerInfo->ptMonitorCfg;

        /* ����������Ϣ��ʽ���ϵͳ */
        if (ptMonitorCfg->tSendHeartBeatInfo.sendHeartBeatFunc != NULL)
        {
            if (ptMonitorCfg->tSendHeartBeatInfo.u16TimeSinceLastSend < ptMonitorCfg->tSendHeartBeatInfo.u16SendPeriod)
            {
                ++ptMonitorCfg->tSendHeartBeatInfo.u16TimeSinceLastSend;
            }
            else
            {
                ptMonitorCfg->tSendHeartBeatInfo.sendHeartBeatFunc();
                ptMonitorCfg->tSendHeartBeatInfo.u16TimeSinceLastSend = 0;
            }
        }

        /* ״̬������ʽ���ϵͳ */
        if (ptMonitorCfg->ptFeedbackInfo != NULL)
        {
            ptFeedbackInfo = ptMonitorCfg->ptFeedbackInfo;
            if (ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft > 0)
            {
                --ptFeedbackInfo->tFeedbackConfirmInfo.u16ConfirmTimeLeft;
            }
            else
            {
                ptFeedbackInfo->bFeedbackStatus = DiasPmGetFeedbackStatus(ptFeedbackInfo);
                if (ptFeedbackInfo->bFeedbackStatus == TRUE)
                {
                    ptMonitorCfg->u32HeartbeatDeadTimer = ptMonitorCfg->u32AbnormalConfirmPeriod;
                }
            }
        }
    }
}

/******************************************************************************
*  function name | ApiPmRegisterEventListener
*  content       | pm app��DCPD��ػص�����ע��
*  parameter[1]  | eConnectType-connect type
*  notice        | ��
*  return        | void
******************************************************************************/
void ApiPmRegisterEventListener(void)
{
    DiasPmLteRegisterEventListener();
}

/******************************************************************************
*  function name | DiasPmSysPowerDownBeforeLowPower
*  content       | ����͹���ǰϵͳ�ϵ紦��
*  parameter     | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmSysPowerDownBeforeLowPower(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo == NULL)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo == NULL)
        {
            continue;
        }

        /* ��Դ״̬ǿ��ͣ��ʱ�����㣬��ֹ���Ѻ����Դ���Ƶȴ� */
        gatPmSysInfo[u8Loop].ptPowerInfo->u16ForceStayLeftTime = 0;

        if ((gatPmSysInfo[u8Loop].ptPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_POWER_OFF) &&
            (gatPmSysInfo[u8Loop].ptPowerInfo->eCurrPowerStatus != DIAS_PM_STATUS_POWER_CTRL_FAULT))
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerDownInLowPower == TRUE)
        {
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerDownFunc != NULL)
            {
                gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerDownFunc();
            }

            gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUp = FALSE;
        }
    }
}

/******************************************************************************
*  function name | DiasPmRefreshPowerUpStatusAfterSysPowerUp
*  content       | �����ϵ��ϵͳ�ϵ�״̬ˢ��
*  parameter     | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmRefreshPowerUpStatusAfterSysPowerUp(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo == NULL)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo == NULL)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUpDefault == FALSE)
        {
            continue;
        }

        gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUp = TRUE;

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerUpFunc != NULL)
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerUpFunc();
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerOnAfterPowerUp == TRUE)
        {
            /* �ϵ�Ĭ�Ͽ�����ϵͳ���ϵ��ȴ����������ſɽ���������Դ���� */
            gatPmSysInfo[u8Loop].ptPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_POWER_ON;
            gatPmSysInfo[u8Loop].ptPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_UNSTABLE;
            gatPmSysInfo[u8Loop].ptPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_FEEDBACK;
            gatPmSysInfo[u8Loop].ptPowerInfo->u32FeedbackTimerCount = 0;
            ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Sys poweron after power up.Sys:%d\n", u8Loop);
        }
    }
}

/******************************************************************************
*  function name | DiasPmRefreshPowerUpStatusWhenWakeupFromDeepstop
*  content       | ���������ģʽ���Ѻ�ϵͳ��Դ״̬ˢ��
*  parameter     | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmRefreshPowerUpStatusWhenWakeupFromDeepstop(void)
{
    uint8_t u8Loop;
    for (u8Loop = 0u; u8Loop < gu8PmSysNum; ++u8Loop)
    {
        if (gatPmSysInfo[u8Loop].ptPowerInfo == NULL)
        {
            continue;
        }

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo == NULL)
        {
            continue;
        }

#if 0   /* ������߱���GPIO״̬ */
        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUpDefault == FALSE)
        {
            continue;
        }
#endif

        if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerDownInLowPower == TRUE)
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUp = FALSE;
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerDownFunc != NULL)
            {
                gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerDownFunc();
            }
        }
        else
        {
            gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->bPowerUp = TRUE;
            if (gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerUpFunc != NULL)
            {
                gatPmSysInfo[u8Loop].ptPowerInfo->ptPowerUpCtrlInfo->powerUpFunc();
            }
        }
    }
}

/******************************************************************************
*  function name | DiasPmProcPowerUpWhenChangingPowerStatus
*  content       | �ڴ���Դ״̬�ı�ʱ�ϵ紦��
*  parameter[1]  | DiasPmSysType_e eSysType��ϵͳ����
*                | DiasPmPowerInfo_t *ptCurrSysPowerInfo����Դ��Ϣ
*  notice        | ��
*  return        | void
******************************************************************************/
static void DiasPmProcPowerUpWhenChangingPowerStatus(DiasPmSysType_e eSysType, DiasPmPowerInfo_t *ptCurrSysPowerInfo)
{
    if ((ptCurrSysPowerInfo == NULL) ||
        (ptCurrSysPowerInfo->ptPowerUpCtrlInfo == NULL))
    {
        return;
    }

    switch (ptCurrSysPowerInfo->ePowerCtrlType)
    {
        case DIAS_PM_CTRL_NONE:
        case DIAS_PM_CTRL_POWER_ON:
        {
            if (ptCurrSysPowerInfo->ptPowerUpCtrlInfo->bPowerUp == FALSE)
            {
                if (ptCurrSysPowerInfo->ptPowerUpCtrlInfo->powerUpFunc != NULL)
                {
                    ptCurrSysPowerInfo->ptPowerUpCtrlInfo->powerUpFunc();
                    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Sys power up Sys:%d\n", eSysType);
                }

                ptCurrSysPowerInfo->ptPowerUpCtrlInfo->bPowerUp = TRUE;

                if (ptCurrSysPowerInfo->ptPowerUpCtrlInfo->bPowerOnAfterPowerUp == TRUE)
                {
                    /* �ϵ��ֱ�ӿ���������ֱ�ӵȴ�ϵͳ������� */
                    ptCurrSysPowerInfo->ePowerCtrlType = DIAS_PM_CTRL_POWER_ON;
                    ptCurrSysPowerInfo->eCurrPowerStatus = DIAS_PM_STATUS_UNSTABLE;
                    ptCurrSysPowerInfo->ePowerCtrlStatus = DIAS_PM_CTRL_FEEDBACK;
                    ptCurrSysPowerInfo->u32FeedbackTimerCount = 0;
                }
            }
            break;
        }
        default:
        {
            /* ϵͳ����OFF״̬���Ѿ��ϵ磬����Ϊȷ������һʧ���ٴο����ϵ����� */
            /* Ӧ�ó���������MCUǰ�����ڵ�Դ��MPU�ӹܣ���������MCU���½ӹ� */
            if (ptCurrSysPowerInfo->ptPowerUpCtrlInfo->powerUpFunc != NULL)
            {
                ptCurrSysPowerInfo->ptPowerUpCtrlInfo->powerUpFunc();
            }

            ptCurrSysPowerInfo->ptPowerUpCtrlInfo->bPowerUp = TRUE;
            break;
        }
    }
}



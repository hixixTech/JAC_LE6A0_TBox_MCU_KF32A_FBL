/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_power_mode_cfg.c
* Description:  PMģ��
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** �޸���ʷ ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  �����        �½�����
* 2020-08-14   V11.01.01.01.00  �����        DCPDģ�����߸���MCU����ǰ
* 2020-08-14   V11.01.01.01.00  �����        ����WIFI����Ƶ��������(��Ƶ������ȱ�٣���Ԥ��)
* 2020-08-21   V11.01.01.01.00  �����        AUX��WANģ�����߻�����ʱ����
* 2020-08-21   V11.01.01.01.00  �����        DTCģ�����߽ӿ��޸�
******************************************************************************/


/******************************************************************************
* ͷ�ļ�
******************************************************************************/
#include "app_pm_power_mode.h"
#include "app_pm_mv.h"
#include "pm_public.h"
#include "app_pm_power_mode_cfg.h"
#include "app_pm_peripheral_sys.h"
#include "os_log.h"
#include "app_pm_vol.h"

//#include "srv_can_signal.h"

/******************************************************************************
* ��Դģʽ�л�״̬��״̬����
******************************************************************************/
DiasPmPowerModeChangeCfg_t gatPmPowerModeChangeCfg[] =
{
    {PM_POWER_MODE_OFF,                  PM_POWER_MODE_SLEEP1,          DIAS_PM_MCU_WAKEUP},
    {PM_POWER_MODE_OFF,                  PM_POWER_MODE_SLEEP2,          DIAS_PM_MCU_WAKEUP},
    {PM_POWER_MODE_OFF,                  PM_POWER_MODE_SLEEP3,          DIAS_PM_MCU_WAKEUP},
    {PM_POWER_MODE_RUNNING,              PM_POWER_MODE_SLEEP1,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_SLEEP1,               PM_POWER_MODE_SLEEP1,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_SLEEP1,               PM_POWER_MODE_SLEEP2,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_SLEEP2,               PM_POWER_MODE_SLEEP3,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_SLEEP1,               PM_POWER_MODE_SLEEP3,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_RUNNING,              PM_POWER_MODE_SLEEP2,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_RUNNING,              PM_POWER_MODE_SLEEP3,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_RUNNING,              PM_POWER_MODE_BAKBAT,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_RUNNING,              PM_POWER_MODE_POWERDOWN,       DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_RUNNING,              PM_POWER_MODE_OFF,             DIAS_PM_ALL_SYSTEM_FORCE_RESET},
    {PM_POWER_MODE_SLEEP1,               PM_POWER_MODE_OFF,             DIAS_PM_ALL_SYSTEM_FORCE_RESET},
    {PM_POWER_MODE_SLEEP2,               PM_POWER_MODE_OFF,             DIAS_PM_ALL_SYSTEM_FORCE_RESET},
    {PM_POWER_MODE_SLEEP3,               PM_POWER_MODE_OFF,             DIAS_PM_ALL_SYSTEM_FORCE_RESET},
    {PM_POWER_MODE_POWERDOWN,            PM_POWER_MODE_OFF,             DIAS_PM_ALL_SYSTEM_FORCE_RESET},
    {PM_POWER_MODE_BAKBAT,               PM_POWER_MODE_OFF,             DIAS_PM_ALL_SYSTEM_FORCE_RESET},

    /* �ݴ��� */
    {PM_POWER_MODE_SLEEP2,               PM_POWER_MODE_SLEEP2,          DIAS_PM_MCU_STANDBY},
    {PM_POWER_MODE_SLEEP3,               PM_POWER_MODE_SLEEP3,          DIAS_PM_MCU_STANDBY},
};

uint8_t gu8PmPowerModeChangeTypeNum = (uint16_t)(sizeof(gatPmPowerModeChangeCfg) / sizeof(gatPmPowerModeChangeCfg[0]));


/******************************************************************************
* ���߻��Ѵ���
******************************************************************************/
const uint8_t gu8PmMaxSleepPriority = 2;
const uint8_t gu8PmMaxWakeupPriority = 3;
DiasPmMcuModuleInfo_t gatPmMcuModuleInfo[] =
{
    {0,                      3, DIAS_PM_WAKEUP_OK,  DiasPmIsSleepReady,          NULL,                         NULL,                       NULL},
    // {0,                      3, DIAS_PM_WAKEUP_OK,  ApiComTaskCanSleep,          ApiComTaskHasSlept,           ApiComTaskSleep,            ApiComTaskWakeup},
    // {1,                      2, DIAS_PM_WAKEUP_OK,  ApiDTCCanSleep,              ApiDTCHasSlept,               ApiDTCSleep,                ApiDTCWakeup},
    // {2,                      1, DIAS_PM_WAKEUP_OK,  ApiAuxTaskCanSleep,          ApiAuxTaskHasSlept,           ApiAuxTaskSleep,            ApiAuxTaskWakeup},
    // {DIAS_PM_PRIORITY_FINAL, 2, DIAS_PM_WAKEUP_OK,  ApiDcpdSrvCanSleep,          ApiDcpdSrvHasSlept,           ApiDcpdTaskSleep,           ApiDcpdTaskWakeup},
    // {0,                      3, DIAS_PM_WAKEUP_OK,  ApiGsenSrvCanSleep,          ApiGsenSrvHasSlept,           ApiGsenSrvSleep,            ApiGsenSrvWakeup},
    // {DIAS_PM_PRIORITY_FINAL, 0, DIAS_PM_WAKEUP_OK,  ApiNvramCanSleep,            ApiNvramTaskHasSlept,         ApiNvramSleep,              ApiNvramTaskWakeup},
    // {2,                      1, DIAS_PM_WAKEUP_OK,  ApiSysMgrSrvCanSleep,        ApiSysMgrTaskHasSlept,        ApiSysMgrTaskSleep,         ApiSysMgrTaskWakeup},
    // {0,                      3, DIAS_PM_WAKEUP_OK,  ApiWanCanSleep,              ApiWanHasSlept,               ApiWanSleep,                ApiWanWakeup},
};

uint16_t gu16PmMcuModuleNum = (uint16_t)(sizeof(gatPmMcuModuleInfo) / sizeof(gatPmMcuModuleInfo[0]));


/******************************************************************************
* ��Դģʽ����
******************************************************************************/
DiasPmPowerModeCfg_t gatPmPowerModeCfg[PM_POWER_MODE_END] =
{
    /* OFF */
    {
        DiasPmOffModeHandle,
        {
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_SLEEP,
            DIAS_PM_STATUS_SLEEP
        },
        {
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID
        }
    },
#if PM_V2X_A_SAMPLE == 0
    /* RUNNING */
    {
        DiasPmRunningModeChangeHandle,
        {
            DIAS_PM_STATUS_WAKE_UP,
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_WAKE_UP,
            DIAS_PM_STATUS_SLEEP      /* T.B.D */
        },
        {
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_POWERDOWN,
            PM_POWER_MODE_BAKBAT
        }
    },
#else
    /* RUNNING */
    {
        NULL,
        {
            DIAS_PM_STATUS_WAKE_UP,
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_WAKE_UP,
            DIAS_PM_STATUS_SLEEP      /* T.B.D */
        },
        {
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_POWERDOWN,
            PM_POWER_MODE_BAKBAT
        }
    },
#endif
    /* SLEEP1 */
    {
        DiasPmSleep1ModeHandle,
        {
            DIAS_PM_STATUS_SLEEP,
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_SLEEP,
            DIAS_PM_STATUS_SLEEP
        },
        {
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID
        }
    },
    /* SLEEP2 */
    {
        DiasPmSleep2ModeHandle,
        {
            DIAS_PM_STATUS_SLEEP,
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_SLEEP,
            DIAS_PM_STATUS_SLEEP
        },
        {
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID
        }
    },
    /* SLEEP3 */
    {
        DiasPmSleep3ModeHandle,
        {
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_SLEEP,
            DIAS_PM_STATUS_SLEEP
        },
        {
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID
        }
    },
    /* BAKBAT */
    {
        DiasPmBakBatModeHandle,
        {
            DIAS_PM_STATUS_WAKE_UP,
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_SLEEP,
            DIAS_PM_STATUS_SLEEP      /* T.B.D */
        },
        {
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_POWERDOWN,
            PM_POWER_MODE_INVALID
        }
    },
    /* POWERDOWN */
    {
        DiasPmPowerDownModeHandle,
        {
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_POWER_OFF,
            DIAS_PM_STATUS_SLEEP,
            DIAS_PM_STATUS_SLEEP
        },
        {
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_INVALID,
            PM_POWER_MODE_BAKBAT
        }
    }
};


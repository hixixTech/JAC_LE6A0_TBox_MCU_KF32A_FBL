/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_peripheral_sys_cfg.c
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
* 2020-08-05   V11.01.01.01.00  �����        ģ�鿪�ػ����߻������GPIO���
* 2020-08-11   V11.01.01.01.00  �����        ģ�����߻���״̬������Ϊ��֧��
* 2020-08-14   V11.01.01.01.00  �����        ����WIFI����Ƶ��������(��Ƶ������������)
* 2020-08-14   V11.01.01.01.00  �����        ���ӿ�������״̬��⹦��
* 2020-09-11   V11.01.01.01.00  �����        ����ģ������״̬��⹦��
******************************************************************************/


/******************************************************************************
* ͷ�ļ�
******************************************************************************/
#include "app_pm_peripheral_sys_cfg.h"
#include "app_pm_peripheral_sys_reset.h"
#include "app_pm_public_define.h"
#include "app_pm_lte_dcpd_com.h"
#include "os_log.h"
#include "app_pm_drv.h"
#include "ecu_gpio.h"

/******************************************************************************
* �궨��
******************************************************************************/
#define DIAS_PM_GENERAL_INIT_MONITOR_PERIOD         (120000u / DIAS_PM_PERIOD_TIMER) /* һ��ϵͳ����״̬��س�ʼֵ */ //20min->2min
#define DIAS_PM_GENERAL_ABNORMAL_CONFIRM_PERIOD     (30000u / DIAS_PM_PERIOD_TIMER)  /* һ��ϵͳ������ʱʱ�� */  //120s->30s
#define DIAS_PM_SYS_ABNORMAL_IGNORE_TIMER           36000u                           /* ���Ժ���ϵͳ�쳣��ʱ��(s) */


/******************************************************************************
* ��������
******************************************************************************/
/* LTEģ���Դ��Ϣ */
#ifdef DIAS_PM_MONITOR_ENABLE
DiasPmFeedbackInfo_t gtPmLteFeedbackInfo =
{
    DIAS_PM_FEEDBACK_MODE_PIN_LOW,
    {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 2000u / DIAS_PM_PERIOD_TIMER, 0}},
    FALSE,
    {
        2000u / DIAS_PM_PERIOD_TIMER,
        2000u / DIAS_PM_PERIOD_TIMER,
        0,
        0
    }
};

static DiasPmSysMonitorCfg_t gtPmLteMonitorCfg =
{
    FALSE,                                                     /* ��ʼʱϵͳ�Ƿ���Ҫ�쳣��� */
    FALSE,                                                     /* ��ǰϵͳ�Ƿ���Ҫ�쳣��� */
    FALSE,                                                     /* ϵͳ�Ƿ����(�쳣������δ���޸�) */
    0,                                                         /* �´��쳣����ʱ���¼λ�� */
    {0},                                                       /* ������ϵͳ�쳣����ʱ�� */
    DIAS_PM_GENERAL_INIT_MONITOR_PERIOD,                       /* ��ʼʱ�쳣������� */
    DIAS_PM_GENERAL_INIT_MONITOR_PERIOD,                       /* ʣ���쳣ȷ��ʱ�䣬��Ϊ0ʱ����ʾ�쳣���� */
    DIAS_PM_GENERAL_ABNORMAL_CONFIRM_PERIOD,                   /* ������ʱʱ�� */
    DIAS_PM_SYS_ABNORMAL_IGNORE_TIMER,                         /* ���Ժ���ϵͳ�쳣��ʱ��(s) */
    &gtPmLteFeedbackInfo,                                      /* ״̬������Ϣ(���ϵͳ�쳣����֮һ������������ⷽ������) */
    {3000u / DIAS_PM_PERIOD_TIMER, 3000u / DIAS_PM_PERIOD_TIMER, NULL},      /* ������������(���ϵͳ�쳣����֮һ������������ⷽ������) */
    ApiPmResetMPU                                              /* ϵͳ�����쳣������ */
};
#endif

static DiasPmPowerUpCtrlInfo_t gtLtePowerUpCtrlInfo =
{
    TRUE,                       /* Ĭ���Ƿ��ϵ� */
    FALSE,                      /* ��ǰ�ϵ�״̬ */
    FALSE,                      /* �ϵ��Ƿ��Զ����� */
    TRUE,                       /* ���ߺ��Ƿ��µ� */
    DiasPmModPowerUp,           /* �����ϵ� */
    DiasPmModPowerDown          /* �����µ� */
};

static DiasPmCtrlStepCfg_t gatPmLtePowerOnStepCfg[] =
{
    {ApiGpioOutputHigh,    GPIO_PWR_CTL_4G,         1000 / DIAS_PM_PERIOD_TIMER},
    {ApiGpioOutputLow,     GPIO_PWR_CTL_4G,         0}
};

static DiasPmCtrlStepCfg_t gatPmLtePowerOffStepCfg[] =
{
    {ApiGpioOutputHigh,    GPIO_PWR_CTL_4G,         7000 / DIAS_PM_PERIOD_TIMER},
    {ApiGpioOutputLow,     GPIO_PWR_CTL_4G,         0}
};

static DiasPmCtrlStepCfg_t gatPmLteSleepStepCfg[] =
{
    {ApiGpioOutputLow,     GPIO_WKUP_4G,            1000 / DIAS_PM_PERIOD_TIMER},
};

static DiasPmCtrlStepCfg_t gatPmLteWakeupStepCfg[] =
{
    {ApiGpioOutputHigh,    GPIO_WKUP_4G,            1000 / DIAS_PM_PERIOD_TIMER}
};

static DiasPmCtrlStepCfg_t gatPmLteResetStepCfg[] =
{
    {ApiGpioOutputHigh,    GPIO_NRST_4G,            3000 / DIAS_PM_PERIOD_TIMER},
    {ApiGpioOutputLow,     GPIO_NRST_4G,            0}
};

static DiasPmPowerInfo_t gtPmLtePowerInfo =
{
    DIAS_PM_STATUS_UNSTABLE,     /* ��ǰ��Դ״̬ */
    DIAS_PM_STATUS_POWER_OFF,    /* �ڴ���Դ״̬ */
    DIAS_PM_STATUS_POWER_OFF,    /* ǰһ���ڴ���Դ״̬ */
    DIAS_PM_STATUS_UNSTABLE,     /* ʵʱ��Դ״̬���� */
#ifdef DIAS_PM_MONITOR_ENABLE
    &gtPmLteMonitorCfg,          /* ϵͳ������� */
#else
    NULL,                        /* ϵͳ������� */
#endif
    {
        {DIAS_PM_FEEDBACK_MODE_PIN_LOW,         {{GPIO_4G_STATUS}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}},        FALSE, {0}},    /* �ж�ϵͳ�Ƿ񿪻� */
        {DIAS_PM_FEEDBACK_MODE_NONE,            {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, TRUE,  {0}},    /* �ж�ϵͳ�Ƿ��� */
        {DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_HIGH, {{GPIO_WKUP_4G}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}},          FALSE, {0}}     /* �ж�ϵͳ�������߿���״̬ */
    },
    DIAS_PM_CTRL_NONE,           /* ��Դ�������� */
    DIAS_PM_CTRL_IDLE,           /* ��Դ����״̬ */
    DIAS_PM_CTRL_IDLE,           /* ǰһ�ε�Դ����״̬ */
    &gtLtePowerUpCtrlInfo,       /* �ϵ���Ϣ */
    0,                           /* ���ƴ��� */
    0,                           /* ��ǰ���Ʋ��� */
    0,                           /* ��Դ״̬ǿ��ͣ��ʱ�� */
    3,                           /* �ȴ���Դ״̬ȷ��ʱ�� */
    TRUE,                        /* �Ƿ����ڽ��е�Դ״̬ȷ�� */
    0,                           /* ��ǰ�ѿ���ʱ�� */
    0,                           /* ��ǰ�ѵȴ�����ʱ�� */
    {
        /* �ֲ��������� */                    /* ���Ʋ��� */                                                         /* LogId */                   /* �����ʱ�� */
        /* DIAS_PM_CTRL_NONE���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_ON���� */
        {gatPmLtePowerOnStepCfg,              sizeof(gatPmLtePowerOnStepCfg) / sizeof(gatPmLtePowerOnStepCfg[0]),    DIAS_U8_INVALID_VALUE,        45000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_POWER_OFF���� */
        {gatPmLtePowerOffStepCfg,             sizeof(gatPmLtePowerOffStepCfg) / sizeof(gatPmLtePowerOffStepCfg[0]),  DIAS_U8_INVALID_VALUE,        30000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_WAKE_UP���� */
        {gatPmLteWakeupStepCfg,               sizeof(gatPmLteWakeupStepCfg) / sizeof(gatPmLteWakeupStepCfg[0]),      DIAS_U8_INVALID_VALUE,        2000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_SLEEP���� */
        {gatPmLteSleepStepCfg,                sizeof(gatPmLteSleepStepCfg) / sizeof(gatPmLteSleepStepCfg[0]),        DIAS_U8_INVALID_VALUE,        2000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_RESET���� */
        {gatPmLteResetStepCfg,                sizeof(gatPmLteResetStepCfg) / sizeof(gatPmLteResetStepCfg[0]),        DIAS_U8_INVALID_VALUE,        5000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_HARD_POWER_OFF���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
    },
    {
        /* DIAS_PM_STATUS_UNSTABLE */
        NULL,
        /* DIAS_PM_STATUS_POWER_OFF */
        DiasPmPeripheralCtrlModOff,
        /* DIAS_PM_STATUS_WAKE_UP */
        DiasPmPeripheralCtrlModRun,
        /* DIAS_PM_STATUS_SLEEP */
        DiasPmPeripheralCtrlModSleep,
        /* DIAS_PM_STATUS_RUN_ABMORMAL */
        NULL
    }
};

/* ������Դ��Ϣ */
static DiasPmPowerInfo_t gtPmBlePowerInfo =
{
    DIAS_PM_STATUS_UNSTABLE,     /* ��ǰ��Դ״̬ */
    DIAS_PM_STATUS_POWER_OFF,    /* �ڴ���Դ״̬ */
    DIAS_PM_STATUS_POWER_OFF,    /* ǰһ���ڴ���Դ״̬ */
    DIAS_PM_STATUS_UNSTABLE,     /* ʵʱ��Դ״̬���� */
    NULL,                        /* ϵͳ������� */
    {
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}},    /* �ж�ϵͳ�Ƿ񿪻� */
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, TRUE,  {0}},    /* �ж�ϵͳ�Ƿ��� */
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}}     /* �ж�ϵͳ�������߿���״̬ */
    },
    DIAS_PM_CTRL_NONE,           /* ��Դ�������� */
    DIAS_PM_CTRL_IDLE,           /* ��Դ����״̬ */
    DIAS_PM_CTRL_IDLE,           /* ǰһ�ε�Դ����״̬ */
    NULL,                        /* �ϵ���Ϣ */
    0,                           /* ���ƴ��� */
    0,                           /* ��ǰ���Ʋ��� */
    0,                           /* ��Դ״̬ǿ��ͣ��ʱ�� */
    0,                           /* �ȴ���Դ״̬ȷ��ʱ�� */
    TRUE,                        /* �Ƿ����ڽ��е�Դ״̬ȷ�� */
    0,                           /* ��ǰ�ѿ���ʱ�� */
    0,                           /* ��ǰ�ѵȴ�����ʱ�� */
    {
        /* �ֲ��������� */                    /* ���Ʋ��� */               /* LogId */                   /* �����ʱ�� */
        /* DIAS_PM_CTRL_NONE���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_ON���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_OFF���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_WAKE_UP���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_SLEEP���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_RESET���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_HARD_POWER_OFF���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
    },
    {
        /* DIAS_PM_STATUS_UNSTABLE */
        NULL,
        /* DIAS_PM_STATUS_POWER_OFF */
        NULL,
        /* DIAS_PM_STATUS_WAKE_UP */
        NULL,
        /* DIAS_PM_STATUS_SLEEP */
        NULL,
        /* DIAS_PM_STATUS_RUN_ABMORMAL */
        NULL
    }
};

/* WIFI��Դ��Ϣ */
// static DiasPmCtrlStepCfg_t gatPmWifiSleepStepCfg[] =
// {
//     {ApiGpioOutputLow,     GPIO_SHUTDOWN_3V3,       0}
// };

// static DiasPmCtrlStepCfg_t gatPmWifiWakeupStepCfg[] =
// {
//     {ApiGpioOutputHigh,    GPIO_SHUTDOWN_3V3,       0}
// };

// static DiasPmCtrlStepCfg_t gatPmWifiResetStepCfg[] =
// {
//     {ApiGpioOutputLow,     GPIO_SHUTDOWN_3V3,       1000 / DIAS_PM_PERIOD_TIMER},
//     {ApiGpioOutputHigh,    GPIO_SHUTDOWN_3V3,       0}
// };

static DiasPmPowerInfo_t gtPmWifiPowerInfo =
{
    DIAS_PM_STATUS_UNSTABLE,     /* ��ǰ��Դ״̬ */
    DIAS_PM_STATUS_POWER_OFF,    /* �ڴ���Դ״̬ */
    DIAS_PM_STATUS_POWER_OFF,    /* ǰһ���ڴ���Դ״̬ */
    DIAS_PM_STATUS_UNSTABLE,     /* ʵʱ��Դ״̬���� */
    NULL,                        /* ϵͳ������� */
    {
        {DIAS_PM_FEEDBACK_MODE_NONE,            {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, TRUE,  {0}},    /* �ж�ϵͳ�Ƿ񿪻� */
        {DIAS_PM_FEEDBACK_MODE_NONE,            {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}},    /* �ж�ϵͳ�Ƿ��� */
        {DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_HIGH, {{GPIO_SOSLED_CTL}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}},     FALSE, {0}}     /* �ж�ϵͳ�������߿���״̬ */
    },
    DIAS_PM_CTRL_NONE,           /* ��Դ�������� */
    DIAS_PM_CTRL_IDLE,           /* ��Դ����״̬ */
    DIAS_PM_CTRL_IDLE,           /* ǰһ�ε�Դ����״̬ */
    NULL,                        /* �ϵ���Ϣ */
    0,                           /* ���ƴ��� */
    0,                           /* ��ǰ���Ʋ��� */
    0,                           /* ��Դ״̬ǿ��ͣ��ʱ�� */
    0,                           /* �ȴ���Դ״̬ȷ��ʱ�� */
    TRUE,                        /* �Ƿ����ڽ��е�Դ״̬ȷ�� */
    0,                           /* ��ǰ�ѿ���ʱ�� */
    0,                           /* ��ǰ�ѵȴ�����ʱ�� */
    {
        /* �ֲ��������� */                    /* ���Ʋ��� */                                                         /* LogId */                   /* �����ʱ�� */
        /* DIAS_PM_CTRL_NONE���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_ON���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_OFF���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_WAKE_UP���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_SLEEP���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_RESET���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_HARD_POWER_OFF���� */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},

    },
    {
        /* DIAS_PM_STATUS_UNSTABLE */
        NULL,
        /* DIAS_PM_STATUS_POWER_OFF */
        NULL,
        /* DIAS_PM_STATUS_WAKE_UP */
        NULL,
        /* DIAS_PM_STATUS_SLEEP */
        NULL,
        /* DIAS_PM_STATUS_RUN_ABMORMAL */
        NULL
    }
};

/* ��Ƶ��Դ��Ϣ */
static DiasPmPowerInfo_t gtPmAudioPowerInfo =
{
    DIAS_PM_STATUS_UNSTABLE,     /* ��ǰ��Դ״̬ */
    DIAS_PM_STATUS_POWER_OFF,    /* �ڴ���Դ״̬ */
    DIAS_PM_STATUS_POWER_OFF,    /* ǰһ���ڴ���Դ״̬ */
    DIAS_PM_STATUS_UNSTABLE,     /* ʵʱ��Դ״̬���� */
    NULL,                        /* ϵͳ������� */
    {
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, TRUE,  {0}},    /* �ж�ϵͳ�Ƿ񿪻� */
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}},    /* �ж�ϵͳ�Ƿ��� */
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}}     /* �ж�ϵͳ�������߿���״̬ */
    },
    DIAS_PM_CTRL_NONE,           /* ��Դ�������� */
    DIAS_PM_CTRL_IDLE,           /* ��Դ����״̬ */
    DIAS_PM_CTRL_IDLE,           /* ǰһ�ε�Դ����״̬ */
    NULL,                        /* �ϵ���Ϣ */
    0,                           /* ���ƴ��� */
    0,                           /* ��ǰ���Ʋ��� */
    0,                           /* ��Դ״̬ǿ��ͣ��ʱ�� */
    0,                           /* �ȴ���Դ״̬ȷ��ʱ�� */
    TRUE,                        /* �Ƿ����ڽ��е�Դ״̬ȷ�� */
    0,                           /* ��ǰ�ѿ���ʱ�� */
    0,                           /* ��ǰ�ѵȴ�����ʱ�� */
    {
        /* �ֲ��������� */                    /* ���Ʋ��� */               /* LogId */                   /* �����ʱ�� */
        /* DIAS_PM_CTRL_NONE���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_ON���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_OFF���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_WAKE_UP���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_SLEEP���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_RESET���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_HARD_POWER_OFF���� */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
    },
    {
        /* DIAS_PM_STATUS_UNSTABLE */
        NULL,
        /* DIAS_PM_STATUS_POWER_OFF */
        NULL,
        /* DIAS_PM_STATUS_WAKE_UP */
        NULL,
        /* DIAS_PM_STATUS_SLEEP */
        NULL,
        /* DIAS_PM_STATUS_RUN_ABMORMAL */
        NULL
    }
};

/* �����쳣LTEģ��֪ͨ���� */
DiasPmMsgNotifyInfo_t gtPmLteSleepFailNotifyInfo =
{
    DIAS_PM_WAIT_SEND,
    0,
    5,
    0,
    3000 / DIAS_PM_PERIOD_TIMER,
    DiasPmLteNotifySleepFailure
};


DiasPmSysInfo_t gatPmSysInfo[DIAS_PM_SYS_TYPE_END] =
{
    /* LTEģ�� */
    {
        &gtPmLtePowerInfo,                /* ��Դ��Ϣ */
        PM_POWER_MODE_RUNNING,            /* �����ĵ�Դģʽ */
        FALSE,                            /* Ĭ���Ƿ�������� */
        FALSE,                            /* �Ƿ�������� */
        TRUE,                             /* �Ƿ����ҵ�� */
        FALSE,                            /* �����������Ƿ���Ժ��� */
        0,                                /* ��ҵ��ʼʱ�� */
        DiasPmLteNotifyPowerMode,         /* ��Դģʽ֪ͨ */
        &gtPmLteSleepFailNotifyInfo,      /* ����ʧ��֪ͨ��Ϣ */
        0                                 /* ����δ�����Ƿ�������ߵ�ʱ�� */
    },
    /* ���� */
    {
        NULL,                             /* ��Դ��Ϣ */
        PM_POWER_MODE_OFF,                /* �����ĵ�Դģʽ */
        TRUE,                             /* Ĭ���Ƿ�������� */
        TRUE,                             /* �Ƿ�������� */
        FALSE,                            /* �Ƿ����ҵ�� */
        FALSE,                            /* �����������Ƿ���Ժ��� */
        0,                                /* ��ҵ��ʼʱ�� */
        NULL,                             /* ��Դģʽ֪ͨ */
        NULL,                             /* ����ʧ��֪ͨ��Ϣ */
        0                                 /* ����δ�����Ƿ�������ߵ�ʱ�� */
    },
    /* WIFI */
    {
        &gtPmWifiPowerInfo,               /* ��Դ��Ϣ */
        PM_POWER_MODE_OFF,                /* �����ĵ�Դģʽ */
        TRUE,                             /* Ĭ���Ƿ�������� */
        TRUE,                             /* �Ƿ�������� */
        FALSE,                            /* �Ƿ����ҵ�� */
        FALSE,                            /* �����������Ƿ���Ժ��� */
        0,                                /* ��ҵ��ʼʱ�� */
        NULL,                             /* ��Դģʽ֪ͨ */
        NULL,                             /* ����ʧ��֪ͨ��Ϣ */
        0                                 /* ����δ�����Ƿ�������ߵ�ʱ�� */
    },
    /* ��Ƶ */
    {
        NULL,                             /* ��Դ��Ϣ */
        PM_POWER_MODE_OFF,                /* �����ĵ�Դģʽ */
        TRUE,                             /* Ĭ���Ƿ�������� */
        TRUE,                             /* �Ƿ�������� */
        FALSE,                            /* �Ƿ����ҵ�� */
        FALSE,                            /* �����������Ƿ���Ժ��� */
        0,                                /* ��ҵ��ʼʱ�� */
        NULL,                             /* ��Դģʽ֪ͨ */
        NULL,                             /* ����ʧ��֪ͨ��Ϣ */
        0                                 /* ����δ�����Ƿ�������ߵ�ʱ�� */
    }
};

const uint8_t gu8PmSysNum = (uint8_t)DIAS_PM_SYS_TYPE_END;


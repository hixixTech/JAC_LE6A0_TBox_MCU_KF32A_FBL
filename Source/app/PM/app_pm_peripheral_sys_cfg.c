/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_peripheral_sys_cfg.c
* Description:  PM模块
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  徐高腾        新建基线
* 2020-08-04   V11.01.01.01.00  徐高腾        一级休眠增加模组短暂唤醒
* 2020-08-05   V11.01.01.01.00  徐高腾        增加总线长时间OFF休眠超时功能
* 2020-08-05   V11.01.01.01.00  徐高腾        模组开关机休眠唤醒相关GPIO变更
* 2020-08-11   V11.01.01.01.00  徐高腾        模组休眠唤醒状态反馈改为不支持
* 2020-08-14   V11.01.01.01.00  徐高腾        增加WIFI和音频控制配置(音频暂无引脚配置)
* 2020-08-14   V11.01.01.01.00  徐高腾        增加控制引脚状态检测功能
* 2020-09-11   V11.01.01.01.00  徐高腾        增加模组运行状态检测功能
******************************************************************************/


/******************************************************************************
* 头文件
******************************************************************************/
#include "app_pm_peripheral_sys_cfg.h"
#include "app_pm_peripheral_sys_reset.h"
#include "app_pm_public_define.h"
#include "app_pm_lte_dcpd_com.h"
#include "os_log.h"
#include "app_pm_drv.h"
#include "ecu_gpio.h"

/******************************************************************************
* 宏定义
******************************************************************************/
#define DIAS_PM_GENERAL_INIT_MONITOR_PERIOD         (120000u / DIAS_PM_PERIOD_TIMER) /* 一般系统运行状态监控初始值 */ //20min->2min
#define DIAS_PM_GENERAL_ABNORMAL_CONFIRM_PERIOD     (30000u / DIAS_PM_PERIOD_TIMER)  /* 一般系统心跳超时时间 */  //120s->30s
#define DIAS_PM_SYS_ABNORMAL_IGNORE_TIMER           36000u                           /* 可以忽略系统异常的时间(s) */


/******************************************************************************
* 变量定义
******************************************************************************/
/* LTE模组电源信息 */
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
    FALSE,                                                     /* 初始时系统是否需要异常监控 */
    FALSE,                                                     /* 当前系统是否需要异常监控 */
    FALSE,                                                     /* 系统是否故障(异常后重启未能修复) */
    0,                                                         /* 下次异常处理时间记录位置 */
    {0},                                                       /* 近几次系统异常处理时间 */
    DIAS_PM_GENERAL_INIT_MONITOR_PERIOD,                       /* 初始时异常检测周期 */
    DIAS_PM_GENERAL_INIT_MONITOR_PERIOD,                       /* 剩余异常确认时间，当为0时，表示异常发生 */
    DIAS_PM_GENERAL_ABNORMAL_CONFIRM_PERIOD,                   /* 心跳超时时间 */
    DIAS_PM_SYS_ABNORMAL_IGNORE_TIMER,                         /* 可以忽略系统异常的时间(s) */
    &gtPmLteFeedbackInfo,                                      /* 状态反馈信息(检测系统异常方法之一，可与其他检测方法并存) */
    {3000u / DIAS_PM_PERIOD_TIMER, 3000u / DIAS_PM_PERIOD_TIMER, NULL},      /* 发送心跳方法(检测系统异常方法之一，可与其他检测方法并存) */
    ApiPmResetMPU                                              /* 系统运行异常处理方法 */
};
#endif

static DiasPmPowerUpCtrlInfo_t gtLtePowerUpCtrlInfo =
{
    TRUE,                       /* 默认是否上电 */
    FALSE,                      /* 当前上电状态 */
    FALSE,                      /* 上电是否自动开机 */
    TRUE,                       /* 休眠后是否下电 */
    DiasPmModPowerUp,           /* 控制上电 */
    DiasPmModPowerDown          /* 控制下电 */
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
    DIAS_PM_STATUS_UNSTABLE,     /* 当前电源状态 */
    DIAS_PM_STATUS_POWER_OFF,    /* 期待电源状态 */
    DIAS_PM_STATUS_POWER_OFF,    /* 前一次期待电源状态 */
    DIAS_PM_STATUS_UNSTABLE,     /* 实时电源状态反馈 */
#ifdef DIAS_PM_MONITOR_ENABLE
    &gtPmLteMonitorCfg,          /* 系统监控配置 */
#else
    NULL,                        /* 系统监控配置 */
#endif
    {
        {DIAS_PM_FEEDBACK_MODE_PIN_LOW,         {{GPIO_4G_STATUS}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}},        FALSE, {0}},    /* 判断系统是否开机 */
        {DIAS_PM_FEEDBACK_MODE_NONE,            {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, TRUE,  {0}},    /* 判断系统是否唤醒 */
        {DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_HIGH, {{GPIO_WKUP_4G}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}},          FALSE, {0}}     /* 判断系统唤醒休眠控制状态 */
    },
    DIAS_PM_CTRL_NONE,           /* 电源控制类型 */
    DIAS_PM_CTRL_IDLE,           /* 电源控制状态 */
    DIAS_PM_CTRL_IDLE,           /* 前一次电源控制状态 */
    &gtLtePowerUpCtrlInfo,       /* 上电信息 */
    0,                           /* 控制次数 */
    0,                           /* 当前控制步数 */
    0,                           /* 电源状态强制停留时间 */
    3,                           /* 等待电源状态确认时间 */
    TRUE,                        /* 是否正在进行电源状态确认 */
    0,                           /* 当前已控制时间 */
    0,                           /* 当前已等待反馈时间 */
    {
        /* 分步控制配置 */                    /* 控制步数 */                                                         /* LogId */                   /* 最长反馈时间 */
        /* DIAS_PM_CTRL_NONE配置 */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_ON配置 */
        {gatPmLtePowerOnStepCfg,              sizeof(gatPmLtePowerOnStepCfg) / sizeof(gatPmLtePowerOnStepCfg[0]),    DIAS_U8_INVALID_VALUE,        45000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_POWER_OFF配置 */
        {gatPmLtePowerOffStepCfg,             sizeof(gatPmLtePowerOffStepCfg) / sizeof(gatPmLtePowerOffStepCfg[0]),  DIAS_U8_INVALID_VALUE,        30000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_WAKE_UP配置 */
        {gatPmLteWakeupStepCfg,               sizeof(gatPmLteWakeupStepCfg) / sizeof(gatPmLteWakeupStepCfg[0]),      DIAS_U8_INVALID_VALUE,        2000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_SLEEP配置 */
        {gatPmLteSleepStepCfg,                sizeof(gatPmLteSleepStepCfg) / sizeof(gatPmLteSleepStepCfg[0]),        DIAS_U8_INVALID_VALUE,        2000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_RESET配置 */
        {gatPmLteResetStepCfg,                sizeof(gatPmLteResetStepCfg) / sizeof(gatPmLteResetStepCfg[0]),        DIAS_U8_INVALID_VALUE,        5000 / DIAS_PM_PERIOD_TIMER},
        /* DIAS_PM_CTRL_HARD_POWER_OFF配置 */
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

/* 蓝牙电源信息 */
static DiasPmPowerInfo_t gtPmBlePowerInfo =
{
    DIAS_PM_STATUS_UNSTABLE,     /* 当前电源状态 */
    DIAS_PM_STATUS_POWER_OFF,    /* 期待电源状态 */
    DIAS_PM_STATUS_POWER_OFF,    /* 前一次期待电源状态 */
    DIAS_PM_STATUS_UNSTABLE,     /* 实时电源状态反馈 */
    NULL,                        /* 系统监控配置 */
    {
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}},    /* 判断系统是否开机 */
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, TRUE,  {0}},    /* 判断系统是否唤醒 */
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}}     /* 判断系统唤醒休眠控制状态 */
    },
    DIAS_PM_CTRL_NONE,           /* 电源控制类型 */
    DIAS_PM_CTRL_IDLE,           /* 电源控制状态 */
    DIAS_PM_CTRL_IDLE,           /* 前一次电源控制状态 */
    NULL,                        /* 上电信息 */
    0,                           /* 控制次数 */
    0,                           /* 当前控制步数 */
    0,                           /* 电源状态强制停留时间 */
    0,                           /* 等待电源状态确认时间 */
    TRUE,                        /* 是否正在进行电源状态确认 */
    0,                           /* 当前已控制时间 */
    0,                           /* 当前已等待反馈时间 */
    {
        /* 分步控制配置 */                    /* 控制步数 */               /* LogId */                   /* 最长反馈时间 */
        /* DIAS_PM_CTRL_NONE配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_ON配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_OFF配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_WAKE_UP配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_SLEEP配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_RESET配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_HARD_POWER_OFF配置 */
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

/* WIFI电源信息 */
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
    DIAS_PM_STATUS_UNSTABLE,     /* 当前电源状态 */
    DIAS_PM_STATUS_POWER_OFF,    /* 期待电源状态 */
    DIAS_PM_STATUS_POWER_OFF,    /* 前一次期待电源状态 */
    DIAS_PM_STATUS_UNSTABLE,     /* 实时电源状态反馈 */
    NULL,                        /* 系统监控配置 */
    {
        {DIAS_PM_FEEDBACK_MODE_NONE,            {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, TRUE,  {0}},    /* 判断系统是否开机 */
        {DIAS_PM_FEEDBACK_MODE_NONE,            {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}},    /* 判断系统是否唤醒 */
        {DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_HIGH, {{GPIO_SOSLED_CTL}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}},     FALSE, {0}}     /* 判断系统唤醒休眠控制状态 */
    },
    DIAS_PM_CTRL_NONE,           /* 电源控制类型 */
    DIAS_PM_CTRL_IDLE,           /* 电源控制状态 */
    DIAS_PM_CTRL_IDLE,           /* 前一次电源控制状态 */
    NULL,                        /* 上电信息 */
    0,                           /* 控制次数 */
    0,                           /* 当前控制步数 */
    0,                           /* 电源状态强制停留时间 */
    0,                           /* 等待电源状态确认时间 */
    TRUE,                        /* 是否正在进行电源状态确认 */
    0,                           /* 当前已控制时间 */
    0,                           /* 当前已等待反馈时间 */
    {
        /* 分步控制配置 */                    /* 控制步数 */                                                         /* LogId */                   /* 最长反馈时间 */
        /* DIAS_PM_CTRL_NONE配置 */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_ON配置 */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_OFF配置 */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_WAKE_UP配置 */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_SLEEP配置 */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_RESET配置 */
        {NULL,                                0,                                                                     DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_HARD_POWER_OFF配置 */
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

/* 音频电源信息 */
static DiasPmPowerInfo_t gtPmAudioPowerInfo =
{
    DIAS_PM_STATUS_UNSTABLE,     /* 当前电源状态 */
    DIAS_PM_STATUS_POWER_OFF,    /* 期待电源状态 */
    DIAS_PM_STATUS_POWER_OFF,    /* 前一次期待电源状态 */
    DIAS_PM_STATUS_UNSTABLE,     /* 实时电源状态反馈 */
    NULL,                        /* 系统监控配置 */
    {
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, TRUE,  {0}},    /* 判断系统是否开机 */
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}},    /* 判断系统是否唤醒 */
        {DIAS_PM_FEEDBACK_MODE_NONE, {{DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE}, {DIAS_U8_INVALID_VALUE, 0, 0, 0}}, FALSE, {0}}     /* 判断系统唤醒休眠控制状态 */
    },
    DIAS_PM_CTRL_NONE,           /* 电源控制类型 */
    DIAS_PM_CTRL_IDLE,           /* 电源控制状态 */
    DIAS_PM_CTRL_IDLE,           /* 前一次电源控制状态 */
    NULL,                        /* 上电信息 */
    0,                           /* 控制次数 */
    0,                           /* 当前控制步数 */
    0,                           /* 电源状态强制停留时间 */
    0,                           /* 等待电源状态确认时间 */
    TRUE,                        /* 是否正在进行电源状态确认 */
    0,                           /* 当前已控制时间 */
    0,                           /* 当前已等待反馈时间 */
    {
        /* 分步控制配置 */                    /* 控制步数 */               /* LogId */                   /* 最长反馈时间 */
        /* DIAS_PM_CTRL_NONE配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_ON配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_POWER_OFF配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_WAKE_UP配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_SLEEP配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_RESET配置 */
        {NULL,                                0,                           DIAS_U8_INVALID_VALUE,        0},
        /* DIAS_PM_CTRL_HARD_POWER_OFF配置 */
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

/* 休眠异常LTE模组通知配置 */
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
    /* LTE模组 */
    {
        &gtPmLtePowerInfo,                /* 电源信息 */
        PM_POWER_MODE_RUNNING,            /* 反馈的电源模式 */
        FALSE,                            /* 默认是否可以休眠 */
        FALSE,                            /* 是否可以休眠 */
        TRUE,                             /* 是否包含业务 */
        FALSE,                            /* 不允许休眠是否可以忽略 */
        0,                                /* 无业务开始时间 */
        DiasPmLteNotifyPowerMode,         /* 电源模式通知 */
        &gtPmLteSleepFailNotifyInfo,      /* 休眠失败通知信息 */
        0                                 /* 持续未反馈是否可以休眠的时间 */
    },
    /* 蓝牙 */
    {
        NULL,                             /* 电源信息 */
        PM_POWER_MODE_OFF,                /* 反馈的电源模式 */
        TRUE,                             /* 默认是否可以休眠 */
        TRUE,                             /* 是否可以休眠 */
        FALSE,                            /* 是否包含业务 */
        FALSE,                            /* 不允许休眠是否可以忽略 */
        0,                                /* 无业务开始时间 */
        NULL,                             /* 电源模式通知 */
        NULL,                             /* 休眠失败通知信息 */
        0                                 /* 持续未反馈是否可以休眠的时间 */
    },
    /* WIFI */
    {
        &gtPmWifiPowerInfo,               /* 电源信息 */
        PM_POWER_MODE_OFF,                /* 反馈的电源模式 */
        TRUE,                             /* 默认是否可以休眠 */
        TRUE,                             /* 是否可以休眠 */
        FALSE,                            /* 是否包含业务 */
        FALSE,                            /* 不允许休眠是否可以忽略 */
        0,                                /* 无业务开始时间 */
        NULL,                             /* 电源模式通知 */
        NULL,                             /* 休眠失败通知信息 */
        0                                 /* 持续未反馈是否可以休眠的时间 */
    },
    /* 音频 */
    {
        NULL,                             /* 电源信息 */
        PM_POWER_MODE_OFF,                /* 反馈的电源模式 */
        TRUE,                             /* 默认是否可以休眠 */
        TRUE,                             /* 是否可以休眠 */
        FALSE,                            /* 是否包含业务 */
        FALSE,                            /* 不允许休眠是否可以忽略 */
        0,                                /* 无业务开始时间 */
        NULL,                             /* 电源模式通知 */
        NULL,                             /* 休眠失败通知信息 */
        0                                 /* 持续未反馈是否可以休眠的时间 */
    }
};

const uint8_t gu8PmSysNum = (uint8_t)DIAS_PM_SYS_TYPE_END;


/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_public_define.h
* Author:
* Description:  PM模块
* 
* Others:       other description
*
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 **********************************
* Date            Version       Author     Description
* 2020-08-03   V11.01.01.01.00  徐高腾        新建基线
* 2020-08-04   V11.01.01.01.00  徐高腾        一级休眠增加模组短暂唤醒
* 2020-08-05   V11.01.01.01.00  徐高腾        增加总线长时间OFF休眠超时功能
* 2020-08-14   V11.01.01.01.00  徐高腾        增加控制引脚状态检测功能
* 2020-09-11   V11.01.01.01.00  徐高腾        增加引脚方波检测功能
******************************************************************************/

#ifndef _APP_PM_PUBLIC_DEFINE_H_
#define _APP_PM_PUBLIC_DEFINE_H_
#include "pm_public.h"
#include "app_pm_mv.h"
#include "rtos_taskinit.h"

/******************************************************************************
* 宏定义
******************************************************************************/
#define DIAS_PM_PERIOD_TIMER                        APP_TASK_PRD_MS
#define DIAS_PM_MAX_SYS_ABNORMAL_COUNT              10u                              /* 最大允许系统异常次数 */

#ifndef OK
#define OK                  0
#endif

#ifndef ERR
#define ERR                 -1
#endif


#define DIAS_U8_INVALID_VALUE                       0xFF
#define DIAS_U16_INVALID_VALUE                      0xFFFF
#define DIAS_U32_INVALID_VALUE                      0xFFFFFFFF

/******************************************************************************
* 类型定义
******************************************************************************/
typedef int32_t  (*DiasPmPowerCtrlFunc)(uint8_t u8Para);
typedef void     (*DiasPmSendHeartBeatFunc)(void);
typedef int32_t  (*DiasPmProcSysAbnormalFunc)(void);
typedef void     (*DiasPmNotifyPowerModeFunc)(PmPowerModeStatus_e ePowerMode);
typedef void     (*DiasPmPeripheralUintCtrlFunc)(void);
typedef void     (*DiasPmPowerUpDownCtrlFunc)(void);
typedef int32_t  (*DiasPmMsgNotifyFunc)(void);



typedef enum
{
    DIAS_PM_CTRL_NONE = 0,       /* 不控制 */
    DIAS_PM_CTRL_POWER_ON,       /* 控制开机 */
    DIAS_PM_CTRL_POWER_OFF,      /* 控制关机 */
    DIAS_PM_CTRL_WAKE_UP,        /* 控制唤醒 */
    DIAS_PM_CTRL_SLEEP,          /* 控制休眠 */
    DIAS_PM_CTRL_RESET,          /* 控制重启 */
    DIAS_PM_CTRL_HARD_POWER_OFF, /* 硬关机 */
    DIAS_PM_CTRL_TYPE_END
}DiasPmPowerCtrlType_e;

typedef enum
{
    DIAS_PM_CTRL_IDLE = 0,       /* 空闲中 */
    DIAS_PM_CTRL_STRAT,          /* 控制启动 */
    DIAS_PM_CTRL_KEEP,           /* 保持控制 */
    DIAS_PM_CTRL_FEEDBACK,       /* 控制结果反馈 */
    DIAS_PM_CTRL_TIMEOUT         /* 控制Timeout */
}DiasPmPowerCtrlStatus_e;

enum
{
    DIAS_PM_FEEDBACK_TYPE_POWERON = 0,      /* 开关机反馈 */
    DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS,    /* 唤醒休眠反馈 */
    DIAS_PM_FEEDBACK_TYPE_WAKEUP_CTRL,      /* 唤醒休眠控制 */
    DIAS_PM_FEEDBACK_TYPE_END
};

typedef enum
{
    DIAS_PM_FEEDBACK_MODE_NONE = 0,             /* 不反馈 */
    DIAS_PM_FEEDBACK_MODE_PIN_HIGH,             /* Input引脚反馈(高电平为工作状态) */
    DIAS_PM_FEEDBACK_MODE_PIN_LOW,              /* Input引脚反馈(低电平为工作状态) */
    DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_HIGH,      /* Output引脚反馈(高电平为工作状态) */
    DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_LOW,       /* Output引脚反馈(低电平为工作状态) */
    DIAS_PM_FEEDBACK_MODE_ADC,                  /* AD通道反馈 */
    DIAS_PM_FEEDBACK_MODE_SQU_WAKE             /*  Input引脚方波反馈(方波为工作状态) */
}DiasPmFeedbackMode_e;

typedef enum
{
    DIAS_PM_WAIT_SEND = 0,             /* 等待发送触发 */
    DIAS_PM_SENDING,                   /* 发送消息 */
    DIAS_PM_WAIT_ACK,                  /* 等待响应消息 */
    DIAS_PM_NOTIFY_SUCCESS,            /* 发送成功 */
    DIAS_PM_NOTIFY_FAILURE,            /* 发送失败 */
}DiasPmMsgNotifyStatus_e;

typedef struct
{
    uint16_t u16ConfirmTimeLeft;               /* 状态变化确认剩余时间 */
    uint16_t u16ConfirmTime;                   /* 状态确认时间 */
    uint16_t u16WorkMinStay;                   /* 工作状态至少停留时间 */
    uint16_t u16OutWorkMinStay;                /* 不工作状态至少停留时间 */
}DiasPmStatusConfirmInfo_t;

typedef struct
{
    uint8_t  u8Pin;                          /* 状态反馈引脚 */
}DiasPmPinPara_t;

typedef struct
{
    uint8_t  u8Channel;                      /* 状态反馈ADC通道 */
}DiasPmChannelPara_t;

typedef struct
{
    uint8_t  u8Pin;                          /* 状态反馈引脚 */
    uint8_t  u8PinStatus;                    /* 引脚状态 */
    uint16_t u16MaxKeepTime;                 /* 电平最大持续时间 */
    uint16_t u16KeepTime;                    /* 当前已持续时间 */
}DiasPmSquWavePara_t;

/* 上海编译器不支持共同体定义时初始化，改为结构体 */
typedef struct
{
    DiasPmPinPara_t      tPinPara;        /* 引脚状态反馈参数 */
    DiasPmChannelPara_t  tChannelPara;    /* ADC通道状态反馈参数 */
    DiasPmSquWavePara_t  tSquWavePara;    /* 方波反馈参数 */
}DiasPmFeedbackPara_t;

typedef struct
{
    DiasPmFeedbackMode_e             eFeedbackMode;         /* 状态反馈方式 */
    DiasPmFeedbackPara_t             tFeedbackPara;         /* 状态反馈参数 */
    BOOL                             bFeedbackStatus;       /* 反馈状态 */
    DiasPmStatusConfirmInfo_t        tFeedbackConfirmInfo;  /* 状态确认相关信息 */
}DiasPmFeedbackInfo_t;

typedef struct
{
    uint16_t                    u16TimeSinceLastSend;         /* 上次发送已过去时间 */
    uint16_t                    u16SendPeriod;                /* 发送周期 */
    DiasPmSendHeartBeatFunc     sendHeartBeatFunc;            /* 发送心跳方法 */
}DiasPmSendHeartBeatInfo_t;

typedef struct
{
    DiasPmPowerCtrlFunc           ctrlFunc;                  /* 控制函数 */
    uint8_t                       u8Para;                    /* 控制参数 */
    uint32_t                      u32CtrlTime;               /* 需要持续控制的时间 */
}DiasPmCtrlStepCfg_t;

typedef struct
{
    DiasPmCtrlStepCfg_t           *ptCtrlStepCfg;            /* 控制步骤 */
    uint8_t                       u8CtrlStepNum;             /* 控制步数 */
    uint8_t                       u8LogId;                   /* Log Id */
    uint16_t                      u16MaxFeedbackTime;        /* 最长反馈时间 */
}DiasPmPowerCtrlCfg_t;

typedef struct
{
    BOOL                        bInitMonitorState;                                    /* 初始时系统是否需要异常监控 */
    BOOL                        bCurrMonitorState;                                    /* 当前系统是否需要异常监控 */
    BOOL                        bSysFault;                                            /* 系统是否故障(异常后重启未能修复) */
    uint8_t                     u8NextAbnormalTimeIndex;                              /* 下次异常处理时间记录位置 */
    uint32_t                    au32SysAbnormalTime[DIAS_PM_MAX_SYS_ABNORMAL_COUNT];  /* 近几次系统异常处理时间 */
    uint32_t                    u32InitMonitorPeriod;                                 /* 初始时异常检测周期 */
    uint32_t                    u32HeartbeatDeadTimer;                                /* 剩余异常确认时间，当为0时，表示异常发生 */
    uint32_t                    u32AbnormalConfirmPeriod;                             /* 心跳超时时间 */
    uint32_t                    u32SysAbnormalIgnorePeriod;                           /* 可以忽略系统异常的时间(s) */
    DiasPmFeedbackInfo_t        *ptFeedbackInfo;                                      /* 状态反馈相关信息(检测系统异常方法之一，可与其他检测方法并存) */
    DiasPmSendHeartBeatInfo_t   tSendHeartBeatInfo;                                   /* 发送心跳相关信息(检测系统异常方法之一，可与其他检测方法并存) */
    DiasPmProcSysAbnormalFunc   procSysAbnormalFunc;                                  /* 系统运行异常处理方法 */
}DiasPmSysMonitorCfg_t;

typedef struct
{
    BOOL                               bPowerUpDefault;                                      /* 默认是否上电 */
    BOOL                               bPowerUp;                                             /* 当前上电状态 */
    BOOL                               bPowerOnAfterPowerUp;                                 /* 上电是否自动开机 */
    BOOL                               bPowerDownInLowPower;                                 /* 休眠后是否下电 */
    DiasPmPowerUpDownCtrlFunc          powerUpFunc;                                          /* 控制上电 */
    DiasPmPowerUpDownCtrlFunc          powerDownFunc;                                        /* 控制下电 */
}DiasPmPowerUpCtrlInfo_t;

typedef struct
{
    DiasPmPowerStatus_e                eCurrPowerStatus;                                     /* 当前电源状态 */
    DiasPmPowerStatus_e                eExpectPowerStatus;                                   /* 期待电源状态 */
    DiasPmPowerStatus_e                eLastExpectPowerStatus;                               /* 前一次期待电源状态 */
    DiasPmPowerStatus_e                eFeedbackPowerStatus;                                 /* 实时电源状态反馈 */
    DiasPmSysMonitorCfg_t              *ptMonitorCfg;                                        /* 系统监控配置 */
    DiasPmFeedbackInfo_t               atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_END];       /* 电源状态反馈配置 */
    DiasPmPowerCtrlType_e              ePowerCtrlType;                                       /* 电源控制类型 */
    DiasPmPowerCtrlStatus_e            ePowerCtrlStatus;                                     /* 电源控制状态 */
    DiasPmPowerCtrlStatus_e            ePrePowerCtrlStatus;                                  /* 前一次电源控制状态 */
    DiasPmPowerUpCtrlInfo_t            *ptPowerUpCtrlInfo;                                   /* 上电信息 */
    uint8_t                            u8CtrlFailCount;                                      /* 控制次数 */
    uint8_t                            u8CurrStep;                                           /* 当前控制步数 */
    uint16_t                           u16PowerStatusConfirmTimeLeft;                        /* 电源状态确认时间 */
    uint16_t                           u16ForceStayLeftTime;                                 /* 电源状态强制停留时间 */
    BOOL                               bPowerStatusConfirm;                                  /* 是否正在进行电源状态确认 */
    uint32_t                           u32CtrlTimerCount;                                    /* 当前已控制时间 */
    uint32_t                           u32FeedbackTimerCount;                                /* 当前已等待反馈时间 */
    DiasPmPowerCtrlCfg_t               atPowerCtrlCfg[DIAS_PM_CTRL_TYPE_END];                /* 电源控制配置 */
    DiasPmPeripheralUintCtrlFunc       aPeripheralUintCtrlFunc[DIAS_PM_STATUS_NUM];          /* 外围器件控制函数 */
}DiasPmPowerInfo_t;

typedef struct
{
    DiasPmMsgNotifyStatus_e eNotifyStatus;
    uint8_t                 u8FailureCount;
    uint8_t                 u8MaxFailureCount;
    uint16_t                u16WaitTime;
    uint16_t                u16SendTimeout;
    DiasPmMsgNotifyFunc     msgNotifyFunc;
}DiasPmMsgNotifyInfo_t;

typedef struct
{
    DiasPmPowerInfo_t          *ptPowerInfo;                                    /* 电源信息 */
    PmPowerModeStatus_e        ePowerModeStatus;                                /* 反馈的电源模式 */
    BOOL                       bIsSleepReadyDefault;                            /* 默认是否可以休眠 */
    BOOL                       bIsSleepReady;                                   /* 是否可以休眠 */
    BOOL                       bHasService;                                     /* 是否包含业务 */
    BOOL                       bIsSleepReadyIgnore;                             /* 不允许休眠是否可以忽略 */
    uint32_t                   u32NoServiceStartTime;                           /* 无业务开始时间 */
    DiasPmNotifyPowerModeFunc  notifyPowerModeFunc;                             /* 电源模式通知 */
    DiasPmMsgNotifyInfo_t*     ptSleepFailNotifyInfo;                           /* 休眠失败通知信息 */
    uint16_t                   u16NoSleepReadyMsgTimer;                         /* 持续未反馈是否可以休眠的时间 */
}DiasPmSysInfo_t;

enum
{
    DIAS_PM_POWER_TIMER_11 = 0,     /* Sleep1模组唤醒周期 */
    DIAS_PM_POWER_TIMER_12,         /* Sleep1模组周期唤醒总时长 */
    DIAS_PM_POWER_TIMER_2,         /* Sleep2模组监听时长 */
    DIAS_PM_POWER_TIMER_END
};

typedef enum
{
    DIAS_PM_WAKEUP_OK = 0,
    DIAS_PM_WAKEUP_ERR,
    DIAS_PM_SLEEP_OK,
    DIAS_PM_SLEEP_ERR,
}DiasPmModuleState_e;

typedef enum
{
    DIAS_PM_HANDLE_NONE = 0,
    DIAS_PM_HANDLE_MCU_START,
    DIAS_PM_MCU_STANDBY,
    DIAS_PM_MCU_WAKEUP,
    DIAS_PM_MCU_WAKEUP_INVALID_SOURCE,
    DIAS_PM_MCU_STANDBY_FAILED,
    DIAS_PM_MCU_UNEXPECT_RESET,
    DIAS_PM_ALL_SYSTEM_RESET,
    DIAS_PM_ALL_SYSTEM_FORCE_RESET,
    DIAS_PM_MCU_POWER_UP,
    DIAS_PM_MCU_WAKEUP_BY_RTC,
    DIAS_PM_MCU_WAKEUP_BY_SERVICE,
}DiasPmPowerModeHandleState_e;

typedef void (*DiasPmPowerModeHandleFunc)(void);
typedef BOOL (*DiasPmConditionJudgeFunc)(void);
typedef BOOL (*DiasPmChangePowerModeFunc)(uint8_t u8Mode);
typedef BOOL (*DiasPmWakeupFunc)(void);

typedef struct
{
    const uint8_t             u8SleepPriority;   /* 值越小优先级越高 */
    const uint8_t             u8WakeupPriority;  /* 值越小优先级越高 */
    DiasPmModuleState_e       eModuleState;      /* 模块休眠唤醒状态 */
    DiasPmConditionJudgeFunc  sleepReadyFunc;
    DiasPmConditionJudgeFunc  hasSleptFunc;
    DiasPmChangePowerModeFunc changePowerModeFunc;
    DiasPmWakeupFunc          wakeupFunc;
}DiasPmMcuModuleInfo_t;

typedef enum
{
    DIAS_PM_MODE_RELATE_TYPE_NORMAL = 0,
    DIAS_PM_MODE_RELATE_TYPE_LOW_POWER,
    DIAS_PM_MODE_RELATE_TYPE_BAK_BAT,
    DIAS_PM_MODE_RELATE_TYPE_NUM
}DiasPmPowerModeRelateType_e;

typedef struct
{
    DiasPmPowerModeHandleFunc handleFunc;
    DiasPmPowerStatus_e       aePowerStatus[DIAS_PM_SYS_TYPE_END];
    PmPowerModeStatus_e       aeRelatePowerMode[DIAS_PM_MODE_RELATE_TYPE_NUM];
}DiasPmPowerModeCfg_t;

typedef struct
{
    PmPowerModeStatus_e       ePreStatus;                   /* 变更前电源模式 */
    PmPowerModeStatus_e       eNextStatus;                  /* 变更后电源模式 */
    DiasPmPowerModeHandleState_e ePowerModeHandleState;        /* 电源模式切换状态机状态 */
}DiasPmPowerModeChangeCfg_t;

/******************************************************************************
* 变量声明
******************************************************************************/
extern BOOL gbPmResetMcu;
extern BOOL gbPmMonitorDisable;
extern BOOL gbMonitorType;


#endif /* _APP_PM_PUBLIC_DEFINE_H_ */



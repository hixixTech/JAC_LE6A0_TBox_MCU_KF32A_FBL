/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_public_define.h
* Author:
* Description:  PMģ��
* 
* Others:       other description
*
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** �޸���ʷ **********************************
* Date            Version       Author     Description
* 2020-08-03   V11.01.01.01.00  �����        �½�����
* 2020-08-04   V11.01.01.01.00  �����        һ����������ģ����ݻ���
* 2020-08-05   V11.01.01.01.00  �����        �������߳�ʱ��OFF���߳�ʱ����
* 2020-08-14   V11.01.01.01.00  �����        ���ӿ�������״̬��⹦��
* 2020-09-11   V11.01.01.01.00  �����        �������ŷ�����⹦��
******************************************************************************/

#ifndef _APP_PM_PUBLIC_DEFINE_H_
#define _APP_PM_PUBLIC_DEFINE_H_
#include "pm_public.h"
#include "app_pm_mv.h"
#include "rtos_taskinit.h"

/******************************************************************************
* �궨��
******************************************************************************/
#define DIAS_PM_PERIOD_TIMER                        APP_TASK_PRD_MS
#define DIAS_PM_MAX_SYS_ABNORMAL_COUNT              10u                              /* �������ϵͳ�쳣���� */

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
* ���Ͷ���
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
    DIAS_PM_CTRL_NONE = 0,       /* ������ */
    DIAS_PM_CTRL_POWER_ON,       /* ���ƿ��� */
    DIAS_PM_CTRL_POWER_OFF,      /* ���ƹػ� */
    DIAS_PM_CTRL_WAKE_UP,        /* ���ƻ��� */
    DIAS_PM_CTRL_SLEEP,          /* �������� */
    DIAS_PM_CTRL_RESET,          /* �������� */
    DIAS_PM_CTRL_HARD_POWER_OFF, /* Ӳ�ػ� */
    DIAS_PM_CTRL_TYPE_END
}DiasPmPowerCtrlType_e;

typedef enum
{
    DIAS_PM_CTRL_IDLE = 0,       /* ������ */
    DIAS_PM_CTRL_STRAT,          /* �������� */
    DIAS_PM_CTRL_KEEP,           /* ���ֿ��� */
    DIAS_PM_CTRL_FEEDBACK,       /* ���ƽ������ */
    DIAS_PM_CTRL_TIMEOUT         /* ����Timeout */
}DiasPmPowerCtrlStatus_e;

enum
{
    DIAS_PM_FEEDBACK_TYPE_POWERON = 0,      /* ���ػ����� */
    DIAS_PM_FEEDBACK_TYPE_WAKEUP_STATUS,    /* �������߷��� */
    DIAS_PM_FEEDBACK_TYPE_WAKEUP_CTRL,      /* �������߿��� */
    DIAS_PM_FEEDBACK_TYPE_END
};

typedef enum
{
    DIAS_PM_FEEDBACK_MODE_NONE = 0,             /* ������ */
    DIAS_PM_FEEDBACK_MODE_PIN_HIGH,             /* Input���ŷ���(�ߵ�ƽΪ����״̬) */
    DIAS_PM_FEEDBACK_MODE_PIN_LOW,              /* Input���ŷ���(�͵�ƽΪ����״̬) */
    DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_HIGH,      /* Output���ŷ���(�ߵ�ƽΪ����״̬) */
    DIAS_PM_FEEDBACK_MODE_OUTPUT_PIN_LOW,       /* Output���ŷ���(�͵�ƽΪ����״̬) */
    DIAS_PM_FEEDBACK_MODE_ADC,                  /* ADͨ������ */
    DIAS_PM_FEEDBACK_MODE_SQU_WAKE             /*  Input���ŷ�������(����Ϊ����״̬) */
}DiasPmFeedbackMode_e;

typedef enum
{
    DIAS_PM_WAIT_SEND = 0,             /* �ȴ����ʹ��� */
    DIAS_PM_SENDING,                   /* ������Ϣ */
    DIAS_PM_WAIT_ACK,                  /* �ȴ���Ӧ��Ϣ */
    DIAS_PM_NOTIFY_SUCCESS,            /* ���ͳɹ� */
    DIAS_PM_NOTIFY_FAILURE,            /* ����ʧ�� */
}DiasPmMsgNotifyStatus_e;

typedef struct
{
    uint16_t u16ConfirmTimeLeft;               /* ״̬�仯ȷ��ʣ��ʱ�� */
    uint16_t u16ConfirmTime;                   /* ״̬ȷ��ʱ�� */
    uint16_t u16WorkMinStay;                   /* ����״̬����ͣ��ʱ�� */
    uint16_t u16OutWorkMinStay;                /* ������״̬����ͣ��ʱ�� */
}DiasPmStatusConfirmInfo_t;

typedef struct
{
    uint8_t  u8Pin;                          /* ״̬�������� */
}DiasPmPinPara_t;

typedef struct
{
    uint8_t  u8Channel;                      /* ״̬����ADCͨ�� */
}DiasPmChannelPara_t;

typedef struct
{
    uint8_t  u8Pin;                          /* ״̬�������� */
    uint8_t  u8PinStatus;                    /* ����״̬ */
    uint16_t u16MaxKeepTime;                 /* ��ƽ������ʱ�� */
    uint16_t u16KeepTime;                    /* ��ǰ�ѳ���ʱ�� */
}DiasPmSquWavePara_t;

/* �Ϻ���������֧�ֹ�ͬ�嶨��ʱ��ʼ������Ϊ�ṹ�� */
typedef struct
{
    DiasPmPinPara_t      tPinPara;        /* ����״̬�������� */
    DiasPmChannelPara_t  tChannelPara;    /* ADCͨ��״̬�������� */
    DiasPmSquWavePara_t  tSquWavePara;    /* ������������ */
}DiasPmFeedbackPara_t;

typedef struct
{
    DiasPmFeedbackMode_e             eFeedbackMode;         /* ״̬������ʽ */
    DiasPmFeedbackPara_t             tFeedbackPara;         /* ״̬�������� */
    BOOL                             bFeedbackStatus;       /* ����״̬ */
    DiasPmStatusConfirmInfo_t        tFeedbackConfirmInfo;  /* ״̬ȷ�������Ϣ */
}DiasPmFeedbackInfo_t;

typedef struct
{
    uint16_t                    u16TimeSinceLastSend;         /* �ϴη����ѹ�ȥʱ�� */
    uint16_t                    u16SendPeriod;                /* �������� */
    DiasPmSendHeartBeatFunc     sendHeartBeatFunc;            /* ������������ */
}DiasPmSendHeartBeatInfo_t;

typedef struct
{
    DiasPmPowerCtrlFunc           ctrlFunc;                  /* ���ƺ��� */
    uint8_t                       u8Para;                    /* ���Ʋ��� */
    uint32_t                      u32CtrlTime;               /* ��Ҫ�������Ƶ�ʱ�� */
}DiasPmCtrlStepCfg_t;

typedef struct
{
    DiasPmCtrlStepCfg_t           *ptCtrlStepCfg;            /* ���Ʋ��� */
    uint8_t                       u8CtrlStepNum;             /* ���Ʋ��� */
    uint8_t                       u8LogId;                   /* Log Id */
    uint16_t                      u16MaxFeedbackTime;        /* �����ʱ�� */
}DiasPmPowerCtrlCfg_t;

typedef struct
{
    BOOL                        bInitMonitorState;                                    /* ��ʼʱϵͳ�Ƿ���Ҫ�쳣��� */
    BOOL                        bCurrMonitorState;                                    /* ��ǰϵͳ�Ƿ���Ҫ�쳣��� */
    BOOL                        bSysFault;                                            /* ϵͳ�Ƿ����(�쳣������δ���޸�) */
    uint8_t                     u8NextAbnormalTimeIndex;                              /* �´��쳣����ʱ���¼λ�� */
    uint32_t                    au32SysAbnormalTime[DIAS_PM_MAX_SYS_ABNORMAL_COUNT];  /* ������ϵͳ�쳣����ʱ�� */
    uint32_t                    u32InitMonitorPeriod;                                 /* ��ʼʱ�쳣������� */
    uint32_t                    u32HeartbeatDeadTimer;                                /* ʣ���쳣ȷ��ʱ�䣬��Ϊ0ʱ����ʾ�쳣���� */
    uint32_t                    u32AbnormalConfirmPeriod;                             /* ������ʱʱ�� */
    uint32_t                    u32SysAbnormalIgnorePeriod;                           /* ���Ժ���ϵͳ�쳣��ʱ��(s) */
    DiasPmFeedbackInfo_t        *ptFeedbackInfo;                                      /* ״̬���������Ϣ(���ϵͳ�쳣����֮һ������������ⷽ������) */
    DiasPmSendHeartBeatInfo_t   tSendHeartBeatInfo;                                   /* �������������Ϣ(���ϵͳ�쳣����֮һ������������ⷽ������) */
    DiasPmProcSysAbnormalFunc   procSysAbnormalFunc;                                  /* ϵͳ�����쳣������ */
}DiasPmSysMonitorCfg_t;

typedef struct
{
    BOOL                               bPowerUpDefault;                                      /* Ĭ���Ƿ��ϵ� */
    BOOL                               bPowerUp;                                             /* ��ǰ�ϵ�״̬ */
    BOOL                               bPowerOnAfterPowerUp;                                 /* �ϵ��Ƿ��Զ����� */
    BOOL                               bPowerDownInLowPower;                                 /* ���ߺ��Ƿ��µ� */
    DiasPmPowerUpDownCtrlFunc          powerUpFunc;                                          /* �����ϵ� */
    DiasPmPowerUpDownCtrlFunc          powerDownFunc;                                        /* �����µ� */
}DiasPmPowerUpCtrlInfo_t;

typedef struct
{
    DiasPmPowerStatus_e                eCurrPowerStatus;                                     /* ��ǰ��Դ״̬ */
    DiasPmPowerStatus_e                eExpectPowerStatus;                                   /* �ڴ���Դ״̬ */
    DiasPmPowerStatus_e                eLastExpectPowerStatus;                               /* ǰһ���ڴ���Դ״̬ */
    DiasPmPowerStatus_e                eFeedbackPowerStatus;                                 /* ʵʱ��Դ״̬���� */
    DiasPmSysMonitorCfg_t              *ptMonitorCfg;                                        /* ϵͳ������� */
    DiasPmFeedbackInfo_t               atPowerFeedbackInfo[DIAS_PM_FEEDBACK_TYPE_END];       /* ��Դ״̬�������� */
    DiasPmPowerCtrlType_e              ePowerCtrlType;                                       /* ��Դ�������� */
    DiasPmPowerCtrlStatus_e            ePowerCtrlStatus;                                     /* ��Դ����״̬ */
    DiasPmPowerCtrlStatus_e            ePrePowerCtrlStatus;                                  /* ǰһ�ε�Դ����״̬ */
    DiasPmPowerUpCtrlInfo_t            *ptPowerUpCtrlInfo;                                   /* �ϵ���Ϣ */
    uint8_t                            u8CtrlFailCount;                                      /* ���ƴ��� */
    uint8_t                            u8CurrStep;                                           /* ��ǰ���Ʋ��� */
    uint16_t                           u16PowerStatusConfirmTimeLeft;                        /* ��Դ״̬ȷ��ʱ�� */
    uint16_t                           u16ForceStayLeftTime;                                 /* ��Դ״̬ǿ��ͣ��ʱ�� */
    BOOL                               bPowerStatusConfirm;                                  /* �Ƿ����ڽ��е�Դ״̬ȷ�� */
    uint32_t                           u32CtrlTimerCount;                                    /* ��ǰ�ѿ���ʱ�� */
    uint32_t                           u32FeedbackTimerCount;                                /* ��ǰ�ѵȴ�����ʱ�� */
    DiasPmPowerCtrlCfg_t               atPowerCtrlCfg[DIAS_PM_CTRL_TYPE_END];                /* ��Դ�������� */
    DiasPmPeripheralUintCtrlFunc       aPeripheralUintCtrlFunc[DIAS_PM_STATUS_NUM];          /* ��Χ�������ƺ��� */
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
    DiasPmPowerInfo_t          *ptPowerInfo;                                    /* ��Դ��Ϣ */
    PmPowerModeStatus_e        ePowerModeStatus;                                /* �����ĵ�Դģʽ */
    BOOL                       bIsSleepReadyDefault;                            /* Ĭ���Ƿ�������� */
    BOOL                       bIsSleepReady;                                   /* �Ƿ�������� */
    BOOL                       bHasService;                                     /* �Ƿ����ҵ�� */
    BOOL                       bIsSleepReadyIgnore;                             /* �����������Ƿ���Ժ��� */
    uint32_t                   u32NoServiceStartTime;                           /* ��ҵ��ʼʱ�� */
    DiasPmNotifyPowerModeFunc  notifyPowerModeFunc;                             /* ��Դģʽ֪ͨ */
    DiasPmMsgNotifyInfo_t*     ptSleepFailNotifyInfo;                           /* ����ʧ��֪ͨ��Ϣ */
    uint16_t                   u16NoSleepReadyMsgTimer;                         /* ����δ�����Ƿ�������ߵ�ʱ�� */
}DiasPmSysInfo_t;

enum
{
    DIAS_PM_POWER_TIMER_11 = 0,     /* Sleep1ģ�黽������ */
    DIAS_PM_POWER_TIMER_12,         /* Sleep1ģ�����ڻ�����ʱ�� */
    DIAS_PM_POWER_TIMER_2,         /* Sleep2ģ�����ʱ�� */
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
    const uint8_t             u8SleepPriority;   /* ֵԽС���ȼ�Խ�� */
    const uint8_t             u8WakeupPriority;  /* ֵԽС���ȼ�Խ�� */
    DiasPmModuleState_e       eModuleState;      /* ģ�����߻���״̬ */
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
    PmPowerModeStatus_e       ePreStatus;                   /* ���ǰ��Դģʽ */
    PmPowerModeStatus_e       eNextStatus;                  /* ������Դģʽ */
    DiasPmPowerModeHandleState_e ePowerModeHandleState;        /* ��Դģʽ�л�״̬��״̬ */
}DiasPmPowerModeChangeCfg_t;

/******************************************************************************
* ��������
******************************************************************************/
extern BOOL gbPmResetMcu;
extern BOOL gbPmMonitorDisable;
extern BOOL gbMonitorType;


#endif /* _APP_PM_PUBLIC_DEFINE_H_ */



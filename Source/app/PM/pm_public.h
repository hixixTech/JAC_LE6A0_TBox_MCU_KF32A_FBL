/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:     app_pm_public.h
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
* 2020-08-06   V11.01.01.01.00  �����        ɾ�������ѹ�������
* 2020-08-14   V11.01.01.01.00  �����        ����WIFI����Ƶ��������
******************************************************************************/
#ifndef _APP_PM_PUBLIC_H_
#define _APP_PM_PUBLIC_H_

// #include "dias_spp_main.h"
#include "app_pm_mv.h"
#include "type.h"

#define DIAS_PM_MODULE_MODE_LISTEN                  0
#define DIAS_PM_MODULE_MODE_SLEEP                   1

#define DIAS_PM_MAX_POWER_CTRL_TIMES                2                                 /* ��Դ����ʧ������������ */


typedef enum
{
    DIAS_PM_LTE = 0,             /* LTEģ�� */
    DIAS_PM_BLE,                 /* ���� */
    DIAS_PM_WIFI,                /* WIFI */
    DIAS_PM_AUDIO,               /* ��Ƶ */
    DIAS_PM_SYS_TYPE_END
}DiasPmSysType_e;

typedef enum
{
    DIAS_PM_STATUS_UNSTABLE = 0,        /* ��ȷ��״̬ */
    DIAS_PM_STATUS_POWER_OFF,           /* �ػ�״̬ */
    DIAS_PM_STATUS_WAKE_UP,             /* ����״̬ */
    DIAS_PM_STATUS_SLEEP,               /* ����״̬ */
    DIAS_PM_STATUS_RUN_ABMORMAL,        /* �쳣����״̬ */
    DIAS_PM_STATUS_POWER_CTRL_FAILURE,  /* ��Դ����ʧ�� */
    DIAS_PM_STATUS_POWER_CTRL_FAULT,    /* ��Դ���ƹ��� */
    DIAS_PM_STATUS_NUM
}DiasPmPowerStatus_e;

/*BATY CONSTRAIN INDEX*/
typedef  enum BatyVolConsIndex_Tag
{
    INDEX_BATY_CONS_OPERA = (0u),
    INDEX_BATY_CONS_OPERCANA,
    INDEX_BATY_CONS_DIAG,
    INDEX_BATY_CONS_POWER_CTRL,
    INDEX_BATY_CONS_ALL
}BatyVolConsIndex_e;


typedef  enum PmPowerModeStatus_Tag
{
    PM_POWER_MODE_OFF = 0u,
    PM_POWER_MODE_RUNNING,
    PM_POWER_MODE_SLEEP1,
    PM_POWER_MODE_SLEEP2,
    PM_POWER_MODE_SLEEP3,
    PM_POWER_MODE_BAKBAT,
    PM_POWER_MODE_POWERDOWN,
    PM_POWER_MODE_END,
    PM_POWER_MODE_RESERVED = (0u), /* T.B.D ������ģ���޸ĺ�ɾ�� */
    PM_POWER_MODE_LISTEN = 0xFD,   /* T.B.D ������ģ���޸ĺ�ɾ�� */
    PM_POWER_MODE_SLEEP = 0xFE,    /* T.B.D ������ģ���޸ĺ�ɾ�� */
    PM_POWER_MODE_INVALID = (0xFFu)
}PmPowerModeStatus_e;

/*BATY CONSTRAIN STATUS INDEX*/
typedef enum BatyVolConsStatus_Tag
{
    STATUS_BATY_CONS_ST_NORMAL = (0u),   /*VOLTAGE IS NORMAL*/       
    STATUS_BATY_CONS_ST_LOW ,            /*VOLTAGE IS UNDER NARMAL*/ 
    STATUS_BATY_CONS_ST_HIGH ,           /*VOLTAGE IS OVER NORMAL*/    
    STATUS_BATY_CONS_ST_INVALID          /*VOLTAGE IS INVALID*/
}BatyVolConsStatus_e;



/*FUNCTION FOR EXTERNAL*/
extern BOOL     ApiPmInit(void);
extern uint16_t ApiPmBatVolGet(void);
extern int32_t  ApiPmResetSystem(BOOL bSavePowerMode);
extern int32_t  ApiPmResetBLE(void);
extern int32_t  ApiPmResetMPU(void);
extern BOOL     ApiPmGetResetMPUStatus(void);
extern void     ApiPmProcess10ms(void);
extern PmPowerModeStatus_e   ApiPmPowerModeGet(void);
extern BatyVolConsStatus_e ApiPmBatyVolConsGet(BatyVolConsIndex_e eIndex);
extern void     ApiPmRegisterEventListener(void);
extern int32_t  ApiPmResetMCU(void);
extern DiasPmPowerStatus_e ApiPmGetCurrSysPowerStatus(DiasPmSysType_e eSysType);
extern void     ApiPmPeripheralCtrlMcuRun(void);
extern BOOL     ApiPmIsKeyStateOff(void);
extern BOOL     ApiPmKL15IsOn(void);


#endif /*_APP_PM_PUBLIC_H_*/


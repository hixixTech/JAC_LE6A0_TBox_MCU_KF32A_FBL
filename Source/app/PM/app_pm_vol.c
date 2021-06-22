/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_vol.c
* Description:  PMģ���ѹ����
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** �޸���ʷ ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  �����        �½�����
* 2020-08-06   V11.01.01.01.00  �����        ɾ�������ѹ�������
* 2020-08-28   V11.01.01.01.00  �����        ����12V/24Vϵͳʶ����
******************************************************************************/


/******************************************************************************
* ͷ�ļ�
******************************************************************************/

#include "os_log.h"
#include "pm_public.h"
#include "app_pm_vol.h"
#include "app_pm_mv.h"
#include "app_pm_public_define.h"


/******************************************************************************
* �궨��
******************************************************************************/
#define DIAS_PM_12V_NORMAL_TO_LOW_BATY_OPERA       8500      /* 12VϵͳA�����ܵ�ѹ��Сֵ(Normal��ΪLow) */
#define DIAS_PM_12V_LOW_TO_NORMAL_BATY_OPERA       9000      /* 12VϵͳA�����ܵ�ѹ���ֵ(Low��ΪNormal) */
#define DIAS_PM_12V_NORMAL_TO_HIGH_BATY_OPERA      16500     /* 12VϵͳA�����ܵ�ѹ���ֵ(Normal��ΪHigh) */
#define DIAS_PM_12V_HIGH_TO_NORMAL_BATY_OPERA      16000     /* 12VϵͳA�����ܵ�ѹ���ֵ(High��ΪNormal) */

#define DIAS_PM_12V_NORMAL_TO_LOW_BATY_OPERCANA    6000      /* 12VϵͳA��CAN�����ѹ��Сֵ(Normal��ΪLow) */
#define DIAS_PM_12V_LOW_TO_NORMAL_BATY_OPERCANA    6500      /* 12VϵͳA��CAN�����ѹ��Сֵ(Low��ΪNormal) */
#define DIAS_PM_12V_NORMAL_TO_HIGH_BATY_OPERCANA   18000     /* 12VϵͳA��CAN�����ѹ���ֵ(Normal��ΪHigh) */
#define DIAS_PM_12V_HIGH_TO_NORMAL_BATY_OPERCANA   17500     /* 12VϵͳA��CAN�����ѹ���ֵ(High��ΪNormal) */

#define DIAS_PM_12V_NORMAL_TO_LOW_BATY_DIAG        8500      /* 12Vϵͳ��ϵ�ѹ��Сֵ(Normal��ΪLow) */
#define DIAS_PM_12V_LOW_TO_NORMAL_BATY_DIAG        9000      /* 12Vϵͳ��ϵ�ѹ��Сֵ(Low��ΪNormal) */
#define DIAS_PM_12V_NORMAL_TO_HIGH_BATY_DIAG       16500     /* 12Vϵͳ��ϵ�ѹ���ֵ(Normal��ΪHigh) */
#define DIAS_PM_12V_HIGH_TO_NORMAL_BATY_DIAG       16000     /* 12Vϵͳ��ϵ�ѹ���ֵ(High��ΪNormal) */

#define DIAS_PM_12V_NORMAL_TO_LOW_BATY_POWER_CTRL  6000      /* 12Vϵͳ��Դ���Ƶ�ѹ��Сֵ(Normal��ΪLow) */
#define DIAS_PM_12V_LOW_TO_NORMAL_BATY_POWER_CTRL  6500      /* 12Vϵͳ��Դ���Ƶ�ѹ��Сֵ(Low��ΪNormal) */
#define DIAS_PM_12V_NORMAL_TO_HIGH_BATY_POWER_CTRL 18000     /* 12Vϵͳ��Դ���Ƶ�ѹ���ֵ(Normal��ΪHigh) */
#define DIAS_PM_12V_HIGH_TO_NORMAL_BATY_POWER_CTRL 17500     /* 12Vϵͳ��Դ���Ƶ�ѹ���ֵ(High��ΪNormal) */

#define DIAS_PM_24V_NORMAL_TO_LOW_BATY_OPERA       17500     /* 24VϵͳA�����ܵ�ѹ��Сֵ(Normal��ΪLow) */
#define DIAS_PM_24V_LOW_TO_NORMAL_BATY_OPERA       18000     /* 24VϵͳA�����ܵ�ѹ���ֵ(Low��ΪNormal) */
#define DIAS_PM_24V_NORMAL_TO_HIGH_BATY_OPERA      32500     /* 24VϵͳA�����ܵ�ѹ���ֵ(Normal��ΪHigh) */
#define DIAS_PM_24V_HIGH_TO_NORMAL_BATY_OPERA      32000     /* 24VϵͳA�����ܵ�ѹ���ֵ(High��ΪNormal) */

#define DIAS_PM_24V_NORMAL_TO_LOW_BATY_OPERCANA    12000     /* 24VϵͳA��CAN�����ѹ��Сֵ(Normal��ΪLow) */
#define DIAS_PM_24V_LOW_TO_NORMAL_BATY_OPERCANA    12500     /* 24VϵͳA��CAN�����ѹ��Сֵ(Low��ΪNormal) */
#define DIAS_PM_24V_NORMAL_TO_HIGH_BATY_OPERCANA   36000     /* 24VϵͳA��CAN�����ѹ���ֵ(Normal��ΪHigh) */
#define DIAS_PM_24V_HIGH_TO_NORMAL_BATY_OPERCANA   35500     /* 24VϵͳA��CAN�����ѹ���ֵ(High��ΪNormal) */

#define DIAS_PM_24V_NORMAL_TO_LOW_BATY_DIAG        17500     /* 24Vϵͳ��ϵ�ѹ��Сֵ(Normal��ΪLow) */
#define DIAS_PM_24V_LOW_TO_NORMAL_BATY_DIAG        18000     /* 24Vϵͳ��ϵ�ѹ��Сֵ(Low��ΪNormal) */
#define DIAS_PM_24V_NORMAL_TO_HIGH_BATY_DIAG       32500     /* 24Vϵͳ��ϵ�ѹ���ֵ(Normal��ΪHigh) */
#define DIAS_PM_24V_HIGH_TO_NORMAL_BATY_DIAG       32000     /* 24Vϵͳ��ϵ�ѹ���ֵ(High��ΪNormal) */

#define DIAS_PM_24V_NORMAL_TO_LOW_BATY_POWER_CTRL  12000     /* 24Vϵͳ��Դ���Ƶ�ѹ��Сֵ(Normal��ΪLow) */
#define DIAS_PM_24V_LOW_TO_NORMAL_BATY_POWER_CTRL  12500     /* 24Vϵͳ��Դ���Ƶ�ѹ��Сֵ(Low��ΪNormal) */
#define DIAS_PM_24V_NORMAL_TO_HIGH_BATY_POWER_CTRL 36000     /* 24Vϵͳ��Դ���Ƶ�ѹ���ֵ(Normal��ΪHigh) */
#define DIAS_PM_24V_HIGH_TO_NORMAL_BATY_POWER_CTRL 35500     /* 24Vϵͳ��Դ���Ƶ�ѹ���ֵ(High��ΪNormal) */


#define DIAS_PM_MAX_KL30_VOL_IN_BAK_BAT        3000      /* ���繤�����KL30��ѹ */


/******************************************************************************
* ���Ͷ���
******************************************************************************/
typedef enum
{
    DIAS_PM_POWER_TYPE_12V = 0,
    DIAS_PM_POWER_TYPE_24V,
    DIAS_PM_POWER_TYPE_NUM
}DiasPmSysPowerType_e;

typedef struct
{
    uint16_t  u16NormalToLowVol;
    uint16_t  u16LowToNormalVol;
    uint16_t  u16NormalToHighVol;
    uint16_t  u16HighToNormalVol;
}DiasPmBatyVolConsCfg_t;


typedef struct 
{
    UINT8 u8DtcLowErr;
    UINT8 u8DtcHighErr;
}PmBatyVolDemCfg_t;

typedef struct
{
    BatyVolConsStatus_e          eCurrStatus;
    const DiasPmBatyVolConsCfg_t atBatyVolConsCfg[DIAS_PM_POWER_TYPE_NUM];
}DiasPmBatyVolConsInfo_t;


/******************************************************************************
* ��������
******************************************************************************/
static uint8_t  gu8PmSysPowerType = DIAS_PM_POWER_TYPE_12V;    /* ϵͳ��Դ���� */
static uint16_t gu16PmBatyVol = 0;                             /* ��ǰ��ѹֵ(mV) */
static BOOL     gbPmIsKl30Off = FALSE;                         /* KL30�Ƿ�Off */


static const PmBatyVolDemCfg_t gatPmBatyVolDtcInfo[INDEX_BATY_CONS_ALL] = 
{
    {0xFF,0xFF},
    {0xFF,0xFF},
    // {DTC_9002_16,DTC_9001_17},
    {0xFF,0xFF},
};


static DiasPmBatyVolConsInfo_t gatPmBatyVolConsInfo[INDEX_BATY_CONS_ALL] =
{
    /* A�����ܵ�ѹ */
    {
        STATUS_BATY_CONS_ST_INVALID,
        {
            {
                DIAS_PM_12V_NORMAL_TO_LOW_BATY_OPERA,
                DIAS_PM_12V_LOW_TO_NORMAL_BATY_OPERA,
                DIAS_PM_12V_NORMAL_TO_HIGH_BATY_OPERA,
                DIAS_PM_12V_HIGH_TO_NORMAL_BATY_OPERA
            },
            {
                DIAS_PM_24V_NORMAL_TO_LOW_BATY_OPERA,
                DIAS_PM_24V_LOW_TO_NORMAL_BATY_OPERA,
                DIAS_PM_24V_NORMAL_TO_HIGH_BATY_OPERA,
                DIAS_PM_24V_HIGH_TO_NORMAL_BATY_OPERA
            }
        }
    },
    /* A��CAN�����ѹ */
    {
        STATUS_BATY_CONS_ST_INVALID,
        {
            {
                DIAS_PM_12V_NORMAL_TO_LOW_BATY_OPERCANA,
                DIAS_PM_12V_LOW_TO_NORMAL_BATY_OPERCANA,
                DIAS_PM_12V_NORMAL_TO_HIGH_BATY_OPERCANA,
                DIAS_PM_12V_HIGH_TO_NORMAL_BATY_OPERCANA
            },
            {
                DIAS_PM_24V_NORMAL_TO_LOW_BATY_OPERCANA,
                DIAS_PM_24V_LOW_TO_NORMAL_BATY_OPERCANA,
                DIAS_PM_24V_NORMAL_TO_HIGH_BATY_OPERCANA,
                DIAS_PM_24V_HIGH_TO_NORMAL_BATY_OPERCANA
            }
        }
    },
    /* ��ϵ�ѹ */
    {
        STATUS_BATY_CONS_ST_INVALID,
        {
            {
                DIAS_PM_12V_NORMAL_TO_LOW_BATY_DIAG,
                DIAS_PM_12V_LOW_TO_NORMAL_BATY_DIAG,
                DIAS_PM_12V_NORMAL_TO_HIGH_BATY_DIAG,
                DIAS_PM_12V_HIGH_TO_NORMAL_BATY_DIAG
            },
            {
                DIAS_PM_24V_NORMAL_TO_LOW_BATY_DIAG,
                DIAS_PM_24V_LOW_TO_NORMAL_BATY_DIAG,
                DIAS_PM_24V_NORMAL_TO_HIGH_BATY_DIAG,
                DIAS_PM_24V_HIGH_TO_NORMAL_BATY_DIAG
            }
        }
    },
    /* ��Դ���Ƶ�ѹ */
    {
        STATUS_BATY_CONS_ST_INVALID,
        {
            {
                DIAS_PM_12V_NORMAL_TO_LOW_BATY_POWER_CTRL,
                DIAS_PM_12V_LOW_TO_NORMAL_BATY_POWER_CTRL,
                DIAS_PM_12V_NORMAL_TO_HIGH_BATY_POWER_CTRL,
                DIAS_PM_12V_HIGH_TO_NORMAL_BATY_POWER_CTRL
            },
            {
                DIAS_PM_24V_NORMAL_TO_LOW_BATY_POWER_CTRL,
                DIAS_PM_24V_LOW_TO_NORMAL_BATY_POWER_CTRL,
                DIAS_PM_24V_NORMAL_TO_HIGH_BATY_POWER_CTRL,
                DIAS_PM_24V_HIGH_TO_NORMAL_BATY_POWER_CTRL
            }
        }
    }
};

static void PmVolDemSetVolLowFail(BatyVolConsIndex_e eVolType)
{
    if(gatPmBatyVolDtcInfo[eVolType].u8DtcLowErr == 0xFF
    ||gatPmBatyVolDtcInfo[eVolType].u8DtcHighErr == 0xFF)
    {
        return;
    }
    // ApiFmReportError(gatPmBatyVolDtcInfo[eVolType].u8DtcLowErr);
}
static void PmVolDemSetVolHighFail(BatyVolConsIndex_e eVolType)
{
    if(gatPmBatyVolDtcInfo[eVolType].u8DtcLowErr == 0xFF
    ||gatPmBatyVolDtcInfo[eVolType].u8DtcHighErr == 0xFF)
    {
        return;
    }
    // ApiFmReportError(gatPmBatyVolDtcInfo[eVolType].u8DtcHighErr);
}

static void PmVolDemClearVolFail(BatyVolConsIndex_e eVolType)
{
    if(gatPmBatyVolDtcInfo[eVolType].u8DtcLowErr == 0xFF
    ||gatPmBatyVolDtcInfo[eVolType].u8DtcHighErr == 0xFF)
    {
        return;
    }
    // ApiFmClearError(gatPmBatyVolDtcInfo[eVolType].u8DtcLowErr);
    // ApiFmClearError(gatPmBatyVolDtcInfo[eVolType].u8DtcHighErr);
}


/******************************************************************************
* ��ѹ��Ϣ����
******************************************************************************/
void DiasPmRefreshBatyVolInfo(void);


/******************************************************************************
*  function name | DiasPmVolInit
*  content       | ��ʼ����ѹ��ػ���
*  parameter[1]  | void
*  notice        | ��
*  return        | int32_t
******************************************************************************/
int32_t DiasPmVolInit(void)
{
    uint8_t u8Loop = 0;
    // if (ApiNvramReadSyncInd(INDEX_NVID_0xE021, sizeof(gu8PmSysPowerType), (uint8_t *)&gu8PmSysPowerType) == FALSE)
    // {
    //     return ERR;
    // }

    if (gu8PmSysPowerType >= DIAS_PM_POWER_TYPE_NUM)
    {
        gu8PmSysPowerType = DIAS_PM_POWER_TYPE_12V;
    }

    for (; u8Loop < INDEX_BATY_CONS_ALL; ++u8Loop)
    {
        gatPmBatyVolConsInfo[u8Loop].eCurrStatus = STATUS_BATY_CONS_ST_INVALID;
    }

    return OK;
}

/******************************************************************************
*  function name | ApiPmBatVolGet
*  content       | ��ȡ��ص�ѹ
*  parameter[1]  | void
*  notice        | ��
*  return        | uint16_t :��ص�ѹ
******************************************************************************/
uint16_t ApiPmBatVolGet(void)
{
    return gu16PmBatyVol;
}

/******************************************************************************
*  function name | ApiPmBatyVolConsGet
*  content       | ��ȡ��ѹ����״̬
*  parameter[1]  | BatyVolConsIndex_e eIndex :��ѹ��������
*  notice        | ��
*  return        | BatyVolConsStatus_e :��ѹ����״̬
******************************************************************************/
BatyVolConsStatus_e ApiPmBatyVolConsGet(BatyVolConsIndex_e eIndex)
{
    if (eIndex >= INDEX_BATY_CONS_ALL)
    {
        return STATUS_BATY_CONS_ST_INVALID;
    }

    return gatPmBatyVolConsInfo[eIndex].eCurrStatus;
}

/******************************************************************************
*  function name | DiasPmRefreshBatyVolInfo
*  content       | ���µ�ѹ��Ϣ
*  parameter[1]  | void
*  notice        | ��
*  return        | void
******************************************************************************/
void DiasPmRefreshBatyVolInfo(void)
{
    uint8_t u8Loop = 0;
    int32_t s32MaxVolt = 0;
    int32_t s32MinVolt = 0;
    int32_t i32Volt = 0;//ApiAdcVoltRead(ADC_CHANNEL_6);

    if ((i32Volt < 0) || (i32Volt > DIAS_U16_INVALID_VALUE))
    {
        i32Volt = 0;
    }
    else if (i32Volt < DIAS_PM_MAX_KL30_VOL_IN_BAK_BAT)
    {
        gbPmIsKl30Off = TRUE;
    }
    else
    {
        gbPmIsKl30Off = FALSE;
    }

    gu16PmBatyVol = (uint16_t)(i32Volt / 100);   /* mV->0.1V */

	//ApiLogPrint(_LOG_TRACE,"AdcVoltRead: %d\n",gu16PmBatyVol);
	
    for (; u8Loop < INDEX_BATY_CONS_ALL; ++u8Loop)
    {
        if (i32Volt == 0)
        {
            gatPmBatyVolConsInfo[u8Loop].eCurrStatus = STATUS_BATY_CONS_ST_INVALID;
            continue;
        }

        /* ����֮ǰ��ѹ������ȡ��ѹ�����ж������� */
        switch (gatPmBatyVolConsInfo[u8Loop].eCurrStatus)
        {
            case STATUS_BATY_CONS_ST_LOW:
            {
                s32MinVolt = gatPmBatyVolConsInfo[u8Loop].atBatyVolConsCfg[gu8PmSysPowerType].u16LowToNormalVol;
                s32MaxVolt = gatPmBatyVolConsInfo[u8Loop].atBatyVolConsCfg[gu8PmSysPowerType].u16NormalToHighVol;
                break;
            }
            case STATUS_BATY_CONS_ST_HIGH:
            {
                s32MinVolt = gatPmBatyVolConsInfo[u8Loop].atBatyVolConsCfg[gu8PmSysPowerType].u16NormalToLowVol;
                s32MaxVolt = gatPmBatyVolConsInfo[u8Loop].atBatyVolConsCfg[gu8PmSysPowerType].u16HighToNormalVol;
                break;
            }
            case STATUS_BATY_CONS_ST_NORMAL:
            {
                s32MinVolt = gatPmBatyVolConsInfo[u8Loop].atBatyVolConsCfg[gu8PmSysPowerType].u16NormalToLowVol;
                s32MaxVolt = gatPmBatyVolConsInfo[u8Loop].atBatyVolConsCfg[gu8PmSysPowerType].u16NormalToHighVol;
                break;
            }
            default:
            {
                s32MinVolt = gatPmBatyVolConsInfo[u8Loop].atBatyVolConsCfg[gu8PmSysPowerType].u16LowToNormalVol;
                s32MaxVolt = gatPmBatyVolConsInfo[u8Loop].atBatyVolConsCfg[gu8PmSysPowerType].u16NormalToHighVol;
                break;
            }
        }

        if (i32Volt < s32MinVolt)
        {
            gatPmBatyVolConsInfo[u8Loop].eCurrStatus = STATUS_BATY_CONS_ST_LOW;
            PmVolDemSetVolLowFail(u8Loop);
        }
    	else if (i32Volt > s32MaxVolt)
    	{
    		gatPmBatyVolConsInfo[u8Loop].eCurrStatus = STATUS_BATY_CONS_ST_HIGH;
            PmVolDemSetVolHighFail(u8Loop);
    	}
        else
        {
            gatPmBatyVolConsInfo[u8Loop].eCurrStatus = STATUS_BATY_CONS_ST_NORMAL;
            PmVolDemClearVolFail(u8Loop);
        }
    }
}

/******************************************************************************
*  function name | DiasPmIsKl30Off
*  content       | �ж�KL30�Ƿ�OFF
*  parameter[1]  | void
*  notice        | ��
*  return        | BOOL��TRUE(KL30 Off)/FALSE(KL30 Not Off)
******************************************************************************/
BOOL DiasPmIsKl30Off(void)
{
    return gbPmIsKl30Off;
}


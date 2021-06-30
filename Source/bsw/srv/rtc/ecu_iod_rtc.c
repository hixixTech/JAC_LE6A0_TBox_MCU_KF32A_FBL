/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ecu_iod_rtc.c                                               */
/* Author:      ����    Version: 01.01    Date: 2019-11-01                     */
/* Description:  �м����RTC��Ԫ                                                   */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS                                                         */
/********************************** �޸���ʷ *********************************/
/* Date         Version          Author        Description                   */
/* 2019-09-24   V31.03.02.01.01  �ﻪ��           �½����߰汾                        */
/* 2020-09-24   V31.03.02.01.02  ����            ����JAC����������޸�                  */
/* 2021-01-27   V31.03.02.01.03  ����            ���Ӳ�ѯ��ǰRTCʱ���Ƿ���Ч���жϽӿ�                  */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* ͷ�ļ�                                                                       */
/*---------------------------------------------------------------------------*/
#include "ecu_iod_rtc.h"
#include "ecu_i2c.h"
#include "ecu_iod_rtc_pcf8563.h"
#include "string.h"
#include "time.h"

/*---------------------------------------------------------------------------*/
/*�궨��                                                                        */
/*---------------------------------------------------------------------------*/

/*-----��������HEX���Ƶ�ת��-----*/
#define BIN_TO_HEX_0B10000000            0X80U
#define BIN_TO_HEX_0B01110000            0X70U
#define BIN_TO_HEX_0B00001111            0X0FU
#define BIN_TO_HEX_0B11000000            0XC0U
#define BIN_TO_HEX_0B00110000            0X30U
#define BIN_TO_HEX_0B11111000            0XF8U
#define BIN_TO_HEX_0B00000111            0X07U
#define BIN_TO_HEX_0B01100000            0X60U
#define BIN_TO_HEX_0B00010000            0X10U
#define BIN_TO_HEX_0B11110000            0XF0U

/*-----MCU����ǰ����ʱ��д��RTCоƬ����غ궨��-----*/

#define A_SET_ALRM_LEN                   5U

#define SET_ALRM_MINUTE_ALARM_INDEX      0U
#define SET_ALRM_HOUR_ALARM_INDEX        1U
#define SET_ALRM_DAY_ALARM_INDEX         2U
#define SET_ALRM_WEEKDAY_ALARM_INDEX     3U
#define SET_ALRM_CONTROL_STATUS_2_INDEX  4U

/*-----MCU����ǰʱ��д��RTCоƬ����غ궨��-----*/

#define A_SET_TIMER_LEN                  3U

#define SET_TIMER_CONTROL_STATUS_2_INDEX 0U
#define SET_TIMER_TIMER_INDEX            1U
#define SET_TIMER_TIMER_CONTROL          2U

/*-----MCU����ǰʱ��д��RTCоƬ����غ궨��-----*/

#define A_SET_CURRENT_TIME_LEN           15U

#define SET_CONTROL_STATUS_1_INDEX       0U
#define SET_CONTROL_STATUS_2_INDEX       1U
#define SET_VL_SECONDS_INDEX             2U
#define SET_MINUTES_INDEX                3U
#define SET_HOURS_INDEX                  4U
#define SET_DAYS_INDEX                   5U
#define SET_WEEKDAYS_INDEX               6U
#define SET_CENTURY_MONTHS_INDEX         7U
#define SET_YEARS_INDEX                  8U
#define SET_MINUTE_ALARM_INDEX           9U
#define SET_HOUR_ALARM_INDEX             10U
#define SET_DAY_ALARM_INDEX              11U
#define SET_WEEKDAY_ALARM_INDEX          12U
#define SET_CLKOUT_CONTROL_INDEX         13U
#define SET_TIMER_CONTROL_INDEX          14U

/*-----MCU��RTCоƬ��ȡ����ʱ�����غ궨��-----*/

#define A_GET_CURRENT_TIME_LEN           7U

#define GET_SECONDS_INDEX                0U
#define GET_MINUTES_INDEX                1U
#define GET_HOURS_INDEX                  2U
#define GET_DAYS_INDEX                   3U
#define GET_WEEKDAYS_INDEX               4U
#define GET_MONTHS_INDEX                 5U
#define GET_YEARS_INDEX                  6U

/*---------------------------------------------------------------------------*/
/*��������                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*���Ͷ���                                                                       */
/*---------------------------------------------------------------------------*/

/*-----BCD����-----*/
/*-----Դ����Ϊ0D12��ת����0X12����u8TensPlace=0X01,u8UnitPlace=0X02-----*/
/*-----u8TensPlace+u8UnitPlace;�����ʮ���Ƶ�12                            -----*/
typedef struct
{
	UINT8 u8TensPlace; /*ʮλ*/
	UINT8 u8UnitPlace; /*��λ*/
}BCDCode_t;

/*-----RTC��������-----*/
typedef enum
{
    RTC_ALARM_TYPE_MINUTES = 0U,
    RTC_ALARM_TYPE_HOURS,
    RTC_ALARM_TYPE_DAYS,
    RTC_ALARM_TYPE_END
}RtcAlarmType_e;

/*---------------------------------------------------------------------------*/
/*ȫ�ֱ�������                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*�ֲ���������                                                                     */
/*---------------------------------------------------------------------------*/
static UINT8                     aSetAlarm[A_SET_ALRM_LEN];
static UINT8                     aSetTimer[A_SET_TIMER_LEN];
static UINT8                     aSetCurrentTime[A_SET_CURRENT_TIME_LEN]; /*MCUд�뵽RTCоƬ��ʱ��*/
static UINT8                     aGetCurrentTime[A_GET_CURRENT_TIME_LEN]; /*MCU��RTCоƬ��ȡ����ʱ��*/

/*-----��RTCоƬ��ȡ���ݵ���ʱ����-----*/
static RtcReg_VL_SECONDS_t       tRtcReg_VL_SECONDS_read;
static RtcReg_MINUTES_t          tRtcReg_MINUTES_read;
static RtcReg_HOURS_t            tRtcReg_HOURS_read;
static RtcReg_DAYS_t             tRtcReg_DAYS_read;
static RtcReg_WEEKDAYS_t         tRtcReg_WEEKDAYS_read;
static RtcReg_CENTURY_MONTHS_t   tRtcReg_CENTURY_MONTHS_read;
static RtcReg_YEARS_t            tRtcReg_YEARS_read;

static BCDCode_t                 tBCDCode_VL_SECONDS_read;
static BCDCode_t                 tBCDCode_MINUTES_read;
static BCDCode_t                 tBCDCode_HOURS_read;
static BCDCode_t                 tBCDCode_DAYS_read;
static BCDCode_t                 tBCDCode_WEEKDAYS_read;
static BCDCode_t                 tBCDCode_CENTURY_MONTHS_read;
static BCDCode_t                 tBCDCode_YEARS_read;

/*---------------------------------------------------------------------------*/
/*�ֲ���������                                                                     */
/*---------------------------------------------------------------------------*/

/*-----Dec��BCD�����ת��-----*/
static INT32  ApiRtcDectoBCD(BCDCode_t *pBCDCodeDst, UINT8 u8DecSrc);
static UINT8  ApiRtcBCDtoDec(BCDCode_t tBCDCode);

/*-----�Ĵ���ת����UINT8�ĺ���-----*/
static UINT8  ApiRtcReg_CONTROL_STATUS_1_to_UINT8(RtcReg_CONTROL_STATUS_1_t tRtcReg_CONTROL_STATUS_1_Src);
static UINT8  ApiRtcReg_CONTROL_STATUS_2_to_UINT8(RtcReg_CONTROL_STATUS_2_t tRtcReg_CONTROL_STATUS_2_Src);
static UINT8  ApiRtcReg_VL_SECONDS_to_UINT8(RtcReg_VL_SECONDS_t tRtcReg_VL_SECONDS_Src);
static UINT8  ApiRtcReg_MINUTES_to_UINT8(RtcReg_MINUTES_t tRtcReg_MINUTES_Src);
static UINT8  ApiRtcReg_HOURS_to_UINT8(RtcReg_HOURS_t tRtcReg_HOURS_Src);
static UINT8  ApiRtcReg_DAYS_to_UINT8(RtcReg_DAYS_t tRtcReg_DAYS_Src);
static UINT8  ApiRtcReg_WEEKDAYS_to_UINT8(RtcReg_WEEKDAYS_t tRtcReg_WEEKDAYS_Src);
static UINT8  ApiRtcReg_CENTURY_MONTHS_to_UINT8(RtcReg_CENTURY_MONTHS_t tRtcReg_CENTURY_MONTHS_Src);
static UINT8  ApiRtcReg_YEARS_to_UINT8(RtcReg_YEARS_t tRtcReg_YEARS_Src);
static UINT8  ApiRtcReg_MINUTE_ALARM_to_UINT8(RtcReg_MINUTE_ALARM_t tRtcReg_MINUTE_ALARM_Src);
static UINT8  ApiRtcReg_HOUR_ALARM_to_UINT8(RtcReg_HOUR_ALARM_t tRtcReg_HOUR_ALARM_Src);
static UINT8  ApiRtcReg_DAY_ALARM_to_UINT8(RtcReg_DAY_ALARM_t tRtcReg_DAY_ALARM_Src);
static UINT8  ApiRtcReg_TIMER_CONTROL_to_UINT8(RtcReg_TIMER_CONTROL_t tRtcReg_TIMER_CONTROL_Src);
static UINT8  ApiRtcReg_TIMER_to_UINT8(RtcReg_TIMER_t tRtcReg_TIMER_Src);

/*-----UINT8ת���ɼĴ����ĺ���-----*/
static INT32  ApiRtcReg_UINT8_to_VL_SECONDS(RtcReg_VL_SECONDS_t *pRtcReg_VL_SECONDS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_MINUTES(RtcReg_MINUTES_t *pRtcReg_MINUTES_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_HOURS(RtcReg_HOURS_t *pRtcReg_HOURS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_DAYS(RtcReg_DAYS_t *pRtcReg_DAYS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_WEEKDAYS(RtcReg_WEEKDAYS_t *pRtcReg_WEEKDAYS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_CENTURY_MONTHS(RtcReg_CENTURY_MONTHS_t *pRtcReg_CENTURY_MONTHS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_YEARS(RtcReg_YEARS_t *pRtcReg_YEARS_Dst , UINT8 u8DataSrc);

/*-----��д����-----*/
static INT32  ApiRtcWriteOneByte(RtcRegisterAdd_e eRtcRegisterAdd,UINT8 u8WriteByte);

/*----MCU��RTCоƬ��ʱ�佻������-----*/
static INT32  ApiRtcSetAlarmToRtcChip(void);
static INT32  ApiRtcSetTimerToRtcChip(void);
static INT32  ApiRtcSetCurrentTimeToRtcChip(void);
static INT32  ApiRtcGetCurrentTimeFromRtcChip(UINT8 aCurrentTime[] , UINT8 u8CurrentTimeLen);

/*---------------------------------------------------------------------------*/
/*�ֲ���������                                                                     */
/*---------------------------------------------------------------------------*/

/******************************************************************************
*  function name | ApiRtcDectoBCD
*  content       | Decת����BCD
*  parameter     | u8DecSrc:Dec����
*                | pBCDCodeDst:BCD����
*  return        | 
*  notice        | 
******************************************************************************/
static INT32 ApiRtcDectoBCD(BCDCode_t *pBCDCodeDst, UINT8 u8DecSrc)
{
    if(NULL == pBCDCodeDst)
    {
        return ERROR;
    }

    pBCDCodeDst->u8TensPlace = (UINT8)(u8DecSrc / 10);
    pBCDCodeDst->u8UnitPlace = (UINT8)(u8DecSrc % 10);

    return OK;
}
    
/******************************************************************************
*  function name | ApiRtcBCDtoDec
*  content       | BCDת����Dec
*  parameter     | tBCDCode:BCD����
*  return        | UINT8:Dec����
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcBCDtoDec(BCDCode_t tBCDCode)
{
    UINT8 u8DecRet    = 0;
    UINT8 u8TensPlace = 0;
    UINT8 u8UnitPlace = 0;    

    u8TensPlace = tBCDCode.u8TensPlace;
    u8UnitPlace = tBCDCode.u8UnitPlace;

    u8DecRet = (UINT8)(10*u8TensPlace + u8UnitPlace);

    return u8DecRet;
}

/******************************************************************************
*  function name | ApiRtcReg_CONTROL_STATUS_1_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_CONTROL_STATUS_1_to_UINT8(RtcReg_CONTROL_STATUS_1_t tRtcReg_CONTROL_STATUS_1_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_CONTROL_STATUS_1_Src.TEST1 << 7);
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_1_Src.none1 << 6) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_1_Src.STOP  << 5) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_1_Src.none2 << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_1_Src.TESTC << 3) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_1_Src.none3     ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_CONTROL_STATUS_2_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_CONTROL_STATUS_2_to_UINT8(RtcReg_CONTROL_STATUS_2_t tRtcReg_CONTROL_STATUS_2_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_CONTROL_STATUS_2_Src.none  << 7);
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_2_Src.TI_TP << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_2_Src.AF    << 3) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_2_Src.TF    << 2) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_2_Src.AIE   << 1) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CONTROL_STATUS_2_Src.TIE       ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_VL_SECONDS_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_VL_SECONDS_to_UINT8(RtcReg_VL_SECONDS_t tRtcReg_VL_SECONDS_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_VL_SECONDS_Src.VL           << 7);
    u8DataRet = (UINT8)((tRtcReg_VL_SECONDS_Src.SECONDS_TENS << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_VL_SECONDS_Src.SECONDS_UINT     ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_MINUTES_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_MINUTES_to_UINT8(RtcReg_MINUTES_t tRtcReg_MINUTES_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_MINUTES_Src.none           << 7);
    u8DataRet = (UINT8)((tRtcReg_MINUTES_Src.MINUTES_TENS   << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_MINUTES_Src.MINUTES_UINT       ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_HOURS_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_HOURS_to_UINT8(RtcReg_HOURS_t tRtcReg_HOURS_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_HOURS_Src.none       << 6);
    u8DataRet = (UINT8)((tRtcReg_HOURS_Src.HOURS_TENS << 5) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_HOURS_Src.HOURS_UINT     ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_DAYS_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_DAYS_to_UINT8(RtcReg_DAYS_t tRtcReg_DAYS_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_DAYS_Src.none      << 6);
    u8DataRet = (UINT8)((tRtcReg_DAYS_Src.DAYS_TENS << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_DAYS_Src.DAYS_UINT     ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_WEEKDAYS_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_WEEKDAYS_to_UINT8(RtcReg_WEEKDAYS_t tRtcReg_WEEKDAYS_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_WEEKDAYS_Src.none    << 3);
    u8DataRet = (UINT8)((tRtcReg_WEEKDAYS_Src.WEEKDAYS    ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_CENTURY_MONTHS_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_CENTURY_MONTHS_to_UINT8(RtcReg_CENTURY_MONTHS_t tRtcReg_CENTURY_MONTHS_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_CENTURY_MONTHS_Src.C           << 7);
    u8DataRet = (UINT8)((tRtcReg_CENTURY_MONTHS_Src.none        << 5) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CENTURY_MONTHS_Src.MONTHS_TENS << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_CENTURY_MONTHS_Src.MONTHS_UINT     ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_YEARS_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_YEARS_to_UINT8(RtcReg_YEARS_t tRtcReg_YEARS_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_YEARS_Src.YEARS_TENS    << 4);
    u8DataRet = (UINT8)((tRtcReg_YEARS_Src.YEARS_UINT        ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_MINUTE_ALARM_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_MINUTE_ALARM_to_UINT8(RtcReg_MINUTE_ALARM_t tRtcReg_MINUTE_ALARM_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_MINUTE_ALARM_Src.AE_M              << 7);
    u8DataRet = (UINT8)((tRtcReg_MINUTE_ALARM_Src.MINUTE_ALARM_TENS << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_MINUTE_ALARM_Src.MINUTE_ALARM_UNIT     ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_HOUR_ALARM_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_HOUR_ALARM_to_UINT8(RtcReg_HOUR_ALARM_t tRtcReg_HOUR_ALARM_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_HOUR_ALARM_Src.AE_H            << 7);
    u8DataRet = (UINT8)((tRtcReg_HOUR_ALARM_Src.none            << 6) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_HOUR_ALARM_Src.HOUR_ALARM_TENS << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_HOUR_ALARM_Src.HOUR_ALARM_UINT     ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_DAY_ALARM_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_DAY_ALARM_to_UINT8(RtcReg_DAY_ALARM_t tRtcReg_DAY_ALARM_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_DAY_ALARM_Src.AE_D           << 7);
    u8DataRet = (UINT8)((tRtcReg_DAY_ALARM_Src.none           << 6) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_DAY_ALARM_Src.DAY_ALARM_TENS << 4) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_DAY_ALARM_Src.DAY_ALARM_UINT     ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_TIMER_CONTROL_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_TIMER_CONTROL_to_UINT8(RtcReg_TIMER_CONTROL_t tRtcReg_TIMER_CONTROL_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)( tRtcReg_TIMER_CONTROL_Src.TE    << 7);
    u8DataRet = (UINT8)((tRtcReg_TIMER_CONTROL_Src.none  << 2) | u8DataRet );
    u8DataRet = (UINT8)((tRtcReg_TIMER_CONTROL_Src.TD        ) | u8DataRet );
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_TIMER_to_UINT8
*  content       | �Ĵ���ת����UINT8
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static UINT8 ApiRtcReg_TIMER_to_UINT8(RtcReg_TIMER_t tRtcReg_TIMER_Src)
{
    UINT8 u8DataRet = 0U;

    u8DataRet = (UINT8)(tRtcReg_TIMER_Src.TIMER);
    
    return u8DataRet;
}

/******************************************************************************
*  function name | ApiRtcReg_UINT8_to_VL_SECONDS
*  content       | UINT8ת���ɼĴ�������ֵ
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static INT32 ApiRtcReg_UINT8_to_VL_SECONDS(RtcReg_VL_SECONDS_t *pRtcReg_VL_SECONDS_Dst , UINT8 u8DataSrc)
{
    if(NULL == pRtcReg_VL_SECONDS_Dst)
    {
        return ERROR;
    }

    pRtcReg_VL_SECONDS_Dst->VL              = (UINT8)((u8DataSrc & BIN_TO_HEX_0B10000000) >> 7);
    pRtcReg_VL_SECONDS_Dst->SECONDS_TENS    = (UINT8)((u8DataSrc & BIN_TO_HEX_0B01110000) >> 4);
    pRtcReg_VL_SECONDS_Dst->SECONDS_UINT    = (UINT8)( u8DataSrc & BIN_TO_HEX_0B00001111      );

    return OK;
}

/******************************************************************************
*  function name | ApiRtcReg_UINT8_to_MINUTES
*  content       | UINT8ת���ɼĴ�������ֵ
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static INT32 ApiRtcReg_UINT8_to_MINUTES(RtcReg_MINUTES_t *pRtcReg_MINUTES_Dst , UINT8 u8DataSrc)
{
    if(NULL == pRtcReg_MINUTES_Dst)
    {
        return ERROR;
    }

    pRtcReg_MINUTES_Dst->none               = (UINT8)((u8DataSrc & BIN_TO_HEX_0B10000000) >> 7);
    pRtcReg_MINUTES_Dst->MINUTES_TENS       = (UINT8)((u8DataSrc & BIN_TO_HEX_0B01110000) >> 4);
    pRtcReg_MINUTES_Dst->MINUTES_UINT       = (UINT8)( u8DataSrc & BIN_TO_HEX_0B00001111      );

    return OK;
}

/******************************************************************************
*  function name | ApiRtcReg_UINT8_to_HOURS
*  content       | UINT8ת���ɼĴ�������ֵ
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static INT32 ApiRtcReg_UINT8_to_HOURS(RtcReg_HOURS_t *pRtcReg_HOURS_Dst , UINT8 u8DataSrc)
{
    if(NULL == pRtcReg_HOURS_Dst)
    {
        return ERROR;
    }

    pRtcReg_HOURS_Dst->none                 = (UINT8)((u8DataSrc & BIN_TO_HEX_0B11000000) >> 6);
    pRtcReg_HOURS_Dst->HOURS_TENS           = (UINT8)((u8DataSrc & BIN_TO_HEX_0B00110000) >> 4);
    pRtcReg_HOURS_Dst->HOURS_UINT           = (UINT8)( u8DataSrc & BIN_TO_HEX_0B00001111      );

    return OK;
}

/******************************************************************************
*  function name | ApiRtcReg_UINT8_to_DAYS
*  content       | UINT8ת���ɼĴ�������ֵ
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static INT32 ApiRtcReg_UINT8_to_DAYS(RtcReg_DAYS_t *pRtcReg_DAYS_Dst , UINT8 u8DataSrc)
{
    if(NULL == pRtcReg_DAYS_Dst)
    {
        return ERROR;
    }

    pRtcReg_DAYS_Dst->none                  = (UINT8)((u8DataSrc & BIN_TO_HEX_0B11000000) >> 6);
    pRtcReg_DAYS_Dst->DAYS_TENS             = (UINT8)((u8DataSrc & BIN_TO_HEX_0B00110000) >> 4);
    pRtcReg_DAYS_Dst->DAYS_UINT             = (UINT8)( u8DataSrc & BIN_TO_HEX_0B00001111      );
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcReg_UINT8_to_WEEKDAYS
*  content       | UINT8ת���ɼĴ�������ֵ
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static INT32 ApiRtcReg_UINT8_to_WEEKDAYS(RtcReg_WEEKDAYS_t *pRtcReg_WEEKDAYS_Dst , UINT8 u8DataSrc)
{
    if(NULL == pRtcReg_WEEKDAYS_Dst)
    {
        return ERROR;
    }

    pRtcReg_WEEKDAYS_Dst->none              = (UINT8)((u8DataSrc & BIN_TO_HEX_0B11111000) >> 3);
    pRtcReg_WEEKDAYS_Dst->WEEKDAYS          = (UINT8)( u8DataSrc & BIN_TO_HEX_0B00000111      );

    return OK;
}

/******************************************************************************
*  function name | ApiRtcReg_UINT8_to_CENTURY_MONTHS
*  content       | UINT8ת���ɼĴ�������ֵ
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static INT32 ApiRtcReg_UINT8_to_CENTURY_MONTHS(RtcReg_CENTURY_MONTHS_t *pRtcReg_CENTURY_MONTHS_Dst , UINT8 u8DataSrc)
{
    if(NULL == pRtcReg_CENTURY_MONTHS_Dst)
    {
        return ERROR;
    }
    
    pRtcReg_CENTURY_MONTHS_Dst->C           = (UINT8)((u8DataSrc & BIN_TO_HEX_0B10000000) >> 7);
    pRtcReg_CENTURY_MONTHS_Dst->none        = (UINT8)((u8DataSrc & BIN_TO_HEX_0B01100000) >> 5);
    pRtcReg_CENTURY_MONTHS_Dst->MONTHS_TENS = (UINT8)((u8DataSrc & BIN_TO_HEX_0B00010000) >> 4);
    pRtcReg_CENTURY_MONTHS_Dst->MONTHS_UINT = (UINT8)( u8DataSrc & BIN_TO_HEX_0B00001111      );

    return OK;
}

/******************************************************************************
*  function name | ApiRtcReg_UINT8_to_YEARS
*  content       | UINT8ת���ɼĴ�������ֵ
*  parameter     |
*  return        | 
*  notice        | 
******************************************************************************/
static INT32 ApiRtcReg_UINT8_to_YEARS(RtcReg_YEARS_t *pRtcReg_YEARS_Dst , UINT8 u8DataSrc)
{
    if(NULL == pRtcReg_YEARS_Dst)
    {
        return ERROR;
    }

    pRtcReg_YEARS_Dst->YEARS_TENS           = (UINT8)((u8DataSrc & BIN_TO_HEX_0B11110000) >> 4);
    pRtcReg_YEARS_Dst->YEARS_UINT           = (UINT8)( u8DataSrc & BIN_TO_HEX_0B00001111      );

    return OK;
}

/******************************************************************************
*  function name | ApiRtcWriteOneByte
*  content       | дһ���ֽ�
*  parameter     | eRegisterAdd:�Ĵ�����ַ
                 | u8WritenByte:д�������
*  return        | OK:���� ������������
*  notice        | ע��ʹ��ö�����ͣ���д���ɻ���
******************************************************************************/
static INT32 ApiRtcWriteOneByte(RtcRegisterAdd_e eRtcRegisterAdd,UINT8 u8WriteByte)
{
  
    if(RTC_REGADD_END == eRtcRegisterAdd)
    {
        return ERROR;
    }
    
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,eRtcRegisterAdd,u8WriteByte);
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetAlarmToRtcChip
*  content       | ����ǰ����ʱ��д��RTCоƬ
*  parameter     | 
*  return        | 
*  notice        | д��alarm�����ض���ʱ�򣬲ο���User Manual for NXP Real Time Clocks PCF85x3��
******************************************************************************/
static INT32 ApiRtcSetAlarmToRtcChip(void)
{

    /*-----�Ĵ�����ֵд������-----*/
    aSetAlarm[SET_ALRM_MINUTE_ALARM_INDEX]     = ApiRtcReg_MINUTE_ALARM_to_UINT8(tRtcReg_MINUTE_ALARM);
    aSetAlarm[SET_ALRM_HOUR_ALARM_INDEX]       = ApiRtcReg_HOUR_ALARM_to_UINT8(tRtcReg_HOUR_ALARM);
    aSetAlarm[SET_ALRM_DAY_ALARM_INDEX]        = ApiRtcReg_DAY_ALARM_to_UINT8(tRtcReg_DAY_ALARM);
    aSetAlarm[SET_ALRM_WEEKDAY_ALARM_INDEX]    = 0X80;   /*weekday alarm is disabled*/
    aSetAlarm[SET_ALRM_CONTROL_STATUS_2_INDEX] = 0X02;   /*clear alarm flag and enable alarm interrupt*/

    /*-----������д��RTCоƬ-----*/
    ApiI2cWriteNumByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_MINUTE_ALARM,aSetAlarm,4U);
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,RTC_REGADD_CONTROL_STATUS_2,aSetAlarm[SET_ALRM_CONTROL_STATUS_2_INDEX]);
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetCurrentTimeToRtcChip
*  content       | ����ǰʱ��д��RTCоƬ
*  parameter     | 
*  return        | 
*  notice        | д��timer�����ض���ʱ�򣬲ο���User Manual for NXP Real Time Clocks PCF85x3��
******************************************************************************/
static INT32 ApiRtcSetTimerToRtcChip(void)
{

    /*-----�Ĵ�����ֵд������-----*/
    aSetTimer[SET_TIMER_CONTROL_STATUS_2_INDEX] = 0X01;
    aSetTimer[SET_TIMER_TIMER_INDEX]            = ApiRtcReg_TIMER_to_UINT8(tRtcReg_TIMER);
    aSetTimer[SET_TIMER_TIMER_CONTROL]          = ApiRtcReg_TIMER_CONTROL_to_UINT8(tRtcReg_TIMER_CONTROL);
    
    /*-----������д��RTCоƬ-----*/
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_CONTROL_STATUS_2,aSetTimer[SET_TIMER_CONTROL_STATUS_2_INDEX]);
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_TIMER,aSetTimer[SET_TIMER_TIMER_INDEX]);
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_TIMER_CONTROL,aSetTimer[SET_TIMER_TIMER_CONTROL]);
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetCurrentTimeToRtcChip
*  content       | ����ǰʱ��д��RTCоƬ
*  parameter     | 
*  return        | 
*  notice        | д��time�����ض���ʱ�򣬲ο���User Manual for NXP Real Time Clocks PCF85x3��
******************************************************************************/
static INT32 ApiRtcSetCurrentTimeToRtcChip(void)
{

    memset(&aSetCurrentTime[0],0,A_SET_CURRENT_TIME_LEN);

    /*-----�Ĵ�����ֵת��������-----*/
    aSetCurrentTime[SET_CONTROL_STATUS_1_INDEX] = 0X00;
    aSetCurrentTime[SET_CONTROL_STATUS_2_INDEX] = 0X00;
    aSetCurrentTime[SET_VL_SECONDS_INDEX]       = ApiRtcReg_VL_SECONDS_to_UINT8(tRtcReg_VL_SECONDS);
    aSetCurrentTime[SET_MINUTES_INDEX]          = ApiRtcReg_MINUTES_to_UINT8(tRtcReg_MINUTES);
    aSetCurrentTime[SET_HOURS_INDEX]            = ApiRtcReg_HOURS_to_UINT8(tRtcReg_HOURS);
    aSetCurrentTime[SET_DAYS_INDEX]             = ApiRtcReg_DAYS_to_UINT8(tRtcReg_DAYS);
    aSetCurrentTime[SET_WEEKDAYS_INDEX]         = ApiRtcReg_WEEKDAYS_to_UINT8(tRtcReg_WEEKDAYS);
    aSetCurrentTime[SET_CENTURY_MONTHS_INDEX]   = ApiRtcReg_CENTURY_MONTHS_to_UINT8(tRtcReg_CENTURY_MONTHS);
    aSetCurrentTime[SET_YEARS_INDEX]            = ApiRtcReg_YEARS_to_UINT8(tRtcReg_YEARS);
    aSetCurrentTime[SET_MINUTE_ALARM_INDEX]     = 0X80;
    aSetCurrentTime[SET_HOUR_ALARM_INDEX]       = 0X80;
    aSetCurrentTime[SET_DAY_ALARM_INDEX]        = 0X80;
    aSetCurrentTime[SET_WEEKDAY_ALARM_INDEX]    = 0X80;
    aSetCurrentTime[SET_CLKOUT_CONTROL_INDEX]   = 0X80;
    aSetCurrentTime[SET_TIMER_CONTROL_INDEX]    = 0X00;

    /*-----������д��RTCоƬ-----*/
    ApiI2cWriteNumByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_CONTROL_STATUS_1,aSetCurrentTime,A_SET_CURRENT_TIME_LEN);

    return OK;
}

/******************************************************************************
*  function name | ApiRtcGetCurrentTimeFromRtcChip
*  content       | ֱ�Ӵ�RTCоƬ��ȡ��ǰʱ��
*  parameter     | 
*  return        | OK:���� ERROR������
*  notice        | ��ȡtime���ض���˳�򣬲ο���Product data sheet��
******************************************************************************/
static INT32 ApiRtcGetCurrentTimeFromRtcChip(UINT8 aCurrentTime[] , UINT8 u8CurrentTimeLen)
{

    if((NULL == aCurrentTime)||(u8CurrentTimeLen < A_GET_CURRENT_TIME_LEN))
    {
        return ERROR;
    }
    
    ApiI2cReadNumByte((UINT8)RTC_CHIPADD_READ,(UINT8)RTC_REGADD_VL_SECONDS,aCurrentTime,u8CurrentTimeLen);
    
    return OK;
}

/*---------------------------------------------------------------------------*/
/*ȫ�ֺ�������                                                                     */
/*---------------------------------------------------------------------------*/

/******************************************************************************
*  function name | ApiRtcInit
*  content       | RTC��ʼ��
*  parameter     |
*  return        | OK:���� ERROR������
*  notice        | 
******************************************************************************/
INT32 ApiRtcInit(void)
{
    ApiRtcPCF8563Init();

    /*-----��ʱ������ʼ��-----*/
    
    memset(&tRtcReg_VL_SECONDS_read      ,0,sizeof(RtcReg_VL_SECONDS_t)      );
    memset(&tRtcReg_MINUTES_read         ,0,sizeof(RtcReg_MINUTES_t)         );
    memset(&tRtcReg_HOURS_read           ,0,sizeof(RtcReg_HOURS_t)           );
    memset(&tRtcReg_DAYS_read            ,0,sizeof(RtcReg_DAYS_t)            );
    memset(&tRtcReg_WEEKDAYS_read        ,0,sizeof(RtcReg_WEEKDAYS_t)        );
    memset(&tRtcReg_CENTURY_MONTHS_read  ,0,sizeof(RtcReg_CENTURY_MONTHS_t)  );
    memset(&tRtcReg_YEARS_read           ,0,sizeof(RtcReg_YEARS_t)           );

    memset(&tBCDCode_VL_SECONDS_read     ,0,sizeof(BCDCode_t)                );
    memset(&tBCDCode_MINUTES_read        ,0,sizeof(BCDCode_t)                );
    memset(&tBCDCode_HOURS_read          ,0,sizeof(BCDCode_t)                );
    memset(&tBCDCode_DAYS_read           ,0,sizeof(BCDCode_t)                );
    memset(&tBCDCode_WEEKDAYS_read       ,0,sizeof(BCDCode_t)                );
    memset(&tBCDCode_CENTURY_MONTHS_read ,0,sizeof(BCDCode_t)                );
    memset(&tBCDCode_YEARS_read          ,0,sizeof(BCDCode_t)                );

    return OK;
}

/******************************************************************************
*  function name | ApiRtcRtcToUtc
*  content       | ��RTCʱ��ת����UTCʱ��
*  parameter     | 
*  return        | 
*  notice        | ��AUX����У�UTCʱ��ͳһ��ָ0ʱ����ʱ�䡣ֻ�д���CN��׺�ģ��ű�ʾ����ʱ�䣬��8ʱ����ʱ��
******************************************************************************/
UINT32 ApiRtcRtcToUtc(RtcTime_t *pRtcTime)
{
    struct tm tTm;    
    int64_t   i64Ret;

    /*-----������Ч���ж�-----*/
    if(pRtcTime == NULL)
    {
        return 0XFFFFFFFF;
    }

    /*-----����ת��-----*/
    tTm.tm_sec   = (int)(pRtcTime->u8Seconds    );
    tTm.tm_min   = (int)(pRtcTime->u8Minutes    );
    tTm.tm_hour  = (int)(pRtcTime->u8Hours      );
    tTm.tm_mday  = (int)(pRtcTime->u8Days       );
    tTm.tm_mon   = (int)(pRtcTime->u8Months -1  );
    tTm.tm_year  = (int)(pRtcTime->u8Years + 100);
    tTm.tm_wday  = (int)(pRtcTime->u8WeekDays   );
    tTm.tm_isdst = FALSE;

    i64Ret = (int64_t)mktime(&tTm);
    if(i64Ret == -1)
    {
        return 0xFFFFFFFF;
    }
    
    return (uint32_t)i64Ret;   
}

/******************************************************************************
*  function name | ApiRtcUtcToRtc
*  content       | ��UTCʱ��ת����RTCʱ��
*  parameter     | 
*  return        | 
*  notice        | ��AUX����У�UTCʱ��ͳһ��ָ0ʱ����ʱ�䡣ֻ�д���CN��׺�ģ��ű�ʾ����ʱ�䣬��8ʱ����ʱ��
******************************************************************************/
INT32 ApiRtcUtcToRtc(RtcTime_t *pRtcTime , UINT32 u32UtcTime)
{
    struct tm       *tTm;
    int64_t          i64Utc = 0;
    static struct tm st_TimeTemp;
        
    if((0 == u32UtcTime)||(NULL == pRtcTime))
    {
        return ERROR;
    }

    i64Utc = (int64_t)u32UtcTime;
    // tTm = gmtime((time_t *)&i64Utc,&st_TimeTemp);
    tTm = gmtime((time_t *)&i64Utc);

    pRtcTime->u8Seconds  = (UINT8)(tTm->tm_sec );
    pRtcTime->u8Minutes  = (UINT8)(tTm->tm_min );
    pRtcTime->u8Hours    = (UINT8)(tTm->tm_hour);
    pRtcTime->u8Days     = (UINT8)(tTm->tm_mday);
    pRtcTime->u8Months   = (UINT8)(tTm->tm_mon );
    pRtcTime->u8Months++;
    pRtcTime->u8Years    = (UINT8)(tTm->tm_year);
    pRtcTime->u8Years   -= 100;
    pRtcTime->u8WeekDays = (UINT8)(tTm->tm_wday);
    tTm->tm_isdst = FALSE;
    
    return OK;    
}

/******************************************************************************
*  function name | ApiRtcAlarmSwitch
*  content       | ��������
*  parameter     |
*  return        | OK:���� ERROR������
*  notice        | 
******************************************************************************/
INT32 ApiRtcAlarmSwitch(RtcSwitchAlarm_e eRtcSwitchAlarm)
{
    UINT8 u8RegConvertedData = 0U; /*�Ĵ���ת������������*/

    switch(eRtcSwitchAlarm)
    {
        case RTC_SWITCH_ALARM_ON:
            tRtcReg_MINUTE_ALARM.AE_M   = 0U;
            tRtcReg_HOUR_ALARM.AE_H     = 0U;
            tRtcReg_DAY_ALARM.AE_D      = 0U;

            tRtcReg_CONTROL_STATUS_2.AF = 0U; /*����жϱ�־λ*/
            break;
        case RTC_SWITCH_ALARM_OFF:
            tRtcReg_MINUTE_ALARM.AE_M   = 1U;
            tRtcReg_HOUR_ALARM.AE_H     = 1U;
            tRtcReg_DAY_ALARM.AE_D      = 1U;

            tRtcReg_CONTROL_STATUS_2.AF = 0U; /*����жϱ�־λ*/
            break;
        default :
            return ERROR;
            break;
    }

    /*-----���Ĵ�������ֵд��RTCоƬ-----*/

    u8RegConvertedData = ApiRtcReg_MINUTE_ALARM_to_UINT8(tRtcReg_MINUTE_ALARM);
    (void)ApiRtcWriteOneByte(RTC_REGADD_MINUTE_ALARM,u8RegConvertedData);
    
    u8RegConvertedData = ApiRtcReg_HOUR_ALARM_to_UINT8(tRtcReg_HOUR_ALARM);
    (void)ApiRtcWriteOneByte(RTC_REGADD_HOUR_ALARM,u8RegConvertedData);
    
    u8RegConvertedData = ApiRtcReg_DAY_ALARM_to_UINT8(tRtcReg_DAY_ALARM);
    (void)ApiRtcWriteOneByte(RTC_REGADD_DAY_ALARM,u8RegConvertedData);

    /*-----ͬʱ������ӵ��жϱ�־λAF-----*/
    u8RegConvertedData = ApiRtcReg_CONTROL_STATUS_2_to_UINT8(tRtcReg_CONTROL_STATUS_2);
    (void)ApiRtcWriteOneByte(RTC_REGADD_CONTROL_STATUS_2,u8RegConvertedData);
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcTimerSwitch
*  content       | ���ض�ʱ��
*  parameter     |
*  return        | OK:���� ERROR������
*  notice        | 
******************************************************************************/
INT32 ApiRtcTimerSwitch(RtcSwitchTimer_e eRtcSwitchTimer)
{
    UINT8 u8RegConvertedData = 0U; /*�Ĵ���ת������������*/

    switch(eRtcSwitchTimer)
    {
        case RTC_SWITCH_TIMER_ON:
            tRtcReg_TIMER_CONTROL.TE = 1U;
            break;
        case RTC_SWITCH_TIMER_OFF:
            tRtcReg_TIMER_CONTROL.TE = 0U;
            break;
        default :
            return ERROR;
            break;
    }

    /*-----���Ĵ�������ֵд��RTCоƬ-----*/
    u8RegConvertedData = ApiRtcReg_TIMER_CONTROL_to_UINT8(tRtcReg_TIMER_CONTROL);
    (void)ApiRtcWriteOneByte(RTC_REGADD_TIMER_CONTROL,u8RegConvertedData);

    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetAlarm
*  content       | ��������
*  parameter     | pRtcAlarm:����ʱ��
*  return        | OK:���� ERROR������
*  notice        | 
******************************************************************************/
INT32 ApiRtcSetAlarm(RtcTime_t *pRtcTimeAlarm)
{
    UINT16         u16RtcAlarmCheck = 0U;
    BCDCode_t      tBCDCodeMinute;
    BCDCode_t      tBCDCodeHour;
    BCDCode_t      tBCDCodeDay;
    RtcAlarmType_e eRtcAlarmType    = RTC_ALARM_TYPE_END;

    UINT32         u32AlarmUtcDst   = 0U;  /*������Ҫ���õ�ʱ�䣬UTC����*/
    UINT32         u32CurrentUtc    = 0U;  /*��ǰʱ�䣬UTC��ʽ*/
    RtcTime_t      tRtcTimeCurrent;        /*��ǰʱ�䣬RTC��ʽ*/
    UINT32         u32UtcDelta      = 0U;  /*������Ҫ���õ�ʱ���뵱ǰʱ���ʱ���*/

    if(NULL == pRtcTimeAlarm)
    {
        return ERROR;
    }

    u16RtcAlarmCheck = (UINT16)(pRtcTimeAlarm->u8Minutes + pRtcTimeAlarm->u8Hours + pRtcTimeAlarm->u8Days);
    if(0U == u16RtcAlarmCheck)
    {
        /*�������õ�ʱ�䣬������ȫΪ0*/
        return ERROR;
    }

    /*-----1 �������ӵļ��ʱ�����ж���ǰ���ӵ�����eRtcAlarmType��minutes��hours��days�е���һ��-----*/

    /*-----1.1 ��������ʱ���뵱ǰʵ��ʱ���ʱ��-----*/
    u32AlarmUtcDst = ApiRtcRtcToUtc(pRtcTimeAlarm);
    (void)ApiRtcGetCurrentTime(&tRtcTimeCurrent);
    u32CurrentUtc = ApiRtcRtcToUtc(&tRtcTimeCurrent);

    if(u32AlarmUtcDst <= u32CurrentUtc)
    {
        /*����ʱ����������*/
        return ERROR;
    }
    else
    {
        u32UtcDelta = (UINT32)(u32AlarmUtcDst - u32CurrentUtc);
    }

    /*-----2.2 ����ʱ������������͵��ж�-----*/
    if((u32UtcDelta >= 60U)&&(u32UtcDelta < 3600U))
    {
        eRtcAlarmType = RTC_ALARM_TYPE_MINUTES;
    }
    else if((u32UtcDelta >= 3600U)&&(u32UtcDelta < (3600*24U)))
    {
        eRtcAlarmType = RTC_ALARM_TYPE_HOURS;
    }
    else if((u32UtcDelta >= (3600*24U))&&(u32UtcDelta < (3600*24*28U)))
    {
        eRtcAlarmType = RTC_ALARM_TYPE_DAYS;
    }
    else
    {
        eRtcAlarmType = RTC_ALARM_TYPE_END;

        return ERROR;/*����ʱ�䳬����Χ*/
    }

    /*-----2 ��������ֵд��Ĵ���-----*/

    /*-----2.1 ���������ж�-----*/
    switch(eRtcAlarmType)
    {
        case RTC_ALARM_TYPE_MINUTES:
            tRtcReg_MINUTE_ALARM.AE_M  = 0U; /*enabled*/
            tRtcReg_HOUR_ALARM.AE_H    = 1U; 
            tRtcReg_DAY_ALARM.AE_D     = 1U; 
            break;
        case RTC_ALARM_TYPE_HOURS:
            tRtcReg_MINUTE_ALARM.AE_M  = 1U; 
            tRtcReg_HOUR_ALARM.AE_H    = 0U; /*enabled*/
            tRtcReg_DAY_ALARM.AE_D     = 1U; 
            break;
        case RTC_ALARM_TYPE_DAYS:
            tRtcReg_MINUTE_ALARM.AE_M  = 1U; 
            tRtcReg_HOUR_ALARM.AE_H    = 1U; 
            tRtcReg_DAY_ALARM.AE_D     = 0U; /*enabled*/
            break;
        default :
            /*��������뵽����*/
            break;
    }
    
    /*-----2.2 ��������ֵд��Ĵ���-----*/
    
    (void)ApiRtcDectoBCD(&tBCDCodeMinute , pRtcTimeAlarm->u8Minutes);
    
    tRtcReg_MINUTE_ALARM.MINUTE_ALARM_TENS = tBCDCodeMinute.u8TensPlace;
    tRtcReg_MINUTE_ALARM.MINUTE_ALARM_UNIT = tBCDCodeMinute.u8UnitPlace;

    (void)ApiRtcDectoBCD(&tBCDCodeHour , pRtcTimeAlarm->u8Hours);

    tRtcReg_HOUR_ALARM.HOUR_ALARM_TENS     = tBCDCodeHour.u8TensPlace;
    tRtcReg_HOUR_ALARM.HOUR_ALARM_UINT     = tBCDCodeHour.u8UnitPlace;

    (void)ApiRtcDectoBCD(&tBCDCodeDay , pRtcTimeAlarm->u8Days);

    tRtcReg_DAY_ALARM.DAY_ALARM_TENS       = tBCDCodeDay.u8TensPlace;
    tRtcReg_DAY_ALARM.DAY_ALARM_UINT       = tBCDCodeDay.u8UnitPlace;

    /*-----3 ���Ĵ�������ֵд��RTCоƬ-----*/
    (void)ApiRtcSetAlarmToRtcChip();
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetTimer
*  content       | ���ö�ʱ��
*  parameter     | u8TimerSeconds:��ʱ����ʱʱ��
*  return        | OK:���� ERROR������
*  notice        | u8TimerMinutes����Ϊ0
******************************************************************************/
INT32 ApiRtcSetTimer(UINT8 u8TimerSeconds)
{
    if(0 == u8TimerSeconds)
    {
        /*����������Ϊ0*/
        return ERROR;
    }

    tRtcReg_TIMER_CONTROL.TE = 1U;    /*enabled*/
    tRtcReg_TIMER_CONTROL.TD = 0X02;  /*��0B10��Ƶ������Ϊ1Hz������Ϊ1s*/
    tRtcReg_TIMER.TIMER      = u8TimerSeconds;

    /*-----���Ĵ�������ֵд��RTCоƬ----*/
    (void)ApiRtcSetTimerToRtcChip();   

    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetCurrentTime
*  content       | ���õ�ǰRTC��ʱ��
*  parameter     | pRtcTime:�趨��ʱ��
*  return        | OK:���� ERROR������
*  notice        | 
******************************************************************************/
INT32 ApiRtcSetCurrentTime(RtcTime_t *pRtcTime)
{
    BCDCode_t tBCDCodeVlSeconds;
    BCDCode_t tBCDCodeMinutes;
    BCDCode_t tBCDCodeHours;
    BCDCode_t tBCDCodeDays;
    BCDCode_t tBCDCodeCenturyMonths;
    BCDCode_t tBCDCodeYears;

    if(NULL == pRtcTime)
    {
        return ERROR;
    }

    /*-----DECת����BCD�����ʽ-----*/
    
    /*-----seconds-----*/
    
    (void)ApiRtcDectoBCD(&tBCDCodeVlSeconds , pRtcTime->u8Seconds);
    
    tRtcReg_VL_SECONDS.SECONDS_TENS = tBCDCodeVlSeconds.u8TensPlace;
    tRtcReg_VL_SECONDS.SECONDS_UINT = tBCDCodeVlSeconds.u8UnitPlace;
    
    /*-----minutes-----*/

    (void)ApiRtcDectoBCD(&tBCDCodeMinutes , pRtcTime->u8Minutes);

    tRtcReg_MINUTES.MINUTES_TENS       = tBCDCodeMinutes.u8TensPlace;
    tRtcReg_MINUTES.MINUTES_UINT       = tBCDCodeMinutes.u8UnitPlace;
    
    /*-----hours-----*/

    (void)ApiRtcDectoBCD(&tBCDCodeHours , pRtcTime->u8Hours);

    tRtcReg_HOURS.HOURS_TENS           = tBCDCodeHours.u8TensPlace;
    tRtcReg_HOURS.HOURS_UINT           = tBCDCodeHours.u8UnitPlace;
    
    /*-----days-----*/

    (void)ApiRtcDectoBCD(&tBCDCodeDays , pRtcTime->u8Days);

    tRtcReg_DAYS.DAYS_TENS             = tBCDCodeDays.u8TensPlace;
    tRtcReg_DAYS.DAYS_UINT             = tBCDCodeDays.u8UnitPlace;

    tRtcReg_WEEKDAYS.WEEKDAYS          = pRtcTime->u8WeekDays;
    
    /*-----months-----*/

    (void)ApiRtcDectoBCD(&tBCDCodeCenturyMonths , pRtcTime->u8Months);

    tRtcReg_CENTURY_MONTHS.MONTHS_TENS = tBCDCodeCenturyMonths.u8TensPlace;
    tRtcReg_CENTURY_MONTHS.MONTHS_UINT = tBCDCodeCenturyMonths.u8UnitPlace;
    
    /*-----years-----*/

    (void)ApiRtcDectoBCD(&tBCDCodeYears , pRtcTime->u8Years);

    tRtcReg_YEARS.YEARS_TENS           = tBCDCodeYears.u8TensPlace;
    tRtcReg_YEARS.YEARS_UINT           = tBCDCodeYears.u8UnitPlace;

    /*-----���Ĵ�������ֵд��RTCоƬ----*/
    (void)ApiRtcSetCurrentTimeToRtcChip();

    return OK;    
}

/******************************************************************************
*  function name | ApiRtcGetCurrentTime
*  content       | ��ѯ��ǰRTC��ʱ��
*  parameter     | pRtcTime:��ѯ����ʱ��
*  return        | OK:���� ERROR������
*  notice        | 
******************************************************************************/
INT32 ApiRtcGetCurrentTime(RtcTime_t *pRtcTime)
{
    if(NULL == pRtcTime)
    {
        return ERROR;
    }

    memset(&aGetCurrentTime[0],0,A_GET_CURRENT_TIME_LEN);

    (void)ApiRtcGetCurrentTimeFromRtcChip(&aGetCurrentTime[0] , A_GET_CURRENT_TIME_LEN);

    /*-----register valueת����register �ṹ��-----*/
    (void)ApiRtcReg_UINT8_to_VL_SECONDS(      &tRtcReg_VL_SECONDS_read     , aGetCurrentTime[GET_SECONDS_INDEX] );
    (void)ApiRtcReg_UINT8_to_MINUTES(         &tRtcReg_MINUTES_read        , aGetCurrentTime[GET_MINUTES_INDEX] );
    (void)ApiRtcReg_UINT8_to_HOURS(           &tRtcReg_HOURS_read          , aGetCurrentTime[GET_HOURS_INDEX]   );
    (void)ApiRtcReg_UINT8_to_DAYS(            &tRtcReg_DAYS_read           , aGetCurrentTime[GET_DAYS_INDEX]    );
    (void)ApiRtcReg_UINT8_to_WEEKDAYS(        &tRtcReg_WEEKDAYS_read       , aGetCurrentTime[GET_WEEKDAYS_INDEX]);
    (void)ApiRtcReg_UINT8_to_CENTURY_MONTHS(  &tRtcReg_CENTURY_MONTHS_read , aGetCurrentTime[GET_MONTHS_INDEX]  );
    (void)ApiRtcReg_UINT8_to_YEARS(           &tRtcReg_YEARS_read          , aGetCurrentTime[GET_YEARS_INDEX]   );

    /*-----register �ṹ��ת����BCD����-----*/
    
    /*-----seconds-----*/
    tBCDCode_VL_SECONDS_read.u8TensPlace     = tRtcReg_VL_SECONDS_read.SECONDS_TENS;
    tBCDCode_VL_SECONDS_read.u8UnitPlace     = tRtcReg_VL_SECONDS_read.SECONDS_UINT;
    
    /*-----minutes-----*/
    tBCDCode_MINUTES_read.u8TensPlace        = tRtcReg_MINUTES_read.MINUTES_TENS;
    tBCDCode_MINUTES_read.u8UnitPlace        = tRtcReg_MINUTES_read.MINUTES_UINT;
    
    /*-----hours-----*/
    tBCDCode_HOURS_read.u8TensPlace          = tRtcReg_HOURS_read.HOURS_TENS;
    tBCDCode_HOURS_read.u8UnitPlace          = tRtcReg_HOURS_read.HOURS_UINT;
    
    /*-----days-----*/
    tBCDCode_DAYS_read.u8TensPlace           = tRtcReg_DAYS_read.DAYS_TENS;
    tBCDCode_DAYS_read.u8UnitPlace           = tRtcReg_DAYS_read.DAYS_UINT;
    
    tBCDCode_WEEKDAYS_read.u8UnitPlace       = tRtcReg_WEEKDAYS_read.WEEKDAYS;
    
    /*-----months-----*/
    tBCDCode_CENTURY_MONTHS_read.u8TensPlace = tRtcReg_CENTURY_MONTHS_read.MONTHS_TENS;
    tBCDCode_CENTURY_MONTHS_read.u8UnitPlace = tRtcReg_CENTURY_MONTHS_read.MONTHS_UINT;
    
    /*-----years-----*/
    tBCDCode_YEARS_read.u8TensPlace          = tRtcReg_YEARS_read.YEARS_TENS;
    tBCDCode_YEARS_read.u8UnitPlace          = tRtcReg_YEARS_read.YEARS_UINT;

    /*-----BCD����ת����DEC����-----*/
    pRtcTime->u8Seconds  = ApiRtcBCDtoDec(tBCDCode_VL_SECONDS_read);
    pRtcTime->u8Minutes  = ApiRtcBCDtoDec(tBCDCode_MINUTES_read);
    pRtcTime->u8Hours    = ApiRtcBCDtoDec(tBCDCode_HOURS_read);
    pRtcTime->u8Days     = ApiRtcBCDtoDec(tBCDCode_DAYS_read);
    pRtcTime->u8WeekDays = ApiRtcBCDtoDec(tBCDCode_WEEKDAYS_read);
    pRtcTime->u8Months   = ApiRtcBCDtoDec(tBCDCode_CENTURY_MONTHS_read);
    pRtcTime->u8Years    = ApiRtcBCDtoDec(tBCDCode_YEARS_read);

    return OK;
}

/******************************************************************************
*  function name | ApiRtcTimeValidityCheck
*  content       | ��ѯ��ǰRTC��ʱ���Ƿ���Ч
*  parameter     | pRtcTime:��ѯ��ʱ��
*  return        | TRUE:��Ч;FALSE:��Ч
*  notice        | 
******************************************************************************/
BOOL ApiRtcTimeValidityCheck(RtcTime_t *pRtcTime)
{
    BOOL bRet = FALSE;
    
    if(NULL == pRtcTime)
    {
        return ERROR;
    }

    if((pRtcTime->u8Years    <= 20 )||\
       (pRtcTime->u8Years    >= 30 )||\
       (pRtcTime->u8Months   == 0  )||\
       (pRtcTime->u8Months   >= 13 )||\
       (pRtcTime->u8Days     == 0  )||\
       (pRtcTime->u8Days     >= 32 )||\
       (pRtcTime->u8Hours    >= 25 )||\
       (pRtcTime->u8Minutes  >= 59 )||\
       (pRtcTime->u8WeekDays >= 7  ))
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }

    return bRet; 
}

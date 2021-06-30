/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ecu_iod_rtc.c                                               */
/* Author:      付恒    Version: 01.01    Date: 2019-11-01                     */
/* Description:  中间件的RTC单元                                                   */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS                                                         */
/********************************** 修改历史 *********************************/
/* Date         Version          Author        Description                   */
/* 2019-09-24   V31.03.02.01.01  孙华骏           新建基线版本                        */
/* 2020-09-24   V31.03.02.01.02  付恒            根据JAC的需求进行修改                  */
/* 2021-01-27   V31.03.02.01.03  付恒            增加查询当前RTC时间是否有效的判断接口                  */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* 头文件                                                                       */
/*---------------------------------------------------------------------------*/
#include "ecu_iod_rtc.h"
#include "ecu_i2c.h"
#include "ecu_iod_rtc_pcf8563.h"
#include "string.h"
#include "time.h"

/*---------------------------------------------------------------------------*/
/*宏定义                                                                        */
/*---------------------------------------------------------------------------*/

/*-----二进制与HEX进制的转换-----*/
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

/*-----MCU将当前闹钟时间写入RTC芯片的相关宏定义-----*/

#define A_SET_ALRM_LEN                   5U

#define SET_ALRM_MINUTE_ALARM_INDEX      0U
#define SET_ALRM_HOUR_ALARM_INDEX        1U
#define SET_ALRM_DAY_ALARM_INDEX         2U
#define SET_ALRM_WEEKDAY_ALARM_INDEX     3U
#define SET_ALRM_CONTROL_STATUS_2_INDEX  4U

/*-----MCU将当前时间写入RTC芯片的相关宏定义-----*/

#define A_SET_TIMER_LEN                  3U

#define SET_TIMER_CONTROL_STATUS_2_INDEX 0U
#define SET_TIMER_TIMER_INDEX            1U
#define SET_TIMER_TIMER_CONTROL          2U

/*-----MCU将当前时间写入RTC芯片的相关宏定义-----*/

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

/*-----MCU从RTC芯片读取到的时间的相关宏定义-----*/

#define A_GET_CURRENT_TIME_LEN           7U

#define GET_SECONDS_INDEX                0U
#define GET_MINUTES_INDEX                1U
#define GET_HOURS_INDEX                  2U
#define GET_DAYS_INDEX                   3U
#define GET_WEEKDAYS_INDEX               4U
#define GET_MONTHS_INDEX                 5U
#define GET_YEARS_INDEX                  6U

/*---------------------------------------------------------------------------*/
/*常量定义                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*类型定义                                                                       */
/*---------------------------------------------------------------------------*/

/*-----BCD编码-----*/
/*-----源数据为0D12，转换成0X12，即u8TensPlace=0X01,u8UnitPlace=0X02-----*/
/*-----u8TensPlace+u8UnitPlace;表意成十进制的12                            -----*/
typedef struct
{
	UINT8 u8TensPlace; /*十位*/
	UINT8 u8UnitPlace; /*个位*/
}BCDCode_t;

/*-----RTC闹钟类型-----*/
typedef enum
{
    RTC_ALARM_TYPE_MINUTES = 0U,
    RTC_ALARM_TYPE_HOURS,
    RTC_ALARM_TYPE_DAYS,
    RTC_ALARM_TYPE_END
}RtcAlarmType_e;

/*---------------------------------------------------------------------------*/
/*全局变量定义                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*局部变量定义                                                                     */
/*---------------------------------------------------------------------------*/
static UINT8                     aSetAlarm[A_SET_ALRM_LEN];
static UINT8                     aSetTimer[A_SET_TIMER_LEN];
static UINT8                     aSetCurrentTime[A_SET_CURRENT_TIME_LEN]; /*MCU写入到RTC芯片的时间*/
static UINT8                     aGetCurrentTime[A_GET_CURRENT_TIME_LEN]; /*MCU从RTC芯片读取到的时间*/

/*-----从RTC芯片读取数据的临时变量-----*/
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
/*局部函数声明                                                                     */
/*---------------------------------------------------------------------------*/

/*-----Dec与BCD编码的转换-----*/
static INT32  ApiRtcDectoBCD(BCDCode_t *pBCDCodeDst, UINT8 u8DecSrc);
static UINT8  ApiRtcBCDtoDec(BCDCode_t tBCDCode);

/*-----寄存器转换成UINT8的函数-----*/
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

/*-----UINT8转换成寄存器的函数-----*/
static INT32  ApiRtcReg_UINT8_to_VL_SECONDS(RtcReg_VL_SECONDS_t *pRtcReg_VL_SECONDS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_MINUTES(RtcReg_MINUTES_t *pRtcReg_MINUTES_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_HOURS(RtcReg_HOURS_t *pRtcReg_HOURS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_DAYS(RtcReg_DAYS_t *pRtcReg_DAYS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_WEEKDAYS(RtcReg_WEEKDAYS_t *pRtcReg_WEEKDAYS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_CENTURY_MONTHS(RtcReg_CENTURY_MONTHS_t *pRtcReg_CENTURY_MONTHS_Dst , UINT8 u8DataSrc);
static INT32  ApiRtcReg_UINT8_to_YEARS(RtcReg_YEARS_t *pRtcReg_YEARS_Dst , UINT8 u8DataSrc);

/*-----读写函数-----*/
static INT32  ApiRtcWriteOneByte(RtcRegisterAdd_e eRtcRegisterAdd,UINT8 u8WriteByte);

/*----MCU与RTC芯片的时间交互函数-----*/
static INT32  ApiRtcSetAlarmToRtcChip(void);
static INT32  ApiRtcSetTimerToRtcChip(void);
static INT32  ApiRtcSetCurrentTimeToRtcChip(void);
static INT32  ApiRtcGetCurrentTimeFromRtcChip(UINT8 aCurrentTime[] , UINT8 u8CurrentTimeLen);

/*---------------------------------------------------------------------------*/
/*局部函数定义                                                                     */
/*---------------------------------------------------------------------------*/

/******************************************************************************
*  function name | ApiRtcDectoBCD
*  content       | Dec转换成BCD
*  parameter     | u8DecSrc:Dec编码
*                | pBCDCodeDst:BCD编码
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
*  content       | BCD转换成Dec
*  parameter     | tBCDCode:BCD编码
*  return        | UINT8:Dec编码
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | 寄存器转换成UINT8
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
*  content       | UINT8转换成寄存器的数值
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
*  content       | UINT8转换成寄存器的数值
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
*  content       | UINT8转换成寄存器的数值
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
*  content       | UINT8转换成寄存器的数值
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
*  content       | UINT8转换成寄存器的数值
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
*  content       | UINT8转换成寄存器的数值
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
*  content       | UINT8转换成寄存器的数值
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
*  content       | 写一个字节
*  parameter     | eRegisterAdd:寄存器地址
                 | u8WritenByte:写入的数据
*  return        | OK:正常 其他：错误码
*  notice        | 注意使用枚举类型，读写不可混用
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
*  content       | 将当前闹钟时间写入RTC芯片
*  parameter     | 
*  return        | 
*  notice        | 写入alarm具有特定的时序，参考《User Manual for NXP Real Time Clocks PCF85x3》
******************************************************************************/
static INT32 ApiRtcSetAlarmToRtcChip(void)
{

    /*-----寄存器数值写入数组-----*/
    aSetAlarm[SET_ALRM_MINUTE_ALARM_INDEX]     = ApiRtcReg_MINUTE_ALARM_to_UINT8(tRtcReg_MINUTE_ALARM);
    aSetAlarm[SET_ALRM_HOUR_ALARM_INDEX]       = ApiRtcReg_HOUR_ALARM_to_UINT8(tRtcReg_HOUR_ALARM);
    aSetAlarm[SET_ALRM_DAY_ALARM_INDEX]        = ApiRtcReg_DAY_ALARM_to_UINT8(tRtcReg_DAY_ALARM);
    aSetAlarm[SET_ALRM_WEEKDAY_ALARM_INDEX]    = 0X80;   /*weekday alarm is disabled*/
    aSetAlarm[SET_ALRM_CONTROL_STATUS_2_INDEX] = 0X02;   /*clear alarm flag and enable alarm interrupt*/

    /*-----将数据写入RTC芯片-----*/
    ApiI2cWriteNumByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_MINUTE_ALARM,aSetAlarm,4U);
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,RTC_REGADD_CONTROL_STATUS_2,aSetAlarm[SET_ALRM_CONTROL_STATUS_2_INDEX]);
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetCurrentTimeToRtcChip
*  content       | 将当前时间写入RTC芯片
*  parameter     | 
*  return        | 
*  notice        | 写入timer具有特定的时序，参考《User Manual for NXP Real Time Clocks PCF85x3》
******************************************************************************/
static INT32 ApiRtcSetTimerToRtcChip(void)
{

    /*-----寄存器数值写入数组-----*/
    aSetTimer[SET_TIMER_CONTROL_STATUS_2_INDEX] = 0X01;
    aSetTimer[SET_TIMER_TIMER_INDEX]            = ApiRtcReg_TIMER_to_UINT8(tRtcReg_TIMER);
    aSetTimer[SET_TIMER_TIMER_CONTROL]          = ApiRtcReg_TIMER_CONTROL_to_UINT8(tRtcReg_TIMER_CONTROL);
    
    /*-----将数据写入RTC芯片-----*/
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_CONTROL_STATUS_2,aSetTimer[SET_TIMER_CONTROL_STATUS_2_INDEX]);
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_TIMER,aSetTimer[SET_TIMER_TIMER_INDEX]);
    ApiI2cWriteByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_TIMER_CONTROL,aSetTimer[SET_TIMER_TIMER_CONTROL]);
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetCurrentTimeToRtcChip
*  content       | 将当前时间写入RTC芯片
*  parameter     | 
*  return        | 
*  notice        | 写入time具有特定的时序，参考《User Manual for NXP Real Time Clocks PCF85x3》
******************************************************************************/
static INT32 ApiRtcSetCurrentTimeToRtcChip(void)
{

    memset(&aSetCurrentTime[0],0,A_SET_CURRENT_TIME_LEN);

    /*-----寄存器数值转换成数组-----*/
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

    /*-----将数据写入RTC芯片-----*/
    ApiI2cWriteNumByte((UINT8)RTC_CHIPADD_WRITE,(UINT8)RTC_REGADD_CONTROL_STATUS_1,aSetCurrentTime,A_SET_CURRENT_TIME_LEN);

    return OK;
}

/******************************************************************************
*  function name | ApiRtcGetCurrentTimeFromRtcChip
*  content       | 直接从RTC芯片读取当前时间
*  parameter     | 
*  return        | OK:正常 ERROR：错误
*  notice        | 读取time有特定的顺序，参考《Product data sheet》
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
/*全局函数定义                                                                     */
/*---------------------------------------------------------------------------*/

/******************************************************************************
*  function name | ApiRtcInit
*  content       | RTC初始化
*  parameter     |
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
INT32 ApiRtcInit(void)
{
    ApiRtcPCF8563Init();

    /*-----临时变量初始化-----*/
    
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
*  content       | 将RTC时间转换成UTC时间
*  parameter     | 
*  return        | 
*  notice        | 在AUX组件中，UTC时间统一代指0时区的时间。只有带有CN后缀的，才表示北京时间，即8时区的时间
******************************************************************************/
UINT32 ApiRtcRtcToUtc(RtcTime_t *pRtcTime)
{
    struct tm tTm;    
    int64_t   i64Ret;

    /*-----数据有效性判断-----*/
    if(pRtcTime == NULL)
    {
        return 0XFFFFFFFF;
    }

    /*-----数据转换-----*/
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
*  content       | 将UTC时间转换成RTC时间
*  parameter     | 
*  return        | 
*  notice        | 在AUX组件中，UTC时间统一代指0时区的时间。只有带有CN后缀的，才表示北京时间，即8时区的时间
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
*  content       | 开关闹钟
*  parameter     |
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
INT32 ApiRtcAlarmSwitch(RtcSwitchAlarm_e eRtcSwitchAlarm)
{
    UINT8 u8RegConvertedData = 0U; /*寄存器转换过来的数据*/

    switch(eRtcSwitchAlarm)
    {
        case RTC_SWITCH_ALARM_ON:
            tRtcReg_MINUTE_ALARM.AE_M   = 0U;
            tRtcReg_HOUR_ALARM.AE_H     = 0U;
            tRtcReg_DAY_ALARM.AE_D      = 0U;

            tRtcReg_CONTROL_STATUS_2.AF = 0U; /*清除中断标志位*/
            break;
        case RTC_SWITCH_ALARM_OFF:
            tRtcReg_MINUTE_ALARM.AE_M   = 1U;
            tRtcReg_HOUR_ALARM.AE_H     = 1U;
            tRtcReg_DAY_ALARM.AE_D      = 1U;

            tRtcReg_CONTROL_STATUS_2.AF = 0U; /*清除中断标志位*/
            break;
        default :
            return ERROR;
            break;
    }

    /*-----将寄存器的数值写入RTC芯片-----*/

    u8RegConvertedData = ApiRtcReg_MINUTE_ALARM_to_UINT8(tRtcReg_MINUTE_ALARM);
    (void)ApiRtcWriteOneByte(RTC_REGADD_MINUTE_ALARM,u8RegConvertedData);
    
    u8RegConvertedData = ApiRtcReg_HOUR_ALARM_to_UINT8(tRtcReg_HOUR_ALARM);
    (void)ApiRtcWriteOneByte(RTC_REGADD_HOUR_ALARM,u8RegConvertedData);
    
    u8RegConvertedData = ApiRtcReg_DAY_ALARM_to_UINT8(tRtcReg_DAY_ALARM);
    (void)ApiRtcWriteOneByte(RTC_REGADD_DAY_ALARM,u8RegConvertedData);

    /*-----同时清除闹钟的中断标志位AF-----*/
    u8RegConvertedData = ApiRtcReg_CONTROL_STATUS_2_to_UINT8(tRtcReg_CONTROL_STATUS_2);
    (void)ApiRtcWriteOneByte(RTC_REGADD_CONTROL_STATUS_2,u8RegConvertedData);
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcTimerSwitch
*  content       | 开关定时器
*  parameter     |
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
INT32 ApiRtcTimerSwitch(RtcSwitchTimer_e eRtcSwitchTimer)
{
    UINT8 u8RegConvertedData = 0U; /*寄存器转换过来的数据*/

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

    /*-----将寄存器的数值写入RTC芯片-----*/
    u8RegConvertedData = ApiRtcReg_TIMER_CONTROL_to_UINT8(tRtcReg_TIMER_CONTROL);
    (void)ApiRtcWriteOneByte(RTC_REGADD_TIMER_CONTROL,u8RegConvertedData);

    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetAlarm
*  content       | 设置闹钟
*  parameter     | pRtcAlarm:闹钟时间
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
INT32 ApiRtcSetAlarm(RtcTime_t *pRtcTimeAlarm)
{
    UINT16         u16RtcAlarmCheck = 0U;
    BCDCode_t      tBCDCodeMinute;
    BCDCode_t      tBCDCodeHour;
    BCDCode_t      tBCDCodeDay;
    RtcAlarmType_e eRtcAlarmType    = RTC_ALARM_TYPE_END;

    UINT32         u32AlarmUtcDst   = 0U;  /*闹钟需要设置的时间，UTC测试*/
    UINT32         u32CurrentUtc    = 0U;  /*当前时间，UTC格式*/
    RtcTime_t      tRtcTimeCurrent;        /*当前时间，RTC格式*/
    UINT32         u32UtcDelta      = 0U;  /*闹钟需要设置的时间与当前时间的时间差*/

    if(NULL == pRtcTimeAlarm)
    {
        return ERROR;
    }

    u16RtcAlarmCheck = (UINT16)(pRtcTimeAlarm->u8Minutes + pRtcTimeAlarm->u8Hours + pRtcTimeAlarm->u8Days);
    if(0U == u16RtcAlarmCheck)
    {
        /*闹钟设置的时间，不可以全为0*/
        return ERROR;
    }

    /*-----1 根据闹钟的间隔时长，判定当前闹钟的类型eRtcAlarmType是minutes、hours、days中的哪一种-----*/

    /*-----1.1 计算闹钟时间与当前实际时间的时差-----*/
    u32AlarmUtcDst = ApiRtcRtcToUtc(pRtcTimeAlarm);
    (void)ApiRtcGetCurrentTime(&tRtcTimeCurrent);
    u32CurrentUtc = ApiRtcRtcToUtc(&tRtcTimeCurrent);

    if(u32AlarmUtcDst <= u32CurrentUtc)
    {
        /*闹钟时间设置有误*/
        return ERROR;
    }
    else
    {
        u32UtcDelta = (UINT32)(u32AlarmUtcDst - u32CurrentUtc);
    }

    /*-----2.2 根据时差进行闹钟类型的判定-----*/
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

        return ERROR;/*闹钟时间超出范围*/
    }

    /*-----2 将闹钟数值写入寄存器-----*/

    /*-----2.1 闹钟类型判定-----*/
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
            /*不允许进入到这里*/
            break;
    }
    
    /*-----2.2 将闹钟数值写入寄存器-----*/
    
    (void)ApiRtcDectoBCD(&tBCDCodeMinute , pRtcTimeAlarm->u8Minutes);
    
    tRtcReg_MINUTE_ALARM.MINUTE_ALARM_TENS = tBCDCodeMinute.u8TensPlace;
    tRtcReg_MINUTE_ALARM.MINUTE_ALARM_UNIT = tBCDCodeMinute.u8UnitPlace;

    (void)ApiRtcDectoBCD(&tBCDCodeHour , pRtcTimeAlarm->u8Hours);

    tRtcReg_HOUR_ALARM.HOUR_ALARM_TENS     = tBCDCodeHour.u8TensPlace;
    tRtcReg_HOUR_ALARM.HOUR_ALARM_UINT     = tBCDCodeHour.u8UnitPlace;

    (void)ApiRtcDectoBCD(&tBCDCodeDay , pRtcTimeAlarm->u8Days);

    tRtcReg_DAY_ALARM.DAY_ALARM_TENS       = tBCDCodeDay.u8TensPlace;
    tRtcReg_DAY_ALARM.DAY_ALARM_UINT       = tBCDCodeDay.u8UnitPlace;

    /*-----3 将寄存器的数值写入RTC芯片-----*/
    (void)ApiRtcSetAlarmToRtcChip();
    
    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetTimer
*  content       | 设置定时器
*  parameter     | u8TimerSeconds:定时器超时时间
*  return        | OK:正常 ERROR：错误
*  notice        | u8TimerMinutes不可为0
******************************************************************************/
INT32 ApiRtcSetTimer(UINT8 u8TimerSeconds)
{
    if(0 == u8TimerSeconds)
    {
        /*参数不允许为0*/
        return ERROR;
    }

    tRtcReg_TIMER_CONTROL.TE = 1U;    /*enabled*/
    tRtcReg_TIMER_CONTROL.TD = 0X02;  /*即0B10，频率设置为1Hz，周期为1s*/
    tRtcReg_TIMER.TIMER      = u8TimerSeconds;

    /*-----将寄存器的数值写入RTC芯片----*/
    (void)ApiRtcSetTimerToRtcChip();   

    return OK;
}

/******************************************************************************
*  function name | ApiRtcSetCurrentTime
*  content       | 设置当前RTC的时间
*  parameter     | pRtcTime:设定的时间
*  return        | OK:正常 ERROR：错误
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

    /*-----DEC转换成BCD编码格式-----*/
    
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

    /*-----将寄存器的数值写入RTC芯片----*/
    (void)ApiRtcSetCurrentTimeToRtcChip();

    return OK;    
}

/******************************************************************************
*  function name | ApiRtcGetCurrentTime
*  content       | 查询当前RTC的时间
*  parameter     | pRtcTime:查询到的时间
*  return        | OK:正常 ERROR：错误
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

    /*-----register value转换成register 结构体-----*/
    (void)ApiRtcReg_UINT8_to_VL_SECONDS(      &tRtcReg_VL_SECONDS_read     , aGetCurrentTime[GET_SECONDS_INDEX] );
    (void)ApiRtcReg_UINT8_to_MINUTES(         &tRtcReg_MINUTES_read        , aGetCurrentTime[GET_MINUTES_INDEX] );
    (void)ApiRtcReg_UINT8_to_HOURS(           &tRtcReg_HOURS_read          , aGetCurrentTime[GET_HOURS_INDEX]   );
    (void)ApiRtcReg_UINT8_to_DAYS(            &tRtcReg_DAYS_read           , aGetCurrentTime[GET_DAYS_INDEX]    );
    (void)ApiRtcReg_UINT8_to_WEEKDAYS(        &tRtcReg_WEEKDAYS_read       , aGetCurrentTime[GET_WEEKDAYS_INDEX]);
    (void)ApiRtcReg_UINT8_to_CENTURY_MONTHS(  &tRtcReg_CENTURY_MONTHS_read , aGetCurrentTime[GET_MONTHS_INDEX]  );
    (void)ApiRtcReg_UINT8_to_YEARS(           &tRtcReg_YEARS_read          , aGetCurrentTime[GET_YEARS_INDEX]   );

    /*-----register 结构体转换成BCD编码-----*/
    
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

    /*-----BCD编码转换成DEC编码-----*/
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
*  content       | 查询当前RTC的时间是否有效
*  parameter     | pRtcTime:查询的时间
*  return        | TRUE:有效;FALSE:无效
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

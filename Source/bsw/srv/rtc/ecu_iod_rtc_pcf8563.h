/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ecu_iod_rtc_pcf8563.h                                       */
/* Author:      付恒    Version: 01.01    Date: 2019-11-01                     */
/* Description:  PCF8563芯片寄存器列表                                              */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS                                                         */
/********************************** 修改历史 *********************************/
/* Date         Version          Author        Description                   */
/* 2020-09-24   V31.03.02.01.01  付恒            根据JAC的需求进行修改                  */
/*****************************************************************************/
#ifndef ECU_IOD_RTC_PCF8563_H_
#define ECU_IOD_RTC_PCF8563_H_

/*---------------------------------------------------------------------------*/
/*头文件                                                                        */
/*---------------------------------------------------------------------------*/
#include "type.h"

/*---------------------------------------------------------------------------*/
/*宏定义                                                                        */
/*---------------------------------------------------------------------------*/

/*-----PCF8563芯片的地址定义-----*/
#define RTC_CHIPADD_WRITE        0X51
#define RTC_CHIPADD_READ         0x51

/*---------------------------------------------------------------------------*/
/*类型定义                                                                       */
/*---------------------------------------------------------------------------*/

/*-----PCF8563芯片的读写指令枚举-----*/
typedef enum
{
    RTC_CHIP_CTRL_RW_WRITE      = 0X00,
    RTC_CHIP_CTRL_RW_READ       = 0X01,
    RTC_CHIP_CTRL_RW_END
}RtcChipCtrlRW_e;

/*-----PCF8563芯片的寄存器地址枚举-----*/
typedef enum
{
    RTC_REGADD_CONTROL_STATUS_1 = 0X00,
    RTC_REGADD_CONTROL_STATUS_2 = 0X01,
    RTC_REGADD_VL_SECONDS       = 0X02,
    RTC_REGADD_MINUTES          = 0X03,
    RTC_REGADD_HOURS            = 0X04,
    RTC_REGADD_DAYS             = 0X05,
    RTC_REGADD_WEEKDAYS         = 0X06,
    RTC_REGADD_CENTURY_MONTHS   = 0X07,
    RTC_REGADD_YEARS            = 0X08,
    RTC_REGADD_MINUTE_ALARM     = 0X09,
    RTC_REGADD_HOUR_ALARM       = 0X0A,
    RTC_REGADD_DAY_ALARM        = 0X0B,
    RTC_REGADD_WEEKDAY_ALARM    = 0X0C,
    RTC_REGADD_CLKOUT_CONTROL   = 0X0D,
    RTC_REGADD_TIMER_CONTROL    = 0X0E,
    RTC_REGADD_TIMER            = 0X0F,
    RTC_REGADD_END
}RtcRegisterAdd_e;

/*-----寄存器的结构体定义-----*/

/*-----CONTROL_STATUS_1-----*/
typedef struct  
{  
    UINT8 TEST1            :1;
    UINT8 none1            :1;
    UINT8 STOP             :1;
    UINT8 none2            :1;
    UINT8 TESTC            :1;
    UINT8 none3            :3;
}RtcReg_CONTROL_STATUS_1_t;

/*------CONTROL_STATUS_2----*/
typedef struct  
{  
    UINT8 none             :3;
    UINT8 TI_TP            :1;
    UINT8 AF               :1;
    UINT8 TF               :1;
    UINT8 AIE              :1;
    UINT8 TIE              :1;
}RtcReg_CONTROL_STATUS_2_t;

/*-----VL_SECONDS-----*/
typedef struct  
{  
    UINT8 VL               :1;
    UINT8 SECONDS_TENS     :3;
    UINT8 SECONDS_UINT     :4;
}RtcReg_VL_SECONDS_t;

/*-----MINUTES-----*/
typedef struct  
{  
    UINT8 none             :1;
    UINT8 MINUTES_TENS     :3;
    UINT8 MINUTES_UINT     :4;
}RtcReg_MINUTES_t;

/*----HOURS------*/
typedef struct  
{ 
    UINT8 none             :2;
    UINT8 HOURS_TENS       :2;
    UINT8 HOURS_UINT       :4;
}RtcReg_HOURS_t;

/*-----DAYS-----*/
typedef struct  
{  
    UINT8 none             :2;
    UINT8 DAYS_TENS        :2;
    UINT8 DAYS_UINT        :4;
}RtcReg_DAYS_t;

/*-----WEEKDAYS-----*/
typedef struct  
{  
    UINT8 none             :5;
    UINT8 WEEKDAYS         :3;
}RtcReg_WEEKDAYS_t;

/*-----CENTURY_MONTHS-----*/
typedef struct  
{  
    UINT8 C                :1;
    UINT8 none             :2;
    UINT8 MONTHS_TENS      :1;
    UINT8 MONTHS_UINT      :4;
}RtcReg_CENTURY_MONTHS_t;

/*-----YEARS-----*/
typedef struct  
{  
    UINT8 YEARS_TENS       :4;
    UINT8 YEARS_UINT       :4;
}RtcReg_YEARS_t;

/*-----MINUTE_ALARM-----*/
typedef struct  
{  
    UINT8 AE_M             :1;
    UINT8 MINUTE_ALARM_TENS:3;
    UINT8 MINUTE_ALARM_UNIT:4;
}RtcReg_MINUTE_ALARM_t;

/*-----HOUR_ALARM-----*/
typedef struct  
{  
    UINT8 AE_H             :1;
    UINT8 none             :1;
    UINT8 HOUR_ALARM_TENS  :2;
    UINT8 HOUR_ALARM_UINT  :4;
}RtcReg_HOUR_ALARM_t;

/*-----DAY_ALARM-----*/
typedef struct  
{  
    UINT8 AE_D             :1;
    UINT8 none             :1;
    UINT8 DAY_ALARM_TENS   :2;
    UINT8 DAY_ALARM_UINT   :4;
}RtcReg_DAY_ALARM_t;

/*-----WEEKDAY_ALARM-----*/
typedef struct  
{  
    UINT8 AE_W             :1;
    UINT8 none             :4;
    UINT8 WEEKDAY_ALARM    :3;
}RtcReg_WEEKDAY_ALARM_t;

/*-----CLKOUT_CONTROL-----*/
typedef struct  
{  
    UINT8 FE               :1;
    UINT8 none             :5;
    UINT8 FD               :2;
}RtcReg_CLKOUT_CONTROL_t;

/*-----TIMER_CONTROL-----*/
typedef struct  
{  
    UINT8 TE               :1;
    UINT8 none             :5;
    UINT8 TD               :2;
}RtcReg_TIMER_CONTROL_t;

/*-----TIMER-----*/
typedef struct  
{  
    UINT8 TIMER            :8;
}RtcReg_TIMER_t;

/*---------------------------------------------------------------------------*/
/*外部公开变量                                                                     */
/*---------------------------------------------------------------------------*/
extern RtcReg_CONTROL_STATUS_1_t tRtcReg_CONTROL_STATUS_1;
extern RtcReg_CONTROL_STATUS_2_t tRtcReg_CONTROL_STATUS_2;
extern RtcReg_VL_SECONDS_t       tRtcReg_VL_SECONDS;
extern RtcReg_MINUTES_t          tRtcReg_MINUTES;
extern RtcReg_HOURS_t            tRtcReg_HOURS;
extern RtcReg_DAYS_t             tRtcReg_DAYS;
extern RtcReg_WEEKDAYS_t         tRtcReg_WEEKDAYS;
extern RtcReg_CENTURY_MONTHS_t   tRtcReg_CENTURY_MONTHS;
extern RtcReg_YEARS_t            tRtcReg_YEARS;
extern RtcReg_MINUTE_ALARM_t     tRtcReg_MINUTE_ALARM;
extern RtcReg_HOUR_ALARM_t       tRtcReg_HOUR_ALARM;
extern RtcReg_DAY_ALARM_t        tRtcReg_DAY_ALARM;
extern RtcReg_WEEKDAY_ALARM_t    tRtcReg_WEEKDAY_ALARM;
extern RtcReg_CLKOUT_CONTROL_t   tRtcReg_CLKOUT_CONTROL;
extern RtcReg_TIMER_CONTROL_t    tRtcReg_TIMER_CONTROL;
extern RtcReg_TIMER_t            tRtcReg_TIMER;

/*---------------------------------------------------------------------------*/
/*参数定义                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*枚举变量定义                                                                     */
/*---------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*外部公开函数声明                                                                   */
/*---------------------------------------------------------------------------*/

/******************************************************************************
*  function name | ApiRtcPCF8563Init
*  content       | RTC芯片PCF8563初始化
*  parameter     |
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
extern void ApiRtcPCF8563Init(void);

#endif

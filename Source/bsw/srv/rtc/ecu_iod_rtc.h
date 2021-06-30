/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ecu_iod_rtc.h                                               */
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
/* 2020-09-24   V31.03.02.01.01  付恒            根据JAC的需求进行修改                  */
/* 2021-01-27   V31.03.02.01.02  付恒            增加查询当前RTC时间是否有效的判断接口                  */
/*****************************************************************************/
#ifndef ECU_IOD_RTC_H_
#define ECU_IOD_RTC_H_

/*---------------------------------------------------------------------------*/
/*头文件                                                                        */
/*---------------------------------------------------------------------------*/
#include "type.h"

/*---------------------------------------------------------------------------*/
/*宏定义                                                                        */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*类型定义                                                                       */
/*---------------------------------------------------------------------------*/

/*-----闹钟的开关-----*/
typedef enum
{
    RTC_SWITCH_ALARM_ON  = 0U,  /*打开*/
    RTC_SWITCH_ALARM_OFF = 1U,  /*关闭*/
    RTC_SWITCH_ALARM_END 
}RtcSwitchAlarm_e;

/*-----定时器的开关-----*/
typedef enum
{
    RTC_SWITCH_TIMER_ON  = 0U,  /*打开*/
    RTC_SWITCH_TIMER_OFF = 1U,  /*关闭*/
    RTC_SWITCH_TIMER_END 
}RtcSwitchTimer_e;
    
/*-----闹钟设置时间-----*/
typedef struct
{
	UINT8 u8Minutes;
	UINT8 u8Hours;
	UINT8 u8Days;
}RtcAlarm_t;

/*-----RTC时间-----*/
typedef struct
{
	UINT8 u8Seconds;
	UINT8 u8Minutes;
	UINT8 u8Hours;
	UINT8 u8Days;
	UINT8 u8WeekDays;
	UINT8 u8Months;
	UINT8 u8Years;
}RtcTime_t;

/*---------------------------------------------------------------------------*/
/*外部公开变量                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*参数定义                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*枚举变量定义                                                                     */
/*---------------------------------------------------------------------------*/
  
/*---------------------------------------------------------------------------*/
/*类型定义                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*外部公开函数声明                                                                   */
/*---------------------------------------------------------------------------*/

/******************************************************************************
*  function name | ApiRtcInit
*  content       | RTC初始化
*  parameter     |
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
extern INT32 ApiRtcInit(void);

/******************************************************************************
*  function name | ApiRtcRtcToUtc
*  content       | 将RTC时间转换成UTC时间
*  parameter     | 
*  return        | 
*  notice        | 在AUX组件中，UTC时间统一代指0时区的时间。只有带有CN后缀的，才表示北京时间，即8时区的时间
******************************************************************************/
extern UINT32 ApiRtcRtcToUtc(RtcTime_t *pRtcTime);

/******************************************************************************
*  function name | ApiRtcUtcToRtc
*  content       | 将UTC时间转换成RTC时间
*  parameter     | 
*  return        | 
*  notice        | 在AUX组件中，UTC时间统一代指0时区的时间。只有带有CN后缀的，才表示北京时间，即8时区的时间
******************************************************************************/
extern INT32 ApiRtcUtcToRtc(RtcTime_t *pRtcTime , UINT32 u32UtcTime);

/******************************************************************************
*  function name | ApiRtcAlarmSwitch
*  content       | 开关闹钟
*  parameter     |
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
extern INT32 ApiRtcAlarmSwitch(RtcSwitchAlarm_e eRtcSwitchAlarm);

/******************************************************************************
*  function name | ApiRtcTimerSwitch
*  content       | 开关定时器
*  parameter     |
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
extern INT32 ApiRtcTimerSwitch(RtcSwitchTimer_e eRtcSwitchTimer);

/******************************************************************************
*  function name | ApiRtcSetAlarm
*  content       | 设置闹钟
*  parameter     | pRtcTimeAlarm:闹钟时间
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
extern INT32 ApiRtcSetAlarm(RtcTime_t *pRtcTimeAlarm);

/******************************************************************************
*  function name | ApiRtcSetTimer
*  content       | 设置定时器
*  parameter     | u8TimerSeconds:定时器超时时间
*  return        | OK:正常 ERROR：错误
*  notice        | u8TimerMinutes不可为0
******************************************************************************/
extern INT32 ApiRtcSetTimer(UINT8 u8TimerSeconds);

/******************************************************************************
*  function name | ApiRtcSetCurrentTime
*  content       | 设置当前RTC的时间
*  parameter     | pRtcTime:设定的时间
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
extern INT32 ApiRtcSetCurrentTime(RtcTime_t *pRtcTime);

/******************************************************************************
*  function name | ApiRtcGetCurrentTime
*  content       | 查询当前RTC的时间
*  parameter     | pRtcTime:查询到的时间
*  return        | OK:正常 ERROR：错误
*  notice        | 
******************************************************************************/
extern INT32 ApiRtcGetCurrentTime(RtcTime_t *pRtcTime);

/******************************************************************************
*  function name | ApiRtcTimeValidityCheck
*  content       | 查询当前RTC的时间是否有效
*  parameter     | pRtcTime:查询的时间
*  return        | TRUE:有效;FALSE:无效
*  notice        | 
******************************************************************************/
extern BOOL ApiRtcTimeValidityCheck(RtcTime_t *pRtcTime);

#endif

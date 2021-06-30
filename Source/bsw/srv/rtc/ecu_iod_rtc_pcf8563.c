/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ecu_iod_gsensor_lis3dh.c                                    */
/* Author:      ����    Version: 01.01    Date: 2019-11-01                     */
/* Description:  LIS3DHоƬ�Ĵ����б�                                               */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS                                                         */
/********************************** �޸���ʷ *********************************/
/* Date         Version          Author        Description                   */
/* 2020-09-24   V31.03.02.01.01  ����            ����JAC����������޸�                  */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*ͷ�ļ�                                                                        */
/*---------------------------------------------------------------------------*/
#include "ecu_iod_rtc_pcf8563.h" 
#include "string.h"

/*---------------------------------------------------------------------------*/
/*�궨��                                                                        */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*��������                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*���Ͷ���                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*ȫ�ֱ�������                                                                     */
/*---------------------------------------------------------------------------*/
RtcReg_CONTROL_STATUS_1_t tRtcReg_CONTROL_STATUS_1;
RtcReg_CONTROL_STATUS_2_t tRtcReg_CONTROL_STATUS_2;
RtcReg_VL_SECONDS_t       tRtcReg_VL_SECONDS;
RtcReg_MINUTES_t          tRtcReg_MINUTES;
RtcReg_HOURS_t            tRtcReg_HOURS;
RtcReg_DAYS_t             tRtcReg_DAYS;
RtcReg_WEEKDAYS_t         tRtcReg_WEEKDAYS;
RtcReg_CENTURY_MONTHS_t   tRtcReg_CENTURY_MONTHS;
RtcReg_YEARS_t            tRtcReg_YEARS;
RtcReg_MINUTE_ALARM_t     tRtcReg_MINUTE_ALARM;
RtcReg_HOUR_ALARM_t       tRtcReg_HOUR_ALARM;
RtcReg_DAY_ALARM_t        tRtcReg_DAY_ALARM;
RtcReg_WEEKDAY_ALARM_t    tRtcReg_WEEKDAY_ALARM;
RtcReg_CLKOUT_CONTROL_t   tRtcReg_CLKOUT_CONTROL;
RtcReg_TIMER_CONTROL_t    tRtcReg_TIMER_CONTROL;
RtcReg_TIMER_t            tRtcReg_TIMER;

/*---------------------------------------------------------------------------*/
/*�ֲ���������                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*�ֲ���������                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*�ֲ���������                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*ȫ�ֺ�������                                                                     */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | ApiRtcPCF8563Init
*  content       | RTCоƬPCF8563��ʼ��
*  parameter     |
*  return        | OK:���� ERROR������
*  notice        | 
******************************************************************************/
void ApiRtcPCF8563Init(void)
{
    memset(&tRtcReg_CONTROL_STATUS_1,0,sizeof(RtcReg_CONTROL_STATUS_1_t));
    memset(&tRtcReg_CONTROL_STATUS_2,0,sizeof(RtcReg_CONTROL_STATUS_2_t));
    memset(&tRtcReg_VL_SECONDS      ,0,sizeof(RtcReg_VL_SECONDS_t)      );
    memset(&tRtcReg_MINUTES         ,0,sizeof(RtcReg_MINUTES_t)         );
    memset(&tRtcReg_HOURS           ,0,sizeof(RtcReg_HOURS_t)           );
    memset(&tRtcReg_DAYS            ,0,sizeof(RtcReg_DAYS_t)            );
    memset(&tRtcReg_WEEKDAYS        ,0,sizeof(RtcReg_WEEKDAYS_t)        );
    memset(&tRtcReg_CENTURY_MONTHS  ,0,sizeof(RtcReg_CENTURY_MONTHS_t)  );
    memset(&tRtcReg_YEARS           ,0,sizeof(RtcReg_YEARS_t)           );
    memset(&tRtcReg_MINUTE_ALARM    ,0,sizeof(RtcReg_MINUTE_ALARM_t)    );
    memset(&tRtcReg_HOUR_ALARM      ,0,sizeof(RtcReg_HOUR_ALARM_t)      );
    memset(&tRtcReg_DAY_ALARM       ,0,sizeof(RtcReg_DAY_ALARM_t)       );
    memset(&tRtcReg_WEEKDAY_ALARM   ,0,sizeof(RtcReg_WEEKDAY_ALARM_t)   );
    memset(&tRtcReg_CLKOUT_CONTROL  ,0,sizeof(RtcReg_CLKOUT_CONTROL_t)  );
    memset(&tRtcReg_TIMER_CONTROL   ,0,sizeof(RtcReg_TIMER_CONTROL_t)   );
    memset(&tRtcReg_TIMER           ,0,sizeof(RtcReg_TIMER_t)           );
}

/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    bl_timer.c                                                  */
/* Description:  Software timer implementation                               */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** �޸���ʷ *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     �»�        ���ߴ���                              */
/*****************************************************************************/


/*---------------------------------------------------------------------------*/
/* ͷ�ļ�																	*/
/*---------------------------------------------------------------------------*/
#include "bl_timer.h"
#include "ll_target.h"

/*---------------------------------------------------------------------------*/
/* ���Ͷ��� 																 */
/*---------------------------------------------------------------------------*/
	
/*---------------------------------------------------------------------------*/
/* �궨��																	*/
/*---------------------------------------------------------------------------*/
static Timer_t tTimerArray[TIMER_ARRAY_SIZE];


/*---------------------------------------------------------------------------*/
/* �������� 																 */
/*---------------------------------------------------------------------------*/
	
/*---------------------------------------------------------------------------*/
/* �������� 																 */
/*---------------------------------------------------------------------------*/
	
	
/*---------------------------------------------------------------------------*/
/* �ⲿ���ձ���															   */
/*---------------------------------------------------------------------------*/
	
/*---------------------------------------------------------------------------*/
/* �ڲ���������															   */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* �ⲿ��������															   */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | ApiInitTimer
*  content       | Setting the initial value of a timer
*  parameter     | timer_index:ʱ����� initval:��ʼֵ
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiInitTimer(UINT8 u8TimerIndex, UINT16 u16Initval)
{
    tTimerArray[u8TimerIndex].u16Initval = u16Initval;
    tTimerArray[u8TimerIndex].u16Counter = u16Initval;
    tTimerArray[u8TimerIndex].eStatus = timerRunning;
}


/******************************************************************************
*  function name | ApiResetTimer
*  content       | Reset a timer
*  parameter     | timer_index:ʱ�����
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiResetTimer(UINT8 u8TimerIndex)
{
    tTimerArray[u8TimerIndex].u16Counter = tTimerArray[u8TimerIndex].u16Initval;
    tTimerArray[u8TimerIndex].eStatus = timerRunning;
}


/******************************************************************************
*  function name | ApiStopTimer
*  content       | Stop a timer
*  parameter     | timer_index:ʱ�����
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiStopTimer(UINT8 u8TimerIndex)
{
    tTimerArray[u8TimerIndex].eStatus = timerStopped;
}


/******************************************************************************
*  function name | ApiIsTimerStopped
*  content       | ��ѯtimer�Ƿ���stop
*  parameter     | timer_index:ʱ�����
*  return        | void
*  notice        | ��
******************************************************************************/
bool ApiIsTimerStopped(UINT8 u8TimerIndex)
{
    return (tTimerArray[u8TimerIndex].eStatus == timerStopped) ? true : false;
}


/******************************************************************************
*  function name | ApiCheckTimer
*  content       | Checking if a timer is expired
*  parameter     | timer_index:ʱ�����
*  return        | void
*  notice        | ��
******************************************************************************/
Timer_e ApiCheckTimer(UINT8 u8TimerIndex)
{
    bool bIsTimerExpired;

    /* keep timert up-to-date */
    ApiTickTimers();

    bIsTimerExpired = ((tTimerArray[u8TimerIndex].eStatus == timerRunning)
                       && (tTimerArray[u8TimerIndex].u16Counter == 0u))? true : false;
    return ((false == bIsTimerExpired) ? not_expired : expired);
}


/******************************************************************************
*  function name | ApiTickTimers
*  content       | Roll over all timers and tick them if necessary
*  parameter     | timer_index:ʱ�����
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiTickTimers(void)
{
    UINT8 u8ctr;

    if(ApiLlTimer() == expired)
    {
        for(u8ctr = 0u; u8ctr < TIMER_ARRAY_SIZE; u8ctr++)
        {
            if(tTimerArray[u8ctr].u16Counter != 0u)
            {
                tTimerArray[u8ctr].u16Counter--;
            }
        }
    }
}


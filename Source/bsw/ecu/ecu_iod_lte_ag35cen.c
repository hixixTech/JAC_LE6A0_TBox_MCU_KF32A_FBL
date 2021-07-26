/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ecu_iod_lte_ag35cen.c                                       */
/* Author:      ����    Version: 01.01    Date: 2019-11-01                     */
/* Description:  AG35ģ����м��                                                  */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS                                                         */
/********************************** �޸���ʷ *********************************/
/* Date         Version          Author        Description                   */
/* 2020-09-24   V31.03.02.01.01  ����            �½����߰汾                        */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*ͷ�ļ�                                                                        */
/*---------------------------------------------------------------------------*/
#include "ecu_iod_lte_ag35cen.h"
#include "type.h"
#include "ecu_gpio.h"
#include "ecu_adc.h"

/*---------------------------------------------------------------------------*/
/*�궨��                                                                        */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*��������                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*ȫ�ֱ�������                                                                     */
/*---------------------------------------------------------------------------*/

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
*  function name | ApiLteInit
*  content       | AG35�м����ʼ��
*  parameter     | ��
*  return        | ��
*  notice        | 
******************************************************************************/
void ApiLteInit(void)
{
    (void)ApiGpioInputRead(GPIO_4G_WORKPWM_3V3);
}

/******************************************************************************
*  function name | ApiLteGetPwmSt
*  content       | ��ѯģ���PWM����״̬
*  parameter     | ��
*  return        | LtePwmState_e:������ǰ״̬
*  notice        | 
******************************************************************************/
LtePwmState_e ApiLteGetPwmSt(void)
{
    INT32         i32PwmStVolRead = 0;
    LtePwmState_e eLtePwmState    = LTE_PWM_ST_PULSE_END;

    /*�����������,Ӧ��ʹ��AD����ͨ����*/
    i32PwmStVolRead = ApiGpioInputRead(GPIO_4G_WORKPWM_3V3);

    if(1 == i32PwmStVolRead)
    {
        eLtePwmState = LTE_PWM_ST_PULSE_HIGH;
    }
    else if(0 == i32PwmStVolRead)
    {
        eLtePwmState = LTE_PWM_ST_PULSE_LOW;
    }
    else
    {
        eLtePwmState = LTE_PWM_ST_PULSE_END; /*��������뵽����*/
    }

    return eLtePwmState;
}

/******************************************************************************
*  function name | ApiLteGetGnssAntSt
*  content       | ��ѯGnssAnt״̬
*  parameter     | ��
*  return        | GnssAntState_e:���ߵ�ǰ״̬
*  notice        | 
******************************************************************************/
GnssAntState_e ApiLteGetGnssAntSt(void)
{
    GnssAntState_e eGnssAntState     = GNSS_ANT_ST_NORMAL;
    INT32          i32GnssAntVolRead = 0;
    
    i32GnssAntVolRead = ApiAdcVoltRead(ADC1_INPUT);

    if((i32GnssAntVolRead <= GNSSANT_VOL_NORMAL_MAX)&&(i32GnssAntVolRead >= GNSSANT_VOL_NORMAL_MIN))
    {
        eGnssAntState = GNSS_ANT_ST_NORMAL;
    }
    else if((i32GnssAntVolRead < GNSSANT_VOL_NORMAL_MIN)&&(GNSSANT_VOL_OPEN_MAX > GNSSANT_VOL_OPEN_MAX))
    {
        eGnssAntState = GNSS_ANT_ST_MIDDLE1;
    }
    else if((i32GnssAntVolRead <= GNSSANT_VOL_OPEN_MAX)&&(i32GnssAntVolRead >= GNSSANT_VOL_OPEN_MIN))
    {
        eGnssAntState = GNSS_ANT_ST_OPEN;
    }
    else if((i32GnssAntVolRead < GNSSANT_VOL_OPEN_MIN)&&(i32GnssAntVolRead > GNSSANT_VOL_SHORT_TO_GND_MAX))
    {
        eGnssAntState = GNSS_ANT_ST_MIDDLE2;
    }
    else if(i32GnssAntVolRead <= GNSSANT_VOL_SHORT_TO_GND_MAX)
    {
        eGnssAntState = GNSS_ANT_ST_SHORT_TO_GND;
    }
    else
    {
        eGnssAntState = GNSS_ANT_ST_END;/*�������������*/
    }
    
    return eGnssAntState;
}

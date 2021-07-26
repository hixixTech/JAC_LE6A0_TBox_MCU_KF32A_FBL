/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ecu_iod_lte_ag35cen.c                                       */
/* Author:      付恒    Version: 01.01    Date: 2019-11-01                     */
/* Description:  AG35模组的中间件                                                  */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS                                                         */
/********************************** 修改历史 *********************************/
/* Date         Version          Author        Description                   */
/* 2020-09-24   V31.03.02.01.01  付恒            新建基线版本                        */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*头文件                                                                        */
/*---------------------------------------------------------------------------*/
#include "ecu_iod_lte_ag35cen.h"
#include "type.h"
#include "ecu_gpio.h"
#include "ecu_adc.h"

/*---------------------------------------------------------------------------*/
/*宏定义                                                                        */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*常量定义                                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*全局变量定义                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*局部变量定义                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*局部函数声明                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*局部函数定义                                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*全局函数定义                                                                     */
/*---------------------------------------------------------------------------*/

/******************************************************************************
*  function name | ApiLteInit
*  content       | AG35中间件初始化
*  parameter     | 无
*  return        | 无
*  notice        | 
******************************************************************************/
void ApiLteInit(void)
{
    (void)ApiGpioInputRead(GPIO_4G_WORKPWM_3V3);
}

/******************************************************************************
*  function name | ApiLteGetPwmSt
*  content       | 查询模组的PWM方波状态
*  parameter     | 无
*  return        | LtePwmState_e:方波当前状态
*  notice        | 
******************************************************************************/
LtePwmState_e ApiLteGetPwmSt(void)
{
    INT32         i32PwmStVolRead = 0;
    LtePwmState_e eLtePwmState    = LTE_PWM_ST_PULSE_END;

    /*这里存在问题,应该使用AD采样通道的*/
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
        eLtePwmState = LTE_PWM_ST_PULSE_END; /*不允许进入到这里*/
    }

    return eLtePwmState;
}

/******************************************************************************
*  function name | ApiLteGetGnssAntSt
*  content       | 查询GnssAnt状态
*  parameter     | 无
*  return        | GnssAntState_e:天线当前状态
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
        eGnssAntState = GNSS_ANT_ST_END;/*不允许进入这里*/
    }
    
    return eGnssAntState;
}

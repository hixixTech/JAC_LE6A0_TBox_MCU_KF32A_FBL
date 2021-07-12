/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_pm.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/06/23
 * BY:           feifei.xu
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

/*****************************************************************************
** #include 
*****************************************************************************/
#include "system_init.h"
#include "ecu_pm.h"
#include "os_log.h"
#include "ecu_gpio.h"
#include "ecu_misc.h"
/*****************************************************************************
** #define
*****************************************************************************/


/*****************************************************************************
** typedef
*****************************************************************************/

/*****************************************************************************
** global variable
*****************************************************************************/


/*****************************************************************************
** static variables
*****************************************************************************/


/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/


/*****************************************************************************
** function prototypes
*****************************************************************************/
/****************************************************************************/
/**
 * Function Name: ApiPmlpRunModel
 * Description: 低功耗运行
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/23, feifei.xu create this function
 ****************************************************************************/
void ApiPmlpRunModel(void)
{
    /******************降低主频进一步降低休眠功耗***************/
	OSC_PLL_Software_Enable(FALSE);  //关闭PLL，以降低功耗
	OSC_HFCK_Enable(FALSE);
	OSC_SCK_Division_Config(SCLK_DIVISION_1);
	OSC_SCK_Source_Config(SCLK_SOURCE_INTLF);
	while(OSC_Get_INTLF_INT_Flag() != SET);//在操作备份域之前先将SCK降低到1M,低功耗模式不允许时钟源大于2M
	BKP_Write_And_Read_Enable(TRUE); //备份域读写使能
	SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LPREN_POS); //LPR使能，进入低功耗运行模式
}
/****************************************************************************/
/**
 * Function Name: ApiPmSleepModel
 * Description: 普通休眠
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/23, feifei.xu create this function
 ****************************************************************************/
void ApiPmSleepModel(void)
{
	// vTaskEndScheduler();
    /******************降低主频进一步降低休眠功耗***************/
	OSC_PLL_Software_Enable(FALSE);  //关闭PLL，以降低功耗
	OSC_HFCK_Enable(FALSE); //关闭HFCK也可以降低功耗
	OSC_SCK_Division_Config(SCLK_DIVISION_1);
	OSC_SCK_Source_Config(SCLK_SOURCE_INTLF); //将SCK降到32K以尽可能的降低功耗
	while(OSC_Get_INTLF_INT_Flag() != SET);
    /* 休眠之前，先执行以下空指令 */
    asm("NOP");
	asm("NOP");
	asm("SLEEP");
	asm("NOP");
	asm("NOP");
	while(1)
	{
		;
	}
}
/****************************************************************************/
/**
 * Function Name: ApiPmLpSleepModel
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/23, feifei.xu create this function
 ****************************************************************************/
void ApiPmLpSleepModel(void)
{
    /******************降低主频进一步降低休眠功耗***************/
	OSC_PLL_Software_Enable(FALSE);  //关闭PLL，以降低功耗
	OSC_HFCK_Enable(FALSE);
	OSC_SCK_Division_Config(SCLK_DIVISION_1);
	OSC_SCK_Source_Config(SCLK_SOURCE_INTLF);
	while(OSC_Get_INTLF_INT_Flag() != SET); //在操作备份域之前先将SCK降低到1M,低功耗模式不允许时钟源大于2M
	BKP_Write_And_Read_Enable(TRUE); //备份域读写使能
	SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LPREN_POS); //LPR使能，进入低功耗运行模式
    /* 休眠之前，先执行以下空指令 */
	asm("NOP" );
	asm("NOP" );
	asm("SLEEP" );
	asm("NOP" );
	asm("NOP" );
}
/****************************************************************************/
/**
 * Function Name: ApiPmStop0Model
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/23, feifei.xu create this function
 ****************************************************************************/
void ApiPmStop0Model(void)
{
    BKP_Write_And_Read_Enable(TRUE); //备份域读写使能
	PM_Low_Power_Mode_Config(PM_LOW_POWER_MODE_STOP_0); //进入Stop0模式
	PM_External_Wakeup_Pin_Enable(PM_PIN_WKP5,TRUE); //使能WakeUp5引脚
	PM_External_Wakeup_Edge_Config(PM_PIN_WKP5 , PM_TRIGGER_RISE_EDGE); //上升沿触发，但该脚需要配置为模拟通道
	PM_Clear_External_Wakeup_Pin_Flag(PM_WAKEUP_EXTERNAL_PIN_WKP5);//清除标志位后要直接休眠，中间如果检测到Wakup有上升沿会导致再次置位标志位，然后导致直接进入普通Sleep模式
	asm("SLEEP" );
}
/****************************************************************************/
/**
 * Function Name: ApiPmStop1Model
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/23, feifei.xu create this function
 ****************************************************************************/
void ApiPmStop1Model(void)
{
    BKP_Write_And_Read_Enable(TRUE); //备份域读写使能
	PM_Low_Power_Mode_Config(PM_LOW_POWER_MODE_STOP_1); //进入Stop1模式
	PM_External_Wakeup_Pin_Enable(PM_PIN_WKP5,TRUE); //使能WakeUp5引脚
	PM_External_Wakeup_Edge_Config(PM_PIN_WKP5 , PM_TRIGGER_RISE_EDGE); //上升沿触发，但该脚需要配置为模拟通道
	PM_Clear_External_Wakeup_Pin_Flag(PM_WAKEUP_EXTERNAL_PIN_WKP5);//清除标志位后要直接休眠，中间如果检测到Wakup有上升沿会导致再次置位标志位，然后导致直接进入普通Sleep模式
	asm("SLEEP" );
}
/****************************************************************************/
/**
 * Function Name: Ecu_PM_StandByModel
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/23, feifei.xu create this function
 ****************************************************************************/
void ApiPmStandByModel(void)
{
    BKP_Write_And_Read_Enable(TRUE); //备份域读写使能
	PM_Low_Power_Mode_Config(PM_LOW_POWER_MODE_STANDBY); //进入StandBy模式
	PM_External_Wakeup_Pin_Enable(PM_PIN_WKP5,TRUE); //使能WakeUp5引脚
	PM_External_Wakeup_Edge_Config(PM_PIN_WKP5 , PM_TRIGGER_RISE_EDGE); //上升沿触发，但该脚需要配置为模拟通道
	PM_Clear_External_Wakeup_Pin_Flag(PM_WAKEUP_EXTERNAL_PIN_WKP5);//清除标志位后要直接休眠，中间如果检测到Wakup有上升沿会导致再次置位标志位，然后导致直接进入普通Sleep模式
	asm("SLEEP" );
}
/****************************************************************************/
/**
 * Function Name: Ecu_PM_ShutDownModel
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/23, feifei.xu create this function
 ****************************************************************************/
void ApiPmShutDownModel(void)
{
	BKP_Write_And_Read_Enable(TRUE); //备份域读写使能
	PM_Low_Power_Mode_Config(PM_LOW_POWER_MODE_SHUTDOWN); //进入ShutDown模式
	PM_External_Wakeup_Pin_Enable(PM_PIN_WKP5,TRUE); //使能WakeUp5引脚
	PM_External_Wakeup_Edge_Config(PM_PIN_WKP5 , PM_TRIGGER_RISE_EDGE); //上升沿触发，但该脚需要配置为模拟通道
	PM_Clear_External_Wakeup_Pin_Flag(PM_WAKEUP_EXTERNAL_PIN_WKP5);//清除标志位后要直接休眠，中间如果检测到Wakup有上升沿会导致再次置位标志位，然后导致直接进入普通Sleep模式
	asm("SLEEP" );
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


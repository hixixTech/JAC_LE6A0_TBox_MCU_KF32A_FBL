/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ll_target.c                                                 */
/* Description:  Target dependent source file                                */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠        基线创建                              */
/* 2019-10-24   V11.01.01.01     陈惠        蓝牙Boot模式只在蓝牙代码刷新时进入，删除初始化进入操作*/
/* 2019-12-30   V11.01.01.02     陈惠        删除清复位源函数*/
/* 2020-03-11   V11.01.01.03     陈惠        增加MPU开机和保持代码*/
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "type.h"
#include "bsw_init.h"
#include "bl_timer.h"
#include "ll_target.h"
#include "dl_engine.h"    /* get semaphores.sa_firstseed */
// #include "mcu_drv_stbc.h"
#include "ecu_gpio.h"
#include "os_log.h"
// #include "mcu_com_uart.h"
// #include "mcu_drv_timer.h"
#include "ecu_adc.h"
// #include "mcu_drv_clock.h"
#include "ecu_flash.h"
// #include "mcu_drv_wdg.h"
// #include "delay.h"
#include "bl_app.h"
#include "mem_ee.h"
#include "bl_config_pc.h"

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/
static void PortInit(void);


/******************************************************************************
*  function name | port_init
*  content       | port初始化
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
static void PortInit(void)
{
	/* Initalize ports */
	// (void)ApiGpioOutput(GPIO_BAT_DET_CTL);        /*配置VBAT检测使能脚为输出*/
	(void)ApiGpioOutputHigh(GPIO_BAT_DET_CTL);    /*配置VBAT检测使能脚为高*/

	/* Initalize mpu ports */
	// (void)ApiGpioOutput(GPIO_4G_VBUS_CNTL);
	(void)ApiGpioOutputHigh(GPIO_4G_VBUS_CNTL);
	// (void)ApiGpioOutput(GPIO_PWR_CTL_4G);
	(void)ApiGpioOutputLow(GPIO_PWR_CTL_4G);
	// (void)ApiGpioOutput(GPIO_WKUP_4G);
	(void)ApiGpioOutputHigh(GPIO_WKUP_4G);

	// (void)ApiGpioInput(GPIO_4G_STATUS);
}


/******************************************************************************
*  function name | ApiLlCopyBuffer
*  content       | 从一个地址copy到另一个地址(一般用于从flash中copy到ram)
*  parameter     | dest：目标地址，src：数据地址，count：长度
*  return        | 无
*  notice        | 无
******************************************************************************/
void ApiLlCopyBuffer(UINT8* pu8Dest, const UINT8* pu8Src, UINT16 u16Count)
{
    while(u16Count != 0)
    {
        *pu8Dest++ = *pu8Src++;
        u16Count--;
    }
}


/******************************************************************************
*  function name | ApiLlTimer
*  content       | 时间参数读取，1ms计数，每检测到1ms减1
*  parameter     | void
*  return        | timer_o：1-expired，0-not_expired
*  notice        | 无
******************************************************************************/
Timer_e ApiLlTimer(void)
{
	// static UINT16 u16Cnt = 0;
	// static UINT32 i = 0;

	// i = TAUB0.CNT0;
	// do
	// { 	
		delayms(1);													 
		// if(((UINT16)(TAUB0.CNT0 - u16Cnt)) < 32768u)	  
		// {													
			// u16Cnt += TIMER_CLOCK_KHZ; 				 
			return(expired);									
		// }														 
		// return(not_expired);									 
	// }while(0);//TODO:
}


/******************************************************************************
*  function name | ApiLlReset
*  content       | 复位
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiLlReset(void)
{
    UINT16 u16Timeout = 0xfff; 
	ApiWdtHwtDisable();
	while(u16Timeout--);
	ApiStbcSoftReset();

    while(1)
    {
    }
}


/******************************************************************************
*  function name | ApiLlEcuInit
*  content       | 板级硬件初始化
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiLlEcuInit(void)
{
	UINT32 u32ResetFac;
	INT32 s32Ret ;

	UINT8 u8InitData[256] = {DATA_0xFF_256bytes};

	// __DI();                 /*关闭中断*/
	SystemInit(); //系统时钟96M,外设高频时钟16M

	ApiLogInit();
	ApiLogEnable();
	
	bsw_init();
	app_init();
	Mem_EE_Init();
	
	ApiLogPrint(_LOG_DEBUG,"FBL Main init complete!\n");

	ApiLedControl(3,TRUE);//开3个灯，表示FBL状态
	
	// ApiStbcGetResetWakeupFactor();   /*获取唤醒源*/
    // u32ResetFac = ApiStbcGetResetFactor();   /*获取唤醒源*/
    // if (RESF_DEEP_RESET == u32ResetFac)
    // {
    //     ApiStbcIOHoldRelease();
    // }

    (void)ApiGpioOutputHigh(GPIO_MCUSW_EN);

   
	ApiLogProcess();
	
	// (void)ApiTimerTaubInit();
	// (void)ApiTimerTaubStart();   /*开启taub0，需修改，关闭中断只计数,还需根据需求配置Up/down count mode*/

	/*看门狗引脚初始化*/
	// ApiWdtHwtInit();
	// ApiWdtHwtEnable();

   	// s32Ret = ApiDflashInit();            /*dflash初始化TODO:*/
	// if (s32Ret == ERROR)
	// {
	// 	ApiLogPrint(_LOG_CRITICAL, "BspInit:eepIniErr\n");
	// 	delayms(2);
	//     ApiLogProcess();
	// }
	// else
	{
		s32Ret = ApiNvramReadSyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN,  &u8InitData[0]);
		if(s32Ret == FALSE)
		{
			ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8InitData[0]);
		}
	}

	// (void)ApiWdtSwtInit();            /*内部狗使能*/
	// ApiWdtSwtFeedDog();               /*喂狗*/

	

	PortInit();
}


/******************************************************************************
*  function name | ApiLlGetSecuritySeed
*  content       | 获取密钥种子
*  parameter     | oldSeed：上一次的种子
*  return        | uint32
*  notice        | 无
******************************************************************************/
UINT32 ApiLlGetSecuritySeed(UINT32 u32OldSeed)
{
    UINT32 u32Seed = 0u;
    if(tSemaphores.bSaFirstSeed == TRUE)
    {
        tSemaphores.bSaFirstSeed = FALSE;
        /* Get a random number from somewhere */
        // u32Seed = ( (UINT32) ((TAUB0.CNT0 & 0x1234) | (TAUB0.CNT0 << 8)) );//TODO:定时器
		u32Seed=rand();
    }
    else
    {
        u32Seed = (u32OldSeed * 0x41C64E6DuL) + 0x3039uL;
    }
    /* never return 0 seed in a currently locked session (ISO 14229) */
    if(u32Seed == 0u)
    {
        u32Seed = 0x41C64E6DuL;
    }
    return u32Seed;
}


/******************************************************************************
*  function name | ApiLlFill
*  content       | 填充
*  parameter     | dst：目标地址，value：填充值，count：填充长度
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiLlFill(UINT8 *pu8Dst, UINT8 u8Value, UINT16 u16Count)
{
    while(u16Count != 0u)
    {
        u16Count--;
        *pu8Dst++ = u8Value;
    }
}


/******************************************************************************
*  function name | ApiLlApplicationPrestart
*  content       | 停止定时器计数
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiLlApplicationPrestart(void)
{
   (void)ApiTimerTaubStop();//TODO:定时器
}


/******************************************************************************
*  function name | ApiLlJudgeBatteryVoltage
*  content       | 判断电压值
*  parameter     | void
*  return        | battery_type
*  notice        | 无
******************************************************************************/
BatteryType_e ApiLlJudgeBatteryVoltage(void)
{
    INT32 s32Voltage=0;
    /*Start conversion*/
	s32Voltage = ApiAdcVoltRead(6);   /*ADC_CHANNEL_6读取电池电压ADC值*/

    if (s32Voltage < BATT_9V)         /*lower limit*/
    {
        return (battVoltageTooLow);
    }
    else if (s32Voltage > BATT_16V)   /*upper limit*/
    {
        return (battVoltageTooHigh);
    }
    else
    {
        /* Battery Voltage measurement: ok */
        return (batteryOk);
    }
}


/******************************************************************************
*  function name | ApiLlMpuPowerOn
*  content       | 模组开机
*  parameter     | void
*  return        | bool
*  notice        | 无
******************************************************************************/
BOOL ApiLlMpuPowerOn(void)
{
	static UINT8 u8Cnt = 0;
	BOOL bRet = FALSE;

	if(ApiGpioInputRead(GPIO_4G_STATUS) == 0)
	{
		return TRUE;
	}

	if(0 == u8Cnt)
	{
		(void)ApiGpioOutputLow(GPIO_PWR_CTL_4G);
		ApiInitTimer(mpu_timer_handle,MPU_TO);
		u8Cnt = 1;
	}
	else
	{
		(void)ApiGpioOutputHigh(GPIO_PWR_CTL_4G);
		if(expired == ApiCheckTimer(mpu_timer_handle))
		{
			(void)ApiGpioOutputLow(GPIO_PWR_CTL_4G);
			ApiStopTimer(mpu_timer_handle);
			u8Cnt = 0;
			bRet = TRUE;
		}
	}
	return bRet;
}


/******************************************************************************
*  function name | ApiSetResetSource
*  content       | 模组开机
*  parameter     | void
*  return        | bool
*  notice        | 无
******************************************************************************/
void ApiSetResetSource(void)
{
    UINT16 u16ResetSource;
    UINT8 u8ResetSourceDid[4];

    /* get stored reset source - do not handle read error */

	// (void)ApiNvramReadSyncInd(0x0008,2,0,&u8ResetSourceDid[0]);

    // /* get current reset source */
    // u16ResetSource = (UINT16)ApiStbcGetResetFactor();

    // /* should store in non-volatile memory */
    // if((u8ResetSourceDid[0] != (u16ResetSource >> 8)) || (u8ResetSourceDid[1] != (UINT8)u16ResetSource))
    // {
	// 	(void)ApiNvramWritAsyncInd(0x0008,4,&u8ResetSourceDid[0]);
    // }
    // else
    // {
    //     /* do not modify if not changed */
    // }
}



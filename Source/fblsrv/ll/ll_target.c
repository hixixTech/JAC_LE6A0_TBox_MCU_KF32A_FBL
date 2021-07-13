/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ll_target.c                                                 */
/* Description:  Target dependent source file                                */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** �޸���ʷ *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     �»�        ���ߴ���                              */
/* 2019-10-24   V11.01.01.01     �»�        ����Bootģʽֻ����������ˢ��ʱ���룬ɾ����ʼ���������*/
/* 2019-12-30   V11.01.01.02     �»�        ɾ���帴λԴ����*/
/* 2020-03-11   V11.01.01.03     �»�        ����MPU�����ͱ��ִ���*/
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* ͷ�ļ�                                                                    */
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
/* ���Ͷ���                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �궨��                                                                    */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �ⲿ���ձ���                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �ڲ���������                                                              */
/*---------------------------------------------------------------------------*/
static void PortInit(void);


/******************************************************************************
*  function name | port_init
*  content       | port��ʼ��
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
static void PortInit(void)
{
	/* Initalize ports */
	// (void)ApiGpioOutput(GPIO_BAT_DET_CTL);        /*����VBAT���ʹ�ܽ�Ϊ���*/
	(void)ApiGpioOutputHigh(GPIO_BAT_DET_CTL);    /*����VBAT���ʹ�ܽ�Ϊ��*/

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
*  content       | ��һ����ַcopy����һ����ַ(һ�����ڴ�flash��copy��ram)
*  parameter     | dest��Ŀ���ַ��src�����ݵ�ַ��count������
*  return        | ��
*  notice        | ��
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
*  content       | ʱ�������ȡ��1ms������ÿ��⵽1ms��1
*  parameter     | void
*  return        | timer_o��1-expired��0-not_expired
*  notice        | ��
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
*  content       | ��λ
*  parameter     | void
*  return        | void
*  notice        | ��
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
*  content       | �弶Ӳ����ʼ��
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiLlEcuInit(void)
{
	UINT32 u32ResetFac;
	INT32 s32Ret ;

	UINT8 u8InitData[256] = {DATA_0xFF_256bytes};

	// __DI();                 /*�ر��ж�*/
	SystemInit(); //ϵͳʱ��96M,�����Ƶʱ��16M

	ApiLogInit();
	ApiLogEnable();
	
	bsw_init();
	app_init();
	Mem_EE_Init();
	
	ApiLogPrint(_LOG_DEBUG,"FBL Main init complete!\n");

	ApiLedControl(3,TRUE);//��3���ƣ���ʾFBL״̬
	
	// ApiStbcGetResetWakeupFactor();   /*��ȡ����Դ*/
    // u32ResetFac = ApiStbcGetResetFactor();   /*��ȡ����Դ*/
    // if (RESF_DEEP_RESET == u32ResetFac)
    // {
    //     ApiStbcIOHoldRelease();
    // }

    (void)ApiGpioOutputHigh(GPIO_MCUSW_EN);

   
	ApiLogProcess();
	
	// (void)ApiTimerTaubInit();
	// (void)ApiTimerTaubStart();   /*����taub0�����޸ģ��ر��ж�ֻ����,���������������Up/down count mode*/

	/*���Ź����ų�ʼ��*/
	// ApiWdtHwtInit();
	// ApiWdtHwtEnable();

   	// s32Ret = ApiDflashInit();            /*dflash��ʼ��TODO:*/
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

	// (void)ApiWdtSwtInit();            /*�ڲ���ʹ��*/
	// ApiWdtSwtFeedDog();               /*ι��*/

	

	PortInit();
}


/******************************************************************************
*  function name | ApiLlGetSecuritySeed
*  content       | ��ȡ��Կ����
*  parameter     | oldSeed����һ�ε�����
*  return        | uint32
*  notice        | ��
******************************************************************************/
UINT32 ApiLlGetSecuritySeed(UINT32 u32OldSeed)
{
    UINT32 u32Seed = 0u;
    if(tSemaphores.bSaFirstSeed == TRUE)
    {
        tSemaphores.bSaFirstSeed = FALSE;
        /* Get a random number from somewhere */
        // u32Seed = ( (UINT32) ((TAUB0.CNT0 & 0x1234) | (TAUB0.CNT0 << 8)) );//TODO:��ʱ��
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
*  content       | ���
*  parameter     | dst��Ŀ���ַ��value�����ֵ��count����䳤��
*  return        | void
*  notice        | ��
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
*  content       | ֹͣ��ʱ������
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiLlApplicationPrestart(void)
{
   (void)ApiTimerTaubStop();//TODO:��ʱ��
}


/******************************************************************************
*  function name | ApiLlJudgeBatteryVoltage
*  content       | �жϵ�ѹֵ
*  parameter     | void
*  return        | battery_type
*  notice        | ��
******************************************************************************/
BatteryType_e ApiLlJudgeBatteryVoltage(void)
{
    INT32 s32Voltage=0;
    /*Start conversion*/
	s32Voltage = ApiAdcVoltRead(6);   /*ADC_CHANNEL_6��ȡ��ص�ѹADCֵ*/

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
*  content       | ģ�鿪��
*  parameter     | void
*  return        | bool
*  notice        | ��
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
*  content       | ģ�鿪��
*  parameter     | void
*  return        | bool
*  notice        | ��
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



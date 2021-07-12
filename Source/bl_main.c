/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    main.c                                                      */
/* Description:  主函数入口                                                       */
/*                                                                           */
/* Others:       Implementation of main function of the Bootloader Layer     */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.01.00  陈惠            基线创建                          */
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/* 头文件                                                                       */
/*---------------------------------------------------------------------------*/
#include "system_init.h"
#include "bl_config_pc.h"
#include "dl_engine.h"
#include "dl_service.h"
#include "bl_timer.h"
#include "bl_app.h"
#include "ecu_can.h"
// #include "mcu_drv_wdg.h"
#include "ll_target.h"
#include "spp_app.h"
#include "spp_main.h"

#include "bsw_init.h"

#include "os_log.h"
/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define APP_ADDRESS (0x38000)

/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/
static BOOL bJudgeMpuFlag = 0;


typedef  void (*pFunction)(void); //定义void函数指针类型，
pFunction Jump_To_Application; //它可以接受任何类型函数的赋值
uint32_t JumpAddress;
/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/
static void GlobalInit(void);
static void InitializeTimers(void);

/****************************************************************************/
/**
 * Function Name: JumpToApp
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  create this function
 ****************************************************************************/

static void JumpToApp()
{
	if(*(volatile uint32_t*)APP_ADDRESS == 0x10020000 ) //判断APP空间的首个字内容是否为栈顶地址0x10020000，正确则说明APP程序已经写入
	{
	SYS_VECTOFF = APP_ADDRESS ; //设置向量表偏移值，即重映射向量表，这对中断服务函数正确执行至关重要
	JumpAddress = *(volatile uint32_t*) (APP_ADDRESS + 4); //获取APP的startup()入口地址
	Jump_To_Application = (pFunction) JumpAddress; //将startup()入口地址赋值给函数指针
	
	Jump_To_Application(); //使用新的函数指针，转向执行APP的startup()函数，这将导致APP程序启动
	}
}
/******************************************************************************
*  function name | GlobalInit
*  content       | 初始化BSP等
*  parameter     | 无
*  return        | 无
*  notice        | 无
******************************************************************************/
static void GlobalInit(void)
{
    ApiLlFill((UINT8 *)&tSemaphores,0, (UINT16)sizeof(Semaphore_t)); /*将服务信号量全部清零*/

    tSemaphores.bLocked = true;                       /*锁信号量置为true*/
    tSemaphores.bSaFirstSeed = true;                 /*sa密钥第一次交换信号量置为true*/

    ApiLlEcuInit();

	// (void)ApiCanInit();  /*初始化can*/

	(void)SppInitForAll();
	// (void)SPP_Add_DCPD_MSG_TO_MCU_FOTA_FBL_COM_Event_Listener(ApilLlFblFotaComMsgCb);
    SppSetListener(SPP_CONNECT_TYPE_MCU_4G,SPP_USER_RECV_DCPD_MSG_TO_MCU_FOTA_FBL_COM_EVT, ApilLlFblFotaComMsgCb);
	SppSetListener(SPP_CONNECT_TYPE_MCU_4G,0x10, ApilLlFblFotaOtherEcuCb);
	SppInit(SPP_CONNECT_TYPE_MCU_4G);
    // (void)SPP_Start();

	// __EI();   /*暂时打开中断，后续等can报文改为查询时关闭*/
}


/******************************************************************************
*  function name | InitializeTimers
*  content       | main initialization function for the timers
*  parameter     | 无
*  return        | 无
*  notice        | 无
******************************************************************************/
static void InitializeTimers(void)                  
{
    ApiInitTimer(startup_timer_handle,STARTUP_TO);
    ApiInitTimer(S3_timer_handle,S3SERVER_TO);
    ApiInitTimer(wdg_timer_handle,WDG_TO);
    ApiInitTimer(NCr_timer_handle,NCr_Timeout);
    ApiInitTimer(NBs_timer_handle,NBs_Timeout);
    ApiInitTimer(securityDelay_timer_handle,SECURITYDELAY_TO);

    ApiStopTimer(NCr_timer_handle);
    ApiStopTimer(NBs_timer_handle);
}


/*---------------------------------------------------------------------------*/
/* 外部函数定义                                                              */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | main
*  content       | 初始化BSP、OS，创建启动任务，开启操作系统
*  parameter     | 无
*  return        | 无
*  notice        | 无
******************************************************************************/
void main(void)
{
    /*** init ***/
    GlobalInit();              /*硬件初始化*/

    InitializeTimers();
    /*switch to programming session if the application set the flag */
    ApiDlAppSwitchToProgramming();

    for (;;)
    {
    	SppMainTask(SPP_CONNECT_TYPE_MCU_4G);
        ApiDlDiagnosticFgTask();          /*处理can报文信息*/
        ApiDlDiagnosticBgTask();
        ApiTickTimers();
		// ApiWdtSwtFeedDog();         /*周期喂狗*/
		// ApiWdtHwtFeedDog();
        JumpToApp();//TEST
        if (tSemaphores.bProgrammingSession == false)  /*如果不是刷新会话，进行校验*/
        {
            ApiApplicationStart();
        }
		if(1 == gu8InvalidateFlag)
		{
			if(TRUE == ApiLlMpuPowerOn())
			{
				gu8InvalidateFlag = 0;
				bJudgeMpuFlag = 1;
			}	
        }
    }
}


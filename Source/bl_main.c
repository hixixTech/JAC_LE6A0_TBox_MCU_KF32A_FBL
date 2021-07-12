/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    main.c                                                      */
/* Description:  ���������                                                       */
/*                                                                           */
/* Others:       Implementation of main function of the Bootloader Layer     */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** �޸���ʷ *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.01.00  �»�            ���ߴ���                          */
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/* ͷ�ļ�                                                                       */
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
/* ���Ͷ���                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �궨��                                                                    */
/*---------------------------------------------------------------------------*/
#define APP_ADDRESS (0x38000)

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/
static BOOL bJudgeMpuFlag = 0;


typedef  void (*pFunction)(void); //����void����ָ�����ͣ�
pFunction Jump_To_Application; //�����Խ����κ����ͺ����ĸ�ֵ
uint32_t JumpAddress;
/*---------------------------------------------------------------------------*/
/* �ⲿ���ձ���                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* �ڲ���������                                                              */
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
	if(*(volatile uint32_t*)APP_ADDRESS == 0x10020000 ) //�ж�APP�ռ���׸��������Ƿ�Ϊջ����ַ0x10020000����ȷ��˵��APP�����Ѿ�д��
	{
	SYS_VECTOFF = APP_ADDRESS ; //����������ƫ��ֵ������ӳ������������жϷ�������ȷִ��������Ҫ
	JumpAddress = *(volatile uint32_t*) (APP_ADDRESS + 4); //��ȡAPP��startup()��ڵ�ַ
	Jump_To_Application = (pFunction) JumpAddress; //��startup()��ڵ�ַ��ֵ������ָ��
	
	Jump_To_Application(); //ʹ���µĺ���ָ�룬ת��ִ��APP��startup()�������⽫����APP��������
	}
}
/******************************************************************************
*  function name | GlobalInit
*  content       | ��ʼ��BSP��
*  parameter     | ��
*  return        | ��
*  notice        | ��
******************************************************************************/
static void GlobalInit(void)
{
    ApiLlFill((UINT8 *)&tSemaphores,0, (UINT16)sizeof(Semaphore_t)); /*�������ź���ȫ������*/

    tSemaphores.bLocked = true;                       /*���ź�����Ϊtrue*/
    tSemaphores.bSaFirstSeed = true;                 /*sa��Կ��һ�ν����ź�����Ϊtrue*/

    ApiLlEcuInit();

	// (void)ApiCanInit();  /*��ʼ��can*/

	(void)SppInitForAll();
	// (void)SPP_Add_DCPD_MSG_TO_MCU_FOTA_FBL_COM_Event_Listener(ApilLlFblFotaComMsgCb);
    SppSetListener(SPP_CONNECT_TYPE_MCU_4G,SPP_USER_RECV_DCPD_MSG_TO_MCU_FOTA_FBL_COM_EVT, ApilLlFblFotaComMsgCb);
	SppSetListener(SPP_CONNECT_TYPE_MCU_4G,0x10, ApilLlFblFotaOtherEcuCb);
	SppInit(SPP_CONNECT_TYPE_MCU_4G);
    // (void)SPP_Start();

	// __EI();   /*��ʱ���жϣ�������can���ĸ�Ϊ��ѯʱ�ر�*/
}


/******************************************************************************
*  function name | InitializeTimers
*  content       | main initialization function for the timers
*  parameter     | ��
*  return        | ��
*  notice        | ��
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
/* �ⲿ��������                                                              */
/*---------------------------------------------------------------------------*/
/******************************************************************************
*  function name | main
*  content       | ��ʼ��BSP��OS�������������񣬿�������ϵͳ
*  parameter     | ��
*  return        | ��
*  notice        | ��
******************************************************************************/
void main(void)
{
    /*** init ***/
    GlobalInit();              /*Ӳ����ʼ��*/

    InitializeTimers();
    /*switch to programming session if the application set the flag */
    ApiDlAppSwitchToProgramming();

    for (;;)
    {
    	SppMainTask(SPP_CONNECT_TYPE_MCU_4G);
        ApiDlDiagnosticFgTask();          /*����can������Ϣ*/
        ApiDlDiagnosticBgTask();
        ApiTickTimers();
		// ApiWdtSwtFeedDog();         /*����ι��*/
		// ApiWdtHwtFeedDog();
        JumpToApp();//TEST
        if (tSemaphores.bProgrammingSession == false)  /*�������ˢ�»Ự������У��*/
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


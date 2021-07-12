/******************************************************************************
 * (C) Copyright 2020 YuChiZhiNeng-Automotive
 * FILE NAME:    bsw_init.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   
 * BY:           
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

/*****************************************************************************
** #include 
*****************************************************************************/
#include "bsw_init.h"

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
 * Function Name: bsw_init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  create this function
 ****************************************************************************/
void bsw_init(void)
{

    //ecu part
    ApiGpioInit();

	ApiUsartDebugInit();
	ApiUsartBleInit();
	ApiUsartMpuInit();
	
	ApiI2cInit();
	ApiAdc0Init();
	ApiSpi3Init();

	ApiCan0Init();
	ApiCan1Init();
	ApiCan2Init();
	ApiSoftWdtInit();

	ApiGeneralTime0Init();
	ApiGeneralTime1Init();
	ApiGeneralTime2Init();
	ApiGeneralTime3Init();
	ApiGeneralTime4Init();

	ApiDmaMpuRxInit(ApiSpi3GetRxBuffAddr(),SppGetMpuBuffAddr());
    //srv part
	Mem_EE_Init();

	(void)ApiGpioOutputLow(GPIO_CAN0_STB);
	(void)ApiGpioOutputLow(GPIO_CAN1_STB);
	(void)ApiGpioOutputLow(GPIO_CAN2_STB);
	(void)ApiGpioOutputHigh(GPIO_MCUSW_EN);
    (void)ApiGpioOutputHigh(GPIO_NMBAT_CHRG);
    (void)ApiGpioOutputHigh(GPIO_BAT_DET_CTL);
    (void)ApiGpioOutputHigh(GPIO_AUX_BOX_MUTE);
    (void)ApiGpioOutputHigh(GPIO_SPK_BOX_MUTE);
    (void)ApiGpioOutputHigh(GPIO_SOSLED_CTL);
    (void)ApiGpioOutputHigh(GPIO_CODEC_EN);
    (void)ApiGpioOutputHigh(GPIO_4G_VBUS_CNTL);
    (void)ApiGpioOutputHigh(GPIO_GSENSOR_EN);
    (void)ApiGpioOutputHigh(GPIO_DIV8_EN);
    (void)ApiGpioOutputHigh(GPIO_MIC_EN);
	(void)ApiGpioOutputHigh(GPIO_PWR_CTL_4G);
}
/*****************************************************************************


/*****************************************************************************
** End File
*****************************************************************************/

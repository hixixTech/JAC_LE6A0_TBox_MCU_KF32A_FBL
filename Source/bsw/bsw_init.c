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
    Ecu_Gpio_Init();

	ApiUsartDebugInit();
	ApiUsartBleInit();
	ApiUsartMpuInit();
	
	ApiI2cInit();
	ApiAdc0Init();

	ApiCan0Init();
	ApiCan1Init();
	ApiCan2Init();

	ApiDmaMpuRxInit(ApiGetRegAddrFormMpuRx(),SppGetMpuBuffAddr());
	
    //srv part
	Mem_EE_Init();

	ApiGpioOutputLow(GPIO_CAN0_STB);
	ApiGpioOutputLow(GPIO_CAN1_STB);
	ApiGpioOutputLow(GPIO_CAN2_STB);
	ApiGpioOutputHigh(GPIO_MCUSW_EN);
    ApiGpioOutputHigh(GPIO_NMBAT_CHRG);
    ApiGpioOutputHigh(GPIO_BAT_DET_CTL);
    ApiGpioOutputHigh(GPIO_AUX_BOX_MUTE);
    ApiGpioOutputHigh(GPIO_SPK_BOX_MUTE);
    // ApiGpioOutputHigh(GPIO_SOSLED_CTL);
    ApiGpioOutputHigh(GPIO_CODEC_EN);
    ApiGpioOutputHigh(GPIO_4G_VBUS_CNTL);
    ApiGpioOutputHigh(GPIO_GSENSOR_EN);
    ApiGpioOutputHigh(GPIO_DIV8_EN);
    ApiGpioOutputHigh(GPIO_MIC_EN);
	ApiGpioOutputHigh(GPIO_PWR_CTL_4G);
	

}
/*****************************************************************************


/*****************************************************************************
** End File
*****************************************************************************/

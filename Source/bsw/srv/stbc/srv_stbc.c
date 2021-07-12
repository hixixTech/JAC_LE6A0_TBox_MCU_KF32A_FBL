/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    srv_sdbc.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/07/06
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
#include "ecu_gpio.h"
#include "srv_stbc.h"
#include "srv_nvram.h"
/*****************************************************************************
** #define
*****************************************************************************/
#define WAKEUP_LEN (4)

/*****************************************************************************
** typedef
*****************************************************************************/


/*****************************************************************************
** global variable
*****************************************************************************/


/*****************************************************************************
** static variables
*****************************************************************************/
static UINT8 s_u8_wakeup = 0;
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
 * Function Name: ApiStbcClrWakeupFactor
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/06, feifei.xu create this function
 ****************************************************************************/
void ApiStbcClrWakeupFactor(void)
{
    s_u8_wakeup = 0;
    (void)ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup);

}
/****************************************************************************/
/**
 * Function Name: ApiStbcGetWakeupFactor
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/06, feifei.xu create this function
 ****************************************************************************/
UINT32 ApiStbcGetWakeupFactor(void)
{
	if(TRUE == ApiNvramReadSyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
    {

    }
    else
    {
        s_u8_wakeup = 0;
    }

    return (UINT32)s_u8_wakeup;
}
/****************************************************************************/
/**
 * Function Name: ApiWakeUpSourceInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/06, feifei.xu create this function
 ****************************************************************************/
void ApiStbcSetWakeupSource(UINT32 u32_wakeup_source)
{
	if(MCU_WAKEUP_SOURCE_ACC == (u32_wakeup_source & MCU_WAKEUP_SOURCE_ACC))
	{
		ApiGpioSetWakeupSource(WAKEUP_ACC_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_SOS == (u32_wakeup_source & MCU_WAKEUP_SOURCE_SOS))
	{
		ApiGpioSetWakeupSource(WAKEUP_SOS_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_LTE == (u32_wakeup_source & MCU_WAKEUP_SOURCE_LTE))
	{
		ApiGpioSetWakeupSource(WAKEUP_4G_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_BLE == (u32_wakeup_source & MCU_WAKEUP_SOURCE_BLE))
	{
		ApiGpioSetWakeupSource(WAKEUP_BLE_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_SRS == (u32_wakeup_source & MCU_WAKEUP_SOURCE_SRS))
	{
		ApiGpioSetWakeupSource(WAKEUP_SRS_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_RTC == (u32_wakeup_source & MCU_WAKEUP_SOURCE_RTC))
	{
		ApiGpioSetWakeupSource(WAKEUP_RTC_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_LIS == (u32_wakeup_source & MCU_WAKEUP_SOURCE_LIS))
	{
		ApiGpioSetWakeupSource(WAKEUP_LIS_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_CAN0 == (u32_wakeup_source & MCU_WAKEUP_SOURCE_CAN0))
	{
		ApiGpioSetWakeupSource(WALEUP_CAN0_RX_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_CAN1 == (u32_wakeup_source & MCU_WAKEUP_SOURCE_CAN1))
	{
		ApiGpioSetWakeupSource(WAKEUP_CAM1_RX_SOURCE);
	}

	if(MCU_WAKEUP_SOURCE_CAN2 == (u32_wakeup_source & MCU_WAKEUP_SOURCE_CAN2))
	{
		ApiGpioSetWakeupSource(WAKEUP_CAM2_RX_SOURCE);
	}
}
/****************************************************************************/
/**
 * Function Name: ApiPmMcuReset
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/06, feifei.xu create this function
 ****************************************************************************/
void ApiStbcSoftReset(void)
{
	asm("RESET");
}

/****************************************************************************/
/**
 * Function Name: ApiStbcGetResetFactor
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/06, feifei.xu create this function
 ****************************************************************************/
UINT32 ApiStbcGetResetFactor(void)
{
	return RESF_POWER_ON;
}

/****************************************************************************/
/**
 * Function Name: ApiStbcClrResetFactor
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/06, feifei.xu create this function
 ****************************************************************************/
void ApiStbcClrResetFactor(void)
{

}
/****************************************************************************/
/**
 * Function Name: _EINT0_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT0_exception(void)
{

}
/****************************************************************************/
/**
 * Function Name: _EINT1_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT1_exception(void)
{
	
}
/****************************************************************************/
/**
 * Function Name: _EINT2_exception
 * Description: WAKEUP_ACC_SOURCE   (2)
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT2_exception(void)
{
	if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_2))
	{
        s_u8_wakeup = 0x01;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
}
/****************************************************************************/
/**
 * Function Name: _EINT3_exception
 * Description: WAKEUP_CAM1_RX_SOURCE   (3)
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT3_exception(void)
{
	if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_3))
	{
        s_u8_wakeup = 0x09;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
}
/****************************************************************************/
/**
 * Function Name: _EINT4_exception
 * Description: WAKEUP_SOS_SOURCE   (4)
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT4_exception(void)
{
	if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_4))
	{
        s_u8_wakeup = 0x02;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
}


/****************************************************************************/
/**
 * Function Name: _EINT9TO5_exception
 * Description: WALEUP_CAN0_RX_SOURCE   (5)
 *              WAKEUP_4G_SOURCE        (6)
 *              WAKEUP_BLE_SOURCE       (7)
 *              WAKEUP_RTC_SOURCE       (9)
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT9TO5_exception(void)
{

	if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_5))
	{
        s_u8_wakeup = 0x08;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
    if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_6))
	{
        s_u8_wakeup = 0x03;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
    if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_7))
	{
        s_u8_wakeup = 0x04;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
    if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_9))
	{
        s_u8_wakeup = 0x06;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
}
/****************************************************************************/
/**
 * Function Name: _EINT15TO10_exception
 * Description: WAKEUP_LIS_SOURCE   (10)
 *              WAKEUP_SRS_SOURCE   (11)
 *              
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT15TO10_exception(void)
{
    if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_10))
	{
        s_u8_wakeup = 0x07;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
    
    if(TRUE == INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_11))
	{
        s_u8_wakeup = 0x05;
        if(TRUE == ApiNvramWritAsyncInd(EEPID_WAKEUPSOURCE,0x01,s_u8_wakeup))
        {
            ApiStbcSoftReset();
        }
	}
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


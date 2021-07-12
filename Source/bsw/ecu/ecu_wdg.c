/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_wdg.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/07/07
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
#include "ecu_wdg.h"
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
static UINT16 u16_TaskFeedWtd = 0U;
static UINT8 u8_TaskFeedCount[MAX_FEED_WTD] = {0};
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
 * Function Name: ApiIWDTInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiSoftWdtInit(void)
{
    OSC_SCK_Source_Config(SCLK_SOURCE_INTHF);                       //选择INTHF作为系统时钟降频
    PM_Independent_Watchdog_Reset_Config(PERIPHERAL_OUTRST_STATUS); //使能独立看门狗退出复位状态
    PM_Internal_Low_Frequency_Enable(TRUE);                         //内部低频晶振使能
    IWDT_Overflow_Config (500);                                     //独立看门狗溢出值，取值为500
    IWDT_Prescaler_Config(IWDT_PRESCALER_64);                       //独立看门狗对INTLF的1：64预分频值
    OSC_SCK_Source_Config(SCLK_SOURCE_PLL);                         //选择pll作为系统时钟
}
/****************************************************************************/
/**
 * Function Name: ApiWdtSoftEnable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiSoftWdtEnable(void)
{
    IWDT_Enable(TRUE);
}
/****************************************************************************/
/**
 * Function Name: ApiSoftWdtDisable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiSoftWdtDisable(void)
{
    IWDT_Enable(FALSE);
}
/****************************************************************************/
/**
 * Function Name: ApiSoftFeedDog
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiSoftFeedDog(void)
{
    IWDT_Feed_The_Dog();
}
/****************************************************************************/
/**
 * Function Name: ApiWdtHwtEnable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
INT32 ApiHardWdttEnable(void)
{
    return ApiGpioOutputHigh(GPIO_DOG_EN);
}
/****************************************************************************/
/**
 * Function Name: ApiWdtHwtDisable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
INT32 ApiHardWdtDisable(void)
{
    return ApiGpioOutputLow(GPIO_DOG_EN);  
}
/****************************************************************************/
/**
 * Function Name: ApiWdtHwtFeedDog
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiHardWdtFeedDog(void)
{
    (void)ApiGpioOutputHigh(GPIO_WDI1);
    delayus(100);
    (void)ApiGpioOutputLow(GPIO_WDI1);
}
/****************************************************************************/
/**
 * Function Name: ApiTaskWtdCheck
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
UINT8 ApiTaskWtdCheck(void)
{
    UINT8 u8_i = 0;
    UINT8 u8_TaskFeedCheck =0;

    u8_TaskFeedCheck = (UINT8)(u16_TaskFeedWtd & TASK_FEED_WTD);
    u16_TaskFeedWtd = 0;

    for (u8_i = 0; u8_i < MAX_FEED_WTD; u8_i++)
    {
        if((UINT8)(0x01 << u8_i) != (u8_TaskFeedCheck & (UINT8)(0x01 << u8_i)))
        {
            u8_TaskFeedCount[u8_i]++;
        }
        else
        {
            u8_TaskFeedCount[u8_i] = 0;
        }

        if(u8_TaskFeedCount[u8_i] > WDT_FEED_CHECK_CNT)
        {
            return u8_i;
        }
    }
    return INBALID_FEED_WTD;
}
/****************************************************************************/
/**
 * Function Name: ApiTaskFeedWtd
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiTaskFeedWtd(UINT8 u8FeedWtd)
{   
    switch(u8FeedWtd)
    {
        case CAN_FEED_WTD:
            u16_TaskFeedWtd = (u16_TaskFeedWtd | 0x01U);
      	  break;

        case DTC_FEED_WTD:
            u16_TaskFeedWtd = (u16_TaskFeedWtd | 0x02U);
          break;

        case BLE_FEED_WTD:
            u16_TaskFeedWtd = (u16_TaskFeedWtd | 0x04U);
          break;

        case DCPD_FEED_WTD:
            u16_TaskFeedWtd = (u16_TaskFeedWtd | 0x08U);
          break;

        case APP_FEED_WTD:
            u16_TaskFeedWtd = (u16_TaskFeedWtd | 0x10U);
          break;

        case AUX_FEED_WTD:
            u16_TaskFeedWtd = (u16_TaskFeedWtd | 0x20U);
          break;

      	default:
          break;                  
    }
}

/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


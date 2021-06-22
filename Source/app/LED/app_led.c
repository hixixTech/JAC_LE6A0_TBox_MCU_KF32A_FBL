/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    app_led.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/05/26
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
#include "app_led.h"

/*****************************************************************************
** #define
*****************************************************************************/
#define LED_CONST_LIGHT (0)
#define LED_SLOW_LIGHT  (1)
#define LED_QUICK_LIGHT (2)


// #define DEBUG_STS
/*****************************************************************************
** typedef
*****************************************************************************/
typedef enum
{
    APP_CAN_LED = 0,
    APP_GPS_LED,
    APP_LTE_LED,
    APP_MAX_LED
}APP_MODEL_NAME_E;

typedef struct
{
    UINT8 u8_model_sts;
    UINT16 u16_slow_delay_cnt;
	UINT16 u16_quick_delay_cnt;
}APP_MODEL_S;
/*****************************************************************************
** global variable
*****************************************************************************/
APP_MODEL_S app_model[APP_MAX_LED];
/*****************************************************************************
** static variables
*****************************************************************************/

/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/
static UINT8 App_Led_GetModelGpio(UINT8 u8_index);
static void App_Led_SlowLight(UINT8 u8_index,UINT16* u16_delay,const UINT16 c_u16_time);
/*****************************************************************************
** function prototypes
*****************************************************************************/
void App_Led_init(void)
{
    //
}
void App_Led_Handler10ms(void)
{

	UINT16 u16_i = 0;

    app_model[APP_CAN_LED].u8_model_sts = 0;
	app_model[APP_GPS_LED].u8_model_sts = 0;
	app_model[APP_LTE_LED].u8_model_sts = 0;
	
	for(u16_i = 0 ;u16_i < APP_MAX_LED; u16_i++)
	{
		switch (app_model[u16_i].u8_model_sts)
		{
			case LED_CONST_LIGHT:
                app_model[u16_i].u16_slow_delay_cnt = 0X00;
                app_model[u16_i].u16_quick_delay_cnt = 0X00;
				(void)GPIO_OUTPUT_HIGH(App_Led_GetModelGpio(u16_i));
				break;

			case LED_SLOW_LIGHT:
                app_model[u16_i].u16_quick_delay_cnt = 0X00;
				App_Led_SlowLight(App_Led_GetModelGpio(u16_i),&app_model[u16_i].u16_slow_delay_cnt,300);
				break;

			case LED_QUICK_LIGHT:
                app_model[u16_i].u16_slow_delay_cnt = 0X00;
				App_Led_SlowLight(App_Led_GetModelGpio(u16_i),&app_model[u16_i].u16_quick_delay_cnt,30);
				break;

			default:
				break;
		}
	}
}

static UINT8 App_Led_GetModelGpio(UINT8 u8_index)
{
#ifdef LCV_24V
	if(APP_CAN_LED == u8_index)
	{
		return GPIO_CAN_LED;
	}
	else if(APP_GPS_LED == u8_index)
	{
		return GPIO_GPS_LED;
	}
	else
	{
		return GPIO_LTE_LED;
	}
#endif
}

static void App_Led_SlowLight(UINT8 u8_index,UINT16* u16_delay,const UINT16 c_u16_time)
{
	if(*u16_delay > 0)
	{
		(*u16_delay)--;
	}
	else
	{
		GPIO_OUTPUT_TURN(u8_index);
		*u16_delay = c_u16_time;
	}
}

void ApiLedControl(LED_NAME_E e_index,BOOL b_flag)
{
#ifdef LCV_24V
	switch (e_index)
	{
	case RED_LED:
		if(TRUE == b_flag)
		{
			GPIO_OUTPUT_HIGH(App_Led_GetModelGpio(APP_CAN_LED));
		}
		else
		{
			GPIO_OUTPUT_LOW(App_Led_GetModelGpio(APP_CAN_LED));
		}
		break;

	case YELLOW_LED:
		if(TRUE == b_flag)
		{
			GPIO_OUTPUT_HIGH(App_Led_GetModelGpio(APP_GPS_LED));
		}
		else
		{
			GPIO_OUTPUT_LOW(App_Led_GetModelGpio(APP_GPS_LED));
		}
		break;

	case GREEN_LED:
		if(TRUE == b_flag)
		{
			GPIO_OUTPUT_HIGH(App_Led_GetModelGpio(APP_LTE_LED));
		}
		else
		{
			GPIO_OUTPUT_LOW(App_Led_GetModelGpio(APP_LTE_LED));
		}
		break;

	case ALL_LED:
		if(TRUE == b_flag)
		{
			GPIO_OUTPUT_HIGH(App_Led_GetModelGpio(APP_CAN_LED));
			GPIO_OUTPUT_HIGH(App_Led_GetModelGpio(APP_GPS_LED));
			GPIO_OUTPUT_HIGH(App_Led_GetModelGpio(APP_LTE_LED));
		}
		else
		{
			GPIO_OUTPUT_LOW(App_Led_GetModelGpio(APP_CAN_LED));
			GPIO_OUTPUT_LOW(App_Led_GetModelGpio(APP_GPS_LED));
			GPIO_OUTPUT_LOW(App_Led_GetModelGpio(APP_LTE_LED));
		}
		break;
	
	default:
		break;
	}
#endif
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


/******************************************************************************
 * (C) Copyright 2020 YuChiZhiNeng-Automotive
 * FILE NAME:    rtos_taskinit.c
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
// #include "system_init.h"
#include "rtos_taskinit.h"
#include "bsw_init.h"
#include "ecu_i2c.h"


/*****************************************************************************
** #define
*****************************************************************************/


/*****************************************************************************
** typedef
*****************************************************************************/


/*****************************************************************************
** global variable
*****************************************************************************/
//任务句柄
// TaskHandle_t LEDTask_Handler;
// TaskHandle_t UARTTask_Handler;
// TaskHandle_t CANTask_Handler;

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
 * Function Name: 
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  create this function
 ****************************************************************************/
static void LED_task(void *pvParameters)//临时task 表示开发板心跳
{
	static BOOL flag = FALSE;
//	static portTickType xLastWakeTime_ble;
	UINT8 u8_data[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

	while (1)
	{

		if(flag)
		{
			flag = FALSE;
			ApiGpioOutputHigh(GPIO_SOSLED_CTL);
		}
		else
		{
			flag = TRUE;
			ApiGpioOutputLow(GPIO_SOSLED_CTL);
		}

		ApiLogPrint(_LOG_DEBUG,"This is Led_task!!\n");
		vTaskDelay(500);
	}
	
}
/****************************************************************************/
/**
 * Function Name: RTOS task init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  create this function
 ****************************************************************************/
void TaskInit(void)
{
	UINT8 A=0;
		//创建任务
	xTaskCreate((TaskFunction_t )CAN_task,
				(const char*    )"CAN_task",
				(uint16_t       )CAN_TASK_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )10,
				(TaskHandle_t*  )NULL);

	xTaskCreate((TaskFunction_t )DCPD_task,
				(const char*    )"DCPD_task",
				(uint16_t       )DCPD_TASK_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )NULL);
	
	xTaskCreate((TaskFunction_t )DTC_task,
				(const char*    )"DTC_task",
				(uint16_t       )DTC_TASK_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )7,
				(TaskHandle_t*  )NULL);
				
	xTaskCreate((TaskFunction_t )BLE_task,
				(const char*    )"BLE_task",
				(uint16_t       )BLE_TASK_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )6,
				(TaskHandle_t*  )NULL);
				
	xTaskCreate((TaskFunction_t )APP_task,
				(const char*    )"APP_task",
				(uint16_t       )APP_TASK_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )4,
				(TaskHandle_t*  )NULL);
				
	xTaskCreate((TaskFunction_t )SYS_task,
				(const char*    )"SYS_task",
				(uint16_t       )SYS_MGR_TASK_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )3,
				(TaskHandle_t*  )NULL);
			
	xTaskCreate((TaskFunction_t )AUX_task,
				(const char*    )"AUX_task",
				(uint16_t       )AUX_TASK_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )2,
				(TaskHandle_t*  )NULL);

	xTaskCreate((TaskFunction_t )NVRAM_task,
				(const char*    )"NVRAM_task",
				(uint16_t       )NVRAM_TASK_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )1,
				(TaskHandle_t*  )NULL);

	xTaskCreate((TaskFunction_t )LED_task,//开发板用，表示心跳
				(const char*    )"LED_task",
				(uint16_t       )configMINIMAL_STACK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )0,
				(TaskHandle_t*  )NULL);	
	
	ApiLogPrint(_LOG_ENTRY,"AllTaskStart!\n");		
				
}
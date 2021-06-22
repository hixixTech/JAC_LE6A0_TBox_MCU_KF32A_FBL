/******************************************************************************
 * (C) Copyright 2020 YuChiZhiNeng-Automotive
 * FILE NAME:    rtos_cantask.c
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
#include "rtos_taskinit.h"
#include "can_config.h"
#include "can_driver.h"
#include "eol_main.h"//临时
#include "type.h"

/*****************************************************************************
** #define
*****************************************************************************/


/*****************************************************************************
** typedef
*****************************************************************************/


/*****************************************************************************
** global variable
*****************************************************************************/
UINT8 u8gCanDataTx[8] = {1,2,3,4,5,6,7,8};
UINT8 u8gCanDataRx[8] = {0U};
UINT8 u8gCanBusoff = 0;

/*****************************************************************************
** static variables
*****************************************************************************/
static BOOL bSimInfoSt = FALSE;

static UINT32 s_u32_send_count = 0;
/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/
static void ApiComSrvInit(void);

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
void CanTaskTimeoutProc(void)
{

	static UINT8 u8MultiPeriod = 1;
	ApiCan0CheckBusOff();
	ApiCan1CheckBusOff();
	ApiCan2CheckBusOff();

	if(2 == u8MultiPeriod)
	{
		u8MultiPeriod = 1;

		ApiDcsWr8(TBOX_AC_RightSetTem, 0xDD);
		ApiDcsWr8(TBOX_AC_LeftSetTem, 0xDD);
		ApiDcsWr8(TBOX_HVAC_SetTem,0xDD);
	}
	else
	{
		u8MultiPeriod++;
	}
	ApiDcsOutput(&dcs_controllConfig_DIAG);

	// ApiCan0Send(0x110,8,u8gCanDataTx);
	// ApiCan0Send(0x111,8,u8gCanDataTx);
	// ApiCan0Send(0x112,8,u8gCanDataTx);
	// ApiCan0Send(0x113,8,u8gCanDataTx);
	// ApiCan0Send(0x114,8,u8gCanDataTx);
	// ApiCan0Send(0x115,8,u8gCanDataTx);

	// ApiCan1Send(0x110,8,u8gCanDataTx);
	// ApiCan2Send(0x110,8,u8gCanDataTx);
	// s_u32_send_count++;

}
/****************************************************************************/
/**
 * Function Name: 
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  create this function
 ****************************************************************************/
void CAN_task(void)
{
	static portTickType xLastWakeTime_CAN = 0;
	xLastWakeTime_CAN = xTaskGetTickCount();
	ApiLogPrint(_LOG_DEBUG,"This is CAN_task\n");
	RUNTASK
	{
		vTaskDelayUntil(&xLastWakeTime_CAN,5);//cantask 5ms
		CanTaskTimeoutProc();
	}
}

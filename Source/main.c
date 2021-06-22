/******************************************************************************
 * (C) Copyright 2020 YuChiZhiNeng-Automotive
 * FILE NAME:    main.c
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
#include "system_init.h"
#include "os_log.h"

/* RTOS系统运行所需头文件 */

#include "ymodem.h"


/*****************************************************************************
** #define
*****************************************************************************/
#define APP_ADDRESS (0x38000)

/*****************************************************************************
** typedef
*****************************************************************************/


/*****************************************************************************
** global variable
*****************************************************************************/


/*****************************************************************************
** static variables
*****************************************************************************/
typedef  void (*pFunction)(void); //定义void函数指针类型，
pFunction Jump_To_Application; //它可以接受任何类型函数的赋值
uint32_t JumpAddress;
uint8_t tab_133[PACKET_SIZE + PACKET_OVERHEAD] =  { 0 };

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

/****************************************************************************/
/**
 * Function Name: main()
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  create this function
 ****************************************************************************/

void main()
{
	UINT64 u64Count=0;
	
	SystemInit(); //系统时钟96M,外设高频时钟16M

	ApiLogInit();
	ApiLogEnable();
	
	bsw_init();
	app_init();
	
	ApiLogPrint(_LOG_DEBUG,"FBL Main init complete!\n");

	ApiLedControl(3,TRUE);//开3个灯，表示FBL状态
	
	
	// TaskInit();
	// vTaskStartScheduler();
	
	
	for (;;)
	{
		u64Count=Ymodem_Receive(tab_133);
		if(u64Count>99)
		{
			JumpToApp();
		}		
		// else if(u64Count>=5000)
		// {
		// 	ApiLedControl(2,FALSE);
		// 	u64Count++;
		// }
		// else
		// {
		// 	u64Count++;
		// }
	}
}

/****************************************************************************/
/**
 * Function Name:check_failed 
 * Description: 报告校验发生错误的文件和行
 *
 * Param:   file: 指向文件名称的指针
 * 			line： 校验错误的行
 * Return:  none
 * Author:  create this function
 ****************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* 用户可以添加自己的代码实现报告文件名和行号,
	             例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	while (1)
	{
		fprintf(USART2_STREAM, "Wrong parameters value: file %s on line %d\r\n", file, line);
	}
}

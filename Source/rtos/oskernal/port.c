/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*-----------------------------------------------------------
 * 为了实现porttable.h的功能而定义了KF32接口
 *----------------------------------------------------------*/
// #include "uart.h"	//debug
/* Scheduler includes. */
/*头文件*/
#include "FreeRTOS.h"
#include "task.h"

uint32_t RTOSSYSTEM_ERROR;

#if configMAX_SYSCALL_INTERRUPT_PRIORITY == 0
	#error configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to 0.
#endif

#ifndef configSYSTICK_CLOCK_HZ
	#define configSYSTICK_CLOCK_HZ configCPU_CLOCK_HZ
	/* 确保RTOS系统时钟与CPU频率相同。*/
	#define portNVIC_SYSTICK_CLK_BIT	( 1UL << 2UL )
#else
	/*在不与核心相同的情况下，不修改SysTick定时。*/
	#define portNVIC_SYSTICK_CLK_BIT	( 0 )
#endif

/* 如果开发者想要用自己的vPortSetupTimerInterrupt函数，将其置1，否则默认定义为0
 * */
#ifndef configOVERRIDE_DEFAULT_TICK_CONFIGURATION
	#define configOVERRIDE_DEFAULT_TICK_CONFIGURATION 0
#endif

/* 核心寄存器*/
#define portST_CTL_REG						( * ( ( volatile uint32_t * ) 0x402000A0 ) )//系统节拍定时器控制寄存器
#define portST_RELOAD_REG					( * ( ( volatile uint32_t * ) 0x402000A4 ) )//系统节拍定时器重加载寄存器
#define portST_CV_REG						( * ( ( volatile uint32_t * ) 0x402000A8 ) )//系统节拍定时器计数器
#define portINT_IP2_REG						( * ( ( volatile uint32_t * ) 0x4020003C ) )//中断优先级寄存器2（用于软件中断和节拍定时器中断）

/* 寄存器的比特位*/
#define portST_CTL_TICKINTEN				( 1UL << 1UL	)//节拍定时器中断使能
#define portST_CTL_STEN						( 1UL << 0UL	)//节拍定时器使能
#define portST_CTL_COUNTZERO				( 1UL << 16UL	)//定时器器计数到0时该位置1

//核心中断优先级
#define portINT_SOFTSV_PRI					( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 16UL )
#define portINT_SYSTICK_PRI					( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

/* 用于检验中断优先级正确性的常量 */
#define portFIRST_USER_INTERRUPT_NUMBER		( 16			)							//第一个用户中断
#define portINT_IP							( 0x40200030	)							//中断优先级控制0寄存器
#define portINT_CTL0						( * ( ( volatile uint32_t * ) 0x40200000 ) )//中断控制寄存器
#define portMAX_8_BIT_VALUE					( ( uint8_t ) 0xff	)						//8比特的最大值
#define portTOP_BIT_OF_BYTE					( ( uint8_t ) 0x80	)						//字节最高比特
#define portMAX_PRIGROUP_BITS				( ( uint8_t ) 3 	)						//最大优先级分组值
#define portPRIORITY_GROUP_MASK				( 0x03UL << 1UL		)						//优先级分组掩码
#define portPRIGROUP_SHIFT					( 1UL 				)						//优先级分组移位

/* 建立初始化栈命令*/
#define portINITIAL_EXEC_RETURN		( 0xfffffffd )

/* 24位计数的系统节拍*/
#define portMAX_24_BIT_NUMBER		( 0xffffffUL )

/* 当SysTick计数在tickless空闲计算时停止时，一个fiddle参数去估计SysTick计数值*/
#define portMISSED_COUNTS_FACTOR	( 45UL )

/* 启动地址掩码，当它从中断中载入到PC中时，0位清0*/
#define portSTART_ADDRESS_MASK		( ( StackType_t ) 0xfffffffeUL )

/* 在临界嵌套变量，使得每个任务保持它们自己的中断状态*/
volatile static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

/* 设置定时器，产生节拍中断。*/
void vPortSetupTimerInterrupt( void );

/* 开始第一个任务：与其它函数分离，能够单独进行*/
static void prvStartFirstTask( void );

/* 任务错误退出函数，可以在开发者错误退出任务时调用。*/
static void prvTaskExitError( void );

/*-----------------------------------------------------------*/

/* 组成一个节拍周期下的SysTick增长的数量*/
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t ulTimerCountsForOneTick = 0;
#endif /* configUSE_TICKLESS_IDLE */

/*24比特SysTick定时器的的最大周期数*/
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t xMaximumPossibleSuppressedTicks = 0;
#endif /* configUSE_TICKLESS_IDLE */

/* 当SysTick停止时(仅低功耗)传递补偿给CPU周期*/
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t ulStoppedTimerCompensation = 0;
#endif /* configUSE_TICKLESS_IDLE */

/* 在portASSERT_IF_INTERRUPT_PRIORITY_INVALID()宏之前去使用，
 * 以确保FreeRTOS的API函数不会使用分配的优先级比最大值
 * (configMAX_SYSCALL_INTERRUPT_PRIORITY)还要大的中断*/
#if ( configASSERT_DEFINED == 1 )
	/* 最大系统调用优先级*/
	static uint8_t ucMaxSysCallPriority = 0;
	/* 最大优先级分组值*/
	static uint32_t ulMaxPRIGROUPValue = 0;
	/* 中断优先级寄存器*/
	static const volatile uint8_t * const pcInterruptPriorityRegisters = ( uint8_t * ) portINT_IP;
#endif /* configASSERT_DEFINED */

/*-----------------------------------------------------------*/

/* 配置任务堆栈，调度控制器可通过该任务堆栈运行进程。通用寄存器会被放入该堆栈。
 * */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	/* xPSR */
	pxTopOfStack--;
	*pxTopOfStack = 0x0000ff00;

	/* 寄存器PC初始化为任务函数pxCode*/
	pxTopOfStack--;
	*pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK;	/* PC */

	/* 寄存器LR初始化为函数prvTaskExitError*/
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) prvTaskExitError;	/* LR */

	/* 寄存器R4,R3,R2,R1,R0*/
	pxTopOfStack -= 5;//跳过4个寄存器R4,R3,R2,R1
	*pxTopOfStack = ( StackType_t ) pvParameters;	/* R0 */

	/*保存EXC_RETURN值，用于设置退出SVC和SoftSV中断的时候处理器应该处于什么状态。处理器进入异常后中断
	 * 服务程序（ISR）时，链接寄存器R14(LR)的数值会被更新为EXC_RETURN数值，之后该数值会在异常处理结束
	 * 时触发异常返回。这里人为的设置为0xFFFFFFFD，表示退出异常以后CPU进入线程模式并且使用进程栈！*/
	pxTopOfStack--;
	*pxTopOfStack = portINITIAL_EXEC_RETURN;/* 0xFFFF FFFD */

	/* 跳过R12,R11,R10,R9,R8,R7,R6,R5*/
	pxTopOfStack -= 8;
	return pxTopOfStack;
}
/*-----------------------------------------------------------*/
//任务错误退出
/*
 * 通常情况下，开发者的任务函数不能退出，要返回前也必须使用vTaskDelete(NULL)函数。
 * 在configASSERT()被定义后，自定一个assert()函数，这样就可以停在这个函数内，开发者可以在这个函数内获取错误信息。
 * */
static void prvTaskExitError( void )
{
	configASSERT( uxCriticalNesting == ~0UL );
	portDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/
/* SVC中断服务函数*/
/* 请求管理调用，仅用于第一个任务，后面用不到SVC了。
 * */
void __attribute__((interrupt)) _SVC_exception (void)
{
	/* SVC中断流程介绍
	 * 1、将第一个任务的R5-R13推出
	 * 2、将指针切换为第一个任务的堆栈
	 * 3、更改中断屏蔽，以使能中断
	 * */
	//************ 中断程序    *******************************
	/*1、将第一个任务的R5-R13推出*/
	asm(".extern pxCurrentTCB"			);
	asm("MOV R3,#pxCurrentTCB"			);
	asm("LD.w R1,[R3]"					);//将R3指向地址的值给R1
	asm("LD.w R0,[R1]"					);
	asm("LD.w R5,[R0++]"				);//这里是将R0指向的地址的值给寄存器。
	asm("LD.w R6,[R0++]"				);
	asm("LD.w R7,[R0++]"				);
	asm("LD.w R8,[R0++]"				);
	asm("LD.w R9,[R0++]"				);
	asm("LD.w R10,[R0++]"				);
	asm("LD.w R11,[R0++]"				);
	asm("LD.w R12,[R0++]"				);
	asm("LD.w R13,[R0++]"				);
	/*2、将指针切换为第一个任务的堆栈*/
	asm("MOV PSP,R0"					);//更换MSP地址，指针指向任务堆栈
	asm("NOP"							);
	/*3、更改中断屏蔽，以使能中断*/
	asm("MOV R0,#0"						);
	asm("LSL R0,#8"						);//屏蔽优先级移8位
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//取出寄存器值
	asm("ANL R3,R1"						);//去掉原值
	asm("ORL R3,R0"						);//放入数值
	asm("ST.w [R2],R3"					);//载入寄存器
	/*中断退出，根据返回值0xFFFFFFFD自动切换指针为PSP，运行第一个任务程序*/
}
/*-----------------------------------------------------------*/
/*使能SVC中断，启动第一个任务*/
void prvStartFirstTask( void )
{
	asm("MOV R0,#0x402000BC"		);//向量表偏移寄存器
	asm("LD.w R0,[R0]"				);//获得其向量表地址
	asm("LD.w R0,[R0]"				);//获得中断向量为0的地址
	asm("MOV MSP,R0"				);//将其地址送入MSP中
	asm("ENI"						);//使能中断
	asm("NOP"						);//
	asm("SVC"						);//软件使能SVC中断
	asm("NOP"						);
	asm("NOP"						);
}
/* 进行硬件配置
 * 配置SysTick中断，包含了中断优先级和计数值，产生和用户相匹配的中断频率。
 * 设置SoftSV中断优先级*/
BaseType_t xPortStartScheduler( void )
{
	/*系统最大中断优先级参数校验*/
	configASSERT( configMAX_SYSCALL_INTERRUPT_PRIORITY );


	#if( configASSERT_DEFINED == 1 )
	{
		volatile uint32_t ulOriginalPriority;
		/*第一个用户使用的优先级寄存器*/
		volatile uint8_t * const pucFirstUserPriorityRegister = ( uint8_t * ) ( portINT_IP + portFIRST_USER_INTERRUPT_NUMBER );
		volatile uint8_t ucMaxPriorityValue;

		/* 确定最大中断优先级能使用。ISR安全相关宏、函数等以"FromISR"结尾。在FreeRTOS系统中
		分离线程和ISR程序应用接口（API）以确保进入中断简洁快速*/
		ulOriginalPriority = *pucFirstUserPriorityRegister;

		/* 确定优先级的比特位，第一个写所有可能的比特位，即0xFF*/
		*pucFirstUserPriorityRegister = portMAX_8_BIT_VALUE;

		/* 读有几个比特位被掩盖，0xF0*/
		ucMaxPriorityValue = *pucFirstUserPriorityRegister;

		/* 核心中断优先级被设置为最低优先级*/
		configASSERT( ucMaxPriorityValue == ( configKERNEL_INTERRUPT_PRIORITY & ucMaxPriorityValue ) );

		/* 利用同样的掩码为最大系统调用优先级*/
		ucMaxSysCallPriority = configMAX_SYSCALL_INTERRUPT_PRIORITY & ucMaxPriorityValue;

		/* 计算最大的优先级分组并返回值*/
		ulMaxPRIGROUPValue = portMAX_PRIGROUP_BITS;
		ucMaxPriorityValue &= 0xE0;//最多的抢占优先级为3位
		while( ( ucMaxPriorityValue & portTOP_BIT_OF_BYTE ) == portTOP_BIT_OF_BYTE )
		{
			ulMaxPRIGROUPValue--;
			ucMaxPriorityValue <<= ( uint8_t ) 0x01;
		}

		/* 移位优先级分组并准备赋值给寄存器*/
		ulMaxPRIGROUPValue <<= portPRIGROUP_SHIFT;
		ulMaxPRIGROUPValue &= portPRIORITY_GROUP_MASK;

		/* 恢复修改的优先级为原来的值*/
		*pucFirstUserPriorityRegister = ulOriginalPriority;
	}
	#endif /* conifgASSERT_DEFINED */

	/* 使SoftSV和SysTick为最低优先级*/
	portINT_IP2_REG |= portINT_SOFTSV_PRI;
	portINT_IP2_REG |= portINT_SYSTICK_PRI;

	/*设置滴答定时器的中断周期，并使能其中断*/
	vPortSetupTimerInterrupt();

	/*初始化临界区嵌套计数器*/
	uxCriticalNesting = 0;

	/* 开始第一个任务*/
	prvStartFirstTask();

	/*任务不应该运行到这，如果运行到这说明没能正常启动任务*/
	return 0;
}
/*-----------------------------------------------------------*/
/* 取消已经通过xPortStartScheduler()执行的硬件/中断服务程序设置，这样调度程序停止执行后
 * 硬件会留下原始状态。
 * 调用硬件层关闭中断的处理函数。在关闭任务调度器中使用。
 * */
void vPortEndScheduler( void )
{
	configASSERT( uxCriticalNesting == 1000UL );
}
/*-----------------------------------------------------------*/
//进入临界区
void vPortEnterCritical( void )
{
	portDISABLE_INTERRUPTS();
	uxCriticalNesting++;

	/* This is not the interrupt safe version of the enter critical function so
	assert() if it is being called from an interrupt context.  Only API
	functions that end in "FromISR" can be used in an interrupt.  Only assert if
	the critical nesting count is 1 to protect against recursive calls if the
	assert function also uses a critical section. */
	/* 这不是进入临界功能的中断安全版本，所以就有了从中断语句中使用assert()函数。
	 * 只有以FromISR结尾用在中断的API函数。仅在临界嵌套计数(uxCriticalNesting)为1时
	 * 才去保护在验证函数被用于一个临界选择时，以递归调用相对的程序*/
//	if( uxCriticalNesting == 1 )
//	{
//		验证当前中断是否是0
//		configASSERT( ( portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK ) == 0 );
//		/* 等待商榷*/
//	}
}
/*-----------------------------------------------------------*/
//退出临界区
void vPortExitCritical( void )
{
	configASSERT( uxCriticalNesting );
	uxCriticalNesting--;
	if( uxCriticalNesting == 0 )
	{
		portENABLE_INTERRUPTS();
	}
}
/*-----------------------------------------------------------*/
/* SoftSV中断服务函数
 * 用于任务切换，也是核心的一个中断服务函数
 * */

void __attribute__((interrupt)) _SoftSV_exception (void)
{
	/*
	 * 该中断具体执行内容是：
	 * 1、压栈当前任务
	 * 2、关闭中断，获取下一个任务
	 * 3、开启中断
	 * 4、出栈下一个任务
	 * */	/*定义一个静态局部常量，等于configMAX_SYSCALL_INTERRUPT_PRIORITY，用后缀区分，嵌汇编中调用*/
//	asm(".extern uxCriticalNesting");
	asm(".extern pxCurrentTCB");
	asm(".extern vTaskSwitchContext");
	/* 1、压栈当前任务*/
	asm("MOV R0,PSP"                    );
	asm("MOV R3,#pxCurrentTCB"          );//获取当前TCB地址
	asm("LD.w R2,[R3]"					);//将当前任务的第一项(堆栈指针)传给R2
	//保存R5-R13
	asm("SUB R0,#4");
	asm("ST.w [R0],R13");
	asm("SUB R0,#4");
	asm("ST.w [R0],R12");
	asm("SUB R0,#4");
	asm("ST.w [R0],R11");
	asm("SUB R0,#4");
	asm("ST.w [R0],R10");
	asm("SUB R0,#4");
	asm("ST.w [R0],R9");
	asm("SUB R0,#4");
	asm("ST.w [R0],R8");
	asm("SUB R0,#4");
	asm("ST.w [R0],R7");
	asm("SUB R0,#4");
	asm("ST.w [R0],R6");
	asm("SUB R0,#4");
	asm("ST.w [R0],R5");
	asm("ST.w [R2],R0");//将当前任务的堆栈指针更新
	/* 2、关闭中断，获取下一个任务*/
	asm("MOV R0,#%0"::"g"(configMAX_SYSCALL_INTERRUPT_PRIORITY));
	asm("LSL R0,#8"						);//屏蔽优先级移8位
	asm("MOV R1,#0xFFFF00FF"			);//中断屏蔽掩码的反码
	asm("MOV R2,#0x40200000"			);//中断控制寄存器
	asm("LD.w R3,[R2]"					);//取出寄存器值
	asm("ANL R3,R1"						);//去掉原值
	asm("ORL R3,R0"						);//放入数值
	asm("ST.w [R2],R3"					);//载入寄存器
	asm("MOV R2,#vTaskSwitchContext"    );
	asm("LJMP R2"                       );//调用函数vTaskSwitchContext()
	/* 3、开启中断*/
	asm("MOV R0,#0"                     );//打开中断
	asm("LSL R0,#8"						);//屏蔽优先级移8位
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//取出寄存器值
	asm("ANL R3,R1"						);//去掉原值
	asm("ORL R3,R0"						);//放入数值
	asm("ST.w [R2],R3"					);//载入寄存器
	/* 出栈下一个任务*/
	asm("MOV R3,#pxCurrentTCB"          );//获取当前TCB地址
	asm("LD.w R1,[R3]"                  );//R3的值指向了新的任务
	asm("LD.w R0,[R1]"                  );//取新任务的堆栈
	asm("LD.w R5,[R0++]"                );//堆栈R5-R13
	asm("LD.w R6,[R0++]"                );
	asm("LD.w R7,[R0++]"                );
	asm("LD.w R8,[R0++]"                );
	asm("LD.w R9,[R0++]"                );
	asm("LD.w R10,[R0++]"               );
	asm("LD.w R11,[R0++]"               );
	asm("LD.w R12,[R0++]"               );
	asm("LD.w R13,[R0++]"               );
	asm("MOV PSP,R0"                    );
	asm("NOP"                          );
	/*中断退出自动加载R0-R4,PC,LR,xPSR等值*/
}

/*
 * SysTick中断服务程序
 * */
void __attribute__((interrupt)) _SysTick_exception (void)
{
	/*关闭中断*/
	vPortRaiseBASEPRI();
	{
		/* 增加计数值 */
		if( xTaskIncrementTick() != pdFALSE )//增加时钟计数器xTickCount的值
		{
			/* 通过向中断控制和状态寄存器ICSR的bit28写入1挂起SoftSV来启动SoftSV中断。
			 * 就可以在SoftSV中断服务函数中进行任务切换*/
			portNVIC_INT_CTRL_REG = portNVIC_SOFTSVSET_BIT;
		}
	}
	vPortClearBASEPRIFromISR();//打开中断
}
/*-----------------------------------------------------------*/
/* 空闲低功耗模式。
 * 尚未调试
 * */
#if configUSE_TICKLESS_IDLE == 1

//	__weak void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
//	{
//	uint32_t ulReloadValue, ulCompleteTickPeriods, ulCompletedSysTickDecrements, ulSysTickCTRL;
//	TickType_t xModifiableIdleTime;
//
//		/* Make sure the SysTick reload value does not overflow the counter. */
//		if( xExpectedIdleTime > xMaximumPossibleSuppressedTicks )
//		{
//			xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
//		}
//
//		/* Stop the SysTick momentarily.  The time the SysTick is stopped for
//		is accounted for as best it can be, but using the tickless mode will
//		inevitably result in some tiny drift of the time maintained by the
//		kernel with respect to calendar time. */
//		portST_CTL_REG &= ~portST_CTL_STEN;
//
//		/* Calculate the reload value required to wait xExpectedIdleTime
//		tick periods.  -1 is used because this code will execute part way
//		through one of the tick periods. */
//		ulReloadValue = portST_CV_REG + ( ulTimerCountsForOneTick * ( xExpectedIdleTime - 1UL ) );
//		if( ulReloadValue > ulStoppedTimerCompensation )
//		{
//			ulReloadValue -= ulStoppedTimerCompensation;
//		}
//
//		/* Enter a critical section but don't use the taskENTER_CRITICAL()
//		method as that will mask interrupts that should exit sleep mode. */
//
//		__disable_irq();
//		__dsb( portSY_FULL_READ_WRITE );
//		__isb( portSY_FULL_READ_WRITE );
//
//		/* If a context switch is pending or a task is waiting for the scheduler
//		to be unsuspended then abandon the low power entry. */
//		if( eTaskConfirmSleepModeStatus() == eAbortSleep )
//		{
//			/* Restart from whatever is left in the count register to complete
//			this tick period. */
//			portST_RELOAD_REG = portST_CV_REG;
//
//			/* Restart SysTick. */
//			portST_CTL_REG |= portST_CTL_STEN;
//
//			/* Reset the reload register to the value required for normal tick
//			periods. */
//			portST_RELOAD_REG = ulTimerCountsForOneTick - 1UL;
//
//			/* Re-enable interrupts - see comments above __disable_irq() call
//			above. */
//			__enable_irq();
//		}
//		else
//		{
//			/* Set the new reload value. */
//			portST_RELOAD_REG = ulReloadValue;
//
//			/* Clear the SysTick count flag and set the count value back to
//			zero. */
//			portST_CV_REG = 0UL;
//
//			/* Restart SysTick. */
//			portST_CTL_REG |= portST_CTL_STEN;
//
//			/* Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
//			set its parameter to 0 to indicate that its implementation contains
//			its own wait for interrupt or wait for event instruction, and so wfi
//			should not be executed again.  However, the original expected idle
//			time variable must remain unmodified, so a copy is taken. */
//			xModifiableIdleTime = xExpectedIdleTime;
//			configPRE_SLEEP_PROCESSING( xModifiableIdleTime );
//			if( xModifiableIdleTime > 0 )
//			{
//				/****************/
//				__dsb( portSY_FULL_READ_WRITE );
//				__wfi();
//				__isb( portSY_FULL_READ_WRITE );
//			}
//			configPOST_SLEEP_PROCESSING( xExpectedIdleTime );
//
//			/* Stop SysTick.  Again, the time the SysTick is stopped for is
//			accounted for as best it can be, but using the tickless mode will
//			inevitably result in some tiny drift of the time maintained by the
//			kernel with respect to calendar time. */
//			ulSysTickCTRL = portST_CTL_REG;
//			portST_CTL_REG = ( ulSysTickCTRL & ~portST_CTL_STEN );
//
//			/* Re-enable interrupts - see comments above __disable_irq() call
//			above. */
//			__enable_irq();
//
//			if( ( ulSysTickCTRL & portST_CTL_COUNTZERO ) != 0 )
//			{
//				uint32_t ulCalculatedLoadValue;
//
//				/* The tick interrupt has already executed, and the SysTick
//				count reloaded with ulReloadValue.  Reset the
//				portST_RELOAD_REG with whatever remains of this tick
//				period. */
//				ulCalculatedLoadValue = ( ulTimerCountsForOneTick - 1UL ) - ( ulReloadValue - portST_CV_REG );
//
//				/* Don't allow a tiny value, or values that have somehow
//				underflowed because the post sleep hook did something
//				that took too long. */
//				if( ( ulCalculatedLoadValue < ulStoppedTimerCompensation ) || ( ulCalculatedLoadValue > ulTimerCountsForOneTick ) )
//				{
//					ulCalculatedLoadValue = ( ulTimerCountsForOneTick - 1UL );
//				}
//
//				portST_RELOAD_REG = ulCalculatedLoadValue;
//
//				/* The tick interrupt handler will already have pended the tick
//				processing in the kernel.  As the pending tick will be
//				processed as soon as this function exits, the tick value
//				maintained by the tick is stepped forward by one less than the
//				time spent waiting. */
//				ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
//			}
//			else
//			{
//				/* Something other than the tick interrupt ended the sleep.
//				Work out how long the sleep lasted rounded to complete tick
//				periods (not the ulReload value which accounted for part
//				ticks). */
//				ulCompletedSysTickDecrements = ( xExpectedIdleTime * ulTimerCountsForOneTick ) - portST_CV_REG;
//
//				/* How many complete tick periods passed while the processor
//				was waiting? */
//				ulCompleteTickPeriods = ulCompletedSysTickDecrements / ulTimerCountsForOneTick;
//
//				/* The reload value is set to whatever fraction of a single tick
//				period remains. */
//				portST_RELOAD_REG = ( ( ulCompleteTickPeriods + 1UL ) * ulTimerCountsForOneTick ) - ulCompletedSysTickDecrements;
//			}
//
//			/* Restart SysTick so it runs from portST_RELOAD_REG
//			again, then set portST_RELOAD_REG back to its standard
//			value.  The critical section is used to ensure the tick interrupt
//			can only execute once in the case that the reload register is near
//			zero. */
//			portST_CV_REG = 0UL;
//			portENTER_CRITICAL();
//			{
//				portST_CTL_REG |= portST_CTL_STEN;
//				vTaskStepTick( ulCompleteTickPeriods );
//				portST_RELOAD_REG = ulTimerCountsForOneTick - 1UL;
//			}
//			portEXIT_CRITICAL();
//		}
//	}

#endif /* #if configUSE_TICKLESS_IDLE */

/*-----------------------------------------------------------*/

//设置节拍定时器的节拍而产生节拍中断
#if configOVERRIDE_DEFAULT_TICK_CONFIGURATION == 0

	void vPortSetupTimerInterrupt( void )
	{
		/* 计算所需的配置节拍定时器的中断*/
		#if configUSE_TICKLESS_IDLE == 1
		{
			ulTimerCountsForOneTick = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ );
			xMaximumPossibleSuppressedTicks = portMAX_24_BIT_NUMBER / ulTimerCountsForOneTick;
			ulStoppedTimerCompensation = portMISSED_COUNTS_FACTOR / ( configCPU_CLOCK_HZ / configSYSTICK_CLOCK_HZ );
		}
		#endif /* configUSE_TICKLESS_IDLE */
		/* 根据需求配置节拍定时器寄存器*/
		portST_CV_REG = 0;
		portST_RELOAD_REG = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
		portST_CTL_REG = ( portNVIC_SYSTICK_CLK_BIT | portST_CTL_TICKINTEN | portST_CTL_STEN );
	}

	void vPortUpdateTimerTick( void )
	{
		if(portST_CTL_REG != 0){
			portST_RELOAD_REG = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
			portST_CTL_REG = ( portNVIC_SYSTICK_CLK_BIT | portST_CTL_TICKINTEN | portST_CTL_STEN );
		}
	}
#endif /* configOVERRIDE_DEFAULT_TICK_CONFIGURATION */
/*-----------------------------------------------------------*/
/* 获取当前正在处理的中断向量编号*/
uint32_t __attribute__((noinline)) vPortGetIPSR(void)
{
	asm("MOV R1,#0x40200000"		);//中断控制寄存器
	asm("MOV R2,#0x7F000000"		);//中断向量掩码
	asm("LD.w R1,[R1]"				);//取出寄存器值
	asm("ANL R1,R2"					);//提取对应值
	asm("MOV R2,#24"				);
	asm("LSR R0,R1,R2"				);//进行移位操作并赋值给R0返回
	asm("JMP LR"					);
}
/*-----------------------------------------------------------*/
#if( configASSERT_DEFINED == 1 )
	/*验证中断优先级*/
	void vPortValidateInterruptPriority( void )
	{
	uint32_t ulCurrentInterrupt;
	uint8_t ucCurrentPriority;
		/* 获取中断向量编号 */
		ulCurrentInterrupt = vPortGetIPSR();

		/* 是否为用户级中断?*/
		if( ulCurrentInterrupt >= portFIRST_USER_INTERRUPT_NUMBER )
		{
			/* 在中断优先级寄存器中查找中断优先级*/
			ucCurrentPriority = pcInterruptPriorityRegisters[ ulCurrentInterrupt ];
			/* 如果一个中断服务程序的优先级高于configMAX_SYSCALL_INTERRUPT_PRIORITY
			 * (能在FreeRTOS系统中正常召唤中断的一个优先级)，程序就在这里停止。所要使用
			 * 的中断优先级必须要低于configMAX_SYSCALL_INTERRUPT_PRIORITY常量。
			 *
			 * 低中断优先级的数字比高优先级的高，所以中断优先级的数字值必须比
			 * configMAX_SYSCALL_INTERRUPT_PRIORITY高。
			 *
			 * 使用FreeRTOS应用程序接口的中断的优先级数值不能设置为默认0，应该高于
			 * configMAX_SYSCALL_INTERRUPT_PRIORITY常量，以确保有效。
			 *
			 * FreeRTOS系统主张线程和中断API函数分开以确保中断进入尽可能快速和简洁。
			 * */
			// MY_MSG("INT_IP9:0x%x",INT_IP9);
			// MY_MSG("\r\n当前中断%d,当前优先级%d,系统调用最大优先级%d\r\n",ulCurrentInterrupt,ucCurrentPriority,ucMaxSysCallPriority);
			configASSERT( ucCurrentPriority >= ucMaxSysCallPriority );
		}

		/* 优先级分组：中断系统中，中断优先级比特位被定义为抢占优先级和子优先级，为了
		 * 方便把尽可能多的优先级配置为抢占优先级。这样如果有一些bit位表现为子优先级验证就会失败。
		 *
		 * 如果开发者在启动调度器之前将优先级分组设置为抢占优先级为0位，子优先级为4位
		 * 的话，结果会导致不可预测的错误
		 * */
		configASSERT( ( portINT_CTL0 & portPRIORITY_GROUP_MASK ) <= ulMaxPRIGROUPValue );
	}

#endif /* configASSERT_DEFINED */



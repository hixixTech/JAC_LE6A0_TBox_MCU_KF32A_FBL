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


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * 特殊接口定义
 * 文件配置了一些硬件和汇编代码，需要正确地配置以配置FreeRTOS系统。
 * 这些设置不应该被用户所更改。
 *-----------------------------------------------------------
 * */
/* Type definitions. */
/* 类型定义*/
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	uint32_t
#define portBASE_TYPE	long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL

	/* 由于是在32位架构下32位时钟，所以读计数值时没有限制*/
	#define portTICK_TYPE_IS_ATOMIC 1
#endif
/*-----------------------------------------------------------*/

/* 架构配置*/
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			8

/*-----------------------------------------------------------*/

/* 任务切换*/
#define portYIELD() 											\
{																\
	/* 启动SotfSV中断进行上下进程切换*/							\
	portNVIC_INT_CTRL_REG = portNVIC_SOFTSVSET_BIT;				\
	asm("NOP");													\
}
/*-----------------------------------------------------------*/

#define portNVIC_INT_CTRL_REG		( * ( ( volatile uint32_t * ) 0x4020001C ) )//中断控制寄存器
#define portNVIC_SOFTSVSET_BIT		( 1UL << 14UL )								//SotfSV位
#define portEND_SWITCHING_ISR( xSwitchRequired ) if( xSwitchRequired != pdFALSE ) portYIELD()//
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )						//从中断放弃（当x不为pdFALSE时，进行任务切换）,暂无函数用到。
/*-----------------------------------------------------------*/

/* 临界段管理*/
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portDISABLE_INTERRUPTS()				vPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()					vPortSetBASEPRI( 0 )
#define portENTER_CRITICAL()					vPortEnterCritical()
#define portEXIT_CRITICAL()						vPortExitCritical()
#define portSET_INTERRUPT_MASK_FROM_ISR()		ulPortRaiseBASEPRI()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vPortSetBASEPRI(x)

/*-----------------------------------------------------------*/

/* Tickless 空闲 /低功耗功能*/
#ifndef portSUPPRESS_TICKS_AND_SLEEP
	extern void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime );
	#define portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime ) vPortSuppressTicksAndSleep( xExpectedIdleTime )
#endif
/*-----------------------------------------------------------*/
//#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
///* Port specific optimisations. */
///* 接口特殊最优化设置
// * 置1使用硬件方法，置0使用通用方法*/
//#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
//	#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
//#endif
//
//#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1
//
//	/* Check the configuration. */
//	#if( configMAX_PRIORITIES > 32 )
//		#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
//	#endif
//
//	/* Store/clear the ready priorities in a bit map. */
//	/* 在一个比特图上写或清即将操作的优先级*/
//	/* 记录准备优先级
//	 * 重设准备优先级*/
//	#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
//	#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )
//
//	/*-----------------------------------------------------------*/
//
//	#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31UL - ( uint32_t ) __clz( ( uxReadyPriorities ) ) )
//
//#endif /* taskRECORD_READY_PRIORITY */
/*-----------------------------------------------------------*/

/*使用函数名来启动任务*/
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )
/*-----------------------------------------------------------*/

	/*参数检查*/
#ifdef configASSERT
	void vPortValidateInterruptPriority( void );
	#define portASSERT_IF_INTERRUPT_PRIORITY_INVALID() 	vPortValidateInterruptPriority()
#endif

/*portNOP()宏可有可无*/
#define portNOP()	/*asm("NOP");*/
/*-----------------------------------------------------------*/
/*
 * 优先级响应基级设置
 * */
static void __attribute__((noinline)) vPortSetBASEPRI( uint32_t ulBASEPRI )
{
	/*
	 * 设置优先级基级
	 * 临界指令通常不用于低优先级值的函数中
	 * */
	asm("LSL R0,#8"						);//屏蔽优先级移8位
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//取出寄存器值
	asm("ANL R3,R1"						);//去掉原值
	asm("ORL R3,R0"						);//放入数值
	asm("ST.w [R2],R3"					);//载入寄存器
	/*指令结束*/
}

/*取消中断*/
static  void __attribute__((noinline)) vPortRaiseBASEPRI( void )
{
	asm("MOV R0,#%0"::"g"(configMAX_SYSCALL_INTERRUPT_PRIORITY));
	asm("LSL R0,#8"						);//屏蔽优先级移8位
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//取出寄存器值
	asm("NOP"                           );
	asm("ANL R3,R1"						);//去掉原值
	asm("ORL R3,R0"						);//放入数值
	asm("ST.w [R2],R3"					);//载入寄存器
	/*指令结束*/
}
		/*-----------------------------------------------------------*/
/*（中断中）清除优先级基级*/
static void __attribute__((noinline)) vPortClearBASEPRIFromISR( void )
{
	/*将优先级基级置0*/
	asm("MOV R0,#0"						);//优先级基级设置为0
	asm("LSL R0,#8"						);//屏蔽优先级移8位
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//取出寄存器值
	asm("ANL R3,R1"						);//去掉原值
	asm("ORL R3,R0"						);//放入数值
	asm("ST.w [R2],R3"					);//载入寄存器
	/*指令结束*/


}
/*
 * 将优先级基级设置为configMAX_SYSCALL_INTERRUPT_PRIORITY，
 * 且返回之前的设置
 * */
static uint32_t __attribute__((noinline)) ulPortRaiseBASEPRI( void )
{

	asm("MOV R1,#0x0000ff00"            );
	asm("MOV R2,#0x40200000"            );
	asm("LD.w R3,[R2]"                  );
	asm("NOP"							);
	asm("ANL R3,R1"                     );
	asm("LSR R3,#8"						);
	asm("MOV R4,R3"						);

	asm("MOV R0,#%0"::"g"(configMAX_SYSCALL_INTERRUPT_PRIORITY));
	asm("LSL R0,#8"						);//屏蔽优先级移8位
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//取出寄存器值
	asm("NOP"                           );
	asm("ANL R3,R1"						);//去掉原值
	asm("ORL R3,R0"						);//放入数值
	asm("ST.w [R2],R3"					);//载入寄存器
	/*指令结束*/
	asm("MOV R0,R4"						);
	asm("JMP LR"						);

}
/*-----------------------------------------------------------*/
/*读取中断号，判断是否为0*/
static BaseType_t __attribute__((noinline)) xPortIsInsideInterrupt( void )
{
	asm("MOV R1,#0x7f000000"            );
	asm("MOV R2,#0x40200000"            );
	asm("LD.w R3,[R2]"                  );
	asm("NOP"							);
	asm("ANL R3,R1"                     );
	asm("LSR R3,#24"					);//R3为当前中断号
	asm("SUB R3,#0"						);
	asm("JZ FFFF"						);
	asm("MOV R0,#0"						);
	asm("JMP LR"						);
	asm("FFFF:MOV R0,#1"				);
	asm("JMP LR"						);
	/*指令结束*/
}



#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */


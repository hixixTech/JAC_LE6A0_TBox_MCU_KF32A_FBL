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
 * ����ӿڶ���
 * �ļ�������һЩӲ���ͻ����룬��Ҫ��ȷ������������FreeRTOSϵͳ��
 * ��Щ���ò�Ӧ�ñ��û������ġ�
 *-----------------------------------------------------------
 * */
/* Type definitions. */
/* ���Ͷ���*/
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

	/* ��������32λ�ܹ���32λʱ�ӣ����Զ�����ֵʱû������*/
	#define portTICK_TYPE_IS_ATOMIC 1
#endif
/*-----------------------------------------------------------*/

/* �ܹ�����*/
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			8

/*-----------------------------------------------------------*/

/* �����л�*/
#define portYIELD() 											\
{																\
	/* ����SotfSV�жϽ������½����л�*/							\
	portNVIC_INT_CTRL_REG = portNVIC_SOFTSVSET_BIT;				\
	asm("NOP");													\
}
/*-----------------------------------------------------------*/

#define portNVIC_INT_CTRL_REG		( * ( ( volatile uint32_t * ) 0x4020001C ) )//�жϿ��ƼĴ���
#define portNVIC_SOFTSVSET_BIT		( 1UL << 14UL )								//SotfSVλ
#define portEND_SWITCHING_ISR( xSwitchRequired ) if( xSwitchRequired != pdFALSE ) portYIELD()//
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )						//���жϷ�������x��ΪpdFALSEʱ�����������л���,���޺����õ���
/*-----------------------------------------------------------*/

/* �ٽ�ι���*/
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portDISABLE_INTERRUPTS()				vPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()					vPortSetBASEPRI( 0 )
#define portENTER_CRITICAL()					vPortEnterCritical()
#define portEXIT_CRITICAL()						vPortExitCritical()
#define portSET_INTERRUPT_MASK_FROM_ISR()		ulPortRaiseBASEPRI()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vPortSetBASEPRI(x)

/*-----------------------------------------------------------*/

/* Tickless ���� /�͹��Ĺ���*/
#ifndef portSUPPRESS_TICKS_AND_SLEEP
	extern void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime );
	#define portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime ) vPortSuppressTicksAndSleep( xExpectedIdleTime )
#endif
/*-----------------------------------------------------------*/
//#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
///* Port specific optimisations. */
///* �ӿ��������Ż�����
// * ��1ʹ��Ӳ����������0ʹ��ͨ�÷���*/
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
//	/* ��һ������ͼ��д���弴�����������ȼ�*/
//	/* ��¼׼�����ȼ�
//	 * ����׼�����ȼ�*/
//	#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
//	#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )
//
//	/*-----------------------------------------------------------*/
//
//	#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31UL - ( uint32_t ) __clz( ( uxReadyPriorities ) ) )
//
//#endif /* taskRECORD_READY_PRIORITY */
/*-----------------------------------------------------------*/

/*ʹ�ú���������������*/
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )
/*-----------------------------------------------------------*/

	/*�������*/
#ifdef configASSERT
	void vPortValidateInterruptPriority( void );
	#define portASSERT_IF_INTERRUPT_PRIORITY_INVALID() 	vPortValidateInterruptPriority()
#endif

/*portNOP()����п���*/
#define portNOP()	/*asm("NOP");*/
/*-----------------------------------------------------------*/
/*
 * ���ȼ���Ӧ��������
 * */
static void __attribute__((noinline)) vPortSetBASEPRI( uint32_t ulBASEPRI )
{
	/*
	 * �������ȼ�����
	 * �ٽ�ָ��ͨ�������ڵ����ȼ�ֵ�ĺ�����
	 * */
	asm("LSL R0,#8"						);//�������ȼ���8λ
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//ȡ���Ĵ���ֵ
	asm("ANL R3,R1"						);//ȥ��ԭֵ
	asm("ORL R3,R0"						);//������ֵ
	asm("ST.w [R2],R3"					);//����Ĵ���
	/*ָ�����*/
}

/*ȡ���ж�*/
static  void __attribute__((noinline)) vPortRaiseBASEPRI( void )
{
	asm("MOV R0,#%0"::"g"(configMAX_SYSCALL_INTERRUPT_PRIORITY));
	asm("LSL R0,#8"						);//�������ȼ���8λ
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//ȡ���Ĵ���ֵ
	asm("NOP"                           );
	asm("ANL R3,R1"						);//ȥ��ԭֵ
	asm("ORL R3,R0"						);//������ֵ
	asm("ST.w [R2],R3"					);//����Ĵ���
	/*ָ�����*/
}
		/*-----------------------------------------------------------*/
/*���ж��У�������ȼ�����*/
static void __attribute__((noinline)) vPortClearBASEPRIFromISR( void )
{
	/*�����ȼ�������0*/
	asm("MOV R0,#0"						);//���ȼ���������Ϊ0
	asm("LSL R0,#8"						);//�������ȼ���8λ
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//ȡ���Ĵ���ֵ
	asm("ANL R3,R1"						);//ȥ��ԭֵ
	asm("ORL R3,R0"						);//������ֵ
	asm("ST.w [R2],R3"					);//����Ĵ���
	/*ָ�����*/


}
/*
 * �����ȼ���������ΪconfigMAX_SYSCALL_INTERRUPT_PRIORITY��
 * �ҷ���֮ǰ������
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
	asm("LSL R0,#8"						);//�������ȼ���8λ
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//ȡ���Ĵ���ֵ
	asm("NOP"                           );
	asm("ANL R3,R1"						);//ȥ��ԭֵ
	asm("ORL R3,R0"						);//������ֵ
	asm("ST.w [R2],R3"					);//����Ĵ���
	/*ָ�����*/
	asm("MOV R0,R4"						);
	asm("JMP LR"						);

}
/*-----------------------------------------------------------*/
/*��ȡ�жϺţ��ж��Ƿ�Ϊ0*/
static BaseType_t __attribute__((noinline)) xPortIsInsideInterrupt( void )
{
	asm("MOV R1,#0x7f000000"            );
	asm("MOV R2,#0x40200000"            );
	asm("LD.w R3,[R2]"                  );
	asm("NOP"							);
	asm("ANL R3,R1"                     );
	asm("LSR R3,#24"					);//R3Ϊ��ǰ�жϺ�
	asm("SUB R3,#0"						);
	asm("JZ FFFF"						);
	asm("MOV R0,#0"						);
	asm("JMP LR"						);
	asm("FFFF:MOV R0,#1"				);
	asm("JMP LR"						);
	/*ָ�����*/
}



#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */


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
 * Ϊ��ʵ��porttable.h�Ĺ��ܶ�������KF32�ӿ�
 *----------------------------------------------------------*/
// #include "uart.h"	//debug
/* Scheduler includes. */
/*ͷ�ļ�*/
#include "FreeRTOS.h"
#include "task.h"

uint32_t RTOSSYSTEM_ERROR;

#if configMAX_SYSCALL_INTERRUPT_PRIORITY == 0
	#error configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to 0.
#endif

#ifndef configSYSTICK_CLOCK_HZ
	#define configSYSTICK_CLOCK_HZ configCPU_CLOCK_HZ
	/* ȷ��RTOSϵͳʱ����CPUƵ����ͬ��*/
	#define portNVIC_SYSTICK_CLK_BIT	( 1UL << 2UL )
#else
	/*�ڲ��������ͬ������£����޸�SysTick��ʱ��*/
	#define portNVIC_SYSTICK_CLK_BIT	( 0 )
#endif

/* �����������Ҫ���Լ���vPortSetupTimerInterrupt������������1������Ĭ�϶���Ϊ0
 * */
#ifndef configOVERRIDE_DEFAULT_TICK_CONFIGURATION
	#define configOVERRIDE_DEFAULT_TICK_CONFIGURATION 0
#endif

/* ���ļĴ���*/
#define portST_CTL_REG						( * ( ( volatile uint32_t * ) 0x402000A0 ) )//ϵͳ���Ķ�ʱ�����ƼĴ���
#define portST_RELOAD_REG					( * ( ( volatile uint32_t * ) 0x402000A4 ) )//ϵͳ���Ķ�ʱ���ؼ��ؼĴ���
#define portST_CV_REG						( * ( ( volatile uint32_t * ) 0x402000A8 ) )//ϵͳ���Ķ�ʱ��������
#define portINT_IP2_REG						( * ( ( volatile uint32_t * ) 0x4020003C ) )//�ж����ȼ��Ĵ���2����������жϺͽ��Ķ�ʱ���жϣ�

/* �Ĵ����ı���λ*/
#define portST_CTL_TICKINTEN				( 1UL << 1UL	)//���Ķ�ʱ���ж�ʹ��
#define portST_CTL_STEN						( 1UL << 0UL	)//���Ķ�ʱ��ʹ��
#define portST_CTL_COUNTZERO				( 1UL << 16UL	)//��ʱ����������0ʱ��λ��1

//�����ж����ȼ�
#define portINT_SOFTSV_PRI					( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 16UL )
#define portINT_SYSTICK_PRI					( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

/* ���ڼ����ж����ȼ���ȷ�Եĳ��� */
#define portFIRST_USER_INTERRUPT_NUMBER		( 16			)							//��һ���û��ж�
#define portINT_IP							( 0x40200030	)							//�ж����ȼ�����0�Ĵ���
#define portINT_CTL0						( * ( ( volatile uint32_t * ) 0x40200000 ) )//�жϿ��ƼĴ���
#define portMAX_8_BIT_VALUE					( ( uint8_t ) 0xff	)						//8���ص����ֵ
#define portTOP_BIT_OF_BYTE					( ( uint8_t ) 0x80	)						//�ֽ���߱���
#define portMAX_PRIGROUP_BITS				( ( uint8_t ) 3 	)						//������ȼ�����ֵ
#define portPRIORITY_GROUP_MASK				( 0x03UL << 1UL		)						//���ȼ���������
#define portPRIGROUP_SHIFT					( 1UL 				)						//���ȼ�������λ

/* ������ʼ��ջ����*/
#define portINITIAL_EXEC_RETURN		( 0xfffffffd )

/* 24λ������ϵͳ����*/
#define portMAX_24_BIT_NUMBER		( 0xffffffUL )

/* ��SysTick������tickless���м���ʱֹͣʱ��һ��fiddle����ȥ����SysTick����ֵ*/
#define portMISSED_COUNTS_FACTOR	( 45UL )

/* ������ַ���룬�������ж������뵽PC��ʱ��0λ��0*/
#define portSTART_ADDRESS_MASK		( ( StackType_t ) 0xfffffffeUL )

/* ���ٽ�Ƕ�ױ�����ʹ��ÿ�����񱣳������Լ����ж�״̬*/
volatile static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

/* ���ö�ʱ�������������жϡ�*/
void vPortSetupTimerInterrupt( void );

/* ��ʼ��һ�������������������룬�ܹ���������*/
static void prvStartFirstTask( void );

/* ��������˳������������ڿ����ߴ����˳�����ʱ���á�*/
static void prvTaskExitError( void );

/*-----------------------------------------------------------*/

/* ���һ�����������µ�SysTick����������*/
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t ulTimerCountsForOneTick = 0;
#endif /* configUSE_TICKLESS_IDLE */

/*24����SysTick��ʱ���ĵ����������*/
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t xMaximumPossibleSuppressedTicks = 0;
#endif /* configUSE_TICKLESS_IDLE */

/* ��SysTickֹͣʱ(���͹���)���ݲ�����CPU����*/
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t ulStoppedTimerCompensation = 0;
#endif /* configUSE_TICKLESS_IDLE */

/* ��portASSERT_IF_INTERRUPT_PRIORITY_INVALID()��֮ǰȥʹ�ã�
 * ��ȷ��FreeRTOS��API��������ʹ�÷�������ȼ������ֵ
 * (configMAX_SYSCALL_INTERRUPT_PRIORITY)��Ҫ����ж�*/
#if ( configASSERT_DEFINED == 1 )
	/* ���ϵͳ�������ȼ�*/
	static uint8_t ucMaxSysCallPriority = 0;
	/* ������ȼ�����ֵ*/
	static uint32_t ulMaxPRIGROUPValue = 0;
	/* �ж����ȼ��Ĵ���*/
	static const volatile uint8_t * const pcInterruptPriorityRegisters = ( uint8_t * ) portINT_IP;
#endif /* configASSERT_DEFINED */

/*-----------------------------------------------------------*/

/* ���������ջ�����ȿ�������ͨ���������ջ���н��̡�ͨ�üĴ����ᱻ����ö�ջ��
 * */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	/* xPSR */
	pxTopOfStack--;
	*pxTopOfStack = 0x0000ff00;

	/* �Ĵ���PC��ʼ��Ϊ������pxCode*/
	pxTopOfStack--;
	*pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK;	/* PC */

	/* �Ĵ���LR��ʼ��Ϊ����prvTaskExitError*/
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) prvTaskExitError;	/* LR */

	/* �Ĵ���R4,R3,R2,R1,R0*/
	pxTopOfStack -= 5;//����4���Ĵ���R4,R3,R2,R1
	*pxTopOfStack = ( StackType_t ) pvParameters;	/* R0 */

	/*����EXC_RETURNֵ�����������˳�SVC��SoftSV�жϵ�ʱ������Ӧ�ô���ʲô״̬�������������쳣���ж�
	 * �������ISR��ʱ�����ӼĴ���R14(LR)����ֵ�ᱻ����ΪEXC_RETURN��ֵ��֮�����ֵ�����쳣�������
	 * ʱ�����쳣���ء�������Ϊ������Ϊ0xFFFFFFFD����ʾ�˳��쳣�Ժ�CPU�����߳�ģʽ����ʹ�ý���ջ��*/
	pxTopOfStack--;
	*pxTopOfStack = portINITIAL_EXEC_RETURN;/* 0xFFFF FFFD */

	/* ����R12,R11,R10,R9,R8,R7,R6,R5*/
	pxTopOfStack -= 8;
	return pxTopOfStack;
}
/*-----------------------------------------------------------*/
//��������˳�
/*
 * ͨ������£������ߵ������������˳���Ҫ����ǰҲ����ʹ��vTaskDelete(NULL)������
 * ��configASSERT()��������Զ�һ��assert()�����������Ϳ���ͣ����������ڣ������߿�������������ڻ�ȡ������Ϣ��
 * */
static void prvTaskExitError( void )
{
	configASSERT( uxCriticalNesting == ~0UL );
	portDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/
/* SVC�жϷ�����*/
/* ���������ã������ڵ�һ�����񣬺����ò���SVC�ˡ�
 * */
void __attribute__((interrupt)) _SVC_exception (void)
{
	/* SVC�ж����̽���
	 * 1������һ�������R5-R13�Ƴ�
	 * 2����ָ���л�Ϊ��һ������Ķ�ջ
	 * 3�������ж����Σ���ʹ���ж�
	 * */
	//************ �жϳ���    *******************************
	/*1������һ�������R5-R13�Ƴ�*/
	asm(".extern pxCurrentTCB"			);
	asm("MOV R3,#pxCurrentTCB"			);
	asm("LD.w R1,[R3]"					);//��R3ָ���ַ��ֵ��R1
	asm("LD.w R0,[R1]"					);
	asm("LD.w R5,[R0++]"				);//�����ǽ�R0ָ��ĵ�ַ��ֵ���Ĵ�����
	asm("LD.w R6,[R0++]"				);
	asm("LD.w R7,[R0++]"				);
	asm("LD.w R8,[R0++]"				);
	asm("LD.w R9,[R0++]"				);
	asm("LD.w R10,[R0++]"				);
	asm("LD.w R11,[R0++]"				);
	asm("LD.w R12,[R0++]"				);
	asm("LD.w R13,[R0++]"				);
	/*2����ָ���л�Ϊ��һ������Ķ�ջ*/
	asm("MOV PSP,R0"					);//����MSP��ַ��ָ��ָ�������ջ
	asm("NOP"							);
	/*3�������ж����Σ���ʹ���ж�*/
	asm("MOV R0,#0"						);
	asm("LSL R0,#8"						);//�������ȼ���8λ
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//ȡ���Ĵ���ֵ
	asm("ANL R3,R1"						);//ȥ��ԭֵ
	asm("ORL R3,R0"						);//������ֵ
	asm("ST.w [R2],R3"					);//����Ĵ���
	/*�ж��˳������ݷ���ֵ0xFFFFFFFD�Զ��л�ָ��ΪPSP�����е�һ���������*/
}
/*-----------------------------------------------------------*/
/*ʹ��SVC�жϣ�������һ������*/
void prvStartFirstTask( void )
{
	asm("MOV R0,#0x402000BC"		);//������ƫ�ƼĴ���
	asm("LD.w R0,[R0]"				);//������������ַ
	asm("LD.w R0,[R0]"				);//����ж�����Ϊ0�ĵ�ַ
	asm("MOV MSP,R0"				);//�����ַ����MSP��
	asm("ENI"						);//ʹ���ж�
	asm("NOP"						);//
	asm("SVC"						);//���ʹ��SVC�ж�
	asm("NOP"						);
	asm("NOP"						);
}
/* ����Ӳ������
 * ����SysTick�жϣ��������ж����ȼ��ͼ���ֵ���������û���ƥ����ж�Ƶ�ʡ�
 * ����SoftSV�ж����ȼ�*/
BaseType_t xPortStartScheduler( void )
{
	/*ϵͳ����ж����ȼ�����У��*/
	configASSERT( configMAX_SYSCALL_INTERRUPT_PRIORITY );


	#if( configASSERT_DEFINED == 1 )
	{
		volatile uint32_t ulOriginalPriority;
		/*��һ���û�ʹ�õ����ȼ��Ĵ���*/
		volatile uint8_t * const pucFirstUserPriorityRegister = ( uint8_t * ) ( portINT_IP + portFIRST_USER_INTERRUPT_NUMBER );
		volatile uint8_t ucMaxPriorityValue;

		/* ȷ������ж����ȼ���ʹ�á�ISR��ȫ��غꡢ��������"FromISR"��β����FreeRTOSϵͳ��
		�����̺߳�ISR����Ӧ�ýӿڣ�API����ȷ�������жϼ�����*/
		ulOriginalPriority = *pucFirstUserPriorityRegister;

		/* ȷ�����ȼ��ı���λ����һ��д���п��ܵı���λ����0xFF*/
		*pucFirstUserPriorityRegister = portMAX_8_BIT_VALUE;

		/* ���м�������λ���ڸǣ�0xF0*/
		ucMaxPriorityValue = *pucFirstUserPriorityRegister;

		/* �����ж����ȼ�������Ϊ������ȼ�*/
		configASSERT( ucMaxPriorityValue == ( configKERNEL_INTERRUPT_PRIORITY & ucMaxPriorityValue ) );

		/* ����ͬ��������Ϊ���ϵͳ�������ȼ�*/
		ucMaxSysCallPriority = configMAX_SYSCALL_INTERRUPT_PRIORITY & ucMaxPriorityValue;

		/* �����������ȼ����鲢����ֵ*/
		ulMaxPRIGROUPValue = portMAX_PRIGROUP_BITS;
		ucMaxPriorityValue &= 0xE0;//������ռ���ȼ�Ϊ3λ
		while( ( ucMaxPriorityValue & portTOP_BIT_OF_BYTE ) == portTOP_BIT_OF_BYTE )
		{
			ulMaxPRIGROUPValue--;
			ucMaxPriorityValue <<= ( uint8_t ) 0x01;
		}

		/* ��λ���ȼ����鲢׼����ֵ���Ĵ���*/
		ulMaxPRIGROUPValue <<= portPRIGROUP_SHIFT;
		ulMaxPRIGROUPValue &= portPRIORITY_GROUP_MASK;

		/* �ָ��޸ĵ����ȼ�Ϊԭ����ֵ*/
		*pucFirstUserPriorityRegister = ulOriginalPriority;
	}
	#endif /* conifgASSERT_DEFINED */

	/* ʹSoftSV��SysTickΪ������ȼ�*/
	portINT_IP2_REG |= portINT_SOFTSV_PRI;
	portINT_IP2_REG |= portINT_SYSTICK_PRI;

	/*���õδ�ʱ�����ж����ڣ���ʹ�����ж�*/
	vPortSetupTimerInterrupt();

	/*��ʼ���ٽ���Ƕ�׼�����*/
	uxCriticalNesting = 0;

	/* ��ʼ��һ������*/
	prvStartFirstTask();

	/*����Ӧ�����е��⣬������е���˵��û��������������*/
	return 0;
}
/*-----------------------------------------------------------*/
/* ȡ���Ѿ�ͨ��xPortStartScheduler()ִ�е�Ӳ��/�жϷ���������ã��������ȳ���ִֹͣ�к�
 * Ӳ��������ԭʼ״̬��
 * ����Ӳ����ر��жϵĴ��������ڹر������������ʹ�á�
 * */
void vPortEndScheduler( void )
{
	configASSERT( uxCriticalNesting == 1000UL );
}
/*-----------------------------------------------------------*/
//�����ٽ���
void vPortEnterCritical( void )
{
	portDISABLE_INTERRUPTS();
	uxCriticalNesting++;

	/* This is not the interrupt safe version of the enter critical function so
	assert() if it is being called from an interrupt context.  Only API
	functions that end in "FromISR" can be used in an interrupt.  Only assert if
	the critical nesting count is 1 to protect against recursive calls if the
	assert function also uses a critical section. */
	/* �ⲻ�ǽ����ٽ繦�ܵ��жϰ�ȫ�汾�����Ծ����˴��ж������ʹ��assert()������
	 * ֻ����FromISR��β�����жϵ�API�����������ٽ�Ƕ�׼���(uxCriticalNesting)Ϊ1ʱ
	 * ��ȥ��������֤����������һ���ٽ�ѡ��ʱ���Եݹ������Եĳ���*/
//	if( uxCriticalNesting == 1 )
//	{
//		��֤��ǰ�ж��Ƿ���0
//		configASSERT( ( portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK ) == 0 );
//		/* �ȴ���ȶ*/
//	}
}
/*-----------------------------------------------------------*/
//�˳��ٽ���
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
/* SoftSV�жϷ�����
 * ���������л���Ҳ�Ǻ��ĵ�һ���жϷ�����
 * */

void __attribute__((interrupt)) _SoftSV_exception (void)
{
	/*
	 * ���жϾ���ִ�������ǣ�
	 * 1��ѹջ��ǰ����
	 * 2���ر��жϣ���ȡ��һ������
	 * 3�������ж�
	 * 4����ջ��һ������
	 * */	/*����һ����̬�ֲ�����������configMAX_SYSCALL_INTERRUPT_PRIORITY���ú�׺���֣�Ƕ����е���*/
//	asm(".extern uxCriticalNesting");
	asm(".extern pxCurrentTCB");
	asm(".extern vTaskSwitchContext");
	/* 1��ѹջ��ǰ����*/
	asm("MOV R0,PSP"                    );
	asm("MOV R3,#pxCurrentTCB"          );//��ȡ��ǰTCB��ַ
	asm("LD.w R2,[R3]"					);//����ǰ����ĵ�һ��(��ջָ��)����R2
	//����R5-R13
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
	asm("ST.w [R2],R0");//����ǰ����Ķ�ջָ�����
	/* 2���ر��жϣ���ȡ��һ������*/
	asm("MOV R0,#%0"::"g"(configMAX_SYSCALL_INTERRUPT_PRIORITY));
	asm("LSL R0,#8"						);//�������ȼ���8λ
	asm("MOV R1,#0xFFFF00FF"			);//�ж���������ķ���
	asm("MOV R2,#0x40200000"			);//�жϿ��ƼĴ���
	asm("LD.w R3,[R2]"					);//ȡ���Ĵ���ֵ
	asm("ANL R3,R1"						);//ȥ��ԭֵ
	asm("ORL R3,R0"						);//������ֵ
	asm("ST.w [R2],R3"					);//����Ĵ���
	asm("MOV R2,#vTaskSwitchContext"    );
	asm("LJMP R2"                       );//���ú���vTaskSwitchContext()
	/* 3�������ж�*/
	asm("MOV R0,#0"                     );//���ж�
	asm("LSL R0,#8"						);//�������ȼ���8λ
	asm("MOV R1,#0xFFFF00FF"			);
	asm("MOV R2,#0x40200000"			);
	asm("LD.w R3,[R2]"					);//ȡ���Ĵ���ֵ
	asm("ANL R3,R1"						);//ȥ��ԭֵ
	asm("ORL R3,R0"						);//������ֵ
	asm("ST.w [R2],R3"					);//����Ĵ���
	/* ��ջ��һ������*/
	asm("MOV R3,#pxCurrentTCB"          );//��ȡ��ǰTCB��ַ
	asm("LD.w R1,[R3]"                  );//R3��ֵָ�����µ�����
	asm("LD.w R0,[R1]"                  );//ȡ������Ķ�ջ
	asm("LD.w R5,[R0++]"                );//��ջR5-R13
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
	/*�ж��˳��Զ�����R0-R4,PC,LR,xPSR��ֵ*/
}

/*
 * SysTick�жϷ������
 * */
void __attribute__((interrupt)) _SysTick_exception (void)
{
	/*�ر��ж�*/
	vPortRaiseBASEPRI();
	{
		/* ���Ӽ���ֵ */
		if( xTaskIncrementTick() != pdFALSE )//����ʱ�Ӽ�����xTickCount��ֵ
		{
			/* ͨ�����жϿ��ƺ�״̬�Ĵ���ICSR��bit28д��1����SoftSV������SoftSV�жϡ�
			 * �Ϳ�����SoftSV�жϷ������н��������л�*/
			portNVIC_INT_CTRL_REG = portNVIC_SOFTSVSET_BIT;
		}
	}
	vPortClearBASEPRIFromISR();//���ж�
}
/*-----------------------------------------------------------*/
/* ���е͹���ģʽ��
 * ��δ����
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

//���ý��Ķ�ʱ���Ľ��Ķ����������ж�
#if configOVERRIDE_DEFAULT_TICK_CONFIGURATION == 0

	void vPortSetupTimerInterrupt( void )
	{
		/* ������������ý��Ķ�ʱ�����ж�*/
		#if configUSE_TICKLESS_IDLE == 1
		{
			ulTimerCountsForOneTick = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ );
			xMaximumPossibleSuppressedTicks = portMAX_24_BIT_NUMBER / ulTimerCountsForOneTick;
			ulStoppedTimerCompensation = portMISSED_COUNTS_FACTOR / ( configCPU_CLOCK_HZ / configSYSTICK_CLOCK_HZ );
		}
		#endif /* configUSE_TICKLESS_IDLE */
		/* �����������ý��Ķ�ʱ���Ĵ���*/
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
/* ��ȡ��ǰ���ڴ�����ж��������*/
uint32_t __attribute__((noinline)) vPortGetIPSR(void)
{
	asm("MOV R1,#0x40200000"		);//�жϿ��ƼĴ���
	asm("MOV R2,#0x7F000000"		);//�ж���������
	asm("LD.w R1,[R1]"				);//ȡ���Ĵ���ֵ
	asm("ANL R1,R2"					);//��ȡ��Ӧֵ
	asm("MOV R2,#24"				);
	asm("LSR R0,R1,R2"				);//������λ��������ֵ��R0����
	asm("JMP LR"					);
}
/*-----------------------------------------------------------*/
#if( configASSERT_DEFINED == 1 )
	/*��֤�ж����ȼ�*/
	void vPortValidateInterruptPriority( void )
	{
	uint32_t ulCurrentInterrupt;
	uint8_t ucCurrentPriority;
		/* ��ȡ�ж�������� */
		ulCurrentInterrupt = vPortGetIPSR();

		/* �Ƿ�Ϊ�û����ж�?*/
		if( ulCurrentInterrupt >= portFIRST_USER_INTERRUPT_NUMBER )
		{
			/* ���ж����ȼ��Ĵ����в����ж����ȼ�*/
			ucCurrentPriority = pcInterruptPriorityRegisters[ ulCurrentInterrupt ];
			/* ���һ���жϷ����������ȼ�����configMAX_SYSCALL_INTERRUPT_PRIORITY
			 * (����FreeRTOSϵͳ�������ٻ��жϵ�һ�����ȼ�)�������������ֹͣ����Ҫʹ��
			 * ���ж����ȼ�����Ҫ����configMAX_SYSCALL_INTERRUPT_PRIORITY������
			 *
			 * ���ж����ȼ������ֱȸ����ȼ��ĸߣ������ж����ȼ�������ֵ�����
			 * configMAX_SYSCALL_INTERRUPT_PRIORITY�ߡ�
			 *
			 * ʹ��FreeRTOSӦ�ó���ӿڵ��жϵ����ȼ���ֵ��������ΪĬ��0��Ӧ�ø���
			 * configMAX_SYSCALL_INTERRUPT_PRIORITY��������ȷ����Ч��
			 *
			 * FreeRTOSϵͳ�����̺߳��ж�API�����ֿ���ȷ���жϽ��뾡���ܿ��ٺͼ�ࡣ
			 * */
			// MY_MSG("INT_IP9:0x%x",INT_IP9);
			// MY_MSG("\r\n��ǰ�ж�%d,��ǰ���ȼ�%d,ϵͳ����������ȼ�%d\r\n",ulCurrentInterrupt,ucCurrentPriority,ucMaxSysCallPriority);
			configASSERT( ucCurrentPriority >= ucMaxSysCallPriority );
		}

		/* ���ȼ����飺�ж�ϵͳ�У��ж����ȼ�����λ������Ϊ��ռ���ȼ��������ȼ���Ϊ��
		 * ����Ѿ����ܶ�����ȼ�����Ϊ��ռ���ȼ������������һЩbitλ����Ϊ�����ȼ���֤�ͻ�ʧ�ܡ�
		 *
		 * ���������������������֮ǰ�����ȼ���������Ϊ��ռ���ȼ�Ϊ0λ�������ȼ�Ϊ4λ
		 * �Ļ�������ᵼ�²���Ԥ��Ĵ���
		 * */
		configASSERT( ( portINT_CTL0 & portPRIORITY_GROUP_MASK ) <= ulMaxPRIGROUPValue );
	}

#endif /* configASSERT_DEFINED */



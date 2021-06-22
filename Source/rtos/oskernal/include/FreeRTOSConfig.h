#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 * ʹ��ʱӦ����ȷ����
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 * �������Ӳ����ƺ�����Ӧ��������
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 * ���²�����FreeRTOS��Ӧ�ó���ӿ�(API)�ļ���Ӧ�ã��û����Խ������ã�����FreeRTOS.org��վ��ȡ��
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
/* ȷ����׼��������ڱ�����򣬶�û�л�����*/
//#ifdef __ICCARM__
	#include <stdint.h>
//	extern uint32_t SystemCoreClock;
//#endif

#ifndef Message_RTOS_DEBUG
	#define RTOS_DEBUG_MSG(...)           	(void)0
	#warning(warning:UART should be initialized already)

#else
	#define MY_MSG(...)           	printf(__VA_ARGS__)
#endif


extern uint32_t RTOSSYSTEM_ERROR;

#define configUSE_PREEMPTION			1								//ʹ����ռʽ��������Ϊ0ʹ��Э��ʽ������
#define configUSE_IDLE_HOOK				0								//��������ҹ�����
#define configUSE_TICK_HOOK				0								//���Ĺҹ�����
#define configCPU_CLOCK_HZ				( 96000000 )					//CPUʱ��Ƶ��,��λHz
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )			//����ʱ��Ƶ�ʣ���λHz
#define configMAX_PRIORITIES			( 5 )							//������ȼ�
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )		//��С����ջ�ռ��С,��Ϊ��λ
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 20  * 1024 ) )	//�ܶ�ջ����(�ֽ�Ϊ��λ)
#define configMAX_TASK_NAME_LEN			( 10 )							//��������󳤶�
#define configUSE_TRACE_FACILITY		1								//��ѯ���ܿ��أ�ʹ��vTaskGetInfo������xTaskGetApplicationTaskTag������
#define configUSE_16_BIT_TICKS			0								//ʹ��16bit����orʹ��32bit����
#define configIDLE_SHOULD_YIELD			1								//���������л����ÿ���
#define configUSE_MUTEXES				1								//ʹ�û����ź�������
#define configQUEUE_REGISTRY_SIZE		20								//���м�¼����
#define configCHECK_FOR_STACK_OVERFLOW	2								//���ջ�������(2-0)
#define configUSE_RECURSIVE_MUTEXES		1								//ʹ�õݹ黥���ź�������
#define configUSE_MALLOC_FAILED_HOOK	1								//�����ڴ����ҹ�����
#define configUSE_APPLICATION_TASK_TAG	0								//ʹ��Ӧ�������ǩ���ã�xTaskGetApplicationTaskTag������vTaskSetApplicationTaskTag������
#define configUSE_COUNTING_SEMAPHORES	1								//ʹ�ü����źſ���
#define configGENERATE_RUN_TIME_STATS	0								//ͳ������ʱ�俪��

//========================================
#define INCLUDE_uxTaskGetStackHighWaterMark		0						//��ѯ����ʣ��ջ��С
#define INCLUDE_xSemaphoreGetMutexHolder		0
//========================================
/* Co-routine definitions. */
/* ���Ͻ��̶���*/
#define configUSE_CO_ROUTINES 		0									//ʹ�����ϳ�������
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )							//���ϳ���������ȼ�

/* Software timer definitions. */
/* �����ʱ������*/
#define configUSE_TIMERS				01								//��ʱ������
#define configTIMER_TASK_PRIORITY		( configMAX_PRIORITIES-1 )		//��ʱ���������ȼ�
#define configTIMER_QUEUE_LENGTH		10								//��ʱ�����г���
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 3 )//��ʱ��ջ��

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
/* ͨ���궨��Ϊ1����Ӧ��Ӧ�ó���ӿڣ�API��������Ϊ0�ر���Ӧ����*/
#define INCLUDE_vTaskPrioritySet		1				//�������ȼ����ú�������
#define INCLUDE_uxTaskPriorityGet		1				//�������ȼ���ȡ��������
#define INCLUDE_vTaskDelete				1				//����ɾ����������
#define INCLUDE_vTaskCleanUpResources	1				//����Դ����������
#define INCLUDE_vTaskSuspend			1				//����������������
#define INCLUDE_vTaskDelayUntil			1				//����ȷ��ʱ��������
#define INCLUDE_vTaskDelay				1				//������ʱ��������

/*���ȼ�����λ*/
#define configPRIO_BITS       			4        		/* 15 priority levels */

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
/* ��͵��ж����ȼ��������������ȼ��ĺ�����*/
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15//����ж����ȼ�

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
/* ������ȼ�����һЩ������FreeRTOS���жϰ�ȫAPI�����ĳ����жϷ������
 * ��Ҫ����������ȼ����������ȼ���ֵ���ͣ�*/

#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	1	//ϵͳ��������ж����ȼ�

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
/* ���Ľӿڲ���ж����ȼ���������Cortex-M�ӿ�����ͬһ�࣬���������κ����п⺯��*/
/*�ں��ж����ȼ�*/
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
/*configMAX_SYSCALL_INTERRUPT_PRIORITY���벻Ϊ0!!!Ϊ0�������÷���/
/*�����жϰ�ȫ�汾FreeRTOS API �������е�����ж����ȼ���*/
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
/*����У��*/
// #define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS();for( ;; ); }	
#define configASSERT(x) if((x) == 0)                        \
{                                                           \
    asm("MOV R0,R13");                                      \
    asm("MOV R1,#RTOSSYSTEM_ERROR");                        \
    asm("ST.W [R1],R0");                                    \
    RTOS_DEBUG_MSG("�����������,��������ַ��0x%08X\r\n",RTOSSYSTEM_ERROR);   \
    RTOS_DEBUG_MSG("Wrong parameters value: file %s on line %d\r\n",__FILE__,__LINE__);\
    RTOS_DEBUG_MSG("��release��debug��lst�ļ��в��ҳ�������\r\n");    \
    RTOS_DEBUG_MSG("����������ֹ��\r\n");   \
    while(1);                       \
}   


/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
/*FreeRTOS�жϺ����ӿ�CMSIS��׼���ƶ���*/
//#define vPortSVCHandler 	_SVCAll_exception			//SVCall�жϺ�
//#define xPortPendSVHandler 	_PendSV_exception			//PendSV�жϺ�
//#define xPortSysTickHandler _SysTick_exception			//SysTick�жϺ�

#endif /* FREERTOS_CONFIG_H */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 * 使用时应有明确定义
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 * 根据你的硬件设计和需求应用来调整
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 * 以下参数在FreeRTOS中应用程序接口(API)文件中应用，用户可以进行配置，可在FreeRTOS.org网站获取。
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
/* 确保标准库仅被用于编译程序，而没有汇编程序*/
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

#define configUSE_PREEMPTION			1								//使用抢占式调度器，为0使用协助式调度器
#define configUSE_IDLE_HOOK				0								//空闲任务挂钩开关
#define configUSE_TICK_HOOK				0								//节拍挂钩开关
#define configCPU_CLOCK_HZ				( 96000000 )					//CPU时钟频率,单位Hz
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )			//节拍时钟频率，单位Hz
#define configMAX_PRIORITIES			( 5 )							//最大优先级
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )		//最小任务栈空间大小,字为单位
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 20  * 1024 ) )	//总堆栈配置(字节为单位)
#define configMAX_TASK_NAME_LEN			( 10 )							//任务名最大长度
#define configUSE_TRACE_FACILITY		1								//查询功能开关（使用vTaskGetInfo函数和xTaskGetApplicationTaskTag函数）
#define configUSE_16_BIT_TICKS			0								//使用16bit节拍or使用32bit节拍
#define configIDLE_SHOULD_YIELD			1								//空闲任务切换配置开关
#define configUSE_MUTEXES				1								//使用互斥信号量配置
#define configQUEUE_REGISTRY_SIZE		20								//队列记录长度
#define configCHECK_FOR_STACK_OVERFLOW	2								//检查栈溢出开关(2-0)
#define configUSE_RECURSIVE_MUTEXES		1								//使用递归互斥信号量配置
#define configUSE_MALLOC_FAILED_HOOK	1								//分配内存错误挂钩开关
#define configUSE_APPLICATION_TASK_TAG	0								//使用应用任务标签配置（xTaskGetApplicationTaskTag函数和vTaskSetApplicationTaskTag函数）
#define configUSE_COUNTING_SEMAPHORES	1								//使用计数信号开关
#define configGENERATE_RUN_TIME_STATS	0								//统计运行时间开关

//========================================
#define INCLUDE_uxTaskGetStackHighWaterMark		0						//查询任务剩余栈大小
#define INCLUDE_xSemaphoreGetMutexHolder		0
//========================================
/* Co-routine definitions. */
/* 联合进程定义*/
#define configUSE_CO_ROUTINES 		0									//使用联合程序配置
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )							//联合程序最大优先级

/* Software timer definitions. */
/* 软件定时器定义*/
#define configUSE_TIMERS				01								//定时器开关
#define configTIMER_TASK_PRIORITY		( configMAX_PRIORITIES-1 )		//定时器任务优先级
#define configTIMER_QUEUE_LENGTH		10								//定时器队列长度
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 3 )//定时器栈长

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
/* 通过宏定义为1打开相应的应用程序接口（API）函数，为0关闭相应函数*/
#define INCLUDE_vTaskPrioritySet		1				//任务优先级设置函数开关
#define INCLUDE_uxTaskPriorityGet		1				//任务优先级获取函数开关
#define INCLUDE_vTaskDelete				1				//任务删除函数开关
#define INCLUDE_vTaskCleanUpResources	1				//任务源清理函数开关
#define INCLUDE_vTaskSuspend			1				//任务阻塞函数开关
#define INCLUDE_vTaskDelayUntil			1				//任务精确延时函数开关
#define INCLUDE_vTaskDelay				1				//任务延时函数开关

/*优先级比特位*/
#define configPRIO_BITS       			4        		/* 15 priority levels */

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
/* 最低的中断优先级被用在设置优先级的函数里*/
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15//最低中断优先级

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
/* 最高优先级用于一些调用了FreeRTOS中中断安全API函数的常规中断服务程序，
 * 不要超过这个优先级！（高优先级的值更低）*/

#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	1	//系统调用最大中断优先级

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
/* 核心接口层的中断优先级，和所有Cortex-M接口属于同一类，不依赖于任何特有库函数*/
/*内核中断优先级*/
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
/*configMAX_SYSCALL_INTERRUPT_PRIORITY必须不为0!!!为0有其他用法。/
/*设置中断安全版本FreeRTOS API 可以运行的最高中断优先级。*/
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
/*参数校验*/
// #define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS();for( ;; ); }	
#define configASSERT(x) if((x) == 0)                        \
{                                                           \
    asm("MOV R0,R13");                                      \
    asm("MOV R1,#RTOSSYSTEM_ERROR");                        \
    asm("ST.W [R1],R0");                                    \
    RTOS_DEBUG_MSG("检验参数出错,出错程序地址：0x%08X\r\n",RTOSSYSTEM_ERROR);   \
    RTOS_DEBUG_MSG("Wrong parameters value: file %s on line %d\r\n",__FILE__,__LINE__);\
    RTOS_DEBUG_MSG("在release或debug的lst文件中查找出错函数。\r\n");    \
    RTOS_DEBUG_MSG("程序运行终止。\r\n");   \
    while(1);                       \
}   


/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
/*FreeRTOS中断函数接口CMSIS标准名称定义*/
//#define vPortSVCHandler 	_SVCAll_exception			//SVCall中断宏
//#define xPortPendSVHandler 	_PendSV_exception			//PendSV中断宏
//#define xPortSysTickHandler _SysTick_exception			//SysTick中断宏

#endif /* FREERTOS_CONFIG_H */


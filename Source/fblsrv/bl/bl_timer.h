/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    bl_timer.h                                                  */
/* Description:  Header of Software timer implementation                     */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠            基线创建                          */
/*****************************************************************************/

#ifndef BL_TIMER_H
#define BL_TIMER_H

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "type.h"

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    timerStopped = 0, 
	timerRunning
}TimerState_e;

typedef struct
{
    UINT16 u16Initval;
    UINT16 u16Counter;
    TimerState_e eStatus;
}Timer_t;

typedef enum
{
	not_expired = 0,
	expired
}Timer_e;


/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define BUILDSPEC_TIMER_ARRAY   (0u)
#define TIMER_ARRAY_SIZE        (UINT8)(10u + BUILDSPEC_TIMER_ARRAY)

/*TODO: timer 0 could be renamed to response_deadline timer*/
#define startup_timer_handle        (0u)
#define crc_timer_handle            (0u)   /** reused timer        */
#define erase_timer_handle          (0u)   /** reused timer        */
#define dl_timer_handle             (0u)   /** reused timer        */
#define app_valid_timer_handle      (0u)   /** reused timer        */
#define S3_timer_handle             (1u)
#define NCr_timer_handle            (2u)
#define NBs_timer_handle            (3u)   /** reused timer        */
#define wdg_timer_handle            (4u)
#define reset_timer_handle          (5u)
#define securityDelay_timer_handle  (6u)
#define com_timer_handle            (7u)
#define mpu_timer_handle            (8u)
#define ota_timer_handle            (9u)


#define MSEC_DIVIDER (1u)

#define STARTUP_TO  ((UINT16)(20u/MSEC_DIVIDER))

#define P2CAN_SERVER_MAX_MS           ((UINT16)(50u))
#define P2SCAN_SERVER_MAX_MS          ((UINT16)(5000u))

#define P2CAN_SERVER_MAX              ((UINT16)(P2CAN_SERVER_MAX_MS/MSEC_DIVIDER))
#define P2SCAN_SERVER_MAX             ((UINT16)(P2SCAN_SERVER_MAX_MS/MSEC_DIVIDER))

#define S3SERVER_TO (P2SCAN_SERVER_MAX)

#define NCr_Timeout     ((UINT16)(25u/MSEC_DIVIDER))
#define NBs_Timeout     ((UINT16)(25u/MSEC_DIVIDER))

#define NCr_FotaTimeout     ((UINT16)(3000u/MSEC_DIVIDER))
#define NBs_FotaTimeout     ((UINT16)(3000u/MSEC_DIVIDER))

#define SECURITYDELAY_TO ((UINT16)(10000u/MSEC_DIVIDER))

#define Com_FotaTimeout    ((UINT16)(6000u/MSEC_DIVIDER))

#define Ota_Timeout    ((UINT16)(10000u/MSEC_DIVIDER))

#define S3OTASERVER_TO (7000)


/*TODO: check these values*/
/* These values modify the timeouts, because we need to response or react
 * before the timeouts expire */
/* Time after wait response for a request to manage an action in one shot without timer checking */
#define LONG_PROC_LOOP_MSEC     500u
/* Time after request to manage an action in one shot without timer checking */
#define PROC_LOOP_MSEC            1u

#define P2xCAN_TO    (P2SCAN_SERVER_MAX - LONG_PROC_LOOP_MSEC)
#define P2CAN_TO     (P2CAN_SERVER_MAX - PROC_LOOP_MSEC)


/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开变量申明                                                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 外部公开函数声明                                                          */
/*---------------------------------------------------------------------------*/
void ApiInitTimer(UINT8 u8TimerIndex, UINT16 u16Initval);
void ApiResetTimer(UINT8 u8TimerIndex);
Timer_e ApiCheckTimer(UINT8 u8TimerIndex);
void ApiStopTimer(UINT8 u8TimerIndex);
bool ApiIsTimerStopped(UINT8 u8TimerIndex);
void ApiTickTimers(void);


#endif /* BL_TIMER_H */

/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ll_target_pc.h                                              */
/* Description:  Common header for types and type manipulation               */
/*               macros for all source file of the bootloader                */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠            基线创建                          */
/*****************************************************************************/

#ifndef LL_TARGET_H_
#define LL_TARGET_H_


/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "bl_timer.h"
#include "type.h"

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    batteryOk=0u,
	battVoltageTooHigh=0x92u,
	battVoltageTooLow=0x93u
}BatteryType_e;


/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define SET_UINT8(reg,bit)    ((reg) |=  (VUINT8)(1u << (bit)))
#define CLEAR_UINT8(reg,bit)  ((reg) &=  (VUINT8)(~(VUINT8)(1u << (bit))))

#define LOG2_FLASHSECTORSIZE 15 /* 2^15 = 0x8000 = 32KB */
    /*Min*/
#define  BATT_9V        (945u)
    /* Max */
#define  BATT_16V       (1750u)

#define TIMER_CLOCK_KHZ  5000u   /*5M频率，配置为5000*/

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
/* 外部公开函数声明                                                          */
/*---------------------------------------------------------------------------*/
extern void ApiLlReset(void);
extern void ApiLlCopyBuffer(UINT8* pu8Dest, const UINT8* pu8Src, UINT16 u16Count);
extern Timer_e ApiLlTimer(void);
extern void ApiLlEcuInit(void);
extern UINT32 ApiLlGetSecuritySeed(UINT32 u32OldSeed);
extern void ApiLlFill(UINT8 *pu8Dst, UINT8 u8Value, UINT16 u16Count);
extern void ApiLlApplicationPrestart(void);
extern BatteryType_e ApiLlJudgeBatteryVoltage(void);
extern void ApiSetResetSource(void);
extern BOOL ApiLlMpuPowerOn(void);

/* Library function */
extern void __DI(void);

#endif /* _LL_TARGET_H_ */


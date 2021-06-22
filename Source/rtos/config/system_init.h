/**
  ********************************************************************
  * 文件名  system_init.h
  * 作  者   ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2018-3-15
  * 描  述  该文件提供了外设库包含的头文件与系统时钟配置相关的宏定义。
  *
  *********************************************************************
*/
#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "KF32A_BASIC.h"
#include "kf32a_basic_adc.h"
#include "kf32a_basic_aes.h"
#include "kf32a_basic_bkp.h"
#include "kf32a_basic_can.h"
#include "kf32a_basic_cfgl.h"
#include "kf32a_basic_cmp.h"
#include "kf32a_basic_crc.h"
#include "kf32a_basic_dac.h"
#include "kf32a_basic_dma.h"
#include "kf32a_basic_flash.h"
#include "kf32a_basic_gpio.h"
#include "kf32a_basic_i2c.h"
#include "kf32a_basic_int.h"
#include "kf32a_basic_iwdt.h"
#include "kf32a_basic_led.h"
#include "kf32a_basic_lcd.h"
#include "kf32a_basic_op.h"
#include "kf32a_basic_osc.h"
#include "kf32a_basic_pclk.h"
#include "kf32a_basic_pm.h"
#include "kf32a_basic_qei.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_rtc.h"
#include "kf32a_basic_spi.h"
#include "kf32a_basic_sysctl.h"
#include "kf32a_basic_systick.h"
#include "kf32a_basic_tim.h"
#include "kf32a_basic_usart.h"
#include "kf32a_basic_usb.h"
#include "kf32a_basic_wwdt.h"

/* 系统时钟选择 */
#define SYSCLK_FREQ_HSE     //外部高频
// #define SYSCLK_FREQ_HSI     //内部高频

/* PPL时钟选择 */
//#define PPL_CLK_48MHz	48000000
//#define PPL_CLK_60MHz	60000000
//#define PPL_CLK_72MHz	72000000
#define PPL_CLK_96MHz	96000000
//#define PPL_CLK_120MHz	120000000

/* 主时钟选择 */
// #define SYS_CLK_1MHz   1000000
// #define SYS_CLK_4MHz   4000000
// #define SYS_CLK_8MHz   8000000
// #define SYS_CLK_16MHz 16000000
// #define SYS_CLK_24MHz 24000000
// #define SYS_CLK_48MHz 48000000
#define SYS_CLK_96MHz 96000000

#define HFC_CLK_48MHz 48000000 

static void SetSysClock(void);
void SystemInit(void);

void bsw_init(void);
void app_init(void);

#endif /* SYS_H_ */

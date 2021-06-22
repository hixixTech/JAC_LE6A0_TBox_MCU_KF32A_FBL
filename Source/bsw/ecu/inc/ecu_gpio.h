/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_gpio.h
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/05/17
 * BY:           feifei.xu
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

#ifndef __ECU_GPIO_H__
#define __ECU_GPIO_H__

/*****************************************************************************
**#include 
*****************************************************************************/
#include "type.h"
/*****************************************************************************
**  typedef
*****************************************************************************/
#define LCV_24V

typedef enum
{
	GPIO_CODEC_EN = 0,			/*1		PD1  输出*/
	GPIO_ACC_DET,				/*2		PD2  输入*/
	GPIO_4G_EN,					/*3		PD3  输出*/
	GPIO_MCU_SOS_IN,			/*4		PD4  输入*/
	GPIO_BLE_RESERT_B,			/*5		PD5  输出*/
	GPIO_WKUP_MCU_4G,			/*6		PD6  输入*/
	GPIO_WKUP_MCU_BLE,			/*7		PD7  输入*/
	GPIO_WKUP_4G,				/*8		PH6  输出*/
	GPIO_4G_STARTINGUP,			/*9		PD13 输入*/
	GPIO_NRST_4G,				/*10	PD14 输出*/
	GPIO_4G_STATUS,				/*11	PH14 输出*/
	GPIO_4G_VBUS_CNTL,			/*12	PH12 输出*/
	GPIO_PWR_CTL_4G,			/*13	PH13 输出*/
	GPIO_SHUT_DOWN_4G,			/*14	PH15 输出*/
	GPIO_ADC2_INPUT,			/*15	PA0  模拟*/
	GPIO_ADC1_INPUT,			/*16	PA1  模拟*/
	GPIO_ADC0_INPUT,			/*17	PA2  模拟*/
	GPIO_BAT_DET,				/*18	PA3  模拟*/
	GPIO_NMBAT_TEMP,			/*19	PA5  模拟*/
	GPIO_NMBAT_DET,				/*20	PA6  模拟*/
	GPIO_USB_DET,				/*21	PA7  模拟*/
	GPIO_MCUSW_EN,				/*22	PB0  输出*/
	GPIO_BT_MCU_RX,				/*23	PB2  复用*/
	GPIO_BT_MCU_TX,				/*24	PB3  复用*/
	GPIO_CAN0_STB,				/*25	PB4  输出*/
	GPIO_NMBAT_SW,				/*26	PB5  输出*/
	GPIO_DEBUG_MCU_RX,			/*27	PB6  复用*/
	GPIO_DEBUG_MCU_TX,			/*28	PB7  复用*///TX0
	GPIO_GSENSOR_EN,			/*29	PB8  输出*/
	GPIO_CAN1_STB,				/*30	PB9  输出*/
	GPIO_WDI1,					/*31	PB10 输出*/
	GPIO_MCU_SRS_IN,			/*32	PB11 输入*/
	GPIO_CAN2_STB,				/*33	PB12 输出*/
	GPIO_MIC_EN,				/*34	PB13 输出*/
	GPIO_SPI3_MISO_3V3,			/*35	PB14 复用*/
	GPIO_SP13_MOSI_3V3,			/*36	PB15 复用*/
	GPIO_SPI3_CLK_3V3,			/*37	PF0  复用*/
	GPIO_SPI3_CS_N_3V3,			/*38	PF1  复用*/
	GPIO_SPI3_MPU_3V3,			/*39	PF2  输出*/
	GPIO_SPI3_MCU_3V3,			/*40	PF3  输入*/
	GPIO_4G_WORKPWM_3V3,		/*41	PF7  输入*/
	GPIO_NMBAT_CHRG,			/*42	PG0  输出*/
	GPIO_BAT_DET_CTL,			/*43	PG1  输出*/
	GPIO_AUX_BOX_MUTE,			/*44	PG3  输出*/
	GPIO_DOG_EN,				/*45	PG4  输出*/
	GPIO_CAN0_RX,				/*46	PG5  复用*/
	GPIO_CAN0_TX,				/*47	PC0  复用*/
	GPIO_I2C0_SDA_MCU,			/*48	PC1  复用*/
	GPIO_I2C0_SCL_MCU,			/*49	PC2  复用*/
	GPIO_CAN1_RX,				/*50	PC3  复用*/
	GPIO_CAN1_TX,				/*51	PC4  复用*/
	GPIO_USART0_RX_MCU,			/*52	PC7  复用*/
	GPIO_USART0_TX_MCU,			/*53	PC8  复用*/
	GPIO_RTC,					/*54	PC9  输出*/
	GPIO_LIS_INT2,				/*55	PC10 输入*/
	GPIO_SPK_BOX_MUTE,			/*56	PC11 输出*/
	GPIO_SOSLED_CTL,			/*57	PC12 输出*/
	GPIO_DIV8_EN,				/*58	PC13 输出*/
	GPIO_CAN2_RX,				/*59	PG6  输出*/
	GPIO_CAN2_TX,				/*60	PG7  输出*/
#ifdef LCV_24V
	//24V
	GPIO_CAN_LED,				/*61	PA14 输出*/	
	GPIO_LTE_LED,				/*62	PA13 输出*/
	GPIO_GPS_LED,				/*63	PA12 输出*/
	GPIO_VBATD_OUT_EN,			/*64	PE4  输出*/
	GPIO_MCU_BCALL_IN,			/*65	PE3  输入*/
#endif
	GPIO_NUM_MAX				//dont't delete this lin
}GPIO_Name_E;
/*****************************************************************************
** Constant Macro Definition
*****************************************************************************/


/*****************************************************************************
** System Macro Definition
*****************************************************************************/


/*****************************************************************************
** Action Macro Definition
*****************************************************************************/

/*****************************************************************************
** Config Macro Definition
*****************************************************************************/


/*****************************************************************************
** Task Macro Definition
*****************************************************************************/


/*****************************************************************************
** Variables
*****************************************************************************/

/*****************************************************************************
** Constants
*****************************************************************************/


/*****************************************************************************
** Function prototypeseb
*****************************************************************************/
void Ecu_Gpio_Init(void);
UINT8 ApiGpioInputRead(GPIO_Name_E e_idx);
UINT8 ApiGpioOutputRead(GPIO_Name_E e_idx);
void ApiGpioOutputLow(GPIO_Name_E e_idx);
void ApiGpioOutputHigh(GPIO_Name_E e_idx);
void ApiGpioOutputTurn(GPIO_Name_E e_idx);
/*****************************************************************************
** other
*****************************************************************************/


/****************************************************************************/

#endif	//__ECU_GPIO_H__

/*****************************************************************************
** End File
*****************************************************************************/


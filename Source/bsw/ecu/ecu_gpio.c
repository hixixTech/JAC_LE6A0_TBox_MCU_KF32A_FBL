/******************************************************************************
 * (C) Copyright 2020 YuChiZhiNeng-Automotive
 * FILE NAME:    
 * DESCRIPTION:
 * 
 * DATE BEGUN:   
 * BY:           
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

/*****************************************************************************
** #include 
*****************************************************************************/
#include "system_init.h"
#include "ecu_gpio.h"
/*****************************************************************************
** #define
*****************************************************************************/
#define CAN_STB_HIG()        GPIO_Set_Output_Data_Bits(GPIOF_SFR,GPIO_PIN_MASK_0,Bit_SET)  //设置为高电平
#define CAN_STB_LOW()        GPIO_Set_Output_Data_Bits(GPIOF_SFR,GPIO_PIN_MASK_0,Bit_RESET)//设置为低电平

#define PROT_A GPIOA_SFR
#define PROT_B GPIOB_SFR
#define PROT_C GPIOC_SFR
#define PROT_D GPIOD_SFR
#define PROT_E GPIOE_SFR
#define PROT_F GPIOF_SFR
#define PROT_G GPIOG_SFR
#define PROT_H GPIOH_SFR

#define PORT_A_CLK PCLK_CTL0_GPIOACLKEN
#define PORT_B_CLK PCLK_CTL0_GPIOBCLKEN
#define PORT_C_CLK PCLK_CTL0_GPIOCCLKEN
#define PORT_D_CLK PCLK_CTL0_GPIODCLKEN
#define PORT_E_CLK PCLK_CTL0_GPIOECLKEN
#define PORT_F_CLK PCLK_CTL0_GPIOFCLKEN
#define PORT_G_CLK PCLK_CTL0_GPIOGCLKEN
#define PORT_H_CLK PCLK_CTL0_GPIOHCLKEN

#define GPIO_GetMask(Pin)	((UINT16)((0x0001)<<Pin)) 
/*****************************************************************************
** typedef
*****************************************************************************/
typedef struct
{
	GPIO_Name_E 		Name;
	GPIO_SFRmap* 		GPIOx;
	UINT16				Pin;
	GPIOMode_TypeDef	Mode;
	GPIOSpeed_TypeDef	Speed;
	GPIOPOD_TypeDef		OpenDrain;
	FunctionalState		PullUp;
	FunctionalState		PullDown;
	UINT8 				GPIO_RMP;
	BOOL				InitSts;
}GPIO_CFG_S;

typedef struct
{
	GPIO_Name_E 		Name;
	UINT32				EintNum;
	FunctionalState		Enable;
	FunctionalState		Rise;
	FunctionalState		Fall;
	UINT32 				Sourse;
}EXIT_INT_S;
/*****************************************************************************
** global variable
*****************************************************************************/
const GPIO_CFG_S c_gpio_cfg[GPIO_NUM_MAX] = 
{	/*	Name				PORT			Pin				Mode			Speed			开漏控制	Pullup	PullDown	重映射*/																																			\
/*1*/	{GPIO_CODEC_EN 		, GPIOD_SFR,	GPIO_Pin_Num_1	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*2*/	{GPIO_ACC_DET		, GPIOD_SFR,	GPIO_Pin_Num_2	, GPIO_MODE_IN 	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	TRUE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*3*/	{GPIO_4G_EN			, GPIOD_SFR,	GPIO_Pin_Num_3	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*4*/	{GPIO_MCU_SOS_IN	, GPIOD_SFR,	GPIO_Pin_Num_4	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	TRUE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*5*/	{GPIO_BLE_RESERT_B	, GPIOD_SFR,	GPIO_Pin_Num_5	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*6*/	{GPIO_WKUP_MCU_4G	, GPIOD_SFR,	GPIO_Pin_Num_6	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	TRUE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*7*/	{GPIO_WKUP_MCU_BLE	, GPIOD_SFR,	GPIO_Pin_Num_7	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE, 		TRUE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*8*/	{GPIO_WKUP_4G		, GPIOH_SFR,	GPIO_Pin_Num_6	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*9*/	{GPIO_4G_STARTINGUP , GPIOD_SFR,	GPIO_Pin_Num_13	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	TRUE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*10*/	{GPIO_NRST_4G		, GPIOD_SFR,	GPIO_Pin_Num_14	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*11*/	{GPIO_4G_STATUS		, GPIOH_SFR,	GPIO_Pin_Num_14	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*12*/	{GPIO_4G_VBUS_CNTL	, GPIOH_SFR,	GPIO_Pin_Num_12	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*13*/	{GPIO_PWR_CTL_4G	, GPIOH_SFR,	GPIO_Pin_Num_13	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*14*/	{GPIO_SHUT_DOWN_4G	, GPIOH_SFR,	GPIO_Pin_Num_15	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*15*/	{GPIO_ADC2_INPUT	, GPIOA_SFR,	GPIO_Pin_Num_0	, GPIO_MODE_AN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*16*/	{GPIO_ADC1_INPUT	, GPIOA_SFR,	GPIO_Pin_Num_1	, GPIO_MODE_AN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*17*/	{GPIO_ADC0_INPUT	, GPIOA_SFR,	GPIO_Pin_Num_2	, GPIO_MODE_AN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*18*/	{GPIO_BAT_DET		, GPIOA_SFR,	GPIO_Pin_Num_3	, GPIO_MODE_AN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*19*/	{GPIO_NMBAT_TEMP	, GPIOA_SFR,	GPIO_Pin_Num_5	, GPIO_MODE_AN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*20*/	{GPIO_NMBAT_DET		, GPIOA_SFR,	GPIO_Pin_Num_6	, GPIO_MODE_AN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*21*/	{GPIO_USB_DET		, GPIOA_SFR,	GPIO_Pin_Num_7	, GPIO_MODE_AN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*22*/	{GPIO_MCUSW_EN		, GPIOB_SFR,	GPIO_Pin_Num_0	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*23*/	{GPIO_BT_MCU_RX		, GPIOB_SFR,	GPIO_Pin_Num_2	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, TRUE,  		FALSE, 	GPIO_RMP_AF6_USART3,	LOW},	
/*24*/	{GPIO_BT_MCU_TX		, GPIOB_SFR,	GPIO_Pin_Num_3	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF6_USART3,	LOW},	
/*25*/	{GPIO_CAN0_STB		, GPIOB_SFR,	GPIO_Pin_Num_4	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	HIGH},	
/*26*/	{GPIO_NMBAT_SW		, GPIOB_SFR,	GPIO_Pin_Num_5	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*27*/	{GPIO_DEBUG_MCU_RX	, GPIOB_SFR,	GPIO_Pin_Num_6	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, TRUE,  		FALSE, 	GPIO_RMP_AF5_USART0,	LOW},	
/*28*/	{GPIO_DEBUG_MCU_TX	, GPIOB_SFR,	GPIO_Pin_Num_7	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF5_USART0,	LOW},	
/*29*/	{GPIO_GSENSOR_EN	, GPIOB_SFR,	GPIO_Pin_Num_8	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*30*/	{GPIO_CAN1_STB		, GPIOB_SFR,	GPIO_Pin_Num_9	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	HIGH},	
/*31*/	{GPIO_WDI1			, GPIOB_SFR,	GPIO_Pin_Num_10	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*32*/	{GPIO_MCU_SRS_IN	, GPIOB_SFR,	GPIO_Pin_Num_11	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	TRUE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*33*/	{GPIO_CAN2_STB		, GPIOB_SFR,	GPIO_Pin_Num_12	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	HIGH},	
/*34*/	{GPIO_MIC_EN		, GPIOB_SFR,	GPIO_Pin_Num_13	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*35*/	{GPIO_SPI3_MISO_3V3	, GPIOB_SFR,	GPIO_Pin_Num_14	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF5_USART2,	LOW},	
/*36*/	{GPIO_SP13_MOSI_3V3	, GPIOB_SFR,	GPIO_Pin_Num_15	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF5_USART2,	LOW},	
/*37*/	{GPIO_SPI3_CLK_3V3	, GPIOF_SFR,	GPIO_Pin_Num_0	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF7_SPI3,		LOW},	
/*38*/	{GPIO_SPI3_CS_N_3V3	, GPIOF_SFR,	GPIO_Pin_Num_1	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF7_SPI3,		LOW},	
/*39*/	{GPIO_SPI3_MPU_3V3	, GPIOF_SFR,	GPIO_Pin_Num_2	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*40*/	{GPIO_SPI3_MCU_3V3	, GPIOF_SFR,	GPIO_Pin_Num_3	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	TRUE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*41*/	{GPIO_4G_WORKPWM_3V3, GPIOF_SFR,	GPIO_Pin_Num_7	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*42*/	{GPIO_NMBAT_CHRG	, GPIOG_SFR,	GPIO_Pin_Num_0	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE, 	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*43*/	{GPIO_BAT_DET_CTL	, GPIOG_SFR,	GPIO_Pin_Num_1	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*44*/	{GPIO_AUX_BOX_MUTE	, GPIOG_SFR,	GPIO_Pin_Num_2	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*45*/	{GPIO_DOG_EN		, GPIOG_SFR,	GPIO_Pin_Num_4	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*46*/	{GPIO_CAN0_RX		, GPIOG_SFR,	GPIO_Pin_Num_5	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF9_CAN4,		LOW},	
/*47*/	{GPIO_CAN0_TX		, GPIOC_SFR,	GPIO_Pin_Num_0	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF9_CAN4,		LOW},	
/*48*/	{GPIO_I2C0_SDA_MCU	, GPIOC_SFR,	GPIO_Pin_Num_1	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_OD, TRUE,  		FALSE,	GPIO_RMP_AF8_I2C2,		LOW},	
/*49*/	{GPIO_I2C0_SCL_MCU	, GPIOC_SFR,	GPIO_Pin_Num_2	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_OD, TRUE,  		FALSE,	GPIO_RMP_AF8_I2C2,		LOW},	
/*50*/	{GPIO_CAN1_RX		, GPIOC_SFR,	GPIO_Pin_Num_3	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF9_CAN1,		LOW},	
/*51*/	{GPIO_CAN1_TX		, GPIOC_SFR,	GPIO_Pin_Num_4	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF9_CAN1,		LOW},	
/*52*/	{GPIO_USART0_RX_MCU	, GPIOC_SFR,	GPIO_Pin_Num_7	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, TRUE,  		FALSE,	GPIO_RMP_AF5_USART1,	LOW},	
/*53*/	{GPIO_USART0_TX_MCU	, GPIOC_SFR,	GPIO_Pin_Num_8	, GPIO_MODE_RMP	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF5_USART1,	LOW},	
/*54*/	{GPIO_RTC			, GPIOC_SFR,	GPIO_Pin_Num_9	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*55*/	{GPIO_LIS_INT2		, GPIOC_SFR,	GPIO_Pin_Num_10	, GPIO_MODE_IN	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	TRUE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*56*/	{GPIO_SPK_BOX_MUTE	, GPIOC_SFR,	GPIO_Pin_Num_11	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*57*/	{GPIO_SOSLED_CTL	, GPIOC_SFR,	GPIO_Pin_Num_12	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*58*/	{GPIO_DIV8_EN		, GPIOC_SFR,	GPIO_Pin_Num_13	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*59*/	{GPIO_CAN2_RX		, GPIOG_SFR,	GPIO_Pin_Num_6 ,  GPIO_MODE_RMP , GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,		FALSE,	GPIO_RMP_AF9_CAN3,		LOW},	
/*60*/	{GPIO_CAN2_TX		, GPIOG_SFR,	GPIO_Pin_Num_7 ,  GPIO_MODE_RMP , GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,		FALSE,	GPIO_RMP_AF9_CAN3,		LOW},	
#ifdef LCV_24V
/*61*/	{GPIO_CAN_LED		, GPIOA_SFR,	GPIO_Pin_Num_14	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},
/*62*/	{GPIO_LTE_LED		, GPIOA_SFR,	GPIO_Pin_Num_13	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*63*/	{GPIO_GPS_LED		, GPIOA_SFR,	GPIO_Pin_Num_12	, GPIO_MODE_OUT	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,  	FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*64*/	{GPIO_VBATD_OUT_EN	, GPIOE_SFR,	GPIO_Pin_Num_4 ,  GPIO_MODE_OUT , GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,		FALSE,	GPIO_RMP_AF0_SYSTEM,	LOW},	
/*65*/	{GPIO_MCU_BCALL_IN	, GPIOE_SFR,	GPIO_Pin_Num_3 ,  GPIO_MODE_IN 	, GPIO_HIGH_SPEED, GPIO_POD_PP, FALSE,		TRUE,	GPIO_RMP_AF0_SYSTEM,	LOW}	
#endif
};

static const EXIT_INT_S s_c_st_exit_cfg[] = 
{
/*1*/	{GPIO_RTC,	INT_EXTERNAL_INTERRUPT_9,	TRUE,	FALSE,	TRUE,	INT_EXTERNAL_SOURCE_PC}
};

/*****************************************************************************
** static variables
*****************************************************************************/


/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/
static Ecu_Gpio_PortReset(void);
static Ecu_Gpio_PortClkEnable(void);


/*****************************************************************************
** function prototypes
*****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/**
 * Function Name: Ecu_Gpio_Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/18, feifei.xu create this function
 ****************************************************************************/
void Ecu_Gpio_Init(void)
{
	UINT16 u16_index = 0;
	UINT16 u16_pin_mask = 0;

	GPIO_CFG_S* p_gpio_cfg = c_gpio_cfg;
	EXIT_INT_S* p_exit_cfg = s_c_st_exit_cfg;

	EINT_InitTypeDef EXIT_InitStructure;

	Ecu_Gpio_PortReset();
	Ecu_Gpio_PortClkEnable();

	for(u16_index = 0;u16_index < GPIO_NUM_MAX; u16_index++)
	{	
		u16_pin_mask = GPIO_GetMask(p_gpio_cfg->Pin);
		
		GPIO_Write_Mode_Bits(p_gpio_cfg->GPIOx, u16_pin_mask, p_gpio_cfg->Mode);			//模式配置
		GPIO_Pull_Down_Enable(p_gpio_cfg->GPIOx,u16_pin_mask, p_gpio_cfg->PullDown);		//下拉使能
		GPIO_Pull_Up_Enable(p_gpio_cfg->GPIOx, u16_pin_mask, p_gpio_cfg->PullUp);			//上拉使能
		GPIO_Open_Drain_Enable(p_gpio_cfg->GPIOx,u16_pin_mask,p_gpio_cfg->OpenDrain);		//开漏控制
		
		if(GPIO_MODE_OUT == p_gpio_cfg->Mode)
		{
			GPIO_Speed_Config(p_gpio_cfg->GPIOx,u16_pin_mask,p_gpio_cfg->Speed);			//速度配置
			
			if( LOW == p_gpio_cfg->InitSts)
			{
				GPIO_Set_Output_Data_Bits(p_gpio_cfg->GPIOx,u16_pin_mask,Bit_RESET);
			}
			else
			{
				GPIO_Set_Output_Data_Bits(p_gpio_cfg->GPIOx,u16_pin_mask,Bit_SET);
			}
		}
		
		if(GPIO_MODE_RMP == p_gpio_cfg->Mode)												//判断是否重映射
		{
			GPIO_Pin_RMP_Config(p_gpio_cfg->GPIOx, p_gpio_cfg->Pin, p_gpio_cfg->GPIO_RMP);	//配置重映射
		}
		p_gpio_cfg++;
	}

	// for(u16_index = 0; u16_index < (sizeof(s_c_st_exit_cfg)/sizeof(EXIT_INT_S)); u16_index++)
	// {
	// 	EXIT_InitStructure.m_Line = p_exit_cfg->EintNum;
	// 	EXIT_InitStructure.m_Mask = p_exit_cfg->Enable;
	// 	EXIT_InitStructure.m_Rise = p_exit_cfg->Rise;
	// 	EXIT_InitStructure.m_Fall = p_exit_cfg->Fall;
	// 	EXIT_InitStructure.m_Source = p_exit_cfg->Sourse;
	// 	INT_External_Configuration(&EXIT_InitStructure);
	// 	INT_External_Source_Enable(p_exit_cfg->EintNum,p_exit_cfg->Sourse);
	// }
	// INT_All_Enable (TRUE);      //使能总中断
	
}
/****************************************************************************/
/**
 * Function Name: Ecu_Gpio_PortReset
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/18, feifei.xu create this function
 ****************************************************************************/
static Ecu_Gpio_PortReset(void)
{
	GPIO_Reset(PROT_A);
	GPIO_Reset(PROT_B);
	GPIO_Reset(PROT_C);
	GPIO_Reset(PROT_D);
	GPIO_Reset(PROT_E);
	GPIO_Reset(PROT_F);
	GPIO_Reset(PROT_G);
	GPIO_Reset(PROT_H);
}
/****************************************************************************/
/**
 * Function Name: Ecu_Gpio_PortClkEnable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/18, feifei.xu create this function
 ****************************************************************************/
static Ecu_Gpio_PortClkEnable(void)
{
	PCLK_CTL0_Peripheral_Clock_Enable(PORT_A_CLK,TRUE);
	PCLK_CTL0_Peripheral_Clock_Enable(PORT_B_CLK,TRUE);
	PCLK_CTL0_Peripheral_Clock_Enable(PORT_C_CLK,TRUE);
	PCLK_CTL0_Peripheral_Clock_Enable(PORT_D_CLK,TRUE);
	PCLK_CTL0_Peripheral_Clock_Enable(PORT_E_CLK,TRUE);
	PCLK_CTL0_Peripheral_Clock_Enable(PORT_F_CLK,TRUE);
	PCLK_CTL0_Peripheral_Clock_Enable(PORT_G_CLK,TRUE);
	PCLK_CTL0_Peripheral_Clock_Enable(PORT_H_CLK,TRUE);
}
/****************************************************************************/
/**
 * Function Name: ApiGpioInputRead
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
UINT8 ApiGpioInputRead(GPIO_Name_E e_idx)
{
	if(e_idx < GPIO_NUM_MAX)
	{
		return (UINT8)GPIO_Read_Input_Data_Bit(c_gpio_cfg[e_idx].GPIOx,GPIO_GetMask(c_gpio_cfg[e_idx].Pin));
	}
}
/****************************************************************************/
/**
 * Function Name: ApiGpioOutputRead
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
UINT8 ApiGpioOutputRead(GPIO_Name_E e_idx)
{
	if(e_idx < GPIO_NUM_MAX)
	{
		return (UINT8)GPIO_Read_Output_Data_Bit(c_gpio_cfg[e_idx].GPIOx,GPIO_GetMask(c_gpio_cfg[e_idx].Pin));
	}
}
/****************************************************************************/
/**
 * Function Name: ApiGpioOutputLow
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiGpioOutputLow(GPIO_Name_E e_idx)
{
	if(e_idx < GPIO_NUM_MAX)
	{
		GPIO_Set_Output_Data_Bits(c_gpio_cfg[e_idx].GPIOx,GPIO_GetMask(c_gpio_cfg[e_idx].Pin),Bit_RESET);
	}
}
/****************************************************************************/
/**
 * Function Name: ApiGpioOutputHigh
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiGpioOutputHigh(GPIO_Name_E e_idx)
{
	if(e_idx < GPIO_NUM_MAX)
	{
		GPIO_Set_Output_Data_Bits(c_gpio_cfg[e_idx].GPIOx,GPIO_GetMask(c_gpio_cfg[e_idx].Pin),Bit_SET);
	}
}
/****************************************************************************/
/**
 * Function Name: ApiGpioOutputTurn
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiGpioOutputTurn(GPIO_Name_E e_idx)
{
	if(e_idx < GPIO_NUM_MAX)
	{
		GPIO_Toggle_Output_Data_Config(c_gpio_cfg[e_idx].GPIOx,GPIO_GetMask(c_gpio_cfg[e_idx].Pin));
	}
}
/****************************************************************************/
/**
 * Function Name: _EINT0_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT0_exception(void)
{

}
/****************************************************************************/
/**
 * Function Name: _EINT1_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT1_exception(void)
{
	
}
/****************************************************************************/
/**
 * Function Name: _EINT2_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT2_exception(void)
{
	
}
/****************************************************************************/
/**
 * Function Name: _EINT3_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT3_exception(void)
{
	
}
/****************************************************************************/
/**
 * Function Name: _EINT4_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT4_exception(void)
{

}
/****************************************************************************/
/**
 * Function Name: _EINT9TO5_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT9TO5_exception(void)
{

}
/****************************************************************************/
/**
 * Function Name: _EINT15TO10_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_EINT15TO10_exception(void)
{

}
/*****************************************************************************
** End File
*****************************************************************************/

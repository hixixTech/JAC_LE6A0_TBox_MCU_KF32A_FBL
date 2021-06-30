/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_adc.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/05/20
 * BY:           feifei.xu
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
#include "ecu_adc.h"

/*****************************************************************************
** #define
*****************************************************************************/


/*****************************************************************************
** typedef
*****************************************************************************/
typedef struct
{
	ADC_SFRmap* ADCx;
	UINT32 u32_clk;
	UINT32 u32_clk_div;
	FunctionalState ScanMode;
	UINT32 u32_ContinuousMode;
	UINT32 u32_DataAlign;
	FunctionalState ExternalTrig_EN;
	UINT32 u32_ExternalTrig;
	FunctionalState HPExternalTrig_EN;
	UINT32 u32_HPExternalTrig;
	UINT32 u32_VoltageRef;
	UINT32 u32_NumOfConv;
	UINT32 u32_NumOfHPConv;
}ADC_CFG_S;


/*****************************************************************************
** global variable
*****************************************************************************/


/*****************************************************************************
** static variables
*****************************************************************************/
static const ADC_CFG_S s_c_st_adc0_cfg = 
{
	ADC0_SFR,
	ADC_HFCLK,
	ADC_CLK_DIV_8,
	FALSE,
	ADC_SINGLE_MODE,
	ADC_DATAALIGN_RIGHT,
	FALSE,
	ADC_EXTERNALTRIG_T1TRGO,
	FALSE,
	ADC_HPEXTERNALTRIG_CCP1_CH1,
	ADC_REF_AVDD,
	1,
	1
};

/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/
static void Ecu_Adc_Delay(UINT32 u32_cnt);
static void Ecu_Adc_Configure(const ADC_CFG_S* p_adc_cfg);
static UINT16 Ecu_Adc_GetValue(ADC_SFRmap* ADCx,ADC_CHANNEL_E u32_channal);
/*****************************************************************************
** function prototypes
*****************************************************************************/
/****************************************************************************/
/**
 * Function Name: Ecu_Adc_Delay
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/21, feifei.xu create this function
 ****************************************************************************/
static void Ecu_Adc_Delay(UINT32 u32_cnt)
{
	while(u32_cnt--);
}

/****************************************************************************/
/**
 * Function Name: Ecu_Adc_Configure
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/21, feifei.xu create this function
 ****************************************************************************/
static void Ecu_Adc_Configure(const ADC_CFG_S* p_adc_cfg)
{
	ADC_InitTypeDef adcStruct;

	ADC_Reset (p_adc_cfg->ADCx);									//复位ADC外设并打开ADC的时钟
	adcStruct.m_Clock = p_adc_cfg->u32_clk;							//使能ADC时钟
	adcStruct.m_ClockDiv = p_adc_cfg->u32_clk_div;					//使能ADC分频
	adcStruct.m_ScanMode = p_adc_cfg->ScanMode;						//扫描模式
	adcStruct.m_ContinuousMode = p_adc_cfg->u32_ContinuousMode; 	//不使用连续转换功能，只对设置的通道采样一遍
	adcStruct.m_DataAlign = p_adc_cfg->u32_DataAlign; 				//转换的结果右对齐
	adcStruct.m_ExternalTrig_EN = p_adc_cfg->HPExternalTrig_EN; 	//常规通道失能外部条件触发ADC转换，
	adcStruct.m_ExternalTrig = p_adc_cfg->u32_ExternalTrig; 		//常规通道外部触发信号源配置为T1TRGO
	adcStruct.m_HPExternalTrig_EN = p_adc_cfg->HPExternalTrig_EN; 	//高优先级通道失能外部条件触发ADC转换，
	adcStruct.m_HPExternalTrig = p_adc_cfg->u32_HPExternalTrig; 	//高优先级通道触发信号源配置为CCP1的通道1
	adcStruct.m_VoltageRef=p_adc_cfg->u32_VoltageRef; 				//转换的参考电压源配置为VDDA
	adcStruct.m_NumOfConv = p_adc_cfg->u32_NumOfConv; 				//扫描的长度配置为1个通道
	adcStruct.m_NumOfHPConv = p_adc_cfg->u32_NumOfHPConv; 			//高优先级通道的扫描长度
	ADC_Configuration (p_adc_cfg->ADCx, &adcStruct);
	ADC_Cmd (p_adc_cfg->ADCx, TRUE); 								//ADC使能
	Ecu_Adc_Delay(0xFF);  											//ADC使能后需要一段短暂的时间，等待电路充电完毕后，才能去采样，否者可能存在较大的采样误差
}


/****************************************************************************/
/**
 * Function Name: Ecu_Adc_GetValue
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/21, feifei.xu create this function
 ****************************************************************************/
static UINT16 Ecu_Adc_GetValue(ADC_SFRmap* ADCx,ADC_CHANNEL_E u32_channal)
{
	ADC_Regular_Channel_Config(ADCx,(UINT32)u32_channal,0x01);
	ADC_Software_Start_Conv(ADCx);
	while(!ADC_Get_INT_Flag(ADCx,ADC_INT_EOC));

	return ADC_Get_Conversion_Value(ADCx);
}
/****************************************************************************/
/**
 * Function Name: ApiAdc0Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/21, feifei.xu create this function
 ****************************************************************************/
void ApiAdc0Init(void)
{
	Ecu_Adc_Configure(&s_c_st_adc0_cfg);
}
/****************************************************************************/
/**
 * Function Name: ApiAdcVoltRead
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/21, feifei.xu create this function
 ****************************************************************************/
UINT32 ApiAdcVoltRead(ADC_CHANNEL_E u32_channal)
{
	UINT32 u32_temp = 0;
	UINT32 u32_adcmv = 0;
	
	u32_temp = (UINT32)Ecu_Adc_GetValue(s_c_st_adc0_cfg.ADCx,u32_channal);
	u32_adcmv = (UINT32)((((3300*10000)/4095)*u32_temp)/10000);

	return u32_adcmv;
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_i2c.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/05/25
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
#include "ecu_i2c.h"

/*****************************************************************************
** #define
*****************************************************************************/
#define I2C_1K_CLK				(1000)
#define I2C_GET_BAUDRATE(x)		((HFC_CLK_48MHz / (x * I2C_1K_CLK))/2)

#define I2C_BAUDRATE_500K (500)
#define I2C_BAUDRATE_400K (400)


/*****************************************************************************
** typedef
*****************************************************************************/
typedef struct
{
	I2C_SFRmap* I2Cx;
	UINT32 u32_Mode;
	UINT32 u32_Clk;
	UINT32 u32_Badr;
	UINT32 u32_MasterSlave;
	UINT32 u32_Baurate;
	BOOL b_AckEn;
	UINT32 u32_AckData;
	/* data */
}I2C_CFG_S;

/*****************************************************************************
** global variable
*****************************************************************************/

/*****************************************************************************
** static variables
*****************************************************************************/


/*****************************************************************************
** static constants
*****************************************************************************/
static const I2C_CFG_S s_c_i2c2_cfg = 
{
	I2C2_SFR,
	I2C_MODE_I2C,
	I2C_CLK_HFCLK,
	I2C_MODE_SMBUSHOST,
	I2C_BUFRADDRESS_7BIT,
	I2C_BAUDRATE_400K,
	TRUE,
	I2C_ACKDATA_ACK
};

/*****************************************************************************
** static function prototypes
*****************************************************************************/
static void Ecu_I2c_Configure(const I2C_CFG_S* p_i2c_cfg);
static void Ecu_I2c_WriteByte(I2C_SFRmap* I2Cx,UINT8 u8_addr,UINT8* u8_data,UINT16 u16_len);
/*****************************************************************************
** function prototypes
*****************************************************************************/
/****************************************************************************/
/**
 * Function Name: Ecu_I2c_Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/04, feifei.xu create this function
 ****************************************************************************/
static void Ecu_I2c_Configure(const I2C_CFG_S* p_i2c_cfg)
{
	I2C_InitTypeDef newStruct_I2C;	

	I2C_Reset(p_i2c_cfg->I2Cx);//复位
	newStruct_I2C.m_Mode = p_i2c_cfg->u32_Mode;				  	//配置为I2C模式
	newStruct_I2C.m_ClockSource = p_i2c_cfg->u32_Clk; 		  	//时钟源选择SCLK
	newStruct_I2C.m_MasterSlave = p_i2c_cfg->u32_MasterSlave; 	//SMBus类型选择从机模式
	newStruct_I2C.m_BADR10 = p_i2c_cfg->u32_Badr;				//从机地址位
	newStruct_I2C.m_BaudRateL = I2C_GET_BAUDRATE(p_i2c_cfg->u32_Baurate); 	//I2C波特率低电平时间周期	这个值 不能小于3
	newStruct_I2C.m_BaudRateH = I2C_GET_BAUDRATE(p_i2c_cfg->u32_Baurate); 	//I2C波特率高电平时间周期	这个值 不能小于3
	newStruct_I2C.m_AckEn = p_i2c_cfg->b_AckEn;					//使能应答
	newStruct_I2C.m_AckData = p_i2c_cfg->u32_AckData;			//选择应答信号为ACK
	I2C_Configuration(p_i2c_cfg->I2Cx, &newStruct_I2C);
	I2C_Clear_INTERRUPT_Flag(p_i2c_cfg->I2Cx);					//清楚中断标志位
	I2C_ISIE_INT_Enable(p_i2c_cfg->I2Cx,TRUE);
	I2C_Cmd(p_i2c_cfg->I2Cx,TRUE); 				   				//使能I2C模块
}
/****************************************************************************/
/**
 * Function Name: Ecu_I2c_WriteByte
 * Description: 写数据
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/04, feifei.xu create this function
 ****************************************************************************/
static void Ecu_I2c_WriteByte(I2C_SFRmap* I2Cx,UINT8 u8_addr,UINT8* u8_data,UINT16 u16_len)
{
	UINT16  u16_i= 0;

	I2C_Cmd(I2Cx,TRUE);						//使能I2C

	I2C_Generate_START(I2Cx,TRUE);			//使能起始信号
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));	//等待起始信号稳定

	I2C_Clear_INTERRUPT_Flag(I2Cx);			//清中断标志位
	I2C_SendData8(I2Cx,u8_addr);    		//发送从机地址
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));  	//等待发送完成
	while(I2C_Get_Ack_Fail_Flag(I2Cx));	 	//判断是否有ACK应答，如没有则停止，如果有则继续发

	for(u16_i = 0; u16_i < u16_len; u16_i++)
	{
		I2C_Clear_INTERRUPT_Flag(I2Cx);
		I2C_SendData(I2Cx,u8_data[u16_i]);		//发送数据
		I2C_Clear_INTERRUPT_Flag(I2Cx);		 	//清I2C中断标志位ISIF位
		while(!I2C_Get_INTERRUPT_Flag(I2Cx));  	//等待发送完成
		while(I2C_Get_Ack_Fail_Flag(I2Cx));	 	//判断是否有ACK应答，如没有则停止，如果有则继续发
	}

	I2C_Generate_STOP(I2Cx,TRUE); 			//使能停止信号
	while(!I2C_Get_INTERRUPT_Flag(I2Cx)); 	//等待停止完成

	I2C_Clear_INTERRUPT_Flag(I2Cx);			//清I2C中断标志位ISIF位
	I2C_Clear_Stop_Flag(I2Cx);				//清I2C停止标志位PIF位
	I2C_Cmd(I2Cx,FALSE);					//关闭I2C模组

}

static void Ecu_I2c_ReadByte(I2C_SFRmap* I2Cx,UINT8 u8_addr,UINT8* u8_data,UINT16 u16_len)
{
	UINT16 u16_i = 0;

	I2C_Cmd(I2Cx,TRUE);						//使能I2C

	I2C_Generate_START(I2Cx,TRUE);			//使能起始信号
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));	//等待起始信号稳定

	I2C_Clear_INTERRUPT_Flag(I2Cx);			//清中断标志位
	I2C_SendData8(I2Cx,u8_addr);    		//发送从机地址
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));  	//等待发送完成
	while(I2C_Get_Ack_Fail_Flag(I2Cx));	 	//判断是否有ACK应答，如没有则停止，如果有则继续发

	for(u16_i = 0; u16_i < u16_len; u16_i++)
	{
		I2C_Clear_INTERRUPT_Flag(I2Cx);
		u8_data[u16_i] = I2C_ReceiveData(I2Cx);
	}

}
/****************************************************************************/
/**
 * Function Name: ApiI2cInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/17, feifei.xu create this function
 ****************************************************************************/
void ApiI2cInit(void)
{
	Ecu_I2c_Configure(&s_c_i2c2_cfg);
}
/****************************************************************************/
/**
 * Function Name: ApiI2cSendByte
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/17, feifei.xu create this function
 ****************************************************************************/
void ApiI2cSendByte(UINT8 u8_addr,UINT8 u8_data)
{
	Ecu_I2c_WriteByte(s_c_i2c2_cfg.I2Cx,u8_addr,&u8_data,0x01);
}
/****************************************************************************/
/**
 * Function Name: ApiI2cSendNumByte
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/17, feifei.xu create this function
 ****************************************************************************/
void ApiI2cSendNumByte(UINT8 u8_addr,UINT8* u8_data,UINT16 u16_len)
{
	Ecu_I2c_WriteByte(s_c_i2c2_cfg.I2Cx,u8_addr,u8_data,u16_len);
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


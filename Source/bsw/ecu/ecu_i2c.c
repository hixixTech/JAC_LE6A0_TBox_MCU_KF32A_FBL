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
#include "ecu_misc.h"
/*****************************************************************************
** #define
*****************************************************************************/
#define I2C_1K_CLK				(1000)
#define I2C_GET_BAUDRATE(x)		((HFC_CLK_48MHz / (x * I2C_1K_CLK))/2)

#define I2C_BAUDRATE_500K	(500)
#define I2C_BAUDRATE_400K	(400)
#define I2C_BAUDRATE_300K	(300)
#define I2C_BAUDRATE_200K	(200)
#define I2C_BAUDRATE_100K	(100)
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
	I2C_ACKDATA_NO_ACK
};

/*****************************************************************************
** static function prototypes
*****************************************************************************/
static void Ecu_I2c_Configure(const I2C_CFG_S* p_i2c_cfg);
static void Ecu_I2c_WriteData(I2C_SFRmap* I2Cx,UINT8 u8_salve_addr,UINT8 u8_reg_addr,UINT8* u8_data,UINT16 u16_len);
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
	newStruct_I2C.m_BaudRateL = I2C_GET_BAUDRATE(400); 	//I2C波特率低电平时间周期	这个值 不能小于3
	newStruct_I2C.m_BaudRateH = I2C_GET_BAUDRATE(400); 	//I2C波特率高电平时间周期	这个值 不能小于3
	newStruct_I2C.m_AckEn = p_i2c_cfg->b_AckEn;					//使能应答
	newStruct_I2C.m_AckData = p_i2c_cfg->u32_AckData;			//选择应答信号为ACK
	I2C_Configuration(p_i2c_cfg->I2Cx, &newStruct_I2C);

	I2C_Start_INT_Enable(p_i2c_cfg->I2Cx,TRUE);
	I2C_Clear_INTERRUPT_Flag(p_i2c_cfg->I2Cx);					//清楚中断标志位
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
static void Ecu_I2c_WriteData(I2C_SFRmap* I2Cx,UINT8 u8_salve_addr,UINT8 u8_reg_addr,UINT8* u8_data,UINT16 u16_len)
{
	UINT16  u16_i= 0;

	I2C_Cmd(I2Cx,TRUE);										//使能I2C

	I2C_Generate_START(I2Cx,TRUE);							//使能起始信号
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));					//等待起始信号稳定

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清中断标志位
	I2C_SendData8(I2Cx,(u8_salve_addr << 0x01) | 0x00);    	//发送从机地址
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));  					//等待发送完成
	while(I2C_Get_Ack_Fail_Flag(I2Cx));						//判断是否有ACK应答，如没有则停止，如果有则继续发

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清中断标志位
	I2C_SendData8(I2Cx,u8_reg_addr);    					//发送寄存器地址
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));  					//等待发送完成
	while(I2C_Get_Ack_Fail_Flag(I2Cx));	 					//判断是否有ACK应答，如没有则停止，如果有则继续发

	for(u16_i = 0; u16_i < u16_len; u16_i++)
	{
		I2C_Clear_INTERRUPT_Flag(I2Cx);						//清中断标志位
		I2C_SendData(I2Cx,u8_data[u16_i]);					//发送数据
		while(!I2C_Get_INTERRUPT_Flag(I2Cx));  				//等待发送完成
		while(I2C_Get_Ack_Fail_Flag(I2Cx));	 				//判断是否有ACK应答，如没有则停止，如果有则继续发
	}

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清中断标志位
	I2C_Generate_STOP(I2Cx,TRUE); 							//使能停止信号
	while(!I2C_Get_INTERRUPT_Flag(I2Cx)); 					//等待停止完成

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清I2C中断标志位ISIF位
	I2C_Clear_Stop_Flag(I2Cx);								//清I2C停止标志位PIF位
	I2C_Cmd(I2Cx,FALSE);									//关闭I2C模组
}
/****************************************************************************/
/**
 * Function Name: Ecu_I2c_ReadData
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/19, feifei.xu create this function
 ****************************************************************************/
static void Ecu_I2c_ReadData(I2C_SFRmap* I2Cx,UINT8 u8_salve_addr,UINT8 u8_reg_addr,UINT8* u8_data,UINT16 u16_len)
{
	UINT16 u16_i = 0;

	I2C_Cmd(I2Cx,TRUE);										//使能I2C

	I2C_Generate_START(I2Cx,TRUE);							//使能起始信号
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));					//等待起始信号稳定

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清中断标志位
	I2C_SendData8(I2Cx,(u8_salve_addr << 0x01) | 0x00);    	//发送从机地址
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));  					//等待发送完成
	while(I2C_Get_Ack_Fail_Flag(I2Cx));	 					//判断是否有ACK应答，如没有则停止，如果有则继续发

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清中断标志位
	I2C_SendData8(I2Cx,u8_reg_addr);    					//发送寄存器地址
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));  					//等待发送完成
	while(I2C_Get_Ack_Fail_Flag(I2Cx));	 					//判断是否有ACK应答，如没有则停止，如果有则继续发

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清中断标志位
	I2C_Generate_START(I2Cx,TRUE);							//重新使能起始信号
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));					//等待起始信号稳定

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清中断标志位
	I2C_SendData8(I2Cx,(u8_salve_addr << 0x01) | 0x01);    	//发送从机地址
	while(!I2C_Get_INTERRUPT_Flag(I2Cx));  					//等待发送完成
	while(I2C_Get_Ack_Fail_Flag(I2Cx));	 					//判断是否有ACK应答，如没有则停止，如果有则继续发

	for(u16_i = 0; u16_i < u16_len; u16_i++)
	{

		if(u16_i == (u16_len-1))
		{
			I2C_Ack_DATA_Config(I2Cx,I2C_ACKDATA_NO_ACK);
		}
		else
		{	
			I2C_Ack_DATA_Config(I2Cx,I2C_ACKDATA_ACK);
		}

		I2C_Clear_INTERRUPT_Flag(I2Cx);						//清中断标志位
		while(!I2C_Get_INTERRUPT_Flag(I2Cx));  				//等待发送完成
		while(I2C_Get_Receive_Buff_Flag(I2Cx))
		{
			u8_data[u16_i] = I2C_ReceiveData(I2Cx);
		}
	}
	delayus(1);
	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清中断标志位
	I2C_Generate_STOP(I2Cx,TRUE); 							//使能停止信号
	while(!I2C_Get_INTERRUPT_Flag(I2Cx)); 					//等待停止完成

	I2C_Clear_INTERRUPT_Flag(I2Cx);							//清I2C中断标志位ISIF位
	I2C_Clear_Stop_Flag(I2Cx);								//清I2C停止标志位PIF位
	I2C_Cmd(I2Cx,FALSE);									//关闭I2C模组

}
/****************************************************************************/
/**
 * Function Name: ApiI2cInit
 * Description: I2C初始化
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
 * Function Name: ApiI2cIntDisable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiI2cIntDisable(void)
{
	INT_Interrupt_Enable(INT_I2C2,FALSE);
}
/****************************************************************************/
/**
 * Function Name: ApiI2cSendByte
 * Description: 写入1个字节
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/17, feifei.xu create this function
 ****************************************************************************/
void ApiI2cWriteByte(UINT8 u8_salve_addr,UINT8 u8_reg_addr,UINT8 u8_data)
{
	Ecu_I2c_WriteData(s_c_i2c2_cfg.I2Cx,u8_salve_addr,u8_reg_addr,&u8_data,0x01);
}
/****************************************************************************/
/**
 * Function Name: ApiI2cSendNumByte
 * Description: 写入多个字节
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/17, feifei.xu create this function
 ****************************************************************************/
void ApiI2cWriteNumByte(UINT8 u8_salve_addr,UINT8 u8_reg_addr,UINT8* u8_data,UINT16 u16_len)
{
	Ecu_I2c_WriteData(s_c_i2c2_cfg.I2Cx,u8_salve_addr,u8_reg_addr,u8_data,u16_len);
}
/****************************************************************************/
/**
 * Function Name: ApiI2cReadByte
 * Description: 读1个字节
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/19, feifei.xu create this function
 ****************************************************************************/
void ApiI2cReadByte(UINT8 u8_salve_addr,UINT8 u8_reg_addr,UINT8* u8_data)
{
	Ecu_I2c_ReadData(s_c_i2c2_cfg.I2Cx,u8_salve_addr,u8_reg_addr,u8_data,0x01);
}
/****************************************************************************/
/**
 * Function Name: ApiI2cReadNumByte
 * Description: 读多个字节
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/21, feifei.xu create this function
 ****************************************************************************/
void ApiI2cReadNumByte(UINT8 u8_salve_addr,UINT8 u8_reg_addr,UINT8* u8_data,UINT16 u16_len)
{
	Ecu_I2c_ReadData(s_c_i2c2_cfg.I2Cx,u8_salve_addr,u8_reg_addr,u8_data,u16_len);
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


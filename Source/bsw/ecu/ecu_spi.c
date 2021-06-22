/******************************************************************************
 * (C) Copyright 2020 YuChiZhiNeng-Automotive
 * FILE NAME:    SPI.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/05/14
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
#include "ecu_spi.h"

/*****************************************************************************
** #define
*****************************************************************************/
#define SPI_MASTER 1

/*****************************************************************************
** typedef
*****************************************************************************/
typedef struct
{
	SPI_SFRmap* SPIx;
	UINT32 u32_Mode;
	UINT32 u32_Clk;
	UINT32 u32_FirstBit;
	UINT32 u32_CKP;
	UINT32 u32_CKE;
	UINT32 u32_DataSize;
	UINT32 u32_BaudRate;
}SPI_CFG_S;

/*****************************************************************************
** global variable
*****************************************************************************/
const SPI_CFG_S c_st_spi1_cfg = 
{
	SPI1_SFR,
	SPI_MODE_MASTER_CLKDIV4,				//主模式主时钟4分频 18M
	SPI_CLK_HFCLK,							//外设高速时钟
	SPI_FIRSTBIT_MSB,						//MSB
	SPI_CKP_LOW,							//SCK空闲为高
	SPI_CKE_1EDGE,							//第一个时钟开始发送数据
	SPI_DATASIZE_8BITS, 					//8bit
	11 									//Fck_spi=Fck/2(m_BaudRate+1)=10us
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


/*****************************************************************************
** function prototypes
*****************************************************************************/
void Ecu_Spi_Init(void)
{
	Ecu_Spi_Configure(&c_st_spi1_cfg);
}
/****************************************************************************/
/**
 * Function Name: Ecu_Spi_Configuration
 * Description: SPI配置
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/14, feifei.xu create this function
 ****************************************************************************/
void Ecu_Spi_Configure(SPI_CFG_S* p_spi_cfg)
{

	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.m_Mode = p_spi_cfg->u32_Mode;				//设置模式
	SPI_InitStructure.m_Clock = p_spi_cfg->u32_Clk;				//外设高速时钟
	SPI_InitStructure.m_FirstBit = p_spi_cfg->u32_FirstBit;		//MSB
	SPI_InitStructure.m_CKP = p_spi_cfg->u32_CKP;				//SCK空闲为高
	SPI_InitStructure.m_CKE = p_spi_cfg->u32_CKE;				//第一个时钟开始发送数据
	SPI_InitStructure.m_DataSize = p_spi_cfg->u32_DataSize;		//DataSize
	SPI_InitStructure.m_BaudRate = p_spi_cfg->u32_BaudRate;		//波特率

	/*SPI配置*/
	SPI_Reset(p_spi_cfg->SPIx);
	SPI_Configuration(p_spi_cfg->SPIx, &SPI_InitStructure);		//写入结构体配置
	SPI_Cmd(p_spi_cfg->SPIx,TRUE); 								//使能
	
}
/****************************************************************************/
/**
 * Function Name: Ecu_Spi_SendData
 * Description: 发送SPI数据
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/14, feifei.xu create this function
 ****************************************************************************/
void Ecu_Spi_SendData(SPI_SFRmap* SPIx, UINT8 u8_tx_data)
{
	UINT8 u8_data = 0x00;
	if(SPI_Get_Transmit_Buf_Flag(SPIx) == RESET)
	{
		SPI_I2S_SendData8(SPIx,u8_tx_data);
	}
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/

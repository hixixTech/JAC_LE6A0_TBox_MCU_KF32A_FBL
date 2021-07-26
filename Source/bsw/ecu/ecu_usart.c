/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_usart.c
 * DESCRIPTION:该文件提供了串口例程相关配置函数，包括
 *          	+ 串口发送
 *          	+ 串口异步配置
 *          	+ 串口同步配置
 *          	+ 串口接收中断使能
 * DATE BEGUN:   2021/05/15
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
#include "ecu_usart.h"

/*****************************************************************************
** #define
*****************************************************************************/
#define FIFO_BUFF_SIZE (1024)

/*****************************************************************************
** typedef
*****************************************************************************/
typedef struct
{
	USART_SFRmap* USARTx;
	UINT32 u32_Mode;
	UINT32 u32_HalfDuplexClk;
	UINT32 u32_TransferDir;
	UINT16 u32_BaudRateClk;
	UINT16 u16_BaudRateInteger;
	UINT16 u16_BaudRateNumerator;
	UINT16 u16_BaudRateDenominator;
	UINT32 u32_Tx_Channal;
	InterruptIndex  Peripheral;
	FunctionalState Rx_Int_Enable;
	FunctionalState Tx_Int_Enable;
	FunctionalState Rx_Dma_Enable;
	FunctionalState Tx_Dma_Enable;
}USART_CFG_S;

typedef struct
{
	UINT16 u16_rx_len;
	UINT8 u8_rx_buff[FIFO_BUFF_SIZE];

	UINT16 u16_tx_len;
	UINT8 u8_tx_buff[FIFO_BUFF_SIZE];
}USART_FIFO_S;
/*****************************************************************************
** global variable
*****************************************************************************/

/*****************************************************************************
** static variables
*****************************************************************************/
/* 波特率 =Fck/(16*z（1+x/y)) */
//4800	  z:208    x:0	  y:0
//9600	  z:104    x:0	  y:0
//19200   z:52	   x:0	  y:0
//115200  z:8	   x:1	  y:13

static const USART_CFG_S s_c_debug_cfg =
{
	USART0_SFR, 							//USART选择：USART0
	USART_MODE_FULLDUPLEXASY,				//USART模式配置：异步
	USART_SLAVE_CLOCKSOURCE_EXTER,			//时钟源选择：从时钟源
	USART_DIRECTION_FULL_DUPLEX,			//传输方向：全双工
	USART_CLK_HFCLK,						//波特率发生器时钟源：外部高速时钟源
	26, 									//波特率整数部分
	0,										//波特率小数分子部分
	0,										//波特率小数分母部分
	USART_U7816R_PASSAGEWAY_TX0, 			//USART通道选择：TX0
	INT_USART0,								//USART 中断向量
	FALSE,									//USART RX中断使能
	FALSE,									//USART TX中断使能
	FALSE,									//USART RX DMA使能
	FALSE									//USART TX DMA使能
};

static const USART_CFG_S s_c_ble_cfg =
{
	USART3_SFR, 							//USART选择：USART3
	USART_MODE_FULLDUPLEXASY,				//USART模式配置：异步
	USART_SLAVE_CLOCKSOURCE_EXTER,			//时钟源选择：从时钟源
	USART_DIRECTION_FULL_DUPLEX,			//传输方向：全双工
	USART_CLK_HFCLK,						//波特率发生器时钟源：外部高速时钟源
	26, 									//波特率整数部分
	0,										//波特率小数分子部分
	0,										//波特率小数分母部分
	USART_U7816R_PASSAGEWAY_TX0, 			//USART通道选择：TX0
	INT_USART2,								//USART 中断向量
	FALSE,									//USART RX中断使能
	FALSE,									//USART TX中断使能
	FALSE,									//USART RX DMA使能
	FALSE									//USART TX DMA使能
};

static const USART_CFG_S s_c_mpu_cfg =
{
	USART1_SFR, 							//USART选择：USART1
	USART_MODE_FULLDUPLEXASY,				//USART模式配置：异步
	USART_SLAVE_CLOCKSOURCE_EXTER,			//时钟源选择：从时钟源
	USART_DIRECTION_FULL_DUPLEX,				//传输方向：全双工
	USART_CLK_HFCLK,						//波特率发生器时钟源：外部高速时钟源
	3, 							     		//波特率整数部分
	1,										//波特率小数分子部分
	12,										//波特率小数分母部分  48M/(16*z(1+x/y))
	USART_U7816R_PASSAGEWAY_TX0, 			//USART通道选择：TX0
	INT_USART1,								//USART 中断向量
	FALSE,									//USART RX中断使能
	FALSE,									//USART TX中断使能
	FALSE,									//USART RX DMA使能
	FALSE									//USART TX DMA使能
};

static USART_FIFO_S s_debug_var;
static USART_FIFO_S s_ble_var;
static USART_FIFO_S s_mpu_var;


static USART_FIFO_S s_test_var;

/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/


/*****************************************************************************
** function prototypes
*****************************************************************************/\
/****************************************************************************/
/**
 * Function Name: Ecu_Usart_Init
 * Description: 串口配置
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/15, zhe.wang create this function
 ****************************************************************************/
static void Ecu_Usart_Configure(USART_CFG_S* p_usart_cfg)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.m_Mode = p_usart_cfg->u32_Mode;									//USART模式配置：异步
	USART_InitStructure.m_HalfDuplexClkSource = p_usart_cfg->u32_HalfDuplexClk;			//时钟源选择：从时钟源
	USART_InitStructure.m_TransferDir = p_usart_cfg->u32_TransferDir;					//传输方向：全双工
	USART_InitStructure.m_WordLength = USART_WORDLENGTH_8B;								//数据长度：8位
	USART_InitStructure.m_StopBits = USART_STOPBITS_1;									//停止位：		1位
	USART_InitStructure.m_Bit9SEL = USART_BIT9_PAR;										//第9位数据选择
	USART_InitStructure.m_Parity = USART_PARITY_ODD;									//校验位：偶校验
	USART_InitStructure.m_ReceiveOrder = USART_RECEIVE_LSB;								//接收次序：LSB
	USART_InitStructure.m_TransmitOrder = USART_TRANSMIT_LSB;							//发送次序：LSB
	USART_InitStructure.m_BRAutoDetect = USART_ABRDEN_OFF;								//自动波特率使能：OFF		
	USART_InitStructure.m_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;			//硬件流控制：无	
	USART_InitStructure.m_BaudRateBRCKS = p_usart_cfg->u32_BaudRateClk;					//波特率发生器时钟源：外部高速时钟源
	USART_InitStructure.m_BaudRateInteger = p_usart_cfg->u16_BaudRateInteger;			//波特率整数部分
	USART_InitStructure.m_BaudRateNumerator = p_usart_cfg->u16_BaudRateNumerator;		//波特率小数分子部分
	USART_InitStructure.m_BaudRateDenominator = p_usart_cfg->u16_BaudRateDenominator;	//波特率小数分母部分

	USART_Reset(p_usart_cfg->USARTx);                                       			//USART复位
	USART_Configuration(p_usart_cfg->USARTx,&USART_InitStructure);     					//USART配置
    USART_Passageway_Select_Config(p_usart_cfg->USARTx,p_usart_cfg->u32_Tx_Channal);	//UASRT选择TX0通道
	USART_Clear_Transmit_BUFR_INT_Flag(p_usart_cfg->USARTx);                			//USARTx发送BUF清零
	USART_RESHD_Enable (p_usart_cfg->USARTx, TRUE);						   				//使能RESHD位
	USART_Cmd(p_usart_cfg->USARTx,TRUE);                                    			//USARTx使能

	USART_Clear_Receive_BUFR_INT_Flag(p_usart_cfg->USARTx);								//清除接收中断标志位
	USART_Clear_Transmit_BUFR_INT_Flag(p_usart_cfg->USARTx);							//清除发送中断标志位
	USART_RDR_INT_Enable(p_usart_cfg->USARTx,p_usart_cfg->Rx_Int_Enable);				//接收中断使能
	USART_TXE_INT_Enable(p_usart_cfg->USARTx,p_usart_cfg->Tx_Int_Enable);				//发送中断使能
	INT_Interrupt_Enable(p_usart_cfg->Peripheral,p_usart_cfg->Rx_Int_Enable || p_usart_cfg->Tx_Int_Enable);	//USART 中断配置
	USART_Receive_DMA_INT_Enable(p_usart_cfg->USARTx,p_usart_cfg->Rx_Dma_Enable);		//接收DMA使能
	USART_Transmit_DMA_INT_Enable(p_usart_cfg->USARTx,p_usart_cfg->Tx_Dma_Enable);		//发送DMA使能
	INT_All_Enable(p_usart_cfg->Rx_Int_Enable || p_usart_cfg->Tx_Int_Enable);			//全局中断使能
}
/****************************************************************************/
/**
 * Function Name: Ecu_Usart_Send
 * Description: 串口发送
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/15, feifei.xu create this function
 ****************************************************************************/
static INT32 Ecu_Usart_SendData(USART_SFRmap* USARTx,const UINT8* Databuf,const UINT32 length)
{
	UINT32 u32_i;
	
	if((NULL == USARTx) || (NULL == Databuf) || (0X00 == length))
	{
		return ERROR;
	}

	for(u32_i = 0; u32_i < length; u32_i++)
	{
		//串口发送
		USART_SendData(USARTx,Databuf[u32_i]);
		//发送完成标志
		while(!USART_Get_Transmitter_Empty_Flag(USARTx));
	}

	return OK;
	
}
/****************************************************************************/
/**
 * Function Name: Ecu_Usart_GetRxDataAddr
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
static UINT32 Ecu_Usart_GetRxDataAddr(USART_SFRmap* USARTx)
{
	return (UINT32)&USARTx->RBUFR;
}
/****************************************************************************/
/**
 * Function Name: ApiUsartDebugInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiUsartDebugInit(void)
{
	Ecu_Usart_Configure(&s_c_debug_cfg);
}
/****************************************************************************/
/**
 * Function Name: ApiUsartBleInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiUsartBleInit(void)
{
	Ecu_Usart_Configure(&s_c_ble_cfg);
}
/****************************************************************************/
/**
 * Function Name: ApiUsartMpuInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiUsartMpuInit(void)
{
	Ecu_Usart_Configure(&s_c_mpu_cfg);
}
/****************************************************************************/
/**
 * Function Name: ApiUsartDebugIntDiable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiUsartDebugIntDiable(void)
{
	INT_Interrupt_Enable(INT_USART0,FALSE);
}
/****************************************************************************/
/**
 * Function Name: ApiUsartBleIntDiable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiUsartBleIntDiable(void)
{
	INT_Interrupt_Enable(INT_USART3,FALSE);
}
/****************************************************************************/
/**
 * Function Name: ApiUsartMpuIntDiable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiUsartMpuIntDiable(void)
{
	INT_Interrupt_Enable(INT_USART1,FALSE);
}
/****************************************************************************/
/**
 * Function Name: ApiUsartHandler10ms
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/30, feifei.xu create this function
 ****************************************************************************/
void ApiUsartHandler10ms(void)
{
	#define	DELAY_CNT	(0xFFF)

	UINT16 u16_i = 0;
	UINT16 u16_count = 0;
	UINT16 u16_delay_cnt = 0;

	u16_count = s_debug_var.u16_tx_len;
	for(u16_i = 0; u16_i < u16_count; u16_i++)
	{
		USART_SendData(s_c_debug_cfg.USARTx,s_debug_var.u8_tx_buff[u16_i]);
		u16_delay_cnt = (UINT16)DELAY_CNT;
		while((u16_delay_cnt--) && (!USART_Get_Transmitter_Empty_Flag(s_c_debug_cfg.USARTx)));
	}
	memset(s_debug_var.u8_tx_buff,0x00,u16_count);
	s_debug_var.u16_tx_len = s_debug_var.u16_tx_len - u16_count;

	u16_count = s_ble_var.u16_tx_len;
	for(u16_i = 0; u16_i <u16_count; u16_i++)
	{
		USART_SendData(s_c_ble_cfg.USARTx,s_ble_var.u8_tx_buff[u16_i]);
		u16_delay_cnt = (UINT16)DELAY_CNT;
		while((u16_delay_cnt--) && (!USART_Get_Transmitter_Empty_Flag(s_c_ble_cfg.USARTx)));
	}
	memset(s_ble_var.u8_tx_buff,0x00,u16_count);
	s_ble_var.u16_tx_len = s_ble_var.u16_tx_len - u16_count;

	u16_count = s_mpu_var.u16_tx_len;
	for(u16_i = 0; u16_i <u16_count; u16_i++)
	{
		USART_SendData(s_c_mpu_cfg.USARTx,s_mpu_var.u8_tx_buff[u16_i]);
		u16_delay_cnt = (UINT16)DELAY_CNT;
		while((u16_delay_cnt--) && (!USART_Get_Transmitter_Empty_Flag(s_c_mpu_cfg.USARTx)));
	}
	memset(s_mpu_var.u8_tx_buff,0x00,u16_count);
	s_mpu_var.u16_tx_len = s_mpu_var.u16_tx_len - u16_count;


	

}
/****************************************************************************/
/**
 * Function Name: ApiUsartDebugRxAddr
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
UINT32 ApiGetRegAddrFormDebugRx(void)
{
	return Ecu_Usart_GetRxDataAddr(s_c_debug_cfg.USARTx);
}
/****************************************************************************/
/**
 * Function Name: ApiGetRegAddrFormMpuRx
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/30, feifei.xu create this function
 ****************************************************************************/
UINT32 ApiGetRegAddrFormMpuRx(void)
{
	return Ecu_Usart_GetRxDataAddr(s_c_mpu_cfg.USARTx);
}
/****************************************************************************/
/**
 * Function Name: ApiUartDebugSend
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/15, feifei.xu create this function
 ****************************************************************************/
INT32 ApiUartDebugSend(const UINT8* u8_buff,const UINT32 u32_len)
{
	memcpy(&s_debug_var.u8_tx_buff[s_debug_var.u16_tx_len], u8_buff, u32_len);
	s_debug_var.u16_tx_len = s_debug_var.u16_tx_len + u32_len;
	return OK;
	// return Ecu_Usart_SendData(s_c_debug_cfg.USARTx,u8_buff,u32_len);
}
/****************************************************************************/
/**
 * Function Name: ApiUartBleSend
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
INT32 ApiUartBleSend(const UINT8* u8_buff,const UINT32 u32_len)
{
	memcpy(&s_ble_var.u8_tx_buff[s_ble_var.u16_tx_len], u8_buff, u32_len);
	s_ble_var.u16_tx_len = s_ble_var.u16_tx_len + u32_len;
	return OK;
	// return Ecu_Usart_SendData(s_c_ble_cfg.USARTx,u8_buff,u32_len);
}
/****************************************************************************/
/**
 * Function Name: ApiUartMpuSend
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
INT32 ApiUartMpuSend(const UINT8* u8_buff,const UINT32 u32_len)
{
	memcpy(&s_mpu_var.u8_tx_buff[s_mpu_var.u16_tx_len], u8_buff, u32_len);
	s_mpu_var.u16_tx_len = s_mpu_var.u16_tx_len + u32_len;
	return OK;
	// return Ecu_Usart_SendData(s_c_mpu_cfg.USARTx,u8_buff,u32_len);

}
/****************************************************************************/
/**
 * Function Name: ApiUsartDebugRead
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/16, feifei.xu create this function
 ****************************************************************************/
void ApiUsartDebugRead(UINT8* u8_data,UINT16* p_u16_len)
{
	if(0x00 == s_debug_var.u16_rx_len)
	{
		return;
	}
	memcpy(u8_data,s_debug_var.u8_rx_buff,s_debug_var.u16_rx_len);
	*p_u16_len = s_debug_var.u16_rx_len;
	memset(s_debug_var.u8_rx_buff,0x00,FIFO_BUFF_SIZE);
	s_debug_var.u16_rx_len = 0;
}
/****************************************************************************/
/**
 * Function Name: ApiUsartBleRead
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/16, feifei.xu create this function
 ****************************************************************************/
void ApiUsartBleRead(UINT8* u8_data,UINT16* p_u16_len)
{
	if(0x00 == s_ble_var.u16_rx_len)
	{
		return;
	}
	memcpy(u8_data,s_ble_var.u8_rx_buff,s_ble_var.u16_rx_len);
	*p_u16_len = s_ble_var.u16_rx_len;
	memset(s_ble_var.u8_rx_buff,0x00,FIFO_BUFF_SIZE);
	s_ble_var.u16_rx_len = 0;
}
/****************************************************************************/
/**
 * Function Name: ApiUsartMpuRead
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/16, feifei.xu create this function
 ****************************************************************************/
void ApiUsartMpuRead(UINT8* u8_data,UINT16* p_u16_len)
{
	if(0x00 == s_mpu_var.u16_rx_len)
	{
		return;
	}
	memcpy(u8_data,s_mpu_var.u8_rx_buff,s_mpu_var.u16_rx_len);
	*p_u16_len = s_mpu_var.u16_rx_len;
	memset(s_mpu_var.u8_rx_buff,0x00,FIFO_BUFF_SIZE);
	s_mpu_var.u16_rx_len = 0;
}
/****************************************************************************/
/**
 * Function Name: ApiGetUartMpuStatus
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  create this function
 ****************************************************************************/

/****************************************************************************/
/**
 * Function Name: _USART0_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/27, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_USART0_exception(void)
{

}
/****************************************************************************/
/**
 * Function Name: _USART2_exception
 * Description: 中断函数
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/27, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_USART3_exception(void)
{

}
/****************************************************************************/
/**
 * Function Name: _USART1_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_USART1_exception(void)
{

}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/

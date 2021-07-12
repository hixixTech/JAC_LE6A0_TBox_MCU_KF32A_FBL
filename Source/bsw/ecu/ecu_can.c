/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    canhl.c
 * DESCRIPTION: 该文件提供了CAN模块相关的配置与发送功能，包括
 *          + CAN引脚重映射
 *          + CAN中断配置
 *          + CAN发送
 *          + CAN初始化
 * 
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
#include "ecu_can.h"
#include "ecu_gpio.h"
#include "ecu_misc.h"
#include <string.h>
#include "os_log.h"
/*****************************************************************************
** #define
*****************************************************************************/
#define 	xCAN_ACR    (uint32_t)0x00000000  //验收代码
#define 	xCAN_MSK 	(uint32_t)0xFFFFFFFF  //验收屏蔽

#define		CAN_RX_MAX_SIZE	(32)

/*****************************************************************************
** typedef
*****************************************************************************/
typedef struct
{
	CAN_SFRmap* CANx;
	UINT32 u32_Mode;
	UINT32 u32_Clk;
	UINT32 u32_BaudRate;
	/******中断配置*********/
	UINT32 u32_Interrupt_Index;
	FunctionalState RX_INT_Enable;
	FunctionalState TX_INT_Enable;
	FunctionalState ERR_ALARM_INIT_Enable;
	FunctionalState DATA_OVERFLOW_INT_Enable;
	FunctionalState	WAKE_UP_INT_Enable;
	FunctionalState ERROR_NEGATIVE_INT_Enable;
	FunctionalState ARBITRATION_LOST_INT_Enable;
	FunctionalState BUS_ERR_INT_Enable;
}CAN_CFG_S;

typedef struct
{
	UINT32 u32_id;
	UINT8 u8_len;
	UINT8 u8_data[8];
	UINT32 u32_format;
}CAN_TX_MSG;

typedef enum
{
	CAN_ERR_OK = 0,
	CAM_ERR_BUFFFULL
}CAN_ERR_E;
typedef struct
{
	UINT16 can_addr;
	UINT8 u8_rx_count;
	CAN_MessageTypeDef rx_msg[CAN_RX_MAX_SIZE];
	UINT8 u8_tx_count;
	CAN_MessageTypeDef tx_msg[CAN_RX_MAX_SIZE];
}CAN_MSG_FIFO_S;

/*****************************************************************************
** global variable
******************************************************	***********************/
/***TSEG1设置与TSEG2设置比值75%采样率，采样点8个，采样率=（Seg1+Sync）/(Seg1+Seg2+Sync)*100%***/
/****************CAN1配置参数******************************/
const CAN_CFG_S c_st_can0_cfg = 
{
	CAN4_SFR,
	CAN_MODE_NORMAL,				//CAN工作模式：
	CAN_SOURCE_HFCLK_DIV_2,			//外设高速时钟：24M
	CAN_BAUDRATE_500K,				//波特率系数：500K
	/**************中断配置***********************/
	INT_CAN4,						//中断向量
	TRUE,							//接收中断使能
	FALSE,							//发送中断使能
	FALSE,							//错误报警中断使能
	FALSE,							//数据溢出中断使能
	TRUE,							//唤醒中断使能
	TRUE,							//错误消极中断使能
	TRUE,							//仲裁丢失中断使能
	FALSE,							//总线错误中断使能
};

const CAN_CFG_S c_st_can1_cfg = 
{
	CAN1_SFR,
	CAN_MODE_NORMAL,				//CAN工作模式：
	CAN_SOURCE_HFCLK_DIV_2,			//外设高速时钟：24M
	CAN_BAUDRATE_500K,				//波特率系数：500K
	/**************中断配置***********************/
	INT_CAN1,						//中断向量
	FALSE,							//接收中断使能
	FALSE,							//发送中断使能
	FALSE,							//错误报警中断使能
	FALSE,							//数据溢出中断使能
	FALSE,							//唤醒中断使能
	FALSE,							//错误消极中断使能
	FALSE,							//仲裁丢失中断使能
	FALSE,							//总线错误中断使能
};

const CAN_CFG_S c_st_can2_cfg = 
{
	CAN3_SFR,
	CAN_MODE_NORMAL,				//CAN工作模式：
	CAN_SOURCE_HFCLK_DIV_2,			//外设高速时钟：24M
	CAN_BAUDRATE_500K,				//波特率系数：500K
	/**************中断配置***********************/
	INT_CAN3,						//中断向量
	FALSE,							//接收中断使能
	FALSE,							//发送中断使能
	FALSE,							//错误报警中断使能
	FALSE,							//数据溢出中断使能
	FALSE,							//唤醒中断使能
	FALSE,							//错误消极中断使能
	FALSE,							//仲裁丢失中断使能
	FALSE,							//总线错误中断使能
};

/*****************************************************************************
** static variables
*****************************************************************************/
static CAN_MSG_FIFO_S s_st_can0_var;
static CAN_MSG_FIFO_S s_st_can1_var;
static CAN_MSG_FIFO_S s_st_can2_var;
/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/
static void Ecu_Can_Configure(const CAN_CFG_S* p_can_cfg);
static CAN_ERR_E Ecu_Can_TransmitData(	CAN_SFRmap* CANx, CAN_MessageTypeDef* CAN_MessageStructure);
/*****************************************************************************
** function prototypes
*****************************************************************************/
/****************************************************************************/
/**
 * Function Name: Ecu_Can_Init
 * Description: can配置
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/19, feifei.xu create this function
 ****************************************************************************/
static void Ecu_Can_Configure(const CAN_CFG_S* p_can_cfg)
{

	UINT32 u32_can_int_enable = 0x00;
	
	CAN_InitTypeDef CAN_InitStructure;

	CAN_InitStructure.m_Enable = TRUE;							//CAN是否使能：TRUE
	CAN_InitStructure.m_Mode = p_can_cfg->u32_Mode;				//CAN工作模式
	CAN_InitStructure.m_WorkSource = p_can_cfg->u32_Clk;		//外设高速时钟：24M
	CAN_InitStructure.m_BaudRate = p_can_cfg->u32_BaudRate;		//波特率系数：500K
	CAN_InitStructure.m_SyncJumpWidth = 0x01;					//同步跳转宽度：1
	CAN_InitStructure.m_TimeSeg1 = 0x04;						//TSEG1：4
	CAN_InitStructure.m_TimeSeg2 = 0x01;						//TSEG2：1
	CAN_InitStructure.m_BusSample = CAN_BUS_SAMPLE_3_TIMES;		//采样点次数：3
	CAN_InitStructure.m_Acceptance = xCAN_ACR;					//验收代码：0x00000000
	CAN_InitStructure.m_AcceptanceMask = xCAN_MSK;				//验收屏蔽：0xffffffff
	CAN_Reset(p_can_cfg->CANx);									//复位can
	CAN_Configuration(p_can_cfg->CANx,&CAN_InitStructure);		//初始化can
	CAN_Cmd(p_can_cfg->CANx,TRUE);
	/********************CAN中断配置****************************/
	
	if(p_can_cfg->RX_INT_Enable)
	{
		u32_can_int_enable |= CAN_INT_RECEIVE;
	}
	if(p_can_cfg->TX_INT_Enable)
	{
		u32_can_int_enable |= CAN_INT_TRANSMIT;
	}
	if(p_can_cfg->ERR_ALARM_INIT_Enable)
	{
		u32_can_int_enable |= CAN_INT_ERROR_ALARM;
	}
	if(p_can_cfg->DATA_OVERFLOW_INT_Enable)
	{
		u32_can_int_enable |= CAN_INT_DATA_OVERFLOW;
	}
	if(p_can_cfg->WAKE_UP_INT_Enable)
	{
		u32_can_int_enable |= CAN_INT_WAKE_UP;
	}
	if(p_can_cfg->ERROR_NEGATIVE_INT_Enable)
	{
		u32_can_int_enable |= CAN_INT_ERROR_NEGATIVE;
	}
	if(p_can_cfg->ARBITRATION_LOST_INT_Enable)
	{
		u32_can_int_enable |= CAN_INT_ARBITRATION_LOST;
	}
	if(p_can_cfg->BUS_ERR_INT_Enable)
	{
		u32_can_int_enable |= CAN_INT_BUS_ERROR;
	}
	
	CAN_Set_INT_Enable(p_can_cfg->CANx,u32_can_int_enable,TRUE);		//开启中断
	INT_Interrupt_Priority_Config(p_can_cfg->u32_Interrupt_Index,3,0);	//CAN抢占优先级4,子优先级0
	INT_Clear_Interrupt_Flag(p_can_cfg->u32_Interrupt_Index);			//CAN清中断标志
	INT_Interrupt_Enable(p_can_cfg->u32_Interrupt_Index,TRUE);			//CAN中断使能
	INT_All_Enable (TRUE);												//全局中断使能
}
/****************************************************************************/
/**
 * Function Name: Ecu_Can_Transmit_Data
 * Description: 数据发送加载
 *
 * Param: CANx：CAN通道
		  u32_id CAN ID
		  u8_data CAN 数据
		  u8_len 数据长度
		  u32_type 帧类型
		  u32_format 帧格式
 * Return:  none
 * Author:  2021/05/15, feifei.xu create this function
 ****************************************************************************/
static CAN_ERR_E Ecu_Can_TransmitData(	CAN_SFRmap* CANx, CAN_MessageTypeDef* CAN_MessageStructure)
{
	/* 发送缓冲器空 */
	while((!CAN_Get_Transmit_Status(CANx,CAN_TX_BUFFER_STATUS)))
	{
	}
	/* 转载数据到发送缓冲器 */
	CAN_Transmit_Message_Configuration(CANx,CAN_MessageStructure);
	// CAN_Transmit_Single(CANx);
	CAN_Transmit_Repeat(CANx);

	return CAN_ERR_OK;
}
/****************************************************************************/
/**
 * Function Name: ApiCanHandler1ms
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/27, feifei.xu create this function
 ****************************************************************************/
void ApiCanHandler1ms(void)
{
	UINT8 u8_i = 0;
	UINT8 u8_count = 0;

	u8_count = CAN_Get_Receive_Message_Counter(c_st_can0_cfg.CANx);
	for(u8_i = 0; u8_i < u8_count; u8_i++)
	{
		if(s_st_can0_var.u8_rx_count >= CAN_RX_MAX_SIZE)
		{
			s_st_can0_var.u8_rx_count = 0;
		}
		CAN_Receive_Message_Configuration(c_st_can0_cfg.CANx, s_st_can0_var.can_addr, &s_st_can0_var.rx_msg[s_st_can0_var.u8_rx_count++]);
		s_st_can0_var.can_addr = s_st_can0_var.can_addr + 0x10;
		CAN_Release_Receive_Buffer(c_st_can0_cfg.CANx,1);
		if(s_st_can0_var.can_addr > 0xF0)
		{
			s_st_can0_var.can_addr = 0x00;
		}
	}
	// CAN_Release_Receive_Buffer(c_st_can0_cfg.CANx,u8_count);

	u8_count = CAN_Get_Receive_Message_Counter(c_st_can1_cfg.CANx);
	for(u8_i = 0; u8_i < u8_count; u8_i++)
	{
		if(s_st_can1_var.u8_rx_count >= CAN_RX_MAX_SIZE)
		{
			s_st_can1_var.u8_rx_count = 0;
		}
		CAN_Receive_Message_Configuration(c_st_can1_cfg.CANx, s_st_can1_var.can_addr, &s_st_can1_var.rx_msg[s_st_can1_var.u8_rx_count++]);
		s_st_can1_var.can_addr = s_st_can1_var.can_addr + 0x10;
		CAN_Release_Receive_Buffer(c_st_can1_cfg.CANx,1);
		if(s_st_can1_var.can_addr > 0xF0)
		{
			s_st_can1_var.can_addr = 0x00;
		}
	}

	u8_count = CAN_Get_Receive_Message_Counter(c_st_can2_cfg.CANx);
	for(u8_i = 0; u8_i < u8_count; u8_i++)
	{
		if(s_st_can2_var.u8_rx_count >= CAN_RX_MAX_SIZE)
		{
			s_st_can2_var.u8_rx_count = 0;
		}
		CAN_Receive_Message_Configuration(c_st_can2_cfg.CANx, s_st_can2_var.can_addr, &s_st_can2_var.rx_msg[s_st_can2_var.u8_rx_count++]);
		s_st_can2_var.can_addr = s_st_can2_var.can_addr + 0x10;
		CAN_Release_Receive_Buffer(c_st_can2_cfg.CANx,1);
		if(s_st_can2_var.can_addr > 0xF0)
		{
			s_st_can2_var.can_addr = 0x00;
		}
	}
}
/****************************************************************************/
/**
 * Function Name: ApiCanHandler5ms
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/27, feifei.xu create this function
 ****************************************************************************/
void ApiCanHandler5ms(void)
{
	UINT8 u8_i = 0;
	UINT8 u8_count = 0;

	u8_count = s_st_can0_var.u8_tx_count;

	for(u8_i = 0; u8_i < u8_count; u8_i++)
	{
		if(CAN_ERR_OK == Ecu_Can_TransmitData(c_st_can0_cfg.CANx, &s_st_can0_var.tx_msg[u8_i]))
		{
			s_st_can0_var.tx_msg[u8_i].m_StandardID = 0;
			s_st_can0_var.tx_msg[u8_i].m_ExtendedID = 0;
			s_st_can0_var.tx_msg[u8_i].m_DataLength = 0;
			s_st_can0_var.tx_msg[u8_i].m_FrameFormat = 0;
			s_st_can0_var.tx_msg[u8_i].m_RemoteTransmit = 0;
			memset(s_st_can0_var.tx_msg[u8_i].m_Data,0x00,8);
		}
	}
	s_st_can0_var.u8_tx_count = s_st_can0_var.u8_tx_count - u8_count;

	u8_count = s_st_can1_var.u8_tx_count;
	for(u8_i = 0; u8_i < s_st_can1_var.u8_tx_count; u8_i++)
	{
		if(CAN_ERR_OK == Ecu_Can_TransmitData(c_st_can1_cfg.CANx, &s_st_can1_var.tx_msg[u8_i]))
		{
			s_st_can1_var.tx_msg[u8_i].m_StandardID = 0;
			s_st_can1_var.tx_msg[u8_i].m_ExtendedID = 0;
			s_st_can1_var.tx_msg[u8_i].m_DataLength = 0;
			s_st_can1_var.tx_msg[u8_i].m_FrameFormat = 0;
			s_st_can1_var.tx_msg[u8_i].m_RemoteTransmit = 0;
			memset(s_st_can1_var.tx_msg[u8_i].m_Data,0x00,8);
		}
	}
	s_st_can1_var.u8_tx_count = s_st_can1_var.u8_tx_count - u8_count;

	u8_count = s_st_can2_var.u8_tx_count;
	for(u8_i = 0; u8_i < s_st_can2_var.u8_tx_count; u8_i++)
	{
		if(CAN_ERR_OK == Ecu_Can_TransmitData(c_st_can2_cfg.CANx, &s_st_can2_var.tx_msg[u8_i]))
		{
			s_st_can2_var.tx_msg[u8_i].m_StandardID = 0;
			s_st_can2_var.tx_msg[u8_i].m_ExtendedID = 0;
			s_st_can2_var.tx_msg[u8_i].m_DataLength = 0;
			s_st_can2_var.tx_msg[u8_i].m_FrameFormat = 0;
			s_st_can2_var.tx_msg[u8_i].m_RemoteTransmit = 0;
			memset(s_st_can2_var.tx_msg[u8_i].m_Data,0x00,8);
		}
	}
	s_st_can2_var.u8_tx_count = s_st_can2_var.u8_tx_count - u8_count;
}
/****************************************************************************/
/**
 * Function Name: ApiCan0Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan0Init(void)
{
	Ecu_Can_Configure(&c_st_can0_cfg);
}
/****************************************************************************/
/**
 * Function Name: ApiCan1Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan1Init(void)
{
	Ecu_Can_Configure(&c_st_can1_cfg);
}
/****************************************************************************/
/**
 * Function Name: ApiCan2Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan2Init(void)
{
	Ecu_Can_Configure(&c_st_can2_cfg);
}
/****************************************************************************/
/**
 * Function Name: ApiCan0IntDisable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiCan0IntDisable(void)
{
	INT_Interrupt_Enable(c_st_can0_cfg.u32_Interrupt_Index,FALSE);
}
/****************************************************************************/
/**
 * Function Name: ApiCan1IntDisable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiCan1IntDisable(void)
{
	INT_Interrupt_Enable(c_st_can1_cfg.u32_Interrupt_Index,FALSE);
}
/****************************************************************************/
/**
 * Function Name: ApiCan2IntDisable
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/07, feifei.xu create this function
 ****************************************************************************/
void ApiCan2IntDisable(void)
{
	INT_Interrupt_Enable(c_st_can2_cfg.u32_Interrupt_Index,FALSE);
}
/****************************************************************************/
/**
 * Function Name: Ecu_Can0_CheckBusOff
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan0CheckBusOff(void)
{
	if(c_st_can0_cfg.CANx->CTLR &(1<<23))	//Boff置位
	{
		c_st_can0_cfg.CANx->CTLR &= (~0x01);//清除复位模式后，
	}
}
/****************************************************************************/
/**
 * Function Name: ApiCan1CheckBusOff
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan1CheckBusOff(void)
{
	if(c_st_can1_cfg.CANx->CTLR &(1<<23))	//Boff置位
	{
		c_st_can1_cfg.CANx->CTLR &= (~0x01);//清除复位模式后，
	}
}
/****************************************************************************/
/**
 * Function Name: ApiCan2CheckBusOff
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan2CheckBusOff(void)
{
	if(c_st_can2_cfg.CANx->CTLR &(1<<23))	//Boff置位
	{
		c_st_can2_cfg.CANx->CTLR &= (~0x01);//清除复位模式后，
	}
}
/****************************************************************************/
/**
 * Function Name: ApiCan0Send
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan0Send(UINT32 u32_id, UINT8 u8_len, UINT8* u8_data)
{
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_StandardID = u32_id;
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_ExtendedID = 0;
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_DataLength = u8_len;
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_FrameFormat = CAN_FRAME_FORMAT_SFF;
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_RemoteTransmit = CAN_DATA_FRAME;
	memcpy(s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_Data, u8_data, u8_len);
	s_st_can0_var.u8_tx_count++;
}
/****************************************************************************/
/**
 * Function Name: ApiCan1Send
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan1Send(UINT32 u32_id, UINT8 u8_len, UINT8* u8_data)
{
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_StandardID = u32_id;
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_ExtendedID = 0;
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_DataLength = u8_len;
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_FrameFormat = CAN_FRAME_FORMAT_SFF;
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_RemoteTransmit = CAN_DATA_FRAME;
	memcpy(s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_Data, u8_data, u8_len);
	s_st_can1_var.u8_tx_count++;
}
/****************************************************************************/
/**
 * Function Name: ApiCan2Send
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan2Send(UINT32 u32_id, UINT8 u8_len, UINT8* u8_data)
{
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_StandardID = u32_id;
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_ExtendedID = 0;
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_DataLength = u8_len;
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_FrameFormat = CAN_FRAME_FORMAT_SFF;
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_RemoteTransmit = CAN_DATA_FRAME;
	memcpy(s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_Data, u8_data, u8_len);
	s_st_can2_var.u8_tx_count++;
}
/****************************************************************************/
/**
 * Function Name: ApiCan0Send
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan0ExtSend(UINT32 u32_id, UINT8 u8_len, UINT8* u8_data)
{
	
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_StandardID = 0;
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_ExtendedID = u32_id;
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_DataLength = u8_len;
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_FrameFormat = CAN_FRAME_FORMAT_EFF;
	s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_RemoteTransmit = CAN_DATA_FRAME;
	memcpy(s_st_can0_var.tx_msg[s_st_can0_var.u8_tx_count].m_Data, u8_data, u8_len);
	s_st_can0_var.u8_tx_count++;
}
/****************************************************************************/
/**
 * Function Name: ApiCan1Send
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan1ExtSend(UINT32 u32_id, UINT8 u8_len, UINT8* u8_data)
{
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_StandardID = 0;
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_ExtendedID = u32_id;
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_DataLength = u8_len;
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_FrameFormat = CAN_FRAME_FORMAT_EFF;
	s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_RemoteTransmit = CAN_DATA_FRAME;
	memcpy(s_st_can1_var.tx_msg[s_st_can1_var.u8_tx_count].m_Data, u8_data, u8_len);
	s_st_can1_var.u8_tx_count++;
}
/****************************************************************************/
/**
 * Function Name: ApiCan2Send
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void ApiCan2ExtSend(UINT32 u32_id, UINT8 u8_len, UINT8* u8_data)
{
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_StandardID = 0;
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_ExtendedID = u32_id;
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_DataLength = u8_len;
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_FrameFormat = CAN_FRAME_FORMAT_EFF;
	s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_RemoteTransmit = CAN_DATA_FRAME;
	memcpy(s_st_can2_var.tx_msg[s_st_can2_var.u8_tx_count].m_Data, u8_data, u8_len);
	s_st_can2_var.u8_tx_count++;
}

/****************************************************************************/
/**
 * Function Name: ApiCan0ReiciveMsg
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/17, feifei.xu create this function
 ****************************************************************************/
BOOL ApiCan0ReiciveMsg(CAN_MSG_S* p_can_msg,UINT8* u8_count)
{
	UINT8 u8_i = 0;
	UINT8 u8_temp = 0;

	u8_temp = s_st_can0_var.u8_rx_count;

	if(0x00 == u8_temp)
	{
		return FALSE;
	}

	for(u8_i = 0; u8_i < u8_temp; u8_i++)
	{
		if(CAN_FRAME_FORMAT_SFF == s_st_can0_var.rx_msg[u8_i].m_FrameFormat)
		{
			p_can_msg[u8_i].u32_id = s_st_can0_var.rx_msg[u8_i].m_StandardID;
		}
		else
		{
			p_can_msg[u8_i].u32_id = s_st_can0_var.rx_msg[u8_i].m_ExtendedID;
		}
		p_can_msg[u8_i].u8_len = s_st_can0_var.rx_msg[u8_i].m_DataLength;

		memcpy(p_can_msg[u8_i].u8_data,s_st_can0_var.rx_msg[u8_i].m_Data,8);
		s_st_can0_var.rx_msg[u8_i].m_StandardID = 0x00;
		s_st_can0_var.rx_msg[u8_i].m_ExtendedID = 0x00;
		s_st_can0_var.rx_msg[u8_i].m_DataLength = 0x00;
		s_st_can0_var.rx_msg[u8_i].m_FrameFormat = 0x00;
		s_st_can0_var.rx_msg[u8_i].m_RemoteTransmit = 0x00;
		memset(s_st_can0_var.rx_msg[u8_i].m_Data,0x00,8);
	}

	*u8_count = u8_temp;
	s_st_can0_var.u8_rx_count = s_st_can0_var.u8_rx_count - u8_temp;

	return TRUE;
}
/****************************************************************************/
/**
 * Function Name: ApiCan1ReiciveMsg
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/17, feifei.xu create this function
 ****************************************************************************/
BOOL ApiCan1ReiciveMsg(CAN_MSG_S* p_can_msg,UINT8* u8_count)
{
	UINT8 u8_i = 0;

	if(0x00 == s_st_can1_var.u8_rx_count)
	{
		return FALSE;
	}

	for(u8_i = 0; u8_i < s_st_can1_var.u8_rx_count; u8_i++)
	{
		if(CAN_FRAME_FORMAT_SFF == s_st_can1_var.rx_msg[u8_i].m_FrameFormat)
		{
			p_can_msg[u8_i].u32_id = s_st_can1_var.rx_msg[u8_i].m_StandardID;
		}
		else
		{
			p_can_msg[u8_i].u32_id = s_st_can1_var.rx_msg[u8_i].m_ExtendedID;
		}
		p_can_msg[u8_i].u8_len = s_st_can1_var.rx_msg[u8_i].m_DataLength;
		memcpy(p_can_msg[u8_i].u8_data,s_st_can1_var.rx_msg[u8_i].m_Data,8);
		s_st_can1_var.rx_msg[u8_i].m_StandardID = 0x00;
		s_st_can1_var.rx_msg[u8_i].m_ExtendedID = 0x00;
		s_st_can1_var.rx_msg[u8_i].m_DataLength = 0x00;
		s_st_can1_var.rx_msg[u8_i].m_FrameFormat = 0x00;
		s_st_can1_var.rx_msg[u8_i].m_RemoteTransmit = 0x00;
		memset(s_st_can1_var.rx_msg[u8_i].m_Data,0x00,8);
	}
	*u8_count = s_st_can1_var.u8_rx_count;
	s_st_can1_var.u8_rx_count = 0;

	return TRUE;
	
}
/****************************************************************************/
/**
 * Function Name: ApiCan2ReiciveMsg
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/17, feifei.xu create this function
 ****************************************************************************/
BOOL ApiCan2ReiciveMsg(CAN_MSG_S* p_can_msg,UINT8* u8_count)
{
	UINT8 u8_i = 0;

	if(0x00 == s_st_can2_var.u8_rx_count)
	{
		return FALSE;
	}

	for(u8_i = 0; u8_i < s_st_can2_var.u8_rx_count; u8_i++)
	{
		if(CAN_FRAME_FORMAT_SFF == s_st_can2_var.rx_msg[u8_i].m_FrameFormat)
		{
			p_can_msg[u8_i].u32_id = s_st_can2_var.rx_msg[u8_i].m_StandardID;
		}
		else
		{
			p_can_msg[u8_i].u32_id = s_st_can2_var.rx_msg[u8_i].m_ExtendedID;
		}
		p_can_msg[u8_i].u8_len = s_st_can2_var.rx_msg[u8_i].m_DataLength;
		memcpy(p_can_msg[u8_i].u8_data,s_st_can2_var.rx_msg[u8_i].m_Data,8);
		s_st_can2_var.rx_msg[u8_i].m_StandardID = 0x00;
		s_st_can2_var.rx_msg[u8_i].m_ExtendedID = 0x00;
		s_st_can2_var.rx_msg[u8_i].m_DataLength = 0x00;
		s_st_can2_var.rx_msg[u8_i].m_FrameFormat = 0x00;
		s_st_can2_var.rx_msg[u8_i].m_RemoteTransmit = 0x00;
		memset(s_st_can2_var.rx_msg[u8_i].m_Data,0x00,8);
	}
	*u8_count = s_st_can2_var.u8_rx_count;
	s_st_can2_var.u8_rx_count = 0;

	return TRUE;

}
/****************************************************************************/
/**
 * Function Name: _CAN1_exception
 * Description: 中断函数
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_CAN1_exception (void)
{

}
/****************************************************************************/
/**
 * Function Name: _CAN3_exception
 * Description: 中断函数
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_CAN3_exception (void)
{

}
/****************************************************************************/
/**
 * Function Name: _CAN4_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/15, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_CAN4_exception (void)
{
	UINT8 u8_i = 0;
	UINT8 u8_count = 0;

	INT_Clear_Interrupt_Flag(c_st_can0_cfg.u32_Interrupt_Index);

	if(CAN_Get_INT_Flag(c_st_can0_cfg.CANx,CAN_INT_ARBITRATION_LOST) != RESET)
	{
		CAN_Clear_INT_Flag(c_st_can0_cfg.CANx,CAN_INT_ARBITRATION_LOST);
	}

	if(CAN_Get_INT_Flag(c_st_can0_cfg.CANx,CAN_INT_ERROR_NEGATIVE) != RESET)
	{
		CAN_Clear_INT_Flag(c_st_can0_cfg.CANx,CAN_INT_ERROR_NEGATIVE);
	}

	if(CAN_Get_INT_Flag(c_st_can0_cfg.CANx,CAN_INT_RECEIVE) != RESET)
	{
		if(s_st_can0_var.u8_rx_count >= CAN_RX_MAX_SIZE)
		{
			s_st_can0_var.u8_rx_count = 0;
		}
		CAN_Receive_Message_Configuration(c_st_can0_cfg.CANx, s_st_can0_var.can_addr, &s_st_can0_var.rx_msg[s_st_can0_var.u8_rx_count++]);
		s_st_can0_var.can_addr = s_st_can0_var.can_addr + 0x10;
		CAN_Release_Receive_Buffer(c_st_can0_cfg.CANx,1);

		if(s_st_can0_var.can_addr > 0xF0)
		{
			s_st_can0_var.can_addr = 0x00;
		}
	}
}
/*****************************************************************************
** End File
*****************************************************************************/

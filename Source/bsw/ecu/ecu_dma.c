/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_dma.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/05/27
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
#include "ecu_dma.h"
/*****************************************************************************
** #define
*****************************************************************************/


/*****************************************************************************
** typedef
*****************************************************************************/
typedef struct
{
	DMA_SFRmap* DMAx;
	UINT8 u8_Channal;
	UINT8 u8_Direction;
	UINT8 u8_DataSize;
	UINT16 u16_Priority;
	UINT16 u16_number; 
	FunctionalState PeripheralInc;
	FunctionalState MemoryInc;
	UINT32 u32_BlockMode;
}DMA_CFG_S;

typedef struct
{
	UINT8 u8_buff[128];
	UINT16 u16_len;
}DEBUG_FIFO_S;
/*****************************************************************************
** global variable
*****************************************************************************/
DMA_CFG_S g_dma_debug_rx_cfg = 
{
	DMA0_SFR,
	DMA_CHANNEL_2,					//DMA通道选择:通道2
	DMA_PERIPHERAL_TO_MEMORY,		//DMA传输方向：外设到内存
	DMA_DATA_WIDTH_8_BITS,			//数据位宽：8位宽
	DMA_CHANNEL_LOWER,				//DMA通道优先级：低优先级
	1,								//传输数据个数: 5
	FALSE,							//外设地址增量模式使能: FALSE
	TRUE,							//储器地址增量模式使能: TRUE
	DMA_TRANSFER_BYTE,				//数据字节输模式
};

DMA_CFG_S g_dma_mpu_rx_cfg = 
{
	DMA0_SFR,
	DMA_CHANNEL_4,					//DMA通道选择:通道2
	DMA_PERIPHERAL_TO_MEMORY,		//DMA传输方向：外设到内存
	DMA_DATA_WIDTH_8_BITS,			//数据位宽：8位宽
	DMA_CHANNEL_LOWER,				//DMA通道优先级：低优先级
	2048,								//传输数据个数: 5
	FALSE,							//外设地址增量模式使能: FALSE
	TRUE,							//储器地址增量模式使能: TRUE
	DMA_TRANSFER_BYTE,				//数据字节输模式
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
static void Ecu_Dma_Comfigure(DMA_CFG_S* p_dma_cfg,UINT32 u32_per_addr,UINT32 u32_mem_addr);

/*****************************************************************************
** function prototypes
*****************************************************************************/
static void Ecu_Dma_Comfigure(DMA_CFG_S* p_dma_cfg,UINT32 u32_per_addr,UINT32 u32_mem_addr)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA复位 */
	DMA_Reset (p_dma_cfg->DMAx);
	DMA_InitStructure.m_Channel = p_dma_cfg->u8_Channal;
	DMA_InitStructure.m_Direction = p_dma_cfg->u8_Direction;
	DMA_InitStructure.m_PeripheralDataSize = p_dma_cfg->u8_DataSize;
	DMA_InitStructure.m_MemoryDataSize = p_dma_cfg->u8_DataSize;
	DMA_InitStructure.m_Priority = p_dma_cfg->u16_Priority;
	DMA_InitStructure.m_Number = p_dma_cfg->u16_number;
	DMA_InitStructure.m_PeripheralInc = p_dma_cfg->PeripheralInc;
	DMA_InitStructure.m_MemoryInc = p_dma_cfg->MemoryInc;
	DMA_InitStructure.m_LoopMode = TRUE;
	DMA_InitStructure.m_BlockMode = p_dma_cfg->u32_BlockMode;
	DMA_InitStructure.m_PeriphAddr = u32_per_addr;
	DMA_InitStructure.m_MemoryAddr = u32_mem_addr;
	/* 配置DMA功能函数 */
	DMA_Configuration (p_dma_cfg->DMAx, &DMA_InitStructure);
	/* 使能通道0DMA */
	DMA_Channel_Enable (p_dma_cfg->DMAx, p_dma_cfg->u8_Channal, TRUE);
	INT_Interrupt_Enable(INT_DMA0,TRUE);
	INT_All_Enable(TRUE);
}
/****************************************************************************/
/**
 * Function Name: ApiDmaDebugRxInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void ApiDmaMpuRxInit(UINT32 u32_per_addr,UINT32 u32_mem_addr)
{
	Ecu_Dma_Comfigure(&g_dma_mpu_rx_cfg,u32_per_addr,u32_mem_addr);
}

/****************************************************************************/
/**
 * Function Name: ApiDmaMpuTransferNum
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
UINT16 ApiDmaMpuTransferNum(void)
{
	UINT16 u16_temp;

	u16_temp = DMA_Get_Transfer_Number_Remain(g_dma_mpu_rx_cfg.DMAx,g_dma_mpu_rx_cfg.u8_Channal);

	if(0x00 != u16_temp)
	{
		u16_temp = g_dma_mpu_rx_cfg.u16_number - u16_temp;
	}

	return u16_temp;
}
/****************************************************************************/
/**
 * Function Name: _DMA0_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/25, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_DMA0_exception(void)
{
	if(TRUE == DMA_Get_Finish_Transfer_INT_Flag(DMA0_SFR,DMA_CHANNEL_2))
	{
		// DMA_Clear_INT_Flag(g_dma_debug_rx_cfg.DMAx,g_dma_debug_rx_cfg.u8_Channal,DMA_INT_FINISH_TRANSFER);
	}
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


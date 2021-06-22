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
#include "ecu_usart.h"
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
	32,							//传输数据个数: 5
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
static void Ecu_Dma_Comfigure(DMA_CFG_S* p_dma_cfg);

/*****************************************************************************
** function prototypes
*****************************************************************************/
void Ecu_Dma_Init(void)
{
	Ecu_Dma_Comfigure(&g_dma_debug_rx_cfg);
}

static void Ecu_Dma_Comfigure(DMA_CFG_S* p_dma_cfg)
{

	DMA_InitTypeDef DMA_InitStructure;

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
	// DMA_InitStructure.m_PeriphAddr = ApiUsartDebugRxAddr();
	//DMA_InitStructure.m_MemoryAddr = (UINT32)&g_u_debug_fifo.u8_data;
	/* DMA复位 */
	DMA_Reset (p_dma_cfg->DMAx);
	/* 配置DMA功能函数 */
	DMA_Configuration (p_dma_cfg->DMAx, &DMA_InitStructure);
	/* 使能通道1 DMA */
	DMA_Channel_Enable (p_dma_cfg->DMAx, p_dma_cfg->u8_Channal, TRUE);
	DMA_Set_INT_Enable(p_dma_cfg->DMAx,p_dma_cfg->u8_Channal,DMA_INT_HALF_TRANSFER,TRUE);
	INT_Interrupt_Enable(INT_DMA0,TRUE);
	INT_All_Enable(TRUE);
}

static UINT8 s_u8_i = 0;

void __attribute__((interrupt))_DMA0_exception(void)
{
	if(TRUE == DMA_Get_Half_Transfer_INT_Flag(DMA0_SFR,DMA_CHANNEL_2))
	{
		s_u8_i = DMA_Get_Transfer_Number_Remain(DMA0_SFR,DMA_CHANNEL_2);
	}
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    ll_can.c                                                      */
/* Description:  主函数入口                                                       */
/*                                                                           */
/* Others:       Implementation of main function of the Bootloader Layer     */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.01.00  陈惠            基线创建                          */
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/* 头文件                                                                       */
/*---------------------------------------------------------------------------*/
#include "system_init.h"
#include "ll_can.h"
#include "ll_target.h"
#include "ecu_can.h"
#include "dl_engine.h"
#include "spp_generated.h"
#include "spp_main.h"



/* NSJW limit values */
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/
#pragma pack(1)
typedef struct
{
    UINT32 u32CanId;
    UINT8  u8DataLen;
    UINT8  u8Data[8];
}DcpdFotaCanFram_t;
#pragma pack()

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define SPP_DCPD_MSG_TO_MCU_CAN_FRAM 0x10 
#define SPP_DCPD_MSG_TO_MPU_CAN_FRAM 0x20
#define REV_CANID_OFFSET 0x08

#define CAN_RTN_OK                            0U        /* 返回正确 */
#define CAN_RTN_ERR                            255U    /* 返回错误 */

/* ---- TX buffer, TX status flag ---- */
#define CAN_TX_IDEL_STATE                    0U        /* 空闲态，可以发送数据 */
#define CAN_TX_BUSY_STATE                    1U        /* 忙碌态，不可以发数据 */
#define CAN_TX_BUS_OFF_STATE                2U

#define SLOT_0      0   /* 0x711 */
#define SLOT_1      1   /* 0x7DF */
#define SLOT_2      2   /* 0x7E8 */

#define CAN_SEND_ID  0x77F   /* 0x77F */
/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/
DcpdPackageFBLCom_t tFotaData;
static UINT8 u8FotaRevFlag = 0;
static UINT8 u8FotaRevFlag1 = 0;
UINT8 gu8CanRevFlag = 0;
UINT8 gu8FotaFlag = 0;
static UINT8 u8OtaRevFlag = 0;
static UINT8 u8OtaRevCheckFlag = 0;
static UINT8 u8OtaRevCanIdSlot = 0;
static UINT32 u32OtaRevCanId = 0;

/* Static function declarations *********************************************/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/


/* ************************************************************************ */
/* AFCAN initialization, only compiled into the PBL                         */
/* Configures slots according to the protocol                               */
/******************************************************************************
*  function name | ApiCan1Rx
*  content       | 读取CAN1报文
*  parameter     | st_CanMsg[] : 读取CAN报文缓存区
*  return        | CAN_RTN_ERR / 读到的报文数量
*  notice        | st_CanMsg[]缓存空间大于MAX_CAN_RX_MSG_FIFO_LEN
******************************************************************************/
BOOL ApiCan1Rx(UINT8 u8Slot, UINT8* pu8Data,UINT32* pu32CanId)
{
    CAN_MSG_S MSGS[16];
	UINT8 u8CanCount,u8Target,i;
	ApiCan0ReiciveMsg(MSGS,u8CanCount);

	if(u8CanCount == 0)
	{
		return FALSE;
	}

	switch (u8Slot)
	{
	case 0:
		u8Target = 0x777;
		break;
	case 1:
		u8Target = 0x7DF;
		break;
	case 2:
		u8Target = 0x7E8;
		break;
	default:
		break;
	}
	
	for(i=0;i<=u8CanCount;i++)
	{
		if(MSGS->u32_id == u8Target)
		{
			pu8Data[7] = MSGS->u8_data[0];
            pu8Data[6] = MSGS->u8_data[1];
            pu8Data[5] = MSGS->u8_data[2];
            pu8Data[4] = MSGS->u8_data[3];
            pu8Data[3] = MSGS->u8_data[4];
            pu8Data[2] = MSGS->u8_data[5];
            pu8Data[1] = MSGS->u8_data[6];
            pu8Data[0] = MSGS->u8_data[7]; 
			return TRUE;
		}

	}

    return FALSE;
}
/******************************************************************************
*  function name | ApiLlRx
*  content       | 初始化BSP、OS，创建启动任务，开启操作系统
*  parameter     | 无
*  return        | 无
*  notice        | 无
******************************************************************************/
void ApiLlRx(RxBuff_t* ptRxBuffer)
{
	UINT8 i;
	DcpdFotaCanFram_t tDcpdFram;
	UINT32 u32CanId[1];
	if(1 == u8OtaRevFlag)
	{
		#if 0
		if(0 == u8OtaRevCheckFlag)
		{
			if(expired == ApiCheckTimer(ota_timer_handle))
			{
				u8OtaRevFlag = 0;
				ApiStopTimer(ota_timer_handle);
				return;
			}
		}
		else
		{
			u8OtaRevCheckFlag = 0;
		}
		#endif
		
		i = ApiCan1Rx(SLOT_2,ptRxBuffer->tFrame.u8Data,&u32CanId[0]);
		if((i != 0) && (u32OtaRevCanId == u32CanId[0]))
		{
			tDcpdFram.u32CanId = u32OtaRevCanId;
			tDcpdFram.u8DataLen = 8;

			memcpy(tDcpdFram.u8Data,&ptRxBuffer->tFrame.u8Data,8);
			SppSend(SPP_CONNECT_TYPE_MCU_4G,0x20,(UINT8*)(&tDcpdFram),sizeof(tDcpdFram));
			ApiInitTimer(S3_timer_handle,S3OTASERVER_TO);
			ApiInitTimer(ota_timer_handle,Ota_Timeout);
		}
		
		if(expired == ApiCheckTimer(ota_timer_handle))
		{
			u8OtaRevFlag = 0;
			ApiStopTimer(ota_timer_handle);
		}
		return;
	}

    /* CAN protocol is defined, either CAN11 or CAN29 */
	if(1 == u8FotaRevFlag)
	{
		if(0 == u8FotaRevFlag1)
		{
			if(expired == ApiCheckTimer(com_timer_handle))
			{
				u8FotaRevFlag = 0;
				gu8FotaFlag = 0;
				ApiStopTimer(com_timer_handle);
				ApiInitTimer(NCr_timer_handle,NCr_Timeout);
				ApiInitTimer(NBs_timer_handle,NBs_Timeout);
			}
			return;
		}
		else
		{
			u8FotaRevFlag1 = 0;
		}
		ptRxBuffer->tFrame.u8Data[0] = tFotaData.u8Data.buf[0];
		ptRxBuffer->tFrame.u8Data[1] = tFotaData.u8Data.buf[1];
		ptRxBuffer->tFrame.u8Data[2] = tFotaData.u8Data.buf[2];
		ptRxBuffer->tFrame.u8Data[3] = tFotaData.u8Data.buf[3];
		ptRxBuffer->tFrame.u8Data[4] = tFotaData.u8Data.buf[4];
		ptRxBuffer->tFrame.u8Data[5] = tFotaData.u8Data.buf[5];
		ptRxBuffer->tFrame.u8Data[6] = tFotaData.u8Data.buf[6];
		ptRxBuffer->tFrame.u8Data[7] = tFotaData.u8Data.buf[7];
		if(((UINT8)(tFotaData.u16Cmd >> 8) == 0x07) && ((UINT8)tFotaData.u16Cmd == 0x77))
		{
			ptRxBuffer->tFrame.eAtyp = Physical;
	    	ptRxBuffer->bReceived = true;
		}
		else if(((UINT8)(tFotaData.u16Cmd >> 8) == 0x07) && ((UINT8)tFotaData.u16Cmd == 0xDF))
		{
			ptRxBuffer->tFrame.eAtyp = Functional;
	    	ptRxBuffer->bReceived = true;
		}
		else
		{
			;
		}
		
		if(expired == ApiCheckTimer(com_timer_handle))
		{
			u8FotaRevFlag = 0;
			gu8FotaFlag = 0;
			ApiStopTimer(com_timer_handle);
			ApiInitTimer(NCr_timer_handle,NCr_Timeout);
			ApiInitTimer(NBs_timer_handle,NBs_Timeout);
		}
	}
	else
	{
		i = ApiCan1Rx(SLOT_1,ptRxBuffer->tFrame.u8Data,&u32CanId[0]);
		if(i != 0)
		{
			gu8CanRevFlag = 1;
			ApiInitTimer(com_timer_handle,Com_FotaTimeout);
			ptRxBuffer->tFrame.eAtyp = Functional;
	        ptRxBuffer->bReceived = true;
	        return;
		}
		i = ApiCan1Rx(SLOT_0,ptRxBuffer->tFrame.u8Data,&u32CanId[0]);
		if(i != 0)
		{
			gu8CanRevFlag = 1;
			ApiInitTimer(com_timer_handle,Com_FotaTimeout);
			ptRxBuffer->tFrame.eAtyp = Physical;
	        ptRxBuffer->bReceived = true;
	        return;
		}

		if((expired == ApiCheckTimer(com_timer_handle)) && (1 == gu8CanRevFlag))
		{
			gu8CanRevFlag = 0;
			ApiStopTimer(com_timer_handle);
		}
	}
}


/******************************************************************************
*  function name | ApilLlFblFotaComMsgCb
*  content       | 初始化BSP、OS，创建启动任务，开启操作系统
*  parameter     | 无
*  return        | 无
*  notice        | 无
******************************************************************************/
void ApilLlFblFotaComMsgCb(UINT8 u8EventId, DcpdPackageFBLCom_t* ptData)
{
	static UINT8 u8Flag = 0;
    if(ptData==NULL)
    {
        return;
    }
	u8EventId = u8EventId;

	if(0 == gu8CanRevFlag)
	{
		ApiLogPrint(_LOG_TRACE,"ApilLlFblFotaComMsgCb Enter\n");
		gu8FotaFlag = 1;
		u8FotaRevFlag = 1;
		u8FotaRevFlag1 = 1;
		ApiInitTimer(com_timer_handle,Com_FotaTimeout);
	}
	else
	{
		return;
	}

	tFotaData.u16Cmd = ptData->u16Cmd;
	tFotaData.u8Data.buf[0] = ptData->u8Data.buf[0];
	tFotaData.u8Data.buf[1] = ptData->u8Data.buf[1];
	tFotaData.u8Data.buf[2] = ptData->u8Data.buf[2];
	tFotaData.u8Data.buf[3] = ptData->u8Data.buf[3];
	tFotaData.u8Data.buf[4] = ptData->u8Data.buf[4];
	tFotaData.u8Data.buf[5] = ptData->u8Data.buf[5];
	tFotaData.u8Data.buf[6] = ptData->u8Data.buf[6];
	tFotaData.u8Data.buf[7] = ptData->u8Data.buf[7];
	ApiLogPrint(_LOG_TRACE,"%d,%d,%d,%d,%d,%d,%d,%d,%d\n",tFotaData.u16Cmd,tFotaData.u8Data.buf[0],tFotaData.u8Data.buf[1],tFotaData.u8Data.buf[2],tFotaData.u8Data.buf[3],tFotaData.u8Data.buf[4],tFotaData.u8Data.buf[5],tFotaData.u8Data.buf[6],tFotaData.u8Data.buf[7]);
	if(0 == u8Flag)
	{
		ApiInitTimer(NCr_timer_handle,NCr_FotaTimeout);
		ApiInitTimer(NBs_timer_handle,NBs_FotaTimeout);
		tSemaphores.bLocked = false;
		u8Flag = 1;
	}
}


/******************************************************************************
*  function name | ApilLlFblFotaOtherEcuCb
*  content       | 初始化BSP、OS，创建启动任务，开启操作系统
*  parameter     | 无
*  return        | 无
*  notice        | 无
******************************************************************************/
void ApilLlFblFotaOtherEcuCb(UINT8 u8EventId, UINT8* ptData, UINT32 u32DataLen)
{
	UINT8 i;
	DcpdFotaCanFram_t tDcpdFram;
	DcpdFotaCanFram_t * ptDcpdCAN = NULL;
    ptDcpdCAN = (DcpdFotaCanFram_t*)ptData;

	#if 1
	if(0 == gu8CanRevFlag)
	{
		u8OtaRevFlag = 1;
		u8OtaRevCheckFlag = 1;

		if(ptDcpdCAN->u32CanId != 0x7DF)
		{
		u32OtaRevCanId = ptDcpdCAN->u32CanId + REV_CANID_OFFSET;
		}

		ApiCan2Send(ptDcpdCAN->u32CanId,8,&ptDcpdCAN->u8Data[0]);
		ApiInitTimer(S3_timer_handle,S3OTASERVER_TO);
		ApiInitTimer(ota_timer_handle,Ota_Timeout);
	}
	else
	{
		return;
	}
	#endif
	
	#if 0
	if((ptDcpdCAN->u8Data[0] == 0x02) && (ptDcpdCAN->u8Data[1] == 0x10))
	{
		tDcpdFram.u32CanId = 0x7E8;
		tDcpdFram.u8DataLen = 8;
		tDcpdFram.u8Data[0] = 0x06;
		tDcpdFram.u8Data[1] = 0x50;
		tDcpdFram.u8Data[2] = 0x02;
		tDcpdFram.u8Data[3] = 0x00;
		tDcpdFram.u8Data[4] = 0x32;
		tDcpdFram.u8Data[5] = 0x01;
		tDcpdFram.u8Data[6] = 0xF4;
		tDcpdFram.u8Data[7] = 0x00;
		DiasSppSend(SPP_CONNECT_TYPE_MCU_4G,0x20,(UINT8*)(&tDcpdFram),sizeof(tDcpdFram));
	}

	if((ptDcpdCAN->u8Data[0] == 0x14) && (ptDcpdCAN->u8Data[1] == 0x00))
	{
		tDcpdFram.u32CanId = 0x7E8;
		tDcpdFram.u8DataLen = 8;
		tDcpdFram.u8Data[0] = 0x30;
		tDcpdFram.u8Data[1] = 0x00;
		tDcpdFram.u8Data[2] = 0x00;
		tDcpdFram.u8Data[3] = 0x00;
		tDcpdFram.u8Data[4] = 0x00;
		tDcpdFram.u8Data[5] = 0x00;
		tDcpdFram.u8Data[6] = 0x00;
		tDcpdFram.u8Data[7] = 0x00;
		DiasSppSend(SPP_CONNECT_TYPE_MCU_4G,0x20,(UINT8*)(&tDcpdFram),sizeof(tDcpdFram));
	}
	
	ApiCan2Send(ptDcpdCAN->u32CanId,8,&ptDcpdCAN->u8Data[0]);
	#endif

}


/******************************************************************************
*  function name | ApilLlTx
*  content       | 初始化BSP、OS，创建启动任务，开启操作系统
*  parameter     | 无
*  return        | 无
*  notice        | 无
******************************************************************************/
bool ApilLlTx(const UINT8* pu8TxBufferData)
{
	UINT8 i;
	if(1 == u8FotaRevFlag)
	{
		ApiLogPrint(_LOG_TRACE,"Response for FOTA REQ\n");
		tFotaData.u16Cmd = 0x077F;
		tFotaData.u8Data.buf[0] = pu8TxBufferData[0];
		tFotaData.u8Data.buf[1] = pu8TxBufferData[1];
		tFotaData.u8Data.buf[2] = pu8TxBufferData[2];
		tFotaData.u8Data.buf[3] = pu8TxBufferData[3];
		tFotaData.u8Data.buf[4] = pu8TxBufferData[4];
		tFotaData.u8Data.buf[5] = pu8TxBufferData[5];
		tFotaData.u8Data.buf[6] = pu8TxBufferData[6];
		tFotaData.u8Data.buf[7] = pu8TxBufferData[7];
		SPP_Send_DCPD_MSG_TO_MPU_FOTA_FBL_COM_Event(&tFotaData);
	}
	else
	{
		ApiCan2Send(CAN_SEND_ID,8,&pu8TxBufferData[0]);
	    // i = ApiCan2Send(CAN_SEND_ID,8,&pu8TxBufferData[0]);
		// if(i == CAN_RTN_OK)
		// {
		// 	return true;
		// }
		// else
		// {
		// 	return false;
		// }
		return true;
	}
}


/******************************************************************************
*  function name | ApilLlIsTxEmpty
*  content       | 初始化BSP、OS，创建启动任务，开启操作系统
*  parameter     | 无
*  return        | 无
*  notice        | 无
******************************************************************************/
bool ApilLlIsTxEmpty(void)
{
	UINT16 u16Timeout = 0xfff; 
	//TODO:
	while( (0 == CAN_Get_Transmit_Status(CAN1_SFR,CAN_TX_STATUS)) && (u16Timeout != 0) )
	{
		--u16Timeout;
	}
	
	return (true);
}


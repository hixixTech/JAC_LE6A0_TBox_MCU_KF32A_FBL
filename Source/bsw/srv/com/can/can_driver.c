/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     can_driver.c                                                */
/* Description:   DIAS CAN Stack的CAN驱动层                                  */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:     RH850F1L                                                   */
/* Compiler:      GHS_201517                                                 */
/********************************** 修改历史 *********************************/
/* Date             Version          Author            Description           */
/* 2019-08-01     V11.01.01.01.00   吴辰雨     新建基线                      */
/* 2019-11-06     V11.01.01.01.01   吴辰雨     修复立即帧发送bug，增加中断发送接口*/
/* 2019-12-17     V11.01.01.01.02   吴辰雨     修改CAN接收函数，修改CAN发送对于周期的判断 */
/* 2020-06-02     V11.01.01.01.03   吴辰雨     修复发送与接收接口bug             */
/* 2020-06-04     V11.01.01.01.04   吴辰雨     修复一个多次启动OSEK网络管理导致接收缓存Buffer异常的问题 */
/* 2020-06-15     V11.01.01.01.05   吴辰雨     增加MOT与INTEL两种DBC格式       */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
 #include "kf32a_basic_can.h"
#include "rtos_taskinit.h"
#include "string.h"
#include "stdint.h"

#include "can_config.h"
#include "can_driver.h"
#include "ecu_can.h"

// #include "osek_nm.h"
/*---------------------------------------------------------------------------*/
/* 全局变量                                                                   */
/*---------------------------------------------------------------------------*/
static BOOL DCSFrameMode[6];
static BOOL DCSFrameModeChanged = FALSE;

static UINT16 u16RcvRuleNum = 0;

static BOOL BReqClearRMNB = FALSE;
/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/
static void CanSetRxRule(const DCS_controllerConfig* dcs_ctlcfg);
static void CanSetTxBuffer(const DCS_controllerConfig* dcs_ctlcfg);
static uint8_t CanMsgTx( const DCS_controllerConfig* dcs_ctlcfg, uint8_t txframe,  DCSTxType_e type);
static BOOL CanMsgRx(const DCS_controllerConfig* dcs_ctlcfg, uint8_t dcs_rxmsg_idx, DCSRxType_e type);
static void SetRxFlag(const DCS_controllerConfig* dcs_ctlcfg, DCS_RxSigf rxframe);
static void CanSetNMRx(void);

static BOOL ApiDcsIsFifoBufferEmpty(CAN_SFRmap* CANx);
static void ApiDcsGetFifoMessage(uint32_t* id, uint8_t* dlc, uint8_t* data);

/*---------------------------------------------------------------------------*/
/* 内部函数定义                                                            */
/*---------------------------------------------------------------------------*/
static uint8_t CanMsgTx( const DCS_controllerConfig* dcs_ctlcfg, uint8_t txframe,  DCSTxType_e type)
{
    uint8_t u8TxBuf;
    uint32_t u32Id;
    uint8_t u8Dlc;
    uint8_t* pu8Data;
    uint8_t SendData[8]={0};
    /* 1. return if CAN controller is in stop mode or in bus-off mode */

    /* 2. return if there is a tranmission in progress, indicate overrrun */
    if ( (type == DCS_TXTYPE_PERIODIC) || (type == DCS_TXTYPE_SPORADIC))
    {
        // u8TxBuf = dcs_ctlcfg->transmitSigFrameConfig[txframe].TrmBuffer;
        pu8Data = dcs_ctlcfg->transmitSigFrameConfig[txframe].MessageAddr;
        u32Id = dcs_ctlcfg->transmitSigFrameConfig[txframe].ID;
        u8Dlc = dcs_ctlcfg->transmitSigFrameConfig[txframe].DLC;
    }
    else if ( type == DCS_TXTYPE_IMMEDIATE )
    {
        // u8TxBuf = dcs_ctlcfg->transmitImFrameConfig[txframe].TrmBuffer;
        pu8Data = dcs_ctlcfg->transmitImFrameConfig[txframe].MessageAddr;
        u32Id = dcs_ctlcfg->transmitImFrameConfig[txframe].ID;
        u8Dlc = dcs_ctlcfg->transmitImFrameConfig[txframe].DLC;
    }
    else
    {
        return DCS_ERROR;
    }
    /*读取发送状态*/
    /* 3. clear send request */

    /* 4. Copy ID, DLC and DataFrame */    

    if (dcs_ctlcfg->byte_order == BYTE_ORDER_INTEL )
    {
        switch (u8Dlc)
        {
            case 8: SendData[0] = pu8Data[7] ;
            case 7: SendData[1] = pu8Data[6] ;
            case 6: SendData[2] = pu8Data[5] ;
            case 5: SendData[3] = pu8Data[4] ;
            case 4: SendData[4] = pu8Data[3] ;
            case 3: SendData[5] = pu8Data[2] ;
            case 2: SendData[6] = pu8Data[1] ;
            case 1: SendData[7] = pu8Data[0] ;
        }
    }
    else
    {
        switch (u8Dlc)
        {
            case 8: SendData[0] = pu8Data[0] ;
            case 7: SendData[1] = pu8Data[1] ;
            case 6: SendData[2] = pu8Data[2] ;
            case 5: SendData[3] = pu8Data[3] ;
            case 4: SendData[4] = pu8Data[4] ;
            case 3: SendData[5] = pu8Data[5] ;
            case 2: SendData[6] = pu8Data[6] ;
            case 1: SendData[7] = pu8Data[7] ;    
        }        
    }
    ApiCan0Send(u32Id,u8Dlc,SendData);
    // Ecu_Can_Transmit_Data(CAN0_SFR,               //CAN通道
	// 					u32Id,                 //ID
	// 					SendData,               //数据指针
	// 					u8Dlc,   					  //长度
	// 					CAN_DATA_FRAME,         //帧类型
	// 					CAN_FRAME_FORMAT_SFF    //帧格式
	// 					);
    /* 5. send request */
    return DCS_OK;
}

static BOOL CanMsgRx(const DCS_controllerConfig* dcs_ctlcfg, uint8_t dcs_rxmsg_idx, DCSRxType_e type)
{
    uint8_t u8RxBuf;
    uint8_t* pu8Data;
    CAN_MessageTypeDef CAN_MessageRxStructrue;//接收报文结构体

    if ( type == DCS_RXTYPE_SIGNAL )
    {
        u8RxBuf = dcs_ctlcfg->receiveSigFrameConfig[dcs_rxmsg_idx].RecBuffer;
        pu8Data = dcs_ctlcfg->receiveSigFrameConfig[dcs_rxmsg_idx].MessageAddr;
    }
    else if ( type == DCS_RXTYPE_IMMEDIATE )
    {
        u8RxBuf = dcs_ctlcfg->receiveImFrameConfig[dcs_rxmsg_idx].RecBuffer;
        pu8Data = dcs_ctlcfg->receiveImFrameConfig[dcs_rxmsg_idx].MessageAddr;
    }
    /* Judge if new message received */
        if ( dcs_ctlcfg->byte_order == BYTE_ORDER_INTEL )
        {
            // pu8Data[0] = u8gCanDataRx[0];
            // pu8Data[1] = u8gCanDataRx[1];
            // pu8Data[2] = u8gCanDataRx[2];
            // pu8Data[3] = u8gCanDataRx[3];
            // pu8Data[4] = u8gCanDataRx[4];
            // pu8Data[5] = u8gCanDataRx[5];
            // pu8Data[6] = u8gCanDataRx[6];
            // pu8Data[7] = u8gCanDataRx[7];
        }
        else
        {
            // pu8Data[7] = u8gCanDataRx[0];
            // pu8Data[6] = u8gCanDataRx[1];
            // pu8Data[5] = u8gCanDataRx[2];
            // pu8Data[4] = u8gCanDataRx[3];
            // pu8Data[3] = u8gCanDataRx[4];
            // pu8Data[2] = u8gCanDataRx[5];
            // pu8Data[1] = u8gCanDataRx[6];
            // pu8Data[0] = u8gCanDataRx[7];          
        }
        // return TRUE;
    return FALSE;
}

static void SetRxFlag(const DCS_controllerConfig* dcs_ctlcfg, DCS_RxSigf rxframe)
{
    DCSReceiveSigFrameConfig_t* pDcsRxMsgConfig = dcs_ctlcfg->receiveSigFrameConfig;
    pDcsRxMsgConfig[rxframe].received = TRUE;
}

/*---------------------------------------------------------------------------*/
/* 全局函数定义                                                                    */
/*---------------------------------------------------------------------------*/
uint8_t ApiDcsSysInit(void)
{
    u16RcvRuleNum = 0;
    BReqClearRMNB = TRUE;
    return DCS_OK;
}


uint8_t ApiDcsCtlConn(const DCS_controllerConfig* dcs_ctlcfg)
{
    /*  transition to channel commucation mode */
    volatile DCSChannelRegAddr_t* pChannelReg;
    uint8_t u8Ch = dcs_ctlcfg->channel;

    /* channel mode */
    if (pChannelReg[u8Ch].STS & 0x03u)
    {
        pChannelReg[u8Ch].CTR &= 0xFFFFFFFCu; /* switch to communication mode */
        while ( ( pChannelReg[u8Ch].STS & 0x02u) == 0x02u)
        {
            /* while halt mode */
        }
        while ( ( pChannelReg[u8Ch].STS & 0x01u) == 0x01u)
        {
            /* while reset mode */            
        }
    }
    /* NM receive FIFO */
    return 0;
}

uint8_t ApiDcsCtlDisConn_Busoff(const DCS_controllerConfig* dcs_ctlcfg)
{
    /*  transition to channel stop mode */
    volatile DCSChannelRegAddr_t* pChannelReg;
    uint8_t u8Ch = dcs_ctlcfg->channel;

    pChannelReg[u8Ch].CTR = (pChannelReg[u8Ch].CTR&0xFFFFFFFCu)|0x02; /* switch to channel reset mode */
    pChannelReg[u8Ch].ERFL = 0;


    return TRUE;

}

uint8_t ApiDcsCtlDisConn(const DCS_controllerConfig* dcs_ctlcfg)
{
    /*  transition to channel stop mode */
    volatile DCSChannelRegAddr_t* pChannelReg;
    uint8_t u8Ch = dcs_ctlcfg->channel;

    pChannelReg[u8Ch].CTR = 0x00000001u; /* switch to channel reset mode */
//    while ( (pChannelReg[ch].STS & 0x01u) != 0x01u )
//    {
//    }
    /* switch to channel stop mode */
    pChannelReg[u8Ch].CTR = 0x00000004u;

    return TRUE;

}




uint8_t ApiDcsImfTx(const DCS_controllerConfig* dcs_ctlcfg, const DCS_TxImf txframe)
{
    uint8_t u8Ret;
    DCSFMDType_e fmd_type = dcs_ctlcfg->transmitImFrameConfig[txframe].FrameType;
    if ( ApiDcsFmdGet(fmd_type) == FALSE)
    {
        return DCS_OK;
    }
    u8Ret = CanMsgTx( dcs_ctlcfg, txframe, DCS_TXTYPE_IMMEDIATE);
    return u8Ret;
}




BOOL ApiDcsImfRx(const DCS_controllerConfig* dcs_ctlcfg, const DCS_RxImf rxframe)
{
    BOOL bRet;
    DCSFMDType_e fmd_type = dcs_ctlcfg->receiveImFrameConfig[rxframe].FrameType;
    if ( ApiDcsFmdGet(fmd_type) == FALSE)
    {
        return DCS_OK;
    }
    bRet = CanMsgRx( dcs_ctlcfg, rxframe, DCS_RXTYPE_IMMEDIATE );

    return bRet;
}
void ApiDcsWr8(DCS_Signal d, uint8_t v)
{    
    UINT64* u64p = (UINT64*)dcs_all_signals[d].frame_pointer;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    DCSTransmitSigFrameConfig_t *TxSigCfg = dcs_all_signals[d].txsig_config;
    UINT64 u64MaskBit;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }
    u64MaskBit = (UINT64)((1ULL << u8Length) - 1);
    *u64p &= ~ (u64MaskBit << u8StartBit);
    *u64p |= (UINT64)((v & u64MaskBit) << u8StartBit);
    if ( TxSigCfg != 0)
    {
        if ( TxSigCfg->TxType == DCS_TXTYPE_SPORADIC)
        {
            TxSigCfg->updated = 1;
        }
    }
}

void ApiDcsWr16(DCS_Signal d, UINT16 v)
{
    UINT64* u64p = (UINT64*)dcs_all_signals[d].frame_pointer;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    DCSTransmitSigFrameConfig_t *TxSigCfg = dcs_all_signals[d].txsig_config;
    UINT64 u64MaskBit;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }
    u64MaskBit = (UINT64)((1ULL << u8Length) - 1);
    *u64p &= ~ (u64MaskBit << u8StartBit);
    *u64p |= (UINT64)((v & u64MaskBit) << u8StartBit);
    if ( TxSigCfg != 0)
    {
        if ( TxSigCfg->TxType == DCS_TXTYPE_SPORADIC)
        {
            TxSigCfg->updated = 1;
        }
    }

}

void ApiDcsWr32(DCS_Signal d, uint32_t v)
{
    UINT64* u64p = (UINT64*)dcs_all_signals[d].frame_pointer;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    UINT64 u64MaskBit;
    DCSTransmitSigFrameConfig_t *TxSigCfg = dcs_all_signals[d].txsig_config;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }    
    u64MaskBit = (UINT64)((1ULL << u8Length) - 1);
    *u64p &= ~ (u64MaskBit << u8StartBit);
    *u64p |= (UINT64)((v & u64MaskBit) << u8StartBit);
    if ( TxSigCfg != 0)
    {
        if ( TxSigCfg->TxType == DCS_TXTYPE_SPORADIC)
        {
            TxSigCfg->updated = 1;
        }
    }

}


void ApiDcsWrbool(DCS_Signal d, BOOL v)
{
    UINT64* u64p = (UINT64*)dcs_all_signals[d].frame_pointer;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    UINT64 u64MaskBit;
    DCSTransmitSigFrameConfig_t *TxSigCfg = dcs_all_signals[d].txsig_config;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }    
    u64MaskBit = (UINT64)((1ULL << u8Length) - 1);
    *u64p &= ~ (u64MaskBit << u8StartBit);
    *u64p |= (UINT64)((v & u64MaskBit) << u8StartBit);
    if ( TxSigCfg != 0)
    {
        if ( TxSigCfg->TxType == DCS_TXTYPE_SPORADIC)
        {
            TxSigCfg->updated = 1;
        }
    }

}


/* TODO: need to modify to Intel */
void ApiDcsWrbytes(DCS_Signal d, uint8_t* src, uint8_t n, uint8_t o)
{
    uint8_t* pu8Dest;
    DCSTransmitSigFrameConfig_t *TxSigCfg = dcs_all_signals[d].txsig_config;
    pu8Dest = ( dcs_all_signals[d].frame_pointer ) + o;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        switch(n)
        {
            case 8: pu8Dest[7] = src[0];
            case 7: pu8Dest[6] = src[1]; 
            case 6: pu8Dest[5] = src[2]; 
            case 5: pu8Dest[4] = src[3];
            case 4: pu8Dest[3] = src[4];
            case 3: pu8Dest[2] = src[5]; 
            case 2: pu8Dest[1] = src[6];
            case 1: pu8Dest[0] = src[7];
                    break;
            default:
                    break;
        }  
    }
    else
    {
        switch(n)
        {
            case 8: pu8Dest[7] = src[0];
            case 7: pu8Dest[6] = src[1]; 
            case 6: pu8Dest[5] = src[2]; 
            case 5: pu8Dest[4] = src[3];
            case 4: pu8Dest[3] = src[4];
            case 3: pu8Dest[2] = src[5]; 
            case 2: pu8Dest[1] = src[6];
            case 1: pu8Dest[0] = src[7];
                    break;
            default:
                    break;
        }  
    }
    
  
    if ( TxSigCfg != 0)
    {
        if ( TxSigCfg->TxType == DCS_TXTYPE_SPORADIC)
        {
            TxSigCfg->updated = 1;
        }
    }

}


uint8_t ApiDcsRd8(DCS_Signal d)
{
    UINT64 u64Data;
    uint8_t u8Ret;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }        
    u64Data = *(UINT64*)dcs_all_signals[d].frame_pointer;
    u8Ret = ( u64Data >> u8StartBit ) &  ( (1ULL << u8Length) - 1 );
    return (uint8_t)u8Ret;
}

UINT16 ApiDcsRd16(DCS_Signal d)
{
    UINT64 u64Data;
    UINT16 u16Ret;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }        
    u64Data = *(UINT64*)dcs_all_signals[d].frame_pointer;
    u16Ret = ( u64Data >> u8StartBit ) &  ( (1ULL <<u8Length) - 1 );
    return (UINT16)u16Ret;
}

uint32_t ApiDcsRd32(DCS_Signal d)
{
    UINT64 u64Data;
    uint32_t u32Ret;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }        
    u64Data = *(UINT64*)dcs_all_signals[d].frame_pointer;
    u32Ret = ( u64Data >> u8StartBit ) & ( (1ULL << u8Length)  - 1 );
    return (uint32_t)u32Ret;
}

UINT64 ApiDcsRd64(DCS_Signal d)
{
    UINT64 u64Data;
    UINT64 u64Ret;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }        
    u64Data = *(UINT64*)dcs_all_signals[d].frame_pointer;
    u64Ret = ( u64Data >> u8StartBit ) & ( (1ULL << u8Length)  - 1 );
    return (UINT64)u64Ret;
}


BOOL ApiDcsRdbool(DCS_Signal d)
{
    UINT64 u64Data;
    BOOL bRet;
    uint8_t u8StartBit = dcs_all_signals[d].start_bit;
    uint8_t u8Length = dcs_all_signals[d].length;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
        u8StartBit = (7 - u8StartBit/8) * 8 + (u8StartBit % 8);
    }        
    u64Data = *(UINT64*)dcs_all_signals[d].frame_pointer;
    bRet = ( u64Data >> u8StartBit ) & ((1ULL <<u8Length) - 1 );
    return (BOOL)bRet;
}


/* TODO: need to modify to Intel */
void ApiDcsRdbytes(DCS_Signal d , uint8_t* dest,  uint8_t n, uint8_t o)
{
    uint8_t* pu8Src;
    pu8Src = (dcs_all_signals[d].frame_pointer) + o;
    if ( dcs_all_signals[d].sig_byte_order == BYTE_ORDER_MOT )
    {
    switch(n)
    {
        case 8: dest[7] = pu8Src[0];
        case 7: dest[6] = pu8Src[1];
        case 6: dest[5] = pu8Src[2];
        case 5: dest[4] = pu8Src[3]; 
        case 4: dest[3] = pu8Src[4]; 
        case 3: dest[2] = pu8Src[5]; 
        case 2: dest[1] = pu8Src[6]; 
        case 1: dest[0] = pu8Src[7]; 
                break;
        default:
                break;

    }
    }
    else
    {
        switch(n)
        {
            case 8: dest[7] = pu8Src[7];
            case 7: dest[6] = pu8Src[6];
            case 6: dest[5] = pu8Src[5];
            case 5: dest[4] = pu8Src[4]; 
            case 4: dest[3] = pu8Src[3]; 
            case 3: dest[2] = pu8Src[2]; 
            case 2: dest[1] = pu8Src[1]; 
            case 1: dest[0] = pu8Src[0]; 
                    break;
            default:
                    break;

        }        
    }

}


BOOL ApiDcsTestRxFlag(const DCS_controllerConfig* dcs_ctlcfg, DCS_RxSigf rxframe)
{
    BOOL bRet;
    DCSReceiveSigFrameConfig_t* DcsRxMsgConfig = dcs_ctlcfg->receiveSigFrameConfig;
    bRet = DcsRxMsgConfig[rxframe].received;
    return bRet;
}

void ApiDcsClearRxFlag(const DCS_controllerConfig* dcs_ctlcfg, DCS_RxSigf rxframe)
{
    DCSReceiveSigFrameConfig_t* DcsRxMsgConfig = dcs_ctlcfg->receiveSigFrameConfig;
    DcsRxMsgConfig[rxframe].received = FALSE;
}



void ApiDcsFmdSet(DCSFMDType_e dcs_fmd)
{
    DCSFrameMode[dcs_fmd] = TRUE;
    if ( dcs_fmd == DCS_TXFMD_APP )
    {
        DCSFrameModeChanged = TRUE;
    }
}

void ApiDcsFmdClr(DCSFMDType_e dcs_fmd)
{
    DCSFrameMode[dcs_fmd] = FALSE;    
}

BOOL ApiDcsFmdGet(DCSFMDType_e dcs_fmd)
{
    return DCSFrameMode[dcs_fmd];
}

void ApiDcsInput(const DCS_controllerConfig* dcs_ctlconfig)
{
    uint8_t u8i;
    BOOL bRxRet = 0;
    DCSReceiveSigFrameConfig_t* DcsRxMsgConfig = dcs_ctlconfig->receiveSigFrameConfig;
    /* Frame Mode judge */
    if ( ApiDcsFmdGet(DCS_RXFMD_APP) == FALSE )
    {
        return;
    }
    for ( u8i = 0; u8i < (dcs_ctlconfig->rx_sig_buffers);  u8i++ )
    {
        bRxRet = CanMsgRx(dcs_ctlconfig, u8i, DCS_RXTYPE_SIGNAL );
        if ( bRxRet == TRUE)
        {
            SetRxFlag(dcs_ctlconfig, u8i);
        }
    }
    return;
}



void ApiDcsOutput(const DCS_controllerConfig* dcs_ctlconfig)
{
    uint8_t u8i;
    DCSTransmitSigFrameConfig_t* DcsTxMsgConfig = dcs_ctlconfig->transmitSigFrameConfig;
    
    // if ( ApiDcsFmdGet(DCS_TXFMD_APP) == FALSE)
    // {
    //     return;
    // }
    
    for ( u8i = 0; u8i < (dcs_ctlconfig->tx_sig_buffers); u8i++ )
    {
        /* Cyclic Message */
        if (( DcsTxMsgConfig[u8i].TxType == DCS_TXTYPE_PERIODIC ) && (DcsTxMsgConfig[u8i].cycle != 0))
        {         
            if ( DcsTxMsgConfig[u8i].counter == 0  )
            {
                (void)CanMsgTx( dcs_ctlconfig, u8i , DCS_TXTYPE_PERIODIC);
                DcsTxMsgConfig[u8i].counter = DcsTxMsgConfig[u8i].cycle;
            }
            DcsTxMsgConfig[u8i].counter--;
        }
        if ( DcsTxMsgConfig[u8i].TxType == DCS_TXTYPE_SPORADIC )
        {
            if ( DcsTxMsgConfig[u8i].updated == 1)
            {
                DcsTxMsgConfig[u8i].updated = 0;
                (void)CanMsgTx( dcs_ctlconfig, u8i , DCS_TXTYPE_SPORADIC);
            }
        }
    }
}







static BOOL ApiDcsIsFifoBufferEmpty(CAN_SFRmap* CANx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*---------------- 读取CANx_CTLR寄存器DOSTA位 ----------------*/
    tmpreg = CANx->CTLR;
    tmpreg &= CAN_CTLR_DOSTA;
    tmpreg >>= CAN_CTLR_DOSTA_POS;

    if (((uint8_t)tmpreg) &0x01 == 0x01)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void ApiDcsGetFifoMessage(uint32_t* id, uint8_t* dlc, uint8_t* data)
{
    static uint8_t Receice_addr=0x00;//接收RAM偏移地址
    CAN_MessageTypeDef canInitStruct;
    CAN_Receive_Message_Configuration (CAN1_SFR,Receice_addr,&canInitStruct);
    Receice_addr+=0x10;
    *id = canInitStruct.m_StandardID;
    *dlc = canInitStruct.m_DataLength;
    data[0] = (uint8_t)canInitStruct.m_Data[0];
    data[1] = (uint8_t)canInitStruct.m_Data[1];
    data[2] = (uint8_t)canInitStruct.m_Data[2];
    data[3] = (uint8_t)canInitStruct.m_Data[3];
    data[4] = (uint8_t)canInitStruct.m_Data[4];
    data[5] = (uint8_t)canInitStruct.m_Data[5];
    data[6] = (uint8_t)canInitStruct.m_Data[6];
    data[7] = (uint8_t)canInitStruct.m_Data[7];
}

void ApiInputSigFrame(const DCS_controllerConfig* dcs_ctlconfig)
{
    uint32_t id;
    uint8_t dlc,data[8];
    uint8_t RxSigCnt;
    uint8_t RxSigNum;
    if(dcs_ctlconfig == NULL)
        return;

    RxSigNum = dcs_ctlconfig ->OSEKNMRxFrameConfig->FIFORxSigNum;
    // while( ApiDcsIsFifoBufferEmpty(CAN1_SFR) == FALSE )
    // {
        ApiDcsGetFifoMessage(&id,&dlc,data);
        for(RxSigCnt=0;RxSigCnt<RxSigNum;RxSigCnt++)
        {
            DCSReceiveSigFrameConfig_t DCSReceiveSigFrameConfig = dcs_ctlconfig ->receiveSigFrameConfig[RxSigCnt];
            uint32_t idMatch = DCSReceiveSigFrameConfig.ID;
            uint32_t idMask = DCSReceiveSigFrameConfig.ID_Mask;
            if((idMatch & idMask) == (id & idMask))
            {
                if ( dcs_ctlconfig->byte_order == BYTE_ORDER_INTEL )
                {
                    DCSReceiveSigFrameConfig.MessageAddr[0] = data[0];
                    DCSReceiveSigFrameConfig.MessageAddr[1] = data[1];
                    DCSReceiveSigFrameConfig.MessageAddr[2] = data[2];
                    DCSReceiveSigFrameConfig.MessageAddr[3] = data[3];
                    DCSReceiveSigFrameConfig.MessageAddr[4] = data[4];
                    DCSReceiveSigFrameConfig.MessageAddr[5] = data[5];
                    DCSReceiveSigFrameConfig.MessageAddr[6] = data[6];
                    DCSReceiveSigFrameConfig.MessageAddr[7] = data[7];
                }
                else
                {
                    DCSReceiveSigFrameConfig.MessageAddr[7] = data[0];
                    DCSReceiveSigFrameConfig.MessageAddr[6] = data[1];
                    DCSReceiveSigFrameConfig.MessageAddr[5] = data[2];
                    DCSReceiveSigFrameConfig.MessageAddr[4] = data[3];
                    DCSReceiveSigFrameConfig.MessageAddr[3] = data[4];
                    DCSReceiveSigFrameConfig.MessageAddr[2] = data[5];
                    DCSReceiveSigFrameConfig.MessageAddr[1] = data[6];
                    DCSReceiveSigFrameConfig.MessageAddr[0] = data[7];
                }
                if(dcs_ctlconfig ->channel == dcs_controllConfig_DIAG.channel && RxSigCnt == 0)
                {
                    // PullNmMsg(id, 8, data);
                }
                SetRxFlag(dcs_ctlconfig,RxSigCnt);
                break;
            }
        }
    // }
}


void ApiDiagTx(const DCS_controllerConfig* dcs_ctlconfig,uint32_t ID,uint8_t* data)
{
    dcs_ctlconfig->transmitImFrameConfig[DCS_TBOX_DIAG].ID = ID;
    ApiDcsWrbytes((UINT16)Diag_Response, data , 0x08, 0x00);
    ApiDcsImfTx(&dcs_controllConfig_PT, (uint8_t)DCS_TBOX_DIAG);
}
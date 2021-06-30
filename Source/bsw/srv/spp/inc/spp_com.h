#ifndef _DIAS_SPP_COM_H_
#define _DIAS_SPP_COM_H_

#include "spp_main.h"


DiasErrorCode_e DiasSppComOpen(DiasSppConnectType_e eConnectType);
DiasErrorCode_e DiasSppComSend(DiasSppConnectType_e eConnectType,UINT8 *pu8SendData, UINT32 u32len);
UINT32 DiasSppComRecv(DiasSppConnectType_e eConnectType,UINT8 *pu8RevDataBuf, UINT32 u32len);
DiasErrorCode_e DiasSppComTransfer(DiasSppConnectType_e eConnectType,UINT8 *pu8SendData,UINT8 *pu8RevDataBuf, UINT32 u32len);
DiasErrorCode_e DiasSppComClose(DiasSppConnectType_e eConnectType);

#endif


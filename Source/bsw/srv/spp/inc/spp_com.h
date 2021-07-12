#ifndef _SPP_COM_H_
#define _SPP_COM_H_

#include "spp_main.h"


SPPErrorCode_e SppComOpen(SppConnectType_e eConnectType);
SPPErrorCode_e SppComSend(SppConnectType_e eConnectType,UINT8 *pu8SendData, UINT32 u32len);
INT16 SppComRecv(SppConnectType_e eConnectType,UINT8 *pu8RevDataBuf, UINT32 u32len);
SPPErrorCode_e SppComTransfer(SppConnectType_e eConnectType,UINT8 *pu8SendData,UINT8 *pu8RevDataBuf, UINT32 u32len);
SPPErrorCode_e SppComClose(SppConnectType_e eConnectType);
UINT32 SppComRecvValidData(SppConnectType_e eConnectType,UINT8 *pu8RevDataBuf, UINT32 u32len);

#endif


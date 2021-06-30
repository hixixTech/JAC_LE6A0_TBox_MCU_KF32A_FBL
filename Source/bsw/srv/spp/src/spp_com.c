#include "spp_com.h"
#include "spp_type.h"
//#include "lib_dias_public.h"
#include "spi.h"


/******************************************************************************
*  function name | DiasSppComOpen
*  content       | 初始化 spi 端口
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppComOpen(DiasSppConnectType_e eConnectType)
{
	INT32 iRet=0;	
	
	iRet=SPI_Open(eConnectType);
	if(OK==iRet)
	{
		//DiasDcpdLog(LOG_INFO,"open spi dev success.");
		return DIAS_SUCCESS;
	}
	
	DiasDcpdLog(LOG_ERR,"can't open spi dev.");
	return DIAS_FAILURE;
	
}

/******************************************************************************
*  function name | DiasSppComSend
*  content       | spi发送字符串
*  parameter     | pu8SendData : 待发送的字符串 eConnectType 连接类型
                   u32len: 发送长度
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppComSend(DiasSppConnectType_e eConnectType,UINT8 *pu8SendData, UINT32 u32len)
{
	INT32 iRet=0;
	static CHAR acDataLog[DIAS_SPP_PACKET_LEN_MAX*3 + 1] = {0};
	UINT8 u8Idx = 0;

	if(NULL==pu8SendData)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppComSend wrong para.");
		return DIAS_FAILURE;
	}

	iRet=SPI_Data_Send(eConnectType,pu8SendData, u32len);
	if(DIAS_SUCCESS==iRet)
	{
	#if 0
		(void)memset(acDataLog, 0, sizeof(acDataLog));
		for(u8Idx=0; u8Idx<u32len; u8Idx++)
		{
			snprintf(acDataLog, sizeof(acDataLog), "%s %02x", acDataLog, pu8SendData[u8Idx]);
		}
		ApiLogPrintExt(1, _LOG_DEBUG, "eConnectType=%d, send: length=%d, data=%s.", eConnectType, u32len, acDataLog);
	#endif
		return DIAS_SUCCESS;
	}
	//ApiLogPrintExt(1, _LOG_DEBUG, "DiasSppComSend failure.");
	return DIAS_FAILURE;
}

/******************************************************************************
*  function name | DiasSppComRecv
*  content       | spi读取字符串
*  parameter     | pu8RevDataBuf : 读取字符串的缓存 eConnectType 连接类型
                   u32Buflen: 读取长度
*  return        | 0 / 读取的字节数
*  notice        | 
******************************************************************************/
UINT32 DiasSppComRecv(DiasSppConnectType_e eConnectType,UINT8 *pu8RevDataBuf, UINT32 u32len)
{

	INT32 iRet=0;

	if(NULL==pu8RevDataBuf)
	{
		DiasDcpdLog(LOG_ERR,"DiasSppComRecv wrong para.");
	}

	iRet=SPI_Data_Recv(eConnectType,pu8RevDataBuf, u32len);
	if(0 < iRet)
	{		
		DiasDcpdLog(LOG_DEBUG,"DiasSppComRecv success len=%d.",iRet);
		return iRet;
	}
	//DiasDcpdLog(LOG_ERR,"DiasSppComRecv fail.");
	return 0;
}

/******************************************************************************
*  function name | DiasSppComTransfer
*  content       | spi收发字符串
*  parameter     | pu8SendData 发字符串的缓存 eConnectType 连接类型
				   pu8RevDataBuf : 读取字符串的缓存
                   u32Buflen: 收发长度
*  return        | 0 / 读取的字节数
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppComTransfer(DiasSppConnectType_e eConnectType,UINT8 *pu8SendData,UINT8 *pu8RevDataBuf, UINT32 u32len)
{
	INT32 iRet=0;
	
	if((NULL==pu8RevDataBuf)||(NULL==pu8RevDataBuf))
	{
		DiasDcpdLog(LOG_ERR,"DiasSppComSend wrong para.");
	}
	
	iRet=SPI_Data_Transfer(eConnectType, pu8SendData, pu8RevDataBuf, u32len);
	
	if(DIAS_SUCCESS==iRet)
	{
		DiasDcpdLog(LOG_DEBUG,"DiasSppComTransfer success.");
		return DIAS_SUCCESS;
	}
	DiasDcpdLog(LOG_ERR,"DiasSppComTransfer fail.");
	return DIAS_FAILURE;
}

/******************************************************************************
*  function name | DiasSppComClose
*  content       | 初始化 spi 端口
*  parameter     | eConnectType 连接类型
*  return        | DiasErrorCode_e
*  notice        | 
******************************************************************************/
DiasErrorCode_e DiasSppComClose(DiasSppConnectType_e eConnectType)
{
	INT32 iRet=0;
	iRet=SPI_Close(eConnectType);
	if(DIAS_SUCCESS==iRet)
	{
		DiasDcpdLog(LOG_DEBUG,"DiasSppComClose success.");
		return DIAS_SUCCESS;
	}
	DiasDcpdLog(LOG_ERR,"DiasSppComClose fail.");
	return DIAS_FAILURE;

}

#include "spi.h"
#include "spp_callout.h" 

static int g_spiInitStatus4G= ERROR;
static int g_spiInitStatusMpu = ERROR;

/******************************************************************************
*  function name | SPI_Close
*  content       | 关闭 spi 端口
*  parameter     | eConnectType 连接类型
*  return        | int 0 success
*  notice        | 
******************************************************************************/
int SPI_Close(DiasSppConnectType_e eConnectType)
{
	int iRet = OK;

	return iRet;
}

/******************************************************************************
*  function name | SPI_Open
*  content       | 打开 spi 端口
*  parameter     | eConnectType 连接类型
*  return        | int 0 success
*  notice        | 
******************************************************************************/
int SPI_Open(DiasSppConnectType_e eConnectType)
{	
    int iRet=ERROR;
	if(SPP_CONNECT_TYPE_MCU_4G==eConnectType)
	{
		if(ERROR==g_spiInitStatus4G)
		{
            (VOID)DiasSppHalInit();
			iRet=OK;
	
			g_spiInitStatus4G=iRet;
		}
		else
		{
			iRet=OK;
		}
	}
	else if(SPP_CONNECT_TYPE_MCU_MPU==eConnectType)
	{
		if(ERROR==g_spiInitStatusMpu)
		{

			g_spiInitStatusMpu=iRet;
		}
		else
		{
			iRet=OK;
		}
	}
	return iRet;
}

/******************************************************************************
*  function name | SPI_data_transfer
*  content       | 收发数据
*  parameter     | eConnectType 连接类型 tx 发数据buf rx接收数据buf len buf长度
*  return        | int 0 success
*  notice        | 
******************************************************************************/
int SPI_Data_Transfer(DiasSppConnectType_e eConnectType, unsigned char* tx, unsigned char* rx, int len)
{	
	return -1;
}

/******************************************************************************
*  function name | SPI_Data_Send
*  content       | 发数据
*  parameter     | eConnectType 连接类型 tx 发数据buf len 数据长度
*  return        | int 0 success
*  notice        | 
******************************************************************************/
int SPI_Data_Send(DiasSppConnectType_e eConnectType,unsigned char* tx, int len)
{
	int iRet=-1;
	int u8Idx;
	static CHAR acDataLog[DIAS_SPP_PACKET_LEN_MAX*3 + 1] = {0};

	if(SPP_CONNECT_TYPE_MCU_4G==eConnectType)
	{

        (VOID)DiasSppHalSendData(tx,len);

		iRet = OK;

	}
	else if(SPP_CONNECT_TYPE_MCU_MPU==eConnectType)
	{
	
	}
	return iRet;
}

/******************************************************************************
*  function name | SPI_Data_Recv
*  content       | 收数据
*  parameter     | eConnectType 连接类型  rx接收数据buf len buf长度
*  return        | int 0 success
*  notice        | 
******************************************************************************/
int SPI_Data_Recv(DiasSppConnectType_e eConnectType,unsigned char* rx, int len)
{
	int iRet=0;
	int iMsgSize=0;
	UINT8 u8Idx = 0;
	BOOL bValidData = FALSE;
	static CHAR acDataLog[DIAS_SPP_PACKET_LEN_MAX*3 + 1] = {0};
	static UINT8 u8PreByteMpu = 0;
	static UINT8 u8PreByte4G = 0;
	UINT8 *pu8PreByte = NULL;

	if(SPP_CONNECT_TYPE_MCU_4G==eConnectType)
	{

	   iRet = DiasSppHalRecvData(rx,len);

	   if(!iRet)
	   {
	      memset(rx,0,len); 
	       iRet = len;
	   }

		
	}
	else if(SPP_CONNECT_TYPE_MCU_MPU==eConnectType)
	{
	
	}
	else
	{
		return iRet;
	}

	if(iRet > 0)
	{
		for(u8Idx=0; u8Idx<iRet; u8Idx++)
		{
			if(0x00 != (UINT8)rx[u8Idx])
			{
				bValidData = TRUE;
				break;
			}
		}

		if(FALSE == bValidData)
		{
			iRet=0;
		}
		else
		{
		}
	}

	return iRet;
}




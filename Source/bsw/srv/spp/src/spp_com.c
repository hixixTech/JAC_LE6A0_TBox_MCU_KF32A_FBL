#include "spp_com.h"
#include "spp_type.h"
#include "os_log.h"
#include "spp_callout.h"


static int s32InitStatus4G= ERROR;
static int s32InitStatusMpu = ERROR;



/******************************************************************************
*  function name | SppComOpen
*  content       | ��ʼ�� spi �˿�
*  parameter     | eConnectType ��������
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppComOpen(SppConnectType_e eConnectType)
{
    SPPErrorCode_e eRet = SPP_FAILURE;

	if(SPP_CONNECT_TYPE_MCU_4G==eConnectType)
	{
		if(ERROR==s32InitStatus4G)
		{
            (VOID)SppHalInit();
	        eRet = SPP_SUCCESS;
			s32InitStatus4G=OK;
		}
		else
		{
            eRet = SPP_SUCCESS;
		}
	}
	else if(SPP_CONNECT_TYPE_MCU_MPU==eConnectType)
	{
		if(ERROR==s32InitStatusMpu)
		{

			s32InitStatusMpu=ERROR;
		}
		else
		{
            eRet = SPP_SUCCESS;
		}
	}
    else
    {
        
    }
	return eRet;

	
}


/******************************************************************************
*  function name | SppComSend
*  content       | spi�����ַ�
*  parameter     | pu8SendData : ���͵��ַ� eConnectType ��������
                   u32len: ���ͳ���
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppComSend(SppConnectType_e eConnectType,UINT8 *pu8SendData, UINT32 u32len)
{
	SPPErrorCode_e eRet=SPP_FAILURE;

	if(NULL==pu8SendData)
	{
		return SPP_FAILURE;
	}

	eRet=SppHalSendData(pu8SendData, u32len);
	if(SPP_SUCCESS==eRet)
	{
	    //ApiLogPrint(_LOG_ENTRY, "UartSendOK\n");
		return SPP_SUCCESS;
	}
	return SPP_FAILURE;
}


/******************************************************************************
*  function name | SppComRecv
*  content       | spi��ȡ�ַ�
*  parameter     | pu8RevDataBuf : ��ȡ�ַ�Ļ��� eConnectType ��������
                   u32Buflen: ��ȡ����
*  return        | 0 / ��ȡ���ֽ���
*  notice        | 
******************************************************************************/
INT16 SppComRecv(SppConnectType_e eConnectType,UINT8 *pu8RevDataBuf, UINT32 u32len)
{

	INT16 s16Ret=0;

	if(NULL==pu8RevDataBuf)
	{
	    return s16Ret;
	}

	if(SPP_CONNECT_TYPE_MCU_4G==eConnectType)
	{
	   s16Ret = SppHalRecvData(pu8RevDataBuf,u32len);
	}
	else if(SPP_CONNECT_TYPE_MCU_MPU==eConnectType)
	{
	}
	else
	{
	}
	if(0 < s16Ret)
	{		
		return s16Ret;
	}
	return 0;
}


/******************************************************************************
*  function name | SppComRecvTimeout
*  content       | receive timeout
*  parameter[1]  | eConnectType :
*  parameter[2]  | pu8Data :
*  notice        | 
*  return        | 
******************************************************************************/
INT16 SppComRecvTimeout(SppConnectType_e eConnectType,UINT8* pu8Data)
{
    INT16 s16RecRtv;
    UINT32 u32Timeout;
    u32Timeout = 0x60000;
    s16RecRtv = 0;
    while ((s16RecRtv == 0)&&(u32Timeout !=0))
    {            
        s16RecRtv = SppHalRecvData(pu8Data,1);
        u32Timeout--;
    }
    return s16RecRtv;
}


/******************************************************************************
*  function name | SppComRecv
*  content       | ����ʵ����Ч���
*  parameter[1]  | eConnectType :��������
*  parameter[2]  | pu8RevDataBuf :��ݷ��ص�ַ
*  parameter[3]  | u32len :������ݳ���
*  notice        | �ų�Ƿ���ݺ�������
*  return        | 0����ǰ����ݰ� other����Ϣ���
******************************************************************************/
UINT32 SppComRecvValidData(SppConnectType_e eConnectType,UINT8 *pu8RevDataBuf, UINT32 u32len)
{
    INT16 s16RecRtv;
    UINT32 u32Timeout;
    UINT8 u8_data;
    UINT16 u16RecIndex;
    UINT8 pu8acketHead[3] = {0,0,0};
    if (pu8RevDataBuf == NULL)
    {
        return 0;
    }
    if(SPP_CONNECT_TYPE_MCU_4G == eConnectType)
    {
        while(1)  /*packet head*/
        {
            s16RecRtv = SppHalRecvData(&pu8acketHead[0],1);
            if((s16RecRtv == 1)
                &&(pu8acketHead[0] == SPP_PACKET_HEAD_DLE))
            {
                s16RecRtv = SppComRecvTimeout(eConnectType,&pu8acketHead[1]);
                if((s16RecRtv == 1)
                    &&(pu8acketHead[1] == SPP_PACKET_HEAD_SOM))
                {                
                    s16RecRtv = SppComRecvTimeout(eConnectType,&pu8acketHead[2]);                    
                    break;
                }
                else{}
            }
            else{}                          
          
            if (s16RecRtv == 0)
            {
                return 0;
            }
            
        }
        u16RecIndex = 2;
        (void)memcpy(pu8RevDataBuf,pu8acketHead,3);
        
        SppHalRecvData(&pu8RevDataBuf[3],253);
        return 256;
#if 1
        while(1)  /*payload*/
        {
            s16RecRtv = SppComRecvTimeout(eConnectType,&u8_data);
            if(s16RecRtv == 1)
            {
                u16RecIndex ++;
                pu8RevDataBuf[u16RecIndex] = u8_data;
                if (u8_data == SPP_PACKET_TAIL_DLE)
                {
                    s16RecRtv = SppComRecvTimeout(eConnectType,&u8_data);
                    if (s16RecRtv == 1)
                    {
                        u16RecIndex ++;
                        pu8RevDataBuf[u16RecIndex] = u8_data;
                        if (u8_data == SPP_PACKET_TAIL_EOF)
                        {
                            return (u16RecIndex + 1);
                        }
                        else if(u16RecIndex == (u32len-1))
                        {
                            ApiLogPrint(_LOG_ENTRY, "SPP:TAILEOFerr\n");
                            return 0;
                        }
                        else
                        {}
                    }
                }
                else if (u16RecIndex == (u32len-2))
                {
                    ApiLogPrint(_LOG_ENTRY, "SPP:TAILDLEerr\n");
                    return 0;
                }
                else
                {}

            }
            else
            {
                ApiLogPrint(_LOG_ENTRY, "SPP:PAYLOADerr\n");
                return 0;
            }
        }
#endif
    }
    else
    {
        return 0;  /*no data*/
    }
}


/******************************************************************************
*  function name | SppComTransfer
*  content       | spi�շ��ַ�
*  parameter     | pu8SendData ���ַ�Ļ��� eConnectType ��������
				   pu8RevDataBuf : ��ȡ�ַ�Ļ���
                   u32Buflen: �շ�����
*  return        | 0 / ��ȡ���ֽ���
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppComTransfer(SppConnectType_e eConnectType,UINT8 *pu8SendData,UINT8 *pu8RevDataBuf, UINT32 u32len)
{
	return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppComClose
*  content       | ��ʼ�� spi �˿�
*  parameter     | eConnectType ��������
*  return        | SPPErrorCode_e
*  notice        | 
******************************************************************************/
SPPErrorCode_e SppComClose(SppConnectType_e eConnectType)
{
    return SPP_SUCCESS;

}

/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     spp_callout.c                                         */
/* Author:        Jianzhou.Zhang         Version:  v11.01.01.01.00        Data�� 2020-01-03*/
/* Description:   sppCallout                                                 */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:     RH850F1L                                                   */
/* Compiler:      GHS_201517                                                 */
/********************************** �޸���ʷ *********************************/
/* Date             Version    Author            Description                 */
/* 2020-01-03       v11.01.01.01.00         Jianzhou.Zhang              �½�����    */
/*****************************************************************************/



#include "spp_type.h"
#include "spp_callout.h"
#include "string.h"
#include "ecu_usart.h"


/*#include "mcu_com_uart.h"*//*by s811*/
/* #include "Os.h"
#include "Os_cfg.h" *//*by s811*/

#define MAX_FRAME_LENGTH (256u)
#define MAX_UART_BUFFER_LENGTH (MAX_FRAME_LENGTH*8)

static  UINT8 au8UartBuffer[MAX_UART_BUFFER_LENGTH] = {0x00};
static  UINT16 u16UartHead = 0;
static  UINT16 u16UartTail = 0;
static  UINT16 u16UartCount= 0;
static  BOOL bPortAlreadyOpenFlag = 0;
static UINT8 u8ContinueSearchCount =0;
static UINT8 au8Head[3] = {0,0,0};


static UINT16 au16UartBuffLenCfg[7] = {0,
    SPP_BIG_PACKET_LEN_MAX,   
    SPP_BIG_PACKET_LEN_MIN,
    SPP_BIG_PACKET_LEN_MAX,
    SPP_BIG_PACKET_LEN_MIN,
    SPP_BIG_PACKET_LEN_MAX,
    SPP_BIG_PACKET_LEN_MIN};

/******************************************************************************
*  function name | SppHalInit
*  content       | Spp driver init
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppHalInit(void)
{
    INT32 status;
    if(bPortAlreadyOpenFlag == FALSE)
    {
	    memset(au8UartBuffer, 0x00, MAX_UART_BUFFER_LENGTH);
        u16UartHead = 0;
        u16UartTail = 0;
        bPortAlreadyOpenFlag = 1;
        u16UartCount = 0;        
        u8ContinueSearchCount = 0;
        /*status = ApiUartLteInit(BAUDRATE_921600);*/ /* by S811 */
        if(status != 0)
        {
            return SPP_FAILURE;
        }
		
        /*ApiUartLteReadStart(au8UartBuffer, MAX_UART_BUFFER_LENGTH);*/ /* by S811 */
		
        return SPP_SUCCESS;
    }
    else
    {
        return SPP_FAILURE;
    }
}


/******************************************************************************
*  function name | SppHalDeInit
*  content       | spp driver deinit
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppHalDeInit(void)
{
    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppHalOpen
*  content       | spp open
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppHalOpen(void)
{    
    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppHalClose
*  content       | spp close
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppHalClose(void)
{    
    return SPP_SUCCESS;
}


/******************************************************************************
*  function name | SppSendReady
*  content       | spp send ready
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppSendReady(void)
{
    UINT8 u8_cmp = 0;
    UINT32 u32_timeout = 0x3000;
    UINT8 i;
    while((u8_cmp == 0)&&(u32_timeout!=0))
    {
        /*u8_cmp = ApiUartLteSendComplete();*/ /* by S811 */
        u32_timeout --;
    }
    if (u32_timeout == 0)
    {
        return SPP_FAILURE;
    }
    else
    {
        return SPP_SUCCESS;
    }
}


/******************************************************************************
*  function name | SppHalSendData
*  content       | send data
*  parameter[1]  | pu8Tx :
*  parameter[2]  | len :
*  notice        | 
*  return        | 
******************************************************************************/
SPPErrorCode_e SppHalSendData(UINT8* pu8Tx, INT16 len)
{ 
    UINT8 u8_cmp = 0;
    INT32 iRtv = 0;
    UINT32 u32_timeout = 0x6000;
    while((u8_cmp == 0)&&(u32_timeout!=0))
    {
        /* u8_cmp = ApiUartLteSendComplete(); */ /* by s811 */
        u8_cmp = 1; /* use for debug */
        u32_timeout --;
    }
    if (u32_timeout == 0)
    {
        return SPP_FAILURE;
    }
    /* iRtv = ApiUartLteSend(pu8Tx,len); */ /* by s811 */
    ApiUartDebugSend(pu8Tx,len);
    #if 0/* use for debug */
    ApiUartDebugSend(pu8Tx,(UINT32)len);
    #endif
    if (iRtv == 0)
    {
        return SPP_SUCCESS;
    }
    else
    {
        return SPP_FAILURE;
    }    
}


/******************************************************************************
*  function name | SppUartRec
*  content       | spp data receive
*  notice        | 
*  return        | 
******************************************************************************/
void SppUartRec(void)
{
    /* u16UartTail = ApiUartLteRecvPosition(); */ /* by s811 */

    if( u16UartHead<= u16UartTail)
    {
        u16UartCount = u16UartTail - u16UartHead;
    }
    else
    {
        u16UartCount = u16UartTail + MAX_UART_BUFFER_LENGTH - u16UartHead;
    }
}


/******************************************************************************
*  function name | SppUartRecDecrease
*  content       | receive data decrease
*  notice        | 
*  return        | 
******************************************************************************/
void SppUartRecDecrease(void)
{
    if (u16UartCount>0)
    {
        u16UartHead++;
        u16UartCount--;
        if (u16UartHead>= MAX_UART_BUFFER_LENGTH )
        {
            u16UartHead= 0;
        }
        
    }
}


/******************************************************************************
*  function name | SppUartRecDecreaseByNum
*  content       | receive data decrease by number
*  parameter[1]  | num :
*  notice        | 
*  return        | 
******************************************************************************/
void SppUartRecDecreaseByNum(UINT16 num)
{
    if (u16UartCount > num)
    {
        while(num != 0)
        {
            u16UartHead++;
            u16UartCount--;
            if (u16UartHead>= MAX_UART_BUFFER_LENGTH )
            {
                u16UartHead= 0;
            }
            num --;
        }        
    }
}


/******************************************************************************
*  function name | SppHalRecvData
*  content       | receive and decode data
*  parameter[1]  | pu8RecvData :
*  parameter[2]  | length :
*  notice        | 
*  return        | 
******************************************************************************/
INT16 SppHalRecvData(UINT8* pu8RecvData, UINT16 length)
{
    BOOL bl_continue = FALSE;
    UINT16 u16_remainLenReqire = 0;
    UINT16 i;
    if (pu8RecvData == (void*)0)        
    {
        return 0;
    }
    length = length;
    
    SppUartRec();
    if (u8ContinueSearchCount == 0)
    {
        if (u16UartCount < SPP_BIG_PACKET_LEN_MIN)
        {
            return 0;
        }
        else
        {
            while (u16UartCount !=0)
            {
                if (au8UartBuffer[u16UartHead] == SPP_PACKET_HEAD_DLE)
                {
                    au8Head[0] = au8UartBuffer[u16UartHead];
                    SppUartRecDecrease();
                    if ((u16UartCount >=1)
                        &&(au8UartBuffer[u16UartHead] == SPP_PACKET_HEAD_SOM))
                    {
                        au8Head[1] = au8UartBuffer[u16UartHead];
                        u8ContinueSearchCount++;
                     //   ApiLogPrint(_LOG_ENTRY, "FindHead:head %dtail%dcount%d\n ",u16UartHead,u16UartTail,u16UartCount);
                        SppUartRecDecrease();
                        break;
                    }
                    else
                    {                        
                        SppUartRecDecrease();
                    }
                }
                else
                {
                    SppUartRecDecrease();
                }
            }
            if (u8ContinueSearchCount == 0)
            {
                ApiLogPrint(_LOG_ENTRY, "FindHeadErr\n");
                return 0;
            }
        }
    }
    if (u8ContinueSearchCount == 0)
    {
        //ApiLogPrint(_LOG_ENTRY, "FindHeadErr\n");
        return 0;
    }
    if ((au8UartBuffer[u16UartHead] <7)
        &&(au8UartBuffer[u16UartHead]>0))
    {
        u16_remainLenReqire = au16UartBuffLenCfg[au8UartBuffer[u16UartHead]] - 2;
    }
    else
    {
        u8ContinueSearchCount = 0;
        ApiLogPrint(_LOG_ENTRY, "typeErr\n");
        return 0;
    }
    if (u16UartCount >= u16_remainLenReqire)
    {
    
        //ApiLogPrint(_LOG_ENTRY, "FindRemain:head %dtail%dcount%d\n ",u16UartHead,u16UartTail,u16UartCount);
        u8ContinueSearchCount = 0;
        
        for(i = 0; i < (u16_remainLenReqire-1); i++)
        {
            pu8RecvData[i+2] = au8UartBuffer[u16UartHead];
            pu8RecvData[i+3] = au8UartBuffer[(u16UartHead+1)%MAX_UART_BUFFER_LENGTH];            
            SppUartRecDecrease();
            if ((pu8RecvData[i+2] == SPP_PACKET_TAIL_DLE)
                &&(pu8RecvData[i+3] == SPP_PACKET_TAIL_EOF))
            {
                //break;
            }
        }
        SppUartRecDecrease();
        if (pu8RecvData[u16_remainLenReqire] != SPP_PACKET_TAIL_DLE
        ||pu8RecvData[u16_remainLenReqire+1] != SPP_PACKET_TAIL_EOF)
        {
            ApiLogPrint(_LOG_ENTRY, "LostOrErrFram%d %d\n",i,u16_remainLenReqire-2);
            ApiLogPrint(_LOG_ENTRY, "%2x %2x %2x %2x %2x %2x %2x %2x\n", pu8RecvData[0], pu8RecvData[1], pu8RecvData[2], pu8RecvData[3], pu8RecvData[4], pu8RecvData[5]
                , pu8RecvData[6], pu8RecvData[7]);
            return 0;
        }
        else
        {
            memcpy(pu8RecvData,au8Head,2);
            return (u16_remainLenReqire+2);
        }
    }
    else
    {
        if (u8ContinueSearchCount >= (2*PROCESS_TIMES_ONE_CYCLE))  /*��һ�����ڽ�����δ�������*/
        {
            u8ContinueSearchCount = 0;
            ApiLogPrint(_LOG_ENTRY, "FindTailTimeOut\n");
        }
        else if(u8ContinueSearchCount >= 1)
        {
            u8ContinueSearchCount++;
        }
        else
        {}
        return 0;
    }
    
    
}


/******************************************************************************
*  function name | SppEnterCritical
*  content       | enter critical
*  notice        | 
*  return        | by S811
******************************************************************************/
/*
void SppEnterCritical(void)
{
    GetResource(gtDiagLock);
    
}
*/


/******************************************************************************
*  function name | SppLeaveCritical
*  content       | leave critical
*  notice        | 
*  return        | by S811
******************************************************************************/
/*void SppLeaveCritical(void)
{
    ReleaseResource(gtDiagLock); 
}*/




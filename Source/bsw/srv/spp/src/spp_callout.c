/*****************************************************************************/
/* Copyright (C), 2020, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:     dias_spp_callout.c                                         */
/* Author:        Jianzhou.Zhang         Version:  v11.01.01.01.00        Data： 2020-01-03*/
/* Description:   sppCallout                                                 */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:     RH850F1L                                                   */
/* Compiler:      GHS_201517                                                 */
/********************************** 修改历史 *********************************/
/* Date             Version    Author            Description                 */
/* 2020-01-03       v11.01.01.01.00         Jianzhou.Zhang              新建基线    */
/*****************************************************************************/



#include "spp_type.h"
#include "spp_callout.h"
#include "string.h"

#include "mcu_com_uart.h"
#define MAX_FRAME_LENGTH (1280u)
#define MAX_UART_BUFFER_LENGTH (MAX_FRAME_LENGTH*4)

static  uint8_t uart_buffer[MAX_UART_BUFFER_LENGTH] = {0x00};
static  uint16_t uart_head = 0;
static  uint16_t uart_tail = 0;
static  BOOL port_already_open_flag = 0;

DiasErrorCode_e DiasSppHalInit(void)
{
    INT32 status;
    if(port_already_open_flag == FALSE)
    {
	    memset(uart_buffer, 0x00, MAX_UART_BUFFER_LENGTH);
        uart_head = 0;
        uart_tail = 0;
        port_already_open_flag = 1;
        status = ApiUartLteInit(BAUDRATE_921600);
        if(status != 0)
        {
            return DIAS_FAILURE;
        }
		
        ApiUartLteReceiveStart(uart_buffer, MAX_UART_BUFFER_LENGTH);
		
        return DIAS_SUCCESS;
    }
    else
    {
        return DIAS_FAILURE;
    }
}

DiasErrorCode_e DiasSppHalDeInit(void)
{
    return DIAS_SUCCESS;
}


DiasErrorCode_e DiasHalOpen(void)
{    
    return DIAS_SUCCESS;
}


DiasErrorCode_e DiasSppHalClose(void)
{    
    return DIAS_SUCCESS;
}

DiasErrorCode_e DiasSppHalSendData(UINT8* tx, INT16 len)
{ 
    UINT8 u8_cmp = 0;
    INT32 iRtv = 0;
    UINT32 u32_timeout = 0x6000;
    //u8_cmp = mcu_com_uart_LTE_SendComplete();
    while((u8_cmp == 0)&&(u32_timeout!=0))
    {
		u8_cmp = ApiUartLTESendComplete();
        u32_timeout --;
    }
    if (u32_timeout == 0)
    {
        return DIAS_FAILURE;
    }

    iRtv = ApiUartLTESend(tx,len);
    if (iRtv == 0)
    {
        return DIAS_SUCCESS;
    }
    else
    {
        return DIAS_FAILURE;
    }    
}

/******************************************************************************
*  function name | DiasSppHalRecvData
*  content       | receive data from mpu
*  parameter[1]  | recv_data :des address 
*  parameter[2]  | length :data length
*  notice        | 
*  return        | data received length
******************************************************************************/
INT16 DiasSppHalRecvData(UINT8* recv_data, uint16_t length)
{
    INT16 nHaveRecv = 0;
    INT16 nTempIndex = 0;
    INT16 nReceivelen = 0;
    if (recv_data == (void*)0)
    {
        return 0;
    }
    uart_tail = ApiUartLTERecvPosition();

    if( uart_head<= uart_tail)
    {
        nHaveRecv = uart_tail - uart_head;
    }
    else
    {
        nHaveRecv = uart_tail + MAX_UART_BUFFER_LENGTH - uart_head;
    }

    if (nHaveRecv >= length)
    {
        if( nHaveRecv >= length)
        {
            nReceivelen = length;
        }
        else
        {
            nReceivelen = nHaveRecv;
        }
        for(nTempIndex = 0; nTempIndex < nReceivelen; nTempIndex++)
        {
            recv_data[nTempIndex] = uart_buffer[uart_head++];
        
            if(uart_head>= MAX_UART_BUFFER_LENGTH)
            {
                uart_head= 0;
            }
        }
        return nReceivelen;
    }
    else
    {
        return 0;
    }
}

void DiasSppInitBuffer(void)
{
    uart_head = uart_tail;
}
void DiasSppEnterCritical(void)
{
    return ;
}

void DiasSppLeaveCritical(void)
{
    return ;
}



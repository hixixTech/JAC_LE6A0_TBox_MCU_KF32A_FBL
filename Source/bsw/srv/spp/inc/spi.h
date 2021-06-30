#ifndef _DIAS_SPI_H_
#define _DIAS_SPI_H_
#include "spp_main.h"

#define SUCCESS        		0x00 
#define ERR_CFG				0xE0
#define ERR_PARAMETER       0xE1 
#define ERR_CMD_SEND        0xE2 
#define ERR_CMD_TIMEOUT     0xE3 
#define ERR_COMM			0xE4
#define ERR_MSG_CHECK		0xE5
#define ERR_MSG_BODY_LEN	0xE6
#define ERR_OPENDEV		    0xE7
#define ERR_SPI_IOCTL		0xE8
#define ERR_MSG_LEN		    0xEA
#define ERR_RSA_LEN			0xEB
#define ERR_GPIO_OPER		0xEC
#define ERR_DATA_LEN		0xED
#define ERR_SEM_FAIL		0xEF

#define DIAS_SPI_BUF_MAX_LEN  128  


int SPI_Open(DiasSppConnectType_e eConnectType);
int SPI_Close(DiasSppConnectType_e eConnectType);
int SPI_Data_Send(DiasSppConnectType_e eConnectType,unsigned char* tx,  int len);
int SPI_Data_Recv(DiasSppConnectType_e eConnectType,unsigned char* rx, int len);
int SPI_Data_Transfer(DiasSppConnectType_e eConnectType,unsigned char* tx, unsigned char* rx, int len);

#endif


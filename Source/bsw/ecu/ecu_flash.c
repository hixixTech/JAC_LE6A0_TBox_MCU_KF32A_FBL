/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_flash.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/05/20
 * BY:           feifei.xu
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

/*****************************************************************************
** #include 
*****************************************************************************/
#include "system_init.h"
#include <string.h>
#include "ecu_flash.h"
#include "ecu_misc.h"

/*****************************************************************************
** #define
*****************************************************************************/
#define FLASH_WORD_LEN				(4)
#define FLASH_BLOCK_LEN  			(8)


#define FLASH_HALF_DATA_LEN 		(FLASH_BLOCK_LEN/2)
#define FLASH_WORD_DATA_LEN 		(FLASH_BLOCK_LEN/4)

/*****************************************************************************
** typedef
*****************************************************************************/
typedef union
{
	UINT8 u8_data[FLASH_BLOCK_LEN];
	UINT16 u16_data[FLASH_HALF_DATA_LEN];
	UINT32 u32_data[FLASH_WORD_DATA_LEN];
}Ecu_Flash_Data_U;

/*****************************************************************************
** global variable
*****************************************************************************/
static FLASH_ProgramTypeDef s_flash_program;


/*****************************************************************************
** static variables
*****************************************************************************/

/*****************************************************************************
** static constants
*****************************************************************************/
static Ecu_Flash_Data_U s_un_flash_data;

/*****************************************************************************
** static function prototypes
*****************************************************************************/


/*****************************************************************************
** function prototypes
*****************************************************************************/
void Ecu_Flash_Init(void)
{

}
void Ecu_Flash_ErasePage(UINT32 u32_addr)
{
	FLASH_Wipe_Configuration_RAM(FLASH_WIPE_CODE_PAGE,u32_addr);
	return TRUE;
}

/****************************************************************************/
/**
 * Function Name: Ecu_Flash_WriteByte
 * Description: flash 程序区写单字节数据
 *				
 * Param:   none
 * Return:  none
 * Author:  2021/05/20, feifei.xu create this function
 ****************************************************************************/
BOOL Ecu_Flash_WriteByteToCode(UINT32 u32_addr,UINT8 u8_buff)
{

	if(0x00 != u32_addr % FLASH_BLOCK_LEN)
	{
		return FALSE;
	}
	
	memset(s_un_flash_data.u8_data,0xFF,FLASH_BLOCK_LEN);
	s_un_flash_data.u8_data[0] = u8_buff;
	s_flash_program.m_Mode = FLASH_PROGRAM_WORD;		//字节写入
	s_flash_program.m_Zone = FLASH_PROGRAM_CODE;		//程序区域
	s_flash_program.m_Addr = u32_addr;					//地址
	s_flash_program.m_WriteSize = 1;					//半页编程不得为0，如果写1（+1）实际写的是2个双字 单字编程无影响
	s_flash_program.m_Data= s_un_flash_data.u32_data;
	FLASH_Program_Configuration_RAM(&s_flash_program);

	return TRUE;

}
/****************************************************************************/
/**
 * Function Name: Ecu_Flash_WriteHalfWord
 * Description: flash 程序区写半字数据
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/21, feifei.xu create this function
 ****************************************************************************/
BOOL Ecu_Flash_WriteHalfWordToCode(UINT32 u32_addr,UINT16 u16_flash_buff)
{

	if(0x00 != u32_addr % FLASH_BLOCK_LEN)
	{
		return FALSE;
	}
	
	memset(s_un_flash_data.u8_data,0xFF,FLASH_BLOCK_LEN);
	s_un_flash_data.u16_data[0] = u16_flash_buff;
	s_flash_program.m_Mode = FLASH_PROGRAM_WORD;		//字节写入
	s_flash_program.m_Zone = FLASH_PROGRAM_CODE;		//程序区域
	s_flash_program.m_Addr = u32_addr;					//地址
	s_flash_program.m_WriteSize = 1;					//半页编程不得为0，如果写1（+1）实际写的是2个双字 单字编程无影响
	s_flash_program.m_Data= s_un_flash_data.u32_data;
	FLASH_Program_Configuration_RAM(&s_flash_program);
	
	return TRUE;
}
/****************************************************************************/
/**
 * Function Name: Ecu_Flash_WriteWord
 * Description: flash 程序区写单字数据
 *
 * Param:   none
 * Return:  none
 * Author:  2021/05/21, feifei.xu create this function
 ****************************************************************************/
BOOL Ecu_Flash_WriteWordToCode(UINT32 u32_addr,UINT32 u32_flash_buff)
{

	if(0x00 != u32_addr % FLASH_BLOCK_LEN)
	{
		return FALSE;
	}
	
	memset(s_un_flash_data.u8_data,0xFF,FLASH_BLOCK_LEN);
	s_un_flash_data.u32_data[0] = u32_flash_buff;
	s_flash_program.m_Mode = FLASH_PROGRAM_WORD;		//字节写入
	s_flash_program.m_Zone = FLASH_PROGRAM_CODE;		//程序区域
	s_flash_program.m_Addr = u32_addr;					//地址
	s_flash_program.m_WriteSize = 1;					//半页编程不得为0，如果写1（+1）实际写的是2个双字 单字编程无影响
	s_flash_program.m_Data= s_un_flash_data.u32_data;
	FLASH_Program_Configuration_RAM(&s_flash_program);

	return TRUE;
}
/****************************************************************************/
/**
 * Function Name: Ecu_Flash_WriteBlockToCode
 * Description: flash 程序区写块数据，8个字节为1块
 *
 * Param:   
 * Return:  none
 * Author:  2021/05/24, feifei.xu create this function
 ****************************************************************************/
BOOL Ecu_Flash_WriteBlockDataToCode(UINT32 u32_addr,UINT8* p_u8_buff, UINT16 u16_len)
{

	if(0x00 != u32_addr % FLASH_BLOCK_LEN)
	{
		return FALSE;
	}

	memset(s_un_flash_data.u8_data,0xFF,FLASH_BLOCK_LEN);
	memcpy(s_un_flash_data.u8_data,p_u8_buff,u16_len);
	s_flash_program.m_Mode = FLASH_PROGRAM_WORD;		//字节写入
	s_flash_program.m_Zone = FLASH_PROGRAM_CODE;		//程序区域
	s_flash_program.m_Addr = u32_addr;					//地址
	s_flash_program.m_WriteSize = 1;					//半页编程不得为0，如果写1（+1）实际写的是2个双字 单字编程无影响
	s_flash_program.m_Data= s_un_flash_data.u32_data;
	FLASH_Program_Configuration_RAM(&s_flash_program);

	return TRUE;
}
/****************************************************************************/
/**
 * Function Name: Ecu_Flash_ReadByte
 * Description: flash 程序区读单字节数据
 *
 * Param:   
 * Return:  none
 * Author:  2021/05/20, feifei.xu create this function
 ****************************************************************************/
BOOL Ecu_Flash_ReadBlockDataFromCode(UINT32 u32_addr, UINT8 *p_u8_buff, UINT16 u16_len)
{

	if(0x00 != u32_addr % FLASH_BLOCK_LEN)
	{
		return FALSE;
	}

	s_un_flash_data.u32_data[0] = Read_Flash_or_CFR_RAM(u32_addr,FLASH_PROGRAM_CODE);
	u32_addr = u32_addr + FLASH_WORD_LEN;
	s_un_flash_data.u32_data[1] = Read_Flash_or_CFR_RAM(u32_addr,FLASH_PROGRAM_CODE);

	
	memcpy(p_u8_buff,s_un_flash_data.u8_data,u16_len);

	return TRUE;

}
/****************************************************************************/
/**
 * Function Name: Ecu_Flash_ReadWriteNumBlockData
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
BOOL Ecu_Flash_ReadWriteNumBlockData(UINT32 u32_addr,UINT8* u8_data,UINT16 u16_len,BOOL b_cmd)
{
	UINT16 u16_i = 0;
	UINT32 u32_flash_addr = u32_addr;

	if(0x00 != u32_flash_addr % FLASH_BLOCK_LEN)
	{
		return FALSE;
	}
	
	for(u16_i = 0; u16_i < u16_len/FLASH_BLOCK_LEN; u16_i++)
	{
		if(TRUE == b_cmd)
		{
			if(FALSE == Ecu_Flash_WriteBlockDataToCode(u32_flash_addr, &u8_data[u16_i*FLASH_BLOCK_LEN], FLASH_BLOCK_LEN))
			{
				return FALSE;
			}
		}
		else
		{
			if(FALSE == Ecu_Flash_ReadBlockDataFromCode(u32_flash_addr,&u8_data[u16_i*FLASH_BLOCK_LEN], FLASH_BLOCK_LEN))
			{
				return FALSE;
			}
		}
		u32_flash_addr = u32_flash_addr + FLASH_BLOCK_LEN;
	}

	return TRUE;
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


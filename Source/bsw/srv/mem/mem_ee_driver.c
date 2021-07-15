/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    mem_ee_driver.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/06/01
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
#include <string.h>
#include "mem_ee_driver.h"
#include "ecu_flash.h"
#include "ecu_misc.h"
// #include "FreeRTOS.h"
#include "os_log.h"
/*****************************************************************************
** #define
*****************************************************************************/
#define EEPROM_BASIC_ADDR       ((UINT32)(0x00018000))                  //EE 起始地址
#define EEPROM_PAGE_SIZE        (0x400)                                 //页大小
#define EEPROM_PAGE_NUM         (8u)                                   //页数量
#define EEPRON_MAX_SIZE         (EEPROM_PAGE_SIZE * EEPROM_PAGE_NUM)    //EE空间大小

#define EEPROM_WRITE_BYTE_MAX   (1024)

#define EEPROM_BLOCK_SIZE       (8)

#define EEPROM_PAGE_0           (0u)
#define EEPROM_PAGE_1           (EEPROM_PAGE_0 + 1)
#define EEPROM_PAGE_2           (EEPROM_PAGE_1 + 1)
#define EEPROM_PAGE_3           (EEPROM_PAGE_2 + 1)
#define EEPROM_PAGE_4           (EEPROM_PAGE_3 + 1)
#define EEPROM_PAGE_5           (EEPROM_PAGE_4 + 1)
#define EEPROM_PAGE_6           (EEPROM_PAGE_5 + 1)
#define EEPROM_PAGE_7           (EEPROM_PAGE_6 + 1)
#define EEPROM_PAGE_8           (EEPROM_PAGE_7 + 1)
#define EEPROM_PAGE_9           (EEPROM_PAGE_8 + 1)
#define EEPROM_PAGE_10          (EEPROM_PAGE_9 + 1)
#define EEPROM_PAGE_11          (EEPROM_PAGE_10 + 1)
#define EEPROM_PAGE_12          (EEPROM_PAGE_11 + 1)
#define EEPROM_PAGE_13          (EEPROM_PAGE_12 + 1)
#define EEPROM_PAGE_14          (EEPROM_PAGE_13 + 1)
#define EEPROM_PAGE_15          (EEPROM_PAGE_14 + 1)
#define EEPROM_PAGE_16          (EEPROM_PAGE_15 + 1)
#define EEPROM_PAGE_17          (EEPROM_PAGE_16 + 1)
#define EEPROM_PAGE_18          (EEPROM_PAGE_17 + 1)
#define EEPROM_PAGE_19          (EEPROM_PAGE_18 + 1)
#define EEPROM_PAGE_20          (EEPROM_PAGE_19 + 1)
#define EEPROM_PAGE_21          (EEPROM_PAGE_20 + 1)
#define EEPROM_PAGE_22          (EEPROM_PAGE_21 + 1)
#define EEPROM_PAGE_23          (EEPROM_PAGE_22 + 1)
#define EEPROM_PAGE_24          (EEPROM_PAGE_23 + 1)
#define EEPROM_PAGE_MAX         (EEPROM_PAGE_24 + 1)

#define EEPROM_HEAD_SIZE        (EEPROM_PAGE_MAX * EEPROM_BLOCK_SIZE)
#define EEPROM_PAGE_BYTE_NUM    (EEPROM_PAGE_SIZE - EEPROM_BLOCK_SIZE)

/*********页头**********/
#define PAGEHEAD_INDEX      (1)
#define PAGEHEAD_STATUS     (1)
#define PAGEHEAD_ERASE_TIME (4)
#define PAGEHEAD_RESERVE    (2)

/*****************************************************************************
** typedef
*****************************************************************************/
typedef enum
{
    EED_CMD_NOME = 0,
    EED_CMD_INIT,
    EED_CMD_ERASE,
    EED_CMD_READ,
    EED_CMD_WRITE,
    EED_CMD_END
}EED_CMD_E;

typedef enum
{
    EED_READ = 0,
    EED_COMPARE
}EED_ERASE_FSM_E;
typedef struct
{
    UINT8 u8_page_num;

    UINT8 u8_f_pageid;  
    UINT8 u8_r_pageid;

    UINT16 u16_f_len;
    UINT16 u16_r_len;

    UINT32 u32_f_reladdr;   //相对地址
    UINT32 u32_r_reladdr;   

    UINT32 u32_f_absaddr;   //绝对地址
    UINT32 u32_r_absaddr;
}EED_PAGE_STS;


typedef struct
{
    EED_CMD_E e_cmd;
    UINT32 u32_id;
    UINT8 u8_read_buff[EEPROM_PAGE_SIZE];
    UINT8 u8_write_buff[EEPROM_PAGE_SIZE];
    EED_PAGE_STS page_sts;
}EED_REQUEST_S;

typedef struct
{
    UINT32 u32_addr;
    UINT32 u32_erase_count;
}EED_PAGE_HEAD;

typedef struct
{
    UINT8 u8_runpage_num;
    UINT8 u8_free_addr_count;
    EED_PAGE_HEAD backpage0;
    EED_PAGE_HEAD backpage1;
    UINT32 u32_free_addr[EEPROM_PAGE_MAX];
    EED_PAGE_HEAD page_head[EEPROM_PAGE_MAX];
}EED_EE_PAGENWM;
/*****************************************************************************
** global variable
*****************************************************************************/


/*****************************************************************************
** static variables
*****************************************************************************/
static EED_REQUEST_S s_st_eed_request;

static EED_EE_PAGENWM s_st_pagenwm;

static UINT8 s_u8_page_buff[EEPROM_PAGE_SIZE];

static UINT8 s_u8_head_block[EEPROM_BLOCK_SIZE];
/*****************************************************************************
** static constants
*****************************************************************************/

/*****************************************************************************
** static function prototypes
*****************************************************************************/
static UINT32 Mem_EED_GetFreeAddr(void);
static BOOL Mem_EED_GetPageSts(EED_PAGE_STS* p_page_sts,UINT16 u16_len,UINT16 offset);
static BOOL Mem_EED_GetAbsoluteAddr(EED_PAGE_STS* p_page_sts, EED_PAGE_HEAD* p_head);
static BOOL Mem_EED_ChangePage(EED_PAGE_STS* p_page_sts, EED_PAGE_HEAD* p_head);
static BOOL Mem_EED_GetEraseFlag(UINT32 u32_id,UINT16 u16_len,UINT16 u16_offset,BOOL* p_flag);
/*****************************************************************************
** function prototypes
*****************************************************************************/
/****************************************************************************/
/**
 * Function Name: Mem_EED_WriteReadFinish
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/10, feifei.xu create this function
 ****************************************************************************/
BOOL Mem_EED_WriteReadFinish(void)
{
    EED_REQUEST_S* p_request = &s_st_eed_request;

    if(EED_CMD_END == p_request->e_cmd)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
void Mem_EED_Init(UINT32 u32_byte_num)
{
    EED_REQUEST_S* p_request = &s_st_eed_request;

    p_request->e_cmd = EED_CMD_INIT;
    p_request->u32_id = 0;
    s_st_pagenwm.u8_free_addr_count = 0;
    memset(p_request->u8_read_buff,0x00,EEPROM_PAGE_SIZE);
    memset(p_request->u8_write_buff,0x00,EEPROM_PAGE_SIZE);
    Mem_EED_RunPageNumInit(u32_byte_num);
    Mem_EED_PageAddrInit();
    Mem_EED_PageHeadInit();
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_RunPageNumInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/10, feifei.xu create this function
 ****************************************************************************/
void Mem_EED_RunPageNumInit(UINT32 u32_byte_num)
{
    s_st_pagenwm.u8_runpage_num = (u32_byte_num / EEPROM_PAGE_BYTE_NUM) + 1;
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_PageAddrInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/10, feifei.xu create this function
 ****************************************************************************/
void Mem_EED_PageAddrInit(void)
{
    UINT8 u8_i = 0;
    UINT32 u32_addr = 0;
    for(u8_i = 0; u8_i < EEPROM_PAGE_MAX; u8_i++)
    {
        u32_addr = EEPROM_BASIC_ADDR + (u8_i * EEPROM_PAGE_SIZE);

        (void)Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_head_block,EEPROM_BLOCK_SIZE,FALSE);

        (void)ApiMiscCompare(s_u8_head_block,0xFF,EEPROM_BLOCK_SIZE);
        
        if(s_u8_head_block[0] < s_st_pagenwm.u8_runpage_num)
        {
            s_st_pagenwm.page_head[s_u8_head_block[0]].u32_addr = u32_addr;
        }
        else
        {
            s_st_pagenwm.u32_free_addr[s_st_pagenwm.u8_free_addr_count] = u32_addr;
            s_st_pagenwm.u8_free_addr_count++;
        }
    }
}

/****************************************************************************/
/**
 * Function Name: Mem_EED_GetFreeAddr
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/10, feifei.xu create this function
 ****************************************************************************/
static UINT32 Mem_EED_GetFreeAddr(void)
{
    UINT8 u8_i = 0;
    UINT32 u32_addr = 0x00;

    for(u8_i = 0; u8_i < s_st_pagenwm.u8_free_addr_count; u8_i++)
    {
        if(0x00 == s_st_pagenwm.u32_free_addr[u8_i])
        {
            continue;
        }
        u32_addr = s_st_pagenwm.u32_free_addr[u8_i];
        s_st_pagenwm.u32_free_addr[u8_i] = 0x00;
        break;
    }

    return u32_addr;
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_PageHeadInit
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/10, feifei.xu create this function
 ****************************************************************************/
void Mem_EED_PageHeadInit(void)
{
    UINT8 u8_i = 0;
    UINT8 u8_count = 0;
    UINT32 u32_addr = 0;

    memset(s_u8_head_block,0x00,EEPROM_BLOCK_SIZE);

    for(u8_i = 0; u8_i < s_st_pagenwm.u8_runpage_num; u8_i++)
    {
        if(0x00 != s_st_pagenwm.page_head[u8_i].u32_addr)
        {
            continue;
        }

        s_u8_head_block[0] = u8_i;
        u32_addr = Mem_EED_GetFreeAddr();
        (void)Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_head_block,EEPROM_BLOCK_SIZE,TRUE);
        s_st_pagenwm.page_head[u8_i].u32_addr = u32_addr;
    }

    s_st_pagenwm.backpage0.u32_addr = Mem_EED_GetFreeAddr();
    s_st_pagenwm.backpage1.u32_addr = Mem_EED_GetFreeAddr();
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_Read
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
BOOL Mem_EED_Read(UINT32 u32_id,UINT8* u8_data,UINT16 u16_len,UINT16 u16_offset)
{
    EED_REQUEST_S* p_request = &s_st_eed_request;

    if(u16_len > EEPROM_WRITE_BYTE_MAX)
    {
        return FALSE;
    }

    // p_request->e_cmd = EED_CMD_READ;
    p_request->u32_id = u32_id;
    memset(p_request->u8_read_buff,0x00,EEPROM_PAGE_SIZE);

    //判断是否需要分页读
    if(FALSE == Mem_EED_GetPageSts(&p_request->page_sts,u16_len,u16_offset))
    {
        return FALSE;
    }
    //获取绝对地址
    if(FALSE == Mem_EED_GetAbsoluteAddr(&p_request->page_sts,s_st_pagenwm.page_head))
    {
        return FALSE;
    }

    //读起始页数据
    (void)Ecu_Flash_ReadWriteNumBlockData(  p_request->page_sts.u32_f_absaddr,
                                            p_request->u8_read_buff,
                                            p_request->page_sts.u16_f_len,
                                            FALSE);

    if(0x02 == p_request->page_sts.u8_page_num)
    {
        //读终止页数据
        (void)Ecu_Flash_ReadWriteNumBlockData(  p_request->page_sts.u32_r_absaddr,
                                                &p_request->u8_read_buff[p_request->page_sts.u16_f_len],
                                                p_request->page_sts.u16_r_len,
                                                FALSE);
    }
    //数据返回
    memcpy(u8_data,p_request->u8_read_buff,u16_len);

    return TRUE;
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_Write
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
BOOL Mem_EED_Write(UINT32 u32_id,UINT8* u8_data,UINT16 u16_len,UINT16 u16_offset)
{
    BOOL b_flag = FALSE; 
    EED_REQUEST_S* p_request = &s_st_eed_request;

    if(u16_len > EEPROM_WRITE_BYTE_MAX)
    {
        return FALSE;
    }
    //判断是否要执行换页算法
    if(FALSE == Mem_EED_GetEraseFlag(u32_id,u16_len,u16_offset,&b_flag))
    {
        return FALSE;
    }

    p_request->u32_id = u32_id;
    memset(p_request->u8_write_buff,0x00,EEPROM_PAGE_SIZE);
    memcpy(p_request->u8_write_buff,u8_data,u16_len);

    //判断是否需要分页
    if(FALSE == Mem_EED_GetPageSts(&p_request->page_sts,u16_len,u16_offset))
    {
        return FALSE;
    }
    //执行换页算法

    if(TRUE == b_flag)
    {
        (void)Mem_EED_ChangePage(&p_request->page_sts,s_st_pagenwm.page_head);
    }

    //获取绝对地址
    if(FALSE == Mem_EED_GetAbsoluteAddr(&p_request->page_sts,s_st_pagenwm.page_head))
    {
        return FALSE;
    }
    //写起始页数据
    (void)Ecu_Flash_ReadWriteNumBlockData(  p_request->page_sts.u32_f_absaddr,
                                            p_request->u8_write_buff,
                                            p_request->page_sts.u16_f_len,
                                            TRUE);
    if(0x02 == p_request->page_sts.u8_page_num)
    {
        //写终止页数据
        (void)Ecu_Flash_ReadWriteNumBlockData(  p_request->page_sts.u32_r_absaddr,
                                                &p_request->u8_write_buff[p_request->page_sts.u16_f_len],
                                                p_request->page_sts.u16_r_len,
                                                TRUE);
    }

    return TRUE;
}

/****************************************************************************/
/**
 * Function Name: Mem_EED_ChangePage
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/09, feifei.xu create this function
 ****************************************************************************/
static BOOL Mem_EED_ChangePage(EED_PAGE_STS* p_page_sts, EED_PAGE_HEAD* p_head)
{

    UINT16 u16_len = 0;
    UINT32 u32_addr = 0;
    UINT32 u32_count = 0;

    u16_len = p_page_sts->u32_f_reladdr;    //相对地址就是需要复制的字节数
    u32_addr = s_st_pagenwm.page_head[p_page_sts->u8_f_pageid].u32_addr;

    if(FALSE == Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_page_buff,u16_len,FALSE))
    {
        return FALSE;
    }
    
    u32_addr = s_st_pagenwm.backpage0.u32_addr;
    if(FALSE == Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_page_buff,u16_len,TRUE))
    {
        return FALSE;
    }

    if((p_page_sts->u32_f_reladdr + p_page_sts->u16_f_len) < EEPROM_PAGE_SIZE)
    {
        u32_addr = s_st_pagenwm.page_head[p_page_sts->u8_f_pageid].u32_addr;
        u32_addr = u32_addr + p_page_sts->u32_f_reladdr + p_page_sts->u16_f_len;
        u16_len = EEPROM_PAGE_SIZE - p_page_sts->u32_f_reladdr - p_page_sts->u16_f_len;

        if(FALSE == Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_page_buff,u16_len,FALSE))
        {
            return FALSE;
        }

        u32_addr = s_st_pagenwm.backpage0.u32_addr + p_page_sts->u32_f_reladdr + p_page_sts->u16_f_len;;

        if(FALSE == Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_page_buff,u16_len,TRUE))
        {
            return FALSE;
        }
    }

    u32_addr = s_st_pagenwm.page_head[p_page_sts->u8_f_pageid].u32_addr;
    s_st_pagenwm.page_head[p_page_sts->u8_f_pageid].u32_addr = s_st_pagenwm.backpage0.u32_addr;
    s_st_pagenwm.backpage0.u32_addr = u32_addr;
    Ecu_Flash_ErasePage(s_st_pagenwm.backpage0.u32_addr);
    ApiLogPrint(_LOG_DEBUG,"ee erase page id:%x\n",s_st_pagenwm.backpage0.u32_addr);
    /*******************************************/

    if(0x02 == p_page_sts->u8_page_num)
    {
        u16_len = p_page_sts->u32_r_reladdr;
        u32_addr = s_st_pagenwm.page_head[p_page_sts->u8_r_pageid].u32_addr;

        if(FALSE == Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_page_buff,u16_len,FALSE))
        {
            return FALSE;
        }

        u32_addr = s_st_pagenwm.backpage1.u32_addr;
        if(FALSE == Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_page_buff,u16_len,TRUE))
        {
            return FALSE;
        }

        if((p_page_sts->u32_r_reladdr + p_page_sts->u16_r_len) < EEPROM_PAGE_SIZE)
        {
            u32_addr = s_st_pagenwm.page_head[p_page_sts->u8_r_pageid].u32_addr;
            u32_addr = u32_addr + p_page_sts->u32_f_reladdr + p_page_sts->u16_f_len;
            u16_len = EEPROM_PAGE_SIZE - p_page_sts->u32_f_reladdr - p_page_sts->u16_f_len;

            if(FALSE == Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_page_buff,u16_len,FALSE))
            {
                return FALSE;
            }

            u32_addr = s_st_pagenwm.backpage1.u32_addr + p_page_sts->u32_f_reladdr + p_page_sts->u16_f_len;;

            if(FALSE == Ecu_Flash_ReadWriteNumBlockData(u32_addr,s_u8_page_buff,u16_len,TRUE))
            {
                return FALSE;
            }
        }

        u32_addr = s_st_pagenwm.page_head[p_page_sts->u8_r_pageid].u32_addr;
        s_st_pagenwm.page_head[p_page_sts->u8_r_pageid].u32_addr = s_st_pagenwm.backpage1.u32_addr;
        s_st_pagenwm.backpage1.u32_addr = u32_addr;
        Ecu_Flash_ErasePage(s_st_pagenwm.backpage1.u32_addr);
        ApiLogPrint(_LOG_DEBUG,"ee erase page id:%x\n",s_st_pagenwm.backpage1.u32_addr);
    }

    return TRUE;
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_GetEraseFlag
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/09, feifei.xu create this function
 ****************************************************************************/
static BOOL Mem_EED_GetEraseFlag(UINT32 u32_id,UINT16 u16_len,UINT16 u16_offset,BOOL* p_flag)
{
    BOOL b_reg = FALSE;

    if(TRUE == Mem_EED_Read(u32_id,s_u8_page_buff,u16_len,u16_offset))
    {
        if(TRUE == ApiMiscCompare(s_u8_page_buff,0xFF,u16_len))
        {
            *p_flag = FALSE;
        }
        else
        {
            *p_flag = TRUE;
        }
        b_reg = TRUE;
    }
    else
    {
        b_reg = FALSE;
    }

    return b_reg;
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_GetPageSts
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/09, feifei.xu create this function
 ****************************************************************************/
static BOOL Mem_EED_GetPageSts(EED_PAGE_STS* p_page_sts,UINT16 u16_len ,UINT16 offset)
{

    UINT16 u16_last_len;

    if(u16_len > EEPROM_WRITE_BYTE_MAX)
    {
        return FALSE;
    }

    if((offset + EEPROM_HEAD_SIZE)> EEPRON_MAX_SIZE)
    {
        return FALSE;
    }

    p_page_sts->u8_f_pageid = (offset / EEPROM_PAGE_BYTE_NUM);              //第一页虚拟标号
    p_page_sts->u32_f_reladdr = offset % EEPROM_PAGE_BYTE_NUM + EEPROM_BLOCK_SIZE;  //获得相对地址
    u16_last_len = EEPROM_PAGE_BYTE_NUM - p_page_sts->u32_f_reladdr;        //获得剩余可写字节数

    if(u16_len > u16_last_len)                                              //判断需不需要分页
    {
        p_page_sts->u8_page_num = 2;                                        //分页
        p_page_sts->u16_f_len = u16_last_len;                               //第一页长度
        p_page_sts->u8_r_pageid = p_page_sts->u8_f_pageid + 1;              //第二页虚拟标号
        p_page_sts->u32_r_reladdr = EEPROM_BLOCK_SIZE;
        p_page_sts->u16_r_len = u16_len - u16_last_len;                     //第二页长度
    }
    else
    {
        p_page_sts->u8_page_num = 1;                                        //不分页
        p_page_sts->u16_f_len = u16_len;                                    //页长度
    }

    return TRUE;
}
/****************************************************************************/
/**
 * Function Name: Mem_EED_GetActualAddr
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/09, feifei.xu create this function
 ****************************************************************************/
static BOOL Mem_EED_GetAbsoluteAddr(EED_PAGE_STS* p_page_sts, EED_PAGE_HEAD* p_head)
{
    BOOL b_reg = FALSE;

    if(p_page_sts->u8_f_pageid > EEPROM_PAGE_MAX)
    {
        return FALSE;
    }

    p_page_sts->u32_f_absaddr = s_st_pagenwm.page_head[p_page_sts->u8_f_pageid].u32_addr + p_page_sts->u32_f_reladdr;

    if(0x02 == p_page_sts->u8_page_num)
    {
        p_page_sts->u32_r_absaddr = s_st_pagenwm.page_head[p_page_sts->u8_r_pageid].u32_addr + p_page_sts->u32_r_reladdr;
    }

    return TRUE;
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


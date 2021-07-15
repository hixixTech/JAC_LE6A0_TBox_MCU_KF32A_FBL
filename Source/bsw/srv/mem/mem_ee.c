/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    mem_ee.c
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
#include "string.h"
#include "os_log.h"
#include "ecu_misc.h"
#include "mem_ee.h"
#include "mem_ee_data.h"
#include "mem_ee_driver.h"
#include "ecu_gpio.h"
/*****************************************************************************
** #define
*****************************************************************************/

/*****************************************************************************
** typedef
*****************************************************************************/
typedef enum
{
    E2_STEP_INIT = 0,
    E2_STEP_READ,
    E2_STEP_WAIT_READ,
    E2_STEP_WRITE,
    E2_STEP_WAIT_WRITE,
    E2_STEP_END
}E2_STEP_FSM_E;
typedef enum
{
    EE_DATA_INIT = 0,
    EE_DATA_BUSY,
    EE_DATA_READ,
    EE_DATA_READ_WAIT,
    EE_DATA_FINISH
}EE_DataInitCmd_S;

typedef struct
{
    UINT32 u32_id;
    UINT16 u16_len;
    UINT8* p_u8_ram;
    UINT16 u16_offset;
    UINT32 u32_initdata;

    /****************/
    UINT8* p_u8_read_data;
     
}EE_DATA_VAR_S;

/*****************************************************************************
** static variables**/
static UINT8* s_p_u8_ram;
static EE_DATA_VAR_S s_st_ee_var;

static BOOL s_b_MemE2DataInitDone = FALSE;  //ee初始化，TURE：初始化完成    FALSE：未初始化
static BOOL s_b_MemE2DataWriteFlag = TRUE;  //ee write，TRUE：写完成        FALSE：正在写入
static BOOL s_b_MemE2DataReadFlag  = TRUE;  //ee write，TRUE：写完成        FALSE：正在写入

static UINT16 s_u16_ee_EntryMapIdx;
/*****************************************************************************
** global variable
*****************************************************************************/
EE_DataInitCmd_S st_EEDataInitCmd = EE_DATA_INIT;
static E2_STEP_FSM_E s_e_step_fsm = E2_STEP_INIT;
/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/
static void Mem_EE_DataInit(void);
static void Mem_EE_SetWriteMode(void);

static void Mem_EE_OperationHandler10ms(void);
/*****************************************************************************
** function prototypes
*****************************************************************************/
/****************************************************************************/
/**
 * Function Name: Mem_EE_Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
void Mem_EE_Init(void)
{
    Mem_EED_Init(u32_e2_byte_num);
    Mem_EE_DataInit();//这个初始化必须放在最后，否则会出错
}
/****************************************************************************/
/**
 * Function Name: Mem_EE_SetEntry
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
static BOOL Mem_EE_SetEntry(UINT16 u16_EntryIdx)
{
    EE_DATA_VAR_S* p_ee_var = &s_st_ee_var;

    if(u16_EntryIdx < u16_e2_data_num)
    {
        p_ee_var->u32_id = c_st_e2DataMap[u16_EntryIdx].u32_eeprom_id;
        p_ee_var->u16_len = c_st_e2DataMap[u16_EntryIdx].u16_eeprom_len;
        p_ee_var->p_u8_ram = c_st_e2DataMap[u16_EntryIdx].u16_eeprom_offset + p_e2_ram;
        p_ee_var->u16_offset = c_st_e2DataMap[u16_EntryIdx].u16_eeprom_offset;
        p_ee_var->u32_initdata = c_st_e2DataMap[u16_EntryIdx].u32_init_data;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}
/****************************************************************************/
/**
 * Function Name: Mem_EE_DataInitHandler
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
static void Mem_EE_DataInit(void)
{
    EE_DATA_VAR_S* p_ee_var = &s_st_ee_var;

    s_u16_ee_EntryMapIdx = 0;
    while(!s_b_MemE2DataInitDone)
    {
        if (Mem_EE_SetEntry(s_u16_ee_EntryMapIdx))
        {
            if(FALSE == Mem_EED_Read(p_ee_var->u32_id, p_ee_var->p_u8_ram,p_ee_var->u16_len, p_ee_var->u16_offset))
            {
                continue;
            }

            if(TRUE == ApiMiscCompare(p_ee_var->p_u8_ram, 0xFF, p_ee_var->u16_len))
            {
                memset(p_ee_var->p_u8_ram, (UINT8)p_ee_var->u32_initdata, p_ee_var->u16_len);
            }
            s_u16_ee_EntryMapIdx++;
        }
        else
        {
            ApiLogPrint(_LOG_DEBUG,"FBL:eeprom init finish!\n");
            s_b_MemE2DataInitDone = TRUE;
        }
    }
}
/****************************************************************************/
/**
 * Function Name: Mem_EE_SearchIndex
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
UINT16 Mem_EE_SearchIndex(UINT32 u32_id)
{
    UINT16 u16_index = 0;

    for(u16_index = 0; u16_index < u16_e2_data_num; u16_index++)
    {
        if(c_st_e2DataMap[u16_index].u32_eeprom_id == u32_id)
        {
            return u16_index;
        }
    }
    return u16_e2_data_num;
}
/****************************************************************************/
/**
 * Function Name: Mem_EE_Read
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
BOOL Mem_EE_Read(UINT32 u32_id,UINT8* u8_data,UINT16 u16_len)
{
    BOOL b_reg = FALSE;
    UINT16 u16_index = 0;
    EE_DATA_VAR_S* p_ee_var = &s_st_ee_var;

    if(!s_b_MemE2DataInitDone)
    {
        return;
    }

    u16_index = Mem_EE_SearchIndex(u32_id);

    if(TRUE == Mem_EE_SetEntry(u16_index))
    {
        if(u16_len > p_ee_var->u16_len)
        {
            b_reg = FALSE;
        }
        else
        {
            memcpy(u8_data,p_ee_var->p_u8_ram,u16_len);
            // ApiLogPrint(_LOG_DEBUG,"eeprom read id:%d\n",p_ee_var->u32_id);
            b_reg = TRUE;
        }
    }
    return b_reg;
}

/****************************************************************************/
/**
 * Function Name: Mem_EE_write
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/08, feifei.xu create this function
 ****************************************************************************/
BOOL Mem_EE_write(UINT32 u32_id,UINT8* u8_data,UINT16 u16_len)
{
    BOOL b_reg = FALSE;
    UINT16 u16_index = 0;
    EE_DATA_VAR_S* p_ee_var = &s_st_ee_var;

    if(!s_b_MemE2DataInitDone)
    {
        return;
    }
    u16_index = Mem_EE_SearchIndex(u32_id);

    if(TRUE == Mem_EE_SetEntry(u16_index))
    {
        if(u16_len > p_ee_var->u16_len)
        {
            b_reg = FALSE;
        }
        else
        {
            memcpy(p_ee_var->p_u8_ram,u8_data,u16_len);
            if(Mem_EED_Write(p_ee_var->u32_id,p_ee_var->p_u8_ram,p_ee_var->u16_len,p_ee_var->u16_offset))
            {
                ApiLogPrint(_LOG_DEBUG,"eeprom write id:%d\n",p_ee_var->u32_id);
                b_reg = TRUE;
            }
            else
            {
                b_reg = FALSE;
            }
        }
    }
    return b_reg;
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


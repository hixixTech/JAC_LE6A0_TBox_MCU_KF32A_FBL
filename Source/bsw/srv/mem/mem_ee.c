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
#include "ecu_misc.h"
#include "mem_ee.h"
#include "mem_ee_data.h"
#include "mem_ee_driver.h"
/*****************************************************************************
** #define
*****************************************************************************/


/*****************************************************************************
** typedef
*****************************************************************************/
typedef enum
{
    E2_DATA_INIT = 0,
    
}E2Data_InitState;

typedef enum
{
    E2_INIT = 0,
    E2_WRITE,
    E2_WAIT_WRITE,
    E2_END
}E2_WRITE_FSM_E;
/*****************************************************************************
** static variables
*****************************************************************************/
static UINT32 s_u32_ee_id;
static UINT16 s_u16_ee_len;
static UINT8* s_u8_p_ram;
static UINT16 s_u16_offset;
static UINT32 s_u32_init_data;

static BOOL s_b_MemE2DataInitDone = FALSE;
static UINT16 s_u16_ee_EntryMapIdx;
/*****************************************************************************
** global variable
*****************************************************************************/
MemEeprom_State mem_ee_state = EE_IDLE;

EE_DataInitCmd_S st_EEDataInitCmd = EE_DATA_INIT;

const BOOL *const p_MemE2DataInitDone = &s_b_MemE2DataInitDone;
/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/
static void Mem_EE_DataInitHandler(void);

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
    
    Mem_EED_Init();
    Mem_EED_RunPageNumInit(u32_e2_byte_num);
    Mem_EED_PageAddrInit();
    Mem_EED_PageHeadInit();
}
/****************************************************************************/
/**
 * Function Name: Mem_EE_Handler10ms
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
void Mem_EE_Handler10ms(void)
{
    Mem_EED_Handler();
    Mem_EE_DataInitHandler();
}
/****************************************************************************/
/**
 * Function Name: Mem_EE_Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/06/07, feifei.xu create this function
 ****************************************************************************/
static void Mem_EE_DataStsInit(void)
{
    mem_ee_state = EE_IDLE;
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
    if(u16_EntryIdx < u16_e2_data_num)
    {
        s_u32_ee_id = c_st_e2DataMap[u16_EntryIdx].u32_eeprom_id;
        s_u16_ee_len = c_st_e2DataMap[u16_EntryIdx].u16_eeprom_len;
        s_u8_p_ram = c_st_e2DataMap[u16_EntryIdx].u16_eeprom_offset + p_e2_ram;
        s_u16_offset = c_st_e2DataMap[u16_EntryIdx].u16_eeprom_offset;
        s_u32_init_data = c_st_e2DataMap[u16_EntryIdx].u32_init_data;
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
static void Mem_EE_DataInitHandler(void)
{

    if(s_b_MemE2DataInitDone)
    {
        return;
    }

    switch(st_EEDataInitCmd)
    {
        case EE_DATA_INIT:
        {
            if(TRUE == Mem_EED_GetIdleSts())
            {
                s_u16_ee_EntryMapIdx = 0;
                st_EEDataInitCmd = EE_DATA_SEARCH;
            }
        }
        break;

        case EE_DATA_SEARCH:
        {
            if (Mem_EE_SetEntry(s_u16_ee_EntryMapIdx++))
            {
                st_EEDataInitCmd = EE_DATA_READ;
            }
            else
            {
                st_EEDataInitCmd = EE_DATA_FINISH;
            }
        }
        break;

        case EE_DATA_READ:
        {
            if(TRUE == Mem_EED_Read(s_u32_ee_id,s_u16_ee_len,s_u16_offset))
            {
                st_EEDataInitCmd = EE_DATA_READ_WAIT;
            }
        }
        break;

        case EE_DATA_READ_WAIT:
        {
            if(TRUE == Mem_EED_GetIdleSts())
            {
                Mem_EED_GetReadData(s_u8_p_ram,s_u16_ee_len);
                if(TRUE == Ecu_Misc_CompareByte(s_u8_p_ram,0xFF,s_u16_ee_len))
                {
                    memset(s_u8_p_ram,(UINT8)s_u32_init_data,s_u16_ee_len);
                }
                st_EEDataInitCmd = EE_DATA_SEARCH;
            }
        }
        break;

        case EE_DATA_FINISH:
        {
            s_b_MemE2DataInitDone = TRUE;
            st_EEDataInitCmd = EE_DATA_INIT;
        }
        break;

        default:
        break;
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
BOOL Mem_EE_Read(UINT32 u32_id,UINT8* u8_data)
{
    BOOL b_reg = FALSE;
    UINT16 u16_index = 0;

    if(!s_b_MemE2DataInitDone)
    {
        return FALSE;
    }

    u16_index = Mem_EE_SearchIndex(u32_id);

    if(TRUE == Mem_EE_SetEntry(u16_index))
    {
        memcpy(u8_data,s_u8_p_ram,s_u16_ee_len);
        b_reg = TRUE;
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
BOOL Mem_EE_write(UINT32 u32_id,UINT8* u8_data)
{
    BOOL b_reg = FALSE;
    UINT16 u16_index = 0;

    static E2_WRITE_FSM_E s_e_write_fsm = E2_INIT;

    switch (s_e_write_fsm)
    {
        case E2_INIT:
        {
            if(s_b_MemE2DataInitDone && Mem_EED_GetIdleSts())
            {
                s_e_write_fsm = E2_WRITE;
            }
        }
        break;

        case E2_WRITE:
        {
            u16_index = Mem_EE_SearchIndex(u32_id);
            if(FALSE == Mem_EE_SetEntry(u16_index))
            {
                s_e_write_fsm = E2_END;
                break;
            }
            if(TRUE == Mem_EED_Write(s_u32_ee_id,u8_data,s_u16_ee_len,s_u16_offset))
            {
                s_e_write_fsm = E2_WAIT_WRITE;
            }
        }
        break;

        case E2_WAIT_WRITE:
        {
            if(TRUE == Mem_EED_GetIdleSts())
            {
                s_e_write_fsm = E2_END;
            }
        }
        break;

        case E2_END:
        {
            b_reg = TRUE;
            s_b_MemE2DataInitDone = FALSE;  
            s_e_write_fsm = E2_INIT;
            
        }
        
        default:
            break;
    }

    return b_reg;
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


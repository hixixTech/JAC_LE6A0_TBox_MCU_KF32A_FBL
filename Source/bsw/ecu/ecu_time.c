/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    ecu_time.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/07/08
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
#include "ecu_time.h"
/*****************************************************************************
** #define
*****************************************************************************/


/*****************************************************************************
** typedef
*****************************************************************************/

typedef enum
{
    TIME_IDX_0 = 0,
    TIME_IDX_1,
    TIME_IDX_2,
    TIME_IDX_3,
    TIME_IDX_4,
    TIME_IDX_MAX
}TIME_NUM_E;

typedef enum
{
    TIME_IDEL = 0,
    TIME_RUN
}TIME_STATE_E;

typedef struct
{
    GPTIM_SFRmap* GPTIMx;
    FunctionalState ImmediatelyUpdate;
    FunctionalState UpdataEnable;
    UINT32 u32_Mode;
    UINT32 u32_Count;
    UINT32 u32_Prescaler;
    UINT32 u32_CounterMode;
    UINT32 u32_Clk;
    InterruptIndex Peripheral;
    FunctionalState OverflowIntEnable;
}TIME_CFG_S;

typedef struct
{
    TIME_NUM_E tine_id;
    TIME_STATE_E Time_sts;
    UINT32 u32_Period;
    ECU_IOD_TIMER_CALLBACK fun_callback;
}TIME_NMW_S;




/*****************************************************************************
** global variable
*****************************************************************************/

/*****************************************************************************
** static variables
*****************************************************************************/
static TIME_NMW_S s_st_timenwm_var[TIME_IDX_MAX];

/*****************************************************************************
** static constants
*****************************************************************************/
static const TIME_CFG_S s_c_st_time0 = 
{
    T0_SFR,
    TRUE,
    TRUE,
    GPTIM_TIMER_MODE,
    0,
    48000,
    GPTIM_COUNT_UP_OF,
    GPTIM_HFCLK,
    INT_T0,
    TRUE
};

static const TIME_CFG_S s_c_st_time1 = 
{
    T1_SFR,
    TRUE,
    TRUE,
    GPTIM_TIMER_MODE,
    0,
    48000,
    GPTIM_COUNT_UP_OF,
    GPTIM_HFCLK,
    INT_T1,
    TRUE
};

static const TIME_CFG_S s_c_st_time2 = 
{
    T2_SFR,
    TRUE,
    TRUE,
    GPTIM_TIMER_MODE,
    0,
    48000,
    GPTIM_COUNT_UP_OF,
    GPTIM_HFCLK,
    INT_T2,
    TRUE
};

static const TIME_CFG_S s_c_st_time3 = 
{
    T3_SFR,
    TRUE,
    TRUE,
    GPTIM_TIMER_MODE,
    0,
    48000,
    GPTIM_COUNT_UP_OF,
    GPTIM_HFCLK,
    INT_T3,
    TRUE
};

static const TIME_CFG_S s_c_st_time4 = 
{
    T4_SFR,
    TRUE,
    TRUE,
    GPTIM_TIMER_MODE,
    0,
    48000,
    GPTIM_COUNT_UP_OF,
    GPTIM_HFCLK,
    INT_T4,
    TRUE
};
/*****************************************************************************
** static function prototypes
*****************************************************************************/


/*****************************************************************************
** function prototypes
*****************************************************************************/
static void Ecu_GeneralTime_Configure(TIME_CFG_S* p_time_cfg)
{
	TIM_Reset(p_time_cfg->GPTIMx);																//定时器外设复位，使能外设时钟
	GPTIM_Updata_Immediately_Config(p_time_cfg->GPTIMx,p_time_cfg->ImmediatelyUpdate);			//立即更新控制
	GPTIM_Updata_Enable(p_time_cfg->GPTIMx,p_time_cfg->UpdataEnable);							//配置更新使能
	GPTIM_Work_Mode_Config(p_time_cfg->GPTIMx,p_time_cfg->u32_Mode);							//定时模式选择
	GPTIM_Set_Counter(p_time_cfg->GPTIMx,p_time_cfg->u32_Count);								//定时器计数值
	
	GPTIM_Set_Prescaler(p_time_cfg->GPTIMx,p_time_cfg->u32_Prescaler);				    		//定时器预分频值
	GPTIM_Counter_Mode_Config(p_time_cfg->GPTIMx,p_time_cfg->u32_CounterMode);				    //向上-向下计数,上溢和下溢产生中断标志
	GPTIM_Clock_Config(p_time_cfg->GPTIMx,p_time_cfg->u32_Clk);									//选用SCLK时钟
	INT_Interrupt_Priority_Config(p_time_cfg->Peripheral,4,0);									//抢占优先级4,子优先级0
	GPTIM_Overflow_INT_Enable(p_time_cfg->GPTIMx,p_time_cfg->OverflowIntEnable);				//计数溢出中断使能
	INT_Interrupt_Enable(p_time_cfg->Peripheral,TRUE);											//外设中断使能
	INT_Clear_Interrupt_Flag(p_time_cfg->Peripheral);											//清中断标志
}
/****************************************************************************/
/**
 * Function Name: Ecu_GeneralTime_Start
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
static INT32 Ecu_GeneralTime_Start(const TIME_NMW_S* p_timenwm_var)
{
    if(p_timenwm_var->tine_id >= TIME_IDX_MAX)
    {
        return ERROR;
    }

    switch (p_timenwm_var->tine_id)
    {
        case TIME_IDX_0:
        {
            GPTIM_Set_Period(s_c_st_time0.GPTIMx, p_timenwm_var->u32_Period);
            GPTIM_Cmd(s_c_st_time0.GPTIMx,TRUE);
        }
        break;

        case TIME_IDX_1:
        {
            GPTIM_Set_Period(s_c_st_time1.GPTIMx, p_timenwm_var->u32_Period);
            GPTIM_Cmd(s_c_st_time1.GPTIMx,TRUE);
        }
        break;

        case TIME_IDX_2:
        {
            GPTIM_Set_Period(s_c_st_time2.GPTIMx, p_timenwm_var->u32_Period);
            GPTIM_Cmd(s_c_st_time2.GPTIMx,TRUE);
        }
        break;

        case TIME_IDX_3:
        {
            GPTIM_Set_Period(s_c_st_time3.GPTIMx, p_timenwm_var->u32_Period);
            GPTIM_Cmd(s_c_st_time3.GPTIMx,TRUE);
        }
        break;

        case TIME_IDX_4:
        {
            GPTIM_Set_Period(s_c_st_time4.GPTIMx, p_timenwm_var->u32_Period);
            GPTIM_Cmd(s_c_st_time4.GPTIMx,TRUE);
        }
        break;
        
        default:
        {
            return ERROR;
        }
        break;
    }

    return OK;
}
/****************************************************************************/
/**
 * Function Name: ApiGeneralTime0Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void ApiGeneralTime0Init(void)
{
    Ecu_GeneralTime_Configure(&s_c_st_time0);
}
/****************************************************************************/
/**
 * Function Name: ApiGeneralTime1Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void ApiGeneralTime1Init(void)
{
    Ecu_GeneralTime_Configure(&s_c_st_time1);
}
/****************************************************************************/
/**
 * Function Name: ApiGeneralTime2Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void ApiGeneralTime2Init(void)
{
    Ecu_GeneralTime_Configure(&s_c_st_time2);
}
/****************************************************************************/
/**
 * Function Name: ApiGeneralTime3Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void ApiGeneralTime3Init(void)
{
    Ecu_GeneralTime_Configure(&s_c_st_time3);
}
/****************************************************************************/
/**
 * Function Name: ApiGeneralTime4Init
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void ApiGeneralTime4Init(void)
{
    Ecu_GeneralTime_Configure(&s_c_st_time4);
}
/****************************************************************************/
/**
 * Function Name: ApiTimerRegistTimer
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
INT32 ApiTimerRegistTimer(uint32_t u32t_Prd,ECU_IOD_TIMER_CALLBACK fp_callback)
{
    UINT8 u8_i = 0;

    /* 入口参数判断 */
    if(((u32t_Prd % 5) != 0) || (fp_callback == NULL) ||(u32t_Prd == 0))
    {
        return ERROR;
    }

    for (u8_i = 0; u8_i < TIME_IDX_MAX; u8_i++)
    {
        if(TIME_IDEL ==s_st_timenwm_var[u8_i].Time_sts)
        {
            s_st_timenwm_var[u8_i].tine_id = u8_i;
            s_st_timenwm_var[u8_i].u32_Period = u32t_Prd;
            s_st_timenwm_var[u8_i].Time_sts = TIME_RUN;
            s_st_timenwm_var[u8_i].fun_callback = fp_callback;
            return Ecu_GeneralTime_Start(&s_st_timenwm_var[u8_i]);
        }
    }
    return ERROR;
}

/****************************************************************************/
/**
 * Function Name: _T0_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_T0_exception (void)
{
    GPTIM_Clear_Updata_INT_Flag(s_c_st_time0.GPTIMx);							 //清更新时间标志位
	GPTIM_Clear_Overflow_INT_Flag(s_c_st_time0.GPTIMx);						     //清T0溢出中断标志位
    s_st_timenwm_var[TIME_IDX_0].fun_callback();

    s_st_timenwm_var[TIME_IDX_0].tine_id = TIME_IDX_MAX;
    s_st_timenwm_var[TIME_IDX_0].Time_sts = TIME_IDEL;
    s_st_timenwm_var[TIME_IDX_0].u32_Period = 0;
    s_st_timenwm_var[TIME_IDX_0].fun_callback = NULL;
    GPTIM_Cmd(s_c_st_time0.GPTIMx,FALSE);
}
/****************************************************************************/
/**
 * Function Name: _T1_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_T1_exception (void)
{
    GPTIM_Clear_Updata_INT_Flag(s_c_st_time1.GPTIMx);							 //清更新时间标志位
	GPTIM_Clear_Overflow_INT_Flag(s_c_st_time1.GPTIMx);						     //清T1溢出中断标志位
    s_st_timenwm_var[TIME_IDX_1].fun_callback();

    s_st_timenwm_var[TIME_IDX_1].tine_id = TIME_IDX_MAX;
    s_st_timenwm_var[TIME_IDX_1].Time_sts = TIME_IDEL;
    s_st_timenwm_var[TIME_IDX_1].u32_Period = 0;
    s_st_timenwm_var[TIME_IDX_1].fun_callback = NULL;
    GPTIM_Cmd(s_c_st_time1.GPTIMx,FALSE);
}
/****************************************************************************/
/**
 * Function Name: _T2_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_T2_exception (void)
{
    GPTIM_Clear_Updata_INT_Flag(s_c_st_time2.GPTIMx);							 //清更新时间标志位
	GPTIM_Clear_Overflow_INT_Flag(s_c_st_time2.GPTIMx);						     //清T2溢出中断标志位
    s_st_timenwm_var[TIME_IDX_2].fun_callback();

    s_st_timenwm_var[TIME_IDX_2].tine_id = TIME_IDX_MAX;
    s_st_timenwm_var[TIME_IDX_2].Time_sts = TIME_IDEL;
    s_st_timenwm_var[TIME_IDX_2].u32_Period = 0;
    s_st_timenwm_var[TIME_IDX_2].fun_callback = NULL;
    GPTIM_Cmd(s_c_st_time2.GPTIMx,FALSE);
}
/****************************************************************************/
/**
 * Function Name: _T3_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_T3_exception (void)
{
    GPTIM_Clear_Updata_INT_Flag(s_c_st_time3.GPTIMx);							 //清更新时间标志位
	GPTIM_Clear_Overflow_INT_Flag(s_c_st_time3.GPTIMx);						     //清T3溢出中断标志位
    s_st_timenwm_var[TIME_IDX_3].fun_callback();

    s_st_timenwm_var[TIME_IDX_3].tine_id = TIME_IDX_MAX;
    s_st_timenwm_var[TIME_IDX_3].Time_sts = TIME_IDEL;
    s_st_timenwm_var[TIME_IDX_3].u32_Period = 0;
    s_st_timenwm_var[TIME_IDX_3].fun_callback = NULL;
    GPTIM_Cmd(s_c_st_time3.GPTIMx,FALSE);
}
/****************************************************************************/
/**
 * Function Name: _T4_exception
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  2021/07/08, feifei.xu create this function
 ****************************************************************************/
void __attribute__((interrupt))_T4_exception (void)
{
    GPTIM_Clear_Updata_INT_Flag(s_c_st_time4.GPTIMx);							 //清更新时间标志位
	GPTIM_Clear_Overflow_INT_Flag(s_c_st_time4.GPTIMx);						     //清T4溢出中断标志位
    s_st_timenwm_var[TIME_IDX_4].fun_callback();

    s_st_timenwm_var[TIME_IDX_4].tine_id = TIME_IDX_MAX;
    s_st_timenwm_var[TIME_IDX_4].Time_sts = TIME_IDEL;
    s_st_timenwm_var[TIME_IDX_4].u32_Period = 0;
    s_st_timenwm_var[TIME_IDX_4].fun_callback = NULL;
    GPTIM_Cmd(s_c_st_time4.GPTIMx,FALSE);
}
/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


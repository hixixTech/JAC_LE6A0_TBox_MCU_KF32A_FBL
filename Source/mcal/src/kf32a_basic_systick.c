/**
  ******************************************************************************
  * �ļ���  kf32a_basic_systick.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˽��Ķ�ʱ�����蹦�ܺ�����������
  *          + ���Ķ�ʱ��(SYSTICK)�����ʼ����������
  *          + ���Ķ�ʱ��(SYSTICK)�������ú�������
  *
  *********************************************************************
  */

#include "kf32a_basic_systick.h"

/*
 * ˵��   ���ļ�δ����STCALIBУ��Ĵ�����Ϣ��STCALIBУ��Ĵ����䶯����
 *        �ᵼ�½��Ķ�ʱ����ʱ���쳣��KungFu�������û��޸ĸüĴ�����
 */


/* ���Ķ�ʱ��˽�ж��� ----------------------------------------------------*/
/* ST_CTL�Ĵ����������� */
#define SYSTICK_CTL_INIT_MASK           (ST_CTL_TICKINTEN \
                                       | ST_CTL_STCLKS)

/**
  *   ##### ���Ķ�ʱ��(SYSTICK)�����ʼ���������� #####
  */
/**
  * ����  ���Ķ�ʱ��(SYSTICK)���á�
  * ����  Clock: ϵͳ���Ķ�ʱ��ʱ��Դѡ��
  *              ȡֵ��ΧΪ��
  *                SYSTICK_SYS_CLOCK_DIV_1: SCLK��Ϊʱ��
  *                SYSTICK_SYS_CLOCK_DIV_2:  SCLK/2��Ϊʱ��
  *       SysTickINT: SysTick�ж�ʹ�ܿ��ƣ�
  *                   ȡֵ��ΧΪ��TRUE �� FALSE��
  *       Reload: ϵͳ���Ķ�ʱ���ؼ���ֵ�����Ķ�ʱ��������ֵΪ�ؼ���ֵ��1��
  *               ȡֵΪ24λ���ݡ�
  * ����  ��
  */
void
SYSTICK_Configuration (uint32_t Clock, uint32_t SysTickINT,
                    uint32_t Reload)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_SYSTICK_CLOCK(Clock));
    CHECK_RESTRICTION(CHECK_SYSTICK_RELOAD(Reload));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(SysTickINT));

    /*------------ ��ֹ���Ķ�ʱ������ֹ�����ж� -------------*/
    SFR_CLR_BIT_ASM(ST_CTL, ST_CTL_STEN_POS);

    /*--------------- ����ST_RELOAD�Ĵ��� ---------------*/
    ST_RELOAD = Reload;

    /*-- ����ST_CV�Ĵ����������㵱ǰֵ�Ĵ���������COUNTZERO��־λ��
         ȷ������ֵ�ܼ��ص���ǰֵ�Ĵ����� -------------------*/
    ST_CV = 0;

    /*------------------ ����ST_CTL�Ĵ��� -----------------*/
    /* ���ݲ���Clock������STCLKSλ�� */
    /* ���ݲ���SysTickINT������TICKINTENλ�� */
    tmpreg = Clock | (SysTickINT << ST_CTL_TICKINTEN_POS);
    ST_CTL = SFR_Config (ST_CTL,
                          ~SYSTICK_CTL_INIT_MASK,
                          tmpreg);
}

/**
  *   ##### ���Ķ�ʱ��(SYSTICK)�����ʼ������������� #####
  */


/**
  *   ##### ���Ķ�ʱ��(SYSTICK)�������ú������� #####
  */
/**
  * ����  ���Ķ�ʱ��(SYSTICK)��������ʹ�ܡ�
  * ����  NewState: ���Ķ�ʱ��ʹ�ܿ��ƣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  ��
  */
void
SYSTICK_Cmd (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����ST_CTL�Ĵ���STENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��ϵͳ���Ķ�ʱ�� */
        SFR_SET_BIT_ASM(ST_CTL, ST_CTL_STEN_POS);
    }
    else
    {
        /* ��ֹϵͳ���Ķ�ʱ�� */
        SFR_CLR_BIT_ASM(ST_CTL, ST_CTL_STEN_POS);
    }
}

/**
  * ����  ���Ķ�ʱ��(SYSTICK)ʱ��Դ���á�
  * ����  SysClock: ϵͳ���Ķ�ʱ��ʱ��Դѡ��
  *                 ȡֵ��ΧΪ��
  *                   SYSTICK_SYS_CLOCK_DIV_1: SCLK��Ϊʱ��
  *                   SYSTICK_SYS_CLOCK_DIV_2:  SCLK/2��Ϊʱ��
  * ����  ��
  */
void
SYSTICK_Clock_Config (uint32_t SysClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_SYSTICK_CLOCK(SysClock));

    /*---------------- ����ST_CTL�Ĵ���STCLKSλ ----------------*/
    if (SysClock != SYSTICK_SYS_CLOCK_DIV_2)
    {
        /*  ѡ��SCLK��Ϊʱ�� */
        SFR_SET_BIT_ASM(ST_CTL, ST_CTL_STCLKS_POS);
    }
    else
    {
        /*  ѡ��SCLK/2��Ϊʱ�� */
        SFR_CLR_BIT_ASM(ST_CTL, ST_CTL_STCLKS_POS);
    }
}

/**
  * ����  ���Ķ�ʱ��(SYSTICK)SysTick�ж�ʹ�����á�
  * ����  SysClock: ϵͳ���Ķ�ʱ��SysTick�ж�ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  ��
  */
void
SYSTICK_Systick_INT_Enable (uint32_t SysClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(SysClock));

    /*---------------- ����ST_CTL�Ĵ���TICKINTENλ ----------------*/
    if (SysClock != FALSE)
    {
        /*  ������0����SysTick�ж����� */
        SFR_SET_BIT_ASM(ST_CTL, ST_CTL_TICKINTEN_POS);
    }
    else
    {
        /*  ������0������SysTick�ж����� */
        SFR_CLR_BIT_ASM(ST_CTL, ST_CTL_TICKINTEN_POS);
    }
}

/**
  * ����  ��ȡ���Ķ�ʱ���������־��
  * ����  �ޡ�
  * ����  ���Ķ�ʱ����������ı�־��0������δ���㣬1���������㡣
  */
FlagStatus
SYSTICK_Get_Count_Zero_Flag (void)
{
    /*-------------------- ����ST_CTL�Ĵ���COUNTZEROλ --------------------*/
    if (ST_CTL & ST_CTL_COUNTZERO)
    {
        /* �������� */
        return SET;
    }
    else
    {
        /* ����δ���� */
        return RESET;
    }
}

/**
  * ����  ���Ķ�ʱ��(SYSTICK)�ؼ��ؼĴ������á�
  * ����  Reload: ϵͳ���Ķ�ʱ���ؼ���ֵ�����Ķ�ʱ��������ֵΪ�ؼ���ֵ��1��
  *               ȡֵΪ24λ���ݡ�
  * ����  ��
  */
void
SYSTICK_Reload_Config (uint32_t Reload)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_SYSTICK_RELOAD(Reload));

    /*--------------- ����ST_RELOAD�Ĵ��� ---------------*/
    ST_RELOAD = Reload;
}

/**
  * ����  ���Ķ�ʱ��(SYSTICK)���������ã��ú���������COUNTZEROλ��
  *       ����STRELOADֵ�ܼ��ص��������Ĵ����С�
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
SYSTICK_Counter_Updata (void)
{
    /*-- ��ST_CV�Ĵ���д����ֵ�������㵱ǰֵ�Ĵ�����
         ������COUNTZERO��־λ��ȷ������ֵ�ܼ��ص���ǰֵ�Ĵ����� --*/
    ST_CV = 0;
}

/**
  * ����  ��ȡϵͳ���Ķ�ʱ���ؼ��ؼĴ�����
  * ����  �ޡ�
  * ����  �ؼ��ؼĴ�����ֵ��24λ��Ч���ݡ�
  */
uint32_t
SYSTICK_Get_Reload (void)
{
    uint32_t tmpreg = 0;

    /* ��ȡST_RELOAD�Ĵ��� */
    tmpreg = ST_RELOAD & ST_RELOAD_STRELOAD;

    return tmpreg;
}

/**
  * ����  ��ȡϵͳ���Ķ�ʱ����������
  * ����  �ޡ�
  * ����  ��ǰ����ֵ��24λ��Ч���ݡ�
  */
uint32_t
SYSTICK_Get_Counter (void)
{
    uint32_t tmpreg = 0;

    /* ��ȡST_CV�Ĵ��� */
    tmpreg = ST_CV & ST_CV_STCV;

    return tmpreg;
}
/**
  *   ##### ���Ķ�ʱ��(SYSTICK)�������ú���������� #####
  */

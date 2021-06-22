/**
  ******************************************************************************
  * �ļ���  kf32a_basic_wwdt.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˴��ڿ��Ź�(WWDT)��صĹ��ܺ�����������
  *          + ���ڿ��Ź�(WWDT)���ƹ��ܺ���
  *          + ���ڿ��Ź�(WWDT)�жϹ�����
  *
  *********************************************************************
  */

#include "kf32a_basic_wwdt.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/**
  *   ##### ���ڿ��Ź�(WWDT)���ƹ��ܺ��� #####
  */
/**
  * ����  ��λWWDTģ�飬ʹ������ʱ�ӡ�
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
WWDT_Reset (void)
{
    /* ��λWWDTģ�� */
    RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_WWDTRST, TRUE);
    RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_WWDTRST, FALSE);

    /* ʹ��WWDTʱ�� */
    PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_WWDTCLKEN, TRUE);
}

/**
  * ����  ���ô��ڿ��Ź��ɲ���������ֵ��
  * ����  Threshold: ���ڿ��Ź��ɲ���������ֵ��ȡֵΪ0~0x3F��
  * ����  �ޡ�
  */
void
WWDT_Threshold_Config (uint32_t Threshold)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_WWDT_THRESHOLD(Threshold));

    /*---------------- ����WWDT_CTL�Ĵ���WWλ ----------------*/
    tmpreg = Threshold << WWDT_CTL_WW0_POS;
    WWDT_CTL = SFR_Config (WWDT_CTL, ~WWDT_CTL_WW, tmpreg);
}

/**
  * ����  ���ô��ڿ��Ź�Ԥ��Ƶ����INTLF���з�Ƶ��
  * ����  Prescaler: ���ڿ��Ź���INTLF��Ԥ��Ƶֵ��ȡֵΪ��
  *                    WWDT_PRESCALER_1: ����Ƶ
  *                    WWDT_PRESCALER_2: 2��Ƶ
  *                    WWDT_PRESCALER_4: 4��Ƶ
  *                    WWDT_PRESCALER_8: 8��Ƶ
  *                    WWDT_PRESCALER_16: 16��Ƶ
  *                    WWDT_PRESCALER_32: 32��Ƶ
  *                    WWDT_PRESCALER_64: 64��Ƶ
  *                    WWDT_PRESCALER_128: 128��Ƶ
  *                    WWDT_PRESCALER_256: 256��Ƶ
  *                    WWDT_PRESCALER_512: 512��Ƶ
  *                    WWDT_PRESCALER_1024: 1024��Ƶ
  *                    WWDT_PRESCALER_2048: 2048��Ƶ
  *                    WWDT_PRESCALER_4096: 4096��Ƶ
  *                    WWDT_PRESCALER_8192: 8192��Ƶ
  *                    WWDT_PRESCALER_16384: 16384��Ƶ
  *                    WWDT_PRESCALER_32768: 32768��Ƶ
  *                    WWDT_PRESCALER_65536: 65536��Ƶ
  *                    WWDT_PRESCALER_131072: 131072��Ƶ
  *                    WWDT_PRESCALER_262144: 262144��Ƶ
  * ����  �ޡ�
  */
void
WWDT_Prescaler_Config (uint32_t Prescaler)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_WWDT_PRESCALER(Prescaler));

    /*---------------- ����WWDT_CTL�Ĵ���WPSλ ----------------*/
    WWDT_CTL = SFR_Config (WWDT_CTL, ~WWDT_CTL_WPS, Prescaler);
}

/**
  * ����  ���ô��ڿ��Ź�ʹ�ܡ�
  * ����  NewState: ���ڿ��Ź�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
WWDT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����WWDT_CTL�Ĵ���WWDTENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܴ��ڿ��Ź���ʱ�� */
        SFR_SET_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTEN_POS);
    }
    else
    {
        /* δʹ�ܴ��ڿ��Ź� */
        SFR_CLR_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTEN_POS);
    }
}

/**
  * ����  ���ô��ڿ��Ź�����ֵ��
  * ����  Counter: ���ڿ��Ź�����ֵ��ȡֵΪ7λ��Ч��ֵ��
  * ����  �ޡ�
  */
void
WWDT_Counter_Config (uint32_t Counter)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_WWDT_THRESHOLD(Counter));

    /*---------------- ����WWDT_CNT�Ĵ���WTλ ----------------*/
    WWDT_CNT = Counter;
}

/**
  * ����  ��ȡ���ڿ��Ź�����ֵ��
  * ����  �ޡ�
  * ����  ���ڿ��Ź�����ֵ��7λ��Ч��ֵ��
  */
uint32_t
WWDT_Get_Counter (void)
{
    uint32_t tmpreg = 0;

    /*---------------- ��ȡWWDT_CNT�Ĵ���WTλ ----------------*/
    tmpreg = WWDT_CNT & WWDT_CNT_WT;
    tmpreg >>= WWDT_CNT_WT_POS;

    return tmpreg;
}
/**
  *   ##### ���ڿ��Ź�(WWDT)���ƹ��ܺ���������� #####
  */


/**
  *   ##### ���ڿ��Ź�(WWDT)�жϹ����� #####
  */
/**
  * ����  ���ô��ڿ��Ź��ж�ʹ�ܡ�
  * ����  NewState: ���ڿ��Ź��ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
WWDT_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����WWDT_CTL�Ĵ���WWDTIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܴ��ڿ��Ź��ж� */
        SFR_SET_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTIE_POS);
    }
    else
    {
        /* ��ֹ���ڿ��Ź��ж� */
        SFR_CLR_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTIE_POS);
    }
}

/**
  * ����  ��ȡ���ڿ��Ź��жϱ�־��
  * ����  �ޡ�
  * ����  1:�����˴��ڿ��Ź��жϣ�0:δ�������ڿ��Ź��жϡ�
  */
FlagStatus
WWDT_Get_INT_Flag (void)
{
    /*---------------- ��ȡWWDT_CTL�Ĵ���WWDTIFλ ----------------*/
    if (WWDT_CTL & WWDT_CTL_WWDTIF)
    {
        /* �����˴��ڿ��Ź��ж� */
        return SET;
    }
    else
    {
        /* δ�������ڿ��Ź��ж� */
        return RESET;
    }
}

/**
  * ����  ���㴰�ڿ��Ź��жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
WWDT_Clear_INT_Flag (void)
{
    /*---------------- ����WWDT_CTL�Ĵ���WWDTICλ ----------------*/
    SFR_SET_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTIC_POS);
    while(WWDT_CTL & WWDT_CTL_WWDTIF);
    SFR_CLR_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTIC_POS);
}
/**
  *   ##### ���ڿ��Ź�(WWDT)�жϹ������������ #####
  */

/**
  ******************************************************************************
  * �ļ���  kf32a_basic_bkp.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˱�����(BKP)���蹦�ܺ�����������
  *          + ������(BKP)��λ����д������
  *          + ������(BKP)�������ú���
  *          + ������(BKP)�жϹ�����
  *
  *********************************************************************
  */

#include "kf32a_basic_bkp.h"


/**
  *   ##### ������(BKP)��λ����д������ #####
  */
/**
  * ����  ��λ������(BKP)���裬�ú�������λ������Ĵ����飬������λRTCģ�顣
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
BKP_Reset (void)
{
    /* ������Ĵ����������λ */
    SFR_SET_BIT_ASM(BKP_CTL, BKP_CTL_BKDRST_POS);
    SFR_CLR_BIT_ASM(BKP_CTL, BKP_CTL_BKDRST_POS);
}

/**
  * ����  ������(BKP)�Ĵ������дʱ����Ҫ�����¼Ĵ���λ�������ã�
  *         1.  OSCCTL0�Ĵ�����PMWRENλ��1����������������Ķ�д������
  *         2.  PM_CTL0�Ĵ�����BKPREGCLRλ��1���Ĵ������˳���λ��
  *         3.  PM_CTL0�Ĵ�����BKPWRλ��1�������������ݼĴ������RTCģ���ڵļĴ�����д������
  *       ��д�Ĵ�����ǰ���øú�����������д������ٴε��øú�������ȱ�ٻ��ε�������޷��ָ�ԭʼ���á�
  * ����  NewState: ������Ĵ������д����ʹ�ܣ�ȡֵ��ΧΪ��
  *                   TRUE: ������Ĵ������дǰ������
  *                   FALSE: ������Ĵ������д������ûָ�
  * ����  �ޡ�
  */
void
BKP_Write_And_Read_Enable (FunctionalState NewState)
{
    uint32_t tmpreg = 0;
    static uint32_t pmwrenbkp = TRUE;
    static uint32_t bkpregclrbkp = TRUE;
    static uint32_t bkpwrbkp = TRUE;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ׼����д���������� */
        /* ����PMWRENλ */
        tmpreg = OSC_CTL0;
        if (OSC_CTL0_PMWREN & tmpreg)
        {
            pmwrenbkp = TRUE;
        }
        else
        {
            pmwrenbkp = FALSE;
            /* ��������������Ķ�д���� */
            SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
        }

        /* ����BKPREGCLRλ */
        tmpreg = PM_CTL0;
        if (PM_CTL0_BKPREGCLR & tmpreg)
        {
            bkpregclrbkp = TRUE;
        }
        else
        {
            bkpregclrbkp = FALSE;
            /* �Ĵ������˳���λ */
            SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
        }

        /* ����BKPWRλ */
        tmpreg = PM_CTL0;
        if (PM_CTL0_BKPWR & tmpreg)
        {
            bkpwrbkp = TRUE;
        }
        else
        {
            bkpwrbkp = FALSE;
            /* �����������ݼĴ������RTCģ���ڵļĴ�����д���� */
            SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);
        }
    }
    else
    {
        /* ������д���������� */
        /* �ָ�PMWRENλ */
        if (pmwrenbkp != FALSE)
        {
            pmwrenbkp = TRUE;
        }
        else
        {
            pmwrenbkp = FALSE;
            /* ��ֹ����������Ķ�д���� */
            SFR_CLR_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
        }

        /* �ָ�BKPREGCLRλ */
        if (bkpregclrbkp != FALSE)
        {
            bkpregclrbkp = TRUE;
        }
        else
        {
            bkpregclrbkp = FALSE;
            /* ��ֹ����������Ķ�д���� */
            SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
        }

        /* �ָ�BKPWRλ */
        if (bkpwrbkp != FALSE)
        {
            bkpwrbkp = TRUE;
        }
        else
        {
            bkpwrbkp = FALSE;
            /* ��ֹ����������Ķ�д���� */
            SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);
        }
    }
}
/**
  *   ##### ������(BKP)��λ����д������������� #####
  */


/**
  *   ##### ������(BKP)�������ú��� #####
  */
/**
  * ����  ���ñ�����Ĵ����������λʹ�ܡ�
  * ����  NewState: ������Ĵ����������λ��ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  ��
  */
void
BKP_Reset_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����BKP_CTL�Ĵ���BKDRSTλ ----------------*/
    if (NewState != FALSE)
    {
        /* ������Ĵ����鸴λ */
        SFR_SET_BIT_ASM(BKP_CTL, BKP_CTL_BKDRST_POS);
    }
    else
    {
        /* ������ */
        SFR_CLR_BIT_ASM(BKP_CTL, BKP_CTL_BKDRST_POS);
    }
}

/**
  * ����  �������������ŵ���Ч��ƽ��
  * ����  PinSel: ����������ѡ��ȡֵΪ��
  *                 BKP_PIN_RTC_TAMP1: ����������RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: ����������RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: ����������RTC_TAMP3
  *       Effective: ��Ч��ƽ��ȡֵΪ:
  *                    BKP_HIGH_LEVEL_EFFECTIVE: �ߵ�ƽ��������е����ݱ��ݼĴ���
  *                    BKP_LOW_LEVEL_EFFECTIVE: �͵�ƽ��������е����ݱ��ݼĴ���
  * ����  �ޡ�
  */
void
BKP_Pin_Effective_Level_Config (uint32_t PinSel, uint32_t Effective)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));
    CHECK_RESTRICTION(CHECK_BKP_EFFECTIVE_LEVEL(Effective));

    /*------------- ����BKP_CTL�Ĵ���TAMPxLVLλ -------------*/
    tmpreg = BKP_CTL_TAMP1LVL << PinSel;
    if (BKP_HIGH_LEVEL_EFFECTIVE != Effective)
    {
        /* �����������ϵĵ͵�ƽ��������е����ݱ��ݼĴ��� */
        BKP_CTL |= tmpreg;
    }
    else
    {
        /* �����������ϵĸߵ�ƽ��������е����ݱ��ݼĴ��� */
        BKP_CTL &= ~tmpreg;
    }
}

/**
  * ����  ��������������RTC_TAMP1ʹ�ܡ�
  * ����  PinSel: ����������ѡ��ȡֵΪ��
  *                 BKP_PIN_RTC_TAMP1: ����������RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: ����������RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: ����������RTC_TAMP3
  *       NewState: ����������RTC_TAMP1ʹ��״̬��ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
BKP_Pin_Enable (uint32_t PinSel, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------- ����BKP_CTL�Ĵ���TAMPxLVLλ -------------*/
    tmpreg = BKP_CTL_TAMP1EN << PinSel;
    if (NewState != FALSE)
    {
        /* ʹ����������1���� */
        BKP_CTL |= tmpreg;
    }
    else
    {
        /* ��ֹ��������1���� */
        BKP_CTL &= ~tmpreg;
    }
}

/**
  * ����  ����RTCʱ��Դ��
  * ����  Source: RTCʱ��Դѡ��ȡֵΪ��
                    BKP_RTC_NO_CLK: ��ʱ��
                    BKP_RTC_EXTLF: EXTLF��ΪRTCʱ��
                    BKP_RTC_INTLF: INTLF��ΪRTCʱ��
                    BKP_RTC_EXTHF_DIV_128: EXTHF����128��Ƶ����ΪRTCʱ��
  * ����  �ޡ�
  */
void
BKP_RTC_Clock_Config (uint32_t Source)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_RTC_CLK(Source));

    /*---------------- ����BKP_CTL�Ĵ���RTCCLKSλ ----------------*/
    BKP_CTL = SFR_Config (BKP_CTL, ~BKP_CTL_RTCCLKS, Source);
}

/**
  * ����  �����ⲿʱ����·����ʹ�ܡ�
  * ����  Source: ʱ��ѡ��ȡֵΪ��
  *                 BKP_EXTHF: �ⲿ��Ƶʱ��
  *                 BKP_EXTLF: �ⲿ��Ƶʱ��
  *       NewState: �ⲿʱ����·����ʹ��״̬��ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
BKP_External_Clock_Bypass_Enable (uint32_t Source, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_CLOCK_SOURCE(Source));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------- ����BKP_CTL�Ĵ���TAMPxLVLλ -------------*/
    tmpreg = BKP_CTL_EXTHFBYP << Source;
    if (FALSE != NewState)
    {
        /* �ⲿʱ�ӱ���· */
        BKP_CTL |= tmpreg;
    }
    else
    {
        /* �ⲿʱ��δ��· */
        BKP_CTL &= ~tmpreg;
    }
}

/**
  * ����  д�����򱸷����ݼĴ�����
  * ����  OrderNumber: �������ݼĴ�����ţ�ȡֵ��ΧΪ0~31��
  *       WriteData: д������ݣ�ȡֵ��ΧΪ32λ���ݡ�
  * ����  �ޡ�
  */
void
BKP_Data_Config (uint32_t OrderNumber, uint32_t WriteData)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_DATA_NUMBER(OrderNumber));

    /*------------- ����BKP_DATA�Ĵ���BKPDATAλ -------------*/
    BKP_SFR->DATA[OrderNumber] = WriteData;
}

/**
  * ����  �������򱸷����ݼĴ�����
  * ����  OrderNumber: �������ݼĴ�����ţ�ȡֵ��ΧΪ0~31��
  * ����  �������ݼĴ�����ֵ��ȡֵ��ΧΪ32λ���ݡ�
  */
uint32_t
BKP_Get_Data (uint32_t OrderNumber)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_DATA_NUMBER(OrderNumber));

    /*------------- ��ȡBKP_DATA�Ĵ���BKPDATAλ -------------*/
    tmpreg = BKP_SFR->DATA[OrderNumber];

    return tmpreg;
}
/**
  *   ##### ������(BKP)�������ú���������� #####
  */


/**
  *   ##### ������(BKP)�жϹ����� #####
  */
/**
  * ����  �������������ж�ʹ�ܡ�
  * ����  PinSel: ����������ѡ��ȡֵΪ��
  *                 BKP_PIN_RTC_TAMP1: ����������RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: ����������RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: ����������RTC_TAMP3
  *       NewState: ���������ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
BKP_Pin_TAMP_INT_Enable (uint32_t PinSel, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����BKP_INT�Ĵ���TAMPxIEλ ----------------*/
    tmpreg = BKP_INT_TAMP1IE << PinSel;
    if (NewState != FALSE)
    {
        /* ʹ���������ж� */
        BKP_INT |= tmpreg;
    }
    else
    {
        /* ��ֹ�������ж� */
        BKP_INT &= ~tmpreg;
    }
}

/**
  * ����  ��ȡ�����жϱ�־��
  * ����  PinSel: ����������ѡ��ȡֵΪ��
  *                 BKP_PIN_RTC_TAMP1: ����������RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: ����������RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: ����������RTC_TAMP3
  * ����  1:���������¼���0:�������¼�������
  */
FlagStatus
BKP_Get_Pin_TAMP_INT_Flag (uint32_t PinSel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));

    /*---------------- ��ȡBKP_INT�Ĵ���TAMPxIFλ ----------------*/
    tmpreg = BKP_INT_TAMP1IF << PinSel;
    if (BKP_INT & tmpreg)
    {
        /* ���������¼� */
        return SET;
    }
    else
    {
        /* �������¼����� */
        return RESET;
    }
}

/**
  * ����  ����������жϱ�־��
  * ����  PinSel: ����������ѡ��ȡֵΪ��
  *                 BKP_PIN_RTC_TAMP1: ����������RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: ����������RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: ����������RTC_TAMP3
  * ����  �ޡ�
  */
void
BKP_Clear_Pin_TAMP_INT_Flag (uint32_t PinSel)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));

    /*---------------- ����BKP_INT�Ĵ���TAMPxICλ ----------------*/
    tmpreg = BKP_INT_TAMP1IC << PinSel;
    tmpreg1 = BKP_INT_TAMP1IF << PinSel;
    BKP_INT |= tmpreg;
    while((BKP_INT & tmpreg1)>>(BKP_INT_TAMP1IF_POS+PinSel));
    BKP_INT &= ~tmpreg;
}
/**
  *   ##### ������(BKP)�жϹ����� #####
  */

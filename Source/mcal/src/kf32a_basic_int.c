/**
  ******************************************************************************
  * �ļ���  kf32a_basic_int.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ���ж���صĹ��ܺ�����������
  *          + �ж�(INT)�������ú�������
  *          + �ⲿ�ж�(INT)���ܳ�ʼ����������
  *          + �ⲿ�ж�(INT)�������ú�������
  *
  *********************************************************************
  */

#include "kf32a_basic_int.h"

/* �ж϶�ʱ��˽�ж��� ----------------------------------------------------*/
/* INT_IP�Ĵ����������� */
#define INT_IP_INIT_MASK                ((uint32_t)0xF)

/* INT_EINTSSx�Ĵ����������� */
#define INT_EINTSS_INIT_MASK            ((uint32_t)0x7)


/**
  *   ##### �ж�(INT)�������ú������� #####
  */
/**
  * ����  ��ȡ��ǰ���ڴ�����ж�������š�
  * ����  �ޡ�
  * ����  ��ǰ�ж�������ţ�7λ��Ч���ݡ�
  */
uint8_t
INT_Get_Interrupt_Action (void)
{
    uint32_t tmpreg = 0;

    /* ��ȡINT_CTL0�Ĵ���INTACTλ�� */
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_INTACT;
    tmpreg >>= INT_CTL0_INTACT0_POS;

    return (uint8_t)tmpreg;
}

/**
  * ����  ��ȡ��ǰ����Ӧ��������ȼ��ж�������š�
  * ����  �ޡ�
  * ����  ����Ӧ��������ȼ��ж�������ţ�7λ��Ч���ݡ�
  */
uint8_t
INT_Get_Priority_Pending_Action (void)
{
    uint32_t tmpreg = 0;

    /* ��ȡINT_CTL0�Ĵ���INTPENDλ�� */
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_INTPEND;
    tmpreg >>= INT_CTL0_INTPEND0_POS;

    return (uint8_t)tmpreg;
}

/**
  * ����  �������ȼ���Ӧ������
  * ����  PriBase: ���ȼ���Ӧ������ȡֵ4λ���ݡ�
  * ����  �ޡ�
  */
void
INT_Priority_Base (uint8_t PriBase)
{
    uint32_t tmpreg = 0;

    CHECK_RESTRICTION(CHECK_INT_PRIORITY_BASE(PriBase));

    /*------------------ ����INT_CTL0�Ĵ��� -----------------*/
    /* ���ݲ���PriBase������PRIBASEλ�� */
    tmpreg = (uint32_t)PriBase << INT_CTL0_PRIBASE0_POS;
    INT_CTL0 = SFR_Config (INT_CTL0,
                          ~INT_CTL0_PRIBASE,
                          tmpreg);
}

/**
  * ����  ��ȡ���ȼ���Ӧ������
  * ����  �ޡ�
  * ����  ���ȼ���Ӧ������4λ��Ч���ݡ�
  */
uint8_t
INT_Get_Priority_Base (void)
{
    uint32_t tmpreg = 0;

    /* ��ȡINT_CTL0�Ĵ���PRIBASEλ�� */
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_PRIBASE;
    tmpreg >>= INT_CTL0_PRIBASE0_POS;

    return (uint8_t)tmpreg;
}

/**
  * ����  �����ж��Զ���ջ����ѡ��
  * ����  StackAlign: �ж��Զ���ջ����ѡ��
  *                   ȡֵΪ��
  *                     INT_STACK_DOUBLE_ALIGN: �ж��Զ���ջʹ��˫�ֶ���
  *                     INT_STACK_SINGLE_ALIGN: �ж��Զ���ջʹ�õ��ֶ���
  * ����  �ޡ�
  */
void
INT_Stack_Align_Config (uint32_t StackAlign)
{
    CHECK_RESTRICTION(CHECK_INT_STACK_ALIGN(StackAlign));

    /*---------------- ����INT_CTL0�Ĵ���DSALIGNλ ----------------*/
    if (StackAlign != INT_STACK_DOUBLE_ALIGN)
    {
        /* �ж��Զ���ջʹ�õ��ֶ��� */
        SFR_SET_BIT_ASM(INT_CTL0, INT_CTL0_DSALIGN_POS);
    }
    else
    {
        /* �ж��Զ���ջʹ��˫�ֶ��� */
        SFR_CLR_BIT_ASM(INT_CTL0, INT_CTL0_DSALIGN_POS);
    }
}

/**
  * ����  ����Ӳ�������ж�����λ��
  * ����  NewState: �ж��Զ���ջ����ѡ��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
INT_Fault_Masking_Config (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����INT_CTL0�Ĵ���FAULTMASKλ ----------------*/
    if (NewState != FALSE)
    {
        /* FAULT�쳣�����ȼ��������жϱ����� */
        SFR_SET_BIT_ASM(INT_CTL0, INT_CTL0_FAULTMASK_POS);
    }
    else
    {
        /* FAULT�쳣δ���� */
        SFR_CLR_BIT_ASM(INT_CTL0, INT_CTL0_FAULTMASK_POS);
    }
}

/**
  * ����  �ж��Ƿ�һ��������жϽ�����һ��ʱ����״̬��
  * ����  �ޡ�
  * ����  1:����״̬��0:������״̬��
  */
FlagStatus
INT_Get_Pre_Empty (void)
{
    uint32_t tmpreg = 0;

    /*---------------- ����INT_CTL0�Ĵ���INTPREEMPTλ ----------------*/
    tmpreg = INT_CTL0;
    if (tmpreg & INT_CTL0_INTPREEMPT)
    {
        /* ������жϽ�����һ��ʱ����״̬ */
        return SET;
    }
    else
    {
        /* �жϲ�����״̬ */
        return RESET;
    }
}

/**
  * ����  �жϵ�ǰ�Ƿ��г�NMI֮����жϹ���
  * ����  �ޡ�
  * ����  1:�г�NMI֮����жϹ���0:û�г�NMI֮����жϹ���
  */
FlagStatus
INT_Get_Pending_Flag (void)
{
    uint32_t tmpreg = 0;

    /*---------------- ����INT_CTL0�Ĵ���INTPENDINGλ ----------------*/
    tmpreg = INT_CTL0;
    if (tmpreg & INT_CTL0_INTPENDING)
    {
        /* �г�NMI֮����жϹ��� */
        return SET;
    }
    else
    {
        /* û�г�NMI֮����жϹ��� */
        return RESET;
    }
}

/**
  * ����  �������ȼ����顣
  * ����  PriorityGroup: �ж��Զ���ջ����ѡ��
  *                      ȡֵ��ΧΪ��
  *                        INT_PRIORITY_GROUP_3VS1: ��ռ���ȼ�Ϊ3λ�������ȼ�Ϊ1λ
  *                        INT_PRIORITY_GROUP_2VS2: ��ռ���ȼ�Ϊ2λ�������ȼ�Ϊ2λ
  *                        INT_PRIORITY_GROUP_1VS3: ��ռ���ȼ�Ϊ1λ�������ȼ�Ϊ3λ
  *                        INT_PRIORITY_GROUP_0VS4: ��ռ���ȼ�Ϊ0λ�������ȼ�Ϊ4λ
  * ����  �ޡ�
  */
void
INT_Priority_Group_Config (uint32_t PriorityGroup)
{
    CHECK_RESTRICTION(CHECK_INT_PRIORITY_GROUP(PriorityGroup));

    /*------------------ ����INT_CTL0�Ĵ��� -----------------*/
    /* ���ݲ���PriorityGroup������PRIGROUPλ�� */
    INT_CTL0 = SFR_Config (INT_CTL0,
                          ~INT_CTL0_PRIGROUP,
                          PriorityGroup);
}

/**
  * ����  ��ȡ���ȼ����顣
  * ����  �ޡ�
  * ����  ���ȼ�����λ�����ã�32λ��Ч���ݡ�
  */
uint32_t
INT_Get_Priority_Group (void)
{
    uint32_t tmpreg = 0;

    /*------------------ ��ȡINT_CTL0�Ĵ��� -----------------*/
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_PRIGROUP;

    return tmpreg;
}

/**
  * ����  ȫ�ֿ������ж�ʹ��λ�����ж�ʹ�ܿ��Ʋ�������λ/NMI/Ӳ�������жϡ�
  * ����  NewState: ȫ�ֿ������ж�ʹ�ܣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
INT_All_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����INT_CTL0�Ĵ���AIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ȫ�ֿ������ж�ʹ�� */
        SFR_SET_BIT_ASM(INT_CTL0, INT_CTL0_AIE_POS);
    }
    else
    {
        /* ȫ�ֿ������жϽ�ֹ */
        SFR_CLR_BIT_ASM(INT_CTL0, INT_CTL0_AIE_POS);
    }
}

/**
  * ����  ������ں��ж�ʹ�ܿ��ƣ������û�δ����ı������ж�������
  *       ���ܳ���δ֪�����
  * ����  Peripheral: ������ں��ж�������ţ�ȡֵ��ΧΪ��
  *                   ö������InterruptIndex�е������ж�������š�
  *       NewState: ������ں��ж�ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
INT_Interrupt_Enable (InterruptIndex Peripheral, FunctionalState NewState)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_INTERRUPT_INDEX(Peripheral));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (Peripheral <= INT_SysTick)
    {
        /*---------------- ����INT_EIE0�Ĵ��� ----------------*/
        tmpreg = Peripheral - INT_StackFault;
        tmpreg1 = INT_EIE0_STACKIE << tmpreg;
        INT_EIE0 = SFR_Config (INT_EIE0,
                          ~tmpreg1,
                          NewState << (tmpreg+5));
    }
    else if (Peripheral <= INT_SPI1)
    {
        /*---------------- ����INT_EIE1�Ĵ��� ----------------*/
        tmpreg = Peripheral - INT_WWDT;
        tmpreg1 = INT_EIE1_WWDTIE << tmpreg;
        INT_EIE1 = SFR_Config (INT_EIE1,
                          ~tmpreg1,
                          NewState << tmpreg);
    }
    else if (Peripheral <= INT_USART7)
    {
        /*---------------- ����INT_EIE2�Ĵ��� ----------------*/
        tmpreg = Peripheral - INT_DMA1;
        tmpreg1 = INT_EIE2_DMA1IE << tmpreg;
        INT_EIE2 = SFR_Config (INT_EIE2,
                          ~tmpreg1,
                          NewState << tmpreg);
    }
    else
    {
        /*---------------- ����INT_EIE4�Ĵ��� ----------------*/
        /*---------------- ����������Ϊ����ж� ----------------*/
    }
}

/**
  * ����  SYSTICK�жϱ�־�����λ��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
INT_Set_Systick_Flag (void)
{
    SFR_SET_BIT_ASM(INT_EIF0, INT_EIF0_SYSTICKIF_POS);
}

/**
  * ����  PendSV�жϱ�־�����λ��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
INT_Set_PendSV_Flag (void)
{
    SFR_SET_BIT_ASM(INT_EIF0, INT_EIF0_PENDSVIF_POS);
}

/**
  * ����  ��ȡ������ں��жϱ�־�������û�δ����ı������ж�������
  *       ���ܳ���δ֪�����
  * ����  Peripheral: ������ں��ж�������ţ�ȡֵ��ΧΪ��
  *                   ö������InterruptIndex�е������ж�������š�
  * ����  �ޡ�
  */
FlagStatus
INT_Get_Interrupt_Flag (InterruptIndex Peripheral)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_INTERRUPT_INDEX(Peripheral));

    if (Peripheral <= INT_SysTick)
    {
        /*---------------- ����INT_EIF0�Ĵ��� ----------------*/
        tmpreg = INT_EIF0_NMIIF << (Peripheral - INT_NMI);
        tmpreg1 = INT_EIF0;
        if (INT_EIF0 & tmpreg)
        {
            /* �жϹ��� */
            return SET;
        }
        else
        {
            /* ���жϹ��� */
            return RESET;
        }
    }
    else if (Peripheral <= INT_SPI1)
    {
        /*---------------- ����INT_EIF1�Ĵ��� ----------------*/
        tmpreg = INT_EIF1_WWDTIF << (Peripheral - INT_WWDT);
        tmpreg1 = INT_EIF1;
        if (INT_EIF1 & tmpreg)
        {
            /* �жϹ��� */
            return SET;
        }
        else
        {
            /* ���жϹ��� */
            return RESET;
        }
    }
    else if (Peripheral <= INT_USART7)
    {
        /*---------------- ����INT_EIF2�Ĵ��� ----------------*/
        tmpreg = INT_EIF2_DMA1IF << (Peripheral - INT_DMA1);
        tmpreg1 = INT_EIF2;

    }
    else
    {
        /*---------------- ����INT_EIF4�Ĵ��� ----------------*/
        /*---------------- ����������Ϊ����ж� ----------------*/
    }

    /*---------------- �����Ӧ�жϱ�־λ״̬ ----------------*/
    if ((*(volatile uint32_t*)tmpreg1) & tmpreg)
    {
        /* �жϹ��� */
        return SET;
    }
    else
    {
        /* ���жϹ��� */
        return RESET;
    }
}
/**
  * ���� ��������ں��жϱ�־
  * ����  Peripheral: ������ں��ж�������ţ�ȡֵ��ΧΪ��
  *                   ö������InterruptIndex�е������ж�������š�
  * ����  �ޡ�
  */
void
INT_Clear_Interrupt_Flag (InterruptIndex Peripheral)
{
    uint32_t tmpreg = 0;
    volatile uint32_t *tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_INTERRUPT_INDEX(Peripheral));

    if (Peripheral <= INT_SysTick)
    {
        /*---------------- ����INT_EIF0�Ĵ��� ----------------*/
        tmpreg = INT_EIF0_NMIIF << (Peripheral - INT_NMI);
        tmpreg1 = &INT_EIF0;
    }
    else if (Peripheral <= INT_SPI1)
    {
        /*---------------- ����INT_EIF1�Ĵ��� ----------------*/
        tmpreg = INT_EIF1_WWDTIF << (Peripheral - INT_WWDT);
        tmpreg1 = &INT_EIF1;
    }
    else if (Peripheral <= INT_USART7)
    {
        /*---------------- ����INT_EIF2�Ĵ��� ----------------*/
        tmpreg = INT_EIF2_DMA1IF << (Peripheral - INT_DMA1);
        tmpreg1 = &INT_EIF2;
    }
    else
    {
        /*---------------- ����INT_EIF4�Ĵ��� ----------------*/
        /*---------------- ����������Ϊ����ж� ----------------*/
    }

    /*---------------- ���Ӧ�жϱ�־λ״̬ ----------------*/
    //(*(volatile uint32_t*)tmpreg1) &= ~tmpreg;
    *tmpreg1 &= ~tmpreg;

}
/**
  * ����  ������ں��ж����ȼ����ã������û�δ����ı������ж�������
  *       ���ܳ���δ֪�����
  * ����  Peripheral: ������ں��ж�������ţ�ȡֵ��ΧΪ��
  *                   ö������InterruptIndex�е������ж�������š�
  *       Preemption: ��ռ���ȼ���ͬʱ����PRIGROUP���á�
  *       SubPriority: �����ȼ��� ͬʱ����PRIGROUP���á�
  *       (GROUP) == INT_PRIORITY_GROUP_3VS1ʱ��Preemptionȡֵ��ΧΪ��0~7��SubPriorityȡֵ��ΧΪ��0~1
  *       (GROUP) == INT_PRIORITY_GROUP_2VS2ʱ��Preemptionȡֵ��ΧΪ��0~3��SubPriorityȡֵ��ΧΪ��0~3
  *       (GROUP) == INT_PRIORITY_GROUP_1VS3ʱ��Preemptionȡֵ��ΧΪ��0~1��SubPriorityȡֵ��ΧΪ��0~7
  *       (GROUP) == INT_PRIORITY_GROUP_0VS4ʱ��Preemptionȡֵ��ΧΪ��0~0��SubPriorityȡֵ��ΧΪ��0~15
  *
  * ����  �ޡ�
  */
void
INT_Interrupt_Priority_Config (InterruptIndex Peripheral,
                    uint32_t Preemption, uint32_t SubPriority)
{
    uint32_t tmpreg = 0;
    uint32_t regoffset = 0;
    uint32_t bitoffset = 0;
    uint32_t priorityconfig = 0;

    /*--------- ��ȡINT_CTL0�Ĵ�����PRIGROUPλ ---------*/
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_PRIGROUP;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_INTERRUPT_INDEX(Peripheral));
    CHECK_RESTRICTION(CHECK_PRIORITY_CONFIG(tmpreg, Preemption, SubPriority));

    /*--------- ��ȡ�ж����ȼ�����λ��ֵ ---------*/
    switch (tmpreg)
    {
    case INT_PRIORITY_GROUP_3VS1:
        priorityconfig = (Preemption << 1) | (SubPriority & 0x1);
        break;
    case INT_PRIORITY_GROUP_2VS2:
        priorityconfig = (Preemption << 2) | (SubPriority & 0x3);
        break;
    case INT_PRIORITY_GROUP_1VS3:
        priorityconfig = (Preemption << 3) | (SubPriority & 0x7);
        break;
    case INT_PRIORITY_GROUP_0VS4:
        priorityconfig = SubPriority & 0xF;
        break;
    default:
        CHECK_RESTRICTION(0);
        break;
    }

    /*--------- �����ж����ȼ�����λ ---------*/
    if (Peripheral <= INT_USART7)
    {
        /*---------------- ����INT_IPx(x=0~18)�Ĵ��� ----------------*/
        /* ��ȡ�ж����ȼ��Ĵ�����ַ */
        regoffset = (Peripheral - INT_Reserved4) >> 2;
        tmpreg = (uint32_t)&INT_IP0;
        tmpreg = tmpreg + (regoffset << 2);
    }
    else
    {
    }

    /*---------------- ����INT_IPx(x=0~30)�Ĵ��� ----------------*/
    bitoffset = ((Peripheral & 0x3) * 8) + 4;
    *(volatile uint32_t*)tmpreg
          = SFR_Config (*(volatile uint32_t*)tmpreg,
                      ~(INT_IP_INIT_MASK << bitoffset),
                      priorityconfig << bitoffset);
}

/**
  * ����  �ж���ʱ���á�
  * ����  IntDelay: �ж���ʱ���ƣ�ȡֵ8λ���ݡ�
  * ����  �ޡ�
  */
void
INT_Stack_Delay_Enable (uint8_t IntDelay)
{
    /*---------------- ����INT_CTL1�Ĵ���INTDELYλ ----------------*/
    INT_CTL1 = SFR_Config (INT_CTL1,
                      ~(INT_CTL1_INTDELY),
                      (uint32_t)IntDelay << INT_CTL1_INTDELY0_POS);
}
/**
  *   ##### �ж�(INT)�������ú���������� #####
  */


/**
  *   ##### �ⲿ�ж�(INT)���ܳ�ʼ���������� #####
  */
/**
  * ����  �ⲿ�ж�(EINT)���ã���ʹ���жϡ�
  * ����  eintInitStruct: �ⲿ�ж�������Ϣ�ṹ��ָ�롣
  * ����  �ޡ�
  */
void
INT_External_Configuration (EINT_InitTypeDef* eintInitStruct)
{
    uint32_t eintoffset = 0;
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_NUM(eintInitStruct->m_Line));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eintInitStruct->m_Mask));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eintInitStruct->m_Rise));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eintInitStruct->m_Fall));
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_SOURCE(eintInitStruct->m_Source));

    eintoffset = eintInitStruct->m_Line;
    /* ʹ���������ж� */
    /*------------------ ����INT_EINTRISE�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Line��m_Rise������EINTRIλ�� */
    INT_EINTRISE = SFR_Config (INT_EINTRISE,
                          ~(INT_EINTRISE_EINTRI0 << eintoffset),
                          eintInitStruct->m_Rise << eintoffset);

    /* ʹ���½����ж� */
    /*------------------ ����INT_EINTFALL�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Line��m_Fall������EINTFAλ�� */
    INT_EINTFALL = SFR_Config (INT_EINTFALL,
                          ~(INT_EINTFALL_EINTFA0 << eintoffset),
                          eintInitStruct->m_Fall << eintoffset);

    /* ѡ���ж�Դ */
    /*------------------ ����INT_EINTSSx�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Line��m_Source������EINTRIλ�� */
    if (eintoffset < 8)
    {
        tmpreg = eintInitStruct->m_Line * 4;
        INT_EINTSS0 = SFR_Config (INT_EINTSS0,
                          ~(INT_EINTSS_INIT_MASK << tmpreg),
                          eintInitStruct->m_Source << tmpreg);
    }
    else if (eintoffset < 16)
    {
        tmpreg = (eintInitStruct->m_Line - 8) * 4;
        INT_EINTSS1 = SFR_Config (INT_EINTSS1,
                          ~(INT_EINTSS_INIT_MASK << tmpreg),
                          eintInitStruct->m_Source << tmpreg);
    }
    else
    {
    }

    /* ʹ���ⲿ�ж� */
    /*------------------ ����INT_EINTMASK�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Line��m_Mask������EINTMλ�� */
    INT_EINTMASK = SFR_Config (INT_EINTMASK,
                          ~(INT_EINTMASK_EINTM0 << eintoffset),
                          eintInitStruct->m_Mask << eintoffset);
}

/**
  * ����  ��ʼ���ⲿ�ж�(EINT)������Ϣ�ṹ�塣
  * ����  eintInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
INT_External_Struct_Init (EINT_InitTypeDef* eintInitStruct)
{
    /* �����ⲿ�жϱ�� */
    eintInitStruct->m_Line = INT_EXTERNAL_INTERRUPT_0;
    /* �����ⲿ�ж�ʹ�ܿ��� */
    eintInitStruct->m_Mask = FALSE;
    /* �����ⲿ�ж��������ж�ʹ�� */
    eintInitStruct->m_Rise = FALSE;
    /* �����ⲿ�ж��½����ж�ʹ�� */
    eintInitStruct->m_Fall = FALSE;
    /* �����ⲿ�жϵ��ж�Դѡ�� */
    eintInitStruct->m_Source = INT_EXTERNAL_SOURCE_PA;
}
/**
  *   ##### �ⲿ�ж�(INT)���ܳ�ʼ������������� #####
  */


/**
  *   ##### �ⲿ�ж�(INT)�������ú������� #####
  */
/**
  * ����  �ⲿ�ж�(EINT)ʹ�����á�
  * ����  EintMask: �ⲿ�жϱ�����룬
  *                 ȡֵΪ��INT_EINTMASK_EINTM0��INT_EINTMASK_EINTM31��λ����ϡ�
  *       NewState: �ⲿ�ж�ʹ������
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
INT_External_Mask_Enable (uint32_t EintMask, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����INT_EINTMASK�Ĵ���EINTMxλ ----------------*/
    if (NewState != FALSE)
    {
        INT_EINTMASK |= EintMask;
    }
    else
    {
        INT_EINTMASK &= ~EintMask;
    }
}

/**
  * ����  �ⲿ�ж�(EINT)�������ж�ʹ�����á�
  * ����  EintMask: �ⲿ�жϱ�����룬
  *                 ȡֵΪ��INT_EINTMASK_EINTM0��INT_EINTMASK_EINTM31��λ����ϡ�
  *       NewState: �ⲿ�ж��������ж�ʹ�ܣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
INT_External_Rise_Enable (uint32_t EintMask, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����INT_EINTRISE�Ĵ���EINTRIxλ ----------------*/
    if (NewState != FALSE)
    {
        INT_EINTRISE |= EintMask;
    }
    else
    {
        INT_EINTRISE &= ~EintMask;
    }
}

/**
  * ����  �ⲿ�ж�(EINT)�½����ж�ʹ�����á�
  * ����  EintMask: �ⲿ�жϱ�����룬
  *                 ȡֵΪ��INT_EINTMASK_EINTM0��INT_EINTMASK_EINTM31��λ����ϡ�
  *       NewState: �ⲿ�ж��½����ж�ʹ�ܣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
INT_External_Fall_Enable (uint32_t EintMask, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����INT_EINTFALL�Ĵ���EINTFAxλ ----------------*/
    if (NewState != FALSE)
    {
        INT_EINTFALL |= EintMask;
    }
    else
    {
        INT_EINTFALL &= ~EintMask;
    }
}

/**
  * ����  ��ȡ�ⲿ�ж�(EINT)�жϱ�־λ��
  * ����  EintNum: �ⲿ�жϱ�ţ�
  *                ȡֵΪ��INT_EXTERNAL_INTERRUPT_0��
  *                INT_EXTERNAL_INTERRUPT_31�е�һ����
  *                ��0~31��
  * ����  �ⲿ�ж�(EINT)�жϱ�־��0��û�з����ⲿ�жϣ�1�������ⲿ�жϡ�
  */
FlagStatus
INT_Get_External_Flag (uint32_t EintNum)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_NUM(EintNum));

    /*---------------- ��ȡINT_EINTF�Ĵ���EINTIFxλ ----------------*/
    tmpreg = INT_EINTF_EINTIF0 << EintNum;
    if (INT_EINTF & tmpreg)
    {
        /* �������ⲿ�ж� */
        return SET;
    }
    else
    {
        /* û�з������ⲿ�ж� */
        return RESET;
    }
}

/**
  * ����  ����ⲿ�ж�(EINT)�жϱ�־λ��
  * ����  EintNum: �ⲿ�жϱ�ţ�
  *                ȡֵΪ��INT_EXTERNAL_INTERRUPT_0��
  *                INT_EXTERNAL_INTERRUPT_31�е�һ����
  *                ��0~31��
  * ����  �ޡ�
  */
void
INT_External_Clear_Flag (uint32_t EintNum)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_NUM(EintNum));

    /*---------------- ����INT_EINTF�Ĵ���EINTIFxλ ----------------*/
    tmpreg = INT_EINTF_EINTIF0 << EintNum;
    while((INT_EINTF & tmpreg)>>(EintNum+INT_EINTF_EINTIF0_POS))
    {
    	INT_EINTF &= ~tmpreg;
    }
}

/**
  * ����  �ⲿ�ж�(EINT)�ж�Դ���á�
  * ����  EintNum: �ⲿ�жϱ�ţ�
  *                ȡֵΪ��INT_EXTERNAL_INTERRUPT_0��
  *                INT_EXTERNAL_INTERRUPT_31�е�һ����
  *                ��0~15��
  *       PeripheralSource: �����ж��ߵ��ж�����Դ��
  *                         ȡֵ��ΧΪ��
  *                           INT_EXTERNAL_SOURCE_PA
  *                           INT_EXTERNAL_SOURCE_PB
  *                           INT_EXTERNAL_SOURCE_PC
  *                           INT_EXTERNAL_SOURCE_PD
  *                           INT_EXTERNAL_SOURCE_PE
  *                           INT_EXTERNAL_SOURCE_PF
  *                           INT_EXTERNAL_SOURCE_PG
  *                           INT_EXTERNAL_SOURCE_PH
  *                           INT_EXTERNAL_SOURCE_PVD
  *                           INT_EXTERNAL_SOURCE_RTC
  *                           INT_EXTERNAL_SOURCE_TAMPER
  *                           INT_EXTERNAL_SOURCE_ALARMCLK
  *                           INT_EXTERNAL_SOURCE_AES
  *                           INT_EXTERNAL_SOURCE_EINT21TO31
  * ����  �ޡ�
  */
void
INT_External_Source_Enable (uint32_t EintNum, uint32_t PeripheralSource)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_PERIPHERAL(EintNum));
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_SOURCE(PeripheralSource));

    /*---------------- ����INT_EINTSS0�Ĵ���EINTSOUxλ ----------------*/
    tmpreg = (EintNum % 8) * 4;
    if (EintNum < 8)
    {
        INT_EINTSS0 = SFR_Config (INT_EINTSS0,
                          ~(INT_EINTSS_INIT_MASK << tmpreg),
                          PeripheralSource << tmpreg);
    }
    else if (EintNum < 16)
    {
        INT_EINTSS1 = SFR_Config (INT_EINTSS1,
                          ~(INT_EINTSS_INIT_MASK << tmpreg),
                          PeripheralSource << tmpreg);
    }
    else
    {
    }
}
/**
  *   ##### �ⲿ�ж�(INT)�������ú���������� #####
  */

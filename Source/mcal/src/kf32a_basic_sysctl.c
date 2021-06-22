/**
  ******************************************************************************
  * �ļ���  kf32a_basic_sysctl.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��ϵͳ������صĹ��ܺ�����������
  *          + ϵͳ����(SYSCTL)�������ú���
  *          + DOָ������ú���
  *
  *********************************************************************
  */

#include "kf32a_basic_sysctl.h"

/* ϵͳ����˽�ж��� ----------------------------------------------------*/
/* SYS_ARCTL�Ĵ���д����ARKEY���� */
#define SYS_ARCTL_WRITE_CTL             ((uint32_t)0x05FA \
                                      << SYS_ARCTL_ARKEY_POS)


/**
  *   ##### ϵͳ����(SYSCTL)�������ú��� #####
  */
/**
  * ����  ��ȡ״̬��Vλ�����־��
  * ����  �ޡ�
  * ����  1:�����0:�������
  */
FlagStatus
SYSCTL_Get_V_Flag (void)
{
    /*---------------- ��ȡSYS_PSW�Ĵ���Vλ ----------------*/
    if (SYS_PSW & SYS_PSW_V)
    {
        /* ��� */
        return SET;
    }
    else
    {
        /* ����� */
        return RESET;
    }
}

/**
  * ����  ��ȡ״̬��Cλ��λ���λ��־��
  * ����  �ޡ�
  * ����  1:�ӷ��н�λ������޽�λ��0:�ӷ��޽�λ������н�λ��
  */
FlagStatus
SYSCTL_Get_C_Flag (void)
{
    /*---------------- ��ȡSYS_PSW�Ĵ���Cλ ----------------*/
    if (SYS_PSW & SYS_PSW_C)
    {
        /* �ӷ��н�λ������޽�λ */
        return SET;
    }
    else
    {
        /* �ӷ��޽�λ������н�λ */
        return RESET;
    }
}

/**
  * ����  ��ȡ״̬��Zλ��ֵ��־��
  * ����  �ޡ�
  * ����  1:��ֵ��0:����ֵ��
  */
FlagStatus
SYSCTL_Get_Z_Flag (void)
{
    /*---------------- ��ȡSYS_PSW�Ĵ���Zλ ----------------*/
    if (SYS_PSW & SYS_PSW_Z)
    {
        /* ��ֵ */
        return SET;
    }
    else
    {
        /* ����ֵ */
        return RESET;
    }
}

/**
  * ����  ��ȡ״̬��Nλ��ֵ��־��
  * ����  �ޡ�
  * ����  1:��ֵ��0:�Ǹ�ֵ��
  */
FlagStatus
SYSCTL_Get_N_Flag (void)
{
    /*---------------- ��ȡSYS_PSW�Ĵ���Nλ ----------------*/
    if (SYS_PSW & SYS_PSW_N)
    {
        /* ��ֵ */
        return SET;
    }
    else
    {
        /* �Ǹ�ֵ */
        return RESET;
    }
}

/**
  * ����  ����״̬��Vλ�����־��
  * ����  NewState: ״̬��Vλ�����־��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Set_V_Flag (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����SYS_PSW�Ĵ���Vλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���������־ */
        SFR_SET_BIT_ASM(SYS_PSW, SYS_PSW_V_POS);
    }
    else
    {
        /* ���������־ */
        SFR_CLR_BIT_ASM(SYS_PSW, SYS_PSW_V_POS);
    }
}

/**
  * ����  ����״̬��Cλ�����־��
  * ����  NewState: ״̬��Cλ��λ���λ��־��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Set_C_Flag (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����SYS_PSW�Ĵ���Cλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���ý�λ���λ��־ */
        SFR_SET_BIT_ASM(SYS_PSW, SYS_PSW_C_POS);
    }
    else
    {
        /* �����λ���λ��־ */
        SFR_CLR_BIT_ASM(SYS_PSW, SYS_PSW_C_POS);
    }
}

/**
  * ����  ����״̬��Zλ�����־��
  * ����  NewState: ״̬��Zλ��λ���λ��־��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Set_Z_Flag (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����SYS_PSW�Ĵ���Zλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���ý�λ���λ��־ */
        SFR_SET_BIT_ASM(SYS_PSW, SYS_PSW_Z_POS);
    }
    else
    {
        /* �����λ���λ��־ */
        SFR_CLR_BIT_ASM(SYS_PSW, SYS_PSW_Z_POS);
    }
}
/**
  * ����  ����״̬��Nλ��ֵ��־��
  * ����  NewState: ״̬��Nλ��ֵ��Ǹ�ֵ��־��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Set_N_Flag (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����SYS_PSW�Ĵ���Zλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���ý�λ���λ��־ */
        SFR_SET_BIT_ASM(SYS_PSW, SYS_PSW_N_POS);
    }
    else
    {
        /* �����λ���λ��־ */
        SFR_CLR_BIT_ASM(SYS_PSW, SYS_PSW_N_POS);
    }
}

/**
  * ����  �����жϴ����ؽ�������ģʽʹ�ܡ�
  * ����  NewState: �жϴ����ؽ�������ģʽʹ��״̬��ȡֵΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Sleep_On_Exit_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����SYS_MCTL�Ĵ���SLEEPONEXITλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ���жϷ���ʱ��������ģʽ */
        SFR_SET_BIT_ASM(SYS_MCTL, SYS_MCTL_SLEEPONEXIT_POS);
    }
    else
    {
        /* ��ֹ�жϷ���ʱ��������ģʽ */
        SFR_CLR_BIT_ASM(SYS_MCTL, SYS_MCTL_SLEEPONEXIT_POS);
    }
}

/**
  * ����  �����������ģʽʹ�ܡ�
  * ����  NewState: �������ģʽʹ��״̬��ȡֵΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Deep_Sleep_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����SYS_MCTL�Ĵ���SLEEPDEEPλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ���������ģʽ */
        SFR_SET_BIT_ASM(SYS_MCTL, SYS_MCTL_SLEEPDEEP_POS);
    }
    else
    {
        /* ��ֹ�������ģʽ */
        SFR_CLR_BIT_ASM(SYS_MCTL, SYS_MCTL_SLEEPDEEP_POS);
    }
}

/**
  * ����  ���û���ģʽѡ��
  * ����  NewState: �����жϻ���CPUʹ��״̬��ȡֵΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Interrupt_Awake_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����SYS_MCTL�Ĵ���SLPONMODλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���������ж϶��ܻ���CPU */
        SFR_SET_BIT_ASM(SYS_MCTL, SYS_MCTL_SLPONMOD_POS);
    }
    else
    {
        /* ֻ�е�������ж����ȼ��ȵ�ǰ���ȼ���ʱ���ܻ���CPU */
        SFR_CLR_BIT_ASM(SYS_MCTL, SYS_MCTL_SLPONMOD_POS);
    }
}

/**
  * ����  ��ȡ��ǰ�ж��Զ���ջ�Ķ��뷽ʽ��
  * ����  �ޡ�
  * ����  1:˫�ֶ���ʹ�ܣ�0:˫�ֶ���δʹ�ܡ�
  */
FlagStatus
SYSCTL_Stack_Align_State (void)
{
    /*---------------- ��ȡSYS_MCTL�Ĵ���STACKALIGNλ ----------------*/
    if (SYS_MCTL & SYS_MCTL_STACKALIGN)
    {
        /* ˫�ֶ���ʹ�� */
        return SET;
    }
    else
    {
        /* ˫�ֶ���δʹ�� */
        return RESET;
    }
}

/**
  * ����  ���ó����û����ơ�
  * ����  NewState: ѡ�񳬼��û���־��ȡֵΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Super_User_Config (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����SYS_MCTL�Ĵ���SUSTAλ ----------------*/
    if (NewState != FALSE)
    {
        /* �����û�ģʽ */
        SFR_SET_BIT_ASM(SYS_MCTL, SYS_MCTL_SUSTA_POS);
    }
    else
    {
        /* �û�ģʽ */
        SFR_CLR_BIT_ASM(SYS_MCTL, SYS_MCTL_SUSTA_POS);
    }
}

/**
  * ����  ��ȡ��ǰ��Ч��ջָ��״̬��־��
  * ����  �ޡ�
  * ����  1:PSP�ǵ�ǰ�Ķ�ջָ�룻0:MSP�ǵ�ǰ�Ķ�ջָ�롣
  */
FlagStatus
SYSCTL_Stack_Pointer_State (void)
{
    /*---------------- ��ȡSYS_MCTL�Ĵ���SPSELλ ----------------*/
    if (SYS_MCTL & SYS_MCTL_SPSEL)
    {
        /* PSP�ǵ�ǰ�Ķ�ջָ�� */
        return SET;
    }
    else
    {
        /* MSP�ǵ�ǰ�Ķ�ջָ�� */
        return RESET;
    }
}

/**
  * ����  ���õ�ǰ��Ч��ջָ�롣
  * ����  PresentSP: ��ǰ��Ч��ջָ�룬ȡֵΪ��
  *                    SYSCTL_SP_IS_MSP: MSP�ǵ�ǰ�Ķ�ջָ��
  *                    SYSCTL_SP_IS_PSP: PSP�ǵ�ǰ�Ķ�ջָ��
  * ����  �ޡ�
  */
void
SYSCTL_Stack_Pointer_Config (uint32_t PresentSP)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_SYSCTL_SP(PresentSP));

    /*------------------ ����SYS_MCTL�Ĵ���SPSELλ -----------------*/
    SYS_MCTL = SFR_Config (SYS_MCTL, ~SYS_MCTL_SPSEL, PresentSP);
}

/**
  * ����  �����쳣���Ϣ��λ����
  * ����  NewState: �쳣���Ϣ��λ����ȡֵΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_Exception_Reset_Enable (FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------------ ����SYS_ARCTL�Ĵ���VECTRSTλ -----------------*/
    tmpreg = SYS_ARCTL_WRITE_CTL | \
            ((uint32_t)NewState << SYS_ARCTL_VECTRST_POS);
    SYS_ARCTL = SFR_Config (SYS_ARCTL,
                        ~(SYS_ARCTL_VECTRST | SYS_ARCTL_ARKEY),
                        tmpreg);
}

/**
  * ����  ����ϵͳ��λ����
  * ����  NewState: ϵͳ��λ����ȡֵΪ��TRUE �� FALSE��
  * ����  �ޡ�
  */
void
SYSCTL_System_Reset_Enable (FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------------ ����SYS_ARCTL�Ĵ���SYSTEMRSTλ -----------------*/
    tmpreg = SYS_ARCTL_WRITE_CTL | \
            ((uint32_t)NewState << SYS_ARCTL_SYSTEMRST_POS);
    SYS_ARCTL = SFR_Config (SYS_ARCTL,
                        ~(SYS_ARCTL_SYSTEMRST | SYS_ARCTL_ARKEY),
                        tmpreg);
}

/**
  * ����  �ж���������ӳ���ַ���á�
  * ����  VectorOffset: �ж���������ӳ���ַ��ȡֵ32λ���ݣ�����λӲ�����ԡ�
  * ����  �ޡ�
  */
void
SYSCTL_Vector_Offset_Config (uint32_t VectorOffset)
{
	uint32_t tmpreg = 0;
    /*---------------- ����SYS_VECTOFF�Ĵ���TBLOFFλ ----------------*/
	tmpreg = VectorOffset << SYS_VECTOFF_TBLOFF0_POS;
    SYS_VECTOFF = SFR_Config (SYS_VECTOFF,
                      ~(SYS_VECTOFF_TBLOFF),
                      tmpreg);
}


/**
  * ����  ����RAM�ռ䳤�ȣ�ָʾRAM�ռ������ַ��
  * ����  RamSpace: RAM�ռ䳤�ȣ�ȡֵΪ26λ��Ч��ֵ��
  * ����  �ޡ�
  */
void
SYSCTL_Ram_Space_Config (uint32_t RamSpace)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RAM_SPACE(RamSpace));

    /*---------------- ����SYS_RAMSPA�Ĵ���RAMSPAλ ----------------*/
    tmpreg = RamSpace << SYS_RAMSPA_RAMSPA0_POS;
    SYS_RAMSPA = SFR_Config (SYS_RAMSPA,
                      ~SYS_RAMSPA_RAMSPA,
                      tmpreg);
}

/**
  * ����  ����FLASH��ʼ512�ֽڿռ�ӳ����ơ�
  * ����  MemCtl: ӳ����ƣ�ȡֵ��ΧΪ:
  *                 SYSCTL_FLASH_REMAP_FOR_ROM: ΪROM��ӳ��
  *                 SYSCTL_FLASH_REMAP_FOR_RAM: ΪRAM��ӳ��
  *                 SYSCTL_FLASH_REMAP_FOR_FLASH: ΪFLASH��ӳ��
  *                 SYSCTL_FLASH_REMAP_STOP_CPU: ֹͣCPU����
  * ����  �ޡ�
  */
void
SYSCTL_Flash_Start_Remap_Config (uint32_t MemCtl)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_SYSCTL_FLASH_REMAP(MemCtl));

    /*---------------- ����SYS_MEMCTL�Ĵ���MEMMλ ----------------*/
    SYS_MEMCTL = SFR_Config (SYS_MEMCTL, ~SYS_MEMCTL_MEMM, MemCtl);
}
/**
  *   ##### ϵͳ����(SYSCTL)�������ú���������� #####
  */



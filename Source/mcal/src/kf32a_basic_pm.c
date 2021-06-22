/**
  ******************************************************************************
  * �ļ���  kf32a_basic_pm.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˵�Դ����(PM)��صĹ��ܺ�����������
  *          + ��Դ����(PM)���ƹ��ܺ���
  *          + ��Դ����(PM)״̬��־������
  *          + ��Դ����(PM)У׼���ƺ���
  *
  *********************************************************************
  */

#include "kf32a_basic_pm.h"
#include "kf32a_basic_osc.h"


/**
  *   ##### ��Դ����(PM)���ƹ��ܺ��� #####
  */
/**
  * ����  ����IO��״̬����ʹ�ܡ�
  * ����  NewState: IO��״̬����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_IO_Latch_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���IOLATCHλ ----------------*/
    if (NewState != FALSE)
    {
        /*  IO��״̬������ */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_IOLATCH_POS);
    }
    else
    {
        /* IO��״̬δ������ */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_IOLATCH_POS);
    }
}

/**
  * ����  ��ȡIO��״̬���档
  * ����  �ޡ�
  * ����  1:IO��״̬�����棻0:IO��״̬δ�����档
  */
FlagStatus
PM_Get_IO_Latch_Status (void)
{
    /*---------------- ��ȡPM_CTL0�Ĵ���IOLATCHλ ----------------*/
    if (PM_CTL0 & PM_CTL0_IOLATCH)
    {
        /* IO��״̬������ */
        return SET;
    }
    else
    {
        /* IO��״̬δ������ */
        return RESET;
    }
}

/**
  * ����  �����ڲ���Ƶ�������ʹ�ܡ�
  * ����  NewState: �ڲ���Ƶ�������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Internal_Low_Frequency_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���LSIENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���ʹ���ڲ���Ƶ���� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LSIEN_POS);
    }
    else
    {
        /* ���δʹ���ڲ���Ƶ���� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_LSIEN_POS);
    }
}

/**
  * ����  �����ⲿ��Ƶ�������ʹ�ܡ�
  * ����  NewState: �ⲿ��Ƶ�������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_External_Low_Frequency_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���LSEENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���ʹ���ⲿ��Ƶ���� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LSEEN_POS);
    }
    else
    {
        /* ���δʹ���ⲿ��Ƶ���� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_LSEEN_POS);
    }
}

/**
  * ����  �����ⲿ��Ƶ����ʱ������ʹ�ܡ�
  * ����  NewState: �ⲿ��Ƶ����ʱ������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_External_Low_Frequency_Clock_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���LSEEXENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ��ֱ�Ӵ��ⲿ��Ƶ������������ʱ���ź���Ϊ�������� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LSEEXEN_POS);
    }
    else
    {
        /* ��ֹ�ⲿʱ�����빦�� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_LSEEXEN_POS);
    }
}

/**
  * ����  ������BG���ʹ�ܡ�
  * ����  NewState: ��BG���ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Main_Bandgap_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���MRBGENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���ʹ����BGģ�� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_MRBGEN_POS);
    }
    else
    {
        /* δ���ʹ����BGģ�� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_MRBGEN_POS);
    }
}

/**
  * ����  ����LDO18���ʹ�ܡ�
  * ����  NewState: LDO18���ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_LDO18_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���LDO18ENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��LDO18ģ�� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LDO18EN_POS);
    }
    else
    {
        /* ��ֹLDO18ģ�飬Power18��flash������ */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_LDO18EN_POS);
    }
}

/**
  * ����  ���ñ������Ĵ���ģ�������λ��
  * ����  BkpReset: �������Ĵ���ģ�������λ״̬��ȡֵΪ:
  *                   PERIPHERAL_RST_STATUS: �������Ĵ���ģ�鴦�ڸ�λ״̬
  *                   PERIPHERAL_OUTRST_STATUS: �������Ĵ���ģ���˳���λ
  * ����  �ޡ�
  */
void
PM_Backup_Registers_Reset_Config (uint32_t BkpReset)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_RST(BkpReset));

    /*---------------- ����PM_CTL0�Ĵ���BKPREGCLRλ ----------------*/
    if (BkpReset != PERIPHERAL_RST_STATUS)
    {
        /* �������Ĵ���ģ���˳���λ */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
    }
    else
    {
        /* �������Ĵ���ģ�鴦�ڸ�λ״̬ */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
    }
}

/**
  * ����  ����IWDTģ�������λ��
  * ����  IWDTReset: IWDTģ�������λ״̬��ȡֵΪ:
  *                   PERIPHERAL_RST_STATUS: IWDT���ڸ�λ״̬
  *                   PERIPHERAL_OUTRST_STATUS: IWDT�˳���λ
  * ����  �ޡ�
  */
void
PM_Independent_Watchdog_Reset_Config (uint32_t IWDTReset)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_RST(IWDTReset));

    /*---------------- ����PM_CTL0�Ĵ���IWDTCLRλ ----------------*/
    if (IWDTReset != PERIPHERAL_RST_STATUS)
    {
        /* IWDT�˳���λ */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_IWDTCLR_POS);
    }
    else
    {
        /* IWDT���ڸ�λ״̬ */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_IWDTCLR_POS);
    }
}

/**
  * ����  ����SRAM��A����standbyģʽ�¹������á�
  * ����  WorkMode: SRAM��A����standbyģʽ�¹������ã�ȡֵΪ:
  *                   PM_SRAMA_IN_STANDBY_POWER_DOWN: SRAM��A����standbyģʽ�µ���
  *                   PM_SRAMA_IN_STANDBY_KEEP_DATA: SRAM��A����standbyģʽ�±�������
  * ����  �ޡ�
  */
void
PM_SRAMA_In_Standby_Work_Mode_Config (uint32_t WorkMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_SRAMA_IN_STANDBY(WorkMode));

    /*---------------- ����PM_CTL0�Ĵ���SRAMASELλ ----------------*/
    if (WorkMode != PM_SRAMA_IN_STANDBY_POWER_DOWN)
    {
        /* SRAM��A����standbyģʽ�±������� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_SRAMASEL_POS);
    }
    else
    {
        /* SRAM��A����standbyģʽ�µ��� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_SRAMASEL_POS);
    }
}

/**
  * ����  ����LPRAM��standbyģʽ�¹������á�
  * ����  WorkMode: LPRAM��standbyģʽ�¹������ã�ȡֵΪ:
  *                   PM_LPRAM_IN_STANDBY_POWER_DOWN: LPRAM��standbyģʽ�µ���
  *                   PM_LPRAM_IN_STANDBY_KEEP_DATA: LPRAM��standbyģʽ�±�������
  * ����  �ޡ�
  */
void
PM_LPRAM_In_Standby_Work_Mode_Config (uint32_t WorkMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_LPSRAM_IN_STANDBY(WorkMode));

    /*---------------- ����PM_CTL0�Ĵ���SRAMASELλ ----------------*/
    if (WorkMode != PM_LPRAM_IN_STANDBY_POWER_DOWN)
    {
        /* SRAM��B����standbyģʽ�±������� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LPSRAMSEL_POS);
    }
    else
    {
        /* SRAM��B����standbyģʽ�µ��� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_LPSRAMSEL_POS);
    }
}


/**
  * ����  ����BKP_POR��ʱʱ�䡣
  * ����  DelayTime: BKP_POR��ʱʱ�䣬ȡֵΪ:
  *                    DELAY_TIME_1ms: 1ms
  *                    DELAY_TIME_32us: 32us
  * ����  �ޡ�
  */
void
PM_Backup_POR_Delay_Time_Config (uint32_t DelayTime)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DELAY_TIME(DelayTime));

    /*---------------- ����PM_CTL0�Ĵ���BKPPORDELAYSELλ ----------------*/
    if (DelayTime != DELAY_TIME_1ms)
    {
        /* ��ʱ1ms */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPPORDELAYSEL_POS);
    }
    else
    {
        /* ��ʱ32us */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_BKPPORDELAYSEL_POS);
    }
}

/**
  * ����  ������POR��POR12��POR18��ʱʱ�䡣
  * ����  DelayTime: ��POR��POR12��POR18��ʱʱ�䣬ȡֵΪ:
  *                    DELAY_TIME_2ms: 2ms
  *                    DELAY_TIME_32us: 32us
  * ����  �ޡ�
  */
void
PM_Main_POR_Delay_Time_Config (uint32_t DelayTime)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DELAY_TIME(DelayTime));

    /*---------------- ����PM_CTL0�Ĵ���PORDELAYSELλ ----------------*/
    if (DelayTime != DELAY_TIME_2ms)
    {
        /* ��ʱ2ms */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_PORDELAYSEL_POS);
    }
    else
    {
        /* ��ʱ32us */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_PORDELAYSEL_POS);
    }
}

/**
  * ����  ���õ͹�������IO�ڡ�
  * ����  PeripheralPort: �͹�������IO�ڣ�ȡֵΪ:
  *                         PM_GENERAL_PURPOSE_IO_PORT: ͨ��IO��
  *                         PM_LOW_POWER_IO_PORT: �͹���ר��IO��
  * ����  �ޡ�
  */
void
PM_Peripheral_IO_Port_Config (uint32_t PeripheralPort)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_LOW_POWER_IO(PeripheralPort));

    /*---------------- ����PM_CTL0�Ĵ���PHERIIOSELλ ----------------*/
    if (PeripheralPort != PM_GENERAL_PURPOSE_IO_PORT)
    {
        /* �͹���ר��IO�� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_PHERIIOSEL_POS);
    }
    else
    {
        /* ͨ��IO�� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_PHERIIOSEL_POS);
    }
}
/**
  * ����  �͹���ģʽ�������ڲ���Ƶ����У׼ֵ
  * ����  NewState: HSI�ڲ���Ƶ����У׼ֵ����λ,ȡֵΪTRUE �� FALSE
  * ����  ��
  */
void
PM_OCAL0LOCK_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���OCALLOCKλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���澧��У׼ֵ */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_OCAL0LOCK_POS);
    }
    else
    {
        /* �����澧��У׼ֵ */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_OCAL0LOCK_POS);
    }
}

/**
  * ����  �͹���ģʽ��MEMM���ݱ���ʹ��
  * ����  NewState: MEMM���ݱ���ʹ��λ,ȡֵΪTRUE �� FALSE
  * ����  ��
  */
void
PM_MEMSEL_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���OCALLOCKλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���澧��У׼ֵ */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_MEMSEL_POS);
    }
    else
    {
        /* �����澧��У׼ֵ */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_MEMSEL_POS);
    }
}


/**
  * ����  ����FLASH�����Դ����ض�ʹ�ܡ�
  * ����  NewState: FLASH�����Դ����ض�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Flash_Power_Off_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL2�Ĵ���VF12ENλ ----------------*/
    if (NewState != FALSE)
    {
        /* �ض�FLASH�����Դ���أ�FLASHģ����� */
        SFR_CLR_BIT_ASM(PM_CTL2, PM_CTL2_VF12EN_POS);
    }
    else
    {
        /* ���δ�ض�FLASH�����Դ���� */
        SFR_SET_BIT_ASM(PM_CTL2, PM_CTL2_VF12EN_POS);
    }
}

/**
  * ����  ����CCP0�ڲ���Ƶʱ��ʹ�ܡ�
  * ����  NewState: CCP0�ڲ���Ƶʱ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_CCP0CLKLPEN_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL2�Ĵ���CCP0CLKLPENλ ----------------*/
    if (NewState != FALSE)
    {
        /* �����ڲ���Ƶʱ����Ϊ CCP0 ����ʱ�� */
        SFR_SET_BIT_ASM(PM_CTL2, PM_CTL2_CCP0CLKLPEN_POS);
    }
    else
    {
        /* ��ֹ�ڲ���Ƶʱ����Ϊ CCP0 ����ʱ�� */
        SFR_CLR_BIT_ASM(PM_CTL2, PM_CTL2_CCP0CLKLPEN_POS);
    }
}
/**
  * ����  ���ñ�������д��������ʹ�ܡ�
  * ����  NewState: ��������д��������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Backup_Write_And_Read_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���BKPWRλ ----------------*/
    if (NewState != FALSE)
    {
        /* ����CPU�Ա������ڼĴ������ж�д���� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);
    }
    else
    {
        /* ��ֹCPU�Ա������ڼĴ������ж�д���� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);
    }
}
/**
  * ����  ���òο���ѹʹ�ܡ�
  * ����  NewState: �ο���ѹʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_VREF_Software_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���VREFENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���ʹ�ܲο���ѹ */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_VREFEN_POS);
    }
    else
    {
        /* ���δʹ�ܲο���ѹ*/
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_VREFEN_POS);
    }
}
/**
  * ����  ���òο���ѹ��λѡ��
  * ����  NewState: �ο���ѹ��λѡ��
  * 			PM_VREF_VOLTAGE_2P0V
  * 			PM_VREF_VOLTAGE_1P5V
  * 			PM_VREF_VOLTAGE_2P5V
  * 			PM_VREF_VOLTAGE_3P0V
  * ����  �ޡ�
  */
void
PM_VREF_SELECT (uint32_t Voltage)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_VREF_VOLTAGE(Voltage));

    /*---------------- ����PM_CTL0�Ĵ���VREFSELλ ----------------*/
    PM_CTL1 = SFR_Config (PM_CTL1, ~PM_CTL1_VREFSEL, Voltage);
}
/**
  * ����  ���õ͹��ĵ�ѹ������LPR���ʹ�ܡ�
  * ����  NewState: �͹��ĵ�ѹ������LPR���ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_LPR_Software_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���LPRENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���ʹ��LPR */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LPREN_POS);
    }
    else
    {
        /* ���δʹ��LPR */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_LPREN_POS);
    }
}

/**
  * ����  ���õ͹���ģʽ��
  * ����  LowPowerMode: �͹���ģʽ��ȡֵΪ:
  *                       PM_LOW_POWER_MODE_STOP_0: ֹͣģʽ0
  *                       PM_LOW_POWER_MODE_STOP_1: ֹͣģʽ1
  *                       PM_LOW_POWER_MODE_STANDBY: ����ģʽ
  *                       PM_LOW_POWER_MODE_SHUTDOWN: �ض�ģʽ
  * ����  �ޡ�
  */
void
PM_Low_Power_Mode_Config (uint32_t LowPowerMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_LOW_POWER_MODE(LowPowerMode));

    /*---------------- ����PM_CTL0�Ĵ���LPMSλ ----------------*/
    PM_CTL0 = SFR_Config (PM_CTL0, ~PM_CTL0_LPMS, LowPowerMode);
}

/**
  * ����  ����BORʹ�ܡ�
  * ����  NewState: BORʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_BOR_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���BORENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��BOR */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_BOREN_POS);
    }
    else
    {
        /* ��ֹBOR */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_BOREN_POS);
    }
}

/**
  * ����  ����LPBORʹ�ܡ�
  * ����  NewState: LPBORʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Low_Power_BOR_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���LPBORENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��LPBOR */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_LPBOREN_POS);
    }
    else
    {
        /* ��ֹLPBOR */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_LPBOREN_POS);
    }
}

/**
  * ����  ����TEMPSENSORʹ�ܡ�
  * ����  NewState: TEMPSENSORʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Temperature_Sensor_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���TSENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��TEMPSENSOR */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_TSEN_POS);
    }
    else
    {
        /* ��ֹTEMPSENSOR */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_TSEN_POS);
    }
}

/**
  * ����  ����TEMPSENSOR�����BUFFERʹ�ܡ�
  * ����  NewState: TEMPSENSOR�����BUFFERʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Temperature_Sensor_Buffer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���TSOEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��TEMPSENSOR�����BUFFER */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_TSOE_POS);
    }
    else
    {
        /* ��ֹTEMPSENSOR�����BUFFER */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_TSOE_POS);
    }
}

/**
  * ����  ���òο���ѹ2Vģ��ʹ�ܡ�
  * ����  NewState: �ο���ѹ2Vģ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Reference_Voltage_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���VREFENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܲο���ѹ2Vģ�� */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_VREFEN_POS);
    }
    else
    {
        /* ��ֹ�ο���ѹ2Vģ�� */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_VREFEN_POS);
    }
}

/**
  * ����  �����ڲ��������BUFFERʱ��ʹ�ܡ�
  * ����  NewState: �ڲ��������BUFFERʱ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Internal_Test_Buffer_Clock_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���BUFCLKENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ���ڲ��������BUFFER����ʱ�� */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_BUFCLKEN_POS);
    }
    else
    {
        /* ��ֹ�ڲ��������BUFFER����ʱ�� */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_BUFCLKEN_POS);
    }
}

/**
  * ����  �����ڲ��������BUFFER����ʱ�ӷ�Ƶ��
  * ����  SclkScaler: �ڲ��������BUFFER����ʱ�ӷ�Ƶ��ȡֵΪ:
  *                     PM_BUFFER_SCLK_DIV_1: SCLK
  *                     PM_BUFFER_SCLK_DIV_2: SCLK/2
  *                     PM_BUFFER_SCLK_DIV_4: SCLK/4
  *                     PM_BUFFER_SCLK_DIV_8: SCLK/8
  *                     PM_BUFFER_SCLK_DIV_16: SCLK/16
  *                     PM_BUFFER_SCLK_DIV_32: SCLK/32
  *                     PM_BUFFER_SCLK_DIV_64: SCLK/64
  *                     PM_BUFFER_SCLK_DIV_128: SCLK/128
  * ����  �ޡ�
  */
void
PM_Internal_Test_Buffer_Clock_Scaler_Config (uint32_t SclkScaler)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_BUFFER_SCLK_SCALER(SclkScaler));

    /*---------------- ����PM_CTL1�Ĵ���BUFCLKDIVλ ----------------*/
    PM_CTL1 = SFR_Config (PM_CTL1, ~PM_CTL1_BUFCLKDIV, SclkScaler);
}

/**
  * ����  ����PLL0ʱ�������BUFFERʹ�ܡ�
  * ����  NewState: PLL0ʱ�������BUFFERʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_PLL0_Output_Buffer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���PLL0OEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PLL0ʱ�������BUFFER */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_PLL0OE_POS);
    }
    else
    {
        /* ��ֹPLL0ʱ�������BUFFER */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_PLL0OE_POS);
    }
}

/**
  * ����  ����PLL1ʱ�������BUFFERʹ�ܡ�
  * ����  NewState: PLL1ʱ�������BUFFERʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_PLL1_Output_Buffer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���PLL1OEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PLL1ʱ�������BUFFER */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_PLL1OE_POS);
    }
    else
    {
        /* ��ֹPLL1ʱ�������BUFFER */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_PLL1OE_POS);
    }
}

/**
  * ����  ����PLL2ʱ�������BUFFERʹ�ܡ�
  * ����  NewState: PLL2ʱ�������BUFFERʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_PLL2_Output_Buffer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���PLL2OEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PLL2ʱ�������BUFFER */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_PLL2OE_POS);
    }
    else
    {
        /* ��ֹPLL2ʱ�������BUFFER */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_PLL2OE_POS);
    }
}

/**
  * ����  ����PLL0_LDO�����BUFFERʹ�ܡ�
  * ����  NewState: PLL0_LDO�����BUFFERʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_PLL0LDO_Output_Buffer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���PLL0LDOOEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PLL0_LDOʱ�������BUFFER */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_PLL0LDOOE_POS);
    }
    else
    {
        /* ��ֹPLL0_LDOʱ�������BUFFER */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_PLL0LDOOE_POS);
    }
}

/**
  * ����  ����PLL1_LDO�����BUFFERʹ�ܡ�
  * ����  NewState: PLL1_LDO�����BUFFERʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_PLL1LDO_Output_Buffer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���PLL1LDOOEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PLL1_LDOʱ�������BUFFER */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_PLL1LDOOE_POS);
    }
    else
    {
        /* ��ֹPLL1_LDOʱ�������BUFFER */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_PLL1LDOOE_POS);
    }
}

/**
  * ����  ����PLL2_LDO�����BUFFERʹ�ܡ�
  * ����  NewState: PLL2_LDO�����BUFFERʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_PLL2LDO_Output_Buffer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���PLL2LDOOEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PLL2_LDOʱ�������BUFFER */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_PLL2LDOOE_POS);
    }
    else
    {
        /* ��ֹPLL2_LDOʱ�������BUFFER */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_PLL2LDOOE_POS);
    }
}

/**
  * ����  ����BAT_BOR��ѹ��ѡ��
  * ����  Voltage: BAT_BOR��ѹ��ѡ��ȡֵΪ:
  *                  PM_BATTERY_VOLTAGE_1P6V: 1.6V
  *                  PM_BATTERY_VOLTAGE_1P8V: 1.8V
  *                  PM_BATTERY_VOLTAGE_2P06V: 2.06V
  *                  PM_BATTERY_VOLTAGE_2P4V: 2.4V
  *                  PM_BATTERY_VOLTAGE_2P88V: 2.88V
  *                  PM_BATTERY_VOLTAGE_3P6V: 3.6V
  * ����  �ޡ�
  */
void
PM_Battery_BOR_Config (uint32_t Voltage)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_BATTERY_VOLTAGE(Voltage));

    /*---------------- ����PM_CTL1�Ĵ���BATBORSELλ ----------------*/
    PM_CTL1 = SFR_Config (PM_CTL1, ~PM_CTL1_BATBORSEL, Voltage);
}

/**
  * ����  ����BAT_BORʹ�ܡ�
  * ����  NewState: BAT_BORʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Battery_BOR_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���BATBORENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��BAT_BOR */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_BATBOREN_POS);
    }
    else
    {
        /* ��ֹBAT_BOR */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_BATBOREN_POS);
    }
}

/**
  * ����  ����PVM1ʹ�ܡ�
  * ����  NewState: PVM1ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Peripheral_Voltage_Monitoring_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���VREFENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܲο���ѹģ�� */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_VREFEN_POS);
    }
    else
    {
        /* ��ֹ�ο���ѹģ�� */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_VREFEN_POS);
    }
}

/**
  * ����  ���õ�ѹ����ѡ��
  * ����  Voltage: ��ѹ����ѡ��ȡֵΪ:
  *                  PM_VOLTAGE_DETECTION_2P1V: 2.1V
  *                  PM_VOLTAGE_DETECTION_2P25V: 2.25V
  *                  PM_VOLTAGE_DETECTION_2P4V: 2.4V
  *                  PM_VOLTAGE_DETECTION_2P55V: 2.55V
  *                  PM_VOLTAGE_DETECTION_2P7V: 2.7V
  *                  PM_VOLTAGE_DETECTION_2P85V: 2.85V
  *                  PM_VOLTAGE_DETECTION_2P95V: 2.95V
  * ����  �ޡ�
  */
void
PM_Voltage_Detection_Config (uint32_t Voltage)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_VOLTAGE_DETECTION(Voltage));

    /*---------------- ����PM_CTL1�Ĵ���PVDSλ ----------------*/
    PM_CTL1 = SFR_Config (PM_CTL1, ~PM_CTL1_PVDS, Voltage);
}

/**
  * ����  ���õ�ѹ��⹦��ʹ�ܡ�
  * ����  NewState: ��ѹ��⹦��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Voltage_Detection_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���PVDENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܵ�ѹ��⹦�� */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_PVDEN_POS);
    }
    else
    {
        /* ��ֹ��ѹ��⹦�� */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_PVDEN_POS);
    }
}

/**
  * ����  �����ⲿ��������WKPxʹ�ܡ�
  * ����  PinSel: �������ţ�ȡֵΪ��
  *                 PM_PIN_WKP1: ����WKP1
  *                 PM_PIN_WKP2: ����WKP2
  *                 PM_PIN_WKP3: ����WKP3
  *                 PM_PIN_WKP4: ����WKP4
  *                 PM_PIN_WKP5: ����WKP5
  *       NewState: �ⲿ��������WKPxʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_External_Wakeup_Pin_Enable (uint32_t PinSel, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_PIN_WKP(PinSel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL2�Ĵ���WKPxENλ ----------------*/
    tmpreg = PM_CTL2_WKP1EN << PinSel;
    if (NewState != FALSE)
    {
        /* ʹ��WKPx��ƽ�仯���� */
        PM_CTL2 |= tmpreg;
    }
    else
    {
        /* ��ֹWKPx��ƽ�仯���� */
        PM_CTL2 &= ~tmpreg;
    }
}

/**
  * ����  �����ⲿ��������WKPx���Ѵ����ء�
  * ����  PinSel: �������ţ�ȡֵΪ��
  *                 PM_PIN_WKP1: ����WKP1
  *                 PM_PIN_WKP2: ����WKP2
  *                 PM_PIN_WKP3: ����WKP3
  *                 PM_PIN_WKP4: ����WKP4
  *                 PM_PIN_WKP5: ����WKP5
  *       TriggerEdge: �ⲿ��������WKPx���Ѵ����أ�ȡֵΪ:
  *                      PM_TRIGGER_RISE_EDGE: WKPx�����ش���
  *                      PM_TRIGGER_FALL_EDGE: WKPx�½��ش���
  * ����  �ޡ�
  */
void
PM_External_Wakeup_Edge_Config (uint32_t PinSel, uint32_t TriggerEdge)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_PIN_WKP(PinSel));
    CHECK_RESTRICTION(CHECK_PM_TRIGGER_EDGE(TriggerEdge));

    /*---------------- ����PM_CTL2�Ĵ���WKP5Pλ ----------------*/
    tmpreg = PM_CTL2_WKP1P << PinSel;
    if (TriggerEdge != PM_TRIGGER_RISE_EDGE)
    {
        /* WKPx�½��ش��� */
        PM_CTL2 |= tmpreg;
    }
    else
    {
        /* WKPx�����ش��� */
        PM_CTL2 &= ~tmpreg;
    }
}

/**
  * ����  ���ò�ͬ���蹤����Stopģʽ���ڲ���Ƶʱ��ʹ�ܡ�
  * ����  Peripheral: ����ѡ��ȡֵΪ��
  *                     PM_PERIPHERAL_CAN0: CAN0
  *                     PM_PERIPHERAL_CCP: CCP
  *                     PM_PERIPHERAL_LCD: LCD
  *                     PM_PERIPHERAL_UART0: UART0
  *       NewState: �ڲ���Ƶʱ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Stop_Mode_Peripheral_INLF_Enable (uint32_t Peripheral,
                    FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_PERIPHERAL(Peripheral));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL2�Ĵ���xxxxCLKLPENλ ----------------*/
    tmpreg = PM_CTL2_CAN0CLKLPEN << Peripheral;
    if (NewState != FALSE)
    {
        /* �����ڲ���Ƶʱ����Ϊ���蹤��ʱ�� */
        PM_CTL2 |= tmpreg;
    }
    else
    {
        /* ��ֹʱ�� */
        PM_CTL2 &= ~tmpreg;
    }
}

/**
  * ����  ���ò�ͬ���踴λ��
  * ����  Peripheral: ����ѡ��ȡֵΪ��
  *                     PM_PERIPHERAL_CAN0: CAN0
  *                     PM_PERIPHERAL_CCP: CCP
  *                     PM_PERIPHERAL_LCD: LCD
  *                     PM_PERIPHERAL_UART0: UART0
  *       ResetStatus: ���踴λ״̬��ȡֵΪ:
  *                      PERIPHERAL_RST_STATUS: ���账�ڸ�λ״̬
  *                      PERIPHERAL_OUTRST_STATUS: �����˳���λ
  * ����  �ޡ�
  */
void
PM_Peripheral_Reset_Config (uint32_t Peripheral, uint32_t ResetStatus)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_PERIPHERAL(Peripheral));
    CHECK_RESTRICTION(CHECK_PERIPHERAL_RST(ResetStatus));

    /*---------------- ����PM_CTL2�Ĵ���xxxxLPENλ ----------------*/
    tmpreg = PM_CTL2_CAN0LPEN << Peripheral;
    if (ResetStatus != PERIPHERAL_RST_STATUS)
    {
        /* ģ���˳���λ */
        PM_CTL2 |= tmpreg;
    }
    else
    {
        /* ģ�鴦�ڸ�λ״̬ */
        PM_CTL2 &= ~tmpreg;
    }
}

#if _CLOSE_SERVICE_
/**
  * ����  ��������ԴPORǿ��ʹ�ܡ�
  * ����  NewState: ����ԴPORǿ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Vdd_Por_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���VDDPORONλ ----------------*/
    if (NewState != FALSE)
    {
        /*  shutdownģʽ��ǿ��ʹ��VDD_POR */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_VDDPORON_POS);
    }
    else
    {
        /* shutdownģʽ��Ӳ�����Զ��ر�VDD_POR */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_VDDPORON_POS);
    }
}

/**
  * ����  ����LP_BG���ʹ�ܡ�
  * ����  NewState: LP_BG���ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Low_Power_Bandgap_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL0�Ĵ���LPBGONλ ----------------*/
    if (NewState != FALSE)
    {
        /*  ���ʹ��LP_BG */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_LPBGON_POS);
    }
    else
    {
        /* �����ʹ��LP_BG��BOR��LPR��PVD��LCDpumpʹ��ʱLP_BG���Զ�ʹ�� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_LPBGON_POS);
    }
}

/**
  * ����  ���ù���ģʽʱ��ѡ��
  * ����  Mode: ����ģʽʱ��ѡ��ȡֵΪ:
  *               PM_POWER_DISSIPATION_SCHEME1: Scheme1����������ʱ��
  *               PM_POWER_DISSIPATION_SCHEME2: Scheme2��ʱ��ο�4003���ϸ���
  * ����  �ޡ�
  */
void
PM_Power_Dissipation_Mode_Config (uint32_t Mode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_POWER_DISSIPATION(Mode));

    /*---------------- ����PM_CTL0�Ĵ���PDMSELλ ----------------*/
    if (Mode != PM_POWER_DISSIPATION_SCHEME1)
    {
        /* Scheme2��ʱ��ο�4003���ϸ��� */
        SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_PDMSEL_POS);
    }
    else
    {
        /* Scheme1����������ʱ�� */
        SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_PDMSEL_POS);
    }
}

/**
  * ����  ����POR12�رյ�LDO�رյ���ʱʱ�䡣
  * ����  DelayTime: POR12�رյ�LDO�رյ���ʱʱ�����ã�ȡֵΪ:
  *                    PM_POWER_DISSIPATION_500ns: 500ns
  *                    PM_POWER_DISSIPATION_500ns_1T: 500ns+1* T(INTLFOSC)
  *                    PM_POWER_DISSIPATION_500ns_2P5T: 500ns+2.5* T(INTLFOSC)
  * ����  �ޡ�
  */
void
PM_Power_Dissipation_Mode_Delay_Config (uint32_t DelayTime)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_POWER_DISSIPATION_DELAY(DelayTime));

    /*---------------- ����PM_CTL0�Ĵ���PDMDELAYλ ----------------*/
    PM_CTL0 = SFR_Config (PM_CTL0, ~PM_CTL0_PDMDELAY, DelayTime);
}

/**
  * ����  �����ڲ��������BUFFERʹ�ܡ�
  * ����  NewState: �ڲ��������BUFFERʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Internal_Test_Buffer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CTL1�Ĵ���INTBUFENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ���ڲ��������BUFFER */
        SFR_SET_BIT_ASM(PM_CTL1, PM_CTL1_INTBUFEN_POS);
    }
    else
    {
        /* ��ֹ�ڲ��������BUFFER */
        SFR_CLR_BIT_ASM(PM_CTL1, PM_CTL1_INTBUFEN_POS);
    }
}
#endif /* _CLOSE_SERVICE_ */
/**
  *   ##### ��Դ����(PM)���ƹ��ܺ���������� #####
  */


/**
  *   ##### ��Դ����(PM)״̬��־������ #####
  */
/**
  * ����  ���㸴λ/����״̬��־λ��
  * ����  EventSel: ��λ/�����¼���ȡֵΪ��
  *                   PM_RESET_IWDT: �������Ź���λ�¼�
  *                   PM_WAKEUP_RTC_ALARM: RTC���ӻ����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN: �ⲿ�������Ż����¼�
  *                   PM_RESET_POR: POR��λ�¼�
  *                   PM_RESET_BOR: BOR��λ�¼�
  *                   PM_RESET_SOFTWARE: �����λ�¼�
  * ����  �ޡ�
  */
void
PM_Clear_Reset_And_Wakeup_Flag (uint32_t EventSel)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CLEAR_STA0_RESET_WAKEUP(EventSel));
    tmpmask = (uint32_t)1 << EventSel;
    /*---------------- ����PM_STA0�Ĵ���RSTFCλ ----------------*/
    SFR_SET_BIT_ASM(PM_STA0, PM_STA0_RSTFC_POS);
    while(PM_STA0 & tmpmask);
    SFR_CLR_BIT_ASM(PM_STA0, PM_STA0_RSTFC_POS);
}

/**
  * ����  ��ȡ��Ӧ��λ/�����¼���״̬��־��
  * ����  EventSel: ��λ/�����¼���ȡֵΪ��
  *                   PM_RESET_IWDT: �������Ź���λ�¼�
  *                   PM_WAKEUP_RTC_ALARM: RTC���ӻ����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN: �ⲿ�������Ż����¼�
  *                   PM_RESET_POR: POR��λ�¼�
  *                   PM_RESET_BOR: BOR��λ�¼�
  *                   PM_RESET_SOFTWARE: �����λ�¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP5: WKP5���Ż����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP4: WKP4���Ż����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP3: WKP3���Ż����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP2: WKP2���Ż����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP1: WKP1���Ż����¼�
  * ����  1: �����˸�λ/�����¼���
  *       0: δ������λ/�����¼���
  */
FlagStatus
PM_Get_IWDT_Reset_Flag (uint32_t EventSel)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_RESET_WAKEUP(EventSel));

    if (EventSel < 32)
    {
        tmpreg = PM_STA0;
        tmpmask = (uint32_t)1 << EventSel;
    }
    else
    {
        tmpreg = PM_STA1;
        tmpmask = (uint32_t)1 << (EventSel - 32);
    }

    /*---------------- ��ȡPM_STAx�Ĵ�����Ӧλ ----------------*/
    if (tmpreg & tmpmask)
    {
        /* �����˸�λ/�����¼� */
        return SET;
    }
    else
    {
        /* δ������λ/�����¼� */
        return RESET;
    }
}

/**
  * ����  �����ⲿ��������WKPx״̬��־��
  * ����  EventSel: ��λ/�����¼���ȡֵΪ��
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP5: WKP5���Ż����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP4: WKP4���Ż����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP3: WKP3���Ż����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP2: WKP2���Ż����¼�
  *                   PM_WAKEUP_EXTERNAL_PIN_WKP1: WKP1���Ż����¼�
  * ����  �ޡ�
  */
void
PM_Clear_External_Wakeup_Pin_Flag (uint32_t EventSel)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CLEAR_STA1_WAKEUP(EventSel));
    tmpmask = (uint32_t)1 << (EventSel - 32);
    /*---------------- ����PM_STAC�Ĵ���WKPxSCλ ----------------*/
    PM_STAC |= tmpmask;
    while(PM_STA1 & tmpmask);
    PM_STAC &= ~tmpmask;
}

/**
  * ����  ��ȡ�͹�������ģʽ״̬��־��
  * ����  �ޡ�
  * ����  1: ��ѹ�������л���LPR��0: ����ѹ������MR��ʹ����ɡ�
  */
FlagStatus
PM_Get_Low_Power_Running_State (void)
{
    /*---------------- ��ȡPM_STA0�Ĵ���LPRUNFλ ----------------*/
    if (PM_STA0 & PM_STA0_LPRUNF)
    {
        /* ��ѹ�������л���LPR */
        return SET;
    }
    else
    {
        /* ����ѹ������MR��ʹ����� */
        return RESET;
    }
}

/**
  * ����  ��ȡ�͹��ĵ�ѹ������LPR״̬��
  * ����  �ޡ�
  * ����  1: �͹��ĵ�ѹ������LPR��ʹ����ɣ�
  *       0: �͹��ĵ�ѹ������LPRδʹ����ɡ�
  */
FlagStatus
PM_Get_LPR_Status (void)
{
    /*---------------- ��ȡPM_STA0�Ĵ���LPRSTAλ ----------------*/
    if (PM_STA0 & PM_STA0_LPRSTA)
    {
        /* �͹��ĵ�ѹ������LPR��ʹ����� */
        return SET;
    }
    else
    {
        /* �͹��ĵ�ѹ������LPRδʹ����� */
        return RESET;
    }
}

/**
  * ����  ��ȡ��ѹ���״̬��
  * ����  �ޡ�
  * ����  1: VDD��ѹ����PVD��ѹ�㣻
  *       0: VDD��ѹ����PVD��ѹ�㡣
  */
FlagStatus
PM_Get_Peripheral_Voltage_Detection_Status (void)
{
    /*---------------- ��ȡPM_STA1�Ĵ���PVDSTAλ ----------------*/
    if (PM_STA1 & PM_STA1_PVDSTA)
    {
        /* VDD��ѹ����PVD��ѹ�� */
        return SET;
    }
    else
    {
        /* VDD��ѹ����PVD��ѹ�� */
        return RESET;
    }
}
/**
  *   ##### ��Դ����(PM)״̬��־������������� #####
  */


/**
  *   ##### ��Դ����(PM)У׼���ƺ��� #####
  */
/**
  * ����  ��������Ư������У׼��
  * ����  Calibration: ����Ư������У׼��ȡֵΪ:
  *                      PM_CURRENT_OUTPUT_2uA: �������2uA
  *                      PM_CURRENT_REDUCE_10_PERCENT: ������С��10%
  *                      PM_CURRENT_INCREASE_10_PERCENT: ��������10%
  *                      PM_CURRENT_INCREASE_20_PERCENT: ��������20%
  * ����  �ޡ�
  */
void
PM_Zero_Drift_Current_Config (uint32_t Calibration)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_CURRENT_CONFIG(Calibration));

    /*---------------- ����PM_CAL0�Ĵ���ZEROTISTRIMλ ----------------*/
    PM_CAL0 = SFR_Config (PM_CAL0, ~PM_CAL0_ZEROTISTRIM, Calibration);
}

/**
  * ����  ����BOR��ѹ��ѡ��
  * ����  Voltage: BOR��ѹ��ѡ��ȡֵΪ:
  *                  PM_BOR_VOLTAGE_2P05V: BOR��ѹ��Ϊ2.05V
  *                  PM_BOR_VOLTAGE_2P25V: BOR��ѹ��Ϊ2.25V
  *                  PM_BOR_VOLTAGE_2P55V: BOR��ѹ��Ϊ2.55V
  *                  PM_BOR_VOLTAGE_2P85V: BOR��ѹ��Ϊ2.85V
  * ����  �ޡ�
  */
void
PM_BOR_Voltage_Config (uint32_t Voltage)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_BOR_VOLTAGE(Voltage));

    /*---------------- ����PM_CAL0�Ĵ���BORSELλ ----------------*/
    PM_CAL0 = SFR_Config (PM_CAL0, ~PM_CAL0_BORSEL, Voltage);
}

/**
  * ����  ����MR��MR_HVģ��У׼��
  * ����  MRSel: MR��MR_HVģ��ѡ��ȡֵΪ��
  *                PM_MR_MODULE: MRģ��
  *                PM_MR_HV_MODULE: MR_HVģ��
  *       Voltage: У׼��ѹѡ��ȡֵΪ:
  *                  PM_MR_VOLTAGE_1P2V: BOR��ѹ��Ϊ1.2V
  *                  PM_MR_VOLTAGE_0P9V: BOR��ѹ��Ϊ0.9V
  *                  PM_MR_VOLTAGE_1V: BOR��ѹ��Ϊ1V
  *                  PM_MR_VOLTAGE_1P32V: BOR��ѹ��Ϊ1.32V
  * ����  �ޡ�
  */
void
PM_Main_Regulator_Voltage_Config (uint32_t MRSel, uint32_t Voltage)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_MR_MODULE(MRSel));
    CHECK_RESTRICTION(CHECK_PM_MR_VOLTAGE(Voltage));

    /*------------- ����PM_CAL0�Ĵ���MRTRIM��MRHVTRIMλ -------------*/
    tmpreg = Voltage << MRSel;
    tmpmask = (uint32_t)0xF << MRSel;
    PM_CAL0 = SFR_Config (PM_CAL0, ~tmpmask, tmpreg);
}

/**
  * ����  ����MR_HVģ�����ʹ�ܡ�
  * ����  NewState: MR_HVģ�����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
PM_Main_Regulator_HV_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����PM_CAL0�Ĵ���MRHVENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��MR_HVģ�� */
        SFR_SET_BIT_ASM(PM_CAL0, PM_CAL0_MRHVEN_POS);
    }
    else
    {
        /* ��ֹMR_HVģ�� */
        SFR_CLR_BIT_ASM(PM_CAL0, PM_CAL0_MRHVEN_POS);
    }
}

/**
  * ����  ���ò�ͬ�Ļ�׼У׼��
  * ����  Reference: ��ͬ�Ļ�׼ѡ��ȡֵΪ��
  *                    PM_REFERENCE_BUFFER: �ο�BUFFER��׼У׼
  *                    PM_REFERENCE_LDO12: LDO12��׼У׼
  *                    PM_REFERENCE_LDO18: LDO18��׼У׼
  *       Calibration: У׼��ѹѡ��
  *                    PM_REFERENCE_BUFFERȡֵ��ΧΪ0~0x1F,
  *                    PM_REFERENCE_LDO12ȡֵ��ΧΪ0~0xF,
  *                    PM_REFERENCE_LDO18ȡֵ��ΧΪ0~0xF��
  * ����  �ޡ�
  */
void
PM_Reference_Calibration_Config (uint32_t Reference, uint32_t Calibration)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* ����У�� */
    switch (Reference)
    {
        case PM_REFERENCE_BUFFER:
            CHECK_RESTRICTION(CHECK_PM_BUFFER_CALIBRATION(Calibration));
            tmpreg = Calibration << PM_REFERENCE_BUFFER;
            tmpmask = PM_CAL0_BGTRIMREBUF;
            break;
        case PM_REFERENCE_LDO12:
            CHECK_RESTRICTION(CHECK_PM_LDO12_CALIBRATION(Calibration));
            tmpreg = Calibration << PM_REFERENCE_LDO12;
            tmpmask = PM_CAL0_BGTRIMMRLDO;
            break;
        case PM_REFERENCE_LDO18:
            CHECK_RESTRICTION(CHECK_PM_LDO18_CALIBRATION(Calibration));
            tmpreg = Calibration << PM_REFERENCE_LDO18;
            tmpmask = PM_CAL0_BGTRIMFLLDO;
            break;
        default:
            CHECK_RESTRICTION(0);
            break;
    }

    /*------------- ����PM_CAL0�Ĵ�����Ӧ��У׼λ�� -------------*/
    PM_CAL0 = SFR_Config (PM_CAL0, ~tmpmask, tmpreg);
}

/**
  * ����  �����ڲ���Ƶ��������У׼��
  * ����  Calibration: ƫ�õ���ѡ��ȡֵΪ��
  *                      PM_INTLF_BIAS_CURRENT_10nA: 10nA
  *                      PM_INTLF_BIAS_CURRENT_12P5nA: 12.5nA
  *                      PM_INTLF_BIAS_CURRENT_15nA: 15nA
  *                      PM_INTLF_BIAS_CURRENT_17P5nA: 17.5nA
  *                      PM_INTLF_BIAS_CURRENT_0nA: 0nA
  *                      PM_INTLF_BIAS_CURRENT_2P5nA: 2.5nA
  *                      PM_INTLF_BIAS_CURRENT_5nA: 5nA
  *                      PM_INTLF_BIAS_CURRENT_7P5nA: 7.5nA
  * ����  �ޡ�
  */
void
PM_INTLF_Bias_Current_Config (uint32_t Calibration)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_INTLF_BIAS_CURRENT(Calibration));

    /*------------- ����PM_CAL1�Ĵ���LSITRIMBIAS�� -------------*/
    PM_CAL1 = SFR_Config (PM_CAL1, ~PM_CAL1_LSITRIMBIAS, Calibration);
}

/**
  * ����  �����ⲿ��Ƶ��������У׼��
  * ����  Calibration: ƫ�õ���ѡ��ȡֵΪ��
  *                      PM_EXTLF_BIAS_CURRENT_20nA: 20nA
  *                      PM_EXTLF_BIAS_CURRENT_25nA: 25nA
  *                      PM_EXTLF_BIAS_CURRENT_30nA: 30nA
  *                      PM_EXTLF_BIAS_CURRENT_35nA: 35nA
  *                      PM_EXTLF_BIAS_CURRENT_0nA: 0nA
  *                      PM_EXTLF_BIAS_CURRENT_5nA: 5nA
  *                      PM_EXTLF_BIAS_CURRENT_10nA: 10nA
  *                      PM_EXTLF_BIAS_CURRENT_15nA: 15nA
  * ����  �ޡ�
  */
void
PM_EXTLF_Bias_Current_Config (uint32_t Calibration)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_EXTLF_BIAS_CURRENT(Calibration));

    /*------------- ����PM_CAL1�Ĵ���LSETRIMBIAS�� -------------*/
    PM_CAL1 = SFR_Config (PM_CAL1, ~PM_CAL1_LSETRIMBIAS, Calibration);
}

/**
  * ����  �����ڲ���Ƶ��������У׼��
  * ����  Calibration: У׼ֵ��ȡֵ��ΧΪ0~0x1F��
  * ����  �ޡ�
  */
void
PM_INTLF_Capacitance_Calibration_Config (uint32_t Calibration)
{
	uint32_t tmpreg=0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_CAPACITANCE_CALIBRATION(Calibration));

    /*------------- ����PM_CAL1�Ĵ���LSETRIMBIAS�� -------------*/
    tmpreg = Calibration << PM_CAL1_LSETRIMBIAS0_POS;
    PM_CAL1 = SFR_Config (PM_CAL1, ~PM_CAL1_LSETRIMBIAS, tmpreg);
}

/**
  * ����  ���õ͹���ƫ�õ���У׼��
  * ����  Calibration: У׼ֵ��ȡֵ��ΧΪ0~0x7��
  * ����  �ޡ�
  */
void
PM_LP_Bias_Calibration_Config (uint32_t Calibration)
{
	uint32_t tmpreg=0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_LP_BIAS_CALIBRATION(Calibration));

    /*------------- ����PM_CAL1�Ĵ���LPTRIMBIAS�� -------------*/
    tmpreg = Calibration << PM_CAL1_LPTRIMBIAS0_POS;
    PM_CAL1 = SFR_Config (PM_CAL1, ~PM_CAL1_LPTRIMBIAS, tmpreg);
}

/**
  * ����  �����ڲ���Ƶ��������У׼��
  * ����  Calibration: У׼ֵ��ȡֵ��ΧΪ0~0x7��
  * ����  �ޡ�
  */
void
PM_LPBG_Pump_Calibration_Config (uint32_t Calibration)
{
	uint32_t tmpreg=0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_PUMP_CALIBRATION(Calibration));

    /*------------- ����PM_CAL1�Ĵ���LPBGTRIMPUMP�� -------------*/
    tmpreg = Calibration << PM_CAL1_LPBGTRIMPUMP0_POS;
    PM_CAL1 = SFR_Config (PM_CAL1, ~PM_CAL1_LPBGTRIMPUMP, tmpreg);

}

/**
  * ����  �����ⲿ��Ƶ����N��ƫ��У׼��
  * ����  Calibration: ����ѡ��ȡֵΪ��
  *                      PM_BRANCH_CURRENT_NONE: û������֧·����
  *                      PM_BRANCH_CURRENT_50_PERCENT: ����50%֧·����
  *                      PM_BRANCH_CURRENT_150_PERCENT: ����150%֧·����
  *                      PM_BRANCH_CURRENT_200_PERCENT: ����200%֧·����
  * ����  �ޡ�
  */
void
PM_EXTLF_N_Bias_Current_Config (uint32_t Calibration)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_BRANCH_CURRENT(Calibration));

    /*------------- ����PM_CAL1�Ĵ���LSETRIMNBIAS�� -------------*/
    PM_CAL1 = SFR_Config (PM_CAL1, ~PM_CAL1_LSETRIMNBIAS, Calibration);
}
/**
  * ����  �����ⲿ��Ƶ��������ѡ��λ��
  * ����  PeripheralPort: �����ⲿ��Ƶ��������IO�ڣ�ȡֵΪ:
  *                         PM_EXTLF_PIN1_IO_PORT: ѡ���ⲿ��Ƶ����1
  *                         PM_EXTLF_PIN2_IO_PORT: ѡ���ⲿ��Ƶ����2
  * ����  �ޡ�
  */
void
PM_EXTLF_PIN_Selection_Config (uint32_t PeripheralPort)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_EXTLF_PIN_IO(PeripheralPort));

    /*---------------- ����PM_CTL1�Ĵ���LSESELλ ----------------*/
    if (PeripheralPort != PM_EXTLF_PIN1_IO_PORT)
    {
        /* ѡ���ⲿ��Ƶ����2*/
        SFR_SET_BIT_ASM(PM_CAL1, PM_CAL1_LSESEL_POS);
    }
    else
    {
        /* ѡ���ⲿ��Ƶ����1 */
        SFR_CLR_BIT_ASM(PM_CAL1, PM_CAL1_LSESEL_POS);
    }
}

/**
  * ����  �����ⲿ��Ƶ��������ѡ��λ��
  * ����  PeripheralPort: �����ⲿ��Ƶ��������IO�ڣ�ȡֵΪ:
  *                         PM_EXHLF_PIN1_IO_PORT: ѡ���ⲿ��Ƶ����1
  *                         PM_EXHLF_PIN2_IO_PORT: ѡ���ⲿ��Ƶ����2
  * ����  �ޡ�
  */
void
PM_EXTHF_PIN_Selection_Config (uint32_t PeripheralPort)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_EXTHF_PIN_IO(PeripheralPort));

    /*---------------- ����PM_CTL1�Ĵ���HSESELλ ----------------*/
    if (PeripheralPort != PM_EXTHF_PIN1_IO_PORT)
    {
        /* ѡ���ⲿ��Ƶ����2*/
        SFR_SET_BIT_ASM(PM_CAL1, PM_CAL1_HSESEL_POS);
    }
    else
    {
        /* ѡ���ⲿ��Ƶ����1 */
        SFR_CLR_BIT_ASM(PM_CAL1, PM_CAL1_HSESEL_POS);
    }
}







#if _CLOSE_SERVICE_
/**
  * ����  ����LDO18����ģ��ѡ��
  * ����  LDO18Config: LDO18����ģ��ѡ��ȡֵΪ:
  *                      PM_LDO18_CAP: LDO18ѡ��cap�ṹ
  *                      PM_LDO18_CAPLESS: LDO18ѡ��capless�ṹ
  * ����  �ޡ�
  */
void
PM_LDO18_Module_Config (uint32_t LDO18Config)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_LDO18_MODULE(LDO18Config));

    /*---------------- ����PM_CAL0�Ĵ���LDO18SELλ ----------------*/
    if (LDO18Config != PM_LDO18_CAP)
    {
        /* LDO18ѡ��capless�ṹ */
        SFR_SET_BIT_ASM(PM_CAL0, PM_CAL0_LDO18SEL_POS);
    }
    else
    {
        /* LDO18ѡ��cap�ṹ */
        SFR_CLR_BIT_ASM(PM_CAL0, PM_CAL0_LDO18SEL_POS);
    }
}

/**
  * ����  ����MR_BGģʽѡ��
  * ����  ModeSel: MR_BGģʽѡ��ȡֵΪ:
  *                  PM_LOW_POWER_MODE: �͹���ģʽ
  *                  PM_HIGH_POWER_MODE: �߹���ģʽ
  * ����  �ޡ�
  */
void
PM_Main_Regulator_Bandgap_Config (uint32_t ModeSel)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_MR_BG_MODE(ModeSel));

    /*---------------- ����PM_CAL0�Ĵ���MRBGSELλ ----------------*/
    if (ModeSel != PM_LOW_POWER_MODE)
    {
        /* �߹���ģʽ */
        SFR_SET_BIT_ASM(PM_CAL0, PM_CAL0_MRBGSEL_POS);
    }
    else
    {
        /* �͹���ģʽ */
        SFR_CLR_BIT_ASM(PM_CAL0, PM_CAL0_MRBGSEL_POS);
    }
}

/**
  * ����  ����LPR����ģ��ѡ��
  * ����  ModeSel: LPR����ģ��ѡ��ȡֵΪ:
  *                  PM_LPR_DEFAULT: �͹���ģʽ
  *                  PM_LPR_BACKUP: �߹���ģʽ
  * ����  �ޡ�
  */
void
PM_LPR_Module_Config (uint32_t ModeSel)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_PM_LPR_MODE(ModeSel));

    /*---------------- ����PM_CAL0�Ĵ���MRBGSELλ ----------------*/
    if (ModeSel != PM_LPR_DEFAULT)
    {
        /* �߹���ģʽ */
        SFR_SET_BIT_ASM(PM_CAL0, PM_CAL0_MRBGSEL_POS);
    }
    else
    {
        /* �͹���ģʽ */
        SFR_CLR_BIT_ASM(PM_CAL0, PM_CAL0_MRBGSEL_POS);
    }
}

#endif /* _CLOSE_SERVICE_ */
/**
  *   ##### ��Դ����(PM)У׼���ƺ���������� #####
  */

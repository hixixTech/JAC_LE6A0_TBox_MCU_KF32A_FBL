/**
  ******************************************************************************
  * �ļ���  kf32a_basic_dac.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��DAC���蹦�ܺ�����������
  *          + ��ʼ�������ú���
  *          + ���ݶ�д����
  *          + DACУ��Ĵ������ú���
  *
  *********************************************************************
  */

#include "kf32a_basic_dac.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* DAC˽�ж��� ----------------------------------------------------*/
/* DACx_CTL�Ĵ����������� */
#define DAC_CTL_INIT_MASK               (DAC_CTL_TRIGEN \
                                       | DAC_CTL_TREVT \
                                       | DAC_CTL_DE \
                                       | DAC_CTL_WAVE \
                                       | DAC_CTL_MAS)

/* DACx_CTL�Ĵ����������� */
#define DAC_CTL1_INIT_MASK              (DAC_CTL1_CLK \
                                       | DAC_CTL1_RFS \
                                       | DAC_CTL1_BUFEN \
                                       | DAC_CTL1_SEL)

/* DACx_DAHD�Ĵ����������� */
#define DAC_CAL_INIT_MASK               (DAC_CAL_PPOSITRIM \
                                       | DAC_CAL_PNEGATRIM \
                                       | DAC_CAL_NPOSITRIM \
                                       | DAC_CAL_NNEGATRIM \
                                       | DAC_CAL_BUFTRIM)


/**
  *   ##### ��ʼ�������ú��� #####
  */
/**
  * ����  DAC���踴λ��ʹ������ʱ�ӡ�
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR��
  * ����  �ޡ�
  */
void
DAC_Reset (DAC_SFRmap* DACx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));

#ifdef KF32A_Periph_dac0
    if (DACx == DAC0_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_DAC0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_DAC0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_DAC0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_dac1
    if (DACx == DAC1_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_DAC1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_DAC1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_DAC1CLKEN, TRUE);
    }
#endif
}

/**
  * ����  DAC�������á�
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR��
  *       dacInitStruct: DAC������Ϣ
  * ����  �ޡ�
  */
void
DAC_Configuration (DAC_SFRmap* DACx, DAC_InitTypeDef* dacInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dacInitStruct->m_TriggerEnable));
    CHECK_RESTRICTION(CHECK_DAC_TRIGGER_EVENT(dacInitStruct->m_TriggerEvent));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dacInitStruct->m_TriggerDMAEnable));
    CHECK_RESTRICTION(CHECK_DAC_WAVE(dacInitStruct->m_Wave));
    CHECK_RESTRICTION(CHECK_DAC_MAS(dacInitStruct->m_Mas));
    CHECK_RESTRICTION(CHECK_DAC_CLK(dacInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_DAC_CLK_DIV(dacInitStruct->m_ClockDiv));
    CHECK_RESTRICTION(CHECK_DAC_RFS(dacInitStruct->m_ReferenceVoltage));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dacInitStruct->m_OutputBuffer));
    CHECK_RESTRICTION(CHECK_DAC_OUTPUT_PIN(dacInitStruct->m_OutputPin));
    CHECK_RESTRICTION(CHECK_DAC_OUTPUT(dacInitStruct->m_Output));

    /*---------------------------- DACx_CTL�Ĵ�������  -----------------*/
    /* ���ݽṹ���Աm_TriggerEnable������TRIGENλ�� */
    /* ���ݽṹ���Աm_TriggerEvent������TREVTλ�� */
    /* ���ݽṹ���Աm_TriggerDMAEnable������DEλ�� */
    /* ���ݽṹ���Աm_Wave������WAVEλ�� */
    /* ���ݽṹ���Աm_Mas������MASλ�� */
    tmpreg = ((dacInitStruct->m_TriggerEnable << DAC_CTL_TRIGEN_POS) | \
              (dacInitStruct->m_TriggerEvent) | \
              (dacInitStruct->m_TriggerDMAEnable << DAC_CTL_DE_POS) | \
              (dacInitStruct->m_Wave) | \
              (dacInitStruct->m_Mas));
    DACx->CTL = SFR_Config (DACx->CTL, ~DAC_CTL_INIT_MASK, tmpreg);

    /*---------------------------- DACx_CTL1�Ĵ������� -----------------*/
    /* ���ݽṹ���Աm_ClockDiv������CLKDIVλ�� */
    /* ���ݽṹ���Աm_Clock������CLKλ�� */
    /* ���ݽṹ���Աm_ReferenceVoltage������RFSλ�� */
    /* ���ݽṹ���Աm_OutputBuffer������BUFENλ�� */
    /* ���ݽṹ���Աm_OutputPin������SELλ�� */
    tmpreg = (dacInitStruct->m_ClockDiv)\
    	   | (dacInitStruct->m_Clock) \
           | (dacInitStruct->m_ReferenceVoltage) \
           | (dacInitStruct->m_OutputBuffer << DAC_CTL1_BUFEN_POS) \
           | (dacInitStruct->m_OutputPin);
    DACx->CTL1 = SFR_Config (DACx->CTL1, ~DAC_CTL1_INIT_MASK, tmpreg);

    /*---------------------------- DACx_DAHD�Ĵ������� -----------------*/
    /* ���ݽṹ���Աm_Output������DAHDλ�� */
    tmpreg = dacInitStruct->m_Output;
    DACx->DAHD = SFR_Config (DACx->DAHD, ~DAC_DAHD_DAHD, tmpreg);
}

/**
  * ����  ��ʼ��DAC������Ϣ�ṹ�塣
  * ����  dacInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
DAC_Struct_Init (DAC_InitTypeDef* dacInitStruct)
{
    /* ��ʼ�� DACͨ������ʹ�� */
    dacInitStruct->m_TriggerEnable = FALSE;
    /* ��ʼ�� DACͨ�������¼� */
    dacInitStruct->m_TriggerEvent = DAC_TRIGGER_T1_TRGO;
    /* ��ʼ�� DAC����DMAʹ�� */
    dacInitStruct->m_TriggerDMAEnable = FALSE;
    /* ��ʼ�� DAC���η�����ʹ�� */
    dacInitStruct->m_Wave = DAC_WAVE_NONE;
    /* ��ʼ�� DAC����/��ֵѡ���� */
    dacInitStruct->m_Mas = DAC_LFSR_UNMASK_BITS0_0;
    /* ��ʼ�� DAC����ʱ�� */
    dacInitStruct->m_Clock = DAC_CLK_SCLK;
    /* ��ʼ�� DACʱ�ӷ�Ƶ */
    dacInitStruct->m_ClockDiv = DAC_CLK_DIV_1;
    /* ��ʼ�� DAC�ο���ѹѡ�� */
    dacInitStruct->m_ReferenceVoltage = DAC_RFS_AVDD;
    /* ��ʼ�� DAC�������ʹ�� */
    dacInitStruct->m_OutputBuffer = FALSE;
    /* ��ʼ�� DAC������� */
    dacInitStruct->m_OutputPin = DAC_OUTPUT_PIN_0;
    /* ��ʼ�� DAC������� */
    dacInitStruct->m_Output = 0;
}

/**
  * ����  ����DACʹ��λ��
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR��
  *       NewState: DACʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
DAC_Cmd (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ʹ��DAC */
        SFR_SET_BIT_ASM(DACx->CTL, DAC_CTL_EN_POS);
    }
    else
    {
        /* ��ֹDAC */
        SFR_CLR_BIT_ASM(DACx->CTL, DAC_CTL_EN_POS);
    }
}

/**
  * ����  ����DACʹ��λ��
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR��
  *       NewState: DACʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
DAC_Software_Trigger_Cmd (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ʹ��DAC������� */
        SFR_SET_BIT_ASM(DACx->CTL, DAC_CTL_SWTRIG_POS);
    }
    else
    {
        /* ��ֹDAC������� */
        SFR_CLR_BIT_ASM(DACx->CTL, DAC_CTL_SWTRIG_POS);
    }
}

/**
  * ����  ����DACͨ��DMAģʽʹ�ܡ�
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR��
  *       NewState: DACʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
DAC_DMA_Cmd (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ʹ��DACͨ��DMAģʽ */
        SFR_SET_BIT_ASM(DACx->CTL, DAC_CTL_DE_POS);
    }
    else
    {
        /* ��ֹDACͨ��DMAģʽ */
        SFR_CLR_BIT_ASM(DACx->CTL, DAC_CTL_DE_POS);
    }
}

/**
  * ����  ����DAC�������ʹ�ܡ�
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR��
  *       NewState: DACʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE���ֱ��ʾ���������򲻴����塣
  * ����  �ޡ�
  */
void
DAC_Output_Buffer_Config (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ʹ��DAC������� */
        SFR_SET_BIT_ASM(DACx->CTL1, DAC_CTL1_BUFEN_POS);
    }
    else
    {
        /* ��ֹDAC������� */
        SFR_CLR_BIT_ASM(DACx->CTL1, DAC_CTL1_BUFEN_POS);
    }
}

/**
  * ����  ����DAC���IO��ѡ��
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR��
  *       NewState: DACʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE���ֱ��ʾ�����IO��1���������IO��0��
  * ����  �ޡ�
  */
void
DAC_Output_Port_Config (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* DAC�����IO��1 */
        SFR_SET_BIT_ASM(DACx->CTL1, DAC_CTL1_SEL_POS);
    }
    else
    {
        /* DAC�����IO��0 */
        SFR_CLR_BIT_ASM(DACx->CTL1, DAC_CTL1_SEL_POS);
    }
}

/**
  *   ##### ��ʼ�������ú������� #####
  */



/**
  *   ##### ���ݶ�д���� #####
  */
/**
  * ����  дDAC������ݣ�ֻ��д��DACx_DAHD�Ĵ�����
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR��
  *       Value: д�����ݼĴ�����ֵ��
  *              ȡֵΪ12λ���ݡ�
  * ����  �ޡ�
  */
void
DAC_Write_Output_Data (DAC_SFRmap* DACx, uint32_t Value)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_DAC_OUTPUT(Value));

    DACx->DAHD = Value;
}

/**
  * ����  ��DAC������ݡ�
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR����
  * ����  32λ�������ݵ�12λ���ݡ�
  */
uint32_t
DAC_Read_Output_Data (DAC_SFRmap* DACx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));

    return (DAC_DAHD_DAHD && DACx->DATA);
}

/**
  *   ##### ���ݶ�д�������� #####
  */



/**
  *   ##### DACУ��Ĵ������ú��� #####
  */
/**
  * ����  DACУ��Ĵ������ú�����
  * ����  DACx: ָ��DAC�ڴ�ṹ��ָ�룬ȡֵΪDAC0_SFR��DAC1_SFR����
  *       dacCalStruct: DACУ��Ĵ���������Ϣ
  * ����  32λ�������ݵ�12λ���ݡ�
  */
void
DAC_Calibration_Config (DAC_SFRmap* DACx, DAC_CALTypeDef* dacCalStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_DAC_CAL(dacCalStruct->m_PPositrim));
    CHECK_RESTRICTION(CHECK_DAC_CAL(dacCalStruct->m_PNegatrim));
    CHECK_RESTRICTION(CHECK_DAC_CAL(dacCalStruct->m_NPositrim));
    CHECK_RESTRICTION(CHECK_DAC_CAL(dacCalStruct->m_NNegatrim));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dacCalStruct->m_Buftrim));

    /*---------------------------- DACx_CAL�Ĵ�������  -----------------*/
    /* ���ݽṹ���Աm_PPositrim������PPOSITRIMλ�� */
    /* ���ݽṹ���Աm_PNegatrim������PNEGATRIMλ�� */
    /* ���ݽṹ���Աm_NPositrim������NPOSITRIMλ�� */
    /* ���ݽṹ���Աm_NNegatrim������NNEGATRIMλ�� */
    tmpreg = (dacCalStruct->m_PPositrim << DAC_CAL_PPOSITRIM0_POS) | \
             (dacCalStruct->m_PNegatrim << DAC_CAL_PNEGATRIM0_POS) | \
             (dacCalStruct->m_NPositrim << DAC_CAL_NPOSITRIM0_POS) | \
             (dacCalStruct->m_NNegatrim << DAC_CAL_NNEGATRIM0_POS) | \
             (dacCalStruct->m_Buftrim << DAC_CAL_BUFTRIM_POS);
    DACx->CAL = SFR_Config (DACx->CAL, ~DAC_CAL_INIT_MASK, tmpreg);
}

/**
  * ����  ��ʼ��DAC������Ϣ�ṹ�塣
  * ����  dacCalStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
DAC_Calibration_Struct_Init (DAC_CALTypeDef* dacCalStruct)
{
    /* ��ʼ�� DACģ��BUFFER����P����ʧ������У׼λ */
    dacCalStruct->m_PPositrim = DAC_CAL_0;
    /* ��ʼ�� DACģ��BUFFER����P�ܸ�ʧ������У׼λ */
    dacCalStruct->m_PNegatrim = DAC_CAL_0;
    /* ��ʼ�� DACģ��BUFFER����N����ʧ������У׼λ */
    dacCalStruct->m_NPositrim = DAC_CAL_0;
    /* ��ʼ�� DACģ��BUFFER����N�ܸ�ʧ������У׼λ */
    dacCalStruct->m_NNegatrim = DAC_CAL_0;
    /* ��ʼ�� DACģ��BUFFERʧ��У׼ʹ��λ */
    dacCalStruct->m_Buftrim = FALSE;
}

/**
  *   ##### DACУ��Ĵ������ú������� #####
  */

/**
  ******************************************************************************
  * �ļ���  kf32a_basic_lcd.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��Һ����ʾ(LCD)��صĹ��ܺ�����������
  *          + Һ����ʾ(LCD)��ʼ������
  *          + Һ����ʾ(LCD)�������ú���
  *          + Һ����ʾ(LCD)�жϹ�����
  *
  *********************************************************************
  */

#include "kf32a_basic_lcd.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_lcd
/* ��ʱ��˽�ж��� ----------------------------------------------------*/
/* LCD_CTL�Ĵ�����ʼ������ */
#define LCD_CTL_INIT_MASK               (LCD_CTL_LCDENANALOG \
                                       | LCD_CTL_LMUX \
                                       | LCD_CTL_FR \
                                       | LCD_CTL_VLCDMD \
                                       | LCD_CTL_CS \
                                       | LCD_CTL_LP)

/**
  *   ##### Һ����ʾ(LCD)��ʼ������ #####
  */
/**
  * ����  ��λLCD���裬ʹ������ʱ�ӡ�
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
LCD_Reset (void)
{
    RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_LCDRST, TRUE);
    RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_LCDRST, FALSE);
    PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_LCDCLKEN, TRUE);
}

/**
  * ����  LCDҺ����ʾ��ʼ����
  * ����  lcdInitStruct: LCDҺ����ʾ������Ϣ�ṹ�塣
  * ����  �ޡ�
  */
void
LCD_Configuration(LCD_InitTypeDef* lcdInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;
    uint32_t tmploc = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_SEG_PIN(lcdInitStruct->m_SegPin));
    CHECK_RESTRICTION(CHECK_LCD_SEG_FUNCTION(lcdInitStruct->m_SegPinEn));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(lcdInitStruct->m_Analog));
    CHECK_RESTRICTION(CHECK_LCD_SOURCE(lcdInitStruct->m_ClockSource));
    CHECK_RESTRICTION(CHECK_LCD_SOURCE_DIVIDE(lcdInitStruct->m_SourcePrescaler));
    CHECK_RESTRICTION(CHECK_LCD_PRESCALER(lcdInitStruct->m_LCDPrescaler));
    CHECK_RESTRICTION(CHECK_LCD_COMMON_PORT(lcdInitStruct->m_CommonPort));
    CHECK_RESTRICTION(CHECK_LCD_VOLTAGE_SELECT(lcdInitStruct->m_VoltageSelect));

    /*---------------- ����LCD_CTL�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_Analog������LCDENANALOGλ�� */
    /* ���ݽṹ���Աm_CommonPort������LMUXλ�� */
    /* ���ݽṹ���Աm_SourcePrescaler������FRλ�� */
    /* ���ݽṹ���Աm_VoltageSelect������VLCDMDλ�� */
    /* ���ݽṹ���Աm_ClockSource������CSλ�� */
    /* ���ݽṹ���Աm_LCDPrescaler������LPλ�� */
    tmpreg = (lcdInitStruct->m_Analog << LCD_CTL_LCDENANALOG_POS) \
           | (lcdInitStruct->m_CommonPort) \
           | (lcdInitStruct->m_SourcePrescaler) \
           | (lcdInitStruct->m_VoltageSelect << LCD_CTL_VLCDMD_POS) \
           | (lcdInitStruct->m_ClockSource) \
           | (lcdInitStruct->m_LCDPrescaler);
    LCD_CTL = SFR_Config (LCD_CTL,
                          ~LCD_CTL_INIT_MASK,
                          tmpreg);

    /*---------------- ����LCD_SEGx�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_SegPin��m_SegPinEn������SEGxλ�� */
    tmpreg = (uint32_t)lcdInitStruct->m_SegPin;
    if (tmpreg != 0)
    {
        if (lcdInitStruct->m_SegPinEn != LCD_SEG_PIN_IO)
        {
            /* ʹ�����ŵĶι��� */
            LCD_SEG0 |= tmpreg;
        }
        else
        {
            /* ʹ�����ŵ�I/O���� */
            LCD_SEG0 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }

    tmpreg = (uint32_t)(lcdInitStruct->m_SegPin >> 32);
    if (tmpreg != 0)
    {
        if (lcdInitStruct->m_SegPinEn != LCD_SEG_PIN_IO)
        {
            /* ʹ�����ŵĶι��� */
            LCD_SEG1 |= tmpreg;
        }
        else
        {
            /* ʹ�����ŵ�I/O���� */
            LCD_SEG1 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }
}

/**
  * ����  ��ʼ��LCDҺ����ʾ������Ϣ�ṹ�塣
  * ����  lcdInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
LCD_Struct_Init (LCD_InitTypeDef* lcdInitStruct)
{
    /* ��ʼ�� SEG���ű������ѡ�� */
    lcdInitStruct->m_SegPin = 0;
    /* ��ʼ�� �����Ź��� */
    lcdInitStruct->m_SegPinEn = LCD_SEG_PIN_IO;
    /* ��ʼ�� LCDģ�ⲿ��ʹ�� */
    lcdInitStruct->m_Analog = FALSE;
    /* ��ʼ�� LCDʱ��Դ */
    lcdInitStruct->m_ClockSource = LCD_SOURCE_HALF_SCLK;
    /* ��ʼ�� LCDʱ��Դ��Ƶ */
    lcdInitStruct->m_SourcePrescaler = LCD_SOURCE_DIVIDE_2;
    /* ��ʼ�� LCDԤ��Ƶ�� */
    lcdInitStruct->m_LCDPrescaler = LCD_PRESCALER_1;
    /* ��ʼ�� LCD������ѡ�� */
    lcdInitStruct->m_CommonPort = LCD_COMMON_PORT_DIV_8;
    /* ��ʼ�� LCD�ڲ�/�ⲿ��Դѡ�� */
    lcdInitStruct->m_VoltageSelect = LCD_VOLTAGE_INTERNAL;
}
/**
  *   ##### Һ����ʾ(LCD)��ʼ������������� #####
  */


/**
  *   ##### Һ����ʾ(LCD)�������ú��� #####
  */
/**
  * ����  ����LCD����ʹ�ܡ�
  * ����  NewState: LCD����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Cmd_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_CTL�Ĵ���LCDENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��LCD����ģ�� */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_LCDEN_POS);
    }
    else
    {
        /* ��ֹLCD����ģ�� */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_LCDEN_POS);
    }
}

/**
  * ����  ����LCDģ�ⲿ��ʹ�ܡ�
  * ����  NewState: LCDģ�ⲿ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Analog_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_CTL�Ĵ���LCDENANALOGλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��LCDģ�ⲿ�� */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_LCDENANALOG_POS);
    }
    else
    {
        /* ��ֹLCDģ�ⲿ�� */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_LCDENANALOG_POS);
    }
}

/**
  * ����  �����ڲ�ƫѹԴ��ѹ��
  * ����  Voltage: �ڲ�ƫѹԴ��ѹ��ȡֵ��ΧΪ:
  *                  LCD_BIAS_VOLTAGE_3P04: 3.04V
  *                  LCD_BIAS_VOLTAGE_2P89: 2.89V
  *                  LCD_BIAS_VOLTAGE_2P76: 2.76V
  *                  LCD_BIAS_VOLTAGE_2P62: 2.62V
  *                  LCD_BIAS_VOLTAGE_3P62: 3.62V
  *                  LCD_BIAS_VOLTAGE_3P46: 3.46V
  *                  LCD_BIAS_VOLTAGE_3P32: 3.32V
  *                  LCD_BIAS_VOLTAGE_3P19: 3.19V
  * ����  �ޡ�
  */
void
LCD_Internal_Bias_Voltage_Config (uint32_t Voltage)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_BIAS_VOLTAGE(Voltage));

    /*---------------- ����LCD_CTL�Ĵ���TRIMCPVOLλ ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_TRIMCPVOL, Voltage);
}

/**
  * ����  ���ù����ˡ�
  * ����  CommonPort: ������ѡ��ȡֵ��ΧΪ:
  *                     LCD_COMMON_PORT_STATIC: ��̬��COM0��
  *                     LCD_COMMON_PORT_DIV_2: 1/2 ��COM<1:0>��
  *                     LCD_COMMON_PORT_DIV_3: 1/3 ��COM<2:0>��
  *                     LCD_COMMON_PORT_DIV_4: 1/4 ��COM<3:0>��
  *                     LCD_COMMON_PORT_DIV_8: 1/8 ��COM<7:0>��
  * ����  �ޡ�
  */
void
LCD_Common_Port_Config (uint32_t CommonPort)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_COMMON_PORT(CommonPort));

    /*---------------- ����LCD_CTL�Ĵ���LMUXλ ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_LMUX, CommonPort);
}

/**
  * ����  ����LCDʱ��Դ��Ƶ��
  * ����  ClkDivider: LCDʱ��Դ��Ƶ��ȡֵ��ΧΪ:
  *                     LCD_SOURCE_DIVIDE_2: 1:2
  *                     LCD_SOURCE_DIVIDE_4: 1:4
  *                     LCD_SOURCE_DIVIDE_8: 1:8
  *                     LCD_SOURCE_DIVIDE_16: 1:16
  *                     LCD_SOURCE_DIVIDE_32: 1:32
  *                     LCD_SOURCE_DIVIDE_64: 1:64
  *                     LCD_SOURCE_DIVIDE_128: 1:128
  *                     LCD_SOURCE_DIVIDE_256: 1:256
  *                     LCD_SOURCE_DIVIDE_512: 1:512
  *                     LCD_SOURCE_DIVIDE_1024: 1:1024
  *                     LCD_SOURCE_DIVIDE_2048: 1:2048
  *                     LCD_SOURCE_DIVIDE_4096: 1:4096
  *                     LCD_SOURCE_DIVIDE_8192: 1:8192
  *                     LCD_SOURCE_DIVIDE_16384: 1:16384
  *                     LCD_SOURCE_DIVIDE_32768: 1:32768
  * ����  �ޡ�
  */
void
LCD_Source_Clock_Divider_Config (uint32_t ClkDivider)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_SOURCE_DIVIDE(ClkDivider));

    /*---------------- ����LCD_CTL�Ĵ���FRλ ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_FR, ClkDivider);
}

/**
  * ����  ����LCD��ѹѡ��
  * ����  VoltageSelect: LCD��ѹѡ��ȡֵ��ΧΪ:
  *                        LCD_VOLTAGE_INTERNAL: ���ڲ���ѹ�����·�ṩ��ѹ
  *                        LCD_VOLTAGE_EXTERNAL: ���ⲿ�ṩ��ѹ
  * ����  �ޡ�
  */
void
LCD_Voltage_Divider_Config (uint32_t VoltageSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_VOLTAGE_SELECT(VoltageSelect));

    /*---------------- ����LCD_CTL�Ĵ���VLCDSλ ----------------*/
    if (VoltageSelect != LCD_VOLTAGE_INTERNAL)
    {
        /* ���ⲿ�ṩ��ѹ */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_VLCDS_POS);
    }
    else
    {
        /* ���ڲ���ѹ�����·�ṩ��ѹ */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_VLCDS_POS);
    }
}

/**
  * ����  ����LCDƫ�õ�ѹԴѡ��
  * ����  VoltageSelect: LCDƫ�õ�ѹԴѡ��ȡֵ��ΧΪ:
  *                        LCD_VOLTAGE_INTERNAL: �ڲ���Դ��LCD����
  *                        LCD_VOLTAGE_EXTERNAL: �ⲿ��Դ��LCD����
  * ����  �ޡ�
  */
void
LCD_Bias_Voltage_Source_Config (uint32_t VoltageSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_VOLTAGE_SELECT(VoltageSelect));

    /*---------------- ����LCD_CTL�Ĵ���VLCDMDλ ----------------*/
    if (VoltageSelect != LCD_VOLTAGE_INTERNAL)
    {
        /* �ⲿ��Դ��LCD���� */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_VLCDMD_POS);
    }
    else
    {
        /* �ڲ���Դ��LCD���� */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_VLCDMD_POS);
    }
}

/**
  * ����  ����LCD PUMP����ʹ�ܡ�
  * ����  NewState: LCD PUMP����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Pump_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_CTL�Ĵ���PUMPENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PUMP */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_PUMPEN_POS);
    }
    else
    {
        /* ��ʹ��PUMP���� */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_PUMPEN_POS);
    }
}

/**
  * ����  ����ʱ��Դѡ��
  * ����  SourceClock: LCDʱ��Դѡ��ȡֵ��ΧΪ:
  *                      LCD_SOURCE_HALF_SCLK: SCLK / 2
  *                      LCD_SOURCE_HALF_HFCLK: HFCLK / 2
  *                      LCD_SOURCE_HALF_LFCLK: LFCLK / 2
  *                      LCD_SOURCE_HALF_INTLF: INTLF / 2
  * ����  �ޡ�
  */
void
LCD_Source_Clock_Config (uint32_t SourceClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_SOURCE(SourceClock));

    /*---------------- ����LCD_CTL�Ĵ���CSλ ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_CS, SourceClock);
}

/**
  * ����  ����LCD��������ѡ��
  * ����  WaveType: LCD��������ѡ��ȡֵ��ΧΪ:
  *                   LCD_WAVE_TYPE_A: A �Ͳ��Σ���ÿһ�����������ڸı���λ��
  *                   LCD_WAVE_TYPE_B: B �Ͳ��Σ���ÿһ֡�߽�ı���λ��
  * ����  �ޡ�
  */
void
LCD_Wave_Type_Config (uint32_t WaveType)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_WAVE_TYPE(WaveType));

    /*---------------- ����LCD_CTL�Ĵ���WFTλ ----------------*/
    if (WaveType != LCD_WAVE_TYPE_A)
    {
        /* B �Ͳ��� */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_WFT_POS);
    }
    else
    {
        /* A �Ͳ��� */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_WFT_POS);
    }
}

/**
  * ����  ����LCDƫ��ģʽѡ����Ҫ������LCD_Common_Port_Config������
  * ����  BiasMode: LCDƫ��ģʽѡ��ȡֵ��ΧΪ:
  *                   LCD_BIAS_MODE_STATIC: ��̬ƫ��ģʽ
  *                   LCD_BIAS_MODE_1_DIV_3: 1/3 ƫ��ģʽ
  *                   LCD_BIAS_MODE_1_DIV_2: 1/2 ƫ��ģʽ
  * ����  �ޡ�
  */
void
LCD_Bias_Mode_Config (uint32_t BiasMode)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_BIAS_MODE(BiasMode));

    /* ��ȡLCD_CTL�Ĵ���LMUX��Ϣ */
    tmpreg = LCD_CTL;
    tmpreg &= LCD_CTL_LMUX;
    switch (tmpreg)
    {
        case LCD_COMMON_PORT_STATIC:
            CHECK_RESTRICTION(LCD_BIAS_MODE_STATIC==BiasMode);
            break;
        case LCD_COMMON_PORT_DIV_4:
        case LCD_COMMON_PORT_DIV_8:
            CHECK_RESTRICTION(LCD_BIAS_MODE_1_DIV_3==BiasMode);
            break;
        default:
            break;
    }

    /*---------------- ����LCD_CTL�Ĵ���BIASMDλ ----------------*/
    if (BiasMode != LCD_BIAS_MODE_1_DIV_3)
    {
        /* 1/3 ƫ��ģʽ �� ��̬ƫ��ģʽ */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_BIASMD_POS);
    }
    else
    {
        /* 1/2 ƫ��ģʽ */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_BIASMD_POS);
    }
}

/**
  * ����  ����LCDԤ��Ƶ�ȡ�
  * ����  Prescaler: LCDԤ��Ƶ�ȣ�ȡֵ��ΧΪ:
  *                    LCD_PRESCALER_1: 1:1
  *                    LCD_PRESCALER_2: 1:2
  *                    LCD_PRESCALER_3: 1:3
  *                    LCD_PRESCALER_4: 1:4
  *                    LCD_PRESCALER_5: 1:5
  *                    LCD_PRESCALER_6: 1:6
  *                    LCD_PRESCALER_7: 1:7
  *                    LCD_PRESCALER_8: 1:8
  *                    LCD_PRESCALER_9: 1:9
  *                    LCD_PRESCALER_10: 1:10
  *                    LCD_PRESCALER_11: 1:11
  *                    LCD_PRESCALER_12: 1:12
  *                    LCD_PRESCALER_13: 1:13
  *                    LCD_PRESCALER_14: 1:14
  *                    LCD_PRESCALER_15: 1:15
  *                    LCD_PRESCALER_16: 1:16
  * ����  �ޡ�
  */
void
LCD_Prescaler_Config (uint32_t Prescaler)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_PRESCALER(Prescaler));

    /*---------------- ����LCD_CTL�Ĵ���LPλ ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_LP, Prescaler);
}

/**
  * ����  ����LCD��˸ģʽ����ʱ�䣬����ʱ��=(32 * FlickerOn)/֡Ƶ�ʡ�
  * ����  FlickerOn: LCD��˸ģʽ����ʱ�䣬ȡֵΪ8λ��Ч��ֵ��
  * ����  �ޡ�
  */
void
LCD_Flicker_On_Time_Config (uint8_t FlickerOn)
{
	uint32_t tmpreg = 0;
    /*---------------- ����LCD_PTL�Ĵ���FONλ ----------------*/
	tmpreg = FlickerOn << LCD_PTL_FON0_POS;
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_FON, tmpreg);
}

/**
  * ����  ����LCD��˸ģʽ����ʱ�䣬����ʱ��=(32 * FlickerOff)/֡Ƶ�ʡ�
  * ����  FlickerOff: LCD��˸ģʽ����ʱ�䣬ȡֵΪ8λ��Ч��ֵ��
  * ����  �ޡ�
  */
void
LCD_Flicker_Off_Time_Config (uint8_t FlickerOff)
{
	uint32_t tmpreg = 0;
    /*---------------- ����LCD_PTL�Ĵ���FOFFλ ----------------*/
	tmpreg = FlickerOff << LCD_PTL_FOFF0_POS;
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_FOFF, tmpreg);
}

/**
  * ����  ����LCD��˸ģʽʹ�ܡ�
  * ����  NewState: LCD��˸ģʽʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Flicker_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_PTL�Ĵ���FLKENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ����˸���� */
        SFR_SET_BIT_ASM(LCD_PTL, LCD_PTL_FLKEN_POS);
    }
    else
    {
        /* ��ֹ��˸���� */
        SFR_CLR_BIT_ASM(LCD_PTL, LCD_PTL_FLKEN_POS);
    }
}

/**
  * ����  ����LCD����ģʽʹ�ܡ�
  * ����  NewState: LCD����ģʽʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Save_Power_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_PTL�Ĵ���SPENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܽ���ģʽ */
        SFR_SET_BIT_ASM(LCD_PTL, LCD_PTL_SPEN_POS);
    }
    else
    {
        /* ��ֹ����ģʽ */
        SFR_CLR_BIT_ASM(LCD_PTL, LCD_PTL_SPEN_POS);
    }
}

/**
  * ����  ����LCDʱ�䵥λ��С��
  * ����  TimeSlice: LCDʱ�䵥λ��С��ȡֵ��ΧΪ��
  *                    LCD_TIME_SLICE_EQU_2_FRAME: ʱ�䵥λt����2֡
  *                    LCD_TIME_SLICE_EQU_4_FRAME: ʱ�䵥λt����4֡
  *                    LCD_TIME_SLICE_EQU_8_FRAME: ʱ�䵥λt����8֡
  *                    LCD_TIME_SLICE_EQU_16_FRAME: ʱ�䵥λt����16֡
  *                    LCD_TIME_SLICE_EQU_32_FRAME: ʱ�䵥λt����32֡
  *                    LCD_TIME_SLICE_EQU_64_FRAME: ʱ�䵥λt����64֡
  *                    LCD_TIME_SLICE_EQU_128_FRAME: ʱ�䵥λt����128֡
  *                    LCD_TIME_SLICE_EQU_256_FRAME: ʱ�䵥λt����256֡
  * ����  �ޡ�
  */
void
LCD_Time_Slice_Config (uint32_t TimeSlice)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_TIME_SLICE(TimeSlice));

    /*---------------- ����LCD_PTL�Ĵ���TSλ ----------------*/
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_TS, TimeSlice);
}

/**
  * ����  ���ù���ʱ��͹ر�ʱ�������
  * ����  WorkTime: ����ʱ��͹ر�ʱ�������ȡֵ��ΧΪ��
  *                   LCD_WORK_VS_STOP_8_8: LCD����ʱ��͹ر�ʱ��ı���Ϊ8:8
  *                   LCD_WORK_VS_STOP_7_9: LCD����ʱ��͹ر�ʱ��ı���Ϊ7:9
  *                   LCD_WORK_VS_STOP_6_10: LCD����ʱ��͹ر�ʱ��ı���Ϊ6:10
  *                   LCD_WORK_VS_STOP_5_11: LCD����ʱ��͹ر�ʱ��ı���Ϊ5:11
  *                   LCD_WORK_VS_STOP_4_12: LCD����ʱ��͹ر�ʱ��ı���Ϊ4:12
  *                   LCD_WORK_VS_STOP_3_13: LCD����ʱ��͹ر�ʱ��ı���Ϊ3:13
  *                   LCD_WORK_VS_STOP_2_14: LCD����ʱ��͹ر�ʱ��ı���Ϊ2:14
  *                   LCD_WORK_VS_STOP_1_15: LCD����ʱ��͹ر�ʱ��ı���Ϊ1:15
  * ����  �ޡ�
  */
void
LCD_Work_Time_Config (uint32_t WorkTime)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_WORK_VS_STOP(WorkTime));

    /*---------------- ����LCD_PTL�Ĵ���IFλ ----------------*/
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_IF, WorkTime);
}

/**
  * ����  ����A����ģʽ���ơ�
  * ����  PowerCtl: ����ģʽ���ƣ�ȡֵ��ΧΪ��
  *                   LCD_HIGH_RESISTANCE_NO_BUF: ʹ�øߵ����Ҳ�����BUF
  *                   LCD_LOW_RESISTANCE_NO_BUF: ʹ�õ͵����Ҳ�����BUF
  *                   LCD_HIGH_RESISTANCE_WITH_BUF: ʹ�øߵ����Ҳ���BUF
  *                   LCD_LOW_RESISTANCE_WITH_BUF: ʹ�õ͵����Ҳ���BUF
  * ����  �ޡ�
  */
void
LCD_A_Power_Control_Config (uint32_t PowerCtl)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_POWER_CONTROL(PowerCtl));

    /*---------------- ����LCD_PTL�Ĵ���APCλ ----------------*/
    tmpreg = PowerCtl << LCD_PTL_APC0_POS;
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_APC, tmpreg);
}

/**
  * ����  ����B����ģʽ���ơ�
  * ����  PowerCtl: ����ģʽ���ƣ�ȡֵ��ΧΪ��
  *                   LCD_HIGH_RESISTANCE_NO_BUF: ʹ�øߵ����Ҳ�����BUF
  *                   LCD_LOW_RESISTANCE_NO_BUF: ʹ�õ͵����Ҳ�����BUF
  *                   LCD_HIGH_RESISTANCE_WITH_BUF: ʹ�øߵ����Ҳ���BUF
  *                   LCD_LOW_RESISTANCE_WITH_BUF: ʹ�õ͵����Ҳ���BUF
  * ����  �ޡ�
  */
void
LCD_B_Power_Control_Config (uint32_t PowerCtl)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_POWER_CONTROL(PowerCtl));

    /*---------------- ����LCD_PTL�Ĵ���BPCλ ----------------*/
    tmpreg = PowerCtl << LCD_PTL_BPC0_POS;
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_BPC, tmpreg);
}

/**
  * ����  ����ʱ�������ȿ��ơ�
  * ����  PowerMode: ʱ�������ȿ��ƣ�ȡֵ��ΧΪ��
  *                    LCD_POWER_B_MODE: ����ģʽ����ΪB����ģʽ
  *                    LCD_POWER_A_VS_B_IS_1_15: ���A����B��ʱ�䳤�ȱ�ֵΪ1:15
  *                    LCD_POWER_A_VS_B_IS_2_14: ���A����B��ʱ�䳤�ȱ�ֵΪ2:14
  *                    LCD_POWER_A_VS_B_IS_3_13: ���A����B��ʱ�䳤�ȱ�ֵΪ3:13
  *                    LCD_POWER_A_VS_B_IS_4_12: ���A����B��ʱ�䳤�ȱ�ֵΪ4:12
  *                    LCD_POWER_A_VS_B_IS_5_11: ���A����B��ʱ�䳤�ȱ�ֵΪ5:11
  *                    LCD_POWER_A_VS_B_IS_6_10: ���A����B��ʱ�䳤�ȱ�ֵΪ6:10
  *                    LCD_POWER_A_VS_B_IS_7_9: ���A����B��ʱ�䳤�ȱ�ֵΪ7:9
  * ����  �ޡ�
  */
void
LCD_Power_Mode_Config (uint32_t PowerMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_POWER_MODE(PowerMode));

    /*---------------- ����LCD_PTL�Ĵ���ATICλ ----------------*/
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_ATIC, PowerMode);
}

/**
  * ����  ����LCD��ʹ�ܡ�
  * ����  SegPin: �����ű�����룬ȡֵΪLCD_SEG_PIN_0~LCD_SEG_PIN_47�е�һ������ϡ�
  *       SegFunction: �����Ź������ã�ȡֵΪ��
  *                      LCD_SEG_PIN_IO: ʹ�����ŵ�I/O����
  *                      LCD_SEG_PIN_SEG: ʹ�����ŵĶι���
  * ����  �ޡ�
  */
void
LCD_Seg_Pin_Function_Config (uint64_t SegPin, uint32_t SegFunction)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_SEG_PIN(SegPin));
    CHECK_RESTRICTION(CHECK_LCD_SEG_FUNCTION(SegFunction));

    tmpreg = (uint32_t)SegPin;
    if (tmpreg != 0)
    {
        /*---------------- ����LCD_SEG0�Ĵ���SEGxλ ----------------*/
        if (SegFunction != LCD_SEG_PIN_IO)
        {
            /* ʹ�����ŵĶι��� */
            LCD_SEG0 |= tmpreg;
        }
        else
        {
            /* ʹ�����ŵ�I/O���� */
            LCD_SEG0 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }

    tmpreg = (uint32_t)(SegPin >> 32);
    if (tmpreg != 0)
    {
        /*---------------- ����LCD_SEG1�Ĵ���SEGxλ ----------------*/
        if (SegFunction != LCD_SEG_PIN_IO)
        {
            /* ʹ�����ŵĶι��� */
            LCD_SEG1 |= tmpreg;
        }
        else
        {
            /* ʹ�����ŵ�I/O���� */
            LCD_SEG1 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }
}

/**
  * ����  ����LCD���ݡ�
  * ����  ComPin: COM���ű�ţ�ȡֵΪLCD_PIN_SOURCE_COM0~LCD_PIN_SOURCE_COM7�е�һ����
  *       SegPin: ȡֵΪLCD_SEG_PIN_0~LCD_SEG_PIN_47�е�һ������ϡ�
  *       Pixel: ���ص���״̬��ȡֵΪ��
  *                      LCD_PIXEL_TRANSPARENCY: ���������أ�͸����
  *                      LCD_PIXEL_LIGHT: �������أ���͸����
  * ����  �ޡ�
  */
void
LCD_Data_Pixel_Config (uint32_t ComPin, uint64_t SegPin, uint32_t Pixel)
{
    uint32_t tmpmask = 0;
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_PIN_SOURCE_COM(ComPin));
    CHECK_RESTRICTION(CHECK_LCD_SEG_PIN(SegPin));
    CHECK_RESTRICTION(CHECK_LCD_PIXEL(Pixel));

    /* ����SEG0~SEG31���� */
    tmpmask = (uint32_t)SegPin;
    if (tmpmask != 0)
    {
        /*---------------- ����LCD_DATAx�Ĵ���COMxSEGxλ ----------------*/
        if (Pixel != LCD_PIXEL_TRANSPARENCY)
        {
            /* �������أ���͸���� */
            LCD_SFR->DATA[ComPin] |= tmpmask;
        }
        else
        {
            /* ���������أ�͸���� */
            LCD_SFR->DATA[ComPin] &= ~tmpmask;
        }
    }
    else
    {
        ;
    }

    /* ����SEG32~SEG39���� */
    tmpmask = (uint32_t)(SegPin >> 32);
    tmpmask &= 0xFF;
    if (tmpmask != 0)
    {
        /*---------------- ����LCD_DATAx�Ĵ���COMxSEGxλ ----------------*/
        tmpreg = (ComPin & 0x3) * 8;
        if (Pixel != LCD_PIXEL_TRANSPARENCY)
        {
            /* �������أ���͸���� */
            LCD_SFR->DATA[(ComPin >> 2) + 8] |= tmpmask << tmpreg;
        }
        else
        {
            /* ���������أ�͸���� */
            LCD_SFR->DATA[(ComPin >> 2) + 8] &= ~(tmpmask << tmpreg);
        }
    }
    else
    {
        ;
    }

    /* ����SEG40~SEG47���� */
    tmpmask = (uint32_t)(SegPin >> (32 + 8));
    if (tmpmask != 0)
    {
        /*---------------- ����LCD_DATAx�Ĵ���COMxSEGxλ ----------------*/
        tmpreg = (ComPin & 0x3) * 8;
        if (Pixel != LCD_PIXEL_TRANSPARENCY)
        {
            /* �������أ���͸���� */
            LCD_SFR->DATA[(ComPin >> 2) + 10] |= tmpmask << tmpreg;
        }
        else
        {
            /* ���������أ�͸���� */
            LCD_SFR->DATA[(ComPin >> 2) + 10] &= ~(tmpmask << tmpreg);
        }
    }
    else
    {
        ;
    }
}
/**
  * ���� : LCDд��Ĵ���DATAx��
  * ���� : LCD_Datax_Sel��0-7(COM0-7)
  * 	   LCD_Datax_Data��Ҫ���ĵ�����
  * 	   LCD_Datax_Loc��Ҫд���λ��
  * ���� �������쳣����0����ȷ�޷���
  * Ч�� ���û�����ʾLCD�����ݣ�д��ָ�������ݼĴ������ṩ��ڡ�
  */
uint16_t LCD_SET_Datareg (uint8_t LCD_Datax_Sel,uint64_t LCD_Datax_Data,uint64_t LCD_Datax_Loc)
{
	uint32_t masktmp;//����
	/*----------------����Ƿ�ΪData0-7 ----------------*/
	CHECK_RESTRICTION(CHECK_LCD_COM_SEL(LCD_Datax_Sel));
	/*----------------����û������64λ������16λ�Ƿ�Ϊ0----------------*/
	if(((LCD_Datax_Data>>48) != 0) | ((LCD_Datax_Loc>>48) != 0))
	{
		return 0;
	}
	/*----------------д��LCD_DATA�Ĵ���DATAλ ----------------
	 * com0-7�ĵ�32λд��0-7
	 * com0-3�ĵ�32-39λд��com8
	 * com4-7�ĵ�32-39λд��com9
	 * com0-3�ĵ�40-47λд��com10
	 * com4-7�ĵ�40-47λд��com11
	 * ����ο�LCD���ݼĴ���
	 * */
	if(LCD_Datax_Sel==0)
	{
		LCD_DATA0=(LCD_DATA0 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//0��0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//�����16-23λ��Ҫ����
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA8 = (LCD_DATA8 & ~masktmp) | ((LCD_Datax_Data >>32) & masktmp );//8��0-7
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//�����24-31λ��Ҫ����
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA10=(LCD_DATA10 & ~masktmp) | ((LCD_Datax_Data >>40) & masktmp );//10��0-7
		}
	}
	if(LCD_Datax_Sel==1)
	{
		LCD_DATA1=(LCD_DATA1 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//1��0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//�����16-23λ��Ҫ����
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA8 = (LCD_DATA8 & ~(masktmp<<8)) | ((LCD_Datax_Data>>32 & masktmp )<<8);//8��8-15
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//�����24-31λ��Ҫ����
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA10 = (LCD_DATA10 & ~(masktmp<<8)) | ((LCD_Datax_Data>>40 & masktmp )<<8);//10��8-15
		}
	}
	if(LCD_Datax_Sel==2)
	{
		LCD_DATA2=(LCD_DATA2 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//2��0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//�����16-23λ��Ҫ����
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA8 = (LCD_DATA8 & ~(masktmp<<16)) | ((LCD_Datax_Data>>32 & masktmp )<<16);//8��16-23
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//�����24-31λ��Ҫ����
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA10 = (LCD_DATA10 & ~(masktmp<<16)) | ((LCD_Datax_Data>>40 & masktmp )<<16);//10��16-23
		}
	}
	if(LCD_Datax_Sel==3)
	{
		LCD_DATA3=(LCD_DATA3 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//3��0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//�����16-23λ��Ҫ����
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA8 = (LCD_DATA8 & ~(masktmp<<24)) | ((LCD_Datax_Data>>32 & masktmp )<<24);//8��16-23//8��24-31
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//�����24-31λ��Ҫ����
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA10 = (LCD_DATA10 & ~(masktmp<<24)) | ((LCD_Datax_Data>>40 & masktmp )<<24);//10��24-31
		}
	}
	if(LCD_Datax_Sel==4)
	{
		LCD_DATA4=(LCD_DATA4 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//4��0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//�����16-23λ��Ҫ����
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA9 = (LCD_DATA9 & ~masktmp) | ((LCD_Datax_Data >>32) & masktmp );//9��0-7
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//�����24-31λ��Ҫ����
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA11=(LCD_DATA11 & ~masktmp) | ((LCD_Datax_Data >>40) & masktmp );//11��0-7
		}
	}
	if(LCD_Datax_Sel==5)
	{
		LCD_DATA5=(LCD_DATA5 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//5��0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//�����16-23λ��Ҫ����
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA9 = (LCD_DATA9 & ~(masktmp<<8)) | ((LCD_Datax_Data>>32 & masktmp )<<8);//9��8-15
		}
	if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//�����24-31λ��Ҫ����
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA11 = (LCD_DATA11 & ~(masktmp<<8)) | ((LCD_Datax_Data>>40 & masktmp )<<8);//11��8-15
		}
	}
	if(LCD_Datax_Sel==6)
	{
		LCD_DATA6=(LCD_DATA6 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//6��0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//�����16-23λ��Ҫ����
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA9 = (LCD_DATA9 & ~(masktmp<<16)) | ((LCD_Datax_Data>>32 & masktmp )<<16);//9��16-23
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//�����24-31λ��Ҫ����
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA11 = (LCD_DATA11 & ~(masktmp<<16)) | ((LCD_Datax_Data>>40 & masktmp )<<16);//11��16-23
		}
	}
	if(LCD_Datax_Sel==7)
	{
		LCD_DATA7=(LCD_DATA7 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//7��0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//�����16-23λ��Ҫ����
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA9 = (LCD_DATA9 & ~(masktmp<<24)) | ((LCD_Datax_Data>>32 & masktmp )<<24);//9��16-23//8��24-31
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//�����24-31λ��Ҫ����
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA11 = (LCD_DATA11 & ~(masktmp<<24)) | ((LCD_Datax_Data>>40 & masktmp )<<24);//11��24-31
		}
	}
}
/**
  * ����  ����LCD SEGͨ��ѡ��
  * ����  SegSel: SEGͨ����ţ�ȡֵΪLCD_SEG_PIN_0~LCD_SEG_PIN_47�е�һ������ϡ�
  *       NewState: SEGͨ��ѡ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Seg_Channel_Select_Config (uint64_t SegSel, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_SEG_PIN(SegSel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    tmpreg = (uint32_t)SegSel;
    if (tmpreg != 0)
    {
        /*---------------- ����LCD_SEL0�Ĵ���SEGSELxλ ----------------*/
        if (NewState != FALSE)
        {
            /* ѡ���Ӧͨ�� */
            LCD_SEL0 |= tmpreg;
        }
        else
        {
            /* ��ѡ���Ӧͨ�� */
            LCD_SEL0 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }

    tmpreg = (uint32_t)(SegSel >> 32);
    if (tmpreg != 0)
    {
        /*---------------- ����LCD_SEL1�Ĵ���SEGSELxλ ----------------*/
        if (NewState != FALSE)
        {
            /* ѡ���Ӧͨ�� */
            LCD_SEL1 |= tmpreg;
        }
        else
        {
            /* ��ѡ���Ӧͨ�� */
            LCD_SEL1 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }
}

/**
  * ����  ����LCD COMͨ��ѡ��
  * ����  ComPin: COMͨ����ţ�ȡֵΪLCD_COM_PIN_0~LCD_COM_PIN_7�е�һ������ϡ�
  *       NewState: COMͨ��ѡ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Com_Channel_Select_Config (uint32_t ComPin,FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_COM_PIN(ComPin));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_SEL1�Ĵ���COMxSELyλ ----------------*/
    tmpreg = (ComPin<<LCD_SEL_COM0_POS) ;
    if (NewState != FALSE)
    {
        /* ѡ���Ӧͨ�� */
        LCD_SEL1 |= tmpreg;
    }
    else
    {
        /* ��ѡ���Ӧͨ�� */
        LCD_SEL1 &= ~tmpreg;
    }
}

/**
  * ����  ����LCDͨ��ѡ��
  * ����  IOConfig: IO��ѡ��ȡֵΪ:
  *                   LCD_IO_NONE: �����ã�
  *                   LCD_SEG0_CONFIGURE_PB4
  *                   LCD_SEG0_CONFIGURE_PD13
  *                   LCD_SEG1_CONFIGURE_PB5
  *                   LCD_SEG1_CONFIGURE_PF5
  *                   LCD_SEG2_CONFIGURE_PB6
  *                   LCD_SEG2_CONFIGURE_PC13
  *                   LCD_SEG3_CONFIGURE_PB7
  *                   LCD_SEG3_CONFIGURE_PH15
  *                   LCD_SEG10_CONFIGURE_PC0
  *                   LCD_SEG10_CONFIGURE_PH12
  *                   LCD_SEG11_CONFIGURE_PC1
  *                   LCD_SEG11_CONFIGURE_PH13
  *                   LCD_SEG12_CONFIGURE_PC2
  *                   LCD_SEG12_CONFIGURE_PH11
  *
  *                   LCD_COM0_CONFIGURE_PB0
  *                   LCD_COM0_CONFIGURE_PB14
  *                   LCD_COM1_CONFIGURE_PB1
  *                   LCD_COM1_CONFIGURE_PB15
  *                   LCD_COM2_CONFIGURE_PB2
  *                   LCD_COM2_CONFIGURE_PF0
  *                   LCD_COM4_CONFIGURE_PD13
  *                   LCD_COM4_CONFIGURE_PB4
  *                   LCD_COM5_CONFIGURE_PF5
  *                   LCD_COM5_CONFIGURE_PB5
  *                   LCD_COM6_CONFIGURE_PC13
  *                   LCD_COM6_CONFIGURE_PB6
  *                   LCD_COM7_CONFIGURE_PH15
  *                   LCD_COM7_CONFIGURE_PB7
  *
  *
  * ����  �ޡ�
  */
void
LCD_Channel_IO_Config (uint32_t IOConfig)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_IO(IOConfig));

    /* ������IO�ڣ������������ */
    if (LCD_IO_NONE == IOConfig)
    {
        return;
    }

    /*---------------- ����LCD_SELx�Ĵ��� ----------------*/
    tmpreg = IOConfig >> 4;
    if (tmpreg < LCD_PIN_SOURCE_SEG32)
    {
        /* ����SEG0~SEG31ͨ�� */
        tmpmask = IOConfig & 0xF;
        tmpmask <<= tmpreg;
        /* SEGSEL0~SEGSEL31������ */
        if (0 != tmpmask)
        {
            LCD_SEL0 |= tmpmask;
        }
        else
        {
            LCD_SEL0 &= ~tmpmask;
        }
    }
    else if (tmpreg < LCD_PIN_SOURCE_SEG47)
    {
        /* ����SEG32~SEG47ͨ�� */
        tmpreg -= LCD_PIN_SOURCE_SEG32;
        tmpmask = IOConfig & 0xF;
        tmpmask <<= tmpreg;
        /* SEGSEL32~SEGSEL47������ */
        if (0 != tmpmask)
        {
            LCD_SEL1 |= tmpmask;
        }
        else
        {
            LCD_SEL1 &= ~tmpmask;
        }
    }
    else
    {
        /* ����COM0SELy~COM7SELyͨ�� */
        tmpreg -= LCD_PIN_SOURCE_SEG32;
        tmpmask = IOConfig & 0xF;
        tmpmask <<= tmpreg;
        /* COM0~COM7������ */
        if (0 != tmpmask)
        {
            LCD_SEL1 |= tmpmask;
        }
        else
        {
            LCD_SEL1 &= ~tmpmask;
        }
    }
}


/**
  * ����  ��ȡLCD��ǰ����״̬��
  * ����  �ޡ�
  * ����  1: LCD����ģ�鹤����0:LCD������ģ�鲻������
  */
FlagStatus
LCD_Get_Work_Status (void)
{
    /*---------------- ��ȡLCD_CTL�Ĵ���LCDAλ ----------------*/
    if (LCD_CTL & LCD_CTL_LCDA)
    {
        /* LCD����ģ�鹤�� */
        return SET;
    }
    else
    {
        /* LCD������ģ�鲻���� */
        return RESET;
    }
}

/**
  * ����  ��ȡLCD��˸ģʽ״̬��
  * ����  �ޡ�
  * ����  1:LCD״̬Ϊ������0:LCD״̬Ϊ������
  */
FlagStatus
LCD_Get_Flicker_Status (void)
{
    /*---------------- ��ȡLCD_PTL�Ĵ���FLKSTUλ ----------------*/
    if (LCD_PTL & LCD_PTL_FLKSTU)
    {
        /* LCD״̬Ϊ���� */
        return SET;
    }
    else
    {
        /* LCD״̬Ϊ���� */
        return RESET;
    }
}
/**
  *   ##### Һ����ʾ(LCD)�������ú���������� #####
  */

/**
  * ����  �����ӦDATA�Ĵ���
  * ����  ComPin: COM���ű�ţ�ȡֵΪLCD_PIN_SOURCE_COM0~LCD_PIN_SOURCE_COM7�е�һ����
  * ����  �ޡ�
  */
void 
LCD_Clear_DATA_Registers(uint32_t ComPin)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_COM_PIN(ComPin));
    /*--------------- ���0~31λ ----------------*/
    LCD_SFR->DATA[ComPin] = 0;
    /*--------------- ���32~39λ ----------------*/
    if(ComPin < LCD_PIN_SOURCE_COM4)
    {
        LCD_SFR->DATA[8] &= ~((uint32_t)0xff<<(8*ComPin));
    }
    else 
    {
        LCD_SFR->DATA[9] &= ~((uint32_t)0xff<<(8*ComPin));
    }
    /*--------------- ���40~47λ ----------------*/
    if(ComPin < LCD_PIN_SOURCE_COM4)
    {
        LCD_SFR->DATA[10] &= ~((uint32_t)0xff<<(8*ComPin));
    }
    else 
    {
        LCD_SFR->DATA[11] &= ~((uint32_t)0xff<<(8*ComPin));
    }
}

/**
  * ����  �������DATA�Ĵ���
  * ����  �ޡ�
  * ����  �ޡ�
  */
void 
LCD_Clear_ALLDATA_Registers(void)
{
    uint8_t i;
    for(i=0;i<12;i=0)
    {
        LCD_SFR->DATA[i] = 0;
    }
}

/**
  *   ##### Һ����ʾ(LCD)�жϹ����� #####
  */
/**
  * ����  ���ø����¼�����
  * ����  NewState: �����¼�����״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Updata_All_Registers_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_INTCTL�Ĵ���UDRλ ----------------*/
    if (NewState != FALSE)
    {
        /* ��������LCD_DATA�Ĵ��� */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDR_POS);
    }
    else
    {
        /* �޶��� */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDR_POS);
    }
}

/**
  * ����  ����֡��������ʹ�ܡ�
  * ����  NewState: ֡��������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Insert_Dead_Time_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_INTCTL�Ĵ���LCDDEADλ ----------------*/
    if (NewState != FALSE)
    {
        /* ����һ֡��ʼ����һ֡���� */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_LCDDEAD_POS);
    }
    else
    {
        /* ���������� */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_LCDDEAD_POS);
    }
}
/**
  * ���� LCD��IO �ӿ�ʹ�ܡ�
  * ����  NewState: LCD �� IO ������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_IO_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_CTL�Ĵ���IOENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���� LCD �� IO ������ */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_IOEN_POS);
    }
    else
    {
        /* ��ֹ LCD �� IO ������ */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_IOEN_POS);
    }
}
/**
  * ���� VLCDPEN�� �ӿ�ʹ�ܡ�
  * ����  NewState: VLCDPEN�� �ӿ�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_VLCDIO_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_CTL�Ĵ���VLCDPENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ���� VLCDIO �ӿ�ʹ�� */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_VLCDPEN_POS);
    }
    else
    {
        /* ��ֹ VLCDIO �ӿ�ʹ�� */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_VLCDPEN_POS);
    }
}
/**
  * ����  ����LCD�ж�ʹ�ܡ�
  * ����  InterruptType: LCD�ж��¼���ȡֵΪ:
  *                        LCD_INT_FRAME: ֡�ж��¼�
  *                        LCD_INT_FLICKER_OFF: �����ж��¼�
  *                        LCD_INT_FLICKER_ON: Ϩ���ж��¼�
  *                        LCD_INT_UPDATA: �����ж��¼�
  *       NewState: �����¼��ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Set_INT_Enable (uint32_t InterruptType, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_INT_EVENT(InterruptType));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_INTCTL�Ĵ�����Ӧλ ----------------*/
    tmpreg = InterruptType & 0xF;
    tmpreg = 0x1 << tmpreg;
    if (NewState != FALSE)
    {
        /* ʹ���ж� */
        LCD_INTCTL |= tmpreg;
    }
    else
    {
        /* ��ֹ�ж� */
        LCD_INTCTL &= ~tmpreg;
    }
}

/**
  * ����  ���LCD�жϱ�־��
  * ����  InterruptType: LCD�ж��¼���ȡֵΪ:
  *                        LCD_INT_FRAME: ֡�ж��¼�
  *                        LCD_INT_FLICKER_OFF: �����ж��¼�
  *                        LCD_INT_FLICKER_ON: Ϩ���ж��¼�
  *                        LCD_INT_UPDATA: �����ж��¼�
  * ����  �ޡ�
  */
void
LCD_Clear_INT_Flag (uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_INT_EVENT(InterruptType));

    /*---------------- ���LCD_INTCTL�Ĵ�����Ӧλ ----------------*/
    tmpreg = InterruptType >> 4;
    tmpreg = 0x1 << tmpreg;
    while(LCD_INTCTL & tmpreg)
    {
    	LCD_INTCTL &= ~tmpreg;
    }
}

/**
  * ����  ��ȡLCD�жϱ�־��
  * ����  InterruptType: LCD�ж��¼���ȡֵΪ:
  *                        LCD_INT_FRAME: ֡�ж��¼�
  *                        LCD_INT_FLICKER_OFF: �����ж��¼�
  *                        LCD_INT_FLICKER_ON: Ϩ���ж��¼�
  *                        LCD_INT_UPDATA: �����ж��¼�
  * ����  1:������Ӧ�жϱ�־��0:δ������Ӧ�жϱ�־��
  */
FlagStatus
LCD_Get_INT_Flag (uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_LCD_INT_EVENT(InterruptType));

    /*---------------- ��ȡLCD_INTCTL�Ĵ�����Ӧλ ----------------*/
    tmpreg = InterruptType >> 4;
    tmpreg = 0x1 << tmpreg;
    if (LCD_INTCTL & tmpreg)
    {
        /* ������Ӧ�жϱ�־ */
        return SET;
    }
    else
    {
        /* δ������Ӧ�жϱ�־ */
        return RESET;
    }
}

/**
  * ����  ���ø����¼��ж�ʹ�ܡ�
  * ����  NewState: �����¼��ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Updata_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_INTCTL�Ĵ���UDDIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܸ����¼��ж� */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDDIE_POS);
    }
    else
    {
        /* ��ֹ�����¼��ж� */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDDIE_POS);
    }
}

/**
  * ����  ���õ����ж�ʹ�ܡ�
  * ����  NewState: �����ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Flicker_On_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_INTCTL�Ĵ���FCKONIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܵ����ж� */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKONIE_POS);
    }
    else
    {
        /* ��ֹ�����ж� */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKONIE_POS);
    }
}

/**
  * ����  ����Ϩ���ж�ʹ�ܡ�
  * ����  NewState: Ϩ���ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Flicker_Off_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_INTCTL�Ĵ���FCKOFFIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��Ϩ���ж� */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKOFFIE_POS);
    }
    else
    {
        /* ��ֹϨ���ж� */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKOFFIE_POS);
    }
}

/**
  * ����  ����֡�ж�ʹ�ܡ�
  * ����  NewState: ֡�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
LCD_Frame_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����LCD_INTCTL�Ĵ���FRAMIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��֡�ж� */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FRAMIE_POS);
    }
    else
    {
        /* ��ֹ֡�ж� */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FRAMIE_POS);
    }
}

/**
  * ����  ��ȡ�����¼��жϱ�־��
  * ����  �ޡ�
  * ����  1:���������¼���0:δ���������¼���
  */
FlagStatus
LCD_Get_Updata_INT_Flag (void)
{
    /*---------------- ��ȡLCD_INTCTL�Ĵ���UDDIFλ ----------------*/
    if (LCD_INTCTL & LCD_INTCTL_UDDIF)
    {
        /* ���������¼� */
        return SET;
    }
    else
    {
        /* δ���������¼� */
        return RESET;
    }
}

/**
  * ����  ��ȡ�����жϱ�־��
  * ����  �ޡ�
  * ����  1:��⵽�����жϣ�0:δ������֡�жϡ�
  */
FlagStatus
LCD_Get_Flicker_On_INT_Flag (void)
{
    /*---------------- ��ȡLCD_INTCTL�Ĵ���FCKONIFλ ----------------*/
    if (LCD_INTCTL & LCD_INTCTL_FCKONIF)
    {
        /* ��⵽�����ж� */
        return SET;
    }
    else
    {
        /* δ������֡�ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡϨ���жϱ�־��
  * ����  �ޡ�
  * ����  1:��⵽Ϩ���жϣ�0:δ���Ϩ��֡�жϡ�
  */
FlagStatus
LCD_Get_Flicker_Off_INT_Flag (void)
{
    /*---------------- ��ȡLCD_INTCTL�Ĵ���FCKOFFIFλ ----------------*/
    if (LCD_INTCTL & LCD_INTCTL_FCKOFFIF)
    {
        /* ��⵽Ϩ���ж� */
        return SET;
    }
    else
    {
        /* δ���Ϩ��֡�ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡ֡�жϱ�־��
  * ����  �ޡ�
  * ����  1:��⵽֡�жϣ�0:δ��⵽֡�жϡ�
  */
FlagStatus
LCD_Get_Frame_INT_Flag (void)
{
    /*---------------- ��ȡLCD_INTCTL�Ĵ���FRAMIFλ ----------------*/
    if (LCD_INTCTL & LCD_INTCTL_FRAMIF)
    {
        /* ��⵽֡�ж� */
        return SET;
    }
    else
    {
        /* δ��⵽֡�ж� */
        return RESET;
    }
}

/**
  * ����  ���LCD�����¼��жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
LCD_Clear_Updata_INT_Flag (void)
{
    /*-------------------- ����LCD_INTCTL�Ĵ���UDDIFλ --------------------*/
	while((LCD_INTCTL & LCD_INTCTL_UDDIF)>>LCD_INTCTL_UDDIF_POS)
	{
		SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDDIF_POS);
	}
}

/**
  * ����  ���LCD�����жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
LCD_Clear_Flicker_On_INT_Flag (void)
{
    /*-------------------- ����LCD_INTCTL�Ĵ���FCKONIFλ --------------------*/
	while((LCD_INTCTL & LCD_INTCTL_FCKONIF)>>LCD_INTCTL_FCKONIF_POS)
	{
		SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKONIF_POS);
	}
}

/**
  * ����  ���LCDϨ���жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
LCD_Clear_Flicker_Off_INT_Flag (void)
{
    /*-------------------- ����LCD_INTCTL�Ĵ���FCKOFFIFλ --------------------*/
	while((LCD_INTCTL & LCD_INTCTL_FCKOFFIF)>>LCD_INTCTL_FCKOFFIF_POS)
	{
		SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKOFFIF_POS);
	}
}

/**
  * ����  ���LCD֡�жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
LCD_Clear_Frame_INT_Flag (void)
{
    /*-------------------- ����LCD_INTCTL�Ĵ���FRAMIFλ --------------------*/
	while((LCD_INTCTL & LCD_INTCTL_FRAMIF)>>LCD_INTCTL_FRAMIF_POS)
	{
		SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FRAMIF_POS);
	}
}

#endif  //KF32A_Periph_lcd
/**
  *   ##### Һ����ʾ(LCD)�жϹ������������ #####
  */

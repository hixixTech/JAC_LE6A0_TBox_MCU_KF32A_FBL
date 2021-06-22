/**
  ******************************************************************************
  * �ļ���  kf32a_basic_led.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��LED����ģ�����蹦�ܺ�����������
  *          + LED����ģ�����蹦��ʼ��
  *          + LED����ģ��������ʾ����
  *          + LED����ģ��״̬��ȡ
  *
  *********************************************************************
  */
#include "kf32a_basic_led.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_led
/**
  *   ##### LED����ģ�����蹦��ʼ������ #####
  */

/* LED_CTL0�Ĵ�����ʼ���������� */
#define LED_CTL0_INIT_MASK              (LED_CTL0_LEDEN \
                                       | LED_CTL0_LEDBR \
                                       | LED_CTL0_PNS \
                                       | LED_CTL0_CS \
                                       | LED_CTL0_CKS1 \
                                       | LED_CTL0_CKS0 )
/* LED_CTL1�Ĵ�����ʼ���������� */
#define LED_CTL1_INIT_MASK              (LED_CTL1_SEG16EN \
                                       | LED_CTL1_SEG12EN \
                                       | LED_CTL1_SEG8EN \
                                       | LED_CTL1_CCOMEN \
                                       | LED_CTL1_LUM )

/**
  * ����  ��λLED���裬ʹ������ʱ�ӡ�
  * ���� :�ޡ�
  * ����  �ޡ�
  */
void
LED_Reset (void)
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_LEDRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_LEDRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_LEDCLKEN, TRUE);

}

/**
  * ����  ģ��ת��ģ��(LED)��ʼ�����á�
  * ����  LEDx: ָ��LED�ڴ�ṹ��ָ�룬ȡֵΪLED0_SFR~LED2_SFR��
  *       ledInitStruct: LEDģ��������Ϣ�ṹ��ָ�롣
  * ����  �ޡ�
  */
void
LED_Configuration (LED_InitTypeDef* ledInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ledInitStruct->m_Module_Enable));
    CHECK_RESTRICTION(CHECK_OUTPUT_CONTROL_SEL(ledInitStruct->m_Output_Control_Select));
    CHECK_RESTRICTION(CHECK_WORK_MODE_SEL(ledInitStruct->m_Anodic_Cathodal_Sel));
    CHECK_RESTRICTION(CHECK_LED_CLKSOURCE_SEL(ledInitStruct->m_Source_Clock_Sel));
    CHECK_RESTRICTION(CHECK_CLK_DIVISION_SEL(ledInitStruct->m_Clock_Division));
    CHECK_RESTRICTION(CHECK_CLK_PREDIVISION_SEL(ledInitStruct->m_Clock_PreDivision));

    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ledInitStruct->m_Output_SEG12_15_Enable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ledInitStruct->m_Output_SEG8_11_Enable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ledInitStruct->m_Output_SEG0_7_Enable));
    CHECK_RESTRICTION(CHECK_COM_NUMBER_SEL(ledInitStruct->m_COM_Number_Select));
    CHECK_RESTRICTION(CHECK_LUM_SEL(ledInitStruct->m_Lum_Select));

    /*---------------- ����LED_CTL0�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_Module_Enable������LEDENλ�� */
    /* ���ݽṹ���Աm_Output_Control_Select������LEDBRλ�� */
    /* ���ݽṹ���Աm_Anodic_Cathodal_Sel������PNSλ�� */
    /* ���ݽṹ���Աm_Source_Clock_Sel������CSλ�� */
    /* ���ݽṹ���Աm_Clock_Division������CKS1λ�� */
    /* ���ݽṹ���Աm_Clock_PreDivision������CKS0λ�� */
    tmpreg = (uint32_t) ((uint32_t)(ledInitStruct->m_Module_Enable << LED_CTL0_LEDEN_POS) | \
                        (ledInitStruct->m_Output_Control_Select) | \
                        (ledInitStruct->m_Anodic_Cathodal_Sel) | \
                        (ledInitStruct->m_Source_Clock_Sel ) | \
                        (ledInitStruct->m_Clock_Division) | \
                        (ledInitStruct->m_Clock_PreDivision)
                        );
    LED_SFR->CTL0 = SFR_Config (LED_SFR->CTL0, ~LED_CTL0_INIT_MASK, tmpreg);

    /*---------------- ����LED_CTL1�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_Output_SEG12_15_Enable������SEG16ENλ�� */
    /* ���ݽṹ���Աm_Output_SEG8_11_Enable������SEG12ENλ�� */
    /* ���ݽṹ���Աm_Output_SEG0_7_Enable������SEG8ENλ�� */
    /* ���ݽṹ���Աm_COM_Number_Select������COMENλ�� */
    /* ���ݽṹ���Աm_Lum_Select������LUMλ�� */
    tmpreg = (uint32_t)(((uint32_t)ledInitStruct->m_Output_SEG12_15_Enable << LED_CTL1_SEG16EN_POS) | \
                         ((uint32_t)ledInitStruct->m_Output_SEG8_11_Enable << LED_CTL1_SEG12EN_POS) | \
                         ((uint32_t)ledInitStruct->m_Output_SEG0_7_Enable << LED_CTL1_SEG8EN_POS) | \
                         ledInitStruct->m_COM_Number_Select | \
                         ledInitStruct->m_Lum_Select);
    LED_SFR->CTL1 = SFR_Config (LED_SFR->CTL1, ~LED_CTL1_INIT_MASK, tmpreg);

}

/**
  * ����  ��ʼ��LED������Ϣ�ṹ�塣
  * ����  LEDInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
LED_Struct_Init (LED_InitTypeDef* LEDInitStruct)
{
    /* ��ʼ��LED��ʾ����ʹ��ѡ�� */
    LEDInitStruct->m_Module_Enable = FALSE;
    /* ��ʼ��LED�����������ѡ�� */
    LEDInitStruct->m_Output_Control_Select = LEDSEG_OUTPUT_DISABLE;
    /* ��ʼ��LED��������ѡ�� */
    LEDInitStruct->m_Anodic_Cathodal_Sel = LED_CATHODAL_DIRECT_MODE;
    /* ��ʼ��LED����ʱ��Դѡ�� */
    LEDInitStruct->m_Source_Clock_Sel = LED_CLKSOURCE_HFCLK;
    /* ��ʼ��LEDʱ��Դ��Ƶѡ�� */
    LEDInitStruct->m_Clock_Division = LED_CLK_DIVISION_2;
    /* ��ʼ��LEDԤ��Ƶ��ѡ�� */
    LEDInitStruct->m_Clock_PreDivision = LED_CLK_PREDIVISION_16;
    /* ��ʼ��LEDͨ��SEG<15:12>���ʹ�� */
    LEDInitStruct->m_Output_SEG12_15_Enable = FALSE;
    /* ��ʼ�� LEDͨ��SEG<11:8>���ʹ�� */
    LEDInitStruct->m_Output_SEG8_11_Enable = FALSE;
    /* ��ʼ��LEDͨ��SEG<7:0>���ʹ�� */
    LEDInitStruct->m_Output_SEG0_7_Enable = FALSE;
    /* ��ʼ��LED COM ʹ������ѡ�� */
    LEDInitStruct->m_COM_Number_Select = LED_COM_NUM_1;
    /* ��ʼ��LED COM �Զȵ�������ѡ�� */
    LEDInitStruct->m_Lum_Select = LED_COM_LUM_1;

}

/**
  * ����  ��ȡLED��ʾ֡��־��
  * ���� :��
  * ����  �ж���Ӧ״̬��1�����1������ɨ�裬0��δ�������ɨ�衣
  */
INTStatus
LED_Get_Frame_Flag (void)
{

    uint32_t tmpreg = 0;

    /*------------- ��ȡLED_CTL1�Ĵ���  FRAMEFλ-------------*/
    tmpreg = LED_SFR->CTL1;
    tmpreg = (tmpreg & 0x01 );

    /* ����ж�ʹ�ܺͱ�־ */
    if (tmpreg)
    {
        /*  LED���1������ɨ�裨1֡�� */
        return SET;
    }
    else
    {
        /* LEDδ�������ɨ�� */
        return RESET;
    }
}
/**
  * ����  ��ȡ��ʾ�������ݡ�
  * ���� :DATAx, (LED_DATA0-LED_DATA7)
  * ���� ����ʾ�������ݣ�16λ��Ч���ݡ�
  */
uint16_t
LED_Display_Data_Value (uint32_t LED_DATAx)
{
    uint32_t tmpreg = 0;

    /*---------------- ����LED_DATA�Ĵ���DATAλ ----------------*/
    if(LED_DATAx==LED_DATA0)
    {
    	tmpreg = LED_SFR->DATA0;
    }
    else if(LED_DATAx==LED_DATA1)
    {
    	tmpreg = LED_SFR->DATA1;
    }
    else if(LED_DATAx==LED_DATA2)
    {
    	tmpreg = LED_SFR->DATA2;
    }
    else if(LED_DATAx==LED_DATA3)
    {
    	tmpreg = LED_SFR->DATA3;
    }
    else if(LED_DATAx==LED_DATA4)
    {
    	tmpreg = LED_SFR->DATA4;
    }
    else if(LED_DATAx==LED_DATA5)
    {
    	tmpreg = LED_SFR->DATA5;
    }
    else if(LED_DATAx==LED_DATA6)
    {
    	tmpreg = LED_SFR->DATA6;
    }
    else if(LED_DATAx==LED_DATA7)
    {
    	tmpreg = LED_SFR->DATA7;
    }
    return (uint16_t) tmpreg;
}

/**
  * ���� : ������ʾ����
  * ���� : LED_Datax_Sel��Data0-7�Ĵ���
  * 	   LED_Datax_Data��Ҫд������ݣ�ȡֵΪ16λ��������
  * 	   LED_Datax_Mask��д���������룬ȡֵΪ16λ��������
  * 	   				����λ��Ӧ����λ��ȡ1�������ݣ�ȡ0����������
  * ���� ����
  */
void
LED_SET_Datareg (uint8_t LED_Datax_Sel,uint16_t LED_Datax_Data,uint16_t LED_Datax_Mask)
{
	uint16_t tmpreg;
	/*----------------����Ƿ�ΪData0-7 ----------------*/
	CHECK_RESTRICTION(CHECK_LED_COM_SEL(LED_Datax_Sel));

	/*----------------д��LED_DATA�Ĵ���DATAλ ----------------*/
	tmpreg = LED_Datax_Data & LED_Datax_Mask;
	switch (LED_Datax_Sel)
	{
	case 0:
		tmpreg |= LED_DATA0 & (~LED_Datax_Mask);
		LED_DATA0 = tmpreg ;
		break;
	case 1:
		tmpreg |= LED_DATA1 & (~LED_Datax_Mask);
		LED_DATA1 = tmpreg ;
		break;
	case 2:
		tmpreg |= LED_DATA2 & (~LED_Datax_Mask);
		LED_DATA2 = tmpreg ;
		break;
	case 3:
		tmpreg |= LED_DATA3 & (~LED_Datax_Mask);
		LED_DATA3 = tmpreg ;
		break;
	case 4:
		tmpreg |= LED_DATA4 & (~LED_Datax_Mask);
		LED_DATA4 = tmpreg ;
		break;
	case 5:
		tmpreg |= LED_DATA5 & (~LED_Datax_Mask);
		LED_DATA5 = tmpreg ;
		break;
	case 6:
		tmpreg |= LED_DATA6 & (~LED_Datax_Mask);
		LED_DATA6 = tmpreg ;
		break;
	case 7:
		tmpreg |= LED_DATA7 & (~LED_Datax_Mask);
		LED_DATA7 = tmpreg ;
		break;
	default:
		break;
	}
}
#endif //KF32A_Periph_led

/**
  ******************************************************************************
  * �ļ���  kf32a_basic_cmp.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��CMP���蹦�ܺ�����������
  *          + CMP�����ʼ�������ú���
  *          + CMP�������ݶ�д����
  *          + CMPУ��Ĵ������ú���
  *
  *********************************************************************
  */

#include "kf32a_basic_cmp.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_cmp
/* CMP˽�ж��� ----------------------------------------------------*/
/* CMP_CTL0�Ĵ����������� */
#define CMP_CTL0_INIT_MASK              (CMP_CTL0_PMOD \
                                       | CMP_CTL0_NMOD \
                                       | CMP_CTL0_IFFEN \
                                       | CMP_CTL0_IFREN \
                                       | CMP_CTL0_FLTCS \
                                       | CMP_CTL0_FDIV \
                                       | CMP_CTL0_FLTCNT \
                                       | CMP_CTL0_FLTEN \
                                       | CMP_CTL0_WINEN \
                                       | CMP_CTL0_POL)
/* CMP_CTL1�Ĵ����������� */
#define CMP_CTL1_INIT_MASK              (CMP_CTL1_PMOD \
                                       | CMP_CTL1_NMOD \
                                       | CMP_CTL1_IFFEN \
                                       | CMP_CTL1_IFREN \
                                       | CMP_CTL1_FLTCS \
                                       | CMP_CTL1_FDIV \
                                       | CMP_CTL1_FLTCNT \
                                       | CMP_CTL1_FLTEN \
                                       | CMP_CTL1_WINEN \
                                       | CMP_CTL1_POL)
/* CMP_CTL2�Ĵ����������� */
#define CMP_CTL2_INIT_MASK              (CMP_CTL2_PMOD \
                                       | CMP_CTL2_NMOD \
                                       | CMP_CTL2_IFFEN \
                                       | CMP_CTL2_IFREN \
                                       | CMP_CTL2_FLTCS \
                                       | CMP_CTL2_FDIV \
                                       | CMP_CTL2_FLTCNT \
                                       | CMP_CTL2_FLTEN \
                                       | CMP_CTL2_WINEN \
                                       | CMP_CTL2_POL)
/* CMP_CTL3�Ĵ����������� */
#define CMP_CTL3_INIT_MASK              (CMP_CTL3_PMOD \
                                       | CMP_CTL3_NMOD \
                                       | CMP_CTL3_FLTCS \
                                       | CMP_CTL3_FDIV \
                                       | CMP_CTL3_FLTCNT \
                                       | CMP_CTL3_FLTEN \
                                       | CMP_CTL3_WINEN \
                                       | CMP_CTL3_POL \
                                       | CMP_CTL3_C3EN)



/**
  *   ##### ��ʼ�������ú��� #####
  */
/**
  * ����  CMP���踴λ��ʹ������ʱ�ӡ�
  * ����  CMPx: ��
  * ����  �ޡ�
  */
void
CMP_Reset ()
{
	RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_CMPRST, TRUE);
	RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_CMPRST, FALSE);
	PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_CMPCLKEN, TRUE);
}

/**
  * ����  CMP�������á�
  * ����  CMPx: ȡֵΪCMP0_SFR��CMP1_SFR��CMP2_SFR��CMP3_SFR��
  *       CMPInitStruct: CMP������Ϣ
  * ����  �ޡ�
  */
void
CMP_Configuration (CMP_SFRmap * CMPx, CMP_InitTypeDef* CMPInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP_ALL_PERIPH(CMPx));

    if(CMPx==CMP0_SFR)
    {
    	CHECK_RESTRICTION(CHECK_CMP0_PositiveINPUT_PIN(CMPInitStruct->m_PositiveInput));
    	CHECK_RESTRICTION(CHECK_CMP0_NegativeINPUT_PIN(CMPInitStruct->m_NegativeInput));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FallTriggerEnable));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_RiseTriggerEnable));
    	CHECK_RESTRICTION(CHECK_CMP0_CLKSOURCE_SEL(CMPInitStruct->m_Clock));
    	CHECK_RESTRICTION(CHECK_CMP0_FLT_SampleNumber_SEL(CMPInitStruct->m_SampleNumber));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FilterEnable));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_ScopecontrolEnable));
    	CHECK_RESTRICTION(CHECK_CMP0_OUTPUT(CMPInitStruct->m_OutputPolarity));
    	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_CmpEnable));
        /*---------------------------- CMP_CTL0�Ĵ�������  -----------------*/
        /* ���ݽṹ���Աm_PositiveInput������PMOD0λ�� */
        /* ���ݽṹ���Աm_NegativeInput������NMOD0λ�� */
        /* ���ݽṹ���Աm_FallTriggerEnable������IFFENλ�� */
        /* ���ݽṹ���Աm_RiseTriggerEnable������IFRENλ�� */
        /* ���ݽṹ���Աm_Clock������FLTCSλ�� */
		/* ���ݽṹ���Աm_FrequencyDivision������FLTDIVλ�� */
        /* ���ݽṹ���Աm_SampleNumber������FLTCNTλ�� */
        /* ���ݽṹ���Աm_FilterEnable������FLTENλ�� */
        /* ���ݽṹ���Աm_ScopecontrolEnable������WINENλ�� */
        /* ���ݽṹ���Աm_OutputPolarity������POLλ�� */

	   tmpreg = ((CMPInitStruct->m_PositiveInput ) | \
				  (CMPInitStruct->m_NegativeInput) | \
				  (CMPInitStruct->m_FallTriggerEnable << CMP_CTL0_IFFEN_POS) | \
				  (CMPInitStruct->m_RiseTriggerEnable << CMP_CTL0_IFREN_POS) | \
				  (CMPInitStruct->m_Clock) | \
				  (CMPInitStruct->m_FrequencyDivision) | \
				  (CMPInitStruct->m_SampleNumber) | \
				  (CMPInitStruct->m_FilterEnable << CMP_CTL0_FLTEN_POS) | \
				  (CMPInitStruct->m_ScopecontrolEnable << CMP_CTL0_WINEN_POS) | \
				  (CMPInitStruct->m_OutputPolarity));
	   CMPx->CTL = SFR_Config (CMPx->CTL,
			   	   ~CMP_CTL0_INIT_MASK,
			   	   tmpreg);

	    if ((CMPInitStruct->m_CmpEnable) != FALSE)
	    {
	        /* ʹ��CMP */
	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0EN_POS);
	    }
	    else
	    {
	        /* ��ֹCMP */
	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0EN_POS);
	    }
    }
    else if(CMPx==CMP1_SFR)
    {
    	CHECK_RESTRICTION(CHECK_CMP1_PositiveINPUT_PIN(CMPInitStruct->m_PositiveInput));
    	CHECK_RESTRICTION(CHECK_CMP1_NegativeINPUT_PIN(CMPInitStruct->m_NegativeInput));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FallTriggerEnable));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_RiseTriggerEnable));
    	CHECK_RESTRICTION(CHECK_CMP1_CLKSOURCE_SEL(CMPInitStruct->m_Clock));
    	CHECK_RESTRICTION(CHECK_CMP1_FLT_SampleNumber_SEL(CMPInitStruct->m_SampleNumber));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FilterEnable));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_ScopecontrolEnable));
    	CHECK_RESTRICTION(CHECK_CMP1_OUTPUT(CMPInitStruct->m_OutputPolarity));
    	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_CmpEnable));
        /*---------------------------- CMP_CTL1�Ĵ�������  -----------------*/
        /* ���ݽṹ���Աm_PositiveInput������PMOD0λ�� */
        /* ���ݽṹ���Աm_NegativeInput������NMOD0λ�� */
        /* ���ݽṹ���Աm_FallTriggerEnable������IFFENλ�� */
        /* ���ݽṹ���Աm_RiseTriggerEnable������IFRENλ�� */
        /* ���ݽṹ���Աm_Clock������FLTCSλ�� */
		/* ���ݽṹ���Աm_FrequencyDivision������FLTDIVλ�� */
        /* ���ݽṹ���Աm_SampleNumber������FLTCNTλ�� */
        /* ���ݽṹ���Աm_FilterEnable������FLTENλ�� */
        /* ���ݽṹ���Աm_ScopecontrolEnable������WINENλ�� */
        /* ���ݽṹ���Աm_OutputPolarity������POLλ�� */

	   tmpreg = ((CMPInitStruct->m_PositiveInput ) | \
				  (CMPInitStruct->m_NegativeInput) | \
				  (CMPInitStruct->m_FallTriggerEnable << CMP_CTL1_IFFEN_POS) | \
				  (CMPInitStruct->m_RiseTriggerEnable << CMP_CTL1_IFREN_POS) | \
				  (CMPInitStruct->m_Clock) | \
				  (CMPInitStruct->m_FrequencyDivision) | \
				  (CMPInitStruct->m_SampleNumber) | \
				  (CMPInitStruct->m_FilterEnable << CMP_CTL1_FLTEN_POS) | \
				  (CMPInitStruct->m_ScopecontrolEnable << CMP_CTL1_WINEN_POS) | \
				  (CMPInitStruct->m_OutputPolarity));
	   CMPx->CTL = SFR_Config (CMPx->CTL, ~CMP_CTL1_INIT_MASK, tmpreg);

	    if ((CMPInitStruct->m_CmpEnable) != FALSE)
	    {
	        /* ʹ��CMP */
	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1EN_POS);
	    }
	    else
	    {
	        /* ��ֹCMP */
	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1EN_POS);
	    }

    }
    else if(CMPx==CMP2_SFR)
    {
       	CHECK_RESTRICTION(CHECK_CMP2_PositiveINPUT_PIN(CMPInitStruct->m_PositiveInput));
        	CHECK_RESTRICTION(CHECK_CMP2_NegativeINPUT_PIN(CMPInitStruct->m_NegativeInput));
            CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FallTriggerEnable));
            CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_RiseTriggerEnable));
        	CHECK_RESTRICTION(CHECK_CMP2_CLKSOURCE_SEL(CMPInitStruct->m_Clock));
        	CHECK_RESTRICTION(CHECK_CMP2_FLT_SampleNumber_SEL(CMPInitStruct->m_SampleNumber));
            CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FilterEnable));
            CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_ScopecontrolEnable));
        	CHECK_RESTRICTION(CHECK_CMP2_OUTPUT(CMPInitStruct->m_OutputPolarity));
        	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_CmpEnable));
            /*---------------------------- CMP_CTL2�Ĵ�������  -----------------*/
            /* ���ݽṹ���Աm_PositiveInput������PMOD0λ�� */
            /* ���ݽṹ���Աm_NegativeInput������NMOD0λ�� */
            /* ���ݽṹ���Աm_FallTriggerEnable������IFFENλ�� */
            /* ���ݽṹ���Աm_RiseTriggerEnable������IFRENλ�� */
            /* ���ݽṹ���Աm_Clock������FLTCSλ�� */
    		/* ���ݽṹ���Աm_FrequencyDivision������FLTDIVλ�� */
            /* ���ݽṹ���Աm_SampleNumber������FLTCNTλ�� */
            /* ���ݽṹ���Աm_FilterEnable������FLTENλ�� */
            /* ���ݽṹ���Աm_ScopecontrolEnable������WINENλ�� */
            /* ���ݽṹ���Աm_OutputPolarity������POLλ�� */

    	   tmpreg = ((CMPInitStruct->m_PositiveInput ) | \
    				  (CMPInitStruct->m_NegativeInput) | \
    				  (CMPInitStruct->m_FallTriggerEnable << CMP_CTL2_IFFEN_POS) | \
    				  (CMPInitStruct->m_RiseTriggerEnable << CMP_CTL2_IFREN_POS) | \
    				  (CMPInitStruct->m_Clock) | \
    				  (CMPInitStruct->m_FrequencyDivision) | \
    				  (CMPInitStruct->m_SampleNumber) | \
    				  (CMPInitStruct->m_FilterEnable << CMP_CTL2_FLTEN_POS) | \
    				  (CMPInitStruct->m_ScopecontrolEnable << CMP_CTL2_WINEN_POS) | \
    				  (CMPInitStruct->m_OutputPolarity));
    	   CMPx->CTL = SFR_Config (CMPx->CTL, ~CMP_CTL2_INIT_MASK, tmpreg);


   	    if ((CMPInitStruct->m_CmpEnable) != FALSE)
   	    {
   	        /* ʹ��CMP */
   	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2EN_POS);
   	    }
   	    else
   	    {
   	        /* ��ֹCMP */
   	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2EN_POS);
   	    }
    }
    else if(CMPx==CMP3_SFR)
    {
       	CHECK_RESTRICTION(CHECK_CMP3_PositiveINPUT_PIN(CMPInitStruct->m_PositiveInput));
		CHECK_RESTRICTION(CHECK_CMP3_NegativeINPUT_PIN(CMPInitStruct->m_NegativeInput));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FallTriggerEnable));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_RiseTriggerEnable));
		CHECK_RESTRICTION(CHECK_CMP3_CLKSOURCE_SEL(CMPInitStruct->m_Clock));
		CHECK_RESTRICTION(CHECK_CMP3_FLT_SampleNumber_SEL(CMPInitStruct->m_SampleNumber));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FilterEnable));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_ScopecontrolEnable));
		CHECK_RESTRICTION(CHECK_CMP3_OUTPUT(CMPInitStruct->m_OutputPolarity));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_CmpEnable));
		/*---------------------------- CMP_CTL3�Ĵ�������  -----------------*/
		/* ���ݽṹ���Աm_PositiveInput������PMOD0λ�� */
		/* ���ݽṹ���Աm_NegativeInput������NMOD0λ�� */
		/* ���ݽṹ���Աm_FallTriggerEnable������IFFENλ�� */
		/* ���ݽṹ���Աm_RiseTriggerEnable������IFRENλ�� */
		/* ���ݽṹ���Աm_Clock������FLTCSλ�� */
		/* ���ݽṹ���Աm_FrequencyDivision������FLTDIVλ�� */
		/* ���ݽṹ���Աm_SampleNumber������FLTCNTλ�� */
		/* ���ݽṹ���Աm_FilterEnable������FLTENλ�� */
		/* ���ݽṹ���Աm_ScopecontrolEnable������WINENλ�� */
		/* ���ݽṹ���Աm_OutputPolarity������POLλ�� */

	   tmpreg = ((CMPInitStruct->m_PositiveInput ) | \
				  (CMPInitStruct->m_NegativeInput) | \
				  (CMPInitStruct->m_Clock) | \
				  (CMPInitStruct->m_FrequencyDivision) | \
				  (CMPInitStruct->m_SampleNumber) | \
				  (CMPInitStruct->m_FilterEnable << CMP_CTL3_FLTEN_POS) | \
				  (CMPInitStruct->m_ScopecontrolEnable << CMP_CTL3_WINEN_POS) | \
				  (CMPInitStruct->m_OutputPolarity) | \
				  (CMPInitStruct->m_CmpEnable << CMP_CTL3_C3EN_POS));
	   CMPx->CTL = SFR_Config (CMPx->CTL, ~CMP_CTL3_INIT_MASK, tmpreg);

  	    if ((CMPInitStruct->m_FallTriggerEnable) != FALSE)
  	    {
  	        /* ʹ�ܱȽ���3�½��ش����ж� */
  	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IFFEN_POS);
  	    }
  	    else
  	    {
  	        /* ��ֹ�Ƚ���3�½��ش����ж� */
  	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IFFEN_POS);
  	    }

  	    if ((CMPInitStruct->m_RiseTriggerEnable) != FALSE)
  	    {
  	        /* ʹ�ܱȽ���3�½��ش����ж� */
  	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IFREN_POS);
  	    }
  	    else
  	    {
  	        /* ��ֹ�Ƚ���3�½��ش����ж� */
  	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IFREN_POS);
  	    }
    }
}

/**
  * ����  ��ʼ��CMP������Ϣ�ṹ�塣
  * ����  CMPInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
CMP_Struct_Init (CMP_InitTypeDef* CMPInitStruct)
{
    /* ��ʼ�� CMP����������ѡ��λ */
    CMPInitStruct->m_PositiveInput = CMP0_PositiveINPUT_PIN_PA0;
    /* ��ʼ�� CMP�ĸ�������ѡ��λ */
    CMPInitStruct->m_NegativeInput = CMP0_NegativeINPUT_PIN_PA1;
    /* ��ʼ�� CMP�½��ش����ж�ʹ�� */
    CMPInitStruct->m_FallTriggerEnable = FALSE;
    /* ��ʼ�� CMP�����ش����ж�ʹ�� */
    CMPInitStruct->m_RiseTriggerEnable = FALSE;
    /* ��ʼ��CMP�˲����˲�ʱ��Դ */
    CMPInitStruct->m_Clock = CMP0_CLKSOURCE_SCLK;
    /* ��ʼ��CMP�˲����˲�ʱ�ӷ�Ƶ*/
    CMPInitStruct->m_FrequencyDivision = 0x00;//1��Ƶ��ֵ��Χ0-255��
    /* ��ʼ�� CMPȡ������ѡ�� */
    CMPInitStruct->m_SampleNumber = CMP0_FLT_OFF;
    /* ��ʼ��CMP�˲���ʹ��λ */
    CMPInitStruct->m_FilterEnable = FALSE;
    /* ��ʼ��CMP��Χ����ʹ��λ*/
    CMPInitStruct->m_ScopecontrolEnable = FALSE;
    /* ��ʼ�� CMP�������ѡ��*/
    CMPInitStruct->m_OutputPolarity =CMP0_OUTPUT_Normal;
    /* ��ʼ��CMP�Ƚ���3ʹ�� */
    CMPInitStruct->m_CmpEnable =FALSE;
}
/**
  * ����  CMP0��������˿��ź�ѡ��
  * ����ѡ��:
  *           CMP0_PositiveINPUT_PIN_PA0
  *           CMP0_PositiveINPUT_PIN_PA9
  *           CMP0_PositiveINPUT_PIN_PB2
  *           CMP0_PositiveINPUT_PIN_PB9
  *           CMP0_PositiveINPUT_PIN_PC11
  *           CMP0_PositiveINPUT_PIN_AGND
  *           CMP0_PositiveINPUT_PIN_DAC0OUT
  *           CMP0_PositiveINPUT_PIN_DAC1OUT
  * ����  �ޡ�
  */
void
CMP0_POSITIVE_INPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP0_PositiveINPUT_PIN(Select));
    /*------------- ����CMP_CTL0�Ĵ���PMODλ -------------*/
    CMP_CTL0 = SFR_Config (CMP_CTL0,
                               ~CMP_CTL0_PMOD,
                               Select);
}
/**
  * ����  CMP0��������˿��ź�ѡ��
  * ����ѡ��:
  *           CMP0_NegativeINPUT_PIN_PA1
  *           CMP0_NegativeINPUT_PIN_PA10
  *           CMP0_NegativeINPUT_PIN_PB3
  *           CMP0_NegativeINPUT_PIN_PB10
  *           CMP0_NegativeINPUT_PIN_PC12
  *           CMP0_NegativeINPUT_PIN_AGND
  *           CMP0_NegativeINPUT_PIN_DAC0OUT
  *           CMP0_NegativeINPUT_PIN_DAC1OUT
  * ����  �ޡ�
  */
void
CMP0_NEGATIVE_INPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP0_NegativeINPUT_PIN(Select));
    /*------------- ����CMP_CTL0�Ĵ���NMODλ -------------*/
    CMP_CTL0 = SFR_Config (CMP_CTL0,
                               ~CMP_CTL0_NMOD,
                               Select);
}
/**
  * ����  CMP1��������˿��ź�ѡ��
  * ����ѡ��:
  *           CMP1_PositiveINPUT_PIN_PA0
  *           CMP1_PositiveINPUT_PIN_PA9
  *           CMP1_PositiveINPUT_PIN_PB2
  *           CMP1_PositiveINPUT_PIN_PB11
  *           CMP1_PositiveINPUT_PIN_PG7
  *           CMP1_PositiveINPUT_PIN_AGND
  *           CMP1_PositiveINPUT_PIN_DAC0OUT
  *           CMP1_PositiveINPUT_PIN_DAC1OUT
  * ����  �ޡ�
  */
void
CMP1_POSITIVE_INPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP1_PositiveINPUT_PIN(Select));
    /*------------- ����CMP_CTL1�Ĵ���PMODλ -------------*/
    CMP_CTL1 = SFR_Config (CMP_CTL1,
                               ~CMP_CTL1_PMOD,
                               Select);
}
/**
  * ����  CMP1��������˿��ź�ѡ��
  * ����ѡ��:
  *           CMP1_NegativeINPUT_PIN_PA1
  *           CMP1_NegativeINPUT_PIN_PA10
  *           CMP1_NegativeINPUT_PIN_PB3
  *           CMP1_NegativeINPUT_PIN_PB12
  *           CMP1_NegativeINPUT_PIN_PC9
  *           CMP1_NegativeINPUT_PIN_AGND
  *           CMP1_NegativeINPUT_PIN_DAC0OUT
  *           CMP1_NegativeINPUT_PIN_DAC1OUT
  * ����  �ޡ�
  */
void
CMP1_NEGATIVE_INPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP1_NegativeINPUT_PIN(Select));
    /*------------- ����CMP_CTL1�Ĵ���NMODλ -------------*/
    CMP_CTL1 = SFR_Config (CMP_CTL1,
                               ~CMP_CTL1_NMOD,
                               Select);
}
/**
  * ����  CMP2��������˿��ź�ѡ��
  * ����ѡ��:
  *           CMP2_PositiveINPUT_PIN_PA0
  *           CMP2_PositiveINPUT_PIN_PA9
  *           CMP2_PositiveINPUT_PIN_PB2
  *           CMP2_PositiveINPUT_PIN_PB13
  *           CMP2_PositiveINPUT_PIN_PC7
  *           CMP2_PositiveINPUT_PIN_AGND
  *           CMP2_PositiveINPUT_PIN_DAC0OUT
  *           CMP2_PositiveINPUT_PIN_DAC1OUT
  * ����  �ޡ�
  */
void
CMP2_POSITIVE_INPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP2_PositiveINPUT_PIN(Select));
    /*------------- ����CMP_CTL2�Ĵ���PMODλ -------------*/
    CMP_CTL2 = SFR_Config (CMP_CTL2,
                               ~CMP_CTL2_PMOD,
                               Select);
}
/**
  * ����  CMP2��������˿��ź�ѡ��
  * ����ѡ��:
  *           CMP2_NegativeINPUT_PIN_PA1
  *           CMP2_NegativeINPUT_PIN_PA10
  *           CMP2_NegativeINPUT_PIN_PB3
  *           CMP2_NegativeINPUT_PIN_PB14
  *           CMP2_NegativeINPUT_PIN_PC8
  *           CMP2_NegativeINPUT_PIN_AGND
  *           CMP2_NegativeINPUT_PIN_DAC0OUT
  *           CMP2_NegativeINPUT_PIN_DAC1OUT
  * ����  �ޡ�
  */
void
CMP2_NEGATIVE_INPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP2_NegativeINPUT_PIN(Select));
    /*------------- ����CMP_CTL2�Ĵ���NMODλ -------------*/
    CMP_CTL2 = SFR_Config (CMP_CTL2,
                               ~CMP_CTL2_NMOD,
                               Select);
}
/**
  * ����  CMP3��������˿��ź�ѡ��
  * ����ѡ��:
  *  		  CMP3_PositiveINPUT_PIN_OP2OUT
  *           CMP3_PositiveINPUT_PIN_PA0
  *           CMP3_PositiveINPUT_PIN_PA9
  *           CMP3_PositiveINPUT_PIN_PB2
  *           CMP3_PositiveINPUT_PIN_PB15
  *           CMP3_PositiveINPUT_PIN_PC5
  *           CMP3_PositiveINPUT_PIN_AGND
  *           CMP3_PositiveINPUT_PIN_DAC0OUT
  *           CMP3_PositiveINPUT_PIN_DAC1OUT
  * ����  �ޡ�
  */
void
CMP3_POSITIVE_INPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP3_PositiveINPUT_PIN(Select));

    if(Select==CMP3_PositiveINPUT_PIN_OP2OUT)
    {
    	SFR_SET_BIT_ASM(CMP3_SFR->CTL, CMP_CTL3_PSEL_POS);
    }
    else
    {
    /*------------- ����CMP_CTL3�Ĵ���PMODλ -------------*/
    SFR_CLR_BIT_ASM(CMP3_SFR->CTL, CMP_CTL3_PSEL_POS);
    CMP_CTL3 = SFR_Config (CMP_CTL3,
                               ~CMP_CTL3_PMOD,
                               Select);
    }
}
/**
  * ����  CMP3��������˿��ź�ѡ��
  * ����ѡ��:
  *           CMP3_NegativeINPUT_PIN_PA1
  *           CMP3_NegativeINPUT_PIN_PA10
  *           CMP3_NegativeINPUT_PIN_PB3
  *           CMP3_NegativeINPUT_PIN_PF0
  *           CMP3_NegativeINPUT_PIN_PC6
  *           CMP3_NegativeINPUT_PIN_AGND
  *           CMP3_NegativeINPUT_PIN_DAC0OUT
  *           CMP3_NegativeINPUT_PIN_DAC1OUT
  * ����  �ޡ�
  */
void
CMP3_NEGATIVE_INPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP3_NegativeINPUT_PIN(Select));
    /*------------- ����CMP_CTL3�Ĵ���NMODλ -------------*/
    CMP_CTL3 = SFR_Config (CMP_CTL3,
                               ~CMP_CTL3_NMOD,
                               Select);
}

/**
  * ����  CMP0�������ѡ��
  * ����ѡ��:
  *           CMP0_OUTPUT_Normal
  *           CMP0_OUTPUT_Opposite
  * ����  �ޡ�
  */
void
CMP0_OUTPUT_POL_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP0_OUTPUT(Select));
    /*------------- ����CMP_CTL0�Ĵ���POLλ -------------*/
    CMP_CTL0 = SFR_Config (CMP_CTL0,
                               ~CMP_CTL0_POL,
                               Select);
}
/**
  * ����  CMP1�������ѡ��
  * ����ѡ��:
  *           CMP1_OUTPUT_Normal
  *           CMP1_OUTPUT_Opposite
  * ����  �ޡ�
  */
void
CMP1_OUTPUT_POL_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP1_OUTPUT(Select));
    /*------------- ����CMP_CTL1�Ĵ���POLλ -------------*/
    CMP_CTL1 = SFR_Config (CMP_CTL1,
                               ~CMP_CTL1_POL,
                               Select);
}
/**
  * ����  CMP2�������ѡ��
  * ����ѡ��:
  *           CMP2_OUTPUT_Normal
  *           CMP2_OUTPUT_Opposite
  * ����  �ޡ�
  */
void
CMP2_OUTPUT_POL_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP2_OUTPUT(Select));
    /*------------- ����CMP_CTL2�Ĵ���POLλ -------------*/
    CMP_CTL2 = SFR_Config (CMP_CTL2,
                               ~CMP_CTL2_POL,
                               Select);
}
/**
  * ����  CMP3�������ѡ��
  * ����ѡ��:
  *           CMP3_OUTPUT_Normal
  *           CMP3_OUTPUT_Opposite
  * ����  �ޡ�
  */
void
CMP3_OUTPUT_POL_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP3_OUTPUT(Select));
    /*------------- ����CMP_CTL2�Ĵ���POLλ -------------*/
    CMP_CTL3 = SFR_Config (CMP_CTL3,
                               ~CMP_CTL3_POL,
                               Select);
}

/**
  * ����  CMP���ѡ��
  * ����ѡ��:
  * 		  NONE
  *           CMP0_OUTPUT
  *           CMP1_OUTPUT
  *           CMP2_OUTPUT
  * ����  �ޡ�
  */
void
CMP_OUTPUT_SELECT (uint32_t Select)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP_OUTPUT(Select));
    /*------------- ����CMP_CTL2�Ĵ���POLλ -------------*/
    CMP_CTL4 = SFR_Config (CMP_CTL4,
                               ~CMP_CTL4_CMPOUT,
                               Select);
}
/**
  * ����  ����CMP0ʹ��λ��
  * ����NewState: CMPʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
CMP0_Cmd (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ʹ��CMP */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0EN_POS);
    }
    else
    {
        /* ��ֹCMP */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0EN_POS);
    }
}
/**
  * ����  ����CMP1ʹ��λ��
  * ����NewState: CMPʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
CMP1_Cmd (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ʹ��CMP */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1EN_POS);
    }
    else
    {
        /* ��ֹCMP */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1EN_POS);
    }
}
/**
  * ����  ����CMP2ʹ��λ��
  * ����NewState: CMPʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
CMP2_Cmd (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ʹ��CMP */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2EN_POS);
    }
    else
    {
        /* ��ֹCMP */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2EN_POS);
    }
}
/**
  * ����  ����CMP3ʹ��λ��
  * ����NewState: CMPʹ��λ������Ϣ��
  *                 ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
CMP3_Cmd (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* ʹ��CMP */
        SFR_SET_BIT_ASM(CMP3_SFR->CTL, CMP_CTL3_C3EN_POS);
    }
    else
    {
        /* ��ֹCMP */
        SFR_CLR_BIT_ASM(CMP3_SFR->CTL, CMP_CTL3_C3EN_POS);
    }
}
/**
  *   ##### ��ʼ�������ú������� #####
  */


/**
  * ����  ��CMP0������ݡ�
  * ����  : ��
  * ����  ���Ƚ������״̬��
  */
FlagStatus
CMP0_Read_Output_State ()
{
    /*---------------- ��ȡCMP_CTL4�Ĵ���C0OUTλ ----------------*/
	if (CMPCTL4_SFR->CTL & CMP_CTL4_C0OUT)
	{
		/* �Ƚ������Ϊ1 */
		return SET;
	}
	else
	{
		/* �Ƚ������Ϊ0 */
		return RESET;
	}
}
/**
  * ����  ��CMP1������ݡ�
  * ����  : ��
  * ����  ���Ƚ������״̬��
  */
FlagStatus
CMP1_Read_Output_State ()
{
    /*---------------- ��ȡCMP_CTL4�Ĵ���C1OUTλ ----------------*/
	if (CMPCTL4_SFR->CTL & CMP_CTL4_C1OUT)
	{
		/* �Ƚ������Ϊ1 */
		return SET;
	}
	else
	{
		/* �Ƚ������Ϊ0 */
		return RESET;
	}
}
/**
  * ����  ��CMP2������ݡ�
  * ����  : ��
  * ����  ���Ƚ������״̬��
  */
FlagStatus
CMP2_Read_Output_State ()
{
    /*---------------- ��ȡCMP_CTL4�Ĵ���C2OUTλ ----------------*/
	if (CMPCTL4_SFR->CTL & CMP_CTL4_C2OUT)
	{
		/* �Ƚ������Ϊ1 */
		return SET;
	}
	else
	{
		/* �Ƚ������Ϊ0 */
		return RESET;
	}
}
/**
  * ����  ��CMP3������ݡ�
  * ����  : ��
  * ����  ���Ƚ������״̬��
  */
FlagStatus
CMP3_Read_Output_State ()
{
    /*---------------- ��ȡCMP_CTL4�Ĵ���C3OUTλ ----------------*/
	if (CMPCTL4_SFR->CTL & CMP_CTL4_C3OUT)
	{
		/* �Ƚ������Ϊ1 */
		return SET;
	}
	else
	{
		/* �Ƚ������Ϊ0 */
		return RESET;
	}
}
/**
  * ����  ��CMP0�жϱ�־��
  * ����  : ��
  * ����  ���Ƚ����жϱ�־״̬��
  */
FlagStatus
CMP0_Get_Updata_INT_Flag ()
{
    /*-------------------- ��ȡCMP_CTL4�Ĵ���C0IFλ --------------------*/
    if (CMPCTL4_SFR->CTL & CMP_CTL4_C0IF)
    {
        /* ��⵽�Ƚ���0�ж� */
        return SET;
    }
    else
    {
        /* û�м�⵽�Ƚ���0�ж� */
        return RESET;
    }
}
/**
  * ����  ��CMP1�жϱ�־��
  * ����  : ��
  * ����  ���Ƚ����жϱ�־״̬��
  */
FlagStatus
CMP1_Get_Updata_INT_Flag ()
{
    /*-------------------- ��ȡCMP_CTL4�Ĵ���C1IFλ --------------------*/
    if (CMPCTL4_SFR->CTL & CMP_CTL4_C1IF)
    {
        /* ��⵽�Ƚ���1�ж� */
        return SET;
    }
    else
    {
        /* û�м�⵽�Ƚ���1�ж� */
        return RESET;
    }
}
/**
  * ����  ��CMP2�жϱ�־��
  * ����  : ��
  * ����  ���Ƚ����жϱ�־״̬��
  */
FlagStatus
CMP2_Get_Updata_INT_Flag ()
{
    /*-------------------- ��ȡCMP_CTL4�Ĵ���C2IFλ --------------------*/
    if (CMPCTL4_SFR->CTL & CMP_CTL4_C2IF)
    {
        /* ��⵽�Ƚ���2�ж� */
        return SET;
    }
    else
    {
        /* û�м�⵽�Ƚ���2�ж� */
        return RESET;
    }
}
/**
  * ����  ��CMP3�жϱ�־��
  * ����  : ��
  * ����  ���Ƚ����жϱ�־״̬��
  */
FlagStatus
CMP3_Get_Updata_INT_Flag ()
{
    /*-------------------- ��ȡCMP_CTL4�Ĵ���C3IFλ --------------------*/
    if (CMPCTL4_SFR->CTL & CMP_CTL4_C3IF)
    {
        /* ��⵽�Ƚ���3�ж� */
        return SET;
    }
    else
    {
        /* û�м�⵽�Ƚ���3�ж� */
        return RESET;
    }
}

/**
  * ����  ���ô���ѡ��λ��
  * ���� TriggerSelect: ����ѡ��
  *                      ȡֵ��ΧΪ��
  *                         CMP_CMPOUT_FlipLatch_INT
  *                         CMP_CMPOUT_Change_INT
  * ����  ��
  */
void
CMP_Trigger_Select_Config (uint32_t TriggerSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CMP_TriggerINT_MODE(TriggerSelect));

    /*-------------------- ����CMP_CTL4�Ĵ���CIFMSλ --------------------*/
    CMPCTL4_SFR->CTL = SFR_Config (CMPCTL4_SFR->CTL,
                          ~CMP_CTL4_CIFMS,
                          TriggerSelect);
}

/**
  * ����  ����Ƚ���0�жϱ�־��
  * ���� : ��
  * ����  ��
  */
void
CMP0_Clear_Trigger_INT_Flag ()
{
    /*-------------------- ����CMPCTL4�Ĵ���C0ICλ --------------------*/
    SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0IC_POS);
    while((CMP_CTL4 & CMP_CTL4_C0IF)>>CMP_CTL4_C0IF_POS);
    SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0IC_POS);
}
/**
  * ����  ����Ƚ���1�жϱ�־��
  * ���� : ��
  * ����  ��
  */
void
CMP1_Clear_Trigger_INT_Flag ()
{
    /*-------------------- ����CMPCTL4�Ĵ���C1ICλ --------------------*/
    SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1IC_POS);
    while((CMP_CTL4 & CMP_CTL4_C1IF)>>CMP_CTL4_C1IF_POS);
    SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1IC_POS);
}
/**
  * ����  ����Ƚ���2�жϱ�־��
  * ���� : ��
  * ����  ��
  */
void
CMP2_Clear_Trigger_INT_Flag ()
{
    /*-------------------- ����CMPCTL4�Ĵ���C2ICλ --------------------*/
    SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2IC_POS);
    while((CMP_CTL4 & CMP_CTL4_C2IF)>>CMP_CTL4_C2IF_POS);
    SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2IC_POS);
}
/**
  * ����  ����Ƚ���3�жϱ�־��
  * ���� : ��
  * ����  ��
  */
void
CMP3_Clear_Trigger_INT_Flag ()
{
    /*-------------------- ����CMPCTL4�Ĵ���C3ICλ --------------------*/
    SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IC_POS);
    while((CMP_CTL4 & CMP_CTL4_C3IF)>>CMP_CTL4_C3IF_POS);
    SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IC_POS);
}

/**
  * ����  ����CMP�ж�ʹ��
  * ���� NewState: �ж�ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
CMP0_INT_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CMPCTL4�Ĵ���C0IEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Ƚ����ж� */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0IE_POS);
    }
    else
    {
        /* ��ֹ�Ƚ����ж� */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0IE_POS);
    }
}
void
CMP1_INT_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CMPCTL4�Ĵ���C1IEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Ƚ����ж� */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1IE_POS);
    }
    else
    {
        /* ��ֹ�Ƚ����ж� */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1IE_POS);
    }
}
void
CMP2_INT_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CMPCTL4�Ĵ���C2IEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Ƚ����ж� */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2IE_POS);
    }
    else
    {
        /* ��ֹ�Ƚ����ж� */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2IE_POS);
    }
}
void
CMP3_INT_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CMPCTL4�Ĵ���C3IEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Ƚ����ж� */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IE_POS);
    }
    else
    {
        /* ��ֹ�Ƚ����ж� */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IE_POS);
    }
}

/**
  * ����  ����CMP���͵�ѹѡ��λ
  * ���� Select: ȡֵ��ΧΪ��
  * CMP_SluggishVoltage_OFF
  * CMP_SluggishVoltage_5mV
  * CMP_SluggishVoltage_10mV
  * CMP_SluggishVoltage_15mV
  * ����  ��
  */
void
CMP_SluggishVoltage_Select (uint32_t Select)
{
    CHECK_RESTRICTION(CHECK_CMP_SluggishVoltage_SEL(Select));

    /*-------------------- ����CMP_CTL4�Ĵ���TRIMλ --------------------*/
    CMPCTL4_SFR->CTL = SFR_Config (CMPCTL4_SFR->CTL,
                          ~CMP_CTL4_TRIM,
                          Select);
}
/**
  * ����  ����ģʽѡ��λ
  * ���� Select: ȡֵ��ΧΪ��
  * CMP_HALLMODE_SINGLE
  * CMP_HALLMODE_BOTH
  * ����  ��
  */
void
CMP_HALLMODE_Select (uint32_t Select)
{
    CHECK_RESTRICTION(CHECK_CMP_HALLMODE_SEL(Select));

    /*-------------------- ����CMP_CTL4�Ĵ���TRIMλ --------------------*/
    CMPCTL4_SFR->CTL = SFR_Config (CMPCTL4_SFR->CTL,
                          ~CMP_CTL4_HALSEL,
                          Select);
}
/**
  * ����  ����綯��ģʽʹ��
  * ���� NewState: �ж�ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
CMP_BEMF_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CMP_CTL4�Ĵ���BEMFENλ --------------------*/
    if (NewState != FALSE)
    {
        /* ������綯��ģʽʹ�� */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_BEMFEN_POS);
    }
    else
    {
        /* ��ֹ����綯��ģʽʹ�� */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_BEMFEN_POS);
    }
}
/**
  * ����  �˲�������ѡ��λ
  * ���� Select: ȡֵ��ΧΪ��
  * CMP_FLTINSEL_CMPOUT
  * CMP_FLTINSEL_IO
  * ����  ��
  */
void
CMP_FLTINSEL_Select (uint32_t Select)
{
    CHECK_RESTRICTION(CHECK_CMP_FLTINSEL(Select));

    /*-------------------- ����CMP_CTL4�Ĵ���FLTINSELλ --------------------*/
    CMPCTL4_SFR->CTL = SFR_Config (CMPCTL4_SFR->CTL,
                          ~CMP_CTL4_FLTINSEL,
                          Select);
}

#endif //KF32A_Periph_cmp
/**
  *   ##### ���ݶ�д�������� #####
  */



/**
  *   ##### CMPУ��Ĵ������ú������� #####
  */

/**
  ******************************************************************************
  * �ļ���  kf32a_basic_cmp.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ����    ���ļ����ڱȽ���(CMP)����Ŀ⺯����������غ궨�塣
  *
  *********************************************************************
  */

#ifndef _KF32A_BASIC_CMP_H_
#define _KF32A_BASIC_CMP_H_

#include "KF32A_BASIC.h"


/**
  * CMP�����ж�
  */
#define CHECK_CMP_ALL_PERIPH(PERIPH)    (((PERIPH) == CMP0_SFR) \
										|| ((PERIPH) == CMP1_SFR)\
										|| ((PERIPH) == CMP2_SFR)\
                                      || ((PERIPH) == CMP3_SFR))

/**
  * ����  CMP������Ϣ�ṹ��
  */
typedef struct
{
	uint32_t m_PositiveInput;    /* CMP���������ѡ��λ
                                           ȡֵΪ�ꡰCMP��������ѡ���е�һ�� */
    uint32_t m_NegativeInput;     /* CMP�ĸ������ѡ��λ
                                           ȡֵΪ�ꡰCMP��������ѡ���е�һ�� */
    FunctionalState m_FallTriggerEnable; /* CMP�½��ش����ж�ʹ�ܣ�ֻ��CMP0/CMP1/CMP2
                                           ȡֵΪTRUE��FALSE */
    FunctionalState m_RiseTriggerEnable; /* CMP�����ش����ж�ʹ��.ֻ��CMP0/CMP1/CMP2
                                             ȡֵΪTRUE��FALSE */
    uint32_t m_Clock;                   /* CMP�˲����˲�ʱ��Դ
                                           ȡֵΪ�ꡰCMP�˲����˲�ʱ��Դѡ��λ���е�һ�� */
    uint32_t m_FrequencyDivision;        /* CMP�˲����˲�ʱ�ӷ�Ƶ
                                           ȡֵΪ  8λ���ݣ�0-255 �е�һ�� */
    uint32_t m_SampleNumber;     /* CMPȡ������ѡ��
                                            ȡֵΪ�ꡰCMP�˲���ȡ������ѡ���е�һ�� */
    FunctionalState m_FilterEnable;   /* CMP�˲���ʹ��λ��
                                           ȡֵΪTRUE��FALSE */
    FunctionalState m_ScopecontrolEnable;  /* CMP��Χ����ʹ��λ��
                                           ȡֵΪTRUE��FALSE */
    uint32_t m_OutputPolarity;        /* CMP�������ѡ��
                                           ȡֵΪ�ꡰCMP�������ѡ���е�һ�� */
    FunctionalState m_CmpEnable;     /* CMP�Ƚ���ʹ��
                                            ȡֵΪTRUE��FALSE */
}CMP_InitTypeDef;//	CMP0/CMP1/CMP2/CMP3



/**
  * CMP0��������ѡ��
  */
#define CMP0_PositiveINPUT_PIN_PA0                ((uint32_t)0 << CMP_CTL0_PMOD0_POS)
#define CMP0_PositiveINPUT_PIN_PA9                ((uint32_t)1 << CMP_CTL0_PMOD0_POS)
#define CMP0_PositiveINPUT_PIN_PB2                ((uint32_t)2 << CMP_CTL0_PMOD0_POS)
#define CMP0_PositiveINPUT_PIN_PB9                ((uint32_t)3 << CMP_CTL0_PMOD0_POS)
#define CMP0_PositiveINPUT_PIN_PC11               ((uint32_t)4 << CMP_CTL0_PMOD0_POS)
#define CMP0_PositiveINPUT_PIN_AGND               ((uint32_t)5 << CMP_CTL0_PMOD0_POS)
#define CMP0_PositiveINPUT_PIN_DAC0OUT            ((uint32_t)6 << CMP_CTL0_PMOD0_POS)
#define CMP0_PositiveINPUT_PIN_DAC1OUT            ((uint32_t)7 << CMP_CTL0_PMOD0_POS)
#define CHECK_CMP0_PositiveINPUT_PIN(SEL)         (((SEL) == CMP0_PositiveINPUT_PIN_PA0) \
										          || ((SEL) == CMP0_PositiveINPUT_PIN_PA9)\
										          || ((SEL) == CMP0_PositiveINPUT_PIN_PB2)\
										          || ((SEL) == CMP0_PositiveINPUT_PIN_PB9)\
										          || ((SEL) == CMP0_PositiveINPUT_PIN_PC11)\
										          || ((SEL) == CMP0_PositiveINPUT_PIN_AGND)\
										          || ((SEL) == CMP0_PositiveINPUT_PIN_DAC0OUT)\
                                                  || ((SEL) == CMP0_PositiveINPUT_PIN_DAC1OUT))
/**
  * CMP1��������ѡ��
  */
#define CMP1_PositiveINPUT_PIN_PA0                ((uint32_t)0 << CMP_CTL1_PMOD0_POS)
#define CMP1_PositiveINPUT_PIN_PA9                ((uint32_t)1 << CMP_CTL1_PMOD0_POS)
#define CMP1_PositiveINPUT_PIN_PB2                ((uint32_t)2 << CMP_CTL1_PMOD0_POS)
#define CMP1_PositiveINPUT_PIN_PB11               ((uint32_t)3 << CMP_CTL1_PMOD0_POS)
#define CMP1_PositiveINPUT_PIN_PG7                ((uint32_t)4 << CMP_CTL1_PMOD0_POS)
#define CMP1_PositiveINPUT_PIN_AGND               ((uint32_t)5 << CMP_CTL1_PMOD0_POS)
#define CMP1_PositiveINPUT_PIN_DAC0OUT            ((uint32_t)6 << CMP_CTL1_PMOD0_POS)
#define CMP1_PositiveINPUT_PIN_DAC1OUT            ((uint32_t)7 << CMP_CTL1_PMOD0_POS)
#define CHECK_CMP1_PositiveINPUT_PIN(SEL)         (((SEL) == CMP1_PositiveINPUT_PIN_PA0) \
										          || ((SEL) == CMP1_PositiveINPUT_PIN_PA9)\
										          || ((SEL) == CMP1_PositiveINPUT_PIN_PB2)\
										          || ((SEL) == CMP1_PositiveINPUT_PIN_PB11)\
										          || ((SEL) == CMP1_PositiveINPUT_PIN_PG7)\
										          || ((SEL) == CMP1_PositiveINPUT_PIN_AGND)\
										          || ((SEL) == CMP1_PositiveINPUT_PIN_DAC0OUT)\
                                                  || ((SEL) == CMP1_PositiveINPUT_PIN_DAC1OUT))
/**
  * CMP2��������ѡ��
  */
#define CMP2_PositiveINPUT_PIN_PA0                ((uint32_t)0 << CMP_CTL2_PMOD0_POS)
#define CMP2_PositiveINPUT_PIN_PA9                ((uint32_t)1 << CMP_CTL2_PMOD0_POS)
#define CMP2_PositiveINPUT_PIN_PB2                ((uint32_t)2 << CMP_CTL2_PMOD0_POS)
#define CMP2_PositiveINPUT_PIN_PB13               ((uint32_t)3 << CMP_CTL2_PMOD0_POS)
#define CMP2_PositiveINPUT_PIN_PC7                ((uint32_t)4 << CMP_CTL2_PMOD0_POS)
#define CMP2_PositiveINPUT_PIN_AGND               ((uint32_t)5 << CMP_CTL2_PMOD0_POS)
#define CMP2_PositiveINPUT_PIN_DAC0OUT            ((uint32_t)6 << CMP_CTL2_PMOD0_POS)
#define CMP2_PositiveINPUT_PIN_DAC1OUT            ((uint32_t)7 << CMP_CTL2_PMOD0_POS)
#define CHECK_CMP2_PositiveINPUT_PIN(SEL)         (((SEL) == CMP2_PositiveINPUT_PIN_PA0) \
										          || ((SEL) == CMP2_PositiveINPUT_PIN_PA9)\
										          || ((SEL) == CMP2_PositiveINPUT_PIN_PB2)\
										          || ((SEL) == CMP2_PositiveINPUT_PIN_PB13)\
										          || ((SEL) == CMP2_PositiveINPUT_PIN_PC7)\
										          || ((SEL) == CMP2_PositiveINPUT_PIN_AGND)\
										          || ((SEL) == CMP2_PositiveINPUT_PIN_DAC0OUT)\
                                                  || ((SEL) == CMP2_PositiveINPUT_PIN_DAC1OUT))
/**
  * CMP3��������ѡ��
  */
#define CMP3_PositiveINPUT_PIN_OP2OUT			  ((uint32_t)1 << CMP_CTL3_PSEL_POS)
#define CMP3_PositiveINPUT_PIN_PA0                ((uint32_t)0 << CMP_CTL3_PMOD0_POS)
#define CMP3_PositiveINPUT_PIN_PA9                ((uint32_t)1 << CMP_CTL3_PMOD0_POS)
#define CMP3_PositiveINPUT_PIN_PB2                ((uint32_t)2 << CMP_CTL3_PMOD0_POS)
#define CMP3_PositiveINPUT_PIN_PB15               ((uint32_t)3 << CMP_CTL3_PMOD0_POS)
#define CMP3_PositiveINPUT_PIN_PC5                ((uint32_t)4 << CMP_CTL3_PMOD0_POS)
#define CMP3_PositiveINPUT_PIN_AGND               ((uint32_t)5 << CMP_CTL3_PMOD0_POS)
#define CMP3_PositiveINPUT_PIN_DAC0OUT            ((uint32_t)6 << CMP_CTL3_PMOD0_POS)
#define CMP3_PositiveINPUT_PIN_DAC1OUT            ((uint32_t)7 << CMP_CTL3_PMOD0_POS)
#define CHECK_CMP3_PositiveINPUT_PIN(SEL)         (((SEL) == CMP3_PositiveINPUT_PIN_PA0) \
												  || ((SEL) == CMP3_PositiveINPUT_PIN_OP2OUT)\
										          || ((SEL) == CMP3_PositiveINPUT_PIN_PA9)\
										          || ((SEL) == CMP3_PositiveINPUT_PIN_PB2)\
										          || ((SEL) == CMP3_PositiveINPUT_PIN_PB15)\
										          || ((SEL) == CMP3_PositiveINPUT_PIN_PC5)\
										          || ((SEL) == CMP3_PositiveINPUT_PIN_AGND)\
										          || ((SEL) == CMP3_PositiveINPUT_PIN_DAC0OUT)\
                                                  || ((SEL) == CMP3_PositiveINPUT_PIN_DAC1OUT))

/**
  * CMP0��������ѡ��
  */
#define CMP0_NegativeINPUT_PIN_PA1                ((uint32_t)0 << CMP_CTL0_NMOD0_POS)
#define CMP0_NegativeINPUT_PIN_PA10               ((uint32_t)1 << CMP_CTL0_NMOD0_POS)
#define CMP0_NegativeINPUT_PIN_PB3                ((uint32_t)2 << CMP_CTL0_NMOD0_POS)
#define CMP0_NegativeINPUT_PIN_PB10               ((uint32_t)3 << CMP_CTL0_NMOD0_POS)
#define CMP0_NegativeINPUT_PIN_PC12               ((uint32_t)4 << CMP_CTL0_NMOD0_POS)
#define CMP0_NegativeINPUT_PIN_AGND               ((uint32_t)5 << CMP_CTL0_NMOD0_POS)
#define CMP0_NegativeINPUT_PIN_DAC0OUT            ((uint32_t)6 << CMP_CTL0_NMOD0_POS)
#define CMP0_NegativeINPUT_PIN_DAC1OUT            ((uint32_t)7 << CMP_CTL0_NMOD0_POS)
#define CHECK_CMP0_NegativeINPUT_PIN(SEL)         (((SEL) == CMP0_NegativeINPUT_PIN_PA1) \
										          || ((SEL) == CMP0_NegativeINPUT_PIN_PA10)\
										          || ((SEL) == CMP0_NegativeINPUT_PIN_PB3)\
										          || ((SEL) == CMP0_NegativeINPUT_PIN_PB10)\
										          || ((SEL) == CMP0_NegativeINPUT_PIN_PC12)\
										          || ((SEL) == CMP0_NegativeINPUT_PIN_AGND)\
										          || ((SEL) == CMP0_NegativeINPUT_PIN_DAC0OUT)\
                                                  || ((SEL) == CMP0_NegativeINPUT_PIN_DAC1OUT))
/**
  * CMP1��������ѡ��
  */
#define CMP1_NegativeINPUT_PIN_PA1                ((uint32_t)0 << CMP_CTL1_NMOD0_POS)
#define CMP1_NegativeINPUT_PIN_PA10               ((uint32_t)1 << CMP_CTL1_NMOD0_POS)
#define CMP1_NegativeINPUT_PIN_PB3                ((uint32_t)2 << CMP_CTL1_NMOD0_POS)
#define CMP1_NegativeINPUT_PIN_PB12               ((uint32_t)3 << CMP_CTL1_NMOD0_POS)
#define CMP1_NegativeINPUT_PIN_PC9                ((uint32_t)4 << CMP_CTL1_NMOD0_POS)
#define CMP1_NegativeINPUT_PIN_AGND               ((uint32_t)5 << CMP_CTL1_NMOD0_POS)
#define CMP1_NegativeINPUT_PIN_DAC0OUT            ((uint32_t)6 << CMP_CTL1_NMOD0_POS)
#define CMP1_NegativeINPUT_PIN_DAC1OUT            ((uint32_t)7 << CMP_CTL1_NMOD0_POS)
#define CHECK_CMP1_NegativeINPUT_PIN(SEL)         (((SEL) == CMP1_NegativeINPUT_PIN_PA1) \
										          || ((SEL) == CMP1_NegativeINPUT_PIN_PA10)\
										          || ((SEL) == CMP1_NegativeINPUT_PIN_PB3)\
										          || ((SEL) == CMP1_NegativeINPUT_PIN_PB12)\
										          || ((SEL) == CMP1_NegativeINPUT_PIN_PC9)\
										          || ((SEL) == CMP1_NegativeINPUT_PIN_AGND)\
										          || ((SEL) == CMP1_NegativeINPUT_PIN_DAC0OUT)\
                                                  || ((SEL) == CMP1_NegativeINPUT_PIN_DAC1OUT))
/**
  * CMP2��������ѡ��
  */
#define CMP2_NegativeINPUT_PIN_PA1                ((uint32_t)0 << CMP_CTL2_NMOD0_POS)
#define CMP2_NegativeINPUT_PIN_PA10               ((uint32_t)1 << CMP_CTL2_NMOD0_POS)
#define CMP2_NegativeINPUT_PIN_PB3                ((uint32_t)2 << CMP_CTL2_NMOD0_POS)
#define CMP2_NegativeINPUT_PIN_PB14               ((uint32_t)3 << CMP_CTL2_NMOD0_POS)
#define CMP2_NegativeINPUT_PIN_PC8                ((uint32_t)4 << CMP_CTL2_NMOD0_POS)
#define CMP2_NegativeINPUT_PIN_AGND               ((uint32_t)5 << CMP_CTL2_NMOD0_POS)
#define CMP2_NegativeINPUT_PIN_DAC0OUT            ((uint32_t)6 << CMP_CTL2_NMOD0_POS)
#define CMP2_NegativeINPUT_PIN_DAC1OUT            ((uint32_t)7 << CMP_CTL2_NMOD0_POS)
#define CHECK_CMP2_NegativeINPUT_PIN(SEL)         (((SEL) == CMP2_NegativeINPUT_PIN_PA1) \
										          || ((SEL) == CMP2_NegativeINPUT_PIN_PA10)\
										          || ((SEL) == CMP2_NegativeINPUT_PIN_PB3)\
										          || ((SEL) == CMP2_NegativeINPUT_PIN_PB14)\
										          || ((SEL) == CMP2_NegativeINPUT_PIN_PC8)\
										          || ((SEL) == CMP2_NegativeINPUT_PIN_AGND)\
										          || ((SEL) == CMP2_NegativeINPUT_PIN_DAC0OUT)\
                                                  || ((SEL) == CMP2_NegativeINPUT_PIN_DAC1OUT))
/**
  * CMP3��������ѡ��
  */
#define CMP3_NegativeINPUT_PIN_PA1                ((uint32_t)0 << CMP_CTL3_NMOD0_POS)
#define CMP3_NegativeINPUT_PIN_PA10               ((uint32_t)1 << CMP_CTL3_NMOD0_POS)
#define CMP3_NegativeINPUT_PIN_PB3                ((uint32_t)2 << CMP_CTL3_NMOD0_POS)
#define CMP3_NegativeINPUT_PIN_PF0                ((uint32_t)3 << CMP_CTL3_NMOD0_POS)
#define CMP3_NegativeINPUT_PIN_PC6                ((uint32_t)4 << CMP_CTL3_NMOD0_POS)
#define CMP3_NegativeINPUT_PIN_AGND               ((uint32_t)5 << CMP_CTL3_NMOD0_POS)
#define CMP3_NegativeINPUT_PIN_DAC0OUT            ((uint32_t)6 << CMP_CTL3_NMOD0_POS)
#define CMP3_NegativeINPUT_PIN_DAC1OUT            ((uint32_t)7 << CMP_CTL3_NMOD0_POS)
#define CHECK_CMP3_NegativeINPUT_PIN(SEL)         (((SEL) == CMP3_NegativeINPUT_PIN_PA1) \
										          || ((SEL) == CMP3_NegativeINPUT_PIN_PA10)\
										          || ((SEL) == CMP3_NegativeINPUT_PIN_PB3)\
										          || ((SEL) == CMP3_NegativeINPUT_PIN_PF0)\
										          || ((SEL) == CMP3_NegativeINPUT_PIN_PC6)\
										          || ((SEL) == CMP3_NegativeINPUT_PIN_AGND)\
										          || ((SEL) == CMP3_NegativeINPUT_PIN_DAC0OUT)\
                                                  || ((SEL) == CMP3_NegativeINPUT_PIN_DAC1OUT))

/**
  * CMP0�˲����˲�ʱ��Դѡ��
  */
#define CMP0_CLKSOURCE_SCLK                      ((uint32_t)0x0 << CMP_CTL0_FLTCS0_POS)
#define CMP0_CLKSOURCE_HFCLK                     ((uint32_t)0x1 << CMP_CTL0_FLTCS0_POS)
#define CMP0_CLKSOURCE_LFCLK                     ((uint32_t)0x2 << CMP_CTL0_FLTCS0_POS)
#define CHECK_CMP0_CLKSOURCE_SEL(SEL)            (((SEL) == CMP0_CLKSOURCE_SCLK) \
										         || ((SEL) == CMP0_CLKSOURCE_HFCLK)\
										         || ((SEL) == CMP0_CLKSOURCE_LFCLK))
/**
  * CMP1�˲����˲�ʱ��Դѡ��
  */
#define CMP1_CLKSOURCE_SCLK                     ((uint32_t)0x0 << CMP_CTL1_FLTCS0_POS)
#define CMP1_CLKSOURCE_HFCLK                    ((uint32_t)0x1 << CMP_CTL1_FLTCS0_POS)
#define CMP1_CLKSOURCE_LFCLK                    ((uint32_t)0x2 << CMP_CTL1_FLTCS0_POS)
#define CHECK_CMP1_CLKSOURCE_SEL(SEL)           (((SEL) == CMP1_CLKSOURCE_SCLK) \
										        || ((SEL) == CMP1_CLKSOURCE_HFCLK)\
										        || ((SEL) == CMP1_CLKSOURCE_LFCLK))
/**
  * CMP2�˲����˲�ʱ��Դѡ��
  */
#define CMP2_CLKSOURCE_SCLK                    ((uint32_t)0x0 << CMP_CTL2_FLTCS0_POS)
#define CMP2_CLKSOURCE_HFCLK                   ((uint32_t)0x1 << CMP_CTL2_FLTCS0_POS)
#define CMP2_CLKSOURCE_LFCLK                   ((uint32_t)0x2 << CMP_CTL2_FLTCS0_POS)
#define CHECK_CMP2_CLKSOURCE_SEL(SEL)          (((SEL) == CMP2_CLKSOURCE_SCLK) \
										       || ((SEL) == CMP2_CLKSOURCE_HFCLK)\
										       || ((SEL) == CMP2_CLKSOURCE_LFCLK))
/**
  * CMP3�˲����˲�ʱ��Դѡ��
  */
#define CMP3_CLKSOURCE_SCLK                    ((uint32_t)0x0 << CMP_CTL3_FLTCS0_POS)
#define CMP3_CLKSOURCE_HFCLK                   ((uint32_t)0x1 << CMP_CTL3_FLTCS0_POS)
#define CMP3_CLKSOURCE_LFCLK                   ((uint32_t)0x2 << CMP_CTL3_FLTCS0_POS)
#define CHECK_CMP3_CLKSOURCE_SEL(SEL)          (((SEL) == CMP3_CLKSOURCE_SCLK) \
										       || ((SEL) == CMP3_CLKSOURCE_HFCLK)\
										       || ((SEL) == CMP3_CLKSOURCE_LFCLK))

/**
  * CMP0�˲���ȡ������ѡ��
  */
#define CMP0_FLT_OFF              			   ((uint32_t)0x0 << CMP_CTL0_FLTCNT0_POS)
#define CMP0_FLT_SampleNumber_1                ((uint32_t)0x1 << CMP_CTL0_FLTCNT0_POS)
#define CMP0_FLT_SampleNumber_2                ((uint32_t)0x2 << CMP_CTL0_FLTCNT0_POS)
#define CMP0_FLT_SampleNumber_3                ((uint32_t)0x3 << CMP_CTL0_FLTCNT0_POS)
#define CMP0_FLT_SampleNumber_4                ((uint32_t)0x4 << CMP_CTL0_FLTCNT0_POS)
#define CMP0_FLT_SampleNumber_5                ((uint32_t)0x5 << CMP_CTL0_FLTCNT0_POS)
#define CMP0_FLT_SampleNumber_6                ((uint32_t)0x6 << CMP_CTL0_FLTCNT0_POS)
#define CMP0_FLT_SampleNumber_7                ((uint32_t)0x7 << CMP_CTL0_FLTCNT0_POS)
#define CHECK_CMP0_FLT_SampleNumber_SEL(SEL)   (((SEL) == CMP0_FLT_OFF) \
										       || ((SEL) == CMP0_FLT_SampleNumber_1)\
										       || ((SEL) == CMP0_FLT_SampleNumber_2)\
										       || ((SEL) == CMP0_FLT_SampleNumber_3)\
										       || ((SEL) == CMP0_FLT_SampleNumber_4)\
										       || ((SEL) == CMP0_FLT_SampleNumber_5)\
										       || ((SEL) == CMP0_FLT_SampleNumber_6)\
										       || ((SEL) == CMP0_FLT_SampleNumber_7))

/**
  * CMP1�˲���ȡ������ѡ��
  */
#define CMP1_FLT_OFF              			   ((uint32_t)0x0 << CMP_CTL1_FLTCNT0_POS)
#define CMP1_FLT_SampleNumber_1                ((uint32_t)0x1 << CMP_CTL1_FLTCNT0_POS)
#define CMP1_FLT_SampleNumber_2                ((uint32_t)0x2 << CMP_CTL1_FLTCNT0_POS)
#define CMP1_FLT_SampleNumber_3                ((uint32_t)0x3 << CMP_CTL1_FLTCNT0_POS)
#define CMP1_FLT_SampleNumber_4                ((uint32_t)0x4 << CMP_CTL1_FLTCNT0_POS)
#define CMP1_FLT_SampleNumber_5                ((uint32_t)0x5 << CMP_CTL1_FLTCNT0_POS)
#define CMP1_FLT_SampleNumber_6                ((uint32_t)0x6 << CMP_CTL1_FLTCNT0_POS)
#define CMP1_FLT_SampleNumber_7                ((uint32_t)0x7 << CMP_CTL1_FLTCNT0_POS)
#define CHECK_CMP1_FLT_SampleNumber_SEL(SEL)   (((SEL) == CMP1_FLT_OFF) \
										       || ((SEL) == CMP1_FLT_SampleNumber_1)\
										       || ((SEL) == CMP1_FLT_SampleNumber_2)\
										       || ((SEL) == CMP1_FLT_SampleNumber_3)\
										       || ((SEL) == CMP1_FLT_SampleNumber_4)\
										       || ((SEL) == CMP1_FLT_SampleNumber_5)\
										       || ((SEL) == CMP1_FLT_SampleNumber_6)\
										       || ((SEL) == CMP1_FLT_SampleNumber_7))

/**
  * CMP2�˲���ȡ������ѡ��
  */
#define CMP2_FLT_OFF              			   ((uint32_t)0x0 << CMP_CTL2_FLTCNT0_POS)
#define CMP2_FLT_SampleNumber_1                ((uint32_t)0x1 << CMP_CTL2_FLTCNT0_POS)
#define CMP2_FLT_SampleNumber_2                ((uint32_t)0x2 << CMP_CTL2_FLTCNT0_POS)
#define CMP2_FLT_SampleNumber_3                ((uint32_t)0x3 << CMP_CTL2_FLTCNT0_POS)
#define CMP2_FLT_SampleNumber_4                ((uint32_t)0x4 << CMP_CTL2_FLTCNT0_POS)
#define CMP2_FLT_SampleNumber_5                ((uint32_t)0x5 << CMP_CTL2_FLTCNT0_POS)
#define CMP2_FLT_SampleNumber_6                ((uint32_t)0x6 << CMP_CTL2_FLTCNT0_POS)
#define CMP2_FLT_SampleNumber_7                ((uint32_t)0x7 << CMP_CTL2_FLTCNT0_POS)
#define CHECK_CMP2_FLT_SampleNumber_SEL(SEL)   (((SEL) == CMP1_FLT_OFF) \
										       || ((SEL) == CMP2_FLT_SampleNumber_1)\
										       || ((SEL) == CMP2_FLT_SampleNumber_2)\
										       || ((SEL) == CMP2_FLT_SampleNumber_3)\
										       || ((SEL) == CMP2_FLT_SampleNumber_4)\
										       || ((SEL) == CMP2_FLT_SampleNumber_5)\
										       || ((SEL) == CMP2_FLT_SampleNumber_6)\
										       || ((SEL) == CMP2_FLT_SampleNumber_7))

/**
  * CMP3�˲���ȡ������ѡ��
  */
#define CMP3_FLT_OFF              			   ((uint32_t)0x0 << CMP_CTL3_FLTCNT0_POS)
#define CMP3_FLT_SampleNumber_1                ((uint32_t)0x1 << CMP_CTL3_FLTCNT0_POS)
#define CMP3_FLT_SampleNumber_2                ((uint32_t)0x2 << CMP_CTL3_FLTCNT0_POS)
#define CMP3_FLT_SampleNumber_3                ((uint32_t)0x3 << CMP_CTL3_FLTCNT0_POS)
#define CMP3_FLT_SampleNumber_4                ((uint32_t)0x4 << CMP_CTL3_FLTCNT0_POS)
#define CMP3_FLT_SampleNumber_5                ((uint32_t)0x5 << CMP_CTL3_FLTCNT0_POS)
#define CMP3_FLT_SampleNumber_6                ((uint32_t)0x6 << CMP_CTL3_FLTCNT0_POS)
#define CMP3_FLT_SampleNumber_7                ((uint32_t)0x7 << CMP_CTL3_FLTCNT0_POS)
#define CHECK_CMP3_FLT_SampleNumber_SEL(SEL)   (((SEL) == CMP3_FLT_OFF) \
										       || ((SEL) == CMP3_FLT_SampleNumber_1)\
										       || ((SEL) == CMP3_FLT_SampleNumber_2)\
										       || ((SEL) == CMP3_FLT_SampleNumber_3)\
										       || ((SEL) == CMP3_FLT_SampleNumber_4)\
										       || ((SEL) == CMP3_FLT_SampleNumber_5)\
										       || ((SEL) == CMP3_FLT_SampleNumber_6)\
										       || ((SEL) == CMP3_FLT_SampleNumber_7))
/**
/**
  * CMP0�������ѡ��
  */
#define CMP0_OUTPUT_Normal                     ((uint32_t)0<<CMP_CTL0_POL_POS)
#define CMP0_OUTPUT_Opposite                   ((uint32_t)1<<CMP_CTL0_POL_POS)
#define CHECK_CMP0_OUTPUT(SEL)                 (((SEL) == CMP0_OUTPUT_Normal) \
        							           || ((SEL) == CMP0_OUTPUT_Opposite))
/**
/**
  * CMP1�������ѡ��
  */
#define CMP1_OUTPUT_Normal                     ((uint32_t)0<<CMP_CTL1_POL_POS)
#define CMP1_OUTPUT_Opposite                   ((uint32_t)1<<CMP_CTL1_POL_POS)
#define CHECK_CMP1_OUTPUT(SEL)                 (((SEL) == CMP1_OUTPUT_Normal) \
        							           || ((SEL) == CMP1_OUTPUT_Opposite))
/**
/**
  * CMP2�������ѡ��
  */
#define CMP2_OUTPUT_Normal                     ((uint32_t)0<<CMP_CTL2_POL_POS)
#define CMP2_OUTPUT_Opposite                   ((uint32_t)1<<CMP_CTL2_POL_POS)
#define CHECK_CMP2_OUTPUT(SEL)                 (((SEL) == CMP2_OUTPUT_Normal) \
        							           || ((SEL) == CMP2_OUTPUT_Opposite))
/**
/**
  * CMP3�������ѡ��
  */
#define CMP3_OUTPUT_Normal                     ((uint32_t)0<<CMP_CTL3_POL_POS)
#define CMP3_OUTPUT_Opposite                   ((uint32_t)1<<CMP_CTL3_POL_POS)
#define CHECK_CMP3_OUTPUT(SEL)                 (((SEL) == CMP3_OUTPUT_Normal) \
        							           || ((SEL) == CMP3_OUTPUT_Opposite))
/**
  * CMP���ѡ��
  */
#define NONE                   		  ((uint32_t)0<<CMP_CTL4_CMPOUT0_POS)
#define CMP0_OUTPUT                   ((uint32_t)1<<CMP_CTL4_CMPOUT0_POS)
#define CMP1_OUTPUT                   ((uint32_t)2<<CMP_CTL4_CMPOUT0_POS)
#define CMP2_OUTPUT                   ((uint32_t)3<<CMP_CTL4_CMPOUT0_POS)
#define CHECK_CMP_OUTPUT(SEL)            (((SEL) == NONE) \
										|| ((SEL) == CMP0_OUTPUT) \
										|| ((SEL) == CMP1_OUTPUT) \
        							    || ((SEL) == CMP2_OUTPUT))
/**
/**
  * �Ƚ����жϱ�־λ����ģʽѡ��λ
  */
#define CMP_CMPOUT_FlipLatch_INT               ((uint32_t)0<<CMP_CTL4_CIFMS_POS)
#define CMP_CMPOUT_Change_INT                  ((uint32_t)1<<CMP_CTL4_CIFMS_POS)
#define CHECK_CMP_TriggerINT_MODE(SEL)         (((SEL) == CMP_CMPOUT_FlipLatch_INT) \
        							           || ((SEL) == CMP_CMPOUT_Change_INT))
/**
/**
  * �Ƚ������͵�ѹУ׼ѡ��
  */
#define CMP_SluggishVoltage_OFF                ((uint32_t)0<<CMP_CTL4_TRIM0_POS)
#define CMP_SluggishVoltage_5mV                ((uint32_t)1<<CMP_CTL4_TRIM0_POS)
#define CMP_SluggishVoltage_10mV               ((uint32_t)2<<CMP_CTL4_TRIM0_POS)
#define CMP_SluggishVoltage_15mV               ((uint32_t)3<<CMP_CTL4_TRIM0_POS)
#define CHECK_CMP_SluggishVoltage_SEL(SEL)     (((SEL) == CMP_SluggishVoltage_OFF) \
											   || ((SEL) == CMP_SluggishVoltage_5mV) \
											   || ((SEL) == CMP_SluggishVoltage_10mV) \
											   || ((SEL) == CMP_SluggishVoltage_15mV))

/**
/**
  * ����ģʽѡ��
  */
#define CMP_HALLMODE_SINGLE                    ((uint32_t)0<<CMP_CTL4_HALSEL_POS)
#define CMP_HALLMODE_BOTH                      ((uint32_t)1<<CMP_CTL4_HALSEL_POS)
#define CHECK_CMP_HALLMODE_SEL(SEL)   		   (((SEL) == CMP_HALLMODE_SINGLE) \
											   || ((SEL) == CMP_HALLMODE_BOTH))
/**
/**
  * �˲�������ѡ��
  */
#define CMP_FLTINSEL_CMPOUT                    ((uint32_t)0<<CMP_CTL4_FLTINSEL_POS)
#define CMP_FLTINSEL_IO                        ((uint32_t)1<<CMP_CTL4_FLTINSEL_POS)
#define CHECK_CMP_FLTINSEL(SEL)   		       (((SEL) == CMP_FLTINSEL_CMPOUT) \
										       || ((SEL) == CMP_FLTINSEL_IO))

#ifdef KF32A_Periph_cmp
/** ��ʼ�������ú��� ************************************************/
void CMP_Reset (void);
void CMP_Configuration (CMP_SFRmap * CMPx, CMP_InitTypeDef* CMPInitStruct);
void CMP_Struct_Init (CMP_InitTypeDef* CMPInitStruct);
void CMP0_Cmd (FunctionalState NewState);
void CMP1_Cmd (FunctionalState NewState);
void CMP2_Cmd (FunctionalState NewState);
void CMP3_Cmd (FunctionalState NewState);
void CMP0_POSITIVE_INPUT_SELECT (uint32_t Select);
void CMP0_NEGATIVE_INPUT_SELECT (uint32_t Select);
void CMP1_POSITIVE_INPUT_SELECT (uint32_t Select);
void CMP1_NEGATIVE_INPUT_SELECT (uint32_t Select);
void CMP2_POSITIVE_INPUT_SELECT (uint32_t Select);
void CMP2_NEGATIVE_INPUT_SELECT (uint32_t Select);
void CMP3_POSITIVE_INPUT_SELECT (uint32_t Select);
void CMP3_NEGATIVE_INPUT_SELECT (uint32_t Select);
void CMP0_OUTPUT_POL_SELECT (uint32_t Select);
void CMP1_OUTPUT_POL_SELECT (uint32_t Select);
void CMP2_OUTPUT_POL_SELECT (uint32_t Select);
void CMP3_OUTPUT_POL_SELECT (uint32_t Select);
void CMP_OUTPUT_SELECT (uint32_t Select);
FlagStatus CMP0_Read_Output_State ();
FlagStatus CMP1_Read_Output_State ();
FlagStatus CMP2_Read_Output_State ();
FlagStatus CMP3_Read_Output_State ();
FlagStatus CMP0_Get_Updata_INT_Flag ();
FlagStatus CMP1_Get_Updata_INT_Flag ();
FlagStatus CMP2_Get_Updata_INT_Flag ();
FlagStatus CMP3_Get_Updata_INT_Flag ();
void CMP_Trigger_Select_Config (uint32_t TriggerSelect);
void CMP0_Clear_Trigger_INT_Flag ();
void CMP1_Clear_Trigger_INT_Flag ();
void CMP2_Clear_Trigger_INT_Flag ();
void CMP3_Clear_Trigger_INT_Flag ();
void CMP0_INT_Enable (FunctionalState NewState);
void CMP1_INT_Enable (FunctionalState NewState);
void CMP2_INT_Enable (FunctionalState NewState);
void CMP3_INT_Enable (FunctionalState NewState);
void CMP0_INT_Enable (FunctionalState NewState);
void CMP1_INT_Enable (FunctionalState NewState);
void CMP2_INT_Enable (FunctionalState NewState);
void CMP3_INT_Enable (FunctionalState NewState);
void CMP_SluggishVoltage_Select (uint32_t Select);
void CMP_HALLMODE_Select (uint32_t Select);
void CMP_BEMF_Enable (FunctionalState NewState);
void CMP_FLTINSEL_Select (uint32_t Select);

#endif //KF32A_Periph_cmp
#endif /* _KF32A_BASIC_CMP_H */

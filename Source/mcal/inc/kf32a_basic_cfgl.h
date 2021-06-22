/**
  ******************************************************************************
  * �ļ���  kf32a_basic_cfgl.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ���߼���Ԫģ��(CFGL)��ع��ܺ�����������غ궨�塣
  *********************************************************************
  */
#ifndef _KF32A_BASIC_CFGL_H_
#define _KF32A_BASIC_CFGL_H_

#include "KF32A_BASIC.h"


/**
  * CFGL����ģ�飨CFGL��ָ�붨��
  */
#define CHECK_CFGL_PERIPH(PERIPH)     (((PERIPH) == CFGL1_SFR)\
									||((PERIPH) == CFGL2_SFR))

/**
 * CFGL ������Ϣ�ṹ��
 */
typedef struct
{
 uint32_t m_Module_EN;                 /*CFGL �߼���Ԫʹ��
                                 ȡֵΪ�ꡰCFGL �߼���Ԫʹ�ܽ�ֹѡ���е�һ��*/
 uint32_t m_Output_EN;         /*CFGL ���ʹ��ѡ��
                                 ȡֵΪ�ꡰCFGL ���ʹ�ܡ��е�һ��*/
 uint32_t m_RaiseINT_EN;            /*CFGL ���������ж�ѡ��
                                 ȡֵΪ�ꡰCFGL ���������ж�ѡ���е�һ��*/
 uint32_t m_FallINT_EN;             /*CFGL �½������ж�ѡ��
                                 ȡֵΪ�ꡰCFGL �½������ж�ѡ���е�һ��*/
 uint32_t m_Mode_Sel;         /*CFGL �������߼���Ԫ����ģʽѡ��
                                 ȡֵΪ�ꡰCFGL �������߼���Ԫ����ģʽ���е�һ��*/
 uint32_t m_Output_Pol;     /*CFGL�������ѡ��λ
                                 ȡֵΪ�ꡰCFGL������ԡ��е�һ��*/
 uint32_t m_G4Output_POL;     /*CFGL��4�������ѡ��
                                 ȡֵΪ�ꡰCFGL��4������ԡ��е�һ��*/
 uint32_t m_G3Output_POL;     /*CFGL��3�������ѡ��
                                 ȡֵΪ�ꡰCFGL��3������ԡ��е�һ��*/
 uint32_t m_G2Output_POL;     /*CFGL��2�������ѡ��
                                 ȡֵΪ�ꡰCFGL��2������ԡ��е�һ��*/
 uint32_t m_G1Output_POL;     /*CFGL��1�������ѡ��
                                 ȡֵΪ�ꡰCFGL��1������ԡ��е�һ��*/
 uint32_t m_G4Input_Sel;     /*CFGL��4��������ͨ��ѡ��
                                 ȡֵΪ�ꡰCFGL��4�������ݡ��е�һ��*/
 uint32_t m_G3Input_Sel;     /*CFGL��3��������ͨ��ѡ��
                                 ȡֵΪ�ꡰCFGL��3�������ݡ��е�һ��*/
 uint32_t m_G2Input_Sel;     /*CFGL��2��������ͨ��ѡ��
                                 ȡֵΪ�ꡰCFGL��2�������ݡ��е�һ��*/
 uint32_t m_G1Input_Sel;     /*CFGL��1��������ͨ��ѡ��λ
                                 ȡֵΪ�ꡰCFGL��1�������ݡ��е�һ��*/
 uint32_t m_CH4Data_Sel;     /*CFGL CH4ͨ����������λѡ��
                                 ȡֵΪ�ꡰCFGL CH4ͨ����������λ���е�һ��*/
 uint32_t m_CH5CH6Data_Sel;     /*CH5\CH6ͨ��ѡ��
                                 ȡֵΪ�ꡰCFGLCH5\CH6ͨ��ѡ���е�һ��*/
 FunctionalState m_G4D4_Inphase_EN;/*��4����4ͬ������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G4D4_Inverse_EN;/*��4����4��������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G4D3_Inphase_EN;/*��4����3ͬ������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G4D3_Inverse_EN;/*��4����3��������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G4D2_Inphase_EN;/*��4����2ͬ������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G4D2_Inverse_EN;/*��4����2��������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G4D1_Inphase_EN;/*��4����1ͬ������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G4D1_Inverse_EN;/*��4����1��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G3D4_Inphase_EN;/*��3����4ͬ������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G3D4_Inverse_EN;/*��3����4��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G3D3_Inphase_EN;/*��3����3ͬ������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G3D3_Inverse_EN;/*��3����3��������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G3D2_Inphase_EN;/*��3����2ͬ������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G3D2_Inverse_EN;/*��3����2��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G3D1_Inphase_EN;/*��3����1ͬ������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G3D1_Inverse_EN;/*��3����1��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G2D4_Inphase_EN;/*��2����4ͬ������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G2D4_Inverse_EN;/*��2����4��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G2D3_Inphase_EN;/*��2����3ͬ������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G2D3_Inverse_EN;/*��2����3��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G2D2_Inphase_EN;/*��2����2ͬ������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G2D2_Inverse_EN;/*��2����2��������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G2D1_Inphase_EN;/*��2����1ͬ������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G2D1_Inverse_EN;/*��2����1��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G1D4_Inphase_EN;/*��1����4ͬ������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G1D4_Inverse_EN;/*��1����4��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G1D3_Inphase_EN;/*��1����3ͬ������ͨ��ѡ��
                                 ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G1D3_Inverse_EN;/*��1����3��������ͨ��ѡ��
                                  ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G1D2_Inphase_EN;/*��1����2ͬ������ͨ��ѡ��
                                ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G1D2_Inverse_EN;/*��1����2��������ͨ��ѡ��
                                ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G1D1_Inphase_EN;/*��1����1ͬ������ͨ��ѡ��
                                ȡֵΪ�� ȡֵΪTRUE��FALSE */
 FunctionalState m_G1D1_Inverse_EN;/*��1����1��������ͨ��ѡ��
                                ȡֵΪ�� ȡֵΪTRUE��FALSE */
} CFGL_InitTypeDef;



/**
 * CFGL �߼���Ԫʹ�ܡ���ֹѡ��
 */
#define CFGL_ENABLE                 ((uint32_t) 0x1 << CFGL_CTL0_EN_POS)
#define CFGL_DISABLE                ((uint32_t) 0x0 << CFGL_CTL0_EN_POS)
#define CHECK_CFGL_EN(SEL)          (((SEL) == CFGL_ENABLE) \
                                    || ((SEL) == CFGL_DISABLE))

/**
 * CFGL ���ʹ��ѡ��
 */
#define CFGL_OUTPUT_ENABLE          ((uint32_t) 0x1 << CFGL_CTL0_OE_POS)
#define CFGL_OUTPUT_DISABLE         ((uint32_t) 0x0 << CFGL_CTL0_OE_POS)
#define CHECK_CFGL_OUTPUT(SEL)      (((SEL) == CFGL_OUTPUT_ENABLE) \
                                    || ((SEL) == CFGL_OUTPUT_DISABLE))

/**
 * CFGL ���������ж�����ѡ��
 */
#define CFGL_RISEINT_DISABLE        ((uint32_t) 0x0 << CFGL_CTL0_INTP_POS)
#define CFGL_RISEINT_ENABLE         ((uint32_t) 0x1 << CFGL_CTL0_INTP_POS)
#define CHECK_CFGL_RISEINT_EN(SEL)  (((SEL) == CFGL_RISEINT_DISABLE) \
                                    || ((SEL) == CFGL_RISEINT_ENABLE))

/**
 * CFGL �½������ж�����ѡ��
 */
#define CFGL_FALLINT_ENABLE         ((uint32_t) 0x1 << CFGL_CTL0_INTN_POS)
#define CFGL_FALLINT_DISABLE        ((uint32_t) 0x0 << CFGL_CTL0_INTN_POS)
#define CHECK_CFGL_FALLINT_EN(SEL)  (((SEL) == CFGL_FALLINT_ENABLE) \
                                    || ((SEL) == CFGL_FALLINT_DISABLE) )

#define CHECK_CFGL_INT_EN(SEL)      (((SEL) == CFGL_RISEINT_DISABLE) \
                                    || ((SEL) == CFGL_RISEINT_ENABLE)\
                                    || ((SEL) == CFGL_FALLINT_ENABLE) \
                                    || ((SEL) == CFGL_FALLINT_DISABLE))
/**
 * CFGL �����߼���Ԫ����ģʽ
 */
#define CFGL_FLIP_LATCH_ONE_INPUT    ((uint32_t) 0x7 << CFGL_CTL0_MD0_POS)
#define CFGL_JK_TRIGGER       		 ((uint32_t) 0x6 << CFGL_CTL0_MD0_POS)
#define CFGL_D_TRIGGER_TWO_INPUT     ((uint32_t) 0x5 << CFGL_CTL0_MD0_POS)
#define CFGL_D_TRIGGER_ONE_INPUT     ((uint32_t) 0x4 << CFGL_CTL0_MD0_POS)
#define CFGL_SR_LATCH        		 ((uint32_t) 0x3 << CFGL_CTL0_MD0_POS)
#define CFGL_AND_FOUR_INPUT       	 ((uint32_t) 0x2 << CFGL_CTL0_MD0_POS)
#define CFGL_OR_XOR         		 ((uint32_t) 0x1 << CFGL_CTL0_MD0_POS)
#define CFGL_AND_OR       	 		 ((uint32_t) 0x0 << CFGL_CTL0_MD0_POS)
#define CHECK_CFGL_MODE(SEL)         (((SEL) == CFGL_FLIP_LATCH_ONE_INPUT) \
								  || ((SEL) == CFGL_JK_TRIGGER) \
								  || ((SEL) == CFGL_D_TRIGGER_TWO_INPUT) \
								  || ((SEL) == CFGL_D_TRIGGER_ONE_INPUT) \
								  || ((SEL) == CFGL_SR_LATCH) \
								  || ((SEL) == CFGL_AND_FOUR_INPUT) \
								  || ((SEL) == CFGL_OR_XOR) \
                                  || ((SEL) == CFGL_AND_OR) )
/**
 * CFGL �������ѡ��
 */
#define CFGL_OUT_INPHASE             ((uint32_t) 0x0 << CFGL_CTL0_POL_POS)
#define CFGL_OUT_INVERSE             ((uint32_t) 0x1 << CFGL_CTL0_POL_POS)
#define CHECK_CFGL_OUT_POL(SEL)      (((SEL) == CFGL_OUT_INPHASE) \
                                     || ((SEL) == CFGL_OUT_INVERSE) )
/**
 * CFGL ��4 �������ѡ��
 */
#define CFGL_G4POL_INPHASE           ((uint32_t) 0x0 << CFGL_CTL0_G4POL_POS)
#define CFGL_G4POL_INVERSE           ((uint32_t) 0x1 << CFGL_CTL0_G4POL_POS)
#define CHECK_G4POL_SEL(SEL)     	 (((SEL) == CFGL_G4POL_INPHASE) \
                                     || ((SEL) == CFGL_G4POL_INVERSE) )
/**
 * CFGL ��3 �������ѡ��
 */
#define CFGL_G3POL_INPHASE           ((uint32_t) 0x0 << CFGL_CTL0_G3POL_POS)
#define CFGL_G3POL_INVERSE           ((uint32_t) 0x1 << CFGL_CTL0_G3POL_POS)
#define CHECK_G3POL_SEL(SEL)     	 (((SEL) == CFGL_G3POL_INPHASE) \
                                     || ((SEL) == CFGL_G3POL_INVERSE) )
/**
 * CFGL ��2 �������ѡ��
 */
#define CFGL_G2POL_INPHASE           ((uint32_t) 0x0 << CFGL_CTL0_G2POL_POS)
#define CFGL_G2POL_INVERSE           ((uint32_t) 0x1 << CFGL_CTL0_G2POL_POS)
#define CHECK_G2POL_SEL(SEL)     	 (((SEL) == CFGL_G2POL_INPHASE) \
                                     || ((SEL) == CFGL_G2POL_INVERSE) )
/**
 * CFGL ��1 �������ѡ��
 */
#define CFGL_G1POL_INPHASE           ((uint32_t) 0x0 << CFGL_CTL0_G1POL_POS)
#define CFGL_G1POL_INVERSE           ((uint32_t) 0x1 << CFGL_CTL0_G1POL_POS)
#define CHECK_G1POL_SEL(SEL)     	 (((SEL) == CFGL_G1POL_INPHASE) \
                                     || ((SEL) == CFGL_G1POL_INVERSE) )

/**
 * CFGL ��4 ��������ͨ��ѡ��
 */
#define CFGL_G4INPUT_CFGL1CH3        ((uint32_t) 0x7 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH2        ((uint32_t) 0x6 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH1        ((uint32_t) 0x5 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH0        ((uint32_t) 0x4 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH15       ((uint32_t) 0x3 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH14       ((uint32_t) 0x2 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH13       ((uint32_t) 0x1 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH12       ((uint32_t) 0x0 << CFGL_CTL0_D4S0_POS)
#define CHECK_G4INPUT_SEL(SEL)       (((SEL) == CFGL_G4INPUT_CFGL1CH3) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH2) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH1) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH0) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH15) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH14) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH13) \
                                     || ((SEL) == CFGL_G4INPUT_CFGL1CH12) )
/**
 * CFGL ��3 ��������ͨ��ѡ��
 */
#define CFGL_G3INPUT_CFGL1CH15        ((uint32_t) 0x7 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH14        ((uint32_t) 0x6 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH13        ((uint32_t) 0x5 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH12        ((uint32_t) 0x4 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH11        ((uint32_t) 0x3 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH10        ((uint32_t) 0x2 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH9         ((uint32_t) 0x1 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH8         ((uint32_t) 0x0 << CFGL_CTL0_D3S0_POS)
#define CHECK_G3INPUT_SEL(SEL)        (((SEL) == CFGL_G3INPUT_CFGL1CH15) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH14) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH13) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH12) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH11) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH10) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH9) \
                                      || ((SEL) == CFGL_G3INPUT_CFGL1CH8) )
/**
 * CFGL ��2 ��������ͨ��ѡ��
 */
#define CFGL_G2INPUT_CFGL1CH11        ((uint32_t) 0x7 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH10        ((uint32_t) 0x6 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH9         ((uint32_t) 0x5 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH8         ((uint32_t) 0x4 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH7         ((uint32_t) 0x3 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH6         ((uint32_t) 0x2 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH5         ((uint32_t) 0x1 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH4         ((uint32_t) 0x0 << CFGL_CTL0_D2S0_POS)
#define CHECK_G2INPUT_SEL(SEL)        (((SEL) == CFGL_G2INPUT_CFGL1CH11) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH10) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH9) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH8) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH7) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH6) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH5) \
                                      || ((SEL) == CFGL_G2INPUT_CFGL1CH4) )
/**
 * CFGL ��1 ��������ͨ��ѡ��
 */
#define CFGL_G1INPUT_CFGL1CH7         ((uint32_t) 0x7 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH6         ((uint32_t) 0x6 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH5         ((uint32_t) 0x5 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH4         ((uint32_t) 0x4 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH3         ((uint32_t) 0x3 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH2         ((uint32_t) 0x2 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH1         ((uint32_t) 0x1 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH0         ((uint32_t) 0x0 << CFGL_CTL0_D1S0_POS)
#define CHECK_G1INPUT_SEL(SEL)        (((SEL) == CFGL_G1INPUT_CFGL1CH7) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH6) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH5) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH4) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH3) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH2) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH1) \
                                      || ((SEL) == CFGL_G1INPUT_CFGL1CH0) )
/**
 * CFGL CH4ͨ����������ѡ��
 */
#define CFGL_CH4_INPUT_0              ((uint32_t) 0x0 << CFGL_CTL0_REGIN_POS)
#define CFGL_CH4_INPUT_1              ((uint32_t) 0x1 << CFGL_CTL0_REGIN_POS)
#define CHECK_CH4_INPUT_SEL(SEL)      (((SEL) == CFGL_CH4_INPUT_0) \
                                      || ((SEL) == CFGL_CH4_INPUT_1) )

/**
 * CFGL CH5��CFGL1CH6ͨ��ѡ��
 */
#define CFGL_CH5_T21TRGO_CH6_CCP21CH1OUT    ((uint32_t) 0x7 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T20TRGO_CH6_CCP20CH1OUT    ((uint32_t) 0x6 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T5TRGO_CH6_CCP5CH1OUT      ((uint32_t) 0x5 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T4TRGO_CH6_CCP4CH1OUT      ((uint32_t) 0x4 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T3TRGO_CH6_CCP3CH1OUT      ((uint32_t) 0x3 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T2TRGO_CH6_CCP2CH1OUT      ((uint32_t) 0x2 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T1TRGO_CH6_CCP1CH1OUT      ((uint32_t) 0x1 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T0TRGO_CH6_CCP0CH1OUT      ((uint32_t) 0x0 << CFGL_CTL0_TPSEL0_POS)
#define CHECK_CH5CH6_INPUT_SEL(SEL)         (((SEL) == CFGL_CH5_T21TRGO_CH6_CCP21CH1OUT) \
										    || ((SEL) == CFGL_CH5_T20TRGO_CH6_CCP20CH1OUT) \
										    || ((SEL) == CFGL_CH5_T5TRGO_CH6_CCP5CH1OUT) \
										    || ((SEL) == CFGL_CH5_T4TRGO_CH6_CCP4CH1OUT) \
										    || ((SEL) == CFGL_CH5_T3TRGO_CH6_CCP3CH1OUT) \
										    || ((SEL) == CFGL_CH5_T2TRGO_CH6_CCP2CH1OUT) \
										    || ((SEL) == CFGL_CH5_T1TRGO_CH6_CCP1CH1OUT) \
                                    	    || ((SEL) == CFGL_CH5_T0TRGO_CH6_CCP0CH1OUT) )

/**
 * CFGL CFGL2���ͬ��ʹ��ѡ��
 */
#define CFGL2_OUT_SYNCHRO_ENABLE            ((uint32_t) 0x1 << CFGL_IFR_CLC2SYNCEN_POS)
#define CFGL2_OUT_SYNCHRO_DISABLE           ((uint32_t) 0x0 << CFGL_IFR_CLC2SYNCEN_POS)
#define CHECK_CFGL2_OUT_SYNCHRO(SEL)     	(((SEL) == CFGL2_OUT_SYNCHRO_ENABLE) \
                                    		|| ((SEL) == CFGL2_OUT_SYNCHRO_DISABLE) )
/**
 * CFGL CFGL1���ͬ��ʹ��ѡ��
 */
#define CFGL1_OUT_SYNCHRO_ENABLE            ((uint32_t) 0x1 << CFGL_IFR_CLC1SYNCEN_POS)
#define CFGL1_OUT_SYNCHRO_DISABLE           ((uint32_t) 0x0 << CFGL_IFR_CLC1SYNCEN_POS)
#define CHECK_CFGL1_OUT_SYNCHRO(SEL)     	(((SEL) == CFGL1_OUT_SYNCHRO_ENABLE) \
                                    		|| ((SEL) == CFGL1_OUT_SYNCHRO_DISABLE) )

#ifdef KF32A_Periph_cfgl
FlagStatus CFGL1_OUT_STATE ();
FlagStatus CFGL2_OUT_STATE ();
FlagStatus CFGL2_Get_INT_Flag ();
FlagStatus CFGL1_Get_INT_Flag ();
void CFGL1_FALLINT_Enable (FunctionalState NewState);
void CFGL1_RISEINT_Enable (FunctionalState NewState);
void CFGL2_FALLINT_Enable (FunctionalState NewState);
void CFGL2_RISEINT_Enable (FunctionalState NewState);
void CFGL1_OUT_SYNCHRO_Enable (FunctionalState NewState);
void CFGL2_OUT_SYNCHRO_Enable (FunctionalState NewState);
void CFGL_Configuration (CFGL_SFRmap* CFGLx, CFGL_InitTypeDef* CFGLInitStruct);
void CFGL_Reset ();
void CFGL1_Clear_RISEINT_Flag (void);
void CFGL1_Clear_FALLINT_Flag (void);
void CFGL1_Clear_RISEFALLINT_Flag (void);
void CFGL2_Clear_RISEINT_Flag (void);
void CFGL2_Clear_FALLINT_Flag (void);
void CFGL2_Clear_RISEFALLINT_Flag (void);
void CFGL_Struct_Init (CFGL_InitTypeDef* CFGLInitStruct);
#endif //KF32A_Periph_cfgl
#endif /* _KF32A_BASIC_CFGL_H_ */

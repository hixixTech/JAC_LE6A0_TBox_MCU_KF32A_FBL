/**
  ******************************************************************************
  * �ļ���  kf32a_basic_cfgl.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˿������߼���(CFLG)��صĹ��ܺ�����������
  *            + �������߼���(CFLG)��ʼ������
  *            + �������߼���(CFLG)�жϹ�����
  *
  *********************************************************************
  */

#include "kf32a_basic_cfgl.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_cfgl
/* (CFGL)ģ��˽�ж��� -------------------------------------------------*/

/* CFGLx_CTL0�Ĵ�����ʼ���������� */
#define CFGL_CTL0_INIT_MASK		 		(CFGL_CTL0_EN \
                                       | CFGL_CTL0_OE \
                                       | CFGL_CTL0_INTP \
                                       | CFGL_CTL0_INTN \
                                       | CFGL_CTL0_MD \
                                       | CFGL_CTL0_POL \
                                       | CFGL_CTL0_G4POL \
                                       | CFGL_CTL0_G3POL \
                                       | CFGL_CTL0_G2POL \
                                       | CFGL_CTL0_G1POL \
                                       | CFGL_CTL0_D4S \
                                       | CFGL_CTL0_D3S \
                                       | CFGL_CTL0_D2S \
                                       | CFGL_CTL0_D1S \
                                       | CFGL_CTL0_REGIN \
                                       | CFGL_CTL0_TPSEL)

#define CFGL_CTL1_INIT_MASK	0XFFFFFFFF
/**
  *   ##### CFGLģ���ʼ������ #####
  */
/**
  * ����  ��λCFGL���裬ʹ������ʱ�ӡ�
  * ����  CFGLx: ָ��CFGL�ڴ�ṹ��ָ�룬ȡֵΪCFGL0_SFR~CFGL2_SFR��
  * ����  �ޡ�
  */
void
CFGL_Reset ()
{

	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_CFGLRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_CFGLRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_CFGLCLKEN, TRUE);

}
/**
  * ����  CFGLģ���ʼ�����á�
  * ����  CFGLx: ָ��CFGL�ڴ�ṹ��ָ�룬ȡֵΪCFGL0_SFR~CFGL2_SFR��
  *       CFGLInitStruct: CFGLģ��������Ϣ�ṹ��ָ�롣
  * ����  �ޡ�
  */
void
CFGL_Configuration (CFGL_SFRmap* CFGLx, CFGL_InitTypeDef* CFGLInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_CFGL_PERIPH(CFGLx));
    CHECK_RESTRICTION(CHECK_CFGL_EN(CFGLInitStruct->m_Module_EN));
    CHECK_RESTRICTION(CHECK_CFGL_OUTPUT(CFGLInitStruct->m_Output_EN));
    CHECK_RESTRICTION(CHECK_CFGL_RISEINT_EN(CFGLInitStruct->m_RaiseINT_EN));
    CHECK_RESTRICTION(CHECK_CFGL_FALLINT_EN(CFGLInitStruct->m_FallINT_EN));
    CHECK_RESTRICTION(CHECK_CFGL_MODE(CFGLInitStruct->m_Mode_Sel));
    CHECK_RESTRICTION(CHECK_CFGL_OUT_POL(CFGLInitStruct->m_Output_Pol));
    CHECK_RESTRICTION(CHECK_G4POL_SEL(CFGLInitStruct->m_G4Output_POL));
    CHECK_RESTRICTION(CHECK_G3POL_SEL(CFGLInitStruct->m_G3Output_POL));
    CHECK_RESTRICTION(CHECK_G2POL_SEL(CFGLInitStruct->m_G2Output_POL));
    CHECK_RESTRICTION(CHECK_G1POL_SEL(CFGLInitStruct->m_G1Output_POL));
    CHECK_RESTRICTION(CHECK_G4INPUT_SEL(CFGLInitStruct->m_G4Input_Sel));
    CHECK_RESTRICTION(CHECK_G3INPUT_SEL(CFGLInitStruct->m_G3Input_Sel));
    CHECK_RESTRICTION(CHECK_G2INPUT_SEL(CFGLInitStruct->m_G2Input_Sel));
    CHECK_RESTRICTION(CHECK_G1INPUT_SEL(CFGLInitStruct->m_G1Input_Sel));
    CHECK_RESTRICTION(CHECK_CH4_INPUT_SEL(CFGLInitStruct->m_CH4Data_Sel));
    CHECK_RESTRICTION(CHECK_CH5CH6_INPUT_SEL(CFGLInitStruct->m_CH5CH6Data_Sel));

    /*---------------- ����CFGL_CTL0�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_Module_EN������ENλ�� */
    /* ���ݽṹ���Աm_Output_EN������OEλ�� */
    /* ���ݽṹ���Աm_RaiseINT_EN������INTPλ�� */
    /* ���ݽṹ���Աm_FallINT_EN������INTNλ�� */
    /* ���ݽṹ���Աm_Mode_Sel������MDλ�� */
    /* ���ݽṹ���Աm_Output_Pol������POLλ�� */
    /* ���ݽṹ���Աm_G4Output_POL������G4POLλ�� */
    /* ���ݽṹ���Աm_G3Output_POL������G3POLλ�� */
    /* ���ݽṹ���Աm_G2Output_POL������G2POLλ�� */
    /* ���ݽṹ���Աm_G1Output_POL������G1POLλ�� */
    /* ���ݽṹ���Աm_G4Input_Sel������D4Sλ�� */
    /* ���ݽṹ���Աm_G3Input_Sel������D3Sλ�� */
    /* ���ݽṹ���Աm_G2Input_Sel������D2Sλ�� */
    /* ���ݽṹ���Աm_G1Input_Sel������D1Sλ�� */
    /* ���ݽṹ���Աm_CH4Data_Sel������REGINλ�� */
    /* ���ݽṹ���Աm_CH5CH6Data_Sel������TPSELλ�� */
    tmpreg = (uint32_t)(CFGLInitStruct->m_Module_EN | \
                        CFGLInitStruct->m_Output_EN | \
                        CFGLInitStruct->m_RaiseINT_EN | \
                        CFGLInitStruct->m_FallINT_EN | \
                        CFGLInitStruct->m_Mode_Sel | \
                        CFGLInitStruct->m_Output_Pol | \
                        CFGLInitStruct->m_G4Output_POL | \
                        CFGLInitStruct->m_G3Output_POL | \
                        CFGLInitStruct->m_G2Output_POL | \
                        CFGLInitStruct->m_G1Output_POL | \
                        CFGLInitStruct->m_G4Input_Sel | \
                        CFGLInitStruct->m_G3Input_Sel | \
                        CFGLInitStruct->m_G2Input_Sel | \
                        CFGLInitStruct->m_G1Input_Sel | \
                        CFGLInitStruct->m_CH4Data_Sel | \
                        CFGLInitStruct->m_CH5CH6Data_Sel);
    CFGLx->CTL0 = SFR_Config (CFGLx->CTL0, ~CFGL_CTL0_INIT_MASK, tmpreg);

    /*---------------- ����CFGL_CTL1�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_G4D4_Inphase_EN������G4D4Tλ�� */
    /* ���ݽṹ���Աm_G4D4_Inverse_EN������G4D4Nλ�� */
    /* ���ݽṹ���Աm_G4D3_Inphase_EN������G4D3Tλ�� */
    /* ���ݽṹ���Աm_G4D3_Inverse_EN������G4D3Nλ�� */
    /* ���ݽṹ���Աm_G4D2_Inphase_EN������G4D2Tλ�� */
    /* ���ݽṹ���Աm_G4D2_Inverse_EN������G4D2Nλ�� */
    /* ���ݽṹ���Աm_G4D1_Inphase_EN������G4D1Tλ�� */
    /* ���ݽṹ���Աm_G4D1_Inverse_EN������G4D1Nλ�� */
    /* ���ݽṹ���Աm_G3D4_Inphase_EN������G3D4Tλ�� */
    /* ���ݽṹ���Աm_G3D4_Inverse_EN������G3D4Nλ�� */
    /* ���ݽṹ���Աm_G3D3_Inphase_EN������G3D3Tλ�� */
    /* ���ݽṹ���Աm_G3D3_Inverse_EN������G3D3Nλ�� */
    /* ���ݽṹ���Աm_G3D2_Inphase_EN������G3D2Tλ�� */
    /* ���ݽṹ���Աm_G3D2_Inverse_EN������G3D2Nλ�� */
    /* ���ݽṹ���Աm_G3D1_Inphase_EN������G3D1Tλ�� */
    /* ���ݽṹ���Աm_G3D1_Inverse_EN������G3D1Nλ�� */
    /* ���ݽṹ���Աm_G2D4_Inphase_EN������G2D4Tλ�� */
    /* ���ݽṹ���Աm_G2D4_Inverse_EN������G2D4Nλ�� */
    /* ���ݽṹ���Աm_G2D3_Inphase_EN������G2D3Tλ�� */
    /* ���ݽṹ���Աm_G2D3_Inverse_EN������G2D3Nλ�� */
    /* ���ݽṹ���Աm_G2D2_Inphase_EN������G2D2Tλ�� */
    /* ���ݽṹ���Աm_G2D2_Inverse_EN������G2D2Nλ�� */
    /* ���ݽṹ���Աm_G2D1_Inphase_EN������G2D1Tλ�� */
    /* ���ݽṹ���Աm_G2D1_Inverse_EN������G2D1Nλ�� */
    /* ���ݽṹ���Աm_G1D4_Inphase_EN������G1D4Tλ�� */
    /* ���ݽṹ���Աm_G1D4_Inverse_EN������G1D4Nλ�� */
    /* ���ݽṹ���Աm_G1D3_Inphase_EN������G1D3Tλ�� */
    /* ���ݽṹ���Աm_G1D3_Inverse_EN������G1D3Nλ�� */
    /* ���ݽṹ���Աm_G1D2_Inphase_EN������G1D2Tλ�� */
    /* ���ݽṹ���Աm_G1D2_Inverse_EN������G1D2Nλ�� */
    /* ���ݽṹ���Աm_G1D1_Inphase_EN������G1D1Tλ�� */
    /* ���ݽṹ���Աm_G1D1_Inverse_EN������G1D1Nλ�� */
    tmpreg = (uint32_t)((CFGLInitStruct->m_G4D4_Inphase_EN << CFGL_CTL1_G4D4T_POS) | \
    					(CFGLInitStruct->m_G4D4_Inverse_EN << CFGL_CTL1_G4D4N_POS) | \
    					(CFGLInitStruct->m_G4D3_Inphase_EN << CFGL_CTL1_G4D3T_POS) | \
    					(CFGLInitStruct->m_G4D3_Inverse_EN << CFGL_CTL1_G4D3N_POS) | \
    					(CFGLInitStruct->m_G4D2_Inphase_EN << CFGL_CTL1_G4D2T_POS) | \
    					(CFGLInitStruct->m_G4D2_Inverse_EN << CFGL_CTL1_G4D2N_POS) | \
    					(CFGLInitStruct->m_G4D1_Inphase_EN << CFGL_CTL1_G4D1T_POS) | \
    					(CFGLInitStruct->m_G4D1_Inverse_EN << CFGL_CTL1_G4D1N_POS) | \
    					(CFGLInitStruct->m_G3D4_Inphase_EN << CFGL_CTL1_G3D4T_POS) | \
						(CFGLInitStruct->m_G3D4_Inverse_EN << CFGL_CTL1_G3D4N_POS) | \
						(CFGLInitStruct->m_G3D3_Inphase_EN << CFGL_CTL1_G3D3T_POS) | \
						(CFGLInitStruct->m_G3D3_Inverse_EN << CFGL_CTL1_G3D3N_POS) | \
						(CFGLInitStruct->m_G3D2_Inphase_EN << CFGL_CTL1_G3D2T_POS) | \
						(CFGLInitStruct->m_G3D2_Inverse_EN << CFGL_CTL1_G3D2N_POS) | \
						(CFGLInitStruct->m_G3D1_Inphase_EN << CFGL_CTL1_G3D1T_POS) | \
						(CFGLInitStruct->m_G3D1_Inverse_EN << CFGL_CTL1_G3D1N_POS) | \
    					(CFGLInitStruct->m_G2D4_Inphase_EN << CFGL_CTL1_G2D4T_POS) | \
						(CFGLInitStruct->m_G2D4_Inverse_EN << CFGL_CTL1_G2D4N_POS) | \
						(CFGLInitStruct->m_G2D3_Inphase_EN << CFGL_CTL1_G2D3T_POS) | \
						(CFGLInitStruct->m_G2D3_Inverse_EN << CFGL_CTL1_G2D3N_POS) | \
						(CFGLInitStruct->m_G2D2_Inphase_EN << CFGL_CTL1_G2D2T_POS) | \
						(CFGLInitStruct->m_G2D2_Inverse_EN << CFGL_CTL1_G2D2N_POS) | \
						(CFGLInitStruct->m_G2D1_Inphase_EN << CFGL_CTL1_G2D1T_POS) | \
						(CFGLInitStruct->m_G2D1_Inverse_EN << CFGL_CTL1_G2D1N_POS) | \
    					(CFGLInitStruct->m_G1D4_Inphase_EN << CFGL_CTL1_G1D4T_POS) | \
						(CFGLInitStruct->m_G1D4_Inverse_EN << CFGL_CTL1_G1D4N_POS) | \
						(CFGLInitStruct->m_G1D3_Inphase_EN << CFGL_CTL1_G1D3T_POS) | \
						(CFGLInitStruct->m_G1D3_Inverse_EN << CFGL_CTL1_G1D3N_POS) | \
						(CFGLInitStruct->m_G1D2_Inphase_EN << CFGL_CTL1_G1D2T_POS) | \
						(CFGLInitStruct->m_G1D2_Inverse_EN << CFGL_CTL1_G1D2N_POS) | \
						(CFGLInitStruct->m_G1D1_Inphase_EN << CFGL_CTL1_G1D1T_POS) | \
						(CFGLInitStruct->m_G1D1_Inverse_EN << CFGL_CTL1_G1D1N_POS));
    CFGLx->CTL1 = SFR_Config (CFGLx->CTL1, ~CFGL_CTL1_INIT_MASK, tmpreg);

}

/**
  * ����  ��ʼ��CFGL������Ϣ�ṹ�塣
  * ����  CFGLInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
CFGL_Struct_Init (CFGL_InitTypeDef* CFGLInitStruct)
{
    /* ��ʼ��CFGL �߼���Ԫʹ�ܿ���λ */
	CFGLInitStruct->m_Module_EN = CFGL_DISABLE;
    /* ��ʼ�� CFGL ���ʹ��ѡ��λ */
    CFGLInitStruct->m_Output_EN = CFGL_OUTPUT_DISABLE;
    /* ��ʼ�� CFGL ���������ж�ѡ�� */
    CFGLInitStruct->m_RaiseINT_EN = CFGL_RISEINT_DISABLE;
    /* ��ʼ�� CFGL �½������ж�ѡ�� */
    CFGLInitStruct->m_FallINT_EN = CFGL_FALLINT_DISABLE;
    /* ��ʼ��CFGL �������߼���Ԫ����ģʽѡ�� */
    CFGLInitStruct->m_Mode_Sel = CFGL_AND_OR;
    /* ��ʼ�� CFGL �������ѡ��λ */
    CFGLInitStruct->m_Output_Pol = CFGL_OUT_INPHASE;
    /* ��ʼ��CFGL��4�������ѡ�� */
	CFGLInitStruct->m_G4Output_POL = CFGL_G4POL_INPHASE;
    /* ��ʼ�� CFGL��3�������ѡ�� */
    CFGLInitStruct->m_G3Output_POL = CFGL_G3POL_INPHASE;
    /* ��ʼ�� CFGL��2�������ѡ�� */
    CFGLInitStruct->m_G2Output_POL = CFGL_G2POL_INPHASE;
    /* ��ʼ�� CFGL��1�������ѡ�� */
    CFGLInitStruct->m_G1Output_POL = CFGL_G1POL_INPHASE;
    /* ��ʼ��CFGL��4��������ͨ��ѡ�� */
    CFGLInitStruct->m_G4Input_Sel = CFGL_G4INPUT_CFGL1CH3;
    /* ��ʼ��CFGL��3��������ͨ��ѡ�� */
    CFGLInitStruct->m_G3Input_Sel = CFGL_G3INPUT_CFGL1CH15;
    /* ��ʼ��CFGL��2��������ͨ��ѡ�� */
    CFGLInitStruct->m_G2Input_Sel = CFGL_G2INPUT_CFGL1CH11;
    /* ��ʼ��CFGL��1��������ͨ��ѡ�� */
    CFGLInitStruct->m_G1Input_Sel = CFGL_G1INPUT_CFGL1CH7;
    /* ��ʼ��CFGL CH4ͨ����������λѡ�� */
    CFGLInitStruct->m_CH4Data_Sel = CFGL_CH4_INPUT_0;
    /* ��ʼ��CH5\CH6ͨ��ѡ�� */
    CFGLInitStruct->m_CH5CH6Data_Sel = CFGL_CH5_T21TRGO_CH6_CCP21CH1OUT;

    /* ��4����4ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G4D4_Inphase_EN = FALSE;
    /* ��4����4��������ͨ��ѡ�� */
    CFGLInitStruct->m_G4D4_Inverse_EN = FALSE;
    /* ��4����3ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G4D3_Inphase_EN = FALSE;
    /* ��4����3��������ͨ��ѡ�� */
    CFGLInitStruct->m_G4D3_Inverse_EN = FALSE;
    /* ��4����2ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G4D2_Inphase_EN = FALSE;
    /* ��4����2��������ͨ��ѡ�� */
    CFGLInitStruct->m_G4D2_Inverse_EN = FALSE;
    /* ��4����1ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G4D1_Inphase_EN = FALSE;
    /* ��4����1��������ͨ��ѡ�� */
    CFGLInitStruct->m_G4D1_Inverse_EN = FALSE;

    /* ��3����4ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G3D4_Inphase_EN = FALSE;
    /* ��3����4��������ͨ��ѡ�� */
    CFGLInitStruct->m_G3D4_Inverse_EN = FALSE;
    /* ��3����3ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G3D3_Inphase_EN = FALSE;
    /* ��3����3��������ͨ��ѡ�� */
    CFGLInitStruct->m_G3D3_Inverse_EN = FALSE;
    /* ��3����2ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G3D2_Inphase_EN = FALSE;
    /* ��3����2��������ͨ��ѡ�� */
    CFGLInitStruct->m_G3D2_Inverse_EN = FALSE;
    /* ��3����1ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G3D1_Inphase_EN = FALSE;
    /* ��3����1��������ͨ��ѡ�� */
    CFGLInitStruct->m_G3D1_Inverse_EN = FALSE;

    /* ��2����4ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G2D4_Inphase_EN = FALSE;
    /* ��2����4��������ͨ��ѡ�� */
    CFGLInitStruct->m_G2D4_Inverse_EN = FALSE;
    /* ��2����3ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G2D3_Inphase_EN = FALSE;
    /* ��2����3��������ͨ��ѡ�� */
    CFGLInitStruct->m_G2D3_Inverse_EN = FALSE;
    /* ��2����2ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G2D2_Inphase_EN = FALSE;
    /* ��2����2��������ͨ��ѡ�� */
    CFGLInitStruct->m_G2D2_Inverse_EN = FALSE;
    /* ��2����1ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G2D1_Inphase_EN = FALSE;
    /* ��2����1��������ͨ��ѡ�� */
    CFGLInitStruct->m_G2D1_Inverse_EN = FALSE;

    /* ��1����4ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G1D4_Inphase_EN = FALSE;
    /* ��1����4��������ͨ��ѡ�� */
    CFGLInitStruct->m_G1D4_Inverse_EN = FALSE;
    /* ��1����3ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G1D3_Inphase_EN = FALSE;
    /* ��1����3��������ͨ��ѡ�� */
    CFGLInitStruct->m_G1D3_Inverse_EN = FALSE;
    /* ��1����2ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G1D2_Inphase_EN = FALSE;
    /* ��1����2��������ͨ��ѡ�� */
    CFGLInitStruct->m_G1D2_Inverse_EN = FALSE;
    /* ��1����1ͬ������ͨ��ѡ�� */
    CFGLInitStruct->m_G1D1_Inphase_EN = FALSE;
    /* ��1����1��������ͨ��ѡ�� */
    CFGLInitStruct->m_G1D1_Inverse_EN = FALSE;
}

/**
  * ����  ����CFGL2 ���ͬ��ʹ�ܡ�
  * ����NewState: ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
CFGL2_OUT_SYNCHRO_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����CFGLIFR�Ĵ���CFGL2SYNCENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��CFGL2 ���ͬ��λ */
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2SYNCEN_POS);

    }
    else
    {
        /* ��ֹCFGL2 ���ͬ��*/
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2SYNCEN_POS);
    }
}
/**
  * ����  ����CFGL1 ���ͬ��ʹ�ܡ�
  * ����NewState: ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
CFGL1_OUT_SYNCHRO_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����CFGLIFR�Ĵ���CFGL1SYNCENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��CFGL1 ���ͬ��λ */
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1SYNCEN_POS);

    }
    else
    {
        /* ��ֹCFGL1 ���ͬ��*/
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1SYNCEN_POS);
    }
}
/**
  * ����  ����CFGL2 �� CFGL2_OUT�ж��½�������ѡ��
  * ����NewState: ʹ��״̬��ȡֵΪTRUE �� FALSE��
  *
  * ����  �ޡ�
  */
void
CFGL2_FALLINT_Enable (FunctionalState NewState)
{
    /* ����У�� */
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����CFGLIFR�Ĵ���CFGL2IEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��CFGL2 �½����ж�λ */
    	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2IE_POS);

    }
    else
    {
        /* ��ֹCFGL2 �½����ж�*/
    	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2IE_POS);
    }
}
/**
  * ����  ����CFGL2 �� CFGL2_OUT�������ж�����ѡ��
  * ����NewState: ʹ��״̬��ȡֵΪTRUE �� FALSE��
  *
  * ����  �ޡ�
  */
void
CFGL2_RISEINT_Enable (FunctionalState NewState)
{
    /* ����У�� */
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����CFGLIFR�Ĵ���CFGL2IEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��CFGL2 �������ж�λ */
    	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2IE_POS);

    }
    else
    {
        /* ��ֹCFGL2 �������ж�*/
    	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2IE_POS);
    }
}
/**
  * ����  ����CFGL1 �� CFGL1_OUT�ж��½�������ѡ��
  * ����NewState: ʹ��״̬��ȡֵΪTRUE �� FALSE��
  *
  * ����  �ޡ�
  */
void
CFGL1_FALLINT_Enable (FunctionalState NewState)
{
    /* ����У�� */
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����CFGLIFR�Ĵ���CFGL2IEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��CFGL1 �½����ж�λ */
    	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1IE_POS);

    }
    else
    {
        /* ��ֹCFGL1 �½����ж�*/
    	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1IE_POS);
    }
}
/**
  * ����  ����CFGL1 �� CFGL1_OUT�������ж�����ѡ��
  * ����NewState: ʹ��״̬��ȡֵΪTRUE �� FALSE��
  *
  * ����  �ޡ�
  */
void
CFGL1_RISEINT_Enable (FunctionalState NewState)
{
    /* ����У�� */
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����CFGLIFR�Ĵ���CFGL1IEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��CFGL1 �������ж�λ */
    	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1IE_POS);

    }
    else
    {
        /* ��ֹCFGL1 �������ж�*/
    	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1IE_POS);
    }
}
/**
  * ����  ��ȡCFGL1 �жϱ�־��
  * ���룺��
  * ���� FlagStatus
  */
FlagStatus
CFGL1_Get_INT_Flag ()
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL1IFλ ----------------*/
    if (CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1IF)
    {
        /* ��⵽CFGL1�ж� */
        return SET;
    }
    else
    {
        /* û�м�⵽CFGL1�ж� */
        return RESET;
    }
}
/**
  * ����  ��ȡCFGL2 �жϱ�־��
  * ���룺��
  * ���� FlagStatus
  */
FlagStatus
CFGL2_Get_INT_Flag ()
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL2IFλ ----------------*/
    if (CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2IF)
    {
        /* ��⵽CFGL2�жϱ�־ */
        return SET;
    }
    else
    {
        /* û�м�⵽CFGL2�ж� */
        return RESET;
    }
}
/**
  * ����  ��CFGL1 �������жϱ�־��
  * ���룺��
  * ����   ��
  */
void
CFGL1_Clear_RISEINT_Flag (void)
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL1IFλ ----------------*/
	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1IF)>>CFGL_IFR_CFGL1IF_POS);
	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
}
/**
  * ����  ��CFGL1 �½����жϱ�־��
  * ���룺��
  * ����   ��
  */
void
CFGL1_Clear_FALLINT_Flag (void)
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL1IFλ ----------------*/
	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1IF)>>CFGL_IFR_CFGL1IF_POS);
	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
}
/**
  * ����  ��CFGL �����غ��½����жϱ�־��
  * ���룺��
  * ����   ��
  */
void
CFGL1_Clear_RISEFALLINT_Flag (void)
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL2IFλ ----------------*/
	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1IF)>>CFGL_IFR_CFGL1IF_POS);
	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
}
/**
  * ����  ��CFGL2 �������жϱ�־��
  * ���룺��
  * ����   ��
  */
void
CFGL2_Clear_RISEINT_Flag (void)
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL2IFλ ----------------*/
	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2IF)>>CFGL_IFR_CFGL2IF_POS);
	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
}
/**
  * ����  ��CFGL �����غ��½����жϱ�־��
  * ���룺��
  * ����   ��
  */
void
CFGL2_Clear_RISEFALLINT_Flag (void)
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL2IFλ ----------------*/
	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2IF)>>CFGL_IFR_CFGL2IF_POS);
	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
}
/**
  * ����  ��CFGL2 �½����жϱ�־��
  * ���룺��
  * ����   ��
  */
void
CFGL2_Clear_FALLINT_Flag (void)
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL2IFλ ----------------*/
	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2IF)>>CFGL_IFR_CFGL2IF_POS);
	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
}
/**
  * ����  ��ȡCFGL2 ����״̬��
  * ���룺��
  * ���� FlagStatus
  */
FlagStatus
CFGL2_OUT_STATE ()
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL2OUTλ ----------------*/
    if (CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2OUT)
    {
        /* ��⵽CFGL2����״̬Ϊ1*/
        return SET;
    }
    else
    {
        /* ��⵽CFGL2����״̬Ϊ0 */
        return RESET;
    }
}
/**
  * ����  ��ȡCFGL1 ����״̬��
  * ���룺��
  * ���� FlagStatus
  */
FlagStatus
CFGL1_OUT_STATE ()
{
    /*---------------- ��CFGLIFR�Ĵ���CFGL1OUTλ ----------------*/
    if (CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1OUT)
    {
        /* ��⵽CFGL1����״̬Ϊ1*/
        return SET;
    }
    else
    {
        /* ��⵽CFGL1����״̬Ϊ0 */
        return RESET;
    }
}
#endif //KF32A_Periph_cfgl

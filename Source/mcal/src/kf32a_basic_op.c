/**
  ******************************************************************************
  * �ļ���  kf32a_basic_op.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��ģ���˷�(OP)���蹦�ܺ�����������
  *          + ģ���˷�(OP)��ʼ��
  *          + ģ���˷�(OP)ʹ��
  *          + ģ���˷�(OP)�����������ѡ��
  *
  *********************************************************************
  */

#include "kf32a_basic_op.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_op
/**
  *   ##### ��ʼ�������ú��� #####
  */
/**
  * ���� OP���踴λ��ʹ������ʱ�ӡ�
  * ����  : ��
  * ����  �ޡ�
  */
void
OP_Reset(void)
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_OPRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_OPRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_OPCLKEN, TRUE);
}

/**
  * ���� OPУ׼ֵ��
  * ����  : OP0/OP1/0P2/0P3
  * 	CAL_VALUE��Χλ��6bitλ��ֵ0x00-0x3f
  * ����  �ޡ�
  */
void OP_CAL_Configure( uint32_t OPx , uint32_t CAL_VALUE)
{
	 uint32_t tmpreg = 0;
	/* ����У�� */
    CHECK_RESTRICTION(CHECK_OP_ALL_PERIPH(OPx));
    CHECK_RESTRICTION(CHECK_OP_CAL_VALUE(CAL_VALUE));
    if(OPx==0)
    {
        /*------------- ����OPCAL�Ĵ���OP0CALλ�� -------------*/
    	tmpreg = CAL_VALUE << OP_CAL_OP0CAL0_POS;
    	OP_SFR->CAL = SFR_Config (OP_SFR->CAL,
									~OP_CAL_OP0CAL,
									tmpreg);
    }
    else if(OPx==1)
    {
        /*------------- ����OPCAL�Ĵ���OP1CALλ�� -------------*/
    	tmpreg = CAL_VALUE << OP_CAL_OP1CAL0_POS;
    	OP_SFR->CAL = SFR_Config (OP_SFR->CAL,
									~OP_CAL_OP1CAL,
									tmpreg);
    }
    else if(OPx==2)
    {
        /*------------- ����OPCAL�Ĵ���OP2CALλ�� -------------*/
    	tmpreg = CAL_VALUE << OP_CAL_OP2CAL0_POS;
    	OP_SFR->CAL = SFR_Config (OP_SFR->CAL,
									~OP_CAL_OP2CAL,
									tmpreg);
    }
    else if(OPx==3)
    {
        /*------------- ����OPCAL�Ĵ���OP3CALλ�� -------------*/
    	tmpreg = CAL_VALUE << OP_CAL_OP3CAL0_POS;
    	OP_SFR->CAL = SFR_Config (OP_SFR->CAL,
									~OP_CAL_OP3CAL,
									tmpreg);
    }
}
/**
  * ���� OP����ѡ��
  * ����  : OP0/OP1/0P2
  * 	CAL_VALUE��Χ:GAIN_10/ GAIN_20/ GAIN_40/ GAIN_80
  * ����  �ޡ�
  */
void OP_GAIN_SELSECT(uint32_t OPx , uint32_t GAIN_VALUE)
{
	uint32_t tmpreg = 0;
	/* ����У�� */
    CHECK_RESTRICTION(CHECK_OP_ALL_PERIPH(OPx));
    CHECK_RESTRICTION(CHECK_OP_GAIN_VALUE(GAIN_VALUE));
    if(OPx==0)
    {
        /*------------- ����OPCTL0�Ĵ���OP0GAINλ�� -------------*/
    	tmpreg = GAIN_VALUE << OP_CTL0_OP0GAIN10X_POS;
    	OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
    								~OP_CTL0_OP0GAIN,
    								tmpreg);
    }
    else if(OPx==1)
    {
        /*------------- ����OPCTL0�Ĵ���OP1GAINλ�� -------------*/
    	tmpreg = GAIN_VALUE << OP_CTL0_OP1GAIN10X_POS;
    	OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
    								~OP_CTL0_OP1GAIN,
    								tmpreg);
    }
    else if(OPx==2)
    {
        /*------------- ����OPCTL0�Ĵ���OP2GAINλ�� -------------*/
    	tmpreg = GAIN_VALUE << OP_CTL0_OP2GAIN10X_POS;
    	OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
    								~OP_CTL0_OP2GAIN,
    								tmpreg);
    }

}
/**
  * ����  �˷�3 ��������ѡ��
  * ����  : OP3_POSITIVE_INPUT_AVDD
  * 	OP3_POSITIVE_INPUT_VREF2V
  * ����  �ޡ�
  */
void OP3_POSITIVE_INPUT_SELSECT(uint32_t SEL)
{
	/* ����У�� */
	CHECK_RESTRICTION(CHECK_OP3_POSITIVE_INPUT_SEL(SEL));
    /*------------- ����OPCTL0�Ĵ���OP3VINPSELλ�� -------------*/
	OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
								~OP_CTL0_OP3VINPSEL,
								SEL);
}
/**
  * ����  �˷����ʹ��ѡ��
  * ����  : OP0/OP1/OP2/OP3
  * 	TRUE/FALSE
  * ����  �ޡ�
  */
void OP_OUTPUT_EN(uint32_t OPx,FunctionalState NewState)
{
	uint32_t tmpreg = 0;
	/* ����У�� */
	 CHECK_RESTRICTION(CHECK_OP_ALL_PERIPH(OPx));
	 CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));
	 if(OPx==0)
	 {
		/*------------- ����OPCTL0�Ĵ���OP0OUTENλ�� -------------*/
		 tmpreg = NewState << OP_CTL0_OP0OUTEN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP0OUTEN,
									tmpreg);
	 }
	 if(OPx==1)
	 {
		/*------------- ����OPCTL0�Ĵ���OP1OUTENλ�� -------------*/
		 tmpreg = NewState << OP_CTL0_OP1OUTEN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP1OUTEN,
									tmpreg);
	 }
	 if(OPx==2)
	 {
		/*------------- ����OPCTL0�Ĵ���OP2OUTENλ�� -------------*/
		 tmpreg = NewState << OP_CTL0_OP2OUTEN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP2OUTEN,
									tmpreg);
	 }
	 if(OPx==3)
	 {
		/*------------- ����OPCTL0�Ĵ���OP3OUTENλ�� -------------*/
		 tmpreg = NewState << OP_CTL0_OP3OUTEN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP3OUTEN,
									tmpreg);
	 }
}
/**
  * ����  �˷�ģ��ʹ��ѡ��
  * ����  : OP0/OP1/OP2/OP3
  * 	TRUE/FALSE
  * ����  �ޡ�
  */
void OP_MODULE_EN(uint32_t OPx,FunctionalState NewState)
{
	uint32_t tmpreg = 0;
	/* ����У�� */
	 CHECK_RESTRICTION(CHECK_OP_ALL_PERIPH(OPx));
	 CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));
	 if(OPx==0)
	 {
		/*------------- ����OPCTL0�Ĵ���OP0ENλ�� -------------*/
		 tmpreg = NewState << OP_CTL0_OP0EN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP0EN,
									tmpreg);
	 }
	 if(OPx==1)
	 {
		/*------------- ����OPCTL0�Ĵ���OP1ENλ�� -------------*/
		 tmpreg = NewState << OP_CTL0_OP1EN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP1EN,
									tmpreg);
	 }
	 if(OPx==2)
	 {
		/*------------- ����OPCTL0�Ĵ���OP2ENλ�� -------------*/
		 tmpreg = NewState << OP_CTL0_OP2EN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP2EN,
									tmpreg);
	 }
	 if(OPx==3)
	 {
		/*------------- ����OPCTL0�Ĵ���OP3ENλ�� -------------*/
		 tmpreg = NewState << OP_CTL0_OP3EN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP3EN,
									tmpreg);
	 }
}

#endif //KF32A_Periph_op

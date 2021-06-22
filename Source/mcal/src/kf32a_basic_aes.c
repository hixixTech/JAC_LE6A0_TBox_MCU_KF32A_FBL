/**
  ******************************************************************************
  * �ļ���  kf32a_basic_aes.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˼���ģ��(AE)���蹦�ܺ�����������
  *          + AES��������
  *
  *********************************************************************
  */


#include "kf32a_basic_aes.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_aes

/* AES˽�ж��� ----------------------------------------------------*/
/* AES_CTL�Ĵ����������� */
#define AES_CTL_INIT_MASK               (AES_CTL_OFBEN \
                                       | AES_CTL_AESCS \
                                       | AES_CTL_AESDIV \
                                       | AES_CTL_AESRUN \
                                       | AES_CTL_AESFIE \
                                       | AES_CTL_AESFIC)
/**
  *   ##### ��ʼ�������ú��� #####
  */
/**
  * ����AES���踴λ��ʹ������ʱ�ӡ�
  * ����  : ��
  * ����  �ޡ�
  */
void
AES_Reset ()
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_AESRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_AESRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_AESCLKEN, TRUE);
}

/**
  * ���� AES�������á�
  * ����  AES: AESInitStruct: AES������Ϣ
  * ����  �ޡ�
  */
void
AES_Configuration (AES_InitTypeDef* AESInitStruct)
{
    uint32_t tmpreg = 0;

	CHECK_RESTRICTION(CHECK_AES_OFB(AESInitStruct->m_OFB));
	CHECK_RESTRICTION(CHECK_AES_CLKSOURCE(AESInitStruct->m_ClockSelect));
	CHECK_RESTRICTION(CHECK_AES_CLK_DIV(AESInitStruct->m_ClockDiv));
	CHECK_RESTRICTION(CHECK_AES_RUN(AESInitStruct->m_Run));
	CHECK_RESTRICTION(CHECK_AES_FINISH_INT(AESInitStruct->m_FinishIntEn));
	CHECK_RESTRICTION(CHECK_AES_FINISH_INT_CLEAR(AESInitStruct->m_FinishIntClr));

	/*---------------------------- AES_CTL�Ĵ�������  -----------------*/
	/* ���ݽṹ���Աm_OFB������OFBENλ�� */
	/* ���ݽṹ���Աm_ClockSelect������AESCSλ�� */
	/* ���ݽṹ���Աm_ClockDiv������AESDIVλ�� */
	/* ���ݽṹ���Աm_Run������AESRUNλ�� */
	/* ���ݽṹ���Աm_FinishIntEn������AESFIEλ�� */
	/* ���ݽṹ���Աm_FinishIntClr������AESFICλ�� */
   tmpreg = ((AESInitStruct->m_OFB ) | \
			  (AESInitStruct->m_ClockSelect) | \
			  (AESInitStruct->m_ClockDiv) | \
			  (AESInitStruct->m_Run) | \
			  (AESInitStruct->m_FinishIntEn) | \
			  (AESInitStruct->m_FinishIntClr));

   AES_SFR->CTL = SFR_Config (AES_SFR->CTL,
			   ~AES_CTL_INIT_MASK,
			   tmpreg);

}
/**
  * ����  ��ʼ��AES������Ϣ�ṹ�塣
  * ����  AESInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
AES_Struct_Init (AES_InitTypeDef* AESInitStruct)
{
    /* ��ʼ��AES OFBģʽ�������λ */
    AESInitStruct->m_OFB = AES_OFB_DISABLE;
    /* ��ʼ�� AES����ʱ��Դѡ��λ */
    AESInitStruct->m_ClockSelect = AES_CLK_SEL_SCLK;
    /* ��ʼ�� AESʱ�ӷ�Ƶ����λ */
    AESInitStruct->m_ClockDiv = AES_CLK_DIV_1;
    /* ��ʼ�� AES����ʹ��λ */
    AESInitStruct->m_Run = AES_RUN_DISABLE;
    /* ��ʼ�� AES��������ж�ʹ��λ */
    AESInitStruct->m_FinishIntEn = AES_FINISH_INT_DISABLE;
    /* ��ʼ�� AES�����������λ */
    AESInitStruct->m_FinishIntClr = AES_FINISH_INT_CLEAR_DISABLE;

}
/**
  * ���� AES��Կ�Ĵ�����
  * ����  KEYDATA:λ��bit32
  * ����  �ޡ�
  */
void
AES_KEY0_DATA (uint32_t DATA)
{

	AES_SFR->KEY0=DATA;
}
void
AES_KEY1_DATA (uint32_t DATA)
{

	AES_SFR->KEY1=DATA;
}
void
AES_KEY2_DATA (uint32_t DATA)
{

	AES_SFR->KEY2=DATA;
}
void
AES_KEY3_DATA (uint32_t DATA)
{

	AES_SFR->KEY3=DATA;
}
/**
  * ���� AES����Ĵ�����
  * ����  INPUTDATA:λ��bit32
  * ����  �ޡ�
  */
void
AES_INPUT0_DATA (uint32_t DATA)
{

	AES_SFR->INPUT0=DATA;
}
void
AES_INPUT1_DATA (uint32_t DATA)
{

	AES_SFR->INPUT1=DATA;
}
void
AES_INPUT2_DATA (uint32_t DATA)
{

	AES_SFR->INPUT2=DATA;
}
void
AES_INPUT3_DATA (uint32_t DATA)
{

	AES_SFR->INPUT3=DATA;
}
/**
  * ���� AES����Ĵ�����
  * ���� :��
  * ����� OUTPUTDATA:λ��bit32
  * ����  �ޡ�
  */
uint32_t
GET_AES_OUTPUT0_DATA (void)
{
	uint32_t DATA;
	DATA=AES_SFR->OUTPUT0;

	return	DATA;
}
uint32_t
GET_AES_OUTPUT1_DATA (void)
{
	uint32_t DATA;
	DATA=AES_SFR->OUTPUT1;

	return	DATA;
}
uint32_t
GET_AES_OUTPUT2_DATA (void)
{
	uint32_t DATA;
	DATA=AES_SFR->OUTPUT2;

	return	DATA;
}
uint32_t
GET_AES_OUTPUT3_DATA (void)
{
	uint32_t DATA;
	DATA=AES_SFR->OUTPUT3;

	return	DATA;
}

FlagStatus
AES_Get_Flag_Status (void)
{
    if ((AES_SFR->CTL & AES_CTL_AESFIF) != RESET)
    {
        /* AES ������� */
        return SET;
    }
    else
    {
        /* AES ����δ��� */
        return RESET;
    }
}

/**
  * ���� AES��ʼ����
  * ���� :��
  * ��� :��
  * ���� :��
  */
void
AES_Start ()
{
	/* AES ��ʼ���� */
	 AES_SFR->CTL |= AES_CTL_AESRUN;
}

/**
  * ���� AES���־λ
  * ���� :��
  * ��� :��
  * ���� :��
  */
void
AES_CLR_Flag_Status ()
{
//	/* AES���־λ */
	 AES_SFR->CTL |= 0x00800000;
	 while(AES_Get_Flag_Status());//�жϱ�־Ϊ0
     AES_SFR->CTL &= 0xFF7FFFFF;
}

#endif //KF32A_Periph_aes

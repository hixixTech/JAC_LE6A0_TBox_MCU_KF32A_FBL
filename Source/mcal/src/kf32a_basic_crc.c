/**
  ******************************************************************************
  * �ļ���  kf32a_basic_crc.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˶���ʽУ��ģ��(CRC)��صĹ��ܺ�����������
  *            + ����ʽУ��ģ��(CRC)��ʼ������
  *			   + ����ʽУ��ģ��(CRC)��������
  *			   + ����ʽУ��ģ��(CRC)��������ȡ
  *
  *********************************************************************
  */

#include "kf32a_basic_crc.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef  KF32A_Periph_crc
/* CRC˽�ж��� ----------------------------------------------------*/
/* CRC_CTL�Ĵ����������� */
#define CRC_CTL_INIT_MASK               (CRC_CTL_RSLTREV \
                                       | CRC_CTL_DATAREV \
                                       | CRC_CTL_DATASIZE \
                                       | CRC_CTL_RSET)

/**
  *   ##### ��ʼ�������ú��� #####
  */
/**
  * ���� CRC���踴λ��ʹ������ʱ�ӡ�
  * ����  : ��
  * ����  �ޡ�
  */
void
CRC_Reset ()
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_CRCRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_CRCRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_CRCCLKEN, TRUE);
}

/**
  * ����  CRC�������á�
  * ����  CRCx: CRCInitStruct: CRC������Ϣ
  * ����  �ޡ�
  */
void
CRC_Configuration (CRC_InitTypeDef* CRCInitStruct)
{
    uint32_t tmpreg = 0;

	CHECK_RESTRICTION(CHECK_CRC_CAL_RSET(CRCInitStruct->m_CalUnitReset));
	CHECK_RESTRICTION(CHECK_CRC_INPUT_SEZE(CRCInitStruct->m_InputSize));
	CHECK_RESTRICTION(CHECK_CRC_INPUT_REVERSE(CRCInitStruct->m_InputReverse));
	CHECK_RESTRICTION(CHECK_CRC_RSLTREV_SEL(CRCInitStruct->m_ResultReverse));

	/*---------------------------- CRC_CTL�Ĵ�������  -----------------*/
	/* ���ݽṹ���Աm_CalUnitReset������RSETλ�� */
	/* ���ݽṹ���Աm_InputSize������DATASIZEλ�� */
	/* ���ݽṹ���Աm_InputReverse������DATAREVλ�� */
	/* ���ݽṹ���Աm_ResultReverse������RSLTREVλ�� */

   tmpreg = ((CRCInitStruct->m_CalUnitReset ) | \
			  (CRCInitStruct->m_InputSize) | \
			  (CRCInitStruct->m_InputReverse) | \
			  (CRCInitStruct->m_ResultReverse));
   CRC_SFR->CTL = SFR_Config (CRC_SFR->CTL,
			   ~CRC_CTL_INIT_MASK,
			   tmpreg);
}

/**
  * ����  ��ʼ��CRC������Ϣ�ṹ�塣
  * ����  CRCInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
CRC_Struct_Init (CRC_InitTypeDef* CRCInitStruct)
{
    /* ��ʼ��CRC���㵥Ԫ��λ����λ */
    CRCInitStruct->m_CalUnitReset = CRC_CAL_RSET_DISABLE;
    /* ��ʼ�� CRC�������ݸ�ʽ����λ */
    CRCInitStruct->m_InputSize = CRC_INPUT_SEZE_8;
    /* ��ʼ�� CRC��������λ�������λ */
    CRCInitStruct->m_InputReverse = CRC_INPUT_REVERSE_DISABLE;
    /* ��ʼ�� CRC����������λ */
    CRCInitStruct->m_ResultReverse = CRC_RESULT_REVERSE_DISABLE;
}

/**
  * ����  CRC���ݼĴ�������
  * ����  DATA: ����ֵ��
  * ����  �ޡ�
  */
void
CRC_INPUT_DATA (uint32_t DATA)
{
    /* ����CRC���ݼĴ��� */
	CRC_SFR->DATA = DATA;
}
/**
  * ����  ��ȡCRC���
  * ����  : ��
  * ����  uint32_tλ���ֵ
  */
uint32_t
CRC_GET_RESULT ()
{
    /* ��ȡCRC����Ĵ��� */
	uint32_t DATA;
	DATA=CRC_SFR->RSLT;
	return DATA;
}
/**
  * ����  ����CRC����ĳ�ʼֵ
  * ����  : ��
  * ����  uint32_tλ���ֵ
  */
void
CRC_SET_INITVALUE (uint32_t DATA)
{
    /* ����CRC����ĳ�ʼֵ�Ĵ��� */
	CRC_SFR->INIT = DATA;
}
/**
  * ����  ����CRC����ʽ
  * ����  : ��
  * ����  uint32_tλ���ֵ
  */
void
CRC_SET_PLN (uint32_t DATA)
{
    /* ����CRC����ʽ�Ĵ��� */
	CRC_SFR->PLN = DATA;
}
/**
  * ����  ����CRC������ֵ
  * ����  : ��
  * ����  uint32_tλ���ֵ
  */
void
CRC_SET_RXOR (uint32_t DATA)
{
    /* ���ý�����ֵ�Ĵ��� */
	CRC_SFR->RXOR = DATA;
}
/**
  * ����  ����CRC��������
  * ����  : ��
  * ����  uint32_tλ���ֵ
  */
void
CRC_SET_IDATA (uint32_t DATA)
{
    /* ���ö������ݼĴ��� */
	CRC_SFR->IDATA = DATA;
}
/**
  * ����  ����CRC����
  * ����  : ��
  * ����  uint32_tλ���ֵ
  */
uint32_t
CRC_GET_TEMP ()
{
    /* ���û���Ĵ��� */
	uint32_t DATA;
	DATA=CRC_SFR->TEMP;
	return DATA;
}
/**
  * ����  CRC ���㵥Ԫ��λ
  * ����  : ��
  * ����
  */
void
CRC_SET_RSET ()
{
    /* ���û���Ĵ��� */
	CRC_SFR->CTL|=CRC_CTL_RSET;
}

#endif  //KF32A_Periph_crc

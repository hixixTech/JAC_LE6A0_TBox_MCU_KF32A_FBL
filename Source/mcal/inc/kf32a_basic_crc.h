/**
  ******************************************************************************
  * �ļ���  kf32a_basic_crc.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��ѭ������У�鵥Ԫ��ع��ܺ�����������غ궨�塣
  *********************************************************************
  */

#ifndef _KF32A_BASIC_CRC_H_
#define _KF32AA_BASIC_CRC_H_

#include "KF32A_BASIC.h"


/**
  * ѭ������У�鵥Ԫ(CRC)ָ�붨��
  */
#define CHECK_CRC_PERIPH(PERIPH)     ((PERIPH) == CRC_SFR)

/**
 * CRC ������Ϣ�ṹ��
 */
typedef struct
{
 uint32_t m_CalUnitReset;       /*  CRC ���㵥Ԫ��λ����
                                    ȡ��Ϊ��CRC���㵥Ԫ��λ���ơ��е�һ�� */
 uint32_t m_InputSize;          /*  CRC �������ݸ�ʽ����λ
                                    ȡ��Ϊ��CRC�������ݸ�ʽ����λ���е�һ�� */
 uint32_t m_InputReverse;       /*  CRC �������ݷ������λ
                                    ȡ��Ϊ��CRC�������ݷ������λ���е�һ�� */
 uint32_t m_ResultReverse;      /*  CRC ����������
                                    ȡ��Ϊ��CRC���������ơ��е�һ�� */
 uint32_t m_Data;               /*  CRC ��������ֵ��ȡֵ32λ���ݣ����ݺ���Ҫ�Ҷ��� */
 uint32_t m_Result;             /*  CRC ���ֵ��ȡֵ32qλ���ݣ���ֵΪֻ�� */
 uint32_t m_InitialData;        /*  CRC �����ʼֵ��ȡֵ32λ���� */
 uint32_t m_Polynomial;         /*  CRC ����ʽֵ��ȡֵ32λ����ֵ����� */
 uint32_t m_ResultXOR;          /*  CRC ������jֵ��ȡֵ32λ����ֵ����� */
 uint32_t m_IndepentData;       /*  CRC �������ݼĴ���ֵ��ȡֵ32λ*/
 uint32_t m_Temp;               /*  CRC ����ֵ��ȡֵ32λ����ֵΪֻ�� */
} CRC_InitTypeDef;

/**
 * CRC���㵥Ԫ��λ����
 */
#define CRC_CAL_RSET_ENABLE             ((uint32_t) 0x1 << CRC_CTL_RSET_POS)
#define CRC_CAL_RSET_DISABLE            ((uint32_t) 0x0 << CRC_CTL_RSET_POS)
#define CHECK_CRC_CAL_RSET(SEL)         (((SEL) == CRC_CAL_RSET_ENABLE) \
                                        || ((SEL) == CRC_CAL_RSET_DISABLE) )

/**
 * CRC�������ݸ�ʽ����λ
 */
#define CRC_INPUT_SEZE_8                ((uint32_t) 0X0 << CRC_CTL_DATASIZE0_POS)
#define CRC_INPUT_SEZE_16               ((uint32_t) 0X1 << CRC_CTL_DATASIZE0_POS)
#define CRC_INPUT_SEZE_24               ((uint32_t) 0X2 << CRC_CTL_DATASIZE0_POS)
#define CRC_INPUT_SEZE_32               ((uint32_t) 0X3 << CRC_CTL_DATASIZE0_POS)
#define CHECK_CRC_INPUT_SEZE(SIZE)      (((SIZE) == CRC_INPUT_SEZE_8) \
                                        || ((SIZE) == CRC_INPUT_SEZE_16) \
                                        || ((SIZE) == CRC_INPUT_SEZE_24) \
                                        || ((SIZE) == CRC_INPUT_SEZE_32) )

/**
 * CRC�������ݷ������λ
 */
#define CRC_INPUT_REVERSE_ENABLE        ( (uint32_t) 0x1 << CRC_CTL_DATAREV_POS )
#define CRC_INPUT_REVERSE_DISABLE       ( (uint32_t) 0x0 << CRC_CTL_DATAREV_POS )
#define CHECK_CRC_INPUT_REVERSE(SEL)    (((SEL) == CRC_INPUT_REVERSE_ENABLE) \
                                        || ((SEL) == CRC_INPUT_REVERSE_DISABLE) )

/**
 * CRC����������
 */
#define CRC_RESULT_REVERSE_ENABLE       ((uint32_t) 0x1 << CRC_CTL_RSLTREV_POS)
#define CRC_RESULT_REVERSE_DISABLE      ((uint32_t) 0x0 << CRC_CTL_RSLTREV_POS)
#define CHECK_CRC_RSLTREV_SEL(SEL)      (((SEL) == CRC_RESULT_REVERSE_ENABLE) \
                                        || ((SEL) == CRC_RESULT_REVERSE_DISABLE))

#ifdef  KF32A_Periph_crc
void CRC_Reset ();
void CRC_Configuration (CRC_InitTypeDef* CRCInitStruct);
void CRC_Struct_Init (CRC_InitTypeDef* CRCInitStruct);
void CRC_INPUT_DATA (uint32_t DATA);
uint32_t CRC_GET_RESULT ();
void CRC_SET_INITVALUE (uint32_t DATA);
void CRC_SET_PLN (uint32_t DATA);
void CRC_SET_RXOR (uint32_t DATA);
void CRC_SET_IDATA (uint32_t DATA);
uint32_t CRC_GET_TEMP ();
void CRC_SET_RSET ();
#endif  //KF32A_Periph_crc
#endif /* _KF32A_BASIC_CRC_H */

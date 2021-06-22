/**
  ******************************************************************************
  * �ļ���  kf32a_basic_pclk.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ������ʱ��ʹ��ģ��(PCLK)��صĿ⺯����������غ궨�塣
  *
  *********************************************************************
  */

#ifndef _KF32A_BASIC_PCLK_H_
#define _KF32A_BASIC_PCLK_H_

#include "KF32A_BASIC.h"


/**
  * ����ʱ�ӿ��ƼĴ���0���ڵ�������
  */
#define CHECK_PCLK_CTL0_PERIPH(PERIPH) ((((PERIPH) >> 8) == 0x00) && ((PERIPH) != 0x00))

/**
  * ����ʱ�ӿ��ƼĴ���0���ڵ�������
  */
#define CHECK_PCLK_CTL1_PERIPH(PERIPH) ((((PERIPH) & 0x40000281) == 0x00) && ((PERIPH) != 0x00))

/**
  * ����ʱ�ӿ��ƼĴ���0���ڵ�������
  */
#define CHECK_PCLK_CTL2_PERIPH(PERIPH) ((((PERIPH) & 0x1810AFD3) == 0x00) && ((PERIPH) != 0x00))

/**
  * ����ʱ�ӿ��ƼĴ���0���ڵ�������
  */
#define CHECK_PCLK_CTL3_PERIPH(PERIPH) ((((PERIPH) & 0xFFE01F79) == 0x00) && ((PERIPH) != 0x00))


/* ����ʱ��ʹ��ģ��(PCLK)�������ú�������***********************************/
void PCLK_CTL0_Peripheral_Clock_Enable (uint32_t PCLK_CTL0_bit,
                    FunctionalState NewState);
void PCLK_CTL1_Peripheral_Clock_Enable (uint32_t PCLK_CTL1_bit,
                    FunctionalState NewState);
void PCLK_CTL2_Peripheral_Clock_Enable (uint32_t PCLK_CTL2_bit,
                    FunctionalState NewState);
void PCLK_CTL3_Peripheral_Clock_Enable (uint32_t PCLK_CTL3_bit,
                    FunctionalState NewState);

#endif /* _KF32A_BASIC_PCLK_H */

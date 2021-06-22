/**
  ******************************************************************************
  * �ļ���  kf32a_basic_rst.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˸�λģ��(RST)��ع��ܺ�����������غ궨�塣
  *
  *********************************************************************
  */

#ifndef _KF32A_BASIC_RST_H_
#define _KF32A_BASIC_RST_H_

#include "KF32A_BASIC.h"


/**
  * ��λ���ƼĴ���0���ڵ�������
  */
#define CHECK_RST_CTL0_RESET_PERIPH(PERIPH) ((((PERIPH) >> 9) == 0x00) \
                                          && ((PERIPH) != 0x00))

/**
  * ��λ���ƼĴ���1���ڵ�������
  */
#define CHECK_RST_CTL1_RESET_PERIPH(PERIPH) ((((PERIPH) & 0x40000281) == 0x00) \
                                          && ((PERIPH) != 0x00))

/**
  * ��λ���ƼĴ���2���ڵ�������
  */
#define CHECK_RST_CTL2_RESET_PERIPH(PERIPH) ((((PERIPH) & 0x1810AFD3) == 0x00) \
                                          && ((PERIPH) != 0x00))

/**
  * ��λ���ƼĴ���3���ڵ�������
  */
#define CHECK_RST_CTL3_RESET_PERIPH(PERIPH) ((((PERIPH) & 0xFFE01F79) == 0x00) \
                                          && ((PERIPH) != 0x00))


/* ��λģ��(RST)�������ú�������********************************************/
void RST_CTL0_Peripheral_Reset_Enable(uint32_t RST_CTL0Periph,
                    FunctionalState NewState);
void RST_CTL1_Peripheral_Reset_Enable(uint32_t RST_CTL1Periph,
                    FunctionalState NewState);
void RST_CTL2_Peripheral_Reset_Enable(uint32_t RST_CTL2Periph,
                    FunctionalState NewState);
void RST_CTL3_Peripheral_Reset_Enable(uint32_t RST_CTL3Periph,
                    FunctionalState NewState);

#endif /* _KF32A_BASIC_RST_H */

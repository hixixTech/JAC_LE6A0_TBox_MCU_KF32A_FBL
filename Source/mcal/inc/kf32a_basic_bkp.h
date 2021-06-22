/**
  ******************************************************************************
  * �ļ���  kf32a_basic_bkp.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˱�����(BKP)�Ŀ⺯����������غ궨�塣
  *********************************************************************
  */

#ifndef _KF32A_BASIC_BKP_H_
#define _KF32A_BASIC_BKP_H_

#include "KF32A_BASIC.h"


/**
  * ����������ѡ��
  */
#define BKP_PIN_RTC_TAMP1               ((uint32_t)0)
#define BKP_PIN_RTC_TAMP2               ((uint32_t)1)
#define BKP_PIN_RTC_TAMP3               ((uint32_t)2)
#define CHECK_BKP_PIN_RTC_TAMP(SEL)     (((SEL) == BKP_PIN_RTC_TAMP1) \
                                      || ((SEL) == BKP_PIN_RTC_TAMP2) \
                                      || ((SEL) == BKP_PIN_RTC_TAMP3))

/**
  * ��Ч��ƽ
  */
#define BKP_HIGH_LEVEL_EFFECTIVE        ((uint32_t)0)
#define BKP_LOW_LEVEL_EFFECTIVE         ((uint32_t)1)
#define CHECK_BKP_EFFECTIVE_LEVEL(SEL)  (((SEL) == BKP_HIGH_LEVEL_EFFECTIVE) \
                                      || ((SEL) == BKP_LOW_LEVEL_EFFECTIVE))

/**
  * RTCʱ��Դ
  */
#define BKP_RTC_NO_CLK                  ((uint32_t)0<<BKP_CTL_RTCCLKS0_POS)
#define BKP_RTC_EXTLF                   ((uint32_t)1<<BKP_CTL_RTCCLKS0_POS)
#define BKP_RTC_INTLF                   ((uint32_t)2<<BKP_CTL_RTCCLKS0_POS)
#define BKP_RTC_EXTHF_DIV_128           ((uint32_t)3<<BKP_CTL_RTCCLKS0_POS)
#define CHECK_BKP_RTC_CLK(SEL)          (((SEL) == BKP_RTC_NO_CLK) \
                                      || ((SEL) == BKP_RTC_EXTLF) \
                                      || ((SEL) == BKP_RTC_INTLF) \
                                      || ((SEL) == BKP_RTC_EXTHF_DIV_128))

/**
  * ʱ��Դѡ��
  */
#define BKP_EXTHF                       ((uint32_t)0)
#define BKP_EXTLF                       ((uint32_t)1)
#define CHECK_BKP_CLOCK_SOURCE(SEL)     (((SEL) == BKP_EXTLF) \
                                      || ((SEL) == BKP_EXTHF))

/**
  * �������ݼĴ������
  */
#define CHECK_BKP_DATA_NUMBER(NUM)      ((uint32_t)(NUM) < 32)


/* ������(BKP)��λ����д����������****************************************/
void BKP_Reset (void);
void BKP_Write_And_Read_Enable (FunctionalState NewState);
/* ������(BKP)�������ú�������**********************************************/
void BKP_Reset_Enable (FunctionalState NewState);
void BKP_Pin_Effective_Level_Config (uint32_t PinSel, uint32_t Effective);
void BKP_Pin_Enable (uint32_t PinSel, FunctionalState NewState);
void BKP_RTC_Clock_Config (uint32_t Source);
void BKP_External_Clock_Bypass_Enable (uint32_t Source,
                    FunctionalState NewState);
void BKP_Data_Config (uint32_t OrderNumber, uint32_t WriteData);
uint32_t BKP_Get_Data (uint32_t OrderNumber);
/* ������(BKP)�жϹ���������**********************************************/
void BKP_Pin_TAMP_INT_Enable (uint32_t PinSel, FunctionalState NewState);
FlagStatus BKP_Get_Pin_TAMP_INT_Flag (uint32_t PinSel);
void BKP_Clear_Pin_TAMP_INT_Flag (uint32_t PinSel);

#endif /* _KF32A_BASIC_BKP_H */

/**
  ********************************************************************
  * �ļ���  kf32a_basic_iwdt.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˶������Ź�(IWDT)��ع��ܺ�����������غ궨�塣
  *********************************************************************
*/

#ifndef _KF32A_BASIC_IWDT_H_
#define _KF32A_BASIC_IWDT_H_

#include "KF32A_BASIC.h"


/**
  * Ԥ��Ƶֵ
  */
#define IWDT_PRESCALER_32               ((uint32_t)0x0<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_64               ((uint32_t)0x1<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_128              ((uint32_t)0x2<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_256              ((uint32_t)0x3<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_512              ((uint32_t)0x4<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_1024             ((uint32_t)0x5<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_2048             ((uint32_t)0x6<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_4096             ((uint32_t)0x7<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_8192             ((uint32_t)0x8<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_16384            ((uint32_t)0x9<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_32768            ((uint32_t)0xA<<IWDT_CTL_IWDTPS0_POS)
#define IWDT_PRESCALER_65536            ((uint32_t)0xB<<IWDT_CTL_IWDTPS0_POS)
#define CHECK_IWDT_PRESCALER(PS)        (((PS) == IWDT_PRESCALER_32) \
                                      || ((PS) == IWDT_PRESCALER_64) \
                                      || ((PS) == IWDT_PRESCALER_128) \
                                      || ((PS) == IWDT_PRESCALER_256) \
                                      || ((PS) == IWDT_PRESCALER_512) \
                                      || ((PS) == IWDT_PRESCALER_1024) \
                                      || ((PS) == IWDT_PRESCALER_2048) \
                                      || ((PS) == IWDT_PRESCALER_4096) \
                                      || ((PS) == IWDT_PRESCALER_8192) \
                                      || ((PS) == IWDT_PRESCALER_16384) \
                                      || ((PS) == IWDT_PRESCALER_32768) \
                                      || ((PS) == IWDT_PRESCALER_65536))

/**
  * ���ֵ
  */
#define CHECK_IWDTPS_OVERFLOW(VALUE)    ((uint32_t)(VALUE) <= 0xFFF)


/* �������Ź�(IWDT)���ƹ��ܺ�������*****************************************/
void IWDT_Prescaler_Config (uint32_t Prescaler);
void IWDT_Overflow_Config (uint32_t Overflow);
void IWDT_Enable (uint32_t NewState);
void IWDT_Feed_The_Dog (void);

#endif /* _KF32A_BASIC_IWDT_H */

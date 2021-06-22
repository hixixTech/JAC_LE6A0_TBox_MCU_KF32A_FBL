/**
  ********************************************************************
  * �ļ���  kf32a_basic_op.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ���˷�(OP)��ع��ܺ�����������غ궨�塣
  *********************************************************************
*/
#ifndef _KF32A_BASIC_OP_H_
#define _KF32A_BASIC_OP_H_

#include "KF32A_BASIC.h"



#define	OP0	0
#define	OP1	1
#define	OP2	2
#define	OP3	3

#define	GAIN_10	0x1
#define	GAIN_20	0x2
#define	GAIN_40	0x4
#define	GAIN_80	0x8

#define CHECK_OP_ALL_PERIPH(PERIPH)       (((PERIPH) == OP0) \
                                          || ((PERIPH) == OP1) \
                                          || ((PERIPH) == OP2) \
                                          || ((PERIPH) == OP3))

#define CHECK_OP_GAIN_VALUE(VALUE)        (((VALUE) == GAIN_10) \
                                          || ((VALUE) == GAIN_20) \
                                          || ((VALUE) == GAIN_40) \
                                          || ((VALUE) == GAIN_80))

#define CHECK_OP_CAL_VALUE(VALUE)         ((uint32_t)(VALUE) <= 0x3F)


/**
 * �˷�2 gain 80dBʹ��λ
 */
#define OP2_GAIN_80dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP2GAIN80X_POS)
#define OP2_GAIN_80dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP2GAIN80X_POS)
#define CHECK_OP2_GAIN_80dB_SEL(SEL)      (((SEL) == OP2_GAIN_80dB_ENABLE) \
                                          || ((SEL) == OP2_GAIN_80dB_DISABLE))
/**
 * �˷�2 gain 40dBʹ��λ
 */
#define OP2_GAIN_40dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP2GAIN40X_POS)
#define OP2_GAIN_40dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP2GAIN40X_POS)
#define CHECK_OP2_GAIN_40dB_SEL(SEL)      (((SEL) == OP2_GAIN_40dB_ENABLE) \
                                          || ((SEL) == OP2_GAIN_40dB_DISABLE))
/**
 * �˷�2 gain 20dBʹ��λ
 */
#define OP2_GAIN_20dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP2GAIN20X_POS)
#define OP2_GAIN_20dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP2GAIN20X_POS)
#define CHECK_OP2_GAIN_20dB_SEL(SEL)      (((SEL) == OP2_GAIN_20dB_ENABLE) \
                                          || ((SEL) == OP2_GAIN_20dB_DISABLE))
/**
 * �˷�2 gain 10dBʹ��λ
 */
#define OP2_GAIN_10dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP2GAIN10X_POS)
#define OP2_GAIN_10dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP2GAIN10X_POS)
#define CHECK_OP2_GAIN_10dB_SEL(SEL)      (((SEL) == OP2_GAIN_10dB_ENABLE) \
                                          || ((SEL) == OP2_GAIN_10dB_DISABLE))

/**
 * �˷�1 gain 80dBʹ��λ
 */
#define OP1_GAIN_80dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP1GAIN80X_POS)
#define OP1_GAIN_80dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP1GAIN80X_POS)
#define CHECK_OP1_GAIN_80dB_SEL(SEL)      (((SEL) == OP1_GAIN_80dB_ENABLE) \
                                          || ((SEL) == OP1_GAIN_80dB_DISABLE))
/**
 * �˷�1 gain 40dBʹ��λ
 */
#define OP1_GAIN_40dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP1GAIN40X_POS)
#define OP1_GAIN_40dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP1GAIN40X_POS)
#define CHECK_OP1_GAIN_40dB_SEL(SEL)      (((SEL) == OP1_GAIN_40dB_ENABLE) \
                                          || ((SEL) == OP1_GAIN_40dB_DISABLE))
/**
 * �˷�1 gain 20dBʹ��λ
 */
#define OP1_GAIN_20dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP1GAIN20X_POS)
#define OP1_GAIN_20dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP1GAIN20X_POS)
#define CHECK_OP1_GAIN_20dB_SEL(SEL)      (((SEL) == OP1_GAIN_20dB_ENABLE) \
                                          || ((SEL) == OP1_GAIN_20dB_DISABLE))
/**
 * �˷�1 gain 10dBʹ��λ
 */
#define OP1_GAIN_10dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP1GAIN10X_POS)
#define OP1_GAIN_10dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP1GAIN10X_POS)
#define CHECK_OP1_GAIN_10dB_SEL(SEL)      (((SEL) == OP1_GAIN_10dB_ENABLE) \
                                          || ((SEL) == OP1_GAIN_10dB_DISABLE))

/**
 * �˷�0 gain 80dBʹ��λ
 */
#define OP0_GAIN_80dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP0GAIN80X_POS)
#define OP0_GAIN_80dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP0GAIN80X_POS)
#define CHECK_OP0_GAIN_80dB_SEL(SEL)      (((SEL) == OP0_GAIN_80dB_ENABLE) \
                                          || ((SEL) == OP0_GAIN_80dB_DISABLE))
/**
 * �˷�0 gain 40dBʹ��λ
 */
#define OP0_GAIN_40dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP0GAIN40X_POS)
#define OP0_GAIN_40dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP0GAIN40X_POS)
#define CHECK_OP0_GAIN_40dB_SEL(SEL)      (((SEL) == OP0_GAIN_40dB_ENABLE) \
                                          || ((SEL) == OP0_GAIN_40dB_DISABLE))
/**
 * �˷�0 gain 20dBʹ��λ
 */
#define OP0_GAIN_20dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP0GAIN20X_POS)
#define OP0_GAIN_20dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP0GAIN20X_POS)
#define CHECK_OP0_GAIN_20dB_SEL(SEL)      (((SEL) == OP0_GAIN_20dB_ENABLE) \
                                          || ((SEL) == OP0_GAIN_20dB_DISABLE))
/**
 * �˷�0 gain 10dBʹ��λ
 */
#define OP0_GAIN_10dB_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP0GAIN10X_POS)
#define OP0_GAIN_10dB_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP0GAIN10X_POS)
#define CHECK_OP0_GAIN_10dB_SEL(SEL)      (((SEL) == OP0_GAIN_10dB_ENABLE) \
                                          || ((SEL) == OP0_GAIN_10dB_DISABLE))
/**
 * �˷�3 ��������ѡ��
 */
#define OP3_POSITIVE_INPUT_AVDD            ((uint32_t) 0x1 << OP_CTL0_OP3VINPSEL0_POS)
#define OP3_POSITIVE_INPUT_VREF2V          ((uint32_t) 0x3 << OP_CTL0_OP3VINPSEL0_POS)
#define CHECK_OP3_POSITIVE_INPUT_SEL(SEL)  (((SEL) == OP3_POSITIVE_INPUT_AVDD) \
                                           || ((SEL) == OP3_POSITIVE_INPUT_VREF2V))
/**
 * �˷�3 ���ʹ��λ
 */
#define OP3_OUTPUT_IO_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP3OUTEN_POS)
#define OP3_OUTPUT_IO_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP3OUTEN_POS)
#define CHECK_OP3_OUTPUT_IO_SEL(SEL)      (((SEL) == OP3_OUTPUT_IO_ENABLE) \
                                          || ((SEL) == OP3_OUTPUT_IO_DISABLE))
/**
 * �˷�2 ���ʹ��λ
 */
#define OP2_OUTPUT_IO_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP2OUTEN_POS)
#define OP2_OUTPUT_IO_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP2OUTEN_POS)
#define CHECK_OP2_OUTPUT_IO_SEL(SEL)      (((SEL) == OP2_OUTPUT_IO_ENABLE) \
                                          || ((SEL) == OP2_OUTPUT_IO_DISABLE))
/**
 * �˷�1 ���ʹ��λ
 */
#define OP1_OUTPUT_IO_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP1OUTEN_POS)
#define OP1_OUTPUT_IO_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP1OUTEN_POS)
#define CHECK_OP1_OUTPUT_IO_SEL(SEL)      (((SEL) == OP1_OUTPUT_IO_ENABLE) \
                                          || ((SEL) == OP1_OUTPUT_IO_DISABLE))
/**
 * �˷�0 ���ʹ��λ
 */
#define OP3_OUTPUT_IO_ENABLE              ((uint32_t) 0x1 << OP_CTL0_OP3OUTEN_POS)
#define OP3_OUTPUT_IO_DISABLE             ((uint32_t) 0x0 << OP_CTL0_OP3OUTEN_POS)
#define CHECK_OP3_OUTPUT_IO_SEL(SEL)      (((SEL) == OP3_OUTPUT_IO_ENABLE) \
                                          || ((SEL) == OP3_OUTPUT_IO_DISABLE))

/**
 *  �˷�3ʹ��λ
 */
#define OP3_MODULE_ENABLE                 ((uint32_t) 0x1 << OP_CTL0_OP3EN_POS)
#define OP3_MODULE_DISABLE                ((uint32_t) 0x0 << OP_CTL0_OP3EN_POS)
#define CHECK_OP3_MODULE_EN_SEL(SEL)      (((SEL) == OP3_MODULE_ENABLE) \
                                          || ((SEL) == OP3_MODULE_DISABLE))

/**
 *  �˷�2ʹ��λ
 */
#define OP2_MODULE_ENABLE                 ((uint32_t) 0x1 << OP_CTL0_OP2EN_POS)
#define OP2_MODULE_DISABLE                ((uint32_t) 0x0 << OP_CTL0_OP2EN_POS)
#define CHECK_OP2_MODULE_EN_SEL(SEL)      (((SEL) == OP2_MODULE_ENABLE) \
                                          || ((SEL) == OP2_MODULE_DISABLE))

/**
 *  �˷�1ʹ��λ
 */
#define OP1_MODULE_ENABLE                 ((uint32_t) 0x1 << OP_CTL0_OP1EN_POS)
#define OP1_MODULE_DISABLE                ((uint32_t) 0x0 << OP_CTL0_OP1EN_POS)
#define CHECK_OP1_MODULE_EN_SEL(SEL)      (((SEL) == OP1_MODULE_ENABLE) \
                                          || ((SEL) == OP1_MODULE_DISABLE))

/**
 *  �˷�0ʹ��λ
 */
#define OP0_MODULE_ENABLE                 ((uint32_t) 0x1 << OP_CTL0_OP0EN_POS)
#define OP0_MODULE_DISABLE                ((uint32_t) 0x0 << OP_CTL0_OP0EN_POS)
#define CHECK_OP0_MODULE_EN_SEL(SEL)      (((SEL) == OP0_MODULE_ENABLE) \
                                          || ((SEL) == OP0_MODULE_DISABLE))

#ifdef KF32A_Periph_op
void OP_Reset(void);
void OP_CAL_Configure( uint32_t OPx , uint32_t CAL_VALUE);
void OP_GAIN_SELSECT(uint32_t OPx , uint32_t GAIN_VALUE);
void OP_OUTPUT_EN(uint32_t OPx,FunctionalState NewState);
void OP_MODULE_EN(uint32_t OPx,FunctionalState NewState);
void OP3_POSITIVE_INPUT_SELSECT(uint32_t SEL);
#endif //KF32A_Periph_op
#endif /* _KF32A_BASIC_OP_H_ */

/**
  ******************************************************************************
  * �ļ���  kf32a_basic_lcd.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��Һ����ʾ(LCD)��ع��ܺ�����������غ궨�塣
  *********************************************************************
  */

#ifndef _KF32A_BASIC_LCD_H
#define _KF32A_BASIC_LCD_H

#include "KF32A_BASIC.h"


/**
  * ����  LCDҺ����ʾ������Ϣ�ṹ��
  */
typedef struct
{
    uint64_t m_SegPin;                 /* SEG���ű������ѡ��
                                          ȡֵΪ�ꡰLCD �����ű�����롱�е�һ��������ϡ� */
    uint32_t m_SegPinEn;               /* �����Ź��ܣ�
                                          ȡֵΪ�ꡰLCD �����Ź��ܡ��е�һ���� */
    FunctionalState m_Analog;          /* LCDģ�ⲿ��ʹ�ܣ�
                                          ȡֵΪTRUE��FALSE�� */
    uint32_t m_ClockSource;            /* LCDʱ��Դ��ѡ���ڲ���Ƶ����INTLFʱ��
                                          �⺯����ʹ��PM_CTL2�Ĵ�����LCDCLKLPENλ��
                                          ȡֵΪ�ꡰLCDʱ��Դѡ���е�һ�� */
    uint32_t m_SourcePrescaler;        /* LCDʱ��Դ��Ƶ��
                                          ȡֵΪ�ꡰLCDʱ��Դ��Ƶ���е�һ���� */
    uint32_t m_LCDPrescaler;           /* LCDԤ��Ƶ�ȣ�
                                          ȡֵΪ�ꡰLCDԤ��Ƶ�ȡ��е�һ���� */
    uint32_t m_CommonPort;             /* LCD������ѡ��
                                          ȡֵΪ�ꡰ������ѡ���е�һ���� */
    uint32_t m_VoltageSelect;          /* LCD�ڲ�/�ⲿ��Դѡ��
                                          ȡֵΪ�ꡰLCD�ڲ�/�ⲿ��Դѡ���е�һ���� */
}LCD_InitTypeDef;

/**
  * �ڲ�ƫѹԴ��ѹ
  */
#define LCD_BIAS_VOLTAGE_3P04           ((uint32_t)0<<LCD_CTL_TRIMCPVOL0_POS)
#define LCD_BIAS_VOLTAGE_2P89           ((uint32_t)1<<LCD_CTL_TRIMCPVOL0_POS)
#define LCD_BIAS_VOLTAGE_2P76           ((uint32_t)2<<LCD_CTL_TRIMCPVOL0_POS)
#define LCD_BIAS_VOLTAGE_2P62           ((uint32_t)3<<LCD_CTL_TRIMCPVOL0_POS)
#define LCD_BIAS_VOLTAGE_3P62           ((uint32_t)4<<LCD_CTL_TRIMCPVOL0_POS)
#define LCD_BIAS_VOLTAGE_3P46           ((uint32_t)5<<LCD_CTL_TRIMCPVOL0_POS)
#define LCD_BIAS_VOLTAGE_3P32           ((uint32_t)6<<LCD_CTL_TRIMCPVOL0_POS)
#define LCD_BIAS_VOLTAGE_3P19           ((uint32_t)7<<LCD_CTL_TRIMCPVOL0_POS)
#define CHECK_LCD_BIAS_VOLTAGE(VOL)     (((VOL) == LCD_BIAS_VOLTAGE_3P04) \
                                      || ((VOL) == LCD_BIAS_VOLTAGE_2P89) \
                                      || ((VOL) == LCD_BIAS_VOLTAGE_2P76) \
                                      || ((VOL) == LCD_BIAS_VOLTAGE_2P62) \
                                      || ((VOL) == LCD_BIAS_VOLTAGE_3P62) \
                                      || ((VOL) == LCD_BIAS_VOLTAGE_3P46) \
                                      || ((VOL) == LCD_BIAS_VOLTAGE_3P32) \
                                      || ((VOL) == LCD_BIAS_VOLTAGE_3P19))

/**
  * ������ѡ��
  */
#define LCD_COMMON_PORT_STATIC          ((uint32_t)0<<LCD_CTL_LMUX0_POS)
#define LCD_COMMON_PORT_DIV_2           ((uint32_t)1<<LCD_CTL_LMUX0_POS)
#define LCD_COMMON_PORT_DIV_3           ((uint32_t)2<<LCD_CTL_LMUX0_POS)
#define LCD_COMMON_PORT_DIV_4           ((uint32_t)3<<LCD_CTL_LMUX0_POS)
#define LCD_COMMON_PORT_DIV_8           ((uint32_t)7<<LCD_CTL_LMUX0_POS)
#define CHECK_LCD_COMMON_PORT(PORT)     (((PORT) == LCD_COMMON_PORT_STATIC) \
                                      || ((PORT) == LCD_COMMON_PORT_DIV_2) \
                                      || ((PORT) == LCD_COMMON_PORT_DIV_3) \
                                      || ((PORT) == LCD_COMMON_PORT_DIV_4) \
                                      || ((PORT) == LCD_COMMON_PORT_DIV_8))

/**
  * LCDʱ��Դ��Ƶ
  */
#define LCD_SOURCE_DIVIDE_2             ((uint32_t)0<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_4             ((uint32_t)2<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_8             ((uint32_t)3<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_16            ((uint32_t)4<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_32            ((uint32_t)5<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_64            ((uint32_t)6<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_128           ((uint32_t)7<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_256           ((uint32_t)8<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_512           ((uint32_t)9<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_1024          ((uint32_t)0xA<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_2048          ((uint32_t)0xB<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_4096          ((uint32_t)0xC<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_8192          ((uint32_t)0xD<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_16384         ((uint32_t)0xE<<LCD_CTL_FR0_POS)
#define LCD_SOURCE_DIVIDE_32768         ((uint32_t)0xF<<LCD_CTL_FR0_POS)
#define CHECK_LCD_SOURCE_DIVIDE(DIV)    (((DIV) == LCD_SOURCE_DIVIDE_2) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_4) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_8) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_16) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_32) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_64) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_128) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_256) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_512) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_1024) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_2048) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_4096) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_8192) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_16384) \
                                      || ((DIV) == LCD_SOURCE_DIVIDE_32768))

/**
  * LCD�ڲ�/�ⲿ��Դѡ��
  */
#define LCD_VOLTAGE_INTERNAL            ((uint32_t)0)
#define LCD_VOLTAGE_EXTERNAL            ((uint32_t)1)
#define CHECK_LCD_VOLTAGE_SELECT(CFG)   (((CFG) == LCD_VOLTAGE_INTERNAL) \
                                      || ((CFG) == LCD_VOLTAGE_EXTERNAL))

/**
  * LCDʱ��Դѡ��
  */
#define LCD_SOURCE_HALF_SCLK            ((uint32_t)0<<LCD_CTL_CS0_POS)
#define LCD_SOURCE_HALF_HFCLK           ((uint32_t)1<<LCD_CTL_CS0_POS)
#define LCD_SOURCE_HALF_LFCLK           ((uint32_t)2<<LCD_CTL_CS0_POS)
#define LCD_SOURCE_HALF_INTLF           ((uint32_t)3<<LCD_CTL_CS0_POS)
#define CHECK_LCD_SOURCE(CLK)           (((CLK) == LCD_SOURCE_HALF_SCLK) \
                                      || ((CLK) == LCD_SOURCE_HALF_HFCLK) \
                                      || ((CLK) == LCD_SOURCE_HALF_LFCLK) \
                                      || ((CLK) == LCD_SOURCE_HALF_INTLF))

/**
  * LCD��������
  */
#define LCD_WAVE_TYPE_A                 ((uint32_t)0<<LCD_CTL_WFT_POS)
#define LCD_WAVE_TYPE_B                 ((uint32_t)1<<LCD_CTL_WFT_POS)
#define CHECK_LCD_WAVE_TYPE(TYPE)       (((TYPE) == LCD_WAVE_TYPE_A) \
                                      || ((TYPE) == LCD_WAVE_TYPE_B))

/**
  * LCDƫ��ģʽ
  */
#define LCD_BIAS_MODE_STATIC            ((uint32_t)0<<LCD_CTL_BIASMD_POS)
#define LCD_BIAS_MODE_1_DIV_3           ((uint32_t)0<<LCD_CTL_BIASMD_POS)
#define LCD_BIAS_MODE_1_DIV_2           ((uint32_t)1<<LCD_CTL_BIASMD_POS)
#define CHECK_LCD_BIAS_MODE(MODE)       (((MODE) == LCD_BIAS_MODE_STATIC) \
                                      || ((MODE) == LCD_BIAS_MODE_1_DIV_3) \
                                      || ((MODE) == LCD_BIAS_MODE_1_DIV_2))

/**
  * LCDԤ��Ƶ��
  */
#define LCD_PRESCALER_1                 ((uint32_t)0<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_2                 ((uint32_t)1<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_3                 ((uint32_t)2<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_4                 ((uint32_t)3<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_5                 ((uint32_t)4<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_6                 ((uint32_t)5<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_7                 ((uint32_t)6<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_8                 ((uint32_t)7<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_9                 ((uint32_t)8<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_10                ((uint32_t)9<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_11                ((uint32_t)10<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_12                ((uint32_t)11<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_13                ((uint32_t)12<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_14                ((uint32_t)13<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_15                ((uint32_t)14<<LCD_CTL_LP0_POS)
#define LCD_PRESCALER_16                ((uint32_t)15<<LCD_CTL_LP0_POS)
#define CHECK_LCD_PRESCALER(PRE)        (((uint32_t)(PRE)>>LCD_CTL_LP0_POS) <= 0xF)

/**
  * LCDʱ�䵥λ��С
  */
#define LCD_TIME_SLICE_EQU_2_FRAME      ((uint32_t)0<<LCD_PTL_TS0_POS)
#define LCD_TIME_SLICE_EQU_4_FRAME      ((uint32_t)1<<LCD_PTL_TS0_POS)
#define LCD_TIME_SLICE_EQU_8_FRAME      ((uint32_t)2<<LCD_PTL_TS0_POS)
#define LCD_TIME_SLICE_EQU_16_FRAME     ((uint32_t)3<<LCD_PTL_TS0_POS)
#define LCD_TIME_SLICE_EQU_32_FRAME     ((uint32_t)4<<LCD_PTL_TS0_POS)
#define LCD_TIME_SLICE_EQU_64_FRAME     ((uint32_t)5<<LCD_PTL_TS0_POS)
#define LCD_TIME_SLICE_EQU_128_FRAME    ((uint32_t)6<<LCD_PTL_TS0_POS)
#define LCD_TIME_SLICE_EQU_256_FRAME    ((uint32_t)7<<LCD_PTL_TS0_POS)
#define CHECK_LCD_TIME_SLICE(FRAME)     (((FRAME) == LCD_TIME_SLICE_EQU_2_FRAME) \
                                      || ((FRAME) == LCD_TIME_SLICE_EQU_4_FRAME) \
                                      || ((FRAME) == LCD_TIME_SLICE_EQU_8_FRAME) \
                                      || ((FRAME) == LCD_TIME_SLICE_EQU_16_FRAME) \
                                      || ((FRAME) == LCD_TIME_SLICE_EQU_32_FRAME) \
                                      || ((FRAME) == LCD_TIME_SLICE_EQU_64_FRAME) \
                                      || ((FRAME) == LCD_TIME_SLICE_EQU_128_FRAME) \
                                      || ((FRAME) == LCD_TIME_SLICE_EQU_256_FRAME))

/**
  * LCD����ʱ��͹ر�ʱ�����
  */
#define LCD_WORK_VS_STOP_8_8            ((uint32_t)0<<LCD_PTL_IF0_POS)
#define LCD_WORK_VS_STOP_7_9            ((uint32_t)1<<LCD_PTL_IF0_POS)
#define LCD_WORK_VS_STOP_6_10           ((uint32_t)2<<LCD_PTL_IF0_POS)
#define LCD_WORK_VS_STOP_5_11           ((uint32_t)3<<LCD_PTL_IF0_POS)
#define LCD_WORK_VS_STOP_4_12           ((uint32_t)4<<LCD_PTL_IF0_POS)
#define LCD_WORK_VS_STOP_3_13           ((uint32_t)5<<LCD_PTL_IF0_POS)
#define LCD_WORK_VS_STOP_2_14           ((uint32_t)6<<LCD_PTL_IF0_POS)
#define LCD_WORK_VS_STOP_1_15           ((uint32_t)7<<LCD_PTL_IF0_POS)
#define CHECK_LCD_WORK_VS_STOP(VS)      (((VS) == LCD_WORK_VS_STOP_8_8) \
                                      || ((VS) == LCD_WORK_VS_STOP_7_9) \
                                      || ((VS) == LCD_WORK_VS_STOP_6_10) \
                                      || ((VS) == LCD_WORK_VS_STOP_5_11) \
                                      || ((VS) == LCD_WORK_VS_STOP_4_12) \
                                      || ((VS) == LCD_WORK_VS_STOP_3_13) \
                                      || ((VS) == LCD_WORK_VS_STOP_2_14) \
                                      || ((VS) == LCD_WORK_VS_STOP_1_15))

/**
  * LCD ����ģʽ����
  */
#define LCD_HIGH_RESISTANCE_NO_BUF      ((uint32_t)0)
#define LCD_LOW_RESISTANCE_NO_BUF    	((uint32_t)1)
#define LCD_HIGH_RESISTANCE_WITH_BUF    ((uint32_t)2)
#define LCD_LOW_RESISTANCE_WITH_BUF     ((uint32_t)3)
#define CHECK_LCD_POWER_CONTROL(CTL)    (((CTL) == LCD_HIGH_RESISTANCE_NO_BUF) \
                                      || ((CTL) == LCD_LOW_RESISTANCE_NO_BUF) \
                                      || ((CTL) == LCD_HIGH_RESISTANCE_WITH_BUF) \
                                      || ((CTL) == LCD_LOW_RESISTANCE_WITH_BUF))

/**
  * LCD ʱ�������ȿ���
  */
#define LCD_POWER_B_MODE                ((uint32_t)0<<LCD_PTL_ATIC0_POS)
#define LCD_POWER_A_VS_B_IS_1_15        ((uint32_t)1<<LCD_PTL_ATIC0_POS)
#define LCD_POWER_A_VS_B_IS_2_14        ((uint32_t)2<<LCD_PTL_ATIC0_POS)
#define LCD_POWER_A_VS_B_IS_3_13        ((uint32_t)3<<LCD_PTL_ATIC0_POS)
#define LCD_POWER_A_VS_B_IS_4_12        ((uint32_t)4<<LCD_PTL_ATIC0_POS)
#define LCD_POWER_A_VS_B_IS_5_11        ((uint32_t)5<<LCD_PTL_ATIC0_POS)
#define LCD_POWER_A_VS_B_IS_6_10        ((uint32_t)6<<LCD_PTL_ATIC0_POS)
#define LCD_POWER_A_VS_B_IS_7_9         ((uint32_t)7<<LCD_PTL_ATIC0_POS)
#define CHECK_LCD_POWER_MODE(MODE)      (((MODE) == LCD_POWER_B_MODE) \
                                      || ((MODE) == LCD_POWER_A_VS_B_IS_1_15) \
                                      || ((MODE) == LCD_POWER_A_VS_B_IS_2_14) \
                                      || ((MODE) == LCD_POWER_A_VS_B_IS_3_13) \
                                      || ((MODE) == LCD_POWER_A_VS_B_IS_4_12) \
                                      || ((MODE) == LCD_POWER_A_VS_B_IS_5_11) \
                                      || ((MODE) == LCD_POWER_A_VS_B_IS_6_10) \
                                      || ((MODE) == LCD_POWER_A_VS_B_IS_7_9))

/**
  * LCD �����ű������
  */
#define LCD_SEG_PIN_0                   ((uint64_t)1<<0)
#define LCD_SEG_PIN_1                   ((uint64_t)1<<1)
#define LCD_SEG_PIN_2                   ((uint64_t)1<<2)
#define LCD_SEG_PIN_3                   ((uint64_t)1<<3)
#define LCD_SEG_PIN_4                   ((uint64_t)1<<4)
#define LCD_SEG_PIN_5                   ((uint64_t)1<<5)
#define LCD_SEG_PIN_6                   ((uint64_t)1<<6)
#define LCD_SEG_PIN_7                   ((uint64_t)1<<7)
#define LCD_SEG_PIN_8                   ((uint64_t)1<<8)
#define LCD_SEG_PIN_9                   ((uint64_t)1<<9)
#define LCD_SEG_PIN_10                  ((uint64_t)1<<10)
#define LCD_SEG_PIN_11                  ((uint64_t)1<<11)
#define LCD_SEG_PIN_12                  ((uint64_t)1<<12)
#define LCD_SEG_PIN_13                  ((uint64_t)1<<13)
#define LCD_SEG_PIN_14                  ((uint64_t)1<<14)
#define LCD_SEG_PIN_15                  ((uint64_t)1<<15)
#define LCD_SEG_PIN_16                  ((uint64_t)1<<16)
#define LCD_SEG_PIN_17                  ((uint64_t)1<<17)
#define LCD_SEG_PIN_18                  ((uint64_t)1<<18)
#define LCD_SEG_PIN_19                  ((uint64_t)1<<19)
#define LCD_SEG_PIN_20                  ((uint64_t)1<<20)
#define LCD_SEG_PIN_21                  ((uint64_t)1<<21)
#define LCD_SEG_PIN_22                  ((uint64_t)1<<22)
#define LCD_SEG_PIN_23                  ((uint64_t)1<<23)
#define LCD_SEG_PIN_24                  ((uint64_t)1<<24)
#define LCD_SEG_PIN_25                  ((uint64_t)1<<25)
#define LCD_SEG_PIN_26                  ((uint64_t)1<<26)
#define LCD_SEG_PIN_27                  ((uint64_t)1<<27)
#define LCD_SEG_PIN_28                  ((uint64_t)1<<28)
#define LCD_SEG_PIN_29                  ((uint64_t)1<<29)
#define LCD_SEG_PIN_30                  ((uint64_t)1<<30)
#define LCD_SEG_PIN_31                  ((uint64_t)1<<31)
#define LCD_SEG_PIN_32                  ((uint64_t)1<<32)
#define LCD_SEG_PIN_33                  ((uint64_t)1<<33)
#define LCD_SEG_PIN_34                  ((uint64_t)1<<34)
#define LCD_SEG_PIN_35                  ((uint64_t)1<<35)
#define LCD_SEG_PIN_36                  ((uint64_t)1<<36)
#define LCD_SEG_PIN_37                  ((uint64_t)1<<37)
#define LCD_SEG_PIN_38                  ((uint64_t)1<<38)
#define LCD_SEG_PIN_39                  ((uint64_t)1<<39)
#define LCD_SEG_PIN_40                  ((uint64_t)1<<40)
#define LCD_SEG_PIN_41                  ((uint64_t)1<<41)
#define LCD_SEG_PIN_42                  ((uint64_t)1<<42)
#define LCD_SEG_PIN_43                  ((uint64_t)1<<43)
#define LCD_SEG_PIN_44                  ((uint64_t)1<<44)
#define LCD_SEG_PIN_45                  ((uint64_t)1<<45)
#define LCD_SEG_PIN_46                  ((uint64_t)1<<46)
#define LCD_SEG_PIN_47                  ((uint64_t)1<<47)
#define LCD_SEG_PIN_ALL                 (((uint64_t)1<<48) - 1)
#define CHECK_LCD_SEG_PIN(NUM)          (((uint64_t)(NUM) >> 48) == 0)

/**
  * LCD �����ű��
  */
#define LCD_PIN_SOURCE_SEG0             ((uint32_t)0)
#define LCD_PIN_SOURCE_SEG1             ((uint32_t)1)
#define LCD_PIN_SOURCE_SEG2             ((uint32_t)2)
#define LCD_PIN_SOURCE_SEG3             ((uint32_t)3)
#define LCD_PIN_SOURCE_SEG4             ((uint32_t)4)
#define LCD_PIN_SOURCE_SEG5             ((uint32_t)5)
#define LCD_PIN_SOURCE_SEG6             ((uint32_t)6)
#define LCD_PIN_SOURCE_SEG7             ((uint32_t)7)
#define LCD_PIN_SOURCE_SEG8             ((uint32_t)8)
#define LCD_PIN_SOURCE_SEG9             ((uint32_t)9)
#define LCD_PIN_SOURCE_SEG10            ((uint32_t)10)
#define LCD_PIN_SOURCE_SEG11            ((uint32_t)11)
#define LCD_PIN_SOURCE_SEG12            ((uint32_t)12)
#define LCD_PIN_SOURCE_SEG13            ((uint32_t)13)
#define LCD_PIN_SOURCE_SEG14            ((uint32_t)14)
#define LCD_PIN_SOURCE_SEG15            ((uint32_t)15)
#define LCD_PIN_SOURCE_SEG16            ((uint32_t)16)
#define LCD_PIN_SOURCE_SEG17            ((uint32_t)17)
#define LCD_PIN_SOURCE_SEG18            ((uint32_t)18)
#define LCD_PIN_SOURCE_SEG19            ((uint32_t)19)
#define LCD_PIN_SOURCE_SEG20            ((uint32_t)20)
#define LCD_PIN_SOURCE_SEG21            ((uint32_t)21)
#define LCD_PIN_SOURCE_SEG22            ((uint32_t)22)
#define LCD_PIN_SOURCE_SEG23            ((uint32_t)23)
#define LCD_PIN_SOURCE_SEG24            ((uint32_t)24)
#define LCD_PIN_SOURCE_SEG25            ((uint32_t)25)
#define LCD_PIN_SOURCE_SEG26            ((uint32_t)26)
#define LCD_PIN_SOURCE_SEG27            ((uint32_t)27)
#define LCD_PIN_SOURCE_SEG28            ((uint32_t)28)
#define LCD_PIN_SOURCE_SEG29            ((uint32_t)29)
#define LCD_PIN_SOURCE_SEG30            ((uint32_t)30)
#define LCD_PIN_SOURCE_SEG31            ((uint32_t)31)
#define LCD_PIN_SOURCE_SEG32            ((uint32_t)32)
#define LCD_PIN_SOURCE_SEG33            ((uint32_t)33)
#define LCD_PIN_SOURCE_SEG34            ((uint32_t)34)
#define LCD_PIN_SOURCE_SEG35            ((uint32_t)35)
#define LCD_PIN_SOURCE_SEG36            ((uint32_t)36)
#define LCD_PIN_SOURCE_SEG37            ((uint32_t)37)
#define LCD_PIN_SOURCE_SEG38            ((uint32_t)38)
#define LCD_PIN_SOURCE_SEG39            ((uint32_t)39)
#define LCD_PIN_SOURCE_SEG40            ((uint32_t)40)
#define LCD_PIN_SOURCE_SEG41            ((uint32_t)41)
#define LCD_PIN_SOURCE_SEG42            ((uint32_t)42)
#define LCD_PIN_SOURCE_SEG43            ((uint32_t)43)
#define LCD_PIN_SOURCE_SEG44            ((uint32_t)44)
#define LCD_PIN_SOURCE_SEG45            ((uint32_t)45)
#define LCD_PIN_SOURCE_SEG46            ((uint32_t)46)
#define LCD_PIN_SOURCE_SEG47            ((uint32_t)47)
#define CHECK_LCD_PIN_SOURCE_SEG(SEG)   ((uint32_t)(SEG) < 47)

/**
  * LCD COM���ű������
  */
#define LCD_COM_PIN_0                   ((uint32_t)1<<0)
#define LCD_COM_PIN_1                   ((uint32_t)1<<1)
#define LCD_COM_PIN_2                   ((uint32_t)1<<2)
#define LCD_COM_PIN_3                   ((uint32_t)1<<3)
#define LCD_COM_PIN_4                   ((uint32_t)1<<4)
#define LCD_COM_PIN_5                   ((uint32_t)1<<5)
#define LCD_COM_PIN_6                   ((uint32_t)1<<6)
#define LCD_COM_PIN_7                   ((uint32_t)1<<7)
#define LCD_COM_PIN_ALL                 (((uint32_t)1<<8) - 1)
#define CHECK_LCD_COM_PIN(NUM)          (((uint32_t)(NUM) >> 8) == 0)

/**
  * LCD COM���ű��
  */
#define LCD_PIN_SOURCE_COM0             ((uint32_t)0)
#define LCD_PIN_SOURCE_COM1             ((uint32_t)1)
#define LCD_PIN_SOURCE_COM2             ((uint32_t)2)
#define LCD_PIN_SOURCE_COM3             ((uint32_t)3)
#define LCD_PIN_SOURCE_COM4             ((uint32_t)4)
#define LCD_PIN_SOURCE_COM5             ((uint32_t)5)
#define LCD_PIN_SOURCE_COM6             ((uint32_t)6)
#define LCD_PIN_SOURCE_COM7             ((uint32_t)7)
#define CHECK_LCD_PIN_SOURCE_COM(COM)   ((uint32_t)(COM) <= 7)

/**
  * LCD �����Ź���
  */
#define LCD_SEG_PIN_IO                  ((uint32_t)0)
#define LCD_SEG_PIN_SEG                 ((uint32_t)1)
#define CHECK_LCD_SEG_FUNCTION(FUNC)    (((FUNC) == LCD_SEG_PIN_IO) \
                                      || ((FUNC) == LCD_SEG_PIN_SEG))

/**
 * LCD COM�˿�ѡ��
 */
#define	LCD_COM_SEL_0                  ((uint16_t) 0)
#define	LCD_COM_SEL_1                  ((uint16_t) 1)
#define LCD_COM_SEL_2                  ((uint16_t) 2)
#define LCD_COM_SEL_3                  ((uint16_t) 3)
#define LCD_COM_SEL_4                  ((uint16_t) 4)
#define LCD_COM_SEL_5                  ((uint16_t) 5)
#define LCD_COM_SEL_6                  ((uint16_t) 6)
#define LCD_COM_SEL_7                  ((uint16_t) 7)
#define CHECK_LCD_COM_SEL(SEL)         ((SEL>>3) ==0)

/**
  * LCD ���ص���״̬
  */
#define LCD_PIXEL_TRANSPARENCY          ((uint32_t)0)
#define LCD_PIXEL_LIGHT                 ((uint32_t)1)
#define CHECK_LCD_PIXEL(LIGHT)          (((LIGHT) == LCD_PIXEL_LIGHT) \
                                      || ((LIGHT) == LCD_PIXEL_TRANSPARENCY))


/**
  * LCD ͨ����ӦIO������
  */
#define LCD_IO_NONE						((uint32_t)0xFF0)
#define LCD_SEG0_CONFIGURE_PB4      	((uint32_t)0x000)
#define LCD_SEG0_CONFIGURE_PD13      	((uint32_t)0x001)
#define LCD_SEG1_CONFIGURE_PB5          ((uint32_t)0x010)
#define LCD_SEG1_CONFIGURE_PF5          ((uint32_t)0x011)
#define LCD_SEG2_CONFIGURE_PB6          ((uint32_t)0x020)
#define LCD_SEG2_CONFIGURE_PC13         ((uint32_t)0x021)
#define LCD_SEG3_CONFIGURE_PB7          ((uint32_t)0x030)
#define LCD_SEG3_CONFIGURE_PH15         ((uint32_t)0x031)
#define LCD_SEG10_CONFIGURE_PC0         ((uint32_t)0x0A0)
#define LCD_SEG10_CONFIGURE_PH12        ((uint32_t)0x0A1)
#define LCD_SEG11_CONFIGURE_PC1         ((uint32_t)0x0B0)
#define LCD_SEG11_CONFIGURE_PH13        ((uint32_t)0x0B1)
#define LCD_SEG12_CONFIGURE_PC2         ((uint32_t)0x0C0)
#define LCD_SEG12_CONFIGURE_PH11        ((uint32_t)0x0C1)
#define LCD_COM0_CONFIGURE_PB0         	((uint32_t)0x300)
#define LCD_COM0_CONFIGURE_PB14         ((uint32_t)0x301)
#define LCD_COM1_CONFIGURE_PB1          ((uint32_t)0x310)
#define LCD_COM1_CONFIGURE_PB15         ((uint32_t)0x311)
#define LCD_COM2_CONFIGURE_PB2          ((uint32_t)0x320)
#define LCD_COM2_CONFIGURE_PF0          ((uint32_t)0x321)
#define LCD_COM4_CONFIGURE_PD13         ((uint32_t)0x341)
#define LCD_COM4_CONFIGURE_PB4          ((uint32_t)0x340)
#define LCD_COM5_CONFIGURE_PF5          ((uint32_t)0x351)
#define LCD_COM5_CONFIGURE_PB5          ((uint32_t)0x350)
#define LCD_COM6_CONFIGURE_PC13         ((uint32_t)0x361)
#define LCD_COM6_CONFIGURE_PB6          ((uint32_t)0x360)
#define LCD_COM7_CONFIGURE_PH15         ((uint32_t)0x371)
#define LCD_COM7_CONFIGURE_PB7          ((uint32_t)0x370)
#define CHECK_LCD_IO(IO)			    (((IO) == LCD_IO_NONE) \
									  || ((IO) == LCD_SEG0_CONFIGURE_PB4) \
                                      || ((IO) == LCD_SEG0_CONFIGURE_PD13) \
                                      || ((IO) == LCD_SEG1_CONFIGURE_PB5) \
                                      || ((IO) == LCD_SEG1_CONFIGURE_PF5) \
                                      || ((IO) == LCD_SEG2_CONFIGURE_PB6) \
                                      || ((IO) == LCD_SEG2_CONFIGURE_PC13) \
                                      || ((IO) == LCD_SEG3_CONFIGURE_PB7) \
                                      || ((IO) == LCD_SEG3_CONFIGURE_PH15) \
                                      || ((IO) == LCD_SEG10_CONFIGURE_PC0) \
                                      || ((IO) == LCD_SEG10_CONFIGURE_PH12) \
                                      || ((IO) == LCD_SEG11_CONFIGURE_PC1) \
                                      || ((IO) == LCD_SEG11_CONFIGURE_PH13) \
                                      || ((IO) == LCD_SEG12_CONFIGURE_PC2) \
                                      || ((IO) == LCD_SEG12_CONFIGURE_PH11)\
                                      || ((IO) == LCD_COM0_CONFIGURE_PB0) \
									  || ((IO) == LCD_COM0_CONFIGURE_PB14) \
									  || ((IO) == LCD_COM1_CONFIGURE_PB1) \
									  || ((IO) == LCD_COM1_CONFIGURE_PB15) \
                                      || ((IO) == LCD_COM2_CONFIGURE_PB2) \
                                      || ((IO) == LCD_COM2_CONFIGURE_PF0) \
									  || ((IO) == LCD_COM4_CONFIGURE_PD13) \
									  || ((IO) == LCD_COM4_CONFIGURE_PB4) \
									  || ((IO) == LCD_COM5_CONFIGURE_PF5) \
									  || ((IO) == LCD_COM5_CONFIGURE_PB5) \
									  || ((IO) == LCD_COM6_CONFIGURE_PC13) \
									  || ((IO) == LCD_COM6_CONFIGURE_PB6) \
									  || ((IO) == LCD_COM7_CONFIGURE_PH15) \
									  || ((IO) == LCD_COM7_CONFIGURE_PB7))

/**
  * LCD COMͨ����ӦIO������
  */
#define CHECK_LCD_DATA(DATA)            (((uint64_t)(DATA) >> 48) == 0)

/**
  * LCD�ж��¼�
  * ��ֵΪ8λ��Ч��������λΪ�жϱ�־λ�ã�����λΪ�ж�ʹ��λ�á�
  */
#define LCD_INT_FRAME                   ((uint32_t)0x40)
#define LCD_INT_FLICKER_OFF             ((uint32_t)0x51)
#define LCD_INT_FLICKER_ON              ((uint32_t)0x62)
#define LCD_INT_UPDATA                  ((uint32_t)0xC3)
#define CHECK_LCD_INT_EVENT(EVENT)      (((EVENT) == LCD_INT_FRAME) \
                                      || ((EVENT) == LCD_INT_FLICKER_OFF) \
                                      || ((EVENT) == LCD_INT_FLICKER_ON) \
                                      || ((EVENT) == LCD_INT_UPDATA))


#ifdef KF32A_Periph_lcd
/* Һ����ʾ(LCD)��ʼ����������**********************************************/
void LCD_Reset (void);
void LCD_Configuration(LCD_InitTypeDef* lcdInitStruct);
void LCD_Struct_Init (LCD_InitTypeDef* lcdInitStruct);
/* Һ����ʾ(LCD)�������ú�������********************************************/
void LCD_Cmd_Enable (FunctionalState NewState);
void LCD_Analog_Enable (FunctionalState NewState);
void LCD_Internal_Bias_Voltage_Config (uint32_t Voltage);
void LCD_Common_Port_Config (uint32_t CommonPort);
void LCD_Source_Clock_Divider_Config (uint32_t ClkDivider);
void LCD_Voltage_Divider_Config (uint32_t VoltageSelect);
void LCD_Bias_Voltage_Source_Config (uint32_t VoltageSelect);
void LCD_Pump_Enable (FunctionalState NewState);
void LCD_Source_Clock_Config (uint32_t SourceClock);
void LCD_Wave_Type_Config (uint32_t WaveType);
void LCD_Bias_Mode_Config (uint32_t BiasMode);
void LCD_Prescaler_Config (uint32_t Prescaler);
void LCD_Flicker_On_Time_Config (uint8_t FlickerOn);
void LCD_Flicker_Off_Time_Config (uint8_t FlickerOff);
void LCD_Flicker_Enable (FunctionalState NewState);
void LCD_Save_Power_Enable (FunctionalState NewState);
void LCD_Time_Slice_Config (uint32_t TimeSlice);
void LCD_Work_Time_Config (uint32_t WorkTime);
void LCD_A_Power_Control_Config (uint32_t PowerCtl);
void LCD_B_Power_Control_Config (uint32_t PowerCtl);
void LCD_Power_Mode_Config (uint32_t PowerMode);
void LCD_Seg_Pin_Function_Config (uint64_t SegPin, uint32_t SegFunction);
void LCD_Data_Pixel_Config (uint32_t ComPin, uint64_t SegPin, uint32_t Pixel);
void LCD_Seg_Channel_Select_Config (uint64_t SegSel,
                    FunctionalState NewState);
void LCD_Com_Channel_Select_Config (uint32_t ComPin, FunctionalState NewState);
void LCD_Channel_IO_Config (uint32_t IOConfig);
//void LCD_Seg_Channel_IO_Config (uint32_t SegSel, uint32_t IOConfig);
//void LCD_Com_Channel_IO_Config (uint32_t ComSel, uint32_t IOConfig);
FlagStatus LCD_Get_Work_Status (void);
FlagStatus LCD_Get_Flicker_Status (void);
/* Һ����ʾ(LCD)�жϹ���������********************************************/
void LCD_Clear_DATA_Registers(uint32_t ComPin);
void LCD_Clear_ALLDATA_Registers(void);
void LCD_Updata_All_Registers_Enable (FunctionalState NewState);
void LCD_Insert_Dead_Time_Enable (FunctionalState NewState);
void LCD_Set_INT_Enable (uint32_t InterruptType, FunctionalState NewState);
void LCD_Clear_INT_Flag (uint32_t InterruptType);
FlagStatus LCD_Get_INT_Flag (uint32_t InterruptType);
void LCD_Updata_INT_Enable (FunctionalState NewState);
void LCD_Flicker_On_INT_Enable (FunctionalState NewState);
void LCD_Flicker_Off_INT_Enable (FunctionalState NewState);
void LCD_Frame_INT_Enable (FunctionalState NewState);
FlagStatus LCD_Get_Updata_INT_Flag (void);
FlagStatus LCD_Get_Flicker_On_INT_Flag (void);
FlagStatus LCD_Get_Flicker_Off_INT_Flag (void);
FlagStatus LCD_Get_Frame_INT_Flag (void);
void LCD_Clear_Updata_INT_Flag (void);
void LCD_Clear_Flicker_On_INT_Flag (void);
void LCD_Clear_Flicker_Off_INT_Flag (void);
void LCD_Clear_Frame_INT_Flag (void);
void LCD_VLCDIO_Enable (FunctionalState NewState);
void LCD_IO_Enable (FunctionalState NewState);

#endif //KF32A_Periph_lcd
#endif /* _KF32A_BASIC_LCD_H */


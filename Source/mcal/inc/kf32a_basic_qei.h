/**
  ******************************************************************************
  * �ļ���  kf32a_basic_qei.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ���������������·(QEI)��ع��ܺ�����������غ궨�塣
  *********************************************************************
  */

#ifndef _KF32A_BASIC_QEI_H_
#define _KF32A_BASIC_QEI_H_

#include "KF32A_BASIC.h"



/**
  * ����  QEIģ��������������Ϣ�ṹ��
  */
typedef struct
{
    FunctionalState m_IndexReset;       /* λ�ü�������λʹ�ܣ�
                                          ȡֵΪTRUE��FALSE�� */
    FunctionalState m_DirectionEn;      /* λ�ü���������״̬���ʹ�ܣ�
                                          ȡֵΪTRUE��FALSE�� */
    uint32_t m_WorkClock;               /* QEI����ʱ�ӣ�
                                          ȡֵΪ�ꡰQEI����ʱ�ӡ��е�һ�� */
    FunctionalState m_DigitalFilterEn; /* QEI�����˲������ʹ�ܣ�
                                          ȡֵΪTRUE��FALSE�� */
    uint32_t m_DigitalFilterPrescaler; /* QEI�����˲���ʱ�ӷ�Ƶ��
                                          ȡֵΪ�ꡰQEI�����˲���ʱ�ӷ�Ƶ���е�һ�� */
    FunctionalState m_SwapABEn;        /* A���B�����뽻��ʹ�ܣ�
                                          ȡֵΪTRUE��FALSE�� */
    uint16_t m_Counter;                /* ����������ֵ��
                                          ȡֵ16λ���ݡ� */
    uint16_t m_Period;                 /* ����ֵ��
                                          ȡֵ16λ���ݡ� */
    uint16_t m_Prescaler;       /* ��ʱ��Ԥ��Ƶֵ��
                                   ȡֵ16λ���ݡ� */
}QEI_InitTypeDef;

/**
  * ����  QEI��ʱ��(T7/T8)��ʱ����Ϣ�ṹ��
  */
typedef struct
{
    uint16_t m_Counter;         /* ��ʱ������ֵ��
                                   ȡֵ16λ���ݡ� */
    uint16_t m_Period;          /* ��ʱ������ֵ��
                                   ȡֵ16λ���ݡ� */
    uint16_t m_Prescaler;       /* ��ʱ��Ԥ��Ƶֵ��
                                   ȡֵ16λ���ݡ� */
    uint32_t m_CounterMode;     /* ����������
                                   ȡֵΪ�ꡰQEIλ�ü����������е�һ�� */
    uint32_t m_ClockSource;     /* ��ʱ��ʱ��Դѡ��
                                   ȡֵΪ�ꡰQEI��ʱ��ʱ��Դ���е�һ�� */
}QEI_TimerTypeDef;

/**
  * QEI�ڴ�ָ��
  */
//#define CHECK_QEI_ALL_PERIPH(PERIPH)    (((PERIPH) == QEI0_SFR) \
//                                      || ((PERIPH) == QEI1_SFR))
#define CHECK_QEI_ALL_PERIPH(PERIPH)    (((PERIPH) == ((QEI_SFRmap *)(uint32_t)0x40000500)) \
                                      || ((PERIPH) == ((QEI_SFRmap *)(uint32_t)0x40000080)))

/**
  * QEI�����������ӿ�ģʽ
  */
#define QEI_ENCODER_OFF                 ((uint32_t)0)
#define QEI_GENERAL_PURPOSE_COUNTER     ((uint32_t)1)
#define QEI_X2_INDEX_PULSE_RESET        ((uint32_t)4)
#define QEI_X2_MATCH_RESET              ((uint32_t)5)
#define QEI_X4_INDEX_PULSE_RESET        ((uint32_t)6)
#define QEI_X4_MATCH_RESET              ((uint32_t)7)
#define CHECK_QEI_PORT_MODE(MODE)       (((MODE) == QEI_ENCODER_OFF) \
                                      || ((MODE) == QEI_GENERAL_PURPOSE_COUNTER) \
                                      || ((MODE) == QEI_X2_INDEX_PULSE_RESET) \
                                      || ((MODE) == QEI_X2_MATCH_RESET) \
                                      || ((MODE) == QEI_X4_INDEX_PULSE_RESET) \
                                      || ((MODE) == QEI_X4_MATCH_RESET))

/**
  * QEI��ʱ��ʱ��Դ
  */
#define QEI_TIMER_SOURCE_SCLK           ((uint32_t)0<<QEI_CTL0_TXCS_POS)
#define QEI_TIMER_SOURCE_EXTERNAL_PIN_QEA \
                                        ((uint32_t)1<<QEI_CTL0_TXCS_POS)
#define CHECK_QEI_TIMER_SOURCE(CLK)     (((CLK) == QEI_TIMER_SOURCE_SCLK) \
                                      || ((CLK) == QEI_TIMER_SOURCE_EXTERNAL_PIN_QEA))

/**
  * QEIλ�ü���������
  */
#define QEI_COUNT_DIRECT_REVERSE        ((uint32_t)0<<QEI_CTL0_TXDIR_POS)
#define QEI_COUNT_DIRECT_FORWARD        ((uint32_t)1<<QEI_CTL0_TXDIR_POS)
#define CHECK_QEI_COUNT_DIRECT(DIR)     (((DIR) == QEI_CTL0_TXDIR_POS) \
                                      || ((DIR) == QEI_COUNT_DIRECT_FORWARD))

/**
  * QEI����ʱ��
  */
#define QEI_SOURCE_SCLK                 ((uint32_t)0<<QEI_CTL1_QEICLK0_POS)
#define QEI_SOURCE_HFCLK                ((uint32_t)1<<QEI_CTL1_QEICLK0_POS)
#define QEI_SOURCE_LFCLK                ((uint32_t)2<<QEI_CTL1_QEICLK0_POS)
#define CHECK_QEI_SOURCE(CLK)           (((CLK) == QEI_SOURCE_SCLK) \
                                      || ((CLK) == QEI_SOURCE_HFCLK) \
                                      || ((CLK) == QEI_SOURCE_LFCLK))

/**
  * QEI�����˲���ʱ�ӷ�Ƶ
  */
#define QEI_DIGITAL_FILTER_CLK_DIV_1    ((uint32_t)0<<QEI_CTL1_QEICK0_POS)
#define QEI_DIGITAL_FILTER_CLK_DIV_2    ((uint32_t)1<<QEI_CTL1_QEICK0_POS)
#define QEI_DIGITAL_FILTER_CLK_DIV_4    ((uint32_t)2<<QEI_CTL1_QEICK0_POS)
#define QEI_DIGITAL_FILTER_CLK_DIV_16   ((uint32_t)3<<QEI_CTL1_QEICK0_POS)
#define QEI_DIGITAL_FILTER_CLK_DIV_32   ((uint32_t)4<<QEI_CTL1_QEICK0_POS)
#define QEI_DIGITAL_FILTER_CLK_DIV_64   ((uint32_t)5<<QEI_CTL1_QEICK0_POS)
#define QEI_DIGITAL_FILTER_CLK_DIV_128  ((uint32_t)6<<QEI_CTL1_QEICK0_POS)
#define QEI_DIGITAL_FILTER_CLK_DIV_256  ((uint32_t)7<<QEI_CTL1_QEICK0_POS)
#define CHECK_QEI_DIGITAL_FILTER_CLK(CLK) \
                                        (((CLK) == QEI_DIGITAL_FILTER_CLK_DIV_1) \
                                      || ((CLK) == QEI_DIGITAL_FILTER_CLK_DIV_2) \
                                      || ((CLK) == QEI_DIGITAL_FILTER_CLK_DIV_4) \
                                      || ((CLK) == QEI_DIGITAL_FILTER_CLK_DIV_16) \
                                      || ((CLK) == QEI_DIGITAL_FILTER_CLK_DIV_32) \
                                      || ((CLK) == QEI_DIGITAL_FILTER_CLK_DIV_64) \
                                      || ((CLK) == QEI_DIGITAL_FILTER_CLK_DIV_128) \
                                      || ((CLK) == QEI_DIGITAL_FILTER_CLK_DIV_256))

/**
  * QEI�ж��¼�
  */
#define QEI_INT_COUNT_ERROR             ((uint32_t)1)
#define QEI_INT_PHASE_ERROR             ((uint32_t)2)
#define QEI_INT_TX_OVERFLOW             ((uint32_t)4)
#define CHECK_QEI_INT_ONE_EVENT(EVENT)  (((EVENT) == QEI_INT_COUNT_ERROR) \
                                      || ((EVENT) == QEI_INT_PHASE_ERROR) \
                                      || ((EVENT) == QEI_INT_TX_OVERFLOW))
#define CHECK_QEI_INT_EVENT(TYPE)       (((TYPE) & (~(QEI_INT_COUNT_ERROR \
                                                    | QEI_INT_PHASE_ERROR \
                                                    | QEI_INT_TX_OVERFLOW))) == 0)


#ifdef KF32A_Periph_qei
/* �������������·(QEI)��ʼ����������**************************************/
void QEI_Reset (QEI_SFRmap* QEIx);
void QEI_Configuration(QEI_SFRmap* QEIx, QEI_InitTypeDef* qeiInitStruct);
void QEI_Timer_Configuration(QEI_SFRmap* QEIx,
                    QEI_TimerTypeDef* qeiTimerInitStruct);
void QEI_Struct_Init (QEI_InitTypeDef* qeiInitStruct);
void QEI_Timer_Struct_Init (QEI_TimerTypeDef* qeiTimerInitStruct);
/* �������������·(QEI)�������ú�������************************************/
void QEI_Encoder_Mode_Config (QEI_SFRmap* QEIx, uint32_t PortMode);
void QEI_Location_Counter_Reset_Enable (QEI_SFRmap* QEIx,
                    FunctionalState NewState);
void QEI_Timer_Source_Clock_Config (QEI_SFRmap* QEIx, uint32_t TimerClock);
FlagStatus QEI_Get_Index_Pin_State (QEI_SFRmap* QEIx);
void QEI_Location_Counter_Output_Enable (QEI_SFRmap* QEIx,
                    FunctionalState NewState);
void QEI_Location_Counter_Direction_Config (QEI_SFRmap* QEIx,
                    uint32_t Direction);
FlagStatus QEI_Get_Location_Counter_Direction (QEI_SFRmap* QEIx);
void QEI_Source_Clock_Config (QEI_SFRmap* QEIx, uint32_t PeripheralClock);
void QEI_Digital_Filter_Output_Enable (QEI_SFRmap* QEIx,
                    FunctionalState NewState);
void QEI_Digital_Filter_Clock_Config (QEI_SFRmap* QEIx,
                    uint32_t PeripheralClock);
void QEI_Swap_Phase_AB_Enable (QEI_SFRmap* QEIx, FunctionalState NewState);
void QEI_Timer_Counter_Config (QEI_SFRmap* QEIx, uint16_t Counter);
uint16_t QEI_Get_Timer_Counter (QEI_SFRmap* QEIx);
void QEI_Timer_Period_Config (QEI_SFRmap* QEIx, uint16_t Period);
uint16_t QEI_Get_Timer_Period (QEI_SFRmap* QEIx);
void QEI_Timer_Prescaler_Config (QEI_SFRmap* QEIx, uint16_t Prescaler);
/* �������������·(QEI)�жϹ���������************************************/
FlagStatus QEI_Get_INT_Flag (QEI_SFRmap* QEIx, uint32_t InterruptType);
void QEI_Clear_INT_Flag (QEI_SFRmap* QEIx, uint32_t InterruptType);
void QEI_Set_INT_Enable (QEI_SFRmap* QEIx,
                    uint32_t InterruptType, FunctionalState NewState);
FlagStatus QEI_Get_Counter_Overflow_INT_Flag (QEI_SFRmap* QEIx);
FlagStatus QEI_Get_Phase_Error_INT_Flag (QEI_SFRmap* QEIx);
FlagStatus QEI_Get_Count_Error_INT_Flag (QEI_SFRmap* QEIx);
void QEI_Clear_Timer_Overflow_INT_Flag (QEI_SFRmap* QEIx);
void QEI_Clear_Phase_Error_INT_Flag (QEI_SFRmap* QEIx);
void QEI_Clear_Counter_Error_INT_Flag (QEI_SFRmap* QEIx);
void QEI_Timer_Overflow_INT_Enable (QEI_SFRmap* QEIx,FunctionalState NewState);
void QEI_Phase_Error_INT_Enable (QEI_SFRmap* QEIx, FunctionalState NewState);
void QEI_Counter_Error_INT_Enable (QEI_SFRmap* QEIx, FunctionalState NewState);
#endif //KF32A_Periph_qei
#endif /* _KF32A_BASIC_QEI_H */

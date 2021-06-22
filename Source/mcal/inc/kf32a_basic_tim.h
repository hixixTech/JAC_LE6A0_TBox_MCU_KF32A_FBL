/**
  *********************************************************************
  * �ļ���  kf32a_basic_tim.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˶�ʱ������Ŀ⺯����������غ궨�塣
  *********************************************************************
  */

#ifndef _KF32A_BASIC_TIM_H_
#define _KF32A_BASIC_TIM_H_

#include "KF32A_BASIC.h"

/*** ��ʱ��ͨ�ö���  ***/
/* ��ʱ����Ŷ��� */
#define TIM0    (0)
#define TIM1    (1)
#define TIM2    (2)
#define TIM3    (3)
#define TIM4    (4)
#define TIM5    (5)
#define TIM6    (6)
#define TIM9    (9)
#define TIM10   (10)
#define TIM14   (14)
#define TIM15   (15)
#define TIM18   (18)
#define TIM19   (19)
#define TIM20   (20)
#define TIM21   (21)
#define TIM22   (22)
#define TIM23   (23)
#define CHECK_TIMX_ALL_NUM(NUM)         (((NUM) == TIM0) \
                                      || ((NUM) == TIM1) \
                                      || ((NUM) == TIM2) \
                                      || ((NUM) == TIM3) \
                                      || ((NUM) == TIM4) \
                                      || ((NUM) == TIM5) \
                                      || ((NUM) == TIM6) \
                                      || ((NUM) == TIM9) \
                                      || ((NUM) == TIM10) \
                                      || ((NUM) == TIM14) \
                                      || ((NUM) == TIM15) \
                                      || ((NUM) == TIM18) \
                                      || ((NUM) == TIM19) \
                                      || ((NUM) == TIM20) \
                                      || ((NUM) == TIM21) \
                                      || ((NUM) == TIM22) \
                                      || ((NUM) == TIM23))
#define CHECK_BTIM_NUM(NUM)             (((NUM) == TIM14) \
                                      || ((NUM) == TIM15))
#define CHECK_GPTIM_NUM(NUM)            (((NUM) == TIM0) \
                                      || ((NUM) == TIM1) \
                                      || ((NUM) == TIM2) \
                                      || ((NUM) == TIM3) \
                                      || ((NUM) == TIM4) \
                                      || ((NUM) == TIM18) \
                                      || ((NUM) == TIM19) \
                                      || ((NUM) == TIM20) \
                                      || ((NUM) == TIM21) \
                                      || ((NUM) == TIM22) \
                                      || ((NUM) == TIM23))
#define CHECK_ATIM_NUM(NUM)             (((NUM) == TIM5) \
                                      || ((NUM) == TIM6) \
                                      || ((NUM) == TIM9) \
                                      || ((NUM) == TIM10))

/* ��ʱ��ָ�붨�� */
#define T0_TYPE                         (GPTIM_SFRmap *)
#define T1_TYPE                         (GPTIM_SFRmap *)
#define T2_TYPE                         (GPTIM_SFRmap *)
#define T3_TYPE                         (GPTIM_SFRmap *)
#define T4_TYPE                         (GPTIM_SFRmap *)
#define T5_TYPE                         (ATIM_SFRmap *)
#define T6_TYPE                         (ATIM_SFRmap *)
#define T9_TYPE                         (ATIM_SFRmap *)
#define T10_TYPE                        (ATIM_SFRmap *)
#define T14_TYPE                        (BTIM_SFRmap *)
#define T15_TYPE                        (BTIM_SFRmap *)
#define T18_TYPE                        (GPTIM_SFRmap *)
#define T19_TYPE                        (GPTIM_SFRmap *)
#define T20_TYPE                        (GPTIM_SFRmap *)
#define T21_TYPE                        (GPTIM_SFRmap *)
#define T22_TYPE                        (GPTIM_SFRmap *)
#define T23_TYPE                        (GPTIM_SFRmap *)
//#define CHECK_TIM_ALL_PERIPH(ADDR)      (((ADDR) == T0_ADDR) \
//                                      || ((ADDR) == T1_ADDR) \
//                                      || ((ADDR) == T2_ADDR) \
//                                      || ((ADDR) == T3_ADDR) \
//                                      || ((ADDR) == T4_ADDR) \
//                                      || ((ADDR) == T5_ADDR) \
//                                      || ((ADDR) == T6_ADDR) \
//                                      || ((ADDR) == T9_ADDR) \
//                                      || ((ADDR) == T10_ADDR) \
//                                      || ((ADDR) == T14_ADDR) \
//                                      || ((ADDR) == T15_ADDR) \
//                                      || ((ADDR) == T18_ADDR) \
//                                      || ((ADDR) == T19_ADDR) \
//                                      || ((ADDR) == T20_ADDR) \
//                                      || ((ADDR) == T21_ADDR) \
//                                      || ((ADDR) == T22_ADDR) \
//                                      || ((ADDR) == T23_ADDR))
#define CHECK_TIM_ALL_PERIPH(ADDR)      (((ADDR) == ((uint32_t)0x40000880)) \
                                      || ((ADDR) == ((uint32_t)0x40000100)) \
                                      || ((ADDR) == ((uint32_t)0x40000180)) \
                                      || ((ADDR) == ((uint32_t)0x40000200)) \
                                      || ((ADDR) == ((uint32_t)0x40000280)) \
                                      || ((ADDR) == ((uint32_t)0x40000300)) \
                                      || ((ADDR) == ((uint32_t)0x40000400)) \
                                      || ((ADDR) == ((uint32_t)0x40001800)) \
                                      || ((ADDR) == ((uint32_t)0x40001880)) \
                                      || ((ADDR) == ((uint32_t)0x40001A80)) \
                                      || ((ADDR) == ((uint32_t)0x40001B00)) \
                                      || ((ADDR) == ((uint32_t)0x40001B80)) \
                                      || ((ADDR) == ((uint32_t)0x40001C00)) \
                                      || ((ADDR) == ((uint32_t)0x40001C80)) \
                                      || ((ADDR) == ((uint32_t)0x40001D00)))
//#define CHECK_BTIM_PERIPH(PERIPH)       (((PERIPH) == T14_SFR) \
//                                      || ((PERIPH) == T15_SFR))
#define CHECK_BTIM_PERIPH(PERIPH)       (((PERIPH) == ((BTIM_SFRmap *)(uint32_t)0x40001800)) \
                                      || ((PERIPH) == ((BTIM_SFRmap *)(uint32_t)0x40001880)))
//#define CHECK_GPTIM_PERIPH(PERIPH)      (((PERIPH) == T0_SFR) \
//                                      || ((PERIPH) == T1_SFR) \
//                                      || ((PERIPH) == T2_SFR) \
//                                      || ((PERIPH) == T3_SFR) \
//                                      || ((PERIPH) == T4_SFR) \
//                                      || ((PERIPH) == T18_SFR) \
//                                      || ((PERIPH) == T19_SFR) \
//                                      || ((PERIPH) == T20_SFR) \
//                                      || ((PERIPH) == T21_SFR) \
//                                      || ((PERIPH) == T22_SFR) \
//                                      || ((PERIPH) == T23_SFR))
#define CHECK_GPTIM_PERIPH(PERIPH)      (((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000880)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000100)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000180)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000200)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000280)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001A80)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001B00)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001B80)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001C00)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001C80)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001D00)))
//#define CHECK_ATIM_PERIPH(PERIPH)       (((PERIPH) == T5_SFR) \
//                                      || ((PERIPH) == T6_SFR) \
//                                      || ((PERIPH) == T9_SFR) \
//                                      || ((PERIPH) == T10_SFR))
#define CHECK_ATIM_PERIPH(PERIPH)       (((PERIPH) == ((ATIM_SFRmap *)(uint32_t)0x40000300)) \
                                      || ((PERIPH) == ((ATIM_SFRmap *)(uint32_t)0x40000400)))

/**
  * ����״̬
  */
#define PIN_INACTIVE                    ((uint32_t)0x0)
#define PIN_ACTIVE                      ((uint32_t)0x1)
#define PIN_TRISTATE                    ((uint32_t)0x2)
#define CHECK_PIN_STATE(STATE)          (((STATE) == PIN_INACTIVE) \
                                      || ((STATE) == PIN_ACTIVE) \
                                      || ((STATE) == PIN_TRISTATE))
/*** ��ʱ��ͨ�ö������  ***/


/*** ������ʱ��(BTIM)��ض���  */
/**
  * ����  ������ʱ��(BTIM)������Ϣ�ṹ��
  */
typedef struct
{
    uint16_t m_Counter;         /* ��ʱ������ֵ��ȡֵ16λ���ݡ� */
    uint16_t m_Period;          /* ��ʱ������ֵ��ȡֵ16λ���ݡ� */
    uint16_t m_Prescaler;       /* ��ʱ��Ԥ��Ƶֵ��ȡֵ16λ���ݡ� */
    uint16_t m_CounterMode;     /* ��ʱ������ģʽ��
                                   ȡֵΪ�ꡰBTIM��ʱ������ģʽ���е�һ�� */
    uint16_t m_Clock;           /* ��ʱ������ʱ�ӣ�
                                   ȡֵΪ�ꡰBTIM��ʱ������ʱ�ӡ��е�һ�� */
    uint16_t m_WorkMode;        /* ��ʱ/����ģʽѡ��
                                   ȡֵΪ�ꡰBTIM��ʱ/����ģʽѡ���е�һ�� */
    uint16_t m_MasterMode;      /* ��ģʽѡ��
                                   ȡֵΪ�ꡰBTIM��ģʽѡ���е�һ�� */
    uint16_t m_SlaveMode;       /* ��ģʽѡ��
                                   ȡֵΪ�ꡰBTIM��ģʽѡ���е�һ�� */
    uint16_t m_EXPulseSync;     /* Tx����ģʽ�ⲿ������������ͬ�����ƣ�
                                   ȡֵΪ�ꡰBTIM����ģʽ�ⲿ������������ͬ�����ơ��е�һ�� */
    uint16_t m_MasterSlaveSync; /* ����ģʽͬ�����ƣ�
                                   ȡֵΪTRUE��FALSE */
} BTIM_InitTypeDef; /* T14 / T15 */


/**
  * BTIM��ʱ������ģʽ
  */
#define BTIM_COUNT_DOWN_UF              ((uint32_t)0 << BTIM_CTL1_TXCMS0_POS)
#define BTIM_COUNT_UP_OF                ((uint32_t)4 << BTIM_CTL1_TXCMS0_POS)
#define BTIM_COUNT_UP_DOWN_OF           ((uint32_t)1 << BTIM_CTL1_TXCMS0_POS)
#define BTIM_COUNT_UP_DOWN_UF           ((uint32_t)2 << BTIM_CTL1_TXCMS0_POS)
#define BTIM_COUNT_UP_DOWN_OUF          ((uint32_t)3 << BTIM_CTL1_TXCMS0_POS)
#define CHECK_BTIM_COUNTER_MODE(MODE)   ((((MODE) & 0x3F) == 0) && \
                                         (((MODE) >> 9) == 0))

/**
  * BTIM��ʱ������ʱ��
  */
#define BTIM_SCLK                       ((uint32_t)0 << BTIM_CTL1_TXCLK0_POS)
#define BTIM_HFCLK                      ((uint32_t)1 << BTIM_CTL1_TXCLK0_POS)
#define BTIM_LFCLK                      ((uint32_t)2 << BTIM_CTL1_TXCLK0_POS)
#define CHECK_BTIM_CLOCK_CONFIG(CLK)    ((((CLK) & 0x7) == 0) && \
                                         (((CLK) >> 5) == 0))

/**
  * BTIM��ʱ/����ģʽѡ��
  */
#define BTIM_TIMER_MODE                 ((uint32_t)0 << BTIM_CTL1_TXCS_POS)
#define BTIM_COUNTER_MODE               ((uint32_t)1 << BTIM_CTL1_TXCS_POS)
#define CHECK_BTIM_WORK_MODE(MODE)      (((MODE) == BTIM_TIMER_MODE) \
                                      || ((MODE) == BTIM_COUNTER_MODE))

/**
  * BTIM��ģʽѡ��
  */
#define BTIM_MASTER_UR_SIGNAL           ((uint32_t)0 << BTIM_CTL2_TXMMS0_POS)
#define BTIM_MASTER_EN_SIGNAL           ((uint32_t)1 << BTIM_CTL2_TXMMS0_POS)
#define BTIM_MASTER_IF_SIGNAL           ((uint32_t)2 << BTIM_CTL2_TXMMS0_POS)
#define CHECK_BTIM_MASTER_MODE(MODE)    (((MODE) == BTIM_MASTER_UR_SIGNAL) \
                                      || ((MODE) == BTIM_MASTER_EN_SIGNAL) \
                                      || ((MODE) == BTIM_MASTER_IF_SIGNAL))

/**
  * BTIM��ģʽѡ��
  */
#define BTIM_SLAVE_FORBIDDEN_MODE       ((uint32_t)0 << BTIM_CTL2_TXSMS0_POS)
#define BTIM_SLAVE_TRIGGER_MODE         ((uint32_t)4 << BTIM_CTL2_TXSMS0_POS)
#define BTIM_SLAVE_GATED_MODE           ((uint32_t)5 << BTIM_CTL2_TXSMS0_POS)
#define BTIM_SLAVE_RESET_MODE           ((uint32_t)6 << BTIM_CTL2_TXSMS0_POS)
#define BTIM_SLAVE_COUNTER_MODE         ((uint32_t)7 << BTIM_CTL2_TXSMS0_POS)
#define CHECK_BTIM_SLAVE_MODE(MODE)     (((MODE) == BTIM_SLAVE_FORBIDDEN_MODE) \
                                      || ((MODE) == BTIM_SLAVE_TRIGGER_MODE) \
                                      || ((MODE) == BTIM_SLAVE_GATED_MODE) \
                                      || ((MODE) == BTIM_SLAVE_RESET_MODE) \
                                      || ((MODE) == BTIM_SLAVE_COUNTER_MODE))

/**
  * BTIM����ģʽ�ⲿ������������ͬ������
  */
#define BTIM_EX_SYNC_MODE               ((uint32_t)0 << BTIM_CTL1_TXSY_POS)
#define BTIM_NO_SYNC_MODE               ((uint32_t)1 << BTIM_CTL1_TXSY_POS)
#define CHECK_BTIM_SYNC_MODE(MODE)      (((MODE) == BTIM_EX_SYNC_MODE) \
                                      || ((MODE) == BTIM_NO_SYNC_MODE))

/**
  * BTIM����ѡ��
  */
#define BTIM_TRIGGER_T1                 ((uint32_t)0 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T2                 ((uint32_t)1 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T3                 ((uint32_t)2 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T4                 ((uint32_t)3 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T5                 ((uint32_t)4 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T9                 ((uint32_t)5 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T14                ((uint32_t)6 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T15                ((uint32_t)7 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T18                ((uint32_t)8 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T19                ((uint32_t)9 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T20                ((uint32_t)0xA << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T21                ((uint32_t)0xB << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_TXCK               ((uint32_t)0xC << BTIM_CTL2_TXTS0_POS)
#define CHECK_BTIM_TRIGGER_MODE(MODE)   (((MODE) == BTIM_TRIGGER_T1) \
                                      || ((MODE) == BTIM_TRIGGER_T2) \
                                      || ((MODE) == BTIM_TRIGGER_T3) \
                                      || ((MODE) == BTIM_TRIGGER_T4) \
                                      || ((MODE) == BTIM_TRIGGER_T5) \
                                      || ((MODE) == BTIM_TRIGGER_T9) \
                                      || ((MODE) == BTIM_TRIGGER_T14) \
                                      || ((MODE) == BTIM_TRIGGER_T15) \
                                      || ((MODE) == BTIM_TRIGGER_T18) \
                                      || ((MODE) == BTIM_TRIGGER_T19) \
                                      || ((MODE) == BTIM_TRIGGER_T20) \
                                      || ((MODE) == BTIM_TRIGGER_T21) \
                                      || ((MODE) == BTIM_TRIGGER_TXCK))

/**
  * BTIM���������־
  */
typedef enum
{
    DIR_DOWN = 0,
    DIR_UP = !DIR_DOWN
} DIRStatus;
#define CHECK_TIMER_DIR(state)          (((state) == DIR_DOWN) \
                                      || ((state) == DIR_UP))


/* ��ʱ�����踴λ�������� ************************************************/
void TIM_Reset (void* TIMx);

/* ������ʱ��(BTIM)��ʼ�������ú������� **********************************/
void BTIM_Configuration (BTIM_SFRmap* BTIMx,
                    BTIM_InitTypeDef* btimInitStruct);
void BTIM_Struct_Init (BTIM_InitTypeDef* btimInitStruct);
void BTIM_Cmd (BTIM_SFRmap* BTIMx, FunctionalState NewState);
void BTIM_Set_Counter (BTIM_SFRmap* BTIMx, uint16_t Counter);
void BTIM_Set_Period (BTIM_SFRmap* BTIMx, uint16_t Period);
void BTIM_Set_Prescaler (BTIM_SFRmap* BTIMx, uint16_t Prescaler);
void BTIM_Counter_Mode_Config (BTIM_SFRmap* BTIMx, uint32_t CounterMode);
void BTIM_Clock_Config (BTIM_SFRmap* BTIMx, uint32_t NewClock);
void BTIM_External_Pulse_Sync_Config (BTIM_SFRmap* BTIMx, uint32_t PulseSync);
void BTIM_Work_Mode_Config (BTIM_SFRmap* BTIMx, uint32_t NewState);
void BTIM_Generate_Trigger_Config (BTIM_SFRmap* BTIMx, FunctionalState NewState);
void BTIM_Single_Pulse_Enable (BTIM_SFRmap* BTIMx, FunctionalState NewState);
void BTIM_Single_Pulse_Shut_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Updata_Immediately_Config (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Master_Slave_Snyc_Config (BTIM_SFRmap* BTIMx, FunctionalState NewState);
void BTIM_Trigger_Select_Config (BTIM_SFRmap* BTIMx,
                    uint32_t TriggerSelect);
void BTIM_Slave_Mode_Config (BTIM_SFRmap* BTIMx,
                    uint32_t SlaveMode);
void BTIM_Master_Mode_Config (BTIM_SFRmap* BTIMx,
                    uint32_t MasterMode);
void BTIM_Updata_Rising_Edge_Config (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Updata_Enable (BTIM_SFRmap* BTIMx, FunctionalState NewState);
DIRStatus BTIM_Get_Direction (BTIM_SFRmap* BTIMx);
uint16_t BTIM_Get_Counter (BTIM_SFRmap* BTIMx);
uint16_t BTIM_Get_Period (BTIM_SFRmap* BTIMx);
uint16_t BTIM_Get_Prescaler (BTIM_SFRmap* BTIMx);

/* ������ʱ��(BTIM)�жϼ���־������ ************************************/
void BTIM_Trigger_DMA_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Updata_DMA_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Overflow_INT_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Trigger_INT_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Updata_INT_Enable (BTIM_SFRmap* BTIMx, FunctionalState NewState);
INTStatus BTIM_Get_Trigger_DMA_INT_Status (BTIM_SFRmap* BTIMx);
INTStatus BTIM_Get_Updata_DMA_INT_Status (BTIM_SFRmap* BTIMx);
INTStatus BTIM_Get_Overflow_INT_Status (BTIM_SFRmap* BTIMx);
INTStatus BTIM_Get_Trigger_INT_Status (BTIM_SFRmap* BTIMx);
INTStatus BTIM_Get_Updata_INT_Status (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Trigger_DMA_INT_Flag (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Updata_DMA_INT_Flag (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Overflow_INT_Flag (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Trigger_INT_Flag (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Updata_INT_Flag (BTIM_SFRmap* BTIMx);
void BTIM_Clear_Overflow_INT_Flag (BTIM_SFRmap* BTIMx);
void BTIM_Clear_Trigger_INT_Flag (BTIM_SFRmap* BTIMx);
void BTIM_Clear_Updata_INT_Flag (BTIM_SFRmap* BTIMx);

/*** ������ʱ��(BTIM)��ض������  */


/*** ͨ�ö�ʱ��(GPTIM)��CCP��ض���  */
/**
  * ����  ͨ�ö�ʱ��(GPTIM)������Ϣ�ṹ��
  */
typedef struct
{
    uint32_t m_Counter;         /* ��ʱ������ֵ��ȡֵ32λ���ݡ� */
    uint32_t m_Period;          /* ��ʱ������ֵ��ȡֵ32λ���ݡ� */
    uint32_t m_Prescaler;       /* ��ʱ��Ԥ��Ƶֵ��ȡֵ32λ���ݡ� */
    uint16_t m_CounterMode;     /* ��ʱ������ģʽ��
                                   ȡֵΪ�ꡰGPTIM��ʱ������ģʽ���е�һ�� */
    uint16_t m_Clock;           /* ��ʱ������ʱ�ӣ�
                                   ȡֵΪ�ꡰGPTIM��ʱ������ʱ�ӡ��е�һ�� */
    uint16_t m_WorkMode;        /* ��ʱ/����ģʽѡ��
                                   ȡֵΪ�ꡰGPTIM��ʱ/����ģʽѡ���е�һ�� */
    uint16_t m_MasterMode;      /* ��ģʽѡ��
                                   ȡֵΪ�ꡰGPTIM��ģʽѡ���е�һ�� */
    uint16_t m_SlaveMode;       /* ��ģʽѡ��
                                   ȡֵΪ�ꡰGPTIM��ģʽѡ���е�һ�� */
    uint16_t m_EXPulseSync;     /* Tx����ģʽ�ⲿ������������ͬ�����ƣ�
                                   ȡֵΪ�ꡰGPTIM����ģʽ�ⲿ������������ͬ�����ơ��е�һ�� */
    uint16_t m_MasterSlaveSync; /* ����ģʽͬ�����ƣ�
                                   ȡֵΪTRUE��FALSE */
} GPTIM_InitTypeDef; /* T0/T1/T2/T3/T4/T18/T19/T20/T21 */

/**
  * ����  CCP�Ƚ�ģʽ������Ϣ�ṹ��
  */
typedef struct
{
    uint16_t m_Channel;         /* ͨ����ţ�
                                   ȡֵ��Χ���㡰CCPͨ�����ĺꡣ */
    uint16_t m_CompareMode;     /* ��׽/�Ƚ���ģʽѡ��
                                   ȡֵ��Χ�����CHECK_CCP_CMP_MODE��Լ�������� */
    uint32_t m_CompareValue;    /* ��TX�Ƚϵ�ֵ��
                                   ȡֵ��ΧΪ32λ���ݡ� */
} CCP_CompareInitTypeDef;

/**
  * ����  CCP��׽ģʽ������Ϣ�ṹ��
  */
typedef struct
{
    uint16_t m_Channel;         /* ͨ����ţ�
                                   ȡֵ��Χ���㡰CCPͨ�����ĺꡣ */
    uint16_t m_CaptureMode;     /* ��׽/�Ƚ���ģʽѡ��
                                   ȡֵ��Χ�����CHECK_CCP_CAP_MODE��Լ�������� */
    FunctionalState m_PWMInput;        /* PWM����ģʽ��
                                   ȡֵΪTRUE��FALSE�� */
    FunctionalState m_XORMode;         /* �������ģʽ��
                                   ȡֵΪTRUE��FALSE�� */
    FunctionalState m_ChannelCompare4; /* �Ƚ��������������׽��ֻ��ͨ��4�������ã�
                                   ȡֵΪTRUE��FALSE�� */
} CCP_CaptureInitTypeDef;

/**
  * ����  CCP PWMģʽ������Ϣ�ṹ��
  */
typedef struct
{
    uint16_t m_Channel;         /* ͨ����ţ�
                                   ȡֵ��Χ���㡰CCPͨ�����ĺꡣ */
    uint16_t m_PwmMode;         /* ��׽/�Ƚ���ģʽѡ��
                                   ȡֵ��Χ�����CHECK_CCP_PWM_MODE��Լ�������� */
    uint32_t m_DutyRatio;       /* ռ�ձȣ�
                                   ȡֵ��ΧΪ32λ���ݡ� */
    uint32_t m_OutputCtl;       /* ͨ��������ƣ�
                                   ȡֵΪ��CCPͨ��������ơ��е�һ���� */
    uint32_t m_SinglePWM;       /* ���������ģʽ��
                                   ȡֵΪTRUE��FALSE�� */
    uint32_t m_CloseTimer;      /* ���������ģʽѡ��
                                   ȡֵΪTRUE��FALSE�� */
} CCP_PWMInitTypeDef;

/**
  * GPTIM��ʱ������ģʽ
  */
#define GPTIM_COUNT_DOWN_UF             ((uint32_t)0x0 << GPTIM_CTL1_TXCMS0_POS)
#define GPTIM_COUNT_UP_OF               ((uint32_t)0x4 << GPTIM_CTL1_TXCMS0_POS)
#define GPTIM_COUNT_UP_DOWN_OF          ((uint32_t)0x1 << GPTIM_CTL1_TXCMS0_POS)
#define GPTIM_COUNT_UP_DOWN_UF          ((uint32_t)0x2 << GPTIM_CTL1_TXCMS0_POS)
#define GPTIM_COUNT_UP_DOWN_OUF         ((uint32_t)0x3 << GPTIM_CTL1_TXCMS0_POS)
#define CHECK_GPTIM_COUNTER_MODE(MODE)  (((MODE) == GPTIM_COUNT_DOWN_UF) \
                                      || ((MODE) == GPTIM_COUNT_UP_OF) \
                                      || ((MODE) == GPTIM_COUNT_UP_DOWN_OF) \
                                      || ((MODE) == GPTIM_COUNT_UP_DOWN_UF) \
                                      || ((MODE) == GPTIM_COUNT_UP_DOWN_OUF))

/**
  * GPTIM��ʱ������ʱ��
  */
#define GPTIM_SCLK                      ((uint32_t)0x0 << GPTIM_CTL1_TXCLK0_POS)
#define GPTIM_HFCLK                     ((uint32_t)0x1 << GPTIM_CTL1_TXCLK0_POS)
#define GPTIM_LFCLK                     ((uint32_t)0x2 << GPTIM_CTL1_TXCLK0_POS)
#define CHECK_GPTIM_CLOCK_CONFIG(CLK)   (((CLK) == GPTIM_SCLK) \
                                      || ((CLK) == GPTIM_HFCLK) \
                                      || ((CLK) == GPTIM_LFCLK))

/**
  * GPTIM��ʱ/����ģʽѡ��
  */
#define GPTIM_TIMER_MODE                ((uint32_t)0 << GPTIM_CTL1_TXCS_POS)
#define GPTIM_COUNTER_MODE              ((uint32_t)1 << GPTIM_CTL1_TXCS_POS)
#define CHECK_GPTIM_WORK_MODE(MODE)     (((MODE) == GPTIM_TIMER_MODE) \
                                      || ((MODE) == GPTIM_COUNTER_MODE))

/**
  * GPTIM��ģʽѡ��
  */
#define GPTIM_MASTER_UR_SIGNAL          ((uint32_t)0 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_TXEN_SIGNAL        ((uint32_t)1 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_TXIF_SIGNAL        ((uint32_t)2 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH1IF_SIGNAL   ((uint32_t)3 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH1_SIGNAL     ((uint32_t)4 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH2_SIGNAL     ((uint32_t)5 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH3_SIGNAL     ((uint32_t)6 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH4_SIGNAL     ((uint32_t)7 << GPTIM_CTL2_TXMMS0_POS)
#define CHECK_GPTIM_MASTER_MODE(MODE)   (((MODE) == GPTIM_MASTER_UR_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_TXEN_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_TXIF_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH1IF_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH1_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH2_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH3_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH4_SIGNAL))

/**
  * GPTIM��ģʽѡ��
  */
#define GPTIM_SLAVE_FORBIDDEN_MODE      ((uint32_t)0 << GPTIM_CTL2_TXSMS0_POS)
#define GPTIM_SLAVE_TRIGGER_MODE        ((uint32_t)4 << GPTIM_CTL2_TXSMS0_POS)
#define GPTIM_SLAVE_GATED_MODE          ((uint32_t)5 << GPTIM_CTL2_TXSMS0_POS)
#define GPTIM_SLAVE_RESET_MODE          ((uint32_t)6 << GPTIM_CTL2_TXSMS0_POS)
#define GPTIM_SLAVE_COUNTER_MODE        ((uint32_t)7 << GPTIM_CTL2_TXSMS0_POS)
#define CHECK_GPTIM_SLAVE_MODE(MODE)    (((MODE) == GPTIM_SLAVE_FORBIDDEN_MODE) \
                                      || ((MODE) == GPTIM_SLAVE_TRIGGER_MODE) \
                                      || ((MODE) == GPTIM_SLAVE_GATED_MODE) \
                                      || ((MODE) == GPTIM_SLAVE_RESET_MODE) \
                                      || ((MODE) == GPTIM_SLAVE_COUNTER_MODE))

/**
  * GPTIM����ģʽ�ⲿ������������ͬ������
  */
#define GPTIM_EX_SYNC_MODE              ((uint32_t)0x0 << GPTIM_CTL1_TXSY_POS)
#define GPTIM_NO_SYNC_MODE              ((uint32_t)0x1 << GPTIM_CTL1_TXSY_POS)
#define CHECK_GPTIM_SYNC_MODE(MODE)     (((MODE) == GPTIM_EX_SYNC_MODE) \
                                      || ((MODE) == GPTIM_NO_SYNC_MODE))

/**
  * GPTIM����ѡ��
  */
#define GPTIM_TRIGGER_T1                ((uint32_t)0x0 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T2                ((uint32_t)0x1 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T3                ((uint32_t)0x2 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T4                ((uint32_t)0x3 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T5                ((uint32_t)0x4 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T9                ((uint32_t)0x5 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T14               ((uint32_t)0x6 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T15               ((uint32_t)0x7 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T18               ((uint32_t)0x8 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T19               ((uint32_t)0x9 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T20               ((uint32_t)0xA << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T21               ((uint32_t)0xB << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_TXCK              ((uint32_t)0xC << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_CCPXCH1           ((uint32_t)0xD << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_CCPXCH2           ((uint32_t)0xE << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_CCPXCH3           ((uint32_t)0xF << GPTIM_CTL2_TXTS0_POS)
#define CHECK_GPTIM_TRIGGER_MODE(MODE)  (((uint32_t)(MODE) & (~GPTIM_CTL2_TXTS)) == 0)

/**
  * CCPͨ��
  */
#define CCP_CHANNEL_1                   ((uint16_t)0)
#define CCP_CHANNEL_2                   ((uint16_t)1)
#define CCP_CHANNEL_3                   ((uint16_t)2)
#define CCP_CHANNEL_4                   ((uint16_t)3)
#define CHECK_CCP_CHANNEL(CHANNEL)      (((CHANNEL) >> 2) == 0)

/**
  * CCPͨ��ģʽѡ��
  */
#define CCP_MODE_RST                    ((uint16_t)0x0)
#define CCP_CMP_TOGGLE_LEVEL            ((uint16_t)0x2)
#define CCP_CAP_FALLING_EDGE            ((uint16_t)0x4)
#define CCP_CAP_RISING_EDGE             ((uint16_t)0x5)
#define CCP_CAP_4TH_RISING_EDGE         ((uint16_t)0x6)
#define CCP_CAP_16TH_RISING_EDGE        ((uint16_t)0x7)
#define CCP_CMP_ACTIVE_LEVEL            ((uint16_t)0x8)
#define CCP_CMP_INACTIVE_LEVEL          ((uint16_t)0x9)
#define CCP_CMP_SOFT_INT                ((uint16_t)0xA)
#define CCP_CMP_SPECIAL_EVENT           ((uint16_t)0xB)
#define CCP_PWM_MODE                    ((uint16_t)0xC)
#define CHECK_CCP_MODE(MODE)            (((MODE) == CCP_MODE_RST) \
                                      || ((MODE) == CCP_CMP_TOGGLE_LEVEL) \
                                      || ((MODE) == CCP_CAP_RISING_EDGE) \
                                      || ((MODE) == CCP_CAP_FALLING_EDGE) \
                                      || ((MODE) == CCP_CAP_4TH_RISING_EDGE) \
                                      || ((MODE) == CCP_CAP_16TH_RISING_EDGE) \
                                      || ((MODE) == CCP_CMP_ACTIVE_LEVEL) \
                                      || ((MODE) == CCP_CMP_INACTIVE_LEVEL) \
                                      || ((MODE) == CCP_CMP_SOFT_INT) \
                                      || ((MODE) == CCP_CMP_SPECIAL_EVENT) \
                                      || ((MODE) == CCP_PWM_MODE))
#define CHECK_CCP_CAP_MODE(MODE)        (((MODE) == CCP_MODE_RST) \
                                      || ((MODE) == CCP_CAP_RISING_EDGE) \
                                      || ((MODE) == CCP_CAP_FALLING_EDGE) \
                                      || ((MODE) == CCP_CAP_4TH_RISING_EDGE) \
                                      || ((MODE) == CCP_CAP_16TH_RISING_EDGE))
#define CHECK_CCP_CMP_MODE(MODE)        (((MODE) == CCP_MODE_RST) \
                                      || ((MODE) == CCP_CMP_TOGGLE_LEVEL) \
                                      || ((MODE) == CCP_CMP_ACTIVE_LEVEL) \
                                      || ((MODE) == CCP_CMP_INACTIVE_LEVEL) \
                                      || ((MODE) == CCP_CMP_SOFT_INT) \
                                      || ((MODE) == CCP_CMP_SPECIAL_EVENT))
#define CHECK_CCP_PWM_MODE(MODE)        (((MODE) == CCP_MODE_RST) \
                                      || ((MODE) == CCP_PWM_MODE))

/**
  * CCP�Ƚ������ѡ��
  */
#define CCP_COMPARE_SELECT_1            ((uint32_t)0x0 << CCP_CTL2_PXASS0_POS)
#define CCP_COMPARE_SELECT_2            ((uint32_t)0x1 << CCP_CTL2_PXASS0_POS)
#define CCP_COMPARE_SELECT_3            ((uint32_t)0x2 << CCP_CTL2_PXASS0_POS)
#define CHECK_CCP_COMPARE_SELECT(ASS)   (((ASS) == CCP_COMPARE_SELECT_1) \
                                      || ((ASS) == CCP_COMPARE_SELECT_2) \
                                      || ((ASS) == CCP_COMPARE_SELECT_3))

/**
  * CCPͨ���������
  */
#define CCP_CHANNEL_OUTPUT_PWM_ACTIVE   ((uint32_t)0x0)
#define CCP_CHANNEL_OUTPUT_PWM_INACTIVE ((uint32_t)0x1)
#define CCP_CHANNEL_OUTPUT_INACTIVE     ((uint32_t)0x2)
#define CCP_CHANNEL_OUTPUT_ACTIVE       ((uint32_t)0x3)
#define CHECK_CCP_CHANNEL_OUTPUT(CTL)   (((CTL) >> 2) == 0)

/**
  * ͨ�ö�ʱ����CCP��������
  */
/* ͨ�ö�ʱ��(GPTIM)��ʼ�������ú������� *********************************/
void GPTIM_Configuration(GPTIM_SFRmap* GPTIMx,
                    GPTIM_InitTypeDef* gptimInitStruct);
void GPTIM_Struct_Init (GPTIM_InitTypeDef* gptimInitStruct);
void GPTIM_Cmd (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Set_Counter (GPTIM_SFRmap* GPTIMx, uint32_t Counter);
void GPTIM_Set_Period (GPTIM_SFRmap* GPTIMx, uint32_t Period);
void GPTIM_Set_Prescaler (GPTIM_SFRmap* GPTIMx, uint32_t Prescaler);
void GPTIM_Counter_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t CounterMode);
void GPTIM_Clock_Config (GPTIM_SFRmap* GPTIMx, uint32_t NewClock);
void GPTIM_External_Pulse_Sync_Config (GPTIM_SFRmap* GPTIMx,
                    uint32_t PulseSync);
void GPTIM_Work_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t NewState);
void GPTIM_Updata_Immediately_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState);
void GPTIM_Master_Slave_Snyc_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState);
void GPTIM_Trigger_Select_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState TriggerSelect);
void GPTIM_Slave_Mode_Config (GPTIM_SFRmap* GPTIMx, FunctionalState SlaveMode);
void GPTIM_Master_Mode_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState MasterMode);
void GPTIM_Updata_Rising_Edge_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState);
void GPTIM_Updata_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Trigger_DMA_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Updata_DMA_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Updata_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Trigger_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Generate_Trigger_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState);
DIRStatus GPTIM_Get_Direction (GPTIM_SFRmap* GPTIMx);
uint32_t GPTIM_Get_Counter (GPTIM_SFRmap* GPTIMx);
uint32_t GPTIM_Get_Period (GPTIM_SFRmap* GPTIMx);
uint32_t GPTIM_Get_Prescaler (GPTIM_SFRmap* GPTIMx);
/* ͨ�ö�ʱ��(GPTIM)�жϼ���־���������� *******************************/
void GPTIM_Overflow_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Clear_Overflow_INT_Flag (GPTIM_SFRmap* GPTIMx);
void GPTIM_Clear_Updata_INT_Flag (GPTIM_SFRmap* GPTIMx);
void GPTIM_Clear_Trigger_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Overflow_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Updata_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Trigger_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Updata_DMA_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Trigger_DMA_INT_Flag (GPTIM_SFRmap* GPTIMx);
/* ͨ�ò�׽/�Ƚ�/PWM����(CCP)��׽���ܺ������� ****************************/
void CCP_Compare_Configuration(CCP_SFRmap* CCPx,
                    CCP_CompareInitTypeDef* ccpInitStruct);
void CCP_Compare_Struct_Init (CCP_CompareInitTypeDef* ccpInitStruct);
void CCP_Capture_Configuration (CCP_SFRmap* CCPx,
                    CCP_CaptureInitTypeDef* ccpInitStruct);
void CCP_Capture_Struct_Init (CCP_CaptureInitTypeDef* ccpInitStruct);
void CCP_PWM_Configuration (CCP_SFRmap* CCPx,
                    CCP_PWMInitTypeDef* ccpInitStruct);
void CCP_PWM_Struct_Init (CCP_PWMInitTypeDef* ccpInitStruct);
void CCP_Capture_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t EdgeConfig);
void CCP_Compare_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t EdgeConfig);
void CCP_PWM_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t EdgeConfig);
uint32_t CCP_Get_Capture_Result (CCP_SFRmap* CCPx, uint32_t Channel);
void CCP_Set_Compare_Result (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t Value);
#define CCP_Set_PWM_Duty_Ratio(CCPX, CHANNEL, VALUE)  \
        CCP_Set_Compare_Result(CCPX, CHANNEL, VALUE)
uint32_t CCP_Get_Compare_Result (CCP_SFRmap* CCPx, uint32_t Channel);
#define CCP_Get_PWM_Duty_Ratio(CCPX, CHANNEL)  \
        CCP_Get_Compare_Result(CCPX, CHANNEL)
void CCP_Compare_PWM_Signal_Clear (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState);
void CCP_Compare_Check_Config (CCP_SFRmap* CCPx, uint32_t CompareSelect);
void CCP_Channel_Output_Control (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t ChannelOutputCtl);
void CCP_Single_Pulse_Shut_Enable (CCP_SFRmap* CCPx, FunctionalState NewState);
void CCP_Single_Pulse_Enable (CCP_SFRmap* CCPx, FunctionalState NewState);
void CCP_PWM_Input_Measurement_Config (CCP_SFRmap* CCPx,
                    FunctionalState NewState);
void CCP_Input_XOR_Config (CCP_SFRmap* CCPx, FunctionalState NewState);
void CCP_Channel_DMA_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState);
void CCP_Generate_Trigger_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState);
void CCP_USART_Receive_Config (CCP_SFRmap* CCPx, FunctionalState NewState);
/* ͨ�ò�׽/�Ƚ�/PWM����(CCP)�жϼ���־���������� **********************/
FlagStatus CCP_Get_Channel_Trigger_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel);
FlagStatus CCP_Get_Trigger_DMA_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel);
void CCP_Channel_INT_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState);
void CCP_Clear_Channel_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel);
/*** ͨ�ö�ʱ��(GPTIM)��CCP��ض������  */


/*** �߼���ʱ��(ATIM)��ECCP��ض���  */
/**
  * ����  �߼���ʱ��(ATIM)������Ϣ�ṹ��
  */
typedef struct
{
    uint16_t m_Counter;         /* ��ʱ������ֵ��ȡֵ16λ���ݡ� */
    uint16_t m_Period;          /* ��ʱ������ֵ��ȡֵ16λ���ݡ� */
    uint16_t m_Prescaler;       /* ��ʱ��Ԥ��Ƶֵ��ȡֵ16λ���ݡ� */
    uint16_t m_Postscaler;      /* ��ʱ�����Ƶ����ֵ��
                                   ȡֵΪ�ꡰATIM��ʱ�����Ƶ����ֵ���е�һ���� */
    uint32_t m_CounterMode;     /* ��ʱ������ģʽ��
                                   ȡֵΪ�ꡰATIM��ʱ������ģʽ���е�һ�� */
    uint16_t m_Clock;           /* ��ʱ������ʱ�ӣ�
                                   ȡֵΪ�ꡰATIM��ʱ������ʱ�ӡ��е�һ�� */
    uint16_t m_WorkMode;        /* ��ʱ/����ģʽѡ��
                                   ȡֵΪ�ꡰATIM��ʱ/����ģʽѡ���е�һ�� */
    uint16_t m_EXPulseSync;     /* Tx����ģʽ�ⲿ������������ͬ�����ƣ�
                                   ȡֵΪ�ꡰATIM����ģʽ�ⲿ������������ͬ�����ơ��е�һ�� */
} ATIM_InitTypeDef; /* T5/T6��T9/T10 */

/**
  * ����  ECCP ��׽ģʽ������Ϣ�ṹ��
  */
typedef struct
{
    uint32_t m_Channel;         /* ͨ����ţ�
                                   ȡֵ��Χ���㡰ECCPͨ�����ĺꡣ */
    uint32_t m_Mode;            /* ��׽ģʽѡ��
                                   ȡֵ��Χ�����CHECK_ECCP_CAP_MODE��Լ�������� */
    FunctionalState m_PWMInput;        /* PWM����ģʽ��
                                   ȡֵΪTRUE��FALSE�� */
    FunctionalState m_XORMode;         /* �������ģʽ��
                                   ȡֵΪTRUE��FALSE�� */
} ECCP_CaptureInitTypeDef;

/**
  * ����  ECCP PWMģʽ������Ϣ�ṹ��
  */
typedef struct
{
    uint32_t m_Channel;         /* ͨ����ţ�
                                   ȡֵ��Χ���㡰ECCPͨ�����ĺꡣ */
    uint32_t m_Mode;            /* ��׽/�Ƚ���ģʽѡ��
                                   ȡֵ��Χ�����CHECK_ECCP_PWM_MODE��Լ�������� */
    uint16_t m_DutyRatio;       /* ռ�ձȣ�
                                   ȡֵ��ΧΪ0~65535�� */
    uint8_t m_DeadTime;         /* ͨ���������ƣ�
                                   ȡֵΪ0~255�� */
    uint8_t m_OutputMode;       /* ͨ�����ģʽ��
                                   ȡֵΪ��ECCPͨ�����ģʽ���е�һ���� */
    uint16_t m_HOutputCtl;      /* ͨ��H������ƣ�
                                   ȡֵΪ��ECCPͨ��������ơ��е�һ���� */
    uint16_t m_LOutputCtl;      /* ͨ��L������ƣ�
                                   ȡֵΪ��ECCPͨ��������ơ��е�һ���� */
    FunctionalState m_PhaseMove;       /* ��λ�ƶ�ʹ�ܣ�
                                   ȡֵΪTRUE��FALSE�� */
    FunctionalState m_SinglePWM;       /* ���������ģʽ��
                                   ȡֵΪTRUE��FALSE�� */
    FunctionalState m_CloseTimer;      /* ���������ģʽѡ��
                                   ȡֵΪTRUE��FALSE�� */
} ECCP_PWMInitTypeDef;

/**
  * ATIM��ʱ�����Ƶ����ֵ
  */
#define ATIM_POSTSCALER_DIV_1           ((uint16_t)0x0)
#define ATIM_POSTSCALER_DIV_2           ((uint16_t)0x1)
#define ATIM_POSTSCALER_DIV_3           ((uint16_t)0x2)
#define ATIM_POSTSCALER_DIV_4           ((uint16_t)0x3)
#define ATIM_POSTSCALER_DIV_5           ((uint16_t)0x4)
#define ATIM_POSTSCALER_DIV_6           ((uint16_t)0x5)
#define ATIM_POSTSCALER_DIV_7           ((uint16_t)0x6)
#define ATIM_POSTSCALER_DIV_8           ((uint16_t)0x7)
#define ATIM_POSTSCALER_DIV_9           ((uint16_t)0x8)
#define ATIM_POSTSCALER_DIV_10          ((uint16_t)0x9)
#define ATIM_POSTSCALER_DIV_11          ((uint16_t)0xA)
#define ATIM_POSTSCALER_DIV_12          ((uint16_t)0xB)
#define ATIM_POSTSCALER_DIV_13          ((uint16_t)0xC)
#define ATIM_POSTSCALER_DIV_14          ((uint16_t)0xD)
#define ATIM_POSTSCALER_DIV_15          ((uint16_t)0xE)
#define ATIM_POSTSCALER_DIV_16          ((uint16_t)0xF)
#define CHECK_ATIM_POSTSCALER(SCALER)   (((SCALER) >= 0) && ((SCALER) <= 0xF))

/**
  * ATIM��ʱ������ģʽ
  */
#define ATIM_COUNT_DOWN_UF              ((uint32_t)0x0 << ATIM_TXCTL_TXCMS0_POS)
#define ATIM_COUNT_UP_OF                ((uint32_t)0x4 << ATIM_TXCTL_TXCMS0_POS)
#define ATIM_COUNT_UP_DOWN_OF           ((uint32_t)0x1 << ATIM_TXCTL_TXCMS0_POS)
#define ATIM_COUNT_UP_DOWN_UF           ((uint32_t)0x2 << ATIM_TXCTL_TXCMS0_POS)
#define ATIM_COUNT_UP_DOWN_OUF          ((uint32_t)0x3 << ATIM_TXCTL_TXCMS0_POS)
#define CHECK_ATIM_COUNTER_MODE(MODE)   (((MODE) == ATIM_COUNT_DOWN_UF) \
                                      || ((MODE) == ATIM_COUNT_UP_OF) \
                                      || ((MODE) == ATIM_COUNT_UP_DOWN_OF) \
                                      || ((MODE) == ATIM_COUNT_UP_DOWN_UF) \
                                      || ((MODE) == ATIM_COUNT_UP_DOWN_OUF))

/**
  * ATIM��ʱ������ʱ��
  */
#define ATIM_SCLK                       ((uint32_t)0x0 << ATIM_TXCTL_TXCLK0_POS)
#define ATIM_HFCLK                      ((uint32_t)0x1 << ATIM_TXCTL_TXCLK0_POS)
#define ATIM_LFCLK                      ((uint32_t)0x2 << ATIM_TXCTL_TXCLK0_POS)
#define CHECK_ATIM_CLOCK_CONFIG(CLK)    (((CLK) == ATIM_SCLK) \
                                      || ((CLK) == ATIM_HFCLK) \
                                      || ((CLK) == ATIM_LFCLK))

/**
  * ATIM��ʱ/����ģʽѡ��
  */
#define ATIM_TIMER_MODE                 ((uint32_t)0 << ATIM_TXCTL_TXCS_POS)
#define ATIM_COUNTER_MODE               ((uint32_t)1 << ATIM_TXCTL_TXCS_POS)
#define CHECK_ATIM_WORK_MODE(MODE)      (((MODE) == ATIM_TIMER_MODE) \
                                      || ((MODE) == ATIM_COUNTER_MODE))

/**
  * ATIM����ģʽ�ⲿ������������ͬ������
  */
#define ATIM_EX_SYNC_MODE               ((uint32_t)0x0 << ATIM_TXCTL_TXSY_POS)
#define ATIM_NO_SYNC_MODE               ((uint32_t)0x1 << ATIM_TXCTL_TXSY_POS)
#define CHECK_ATIM_SYNC_MODE(MODE)      (((MODE) == ATIM_EX_SYNC_MODE) \
                                      || ((MODE) == ATIM_NO_SYNC_MODE))

/**
  * ECCPͨ��
  */
#define ECCP_CHANNEL_1                  ((uint32_t)0x0)
#define ECCP_CHANNEL_2                  ((uint32_t)0x1)
#define ECCP_CHANNEL_3                  ((uint32_t)0x2)
#define ECCP_CHANNEL_4                  ((uint32_t)0x3)
#define CHECK_ECCP_CHANNEL(CHANNEL)     (((CHANNEL) >> 2) == 0)

/**
  * ECCPͨ��ģʽѡ��
  */
#define ECCP_MODE_RST                   ((uint32_t)0x0)
#define ECCP_CMP_TOGGLE_LEVEL           ((uint32_t)0x2)
#define ECCP_CAP_FALLING_EDGE           ((uint32_t)0x4)
#define ECCP_CAP_RISING_EDGE            ((uint32_t)0x5)
#define ECCP_CAP_4TH_RISING_EDGE        ((uint32_t)0x6)
#define ECCP_CAP_16TH_RISING_EDGE       ((uint32_t)0x7)
#define ECCP_CMP_ACTIVE_LEVEL           ((uint32_t)0x8)
#define ECCP_CMP_INACTIVE_LEVEL         ((uint32_t)0x9)
#define ECCP_CMP_SOFT_INT               ((uint32_t)0xA)
#define ECCP_CMP_SPECIAL_EVENT          ((uint32_t)0xB)
#define ECCP_PWM_FREE                   ((uint32_t)0xC)
#define ECCP_PWM_COORDINATION           ((uint32_t)0xD)
#define ECCP_PWM_SINGLE                 ((uint32_t)0xE)
#define CHECK_ECCP_MODE(MODE)           (((MODE) == ECCP_MODE_RST) \
                                      || ((MODE) == ECCP_CMP_TOGGLE_LEVEL) \
                                      || ((MODE) == ECCP_CAP_RISING_EDGE) \
                                      || ((MODE) == ECCP_CAP_FALLING_EDGE) \
                                      || ((MODE) == ECCP_CAP_4TH_RISING_EDGE) \
                                      || ((MODE) == ECCP_CAP_16TH_RISING_EDGE) \
                                      || ((MODE) == ECCP_CMP_ACTIVE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_INACTIVE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_SOFT_INT) \
                                      || ((MODE) == ECCP_CMP_SPECIAL_EVENT) \
                                      || ((MODE) == ECCP_PWM_MODE))
#define CHECK_ECCP_CAP_MODE(MODE)       (((MODE) == ECCP_MODE_RST) \
                                      || ((MODE) == ECCP_CAP_RISING_EDGE) \
                                      || ((MODE) == ECCP_CAP_FALLING_EDGE) \
                                      || ((MODE) == ECCP_CAP_4TH_RISING_EDGE) \
                                      || ((MODE) == ECCP_CAP_16TH_RISING_EDGE))
#define CHECK_ECCP_CMP_MODE(MODE)       (((MODE) == ECCP_MODE_RST) \
                                      || ((MODE) == ECCP_CMP_TOGGLE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_ACTIVE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_INACTIVE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_SOFT_INT) \
                                      || ((MODE) == ECCP_CMP_SPECIAL_EVENT))
#define CHECK_ECCP_PWM_MODE(MODE)       (((MODE) == ECCP_MODE_RST) \
                                      || ((MODE) == ECCP_PWM_FREE) \
                                      || ((MODE) == ECCP_PWM_COORDINATION) \
                                      || ((MODE) == ECCP_PWM_SINGLE))

/**
  * ECCPͨ�����ģʽ
  */
#define ECCP_OUTPUT_INDEPENDENT         ((uint8_t)0x0)
#define ECCP_OUTPUT_COMPLEMENTARY       ((uint8_t)0x1)
#define CHECK_ECCP_OUTPUT_MODE(MODE)    (((MODE) == ECCP_OUTPUT_INDEPENDENT) \
                                     || ((MODE) == ECCP_OUTPUT_COMPLEMENTARY))

/**
  * ECCPͨ���������
  */
#define ECCP_CHANNEL_OUTPUT_PWM_ACTIVE   ((uint16_t)0x0)
#define ECCP_CHANNEL_OUTPUT_PWM_INACTIVE ((uint16_t)0x1)
#define ECCP_CHANNEL_OUTPUT_INACTIVE     ((uint16_t)0x2)
#define ECCP_CHANNEL_OUTPUT_ACTIVE       ((uint16_t)0x3)
#define CHECK_ECCP_CHANNEL_OUTPUT(CTL)   (((CTL) >> 2) == 0)

/**
  * ATIM��ģʽѡ��
  */
#define ATIM_SLAVE_FORBIDDEN_MODE       ((uint32_t)0x0)
#define ATIM_SLAVE_TRIGGER_MODE         ((uint32_t)0x4)
#define ATIM_SLAVE_GATED_MODE           ((uint32_t)0x5)
#define ATIM_SLAVE_RESET_MODE           ((uint32_t)0x6)
#define ATIM_SLAVE_COUNTER_MODE         ((uint32_t)0x7)
#define CHECK_ATIM_SLAVE_MODE(MODE)     (((MODE) == ATIM_SLAVE_FORBIDDEN_MODE) \
                                     || ((MODE) == ATIM_SLAVE_TRIGGER_MODE) \
                                     || ((MODE) == ATIM_SLAVE_GATED_MODE) \
                                     || ((MODE) == ATIM_SLAVE_RESET_MODE) \
                                     || ((MODE) == ATIM_SLAVE_COUNTER_MODE))

/**
  * ATIM��ģʽѡ��
  */
#define ATIM_MASTER_TXEN_SIGNAL         ((uint32_t)1 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_TXIF_SIGNAL         ((uint32_t)2 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH1IF_SIGNAL    ((uint32_t)3 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH1_SIGNAL      ((uint32_t)4 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH2_SIGNAL      ((uint32_t)5 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH3_SIGNAL      ((uint32_t)6 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH4_SIGNAL      ((uint32_t)7 << ECCP_CTL2_TXMMS0_POS)
#define CHECK_ATIM_MASTER_MODE(MODE)    (((MODE) == ATIM_MASTER_TXEN_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_TXIF_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH1IF_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH1_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH2_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH3_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH4_SIGNAL))

/**
  * ATIM����ѡ��
  */
#define ATIM_TRIGGER_T1                 ((uint32_t)0x0 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_T20                ((uint32_t)0x1 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_T5                 ((uint32_t)0x2 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_T9                 ((uint32_t)0x3 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_ECCPXCH1           ((uint32_t)0x4 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_ECCPXCH2           ((uint32_t)0x5 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_ECCPXCH3           ((uint32_t)0x6 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_TXCK               ((uint32_t)0x7 << ECCP_CTL2_TXTS0_POS)
#define CHECK_ATIM_TRIGGER_MODE(MODE)   (((uint32_t)(MODE) & (~ECCP_CTL2_TXTS)) == 0)

/**
  * ECCPͨ���˿�
  */
#define ECCP_PORT_LOW                   ((uint32_t)0x0)
#define ECCP_PORT_HIGH                  ((uint32_t)0x2)
#define CHECK_ECCP_PORT(PORT)           (((PORT) == ECCP_PORT_LOW) \
                                     || ((PORT) == ECCP_PORT_HIGH))

/**
  * ECCP�Զ��ر��¼�״̬
  */
#define ECCP_CHANNEL_WORKING_STATE      ((uint32_t)0x0)
#define ECCP_CHANNEL_SHUTDOWN_STATE     ((uint32_t)0x1)
#define CHECK_ECCP_CHANNEL_STATE(STATE) (((STATE) == ECCP_CHANNEL_SHUTDOWN_STATE) \
                                      || ((STATE) == ECCP_CHANNEL_WORKING_STATE))

/**
  * ͨ���Զ��ر�Դ
  */
#define ECCP_CHANNEL_SHUTDOWN_FORBID    ((uint32_t)0x0)
#define ECCP_CHANNEL_CMP2CMP3_ACTIVE    ((uint32_t)0x1)
#define ECCP_CHANNEL_BKIN_INACTIVE      ((uint32_t)0x2)
#define CHECK_ECCP_SHUTDOWN_SIGNAL(STATE) \
                                        (((STATE) == ECCP_CHANNEL_SHUTDOWN_FORBID) \
                                        || ((STATE) == ECCP_CHANNEL_CMP2CMP3_ACTIVE)\
                                      || ((STATE) == ECCP_CHANNEL_BKIN_INACTIVE))
/**
  * ͨ��4�Ƚ����ر�Դѡ��
  */
#define CMP3_OUTPUT_SHUNT    ((uint32_t)0x0)
#define CMP2_OUTPUT_SHUNT    ((uint32_t)0x1)
#define CHECK_CHANNEL4_SHUTDOWN_SIGNAL(STATE) \
                                        (((STATE) == CMP3_OUTPUT_SHUNT) \
                                      || ((STATE) == CMP2_OUTPUT_SHUNT))
/**
  * �����ʱ��
  */
#define ECCP_ZERO_CLOCK_DIV_1           ((uint32_t)0x0 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_2           ((uint32_t)0x1 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_4           ((uint32_t)0x2 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_8           ((uint32_t)0x3 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_16          ((uint32_t)0x4 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_32          ((uint32_t)0x5 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_64          ((uint32_t)0x6 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_128         ((uint32_t)0x7 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_256         ((uint32_t)0x8 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_512         ((uint32_t)0x9 << ECCP_ZPDCTL0_ZCLK0_POS)
#define CHECK_ECCP_ZERO_CLOCK(CLK)      (((CLK) == ECCP_ZERO_CLOCK_DIV_1) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_2) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_4) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_8) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_16) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_32) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_64) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_128) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_256) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_512))

/**
  * ͨ��������ѹ
  */
#define ECCP_ZERO_VOLTAGE_DECIMAL_15    ((uint32_t)0x0)
#define ECCP_ZERO_VOLTAGE_DECIMAL_25    ((uint32_t)0x1)
#define ECCP_ZERO_VOLTAGE_DECIMAL_35    ((uint32_t)0x2)
#define ECCP_ZERO_VOLTAGE_DECIMAL_45    ((uint32_t)0x3)
#define ECCP_ZERO_VOLTAGE_DECIMAL_55    ((uint32_t)0x4)
#define CHECK_ECCP_ZERO_VOLTAGE(VOL)    (((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_15) \
                                     || ((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_25) \
                                     || ((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_35) \
                                     || ((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_45) \
                                     || ((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_55))
/**
  * �߼���ʱ����ECCP��������
  */
/* �߼���ʱ��(ATIM)��ʼ�������ú�������***********************************/
void ATIM_X_Configuration(ATIM_SFRmap* ATIMx, ATIM_InitTypeDef* atimInitStruct);
void ATIM_Z_Configuration(ATIM_SFRmap* ATIMx, ATIM_InitTypeDef* atimInitStruct);
void ATIM_Struct_Init (ATIM_InitTypeDef* atimInitStruct);
void ATIM_X_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Updata_Configuration (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter,
                    uint32_t UpdataOutput, uint32_t UpdataImmediately);
void ATIM_Z_Updata_Configuration (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter,
                    uint32_t UpdataOutput, uint32_t UpdataImmediately);
void ATIM_X_Updata_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Set_Counter (ATIM_SFRmap* ATIMx, uint16_t Counter);
void ATIM_Z_Set_Counter (ATIM_SFRmap* ATIMx, uint16_t Counter);
void ATIM_X_Set_Period (ATIM_SFRmap* ATIMx, uint16_t Period);
void ATIM_Z_Set_Period (ATIM_SFRmap* ATIMx, uint16_t Period);
void ATIM_X_Set_Prescaler (ATIM_SFRmap* ATIMx, uint16_t Prescaler);
void ATIM_Z_Set_Prescaler (ATIM_SFRmap* ATIMx, uint16_t Prescaler);
void ATIM_X_Counter_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t CounterMode);
void ATIM_Z_Counter_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t CounterMode);
void ATIM_X_Clock_Config (ATIM_SFRmap* ATIMx, uint32_t NewClock);
void ATIM_Z_Clock_Config (ATIM_SFRmap* ATIMx, uint32_t NewClock);
void ATIM_X_Postscaler_Config (ATIM_SFRmap* ATIMx, uint32_t NewPostscaler);
void ATIM_Z_Postscaler_Config (ATIM_SFRmap* ATIMx, uint32_t NewPostscaler);
void ATIM_X_External_Pulse_Sync_Config (ATIM_SFRmap* ATIMx, uint32_t PulseSync);
void ATIM_Z_External_Pulse_Sync_Config (ATIM_SFRmap* ATIMx, uint32_t PulseSync);
void ATIM_X_Work_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t NewState);
void ATIM_Z_Work_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t NewState);
DIRStatus ATIM_X_Get_Direction (ATIM_SFRmap* ATIMx);
DIRStatus ATIM_Z_Get_Direction (ATIM_SFRmap* ATIMx);
void ATIM_X_Overflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Overflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Underflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Underflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_TriggerAD_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_TriggerAD_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Set_TriggerAD_Signal (ATIM_SFRmap* ATIMx,
                    uint16_t CompareAD0, uint16_t CompareAD1);
void ATIM_Z_Set_TriggerAD_Signal (ATIM_SFRmap* ATIMx, uint16_t CompareAD);
void ATIM_X_Updata_Immediately_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState);
void ATIM_Z_Updata_Immediately_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState);
void ATIM_X_Updata_Output_Ctl (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_Output_Ctl (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Updata_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Set_Updata_Counter (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter);
void ATIM_Z_Set_Updata_Counter (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter);
void ATIM_X_Slave_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t SlaveMode);
void ATIM_Z_Slave_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t SlaveMode);
void ATIM_Master_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t MasterMode);
void ATIM_Master_Slave_Snyc_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState);
void ATIM_Trigger_Select_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState TriggerSelect);
void ATIM_Timer_Unite_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
uint16_t ATIM_X_Get_Counter (ATIM_SFRmap* ATIMx);
uint16_t ATIM_Z_Get_Counter (ATIM_SFRmap* ATIMx);
uint16_t ATIM_X_Get_Period (ATIM_SFRmap* ATIMx);
uint16_t ATIM_Z_Get_Period (ATIM_SFRmap* ATIMx);
uint16_t ATIM_X_Get_Prescaler (ATIM_SFRmap* ATIMx);
uint16_t ATIM_Z_Get_Prescaler (ATIM_SFRmap* ATIMx);
/* �߼���ʱ��(ATIM)�жϼ���־����������*********************************/
void ATIM_X_Updata_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Overflow_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Overflow_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Trigger_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Updata_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Trigger_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
FlagStatus ATIM_X_Get_Updata_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_Z_Get_Updata_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_X_Get_Overflow_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_Z_Get_Overflow_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_X_Get_Trigger_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_X_Generate_Trigger_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState);
FlagStatus ATIM_X_Get_Updata_DMA_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_Z_Get_Updata_DMA_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_X_Get_Trigger_DMA_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_X_Clear_Updata_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_Z_Clear_Updata_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_X_Clear_Overflow_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_Z_Clear_Overflow_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_X_Clear_Trigger_INT_Flag (ATIM_SFRmap* ATIMx);
/* ��ǿ�Ͳ�׽/�Ƚ�/PWM����(ECCP)��׽���ܺ�������**************************/
void ECCP_Compare_Configuration(ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t CompareMode, uint16_t CompareValue);
void ECCP_Capture_Configuration (ECCP_SFRmap* ECCPx,
                    ECCP_CaptureInitTypeDef* eccpInitStruct);
void ECCP_Capture_Struct_Init (ECCP_CaptureInitTypeDef* eccpInitStruct);
void ECCP_PWM_Configuration (ECCP_SFRmap* ECCPx,
                    ECCP_PWMInitTypeDef* ECCP_InitStruct);
void ECCP_PWM_Struct_Init (ECCP_PWMInitTypeDef* ECCP_InitStruct);
void ECCP_Capture_Mode_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t EdgeConfig);
void ECCP_Compare_Mode_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t CmpConfig);
void ECCP_PWM_Mode_Config (ECCP_SFRmap* ECCPx, uint32_t PWMConfig);
uint16_t ECCP_Get_Capture_Result (ECCP_SFRmap* ECCPx, uint32_t Channel);
void ECCP_Set_Compare_Result (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint16_t Value);
void ECCP_Generate_Trigger_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, FunctionalState NewState);
void ECCP_PWM_Input_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Input_XOR_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Single_Pulse_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Single_Pulse_Shut_Enable (ECCP_SFRmap* ECCPx,
                    FunctionalState NewState);
void ECCP_PWM_Restart_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Dead_Time_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint8_t DeadTime);
void ECCP_Channel_Output_Control (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t ChannelOutputCtl);
void ECCP_Channel_Output_Mode (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t ChannelOutputMode);
void ECCP_Channel_Work_State_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t WorkingState);
void
ECCP_CHANNEL4_Shutdown_SEL (ECCP_SFRmap* ECCPx,
                    uint32_t ShutDownSignal);
FlagStatus ECCP_Get_Channel_Work_State (ECCP_SFRmap* ECCPx, uint32_t Channel);
void ECCP_Channel_Shutdown_Signal (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t ShutDownSignal);
void ECCP_Channel_Pin_Ctl (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t ChannelPinCtl);
void ECCP_Zero_Clock_Config (ECCP_SFRmap* ECCPx, uint32_t ZeroClock);
void ECCP_Channel_Pin_Tristate_Enable (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t Port, uint32_t PinTristateCtl);
/* ��ǿ�Ͳ�׽/�Ƚ�/PWM����(ECCP)�жϼ���־����������********************/
void ECCP_Channel_INT_Enable (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, FunctionalState NewState);
void ECCP_X_Turn_off_DMA_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Channel_DMA_Enable (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, FunctionalState NewState);
FlagStatus ECCP_Get_Channel_Trigger_INT_Flag (ECCP_SFRmap* ECCPx,
                    uint32_t Channel);
FlagStatus ECCP_X_Get_Turn_off_DMA_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ECCP_Get_Trigger_DMA_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel);
void ECCP_Clear_Channel_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel);
void ECCP_PWM_Move_Phase_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Channel_Zero_Detect_Sequential_Ctl (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t NewState);
FlagStatus ECCP_Get_Channel_Zero_Detection_State (ECCP_SFRmap* ECCPx,
                    uint32_t Channel);
void ECCP_Clear_Channel_Zero_Detection_State (ECCP_SFRmap* ECCPx,
                    uint32_t Channel);
void ECCP_Channel_Zero_Detect_Enable (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t NewState);
void ECCP_Channel_Zero_Voltage_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t ZeroDetectVoltage);

/*** �߼���ʱ��(ATIM)��ECCP��ض������  */


/*** ��ǿ���������ģ��(EPWM)����ض�ʱ������  */
/**
  * ����  ��ǿ���������ģ�鶨ʱ��(ETIM)��ʱ��������Ϣ�ṹ��
  */
typedef struct
{
    uint32_t m_Counter;         /* ��ʱ������ֵ��ȡֵ32λ���ݡ� */
    uint32_t m_Period;          /* ��ʱ������ֵ��ȡֵ32λ���ݡ� */
    uint32_t m_Prescaler;       /* ��ʱ��Ԥ��Ƶֵ��ȡֵ32λ���ݡ� */
    uint32_t m_Phase;           /* ��ʱ����λֵ��ȡֵ32λ���ݡ� */
    uint32_t m_CounterMode;     /* ��ʱ������ģʽ��
                                   ȡֵΪ�ꡰETIM��ʱ������ģʽ���е�һ�� */
    uint32_t m_Sync;            /* Tx����ģʽ�ⲿ������������ͬ�����ƣ�
                                   ȡֵΪ�ꡰETIM����ģʽ�ⲿ������������ͬ�����ơ��е�һ�� */
    uint32_t m_Clock;           /* ��ʱ������ʱ�ӣ�
                                   ȡֵΪ�ꡰETIM��ʱ������ʱ�ӡ��е�һ�� */
    uint32_t m_WorkMode;        /* ��ʱ/����ģʽѡ��
                                   ȡֵΪ�ꡰETIM��ʱ/����ģʽѡ���е�һ�� */
    FunctionalState m_PhaseEn;  /* ��λ�Ĵ�������ʹ��,
                                   ȡֵΪTRUE �� FALSE�� */
    uint32_t m_PhaseDirection;  /* ��λ������ƣ�
                                   ȡֵΪ�ꡰETIM��λ������ơ��е�һ�� */
    FunctionalState m_GateEn;  /* �ſ�ʹ��,
                                   ȡֵΪTRUE �� FALSE�� */
} ETIM_InitTypeDef; /* T11��T12��T13 */

/**
  * ETIM��λ�������
  */
#define ETIM_PHASE_DOWN                 ((uint32_t)0x0 << EPWM_TXCTL_PHSDIR_POS)
#define ETIM_PHASE_UP                   ((uint32_t)0x1 << EPWM_TXCTL_PHSDIR_POS)
#define CHECK_ETIM_PHASE_DIR(DIR)       (((DIR) == ETIM_PHASE_UP) \
                                      || ((DIR) == ETIM_PHASE_DOWN))

/**
  * ETIM��ʱ������ģʽ
  */
#define ETIM_COUNT_DOWN_UF              ((uint32_t)0x0 << EPWM_TXCTL_TXCMS0_POS)
#define ETIM_COUNT_UP_OF                ((uint32_t)0x4 << EPWM_TXCTL_TXCMS0_POS)
#define ETIM_COUNT_UP_DOWN_OF           ((uint32_t)0x1 << EPWM_TXCTL_TXCMS0_POS)
#define ETIM_COUNT_UP_DOWN_UF           ((uint32_t)0x2 << EPWM_TXCTL_TXCMS0_POS)
#define ETIM_COUNT_UP_DOWN_OUF          ((uint32_t)0x3 << EPWM_TXCTL_TXCMS0_POS)
#define CHECK_ETIM_COUNTER_MODE(MODE)   (((MODE) == ETIM_COUNT_DOWN_UF) \
                                      || ((MODE) == ETIM_COUNT_UP_OF) \
                                      || ((MODE) == ETIM_COUNT_UP_DOWN_OF) \
                                      || ((MODE) == ETIM_COUNT_UP_DOWN_UF) \
                                      || ((MODE) == ETIM_COUNT_UP_DOWN_OUF))

/**
  * ETIM��ʱ������ʱ��
  */
#define ETIM_SCLK                       ((uint32_t)0x0 << EPWM_TXCTL_TXCLK0_POS)
#define ETIM_HFCLK                      ((uint32_t)0x1 << EPWM_TXCTL_TXCLK0_POS)
#define ETIM_LFCLK                      ((uint32_t)0x2 << EPWM_TXCTL_TXCLK0_POS)
#define CHECK_ETIM_CLOCK_CONFIG(CLK)    (((CLK) == ETIM_SCLK) \
                                      || ((CLK) == ETIM_HFCLK) \
                                      || ((CLK) == ETIM_LFCLK))

/**
  * ETIM��ʱ/����ģʽѡ��
  */
#define ETIM_TIMER_MODE                 ((uint32_t)0 << EPWM_TXCTL_TXCS_POS)
#define ETIM_COUNTER_MODE               ((uint32_t)1 << EPWM_TXCTL_TXCS_POS)
#define CHECK_ETIM_WORK_MODE(MODE)      (((MODE) == ETIM_TIMER_MODE) \
                                      || ((MODE) == ETIM_COUNTER_MODE))

/**
  * ETIM����ģʽ�ⲿ������������ͬ������
  */
#define ETIM_EX_SYNC_MODE               ((uint32_t)0x0 << EPWM_TXCTL_TXSY_POS)
#define ETIM_NO_SYNC_MODE               ((uint32_t)0x1 << EPWM_TXCTL_TXSY_POS)
#define CHECK_ETIM_SYNC_MODE(MODE)      (((MODE) == ETIM_EX_SYNC_MODE) \
                                      || ((MODE) == ETIM_NO_SYNC_MODE))

#endif /* _KF32A_BASIC_TIM_H */

/**
  ******************************************************************************
  * �ļ���  kf32a_basic_qei.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ���������������·(QEI)��صĹ��ܺ�����������
  *          + �������������·(QEI)��ʼ������
  *          + �������������·(QEI)�������ú���
  *          + �������������·(QEI)�жϹ�����
  *
  *********************************************************************
  */

#include "kf32a_basic_qei.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_qei
/* ��ʱ��˽�ж��� ----------------------------------------------------*/
/* QEIx_CTL0�Ĵ�����ʱ�����ܳ�ʼ������ */
#define QEI_CTL0_TIMER_INIT_MASK        (QEI_CTL0_TXDIR \
                                       | QEI_CTL0_TXCS)

/* QEIx_CTL0�Ĵ�����ʼ������ */
#define QEI_CTL0_INIT_MASK              (QEI_CTL0_INDRES \
                                       | QEI_CTL0_TXDIROUT)

/* QEIx_CTL1�Ĵ�����ʼ������ */
#define QEI_CTL1_INIT_MASK              (QEI_CTL1_QEICLK \
                                       | QEI_CTL1_QEIOUT \
                                       | QEI_CTL1_QEICK \
                                       | QEI_CTL1_SWPAB)

/**
  *   ##### �������������·(QEI)��ʼ������ #####
  */
/**
  * ����  ��λQEI���裬ʹ������ʱ�ӡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  �ޡ�
  */
void
QEI_Reset (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

#ifdef KF32A_Periph_qei0
    if (QEIx == QEI0_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_QEI0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_qei1
    if (QEIx == QEI1_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_QEI1CLKEN, TRUE);
    }
#endif
}

/**
  * ����  QEIģ��������������ʼ����
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       qeiInitStruct: QEIģ��������������Ϣ�ṹ�塣
  * ����  �ޡ�
  */
void
QEI_Configuration(QEI_SFRmap* QEIx, QEI_InitTypeDef* qeiInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(qeiInitStruct->m_IndexReset));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(qeiInitStruct->m_DirectionEn));
    CHECK_RESTRICTION(CHECK_QEI_SOURCE(qeiInitStruct->m_WorkClock));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(qeiInitStruct->m_DigitalFilterEn));
    CHECK_RESTRICTION(CHECK_QEI_DIGITAL_FILTER_CLK( \
                            qeiInitStruct->m_DigitalFilterPrescaler));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(qeiInitStruct->m_SwapABEn));

    /*---------------- ����Tx_CNT�Ĵ��� ----------------*/
    QEIx->CNT = (uint32_t)qeiInitStruct->m_Counter;

    /*---------------- ����PPx���ڼĴ��� ---------------*/
    QEIx->PPX = (uint32_t)qeiInitStruct->m_Period;

    /*---------------- ����Tx_PRSCԤ��Ƶ�Ĵ��� ---------------*/
    QEIx->PRSC = (uint32_t)qeiInitStruct->m_Prescaler;

    /*---------------- ����QEIx_CTL1�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_WorkClock������QEICLKλ�� */
    /* ���ݽṹ���Աm_DigitalFilterEn������QEIOUTλ�� */
    /* ���ݽṹ���Աm_DigitalFilterPrescaler������QEICKλ�� */
    /* ���ݽṹ���Աm_SwapABEn������SWPABλ�� */
    tmpreg = (qeiInitStruct->m_WorkClock) \
           | (qeiInitStruct->m_DigitalFilterEn << QEI_CTL1_QEIOUT_POS) \
           | (qeiInitStruct->m_DigitalFilterPrescaler) \
           | (qeiInitStruct->m_SwapABEn << QEI_CTL1_SWPAB_POS);
    QEIx->CTL1 = SFR_Config (QEIx->CTL1,
                          ~QEI_CTL1_INIT_MASK,
                          tmpreg);

    /*---------------- ����QEIx_CTL0�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_IndexReset������INDRESλ�� */
    /* ���ݽṹ���Աm_DirectionEn������TXDIROUTλ�� */
    tmpreg = (qeiInitStruct->m_IndexReset << QEI_CTL0_INDRES_POS) \
           | (qeiInitStruct->m_DirectionEn << QEI_CTL0_TXDIROUT_POS);
    QEIx->CTL0 = SFR_Config (QEIx->CTL0,
                          ~QEI_CTL0_INIT_MASK,
                          tmpreg);
}

/**
  * ����  QEIģ�鶨ʱ����ʼ����
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       qeiTimerInitStruct: QEI��ʱ��(T7/T8)������Ϣ�ṹ��ָ�롣
  * ����  �ޡ�
  */
void
QEI_Timer_Configuration(QEI_SFRmap* QEIx, QEI_TimerTypeDef* qeiTimerInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_COUNT_DIRECT(qeiTimerInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_QEI_TIMER_SOURCE(qeiTimerInitStruct->m_ClockSource));

    /*---------------- ����Tx_CNT�Ĵ��� ----------------*/
    QEIx->CNT = (uint32_t)qeiTimerInitStruct->m_Counter;

    /*---------------- ����PPx���ڼĴ��� ---------------*/
    QEIx->PPX = (uint32_t)qeiTimerInitStruct->m_Period;

    /*---------------- ����TxԤ��Ƶ�Ĵ��� --------------*/
    QEIx->PRSC = (uint32_t)qeiTimerInitStruct->m_Prescaler;

    /*---------------- ����QEIx_CTL0�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_CounterMode������TXDIRλ�� */
    /* ���ݽṹ���Աm_ClockSource������TXCSλ�� */
    tmpreg = (qeiTimerInitStruct->m_CounterMode) \
           | (qeiTimerInitStruct->m_ClockSource);
    QEIx->CTL0 = SFR_Config (QEIx->CTL0,
                          ~QEI_CTL0_TIMER_INIT_MASK,
                          tmpreg);
}

/**
  * ����  ��ʼ��QEIģ��������������Ϣ�ṹ�塣
  * ����  qeiInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
QEI_Struct_Init (QEI_InitTypeDef* qeiInitStruct)
{
    /* ��ʼ�� λ�ü�������λʹ�� */
    qeiInitStruct->m_IndexReset = FALSE;
    /* ��ʼ�� λ�ü���������״̬���ʹ�� */
    qeiInitStruct->m_DirectionEn = FALSE;
    /* ��ʼ�� QEI����ʱ�� */
    qeiInitStruct->m_WorkClock = QEI_SOURCE_SCLK;
    /* ��ʼ�� QEI�����˲������ʹ�� */
    qeiInitStruct->m_DigitalFilterEn = FALSE;
    /* ��ʼ�� QEI�����˲���ʱ�ӷ�Ƶ */
    qeiInitStruct->m_DigitalFilterPrescaler = QEI_DIGITAL_FILTER_CLK_DIV_1;
    /* ��ʼ�� A���B�����뽻��ʹ�� */
    qeiInitStruct->m_SwapABEn = FALSE;
    /* ��ʼ�� ��ʱ������ֵ */
    qeiInitStruct->m_Counter = 0;
    /* ��ʼ�� ��ʱ������ֵ */
    qeiInitStruct->m_Period = 0;
    /* ��ʼ�� ��ʱ��Ԥ��Ƶ */
    qeiInitStruct->m_Prescaler = 0;
}

/**
  * ����  ��ʼ��QEI��ʱ��������Ϣ�ṹ�塣
  * ����  qeiTimerInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
QEI_Timer_Struct_Init (QEI_TimerTypeDef* qeiTimerInitStruct)
{
    /* ��ʼ�� ��ʱ������ֵ */
    qeiTimerInitStruct->m_Counter = 0;
    /* ��ʼ�� ��ʱ������ֵ */
    qeiTimerInitStruct->m_Period = 0;
    /* ��ʼ�� ��ʱ��Ԥ��Ƶֵ */
    qeiTimerInitStruct->m_Prescaler = 0;
    /* ��ʼ�� ���������� */
    qeiTimerInitStruct->m_CounterMode = QEI_COUNT_DIRECT_REVERSE;
    /* ��ʼ�� ��ʱ��ʱ��Դѡ�� */
    qeiTimerInitStruct->m_ClockSource = QEI_TIMER_SOURCE_SCLK;
}
/**
  *   ##### �������������·(QEI)��ʼ������������� #####
  */


/**
  *   ##### �������������·(QEI)�������ú��� #####
  */
/**
  * ����  ���������������ӿ�ģʽ��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       PortMode: �����������ӿ�ģʽ��ȡֵ��ΧΪ:
  *                   QEI_ENCODER_OFF: �����������ӿ�/��ʱ���ر�
  *                   QEI_GENERAL_PURPOSE_COUNTER: ʹ�ܼ�������/ʱ�ӿ�ѡ��ͨ�ü�����ģʽ
  *                   QEI_X2_INDEX_PULSE_RESET: ʹ�������������ӿڣ�x2ģʽ�����������帴λλ�ü�����
  *                   QEI_X2_MATCH_RESET: ʹ�������������ӿڣ�x2ģʽ����ƥ��ʱ��λλ�ü�������PPX��
  *                   QEI_X4_INDEX_PULSE_RESET: ʹ�������������ӿڣ�x4ģʽ�����������帴λλ�ü�����
  *                   QEI_X4_MATCH_RESET: ʹ�������������ӿڣ�x4ģʽ����ƥ��ʱ��λλ�ü�������PPX��
  * ����  �ޡ�
  */
void
QEI_Encoder_Mode_Config (QEI_SFRmap* QEIx, uint32_t PortMode)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_PORT_MODE(PortMode));

    /*---------------- ����QEI_CTL0�Ĵ���QEIMODλ ----------------*/
    tmpreg = PortMode << QEI_CTL0_QEIMOD0_POS;
    QEIx->CTL0 = SFR_Config (QEIx->CTL0, ~QEI_CTL0_QEIMOD, tmpreg);
}

/**
  * ����  ����λ�ü�������λʹ�ܡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       NewState: λ�ü�������λʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
QEI_Location_Counter_Reset_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����QEI_CTL0�Ĵ���INDRESλ ----------------*/
    if (NewState != FALSE)
    {
        /* �������帴λλ�ü����� */
        SFR_SET_BIT_ASM(QEIx->CTL0, QEI_CTL0_INDRES_POS);
    }
    else
    {
        /* �������岻��λλ�ü����� */
        SFR_CLR_BIT_ASM(QEIx->CTL0, QEI_CTL0_INDRES_POS);
    }
}

/**
  * ����  ����QEI��ʱ��ʱ��Դ��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       TimerClock: QEI��ʱ��ʱ��Դ��ȡֵ��ΧΪ:
  *                     QEI_TIMER_SOURCE_SCLK: �ڲ�ʱ�ӣ�SCLK��
  *                     QEI_TIMER_SOURCE_EXTERNAL_PIN_QEA: ����QEAy���ŵ��ⲿʱ�ӣ��½��ش���������
  * ����  �ޡ�
  */
void
QEI_Timer_Source_Clock_Config (QEI_SFRmap* QEIx, uint32_t TimerClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_TIMER_SOURCE(TimerClock));

    /*---------------- ����QEI_CTL0�Ĵ���TXCSλ ----------------*/
    if (TimerClock != QEI_TIMER_SOURCE_SCLK)
    {
        /* ����QEAy���ŵ��ⲿʱ�� */
        SFR_SET_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXCS_POS);
    }
    else
    {
        /* �ڲ�ʱ�ӣ�SCLK�� */
        SFR_CLR_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXCS_POS);
    }
}

/**
  * ����  ��ȡQEI��������״̬��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  1:��������Ϊ�ߵ�ƽ��0:��������Ϊ�͵�ƽ��
  */
FlagStatus
QEI_Get_Index_Pin_State (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ��ȡQEI_CTL0�Ĵ���INDEXλ ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_INDEX)
    {
        /* ��������Ϊ�ߵ�ƽ */
        return SET;
    }
    else
    {
        /* ��������Ϊ�͵�ƽ */
        return RESET;
    }
}

/**
  * ����  ����λ�ü���������״̬���ʹ�ܡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       NewState: λ�ü���������״̬���ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
QEI_Location_Counter_Output_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����QEI_CTL0�Ĵ���TXDIROUTλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��λ�ü���������״̬��� */
        SFR_SET_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXDIROUT_POS);
    }
    else
    {
        /* ��ֹλ�ü���������״̬��� */
        SFR_CLR_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXDIROUT_POS);
    }
}

/**
  * ����  ����QEIλ�ü���������״̬��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       Direction: QEIλ�ü���������״̬��ȡֵ��ΧΪ:
  *                    QEI_COUNT_DIRECT_REVERSE: λ�ü���������Ϊ����-��
  *                    QEI_COUNT_DIRECT_FORWARD: λ�ü���������Ϊ����+��
  * ����  �ޡ�
  */
void
QEI_Location_Counter_Direction_Config (QEI_SFRmap* QEIx, uint32_t Direction)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_COUNT_DIRECT(Direction));

    /*---------------- ����QEI_CTL0�Ĵ���TXDIRλ ----------------*/
    if (Direction != QEI_COUNT_DIRECT_REVERSE)
    {
        /* λ�ü���������Ϊ����+�� */
        SFR_SET_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXDIR_POS);
    }
    else
    {
        /* λ�ü���������Ϊ����-�� */
        SFR_CLR_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXDIR_POS);
    }
}

/**
  * ����  ��ȡQEIλ�ü���������״̬��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  1:λ�ü���������Ϊ����+����0:λ�ü���������Ϊ����-����
  */
FlagStatus
QEI_Get_Location_Counter_Direction (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ��ȡQEI_CTL0�Ĵ���TXDIRλ ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_TXDIR)
    {
        /* λ�ü���������Ϊ����+�� */
        return SET;
    }
    else
    {
        /* λ�ü���������Ϊ����-�� */
        return RESET;
    }
}

/**
  * ����  ����QEI����ʱ�ӡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       PeripheralClock: QEI����ʱ�ӣ�ȡֵ��ΧΪ:
  *                          QEI_SOURCE_SCLK: ����ʱ��ΪSCLK
  *                          QEI_SOURCE_HFCLK: ����ʱ��ΪHFCLK
  *                          QEI_SOURCE_LFCLK: ����ʱ��ΪLFCLK
  * ����  �ޡ�
  */
void
QEI_Source_Clock_Config (QEI_SFRmap* QEIx, uint32_t PeripheralClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_SOURCE(PeripheralClock));

    /*---------------- ����QEI_CTL1�Ĵ���QEICLKλ ----------------*/
    QEIx->CTL1 = SFR_Config (QEIx->CTL1,
                            ~QEI_CTL1_QEICLK,
                            PeripheralClock);
}

/**
  * ����  ���������˲������ʹ�ܡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       NewState: �����˲������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
QEI_Digital_Filter_Output_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����QEI_CTL1�Ĵ���QEIOUTλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��QEAy/QEBy/INDXy�����ϵ������˲������ */
        SFR_SET_BIT_ASM(QEIx->CTL1, QEI_CTL1_QEIOUT_POS);
    }
    else
    {
        /* ��ֹ�����˲������ */
        SFR_CLR_BIT_ASM(QEIx->CTL1, QEI_CTL1_QEIOUT_POS);
    }
}

/**
  * ����  ����QEI�����˲���ʱ�ӷ�Ƶ��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       DigitalFilterClock: QEI�����˲���ʱ�ӷ�Ƶ����QEAy/QEBy/INDXy�˲�ʱ��
  *                           ���з�Ƶ��ȡֵ��ΧΪ:
  *                             QEI_DIGITAL_FILTER_CLK_DIV_1: 1:1��Ƶ
  *                             QEI_DIGITAL_FILTER_CLK_DIV_2: 1:2��Ƶ
  *                             QEI_DIGITAL_FILTER_CLK_DIV_4: 1:4��Ƶ
  *                             QEI_DIGITAL_FILTER_CLK_DIV_16: 1:16��Ƶ
  *                             QEI_DIGITAL_FILTER_CLK_DIV_32: 1:32��Ƶ
  *                             QEI_DIGITAL_FILTER_CLK_DIV_64: 1:64��Ƶ
  *                             QEI_DIGITAL_FILTER_CLK_DIV_128: 1:128��Ƶ
  *                             QEI_DIGITAL_FILTER_CLK_DIV_256: 1:256��Ƶ
  * ����  �ޡ�
  */
void
QEI_Digital_Filter_Clock_Config (QEI_SFRmap* QEIx, uint32_t DigitalFilterClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_DIGITAL_FILTER_CLK(DigitalFilterClock));

    /*---------------- ����QEI_CTL1�Ĵ���QEICKλ ----------------*/
    QEIx->CTL1 = SFR_Config (QEIx->CTL1,
                            ~QEI_CTL1_QEICK,
                            DigitalFilterClock);
}

/**
  * ����  ����A���B�����뽻��ʹ�ܡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       NewState: A���B�����뽻��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
QEI_Swap_Phase_AB_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����QEI_CTL1�Ĵ���SWPABλ ----------------*/
    if (NewState != FALSE)
    {
        /* A���B�����뽻�� */
        SFR_SET_BIT_ASM(QEIx->CTL1, QEI_CTL1_SWPAB_POS);
    }
    else
    {
        /* A���B�����벻���� */
        SFR_CLR_BIT_ASM(QEIx->CTL1, QEI_CTL1_SWPAB_POS);
    }
}

/**
  * ����  ����QEI 16λ��Tx��ʱ����
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       Counter: 16λ��Tx��ʱ����ȡֵΪ16λ��ֵ��
  * ����  �ޡ�
  */
void
QEI_Timer_Counter_Config (QEI_SFRmap* QEIx, uint16_t Counter)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ����Tx_CNT�Ĵ���TXCNTλ ----------------*/
    tmpreg = Counter << QEI_TXCNT_TXCNT0_POS;
    QEIx->CNT = SFR_Config (QEIx->CNT,
                            ~QEI_TXCNT_TXCNT,
                            (uint32_t)tmpreg);
}

/**
  * ����  ��ȡQEI 16λ��Tx��ʱ����
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  16λ��Tx��ʱ����ȡֵΪ16λ��ֵ��
  */
uint16_t
QEI_Get_Timer_Counter (QEI_SFRmap* QEIx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ��ȡTx_CNT�Ĵ���TXCNTλ ----------------*/
    tmpreg = QEIx->CNT;

    return (uint16_t)tmpreg;
}

/**
  * ����  ����QEI 16λ��Tx���ڼĴ�����
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       Period: 16λ��Tx���ڼĴ�����ȡֵΪ16λ��ֵ��
  * ����  �ޡ�
  */
void
QEI_Timer_Period_Config (QEI_SFRmap* QEIx, uint16_t Period)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ����Tx_PPX�Ĵ���PPXλ ----------------*/
    tmpreg = Period << QEI_PPX_PPX0_POS;
    QEIx->PPX = SFR_Config (QEIx->PPX,
                            ~QEI_PPX_PPX,
                            (uint32_t)tmpreg);
}

/**
  * ����  ��ȡQEI 16λ��Tx���ڼĴ�����
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  16λ��Tx���ڼĴ�����ȡֵΪ16λ��ֵ��
  */
uint16_t
QEI_Get_Timer_Period (QEI_SFRmap* QEIx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ��ȡTx_PPX�Ĵ���PPXλ ----------------*/
    tmpreg = QEIx->PPX;

    return (uint16_t)tmpreg;
}

/**
  * ����  ����QEI 16λ��TxԤ��Ƶ����
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       Prescaler: 16λ��TxԤ��Ƶ����ȡֵΪ16λ��ֵ��
  * ����  �ޡ�
  */
void
QEI_Timer_Prescaler_Config (QEI_SFRmap* QEIx, uint16_t Prescaler)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ����Tx_PRSC�Ĵ���TXPRSCλ ----------------*/
    tmpreg = Prescaler << QEI_TXPRSC_TXPRSC0_POS;
    QEIx->PRSC = SFR_Config (QEIx->PRSC,
                            ~QEI_TXPRSC_TXPRSC,
                            (uint32_t)tmpreg);
}
/**
  *   ##### �������������·(QEI)�������ú���������� #####
  */


/**
  *   ##### �������������·(QEI)�жϹ����� #####
  */
/**
  * ����  ��ȡQEI�жϱ�־��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       InterruptType: QEI�ж��¼���ȡֵΪ�����ṩ��һ��:
  *                        QEI_INT_COUNT_ERROR: ������Tx����ж��¼�
  *                        QEI_INT_PHASE_ERROR: QEIy��λ�����ж��¼�
  *                        QEI_INT_TX_OVERFLOW: QEIy���������ж��¼�
  * ����  1:������Ӧ�жϱ�־��0:δ������Ӧ�жϱ�־��
  */
FlagStatus
QEI_Get_INT_Flag (QEI_SFRmap* QEIx, uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_INT_ONE_EVENT(InterruptType));

    /*---------------- ��ȡQEI_CTL0�Ĵ�����Ӧλ ----------------*/
    tmpreg = InterruptType << QEI_CTL0_CNTERRIF_POS;
    if (QEIx->CTL0 & tmpreg)
    {
        /* ������Ӧ�жϱ�־ */
        return SET;
    }
    else
    {
        /* δ������Ӧ�жϱ�־ */
        return RESET;
    }
}

/**
  * ����  ���QEI�жϱ�־��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       InterruptType: QEI�ж��¼���ȡֵΪ�����һ���������:
  *                        QEI_INT_COUNT_ERROR: ������Tx����ж��¼�
  *                        QEI_INT_PHASE_ERROR: QEIy��λ�����ж��¼�
  *                        QEI_INT_TX_OVERFLOW: QEIy���������ж��¼�
  * ����  �ޡ�
  */
void
QEI_Clear_INT_Flag (QEI_SFRmap* QEIx, uint32_t InterruptType)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_INT_EVENT(InterruptType));

    /*---------------- ���QEI_DIER�Ĵ�����Ӧλ ----------------*/
    tmpreg1 = InterruptType << QEI_CTL0_CNTERRIF_POS;
    tmpreg = InterruptType << QEI_DIER_CNTERRIC_POS;
    QEIx->DIER |= tmpreg;
    while(QEIx->CTL0 & tmpreg1);
    QEIx->DIER &= ~tmpreg;
}

/**
  * ����  ����QEI�ж�ʹ�ܡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       InterruptType: QEI�ж��¼���ȡֵΪ�����һ���������:
  *                        QEI_INT_COUNT_ERROR: ������Tx����ж��¼�
  *                        QEI_INT_PHASE_ERROR: QEIy��λ�����ж��¼�
  *                        QEI_INT_TX_OVERFLOW: QEIy���������ж��¼�
  *       NewState: �ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
QEI_Set_INT_Enable (QEI_SFRmap* QEIx,
                    uint32_t InterruptType, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_INT_EVENT(InterruptType));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����QEI_DIER�Ĵ�����Ӧλ ----------------*/
    tmpreg = InterruptType << QEI_DIER_CNTERRIE_POS;
    if (NewState != FALSE)
    {
        /* ʹ���ж� */
        QEIx->DIER |= tmpreg;
    }
    else
    {
        /* ��ֹ�ж� */
        QEIx->DIER &= ~tmpreg;
    }
}

/**
  * ����  ��ȡ������Tx����жϱ�־��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  1:Tx�������ϻ������������0:Txδ�������ϻ������������
  */
FlagStatus
QEI_Get_Counter_Overflow_INT_Flag (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ��ȡQEI_CTL0�Ĵ���TXIFλ ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_TXIF)
    {
        /* Tx�������ϻ���������� */
        return SET;
    }
    else
    {
        /* Txδ�������ϻ���������� */
        return RESET;
    }
}

/**
  * ����  ��ȡQEI��λ�����жϱ�־��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  1:QEI��λ���ִ���0:QEI��λδ���ִ���
  */
FlagStatus
QEI_Get_Phase_Error_INT_Flag (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ��ȡQEI_CTL0�Ĵ���PHEIFλ ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_PHEIF)
    {
        /* QEI��λ���ִ��� */
        return SET;
    }
    else
    {
        /* QEI��λδ���ִ��� */
        return RESET;
    }
}

/**
  * ����  ��ȡQEI���������жϱ�־��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  1:QEI�������ִ���0:QEI����δ���ִ���
  */
FlagStatus
QEI_Get_Count_Error_INT_Flag (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- ��ȡQEI_CTL0�Ĵ���PHEIFλ ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_CNTERRIF)
    {
        /* QEI�������ִ��� */
        return SET;
    }
    else
    {
        /* QEI����δ���ִ��� */
        return RESET;
    }
}

/**
  * ����  ���QEI Tx����жϱ�־��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  �ޡ�
  */
void
QEI_Clear_Timer_Overflow_INT_Flag (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*-------------------- ����QEI_DIER�Ĵ���TXICλ --------------------*/
    SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_TXIC_POS);
    while(QEIx->CTL0 & QEI_CTL0_TXIF);
    SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_TXIC_POS);
}

/**
  * ����  ���QEI��λ�����жϱ�־��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  �ޡ�
  */
void
QEI_Clear_Phase_Error_INT_Flag (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*-------------------- ����QEI_DIER�Ĵ���PHEICλ --------------------*/
    SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_PHEIC_POS);
    while(QEIx->CTL0 & QEI_CTL0_PHEIF);
    SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_PHEIC_POS);
}

/**
  * ����  ���QEI���������жϱ�־��
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  * ����  �ޡ�
  */
void
QEI_Clear_Counter_Error_INT_Flag (QEI_SFRmap* QEIx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*-------------------- ����QEI_DIER�Ĵ���CNTERRICλ --------------------*/
    SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_CNTERRIC_POS);
    while(QEIx->CTL0 & QEI_CTL0_CNTERRIF);
    SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_CNTERRIC_POS);
}

/**
  * ����  ����QEI Tx����ж�ʹ�ܡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       NewState: Tx����ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
QEI_Timer_Overflow_INT_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����QEI_DIER�Ĵ���TXIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܼ�����Tx����ж� */
        SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_TXIE_POS);
    }
    else
    {
        /* ��ֹ������Tx����ж� */
        SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_TXIE_POS);
    }
}

/**
  * ����  ����QEI��λ�����ж�ʹ�ܡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       NewState:��λ�����ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
QEI_Phase_Error_INT_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����QEI_DIER�Ĵ���PHEIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��QEIy��λ�����ж� */
        SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_PHEIE_POS);
    }
    else
    {
        /* ��ֹQEIy��λ�����ж� */
        SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_PHEIE_POS);
    }
}

/**
  * ����  ����QEI���������ж�ʹ�ܡ�
  * ����  QEIx: ָ��QEI�ڴ�ṹ��ָ�룬ȡֵΪQEI0_SFR��QEI1_SFR��
  *       NewState:���������ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
QEI_Counter_Error_INT_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����QEI_DIER�Ĵ���CNTERRIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��QEIy���������ж� */
        SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_CNTERRIE_POS);
    }
    else
    {
        /* ��ֹQEIy���������ж� */
        SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_CNTERRIE_POS);
    }
}
/**
  *   ##### �������������·(QEI)�жϹ������������ #####
  */
#endif //KF32A_Periph_qei

/**
  ******************************************************************************
  * �ļ���  kf32a_basic_gptim.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��ͨ�ö�ʱ�����蹦�ܺ�����������
  *          + ͨ�ö�ʱ��(GPTIM)��ʼ�������ú�������
  *          + ͨ�ö�ʱ��(GPTIM)�жϼ���־����������
  *          + ͨ�ò�׽/�Ƚ�/PWM����(CCP)��׽���ܺ�������
  *          + ͨ�ò�׽/�Ƚ�/PWM����(CCP)�жϼ���־����������
  *
  *********************************************************************
  */

#include "kf32a_basic_tim.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* ��ʱ��˽�ж��� ----------------------------------------------------*/
/* GPTIMx_CTL1�Ĵ����������� */
#define GPTIM_CTL1_INIT_MASK            (GPTIM_CTL1_TXCLK \
                                       | GPTIM_CTL1_TXCMS \
                                       | GPTIM_CTL1_TXSY \
                                       | GPTIM_CTL1_TXCS)

/* GPTIMx_CTL2�Ĵ����������� */
#define GPTIM_CTL2_INIT_MASK            (GPTIM_CTL2_TXMMS \
                                       | GPTIM_CTL2_TXSMS \
                                       | GPTIM_CTL2_TXMSSYNC)

/* CCPXCTL3�Ĵ�����׽ģʽ�������� */
#define CCP_CTL3_CAP_INIT_MASK          (CCP_CTL3_PXPWMI | CCP_CTL3_PXXORM)

/* CCPXCTL3�Ĵ���PWM�������� */
#define CCP_CTL3_PWM_INIT_MASK          (CCP_CTL3_PXSPM | CCP_CTL3_PXSPMST)

/* CCPXCY�Ĵ���ƫ�Ƶ�ַ */
#define CCP_CY_OFFSET                   ((uint32_t)0x20)

/* CCPXRY�Ĵ���ƫ�Ƶ�ַ */
#define CCP_RY_OFFSET                   ((uint32_t)0x44)

/**
  *   ##### ͨ�ö�ʱ��(GPTIM)��ʼ�������ú������� #####
  */
/**
  * ����  ͨ�ö�ʱ��(GPTIM)���á�
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       gptimInitStruct: ͨ�ö�ʱ��������Ϣ�ṹ��ָ�롣
  * ����  ��
  */
void GPTIM_Configuration(GPTIM_SFRmap* GPTIMx,
                    GPTIM_InitTypeDef* gptimInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_COUNTER_MODE(gptimInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_GPTIM_CLOCK_CONFIG(gptimInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_GPTIM_WORK_MODE(gptimInitStruct->m_WorkMode));
    CHECK_RESTRICTION(CHECK_GPTIM_MASTER_MODE(gptimInitStruct->m_MasterMode));
    CHECK_RESTRICTION(CHECK_GPTIM_SLAVE_MODE(gptimInitStruct->m_SlaveMode));
    CHECK_RESTRICTION(CHECK_GPTIM_SYNC_MODE(gptimInitStruct->m_EXPulseSync));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(gptimInitStruct->m_MasterSlaveSync));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /*---------------- ����TxCNT�Ĵ��� ----------------*/
        GPTIMx->CNT = gptimInitStruct->m_Counter;

        /*--------------- ����PPx���ڼĴ��� ---------------*/
        GPTIMx->PPX = gptimInitStruct->m_Period;

        /*-------------- ����TxԤ��Ƶ�Ĵ��� ---------------*/
        GPTIMx->PRSC = gptimInitStruct->m_Prescaler;
    }
    else
    {
        /*---------------- ����TxCNT�Ĵ��� ----------------*/
        GPTIMx->CNT = (uint16_t)gptimInitStruct->m_Counter;

        /*--------------- ����PPx���ڼĴ��� ---------------*/
        GPTIMx->PPX = (uint16_t)gptimInitStruct->m_Period;

        /*-------------- ����TxԤ��Ƶ�Ĵ��� ---------------*/
        GPTIMx->PRSC = (uint16_t)gptimInitStruct->m_Prescaler;
    }

    /*------------------ ����TXCTL1�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_CounterMode������TXCMSλ�� */
    /* ���ݽṹ���Աm_Clock������TXCLKλ�� */
    /* ���ݽṹ���Աm_WorkMode������TXCSλ�� */
    /* ���ݽṹ���Աm_EXPulseSync������TXSYλ�� */
    tmpreg = ((uint32_t)gptimInitStruct->m_CounterMode) | \
             ((uint32_t)gptimInitStruct->m_Clock) | \
             ((uint32_t)gptimInitStruct->m_WorkMode) | \
             ((uint32_t)gptimInitStruct->m_EXPulseSync);
    GPTIMx->CTL1 = SFR_Config (GPTIMx->CTL1,
                          ~GPTIM_CTL1_INIT_MASK,
                          tmpreg);

    /*-------------------- ����TXCTL2�Ĵ��� --------------------*/
    /* ���ݽṹ���Աm_MasterMode������TXMMSλ�� */
    /* ���ݽṹ���Աm_SlaveMode������TXSMSλ�� */
    /* ���ݽṹ���Աm_MasterSlaveSync������TXMSSYNCλ�� */
    tmpreg = ((uint32_t)gptimInitStruct->m_MasterMode) | \
             ((uint32_t)gptimInitStruct->m_SlaveMode) | \
             ((uint32_t)gptimInitStruct->m_MasterSlaveSync << GPTIM_CTL2_TXMSSYNC_POS);
    GPTIMx->CTL2 = SFR_Config (GPTIMx->CTL2,
                          ~GPTIM_CTL2_INIT_MASK,
                          tmpreg);
}

/**
  * ����  ��ʼ��ͨ�ö�ʱ��������Ϣ�ṹ�塣
  * ����  gptimInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  ��
  */
void
GPTIM_Struct_Init (GPTIM_InitTypeDef* gptimInitStruct)
{
    /* ����Tx����ֵ */
    gptimInitStruct->m_Counter = 0x00000000;
    /* ����Tx����ֵ */
    gptimInitStruct->m_Period = 0xFFFFFFFF;
    /* ����TxԤ��Ƶֵ */
    gptimInitStruct->m_Prescaler = 0x00000000;
    /* ����Tx����ģʽ */
    gptimInitStruct->m_CounterMode = GPTIM_COUNT_UP_OF;
    /* ����Tx����ʱ�� */
    gptimInitStruct->m_Clock = GPTIM_SCLK;
    /* ���ö�ʱ/����ģʽѡ�� */
    gptimInitStruct->m_WorkMode = GPTIM_TIMER_MODE;
    /* ������ģʽѡ�� */
    gptimInitStruct->m_MasterMode = GPTIM_MASTER_UR_SIGNAL;
    /* ���ô�ģʽѡ�� */
    gptimInitStruct->m_SlaveMode = GPTIM_SLAVE_FORBIDDEN_MODE;
    /* ����Tx����ģʽ�ⲿ������������ͬ������ */
    gptimInitStruct->m_EXPulseSync = GPTIM_EX_SYNC_MODE;
    /* ��������ģʽͬ������ */
    gptimInitStruct->m_MasterSlaveSync = FALSE;
}

/**
  * ����  ��ʱ����������ʹ�ܡ�
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       NewState: ��ʱ��ʹ�ܿ��ƣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Cmd (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL1�Ĵ���TXENλ --------------------*/
    if (NewState != FALSE)
    {

        SFR_SET_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXEN_POS);
    }
    else
    {

        SFR_CLR_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXEN_POS);
    }
}

/**
  * ����  ���¶�ʱ������ֵ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       Counter: �µļ���ֵ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
GPTIM_Set_Counter (GPTIM_SFRmap* GPTIMx, uint32_t Counter)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /*---------------- ����TxCNT�Ĵ��� ----------------*/
        GPTIMx->CNT = Counter;
    }
    else
    {
        /*---------------- ����TxCNT�Ĵ��� ----------------*/
        GPTIMx->CNT = (uint16_t)Counter;
    }
}

/**
  * ����  ���¶�ʱ������ֵ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       Period: �µ�����ֵ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
GPTIM_Set_Period (GPTIM_SFRmap* GPTIMx, uint32_t Period)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /*---------------- ����TxPPX�Ĵ��� ----------------*/
        GPTIMx->PPX = Period;
    }
    else
    {
        /*---------------- ����TxPPX�Ĵ��� ----------------*/
        GPTIMx->PPX = (uint16_t)Period;
    }
}

/**
  * ����  ���¶�ʱ��Ԥ��Ƶֵ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       Prescaler: �µ�Ԥ��Ƶ��ȡֵ32λ��16λ���ݡ�
  * ����  ��
  */
void
GPTIM_Set_Prescaler (GPTIM_SFRmap* GPTIMx, uint32_t Prescaler)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /*---------------- ����TxPRSC�Ĵ��� ----------------*/
        GPTIMx->PRSC = Prescaler;
    }
    else
    {
        /*---------------- ����TxPRSC�Ĵ��� ----------------*/
        GPTIMx->PRSC = (uint16_t)Prescaler;
    }
}

/**
  * ����  ���¶�ʱ������ģʽ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       CounterMode: �µļ���ģʽ��
  *                    ȡֵ��ΧΪ��
  *                      GPTIM_COUNT_DOWN_UF: ���¼���,��������жϱ�־
  *                      GPTIM_COUNT_UP_OF: ���ϼ���,��������жϱ�־
  *                      GPTIM_COUNT_UP_DOWN_OF: ����-���¼���,��������жϱ�־
  *                      GPTIM_COUNT_UP_DOWN_UF: ����-���¼���,��������жϱ�־
  *                      GPTIM_COUNT_UP_DOWN_OUF: ����-���¼���,�������������жϱ�־
  * ����  ��
  */
void
GPTIM_Counter_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t CounterMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_COUNTER_MODE(CounterMode));

    /*-------------------- ����TXCTL1�Ĵ���TXCMSλ --------------------*/
    GPTIMx->CTL1 = SFR_Config (GPTIMx->CTL1,
                          ~GPTIM_CTL1_TXCMS,
                          CounterMode);
}

/**
  * ����  ���¶�ʱ������ʱ�ӡ�
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       NewClock: �µĹ���ʱ�ӣ�
  *                 ȡֵ��ΧΪ��
  *                   GPTIM_SCLK: ѡ��SCLKʱ��
  *                   GPTIM_HFCLK: ѡ��HFCLKʱ��
  *                   GPTIM_LFCLK: ѡ��LFCLKʱ��
  * ����  ��
  */
void
GPTIM_Clock_Config (GPTIM_SFRmap* GPTIMx, uint32_t NewClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_CLOCK_CONFIG(NewClock));

    /*-------------------- ����TXCTL1�Ĵ���TXCLKλ --------------------*/
    GPTIMx->CTL1 = SFR_Config (GPTIMx->CTL1,
                          ~GPTIM_CTL1_TXCLK,
                          NewClock);
}

/**
  * ����  ���¶�ʱ������ģʽ�ⲿ������������ͬ������λ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       NewClock: �µĶ�ʱ������ģʽ�ⲿ������������ͬ������λʹ��״̬��
  *                 ȡֵ��ΧΪ��
  *                   GPTIM_EX_SYNC_MODE: ���ⲿ������������ͬ��
  *                   GPTIM_NO_SYNC_MODE: �����ⲿ������������ͬ��
  * ����  ��
  */
void
GPTIM_External_Pulse_Sync_Config (GPTIM_SFRmap* GPTIMx, uint32_t PulseSync)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_SYNC_MODE(PulseSync));

    /*-------------------- ����TXCTL1�Ĵ���TXSYλ --------------------*/
    if (PulseSync != GPTIM_EX_SYNC_MODE)
    {
        /* �����ⲿ������������ͬ�� */
        SFR_SET_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXSY_POS);
    }
    else
    {
        /* ���ⲿ������������ͬ�� */
        SFR_CLR_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXSY_POS);
    }
}

/**
  * ����  ���¶�ʱ/����ģʽѡ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       NewState: �µĶ�ʱ/����ģʽ��
  *                 ȡֵ��ΧΪ��
  *                   GPTIM_TIMER_MODE: ��ʱģʽ
  *                   GPTIM_COUNTER_MODE: ����ģʽ
  * ����  ��
  */
void
GPTIM_Work_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_WORK_MODE(NewState));

    /*-------------------- ����TXCTL1�Ĵ���TXCSλ --------------------*/
    if (NewState != GPTIM_TIMER_MODE)
    {
        /* ����ģʽ */
        SFR_SET_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXCS_POS);
    }
    else
    {
        /* ��ʱģʽ */
        SFR_CLR_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXCS_POS);
    }
}

/**
  * ����  �����������¿���λ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       NewState: ��������ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Updata_Immediately_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL2�Ĵ���TXURλ --------------------*/
    if (NewState != FALSE)
    {
        /* ��������ʹ�� */
        SFR_SET_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUR_POS);
    }
    else
    {
        /* ��������δʹ�� */
        SFR_CLR_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUR_POS);
    }
}

/**
  * ����  ��������ģʽͬ��λ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       NewState: ����ģʽͬ��λ״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Master_Slave_Snyc_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL2�Ĵ���TXMSSYNCλ --------------------*/
    if (NewState != FALSE)
    {
        /* ��������ʹ�� */
        SFR_SET_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXMSSYNC_POS);
    }
    else
    {
        /* ��������δʹ�� */
        SFR_CLR_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXMSSYNC_POS);
    }
}

/**
  * ����  ���ô���ѡ��λ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       TriggerSelect: ����ѡ��
  *                      ȡֵ��ΧΪ��
  *                         GPTIM_TRIGGER_T1
  *                         GPTIM_TRIGGER_T2
  *                         GPTIM_TRIGGER_T3
  *                         GPTIM_TRIGGER_T4
  *                         GPTIM_TRIGGER_T5
  *                         GPTIM_TRIGGER_T9
  *                         GPTIM_TRIGGER_T14
  *                         GPTIM_TRIGGER_T15
  *                         GPTIM_TRIGGER_T18
  *                         GPTIM_TRIGGER_T19
  *                         GPTIM_TRIGGER_T20
  *                         GPTIM_TRIGGER_T21
  *                         GPTIM_TRIGGER_TXCK
  *                         GPTIM_TRIGGER_CCPXCH1
  *                         GPTIM_TRIGGER_CCPXCH2
  *                         GPTIM_TRIGGER_CCPXCH3
  * ����  ��
  */
void
GPTIM_Trigger_Select_Config (GPTIM_SFRmap* GPTIMx,
                    uint32_t TriggerSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_TRIGGER_MODE(TriggerSelect));

    /*-------------------- ����TXCTL2�Ĵ���TXTSλ --------------------*/
    GPTIMx->CTL2 = SFR_Config (GPTIMx->CTL2,
                          ~GPTIM_CTL2_TXTS,
                          TriggerSelect);
}

/**
  * ����  ���ô�ģʽѡ��λ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       SlaveSelect: ��ģʽѡ��
  *                    ȡֵ��ΧΪ��
  *                      GPTIM_SLAVE_FORBIDDEN_MODE: ��ģʽ��ֹ
  *                      GPTIM_SLAVE_TRIGGER_MODE: ����ģʽ
  *                      GPTIM_SLAVE_GATED_MODE: �ſ�ģʽ
  *                      GPTIM_SLAVE_RESET_MODE: ��λģʽ
  *                      GPTIM_SLAVE_COUNTER_MODE: ����ģʽ2
  * ����  ��
  */
void
GPTIM_Slave_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t SlaveMode)
{
   /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_SLAVE_MODE(SlaveMode));

    /*-------------------- ����TXCTL2�Ĵ���TXSMSλ --------------------*/
    GPTIMx->CTL2 = SFR_Config (GPTIMx->CTL2,
                          ~GPTIM_CTL2_TXSMS,
                          SlaveMode);
}

/**
  * ����  ������ģʽѡ��λ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       MasterMode: ��ģʽѡ��
  *                   ȡֵ��ΧΪ��
  *                     GPTIM_MASTER_UR_SIGNAL: URλ��Ϊ����
  *                     GPTIM_MASTER_EN_SIGNAL: TXEN��Ϊ����
  *                     GPTIM_MASTER_TXIF_SIGNAL: TXIF��Ϊ����
  *                     GPTIM_MASTER_CCPXCH1IF_SIGNAL: CCPxCH1IF������Ϊ����
  *                     GPTIM_MASTER_CCPXCH1_SIGNAL: CCPxCH1 ��Ϊ����
  *                     GPTIM_MASTER_CCPXCH2_SIGNAL: CCPxCH2 ��Ϊ����
  *                     GPTIM_MASTER_CCPXCH3_SIGNAL: CCPxCH3 ��Ϊ����
  *                     GPTIM_MASTER_CCPXCH4_SIGNAL: CCPxCH4 ��Ϊ����
  * ����  ��
  */
void
GPTIM_Master_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t MasterMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_MASTER_MODE(MasterMode));

    /*-------------------- ����TXCTL2�Ĵ���TXMMSλ --------------------*/
    GPTIMx->CTL2 = SFR_Config (GPTIMx->CTL2,
                          ~GPTIM_CTL2_TXMMS,
                          MasterMode);
}

/**
  * ����  ���������ظ����¼�����λ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       NewState: �����¼�����״̬��
  *                 ȡֵ��ΧΪ��
  *                   TRUE: ��������������
  *                   FALSE: ÿ���ڸ���
  * ����  ��
  */
void
GPTIM_Updata_Rising_Edge_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL2�Ĵ���TXUDEVTλ --------------------*/
    if (NewState != FALSE)
    {
        /* �������������� */
        SFR_SET_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUDEVT_POS);
    }
    else
    {
        /* ÿ���ڸ��� */
        SFR_CLR_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUDEVT_POS);
    }
}

/**
  * ����  ���ø���ʹ�ܡ�
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *       NewState: ����ʹ��״̬��
  *                 ȡֵ��ΧΪ��
  *                   TRUE: �������
  *                   FALSE: ��ֹ����
  * ����  ��
  */
void
GPTIM_Updata_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL2�Ĵ���TXUDENλ --------------------*/
    if (NewState != FALSE)
    {
        /* �������������� */
        SFR_SET_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUDEN_POS);
    }
    else
    {
        /* ÿ���ڸ��� */
        SFR_CLR_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUDEN_POS);
    }
}

/**
  * ����  �������¼���DMA��������
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: �����¼���DMA����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Trigger_DMA_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL3�Ĵ���TXTDEλ --------------------*/
    if (NewState != FALSE)
    {
        /* �������¼���DMA���� */
        SFR_SET_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXTDE_POS);
    }
    else
    {
        /* ��ֹ�����¼���DMA���� */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXTDE_POS);
    }
}

/**
  * ����  ���ø����¼���DMA����ʹ�ܡ�
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: �����¼���DMA����
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Updata_DMA_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL3�Ĵ���TXUDEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ��������¼���DMA���� */
        SFR_SET_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXUDE_POS);
    }
    else
    {
        /* ��ֹ�����¼���DMA���� */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXUDE_POS);
    }
}

/**
  * ����  ����Tx�����¼��ж�ʹ�ܡ�
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: Tx�����¼��жϣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Updata_INT_Enable (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL3�Ĵ���TXUIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tx�����¼������ж� */
        SFR_SET_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXUIE_POS);
    }
    else
    {
        /* ��ֹTx�����¼������ж� */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXUIE_POS);
    }
}

/**
  * ����  ����Tx�����¼��ж�ʹ�ܡ�
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: Tx�����¼��жϣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Trigger_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL3�Ĵ���TXTIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tx�����¼������ж� */
        SFR_SET_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXTIE_POS);
    }
    else
    {
        /* ��ֹTx�����¼������ж� */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXTIE_POS);
    }
}

/**
  * ����  ���������¼�����λ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: ��ʱ��ʹ�ܿ���״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Generate_Trigger_Config (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXEGIF�Ĵ���TXTRGλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����������Ӧ */
        SFR_SET_BIT_ASM(GPTIMx->CCPXEGIF, CCP_EGIF_TXTRG_POS);
    }
    else
    {
        /* �޶��� */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXEGIF, CCP_EGIF_TXTRG_POS);
    }
}

/**
  * ����  ��TX��������
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  * ����  TX��������0�����£�1�����ϡ�
  */
DIRStatus
GPTIM_Get_Direction (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ��ȡTXCTL1�Ĵ���TXDIRλ --------------------*/
    if (GPTIMx->CTL1 & GPTIM_CTL1_TXDIR)
    {
        /* ��ǰ��������Ϊ���ϼ��� */
        return DIR_UP;
    }
    else
    {
        /* ��ǰ��������Ϊ���¼��� */
        return DIR_DOWN;
    }
}

/**
  * ����  ����ʱ������ֵ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  * ����  Tx����ֵ��32λ
  */
uint32_t
GPTIM_Get_Counter (GPTIM_SFRmap* GPTIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /* ����ʱ������ֵTxCNT */
        tmpreg = GPTIMx->CNT;
    }
    else
    {
        /* ����ʱ������ֵTxCNT */
        tmpreg = (uint16_t)GPTIMx->CNT;
    }
    return tmpreg;
}

/**
  * ����  ����ʱ������ֵ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  * ����  Tx����ֵ��32λ
  */
uint32_t
GPTIM_Get_Period (GPTIM_SFRmap* GPTIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /* ����ʱ������ֵTxPPX */
        tmpreg = GPTIMx->PPX;
    }
    else
    {
        /* ����ʱ������ֵTxPPX */
        tmpreg = (uint16_t)GPTIMx->PPX;
    }
    return tmpreg;
}

/**
  * ����  ����ʱ��Ԥ��Ƶֵ��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  * ����  TxԤ��Ƶֵ��32λ
  */
uint32_t
GPTIM_Get_Prescaler (GPTIM_SFRmap* GPTIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /* ����ʱ��Ԥ��ƵֵTxPRSC */
        tmpreg = GPTIMx->PRSC;
    }
    else
    {
        /* ����ʱ��Ԥ��ƵֵTxPRSC */
        tmpreg = (uint16_t)GPTIMx->PRSC;
    }
    return tmpreg;
}
/**
  *   ##### ͨ�ö�ʱ��(GPTIM)��ʼ�������ú���������� #####
  */


/**
  *   ##### ͨ�ö�ʱ��(GPTIM)�жϼ���־���������� #####
  */
/**
  * ����  ��ȡTx��������ж�ʹ�ܡ�
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: Tx��������жϣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
GPTIM_Overflow_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ��ȡTXCTL1�Ĵ���TXIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tx������������ж� */
        SFR_SET_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXIE_POS);
    }
    else
    {
        /* ��ֹTx������������ж� */
        SFR_CLR_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXIE_POS);
    }
}

/**
  * ����  ���Tx����жϱ�־��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  * ����  ��
  */
void
GPTIM_Clear_Overflow_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ����CCPXSRIC�Ĵ���TXICλ --------------------*/
    SFR_SET_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXIC_POS);
    while((GPTIMx->CTL1 & GPTIM_CTL1_TXIF)>>GPTIM_CTL1_TXIF_POS);
    SFR_CLR_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXIC_POS);
}

/**
  * ����  ���Tx�����¼��жϱ�־��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  * ����  ��
  * ����˵��������ñ�־λ����ʹ�ܶ�Ӧ��ʱ��������½��У�����ᵼ�����ʧ�ܡ�
  */
void
GPTIM_Clear_Updata_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ����CCPXSRIC�Ĵ���TXUICλ --------------------*/
    SFR_SET_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXUIC_POS);
    while((GPTIMx->CCPXEGIF & CCP_EGIF_TXUIF)>>CCP_EGIF_TXUIF_POS);
    SFR_CLR_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXUIC_POS);
}

/**
  * ����  ���Tx�����¼��жϱ�־��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  * ����  ��
  */
void
GPTIM_Clear_Trigger_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ����CCPXSRIC�Ĵ���TXTICλ --------------------*/
    SFR_SET_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXTIC_POS);
    while((GPTIMx->CCPXEGIF & CCP_EGIF_TXTIF)>>CCP_EGIF_TXTIF_POS);
    SFR_CLR_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXTIC_POS);
}

/**
  * ����  ��ȡTx��������жϱ�־��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
GPTIM_Get_Overflow_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ��ȡTXCTL1�Ĵ���TXIFλ --------------------*/
    if (GPTIMx->CTL1 & GPTIM_CTL1_TXIF)
    {
        /* ����Tx��������ж� */
        return SET;
    }
    else
    {
        /* δ����Tx��������ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡTx�����¼��жϱ�־��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
GPTIM_Get_Updata_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ��ȡCCPXEGIF�Ĵ���TXUIFλ --------------------*/
    if (GPTIMx->CCPXEGIF & CCP_EGIF_TXUIF)
    {
        /* ����Tx�����¼��ж� */
        return SET;
    }
    else
    {
        /* δ����Tx�����¼��ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡTx�����¼��жϱ�־��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
GPTIM_Get_Trigger_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ��ȡCCPXEGIF�Ĵ���TXTIFλ --------------------*/
    if (GPTIMx->CCPXEGIF & CCP_EGIF_TXTIF)
    {
        /* ����Tx�����¼��ж� */
        return SET;
    }
    else
    {
        /* δ����Tx�����¼��ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡTx�����¼�����DMA�жϱ�־��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������жϣ�DMA ��Ӧ���λ��Ӳ���Զ�����
  */
FlagStatus
GPTIM_Get_Updata_DMA_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ����CCPXDF�Ĵ���TXUDFλ --------------------*/
    if (GPTIMx->CCPXDF & CCP_DF_TXUDF)
    {
        /* ���������¼�����DMA�ж� */
        return SET;
    }
    else
    {
        /* δ���������¼�����DMA�ж� */
        return RESET;
    }
}

/**
  * ����  ���ô����¼�����DMA�жϱ�־��
  * ����  GPTIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������жϣ�DMA ��Ӧ���λ��Ӳ���Զ�����
  */
FlagStatus
GPTIM_Get_Trigger_DMA_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- ����CCPXDF�Ĵ���TXTDFλ --------------------*/
    if (GPTIMx->CCPXDF & CCP_DF_TXTDF)
    {
        /* ���������¼�����DMA�ж� */
        return SET;
    }
    else
    {
        /* δ���������¼�����DMA�ж� */
        return RESET;
    }
}
/**
  *   ##### ͨ�ö�ʱ��(GPTIM)�жϼ���־������������� #####
  */


/**
  *   ##### ͨ�ò�׽/�Ƚ�/PWM����(CCP)��׽���ܺ������� #####
  */
/**
  * ����  ͨ�ñȽ���(CCP)���á�
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       ccpInitStruct: ͨ�ñȽ���(CCP)������Ϣ�ṹ��ָ�롣
  * ����  ��
  */
void
CCP_Compare_Configuration(CCP_SFRmap* CCPx,
                    CCP_CompareInitTypeDef* ccpInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(ccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_CCP_CMP_MODE(ccpInitStruct->m_CompareMode));

    /*------------------ ����CCPXCTL1�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel��m_CompareMode������CCPxCHyMλ�� */
    tmpreg = 4 * ccpInitStruct->m_Channel;
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << tmpreg),
                          (uint32_t)ccpInitStruct->m_CompareMode << tmpreg);

    /*------------------ ����CCPXRy�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_CompareValue������CCPXRyλ�� */
    /* ��ȡCCPXRY�Ĵ�����ַ */
    tmpreg1 = (uint32_t)CCPx;
    tmpreg1 = tmpreg1 + CCP_RY_OFFSET + tmpreg;
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        *(volatile uint32_t*)tmpreg1
            = ccpInitStruct->m_CompareValue;
    }
    else
    {
        *(volatile uint32_t*)tmpreg1
            = (uint16_t)ccpInitStruct->m_CompareValue;
    }
}

/**
  * ����  ��ʼ��ͨ�ñȽ���(CCP)������Ϣ�ṹ�塣
  * ����  ccpInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  ��
  */
void
CCP_Compare_Struct_Init (CCP_CompareInitTypeDef* ccpInitStruct)
{
    /* ���ñȽ���ͨ�� */
    ccpInitStruct->m_Channel = CCP_CHANNEL_1;
    /* ����ͨ����ģʽѡ��λ */
    ccpInitStruct->m_CompareMode = CCP_MODE_RST;
    /* ���ñȽ�ֵ */
    ccpInitStruct->m_CompareValue = 0x00000000;
}

/**
  * ����  ͨ�ò�׽ģ��(CCP)���á�
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       ccpInitStruct: ͨ�ò�׽ģ��(CCP)������Ϣ�ṹ��ָ�롣
  * ����  ��
  */
void
CCP_Capture_Configuration (CCP_SFRmap* CCPx,
                    CCP_CaptureInitTypeDef* ccpInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(ccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_CCP_CAP_MODE(ccpInitStruct->m_CaptureMode));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_PWMInput));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_XORMode));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_ChannelCompare4));

    /*------------------ ����CCPXCTL1�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel��m_CaptureMode������CCPxCHyMλ�� */
    tmpreg = 4 * ccpInitStruct->m_Channel;
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << tmpreg),
                          (uint32_t)ccpInitStruct->m_CaptureMode << tmpreg);

    /*------------------ ����CCPXCTL2�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_ChannelCompare4������PxOC4CEλ�� */
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~CCP_CTL2_PXOC4CE,
                          ccpInitStruct->m_ChannelCompare4);

    /*------------------ ����CCPXCTL3�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_PWMInput������PxPWMIλ�� */
    /* ���ݽṹ���Աm_XORMode������PxXORMλ�� */
    tmpreg = (ccpInitStruct->m_PWMInput << CCP_CTL3_PXPWMI_POS) \
             | (ccpInitStruct->m_XORMode << CCP_CTL3_PXXORM_POS);
    CCPx->CCPXCTL3 = SFR_Config (CCPx->CCPXCTL3,
                          ~CCP_CTL3_CAP_INIT_MASK,
                          tmpreg);
}

/**
  * ����  ��ʼ��ͨ�ò�׽ģ��(CCP)������Ϣ�ṹ�塣
  * ����  ccpInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  ��
  */
void
CCP_Capture_Struct_Init (CCP_CaptureInitTypeDef* ccpInitStruct)
{
    /* ���ò�׽ͨ�� */
    ccpInitStruct->m_Channel = CCP_CHANNEL_1;
    /* ����ͨ����ģʽѡ��λ */
    ccpInitStruct->m_CaptureMode = CCP_MODE_RST;
    /* ����PWM����ģʽ */
    ccpInitStruct->m_PWMInput = FALSE;
    /* �����������ģʽ */
    ccpInitStruct->m_XORMode = FALSE;
    /* ���ñȽ��������������׽ */
    ccpInitStruct->m_ChannelCompare4 = FALSE;
}

/**
  * ����  ͨ��PWMģ��(CCP)���á�
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       ccpInitStruct: ͨ��PWMģ��(CCP)������Ϣ�ṹ��ָ�롣
  * ����  ��
  */
void
CCP_PWM_Configuration (CCP_SFRmap* CCPx,
                    CCP_PWMInitTypeDef* ccpInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(ccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_CCP_PWM_MODE(ccpInitStruct->m_PwmMode));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL_OUTPUT(ccpInitStruct->m_OutputCtl));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_SinglePWM));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_CloseTimer));

    /*------------------ ����CCPXCTL1�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel��m_PwmMode������CCPxCHyMλ�� */
    tmpreg = 4 * ccpInitStruct->m_Channel;
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << tmpreg),
                          (uint32_t)ccpInitStruct->m_PwmMode << tmpreg);

    /*------------------ ����CCPXRy�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel����ȡCCPXRY�Ĵ�����ַ */
    tmpreg1 = (uint32_t)CCPx;
    tmpreg1 = tmpreg1 + CCP_RY_OFFSET + tmpreg;
    /* ���ݽṹ���Աm_DutyRatio������CCPXRyλ�� */
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        *(volatile uint32_t*)tmpreg1
            = ccpInitStruct->m_DutyRatio;
    }
    else
    {
        *(volatile uint32_t*)tmpreg1
            = (uint16_t)ccpInitStruct->m_DutyRatio;
    }

    /*------------------ ����CCPXCTL2�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel��m_OutputCtl������PxOCyλ�� */
    tmpreg = 2 * ccpInitStruct->m_Channel;
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~(CCP_CTL2_PXOC1 << tmpreg),
                          ccpInitStruct->m_OutputCtl << tmpreg);

    /*------------------ ����CCPXCTL3�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_SinglePWM������PxSPMλ�� */
    /* ���ݽṹ���Աm_CloseTimer������PxSPMSTλ�� */
    tmpreg = (ccpInitStruct->m_SinglePWM << CCP_CTL3_PXSPM_POS) \
             | (ccpInitStruct->m_CloseTimer << CCP_CTL3_PXSPMST_POS);
    CCPx->CCPXCTL3 = SFR_Config (CCPx->CCPXCTL3,
                          ~CCP_CTL3_PWM_INIT_MASK,
                          tmpreg);
}

/**
  * ����  ��ʼ��ͨ��PWMģ��(CCP)������Ϣ�ṹ�塣
  * ����  ccpInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  ��
  */
void
CCP_PWM_Struct_Init (CCP_PWMInitTypeDef* ccpInitStruct)
{
    /* ����PWMͨ�� */
    ccpInitStruct->m_Channel = CCP_CHANNEL_1;
    /* ����ͨ����ģʽѡ��λ */
    ccpInitStruct->m_PwmMode = CCP_MODE_RST;
    /* ����PWMռ�ձ� */
    ccpInitStruct->m_DutyRatio = 0x00000000;
    /* ����������� */
    ccpInitStruct->m_OutputCtl = CCP_CHANNEL_OUTPUT_PWM_ACTIVE;
    /* ���õ��������ģʽ */
    ccpInitStruct->m_SinglePWM = FALSE;
    /* ���õ��������ģʽѡ�� */
    ccpInitStruct->m_CloseTimer = FALSE;
}

/**
  * ����  ����CCP��׽����ͨ��ģʽ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  *       EdgeConfig: ƥ����أ�ȡֵ��ΧΪ:
  *                     CCP_MODE_RST: ģʽ�ر�
  *                     CCP_CAP_FALLING_EDGE: ��ÿ���½��ط�����׽
  *                     CCP_CAP_RISING_EDGE: ��ÿ�������ط�����׽
  *                     CCP_CAP_4TH_RISING_EDGE: ÿ4�������ط�����׽
  *                     CCP_CAP_16TH_RISING_EDGE: ÿ16�������ط�����׽
  * ����  ��
  */
void
CCP_Capture_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel, uint32_t EdgeConfig)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_CCP_CAP_MODE(EdgeConfig));

    /*-------------------- ����CCPXCTL1�Ĵ��� --------------------*/
    tmpreg = EdgeConfig << (4 * Channel);
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * ����  ����CCP�ȽϹ���ͨ��ģʽ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  *       EdgeConfig: ƥ����أ�ȡֵ��ΧΪ:
  *                     CCP_MODE_RST: ģʽ�ر�
  *                     CCP_CMP_ACTIVE_LEVEL: �Ƚ�ƥ��ʱ����ߵ�ƽ
  *                     CCP_CMP_INACTIVE_LEVEL: �Ƚ�ƥ��ʱ����͵�ƽ
  *                     CCP_CMP_SOFT_INT: �Ƚ�ƥ��ʱ��������ж�
  *                     CCP_CMP_SPECIAL_EVENT: ���������¼�
  * ����  ��
  */
void
CCP_Compare_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel, uint32_t EdgeConfig)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_CCP_CMP_MODE(EdgeConfig));

    /*-------------------- ����CCPXCTL1�Ĵ��� --------------------*/
    tmpreg = EdgeConfig << (4 * Channel);
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * ����  ����CCP PWM����ͨ��ģʽ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  *       EdgeConfig: ƥ����أ�ȡֵ��ΧΪ:
  *                     CCP_MODE_RST: ģʽ�ر�
  *                     CCP_PWM_MODE: PWMģʽ
  * ����  ��
  */
void
CCP_PWM_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel, uint32_t EdgeConfig)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_CCP_PWM_MODE(EdgeConfig));

    /*-------------------- ����CCPXCTL1�Ĵ��� --------------------*/
    tmpreg = EdgeConfig << (4 * Channel);
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * ����  ��ȡCCP��׽�Ĵ�����
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  * ����  ��׽�Ĵ�����ֵ��
  */
uint32_t
CCP_Get_Capture_Result (CCP_SFRmap* CCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));

    /* ��ȡ�Ĵ������ʵ�ַ */
    tmpreg = (uint32_t)CCPx;
    tmpreg = tmpreg + CCP_CY_OFFSET + (4 * Channel);

    /* ��ȡ�Ĵ�����ֵ */
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        return (*(volatile const uint32_t*)  tmpreg);
    }
    else
    {
        return (uint16_t)(*(volatile const uint32_t*)  tmpreg);
    }
}

/**
  * ����  ����CCP�Ƚ�/PWMռ�ձȼĴ�����
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  * ����  �ޡ�
  */
void
CCP_Set_Compare_Result (CCP_SFRmap* CCPx, uint32_t Channel, uint32_t Value)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));

    /* ��ȡCCPXRY�Ĵ�����ַ */
    tmpreg = (uint32_t)CCPx;
    tmpreg = tmpreg + CCP_RY_OFFSET + (4 * Channel);

    /*-------------------- ����CCPXRY�Ĵ��� --------------------*/
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        *(volatile uint32_t*)tmpreg = Value;
    }
    else
    {
        *(volatile uint32_t*)tmpreg = (uint16_t)Value;
    }
}

/**
  * ����  ��ȡCCP�Ƚ�/PWMռ�ձȼĴ�����
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  * ����  �Ƚ�/PWMռ�ձȼĴ�����ֵ��
  */
uint32_t
CCP_Get_Compare_Result (CCP_SFRmap* CCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));

    /* ��ȡ�Ĵ������ʵ�ַ */
    tmpreg = (uint32_t)CCPx;
    tmpreg = tmpreg + CCP_RY_OFFSET + (4 * Channel);

    /* ��ȡ�Ĵ�����ֵ */
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        return (*(volatile uint32_t*)  tmpreg);
    }
    else
    {
        return (uint16_t)(*(volatile uint32_t*)  tmpreg);
    }
}

/**
  * ����  ����CCP����Ƚ�/PWM�ź�����ʹ��λ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  *       NewState: CCP����Ƚ�/PWM�ź�����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  �ޡ�
  */
void
CCP_Compare_PWM_Signal_Clear (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL2�Ĵ���PXOCyCEλ --------------------*/
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~(CCP_CTL2_PXOC1CE << (Channel)),
                          (uint32_t)NewState << (CCP_CTL2_PXOC1CE_POS + Channel));
}

/**
  * ����  ����CCP�Ƚ������ѡ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       CompareSelect: �Ƚ������ѡ��ȡֵ��ΧΪ:
  *                        CCP_COMPARE_SELECT_1: ���Ƚ���1�����
  *                        CCP_COMPARE_SELECT_2: ���Ƚ���2�����
  *                        CCP_COMPARE_SELECT_3: ���Ƚ���3�����
  * ����  �ޡ�
  */
void
CCP_Compare_Check_Config (CCP_SFRmap* CCPx, uint32_t CompareSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_COMPARE_SELECT(CompareSelect));

    /*-------------------- ����CCPXCTL2�Ĵ���PXASSλ --------------------*/
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~(CCP_CTL2_PXASS),
                          CompareSelect);
}

/**
  * ����  ����CCPͨ��������ơ�
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  *       ChannelOutputCtl: �Ƚ������ѡ��ȡֵ��ΧΪ:
  *                           CCP_CHANNEL_OUTPUT_PWM_ACTIVE: PWM���������Ч
  *                           CCP_CHANNEL_OUTPUT_PWM_INACTIVE: PWM���������Ч
  *                           CCP_CHANNEL_OUTPUT_INACTIVE: ǿ�Ƶ͵�ƽ���
  *                           CCP_CHANNEL_OUTPUT_ACTIVE: ǿ�Ƹߵ�ƽ���
  * ����  �ޡ�
  */
void
CCP_Channel_Output_Control (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t ChannelOutputCtl)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL_OUTPUT(ChannelOutputCtl));

    /*-------------------- ����CCPXCTL2�Ĵ���PXOCyλ --------------------*/
    tmpreg = ChannelOutputCtl << (Channel * 2);
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~(CCP_CTL2_PXOC1 << (Channel * 2)),
                          tmpreg);
}

/**
  * ����  ���õ��������ģʽ��
  * ����  CCPx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: ���������ģʽѡ��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
CCP_Single_Pulse_Shut_Enable (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL3�Ĵ���PXSPMSTλ --------------------*/
    if (NewState != FALSE)
    {
        /* �����������رն�ʱ��ʹ��λ */
        SFR_SET_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXSPMST_POS);
    }
    else
    {
        /* ����������󲻹رն�ʱ��ʹ��λ */
        SFR_CLR_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXSPMST_POS);
    }
}

/**
  * ����  ���õ��������ģʽ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: ���������ģʽʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
CCP_Single_Pulse_Enable (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL3�Ĵ���PXSPMλ --------------------*/
    if (NewState != FALSE)
    {
        /* ���������ģʽʹ�� */
        SFR_SET_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXSPM_POS);
    }
    else
    {
        /* ���������ģʽδʹ�� */
        SFR_CLR_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXSPM_POS);
    }
}

/**
  * ����  PWM�������ģʽʹ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: PWM�������ģʽʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
CCP_PWM_Input_Measurement_Config (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL2�Ĵ���PXPWMIλ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PWM�������ģʽ */
        SFR_SET_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXPWMI_POS);
    }
    else
    {
        /* δʹ��PWM�������ģʽ */
        SFR_CLR_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXPWMI_POS);
    }
}

/**
  * ����  �������ʹ��λ����
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: �������ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
CCP_Input_XOR_Config (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL2�Ĵ���PXXORMλ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PWM�������ģʽ */
        SFR_SET_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXXORM_POS);
    }
    else
    {
        /* δʹ��PWM�������ģʽ */
        SFR_CLR_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXXORM_POS);
    }
}

/**
  * ����  ����/�Ƚ�ͨ����DMA�������á�
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  *       NewState: ����/�Ƚ�ͨ����DMA����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  �ޡ�
  */
void
CCP_Channel_DMA_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL3�Ĵ���CCPXCCYDEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ������/�Ƚ�ͨ����DMA���� */
        CCPx->CCPXCTL3 |= CCP_CTL3_CC1DE << Channel;
    }
    else
    {
        /* ��ֹ����/�Ƚ�ͨ����DMA���� */
        CCPx->CCPXCTL3 &= ~(CCP_CTL3_CC1DE << Channel);
    }
}

/**
  * ����  ������׽/�Ƚ��¼�����λ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  *       NewState: ������׽/�Ƚ��¼�����״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
CCP_Generate_Trigger_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------------ ����CCPXEGIF�Ĵ���CCPXCCyGλ ------------------*/
    if (NewState != FALSE)
    {
        /* ��ͨ��Channel�ϲ���һ����׽/�Ƚ��¼� */
        CCPx->CCPXEGIF |= CCP_EGIF_CC1G << Channel;
    }
    else
    {
        /* �޶��� */
        CCPx->CCPXEGIF &= ~(CCP_EGIF_CC1G << Channel);
    }
}

/**
  * ����  �������ڽ����źŲ����¼�����λ��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       NewState: �������ڽ����źŲ���ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
CCP_USART_Receive_Config (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXEGIF�Ĵ���USARTRXENλ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܲ������ڽ����ź� */
        SFR_SET_BIT_ASM(CCPx->CCPXEGIF, CCP_EGIF_UARTRXEN_POS);
    }
    else
    {
        /* ��ֹ�������ڽ����ź� */
        SFR_CLR_BIT_ASM(CCPx->CCPXEGIF, CCP_EGIF_UARTRXEN_POS);
    }
}
/**
  *   ##### ͨ�ò�׽/�Ƚ�/PWM����(CCP)��׽���ܺ���������� #####
  */


/**
  *   ##### ͨ�ò�׽/�Ƚ�/PWM����(CCP)�жϼ���־���������� #####
  */
/**
  * ����  ��ȡCCPͨ����׽/�Ƚ��жϱ�־��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
CCP_Get_Channel_Trigger_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));

    /*-------------------- ��ȡCCPXEGIF�Ĵ���TXTIFλ --------------------*/
    tmpreg = CCP_EGIF_CC1IF << Channel;
    if (CCPx->CCPXEGIF & tmpreg)
    {
        /* �����ж� */
        return SET;
    }
    else
    {
        /* δ�����ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡCCPͨ������DMA�жϱ�־��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  * ����  �ж�״̬��0��δ�����жϣ�1�������жϣ�DMA ��Ӧ���λ��Ӳ���Զ�����
  */
FlagStatus
CCP_Get_Trigger_DMA_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));

    /*-------------------- ��ȡCCPXDF�Ĵ���CCyDFλ --------------------*/
    tmpreg = CCP_DF_CC1DF << Channel;
    if (CCPx->CCPXDF & tmpreg)
    {
        /*  CCPͨ������DMA */
        return SET;
    }
    else
    {
        /*  CCPͨ���޴���DMA */
        return RESET;
    }
}

/**
  * ����  ͨ������/�Ƚϵ��ж�ʹ�����á�
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  *       NewState: ͨ������/�Ƚϵ��ж�ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  �ޡ�
  */
void
CCP_Channel_INT_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����CCPXCTL3�Ĵ���CCPXCCYIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����CCPͨ��Channel��׽/�Ƚ��ж� */
        CCPx->CCPXCTL3 |= CCP_CTL3_CC1IE << Channel;
    }
    else
    {
        /* ��ֹCCPͨ��Channel��׽/�Ƚ��ж� */
        CCPx->CCPXCTL3 &= ~(CCP_CTL3_CC1IE << Channel);
    }
}

/**
  * ����  ���CCPͨ������/�Ƚϵ��жϱ�־��
  * ����  CCPx: ָ��CCP��ͨ�ö�ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR��
  *               ��CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  CCP_CHANNEL_1: ͨ��1
  *                  CCP_CHANNEL_2: ͨ��2
  *                  CCP_CHANNEL_3: ͨ��3
  *                  CCP_CHANNEL_4: ͨ��4
  * ����  �ޡ�
  */
void
CCP_Clear_Channel_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));

    /*-------------------- ����CCPXSRIC�Ĵ���CCPXCCYICλ --------------------*/
    tmpreg = CCP_EGIF_CC1IF << Channel;
    CCPx->CCPXSRIC |= CCP_SRIC_CC1IC << Channel;
    while((CCPx->CCPXEGIF & tmpreg)>>(Channel+CCP_EGIF_CC1IF_POS));
    CCPx->CCPXSRIC &= ~(CCP_SRIC_CC1IC << Channel);
}
/**
  *   ##### ͨ�ò�׽/�Ƚ�/PWM����(CCP)�жϼ���־������������� #####
  */

/**
  ******************************************************************************
  * �ļ���  kf32a_basic_atim.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˸߼���ʱ�����蹦�ܺ�����������
  *          + �߼���ʱ��(ATIM)��ʼ�������ú�������
  *          + �߼���ʱ��(ATIM)�жϼ���־����������
  *          + ��ǿ�Ͳ�׽/�Ƚ�/PWM����(ECCP)��׽���ܺ�������
  *          + ��ǿ�Ͳ�׽/�Ƚ�/PWM����(ECCP)�жϼ���־����������
  *
  *********************************************************************
  */

#include "kf32a_basic_tim.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* ��ʱ��˽�ж��� ----------------------------------------------------*/
/* ATIMx_TXCTL�Ĵ����������� */
#define ATIM_TXCTL_INIT_MASK            (ATIM_TXCTL_TXCS \
                                       | ATIM_TXCTL_TXSY \
                                       | ATIM_TXCTL_TXCKBS \
                                       | ATIM_TXCTL_TXCLK \
                                       | ATIM_TXCTL_TXCMS)

/* ATIMx_TZCTL�Ĵ����������� */
#define ATIM_TZCTL_INIT_MASK            (ATIM_TZCTL_TZCS \
                                       | ATIM_TZCTL_TZSY \
                                       | ATIM_TZCTL_TZCKBS \
                                       | ATIM_TZCTL_TZCLK \
                                       | ATIM_TZCTL_TZCMS)

/* ATIMx_PXUDCTL��TxΪʱ���ļĴ��������������� */
#define ATIM_X_PXUDCTL_INIT_MASK  (ECCP_PXUDCTL_PXUDEVT0 \
                                  | ECCP_PXUDCTL_PXUDEVT1)

/* ATIMx_PXUDCTL��TzΪʱ���ļĴ��������������� */
#define ATIM_Z_PXUDCTL_INIT_MASK  (ECCP_PXUDCTL_PXUDEVT2 \
                                  | ECCP_PXUDCTL_PXUDEVT3)

/* ECCPX_CTL2�Ĵ�����ʼ������ */
#define ECCP_CTL2_INIT_MASK             (ECCP_CTL2_PXPWMI \
                                       | ECCP_CTL2_PXXORM)

/* ECCPXRY�Ĵ���ƫ�Ƶ�ַ */
#define ECCP_RY_OFFSET                   ((uint32_t)0x84)

/* ECCPXCY�Ĵ���ƫ�Ƶ�ַ */
#define ECCP_CY_OFFSET                   ((uint32_t)0xD0)


/**
  *   ##### �߼���ʱ��(ATIM)��ʼ�������ú������� #####
  */
/**
  * ����  �߼���ʱ��(ATIM)���ã�����Tx��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       atimInitStruct: �߼���ʱ��������Ϣ�ṹ��ָ�롣
  * ����  ��
  */
void ATIM_X_Configuration(ATIM_SFRmap* ATIMx,
                    ATIM_InitTypeDef* atimInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_POSTSCALER(atimInitStruct->m_Postscaler));
    CHECK_RESTRICTION(CHECK_ATIM_COUNTER_MODE(atimInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_ATIM_CLOCK_CONFIG(atimInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_ATIM_WORK_MODE(atimInitStruct->m_WorkMode));
    CHECK_RESTRICTION(CHECK_ATIM_SYNC_MODE(atimInitStruct->m_EXPulseSync));

    /*---------------- ����TxCNT�Ĵ��� ----------------*/
    ATIMx->TXCNT = (uint32_t)atimInitStruct->m_Counter;

    /*--------------- ����PPx���ڼĴ��� ---------------*/
    ATIMx->TXPPX = (uint32_t)atimInitStruct->m_Period;

    /*-------------- ����TxԤ��Ƶ�Ĵ��� ---------------*/
    ATIMx->TXPRSC = (uint32_t)atimInitStruct->m_Prescaler;

    /*------------------ ����TXCTL�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Postscaler������TXCKBSλ�� */
    /* ���ݽṹ���Աm_CounterMode������TXCMSλ�� */
    /* ���ݽṹ���Աm_Clock������TXCLKλ�� */
    /* ���ݽṹ���Աm_WorkMode������TXCSλ�� */
    /* ���ݽṹ���Աm_EXPulseSync������TXSYλ�� */
    tmpreg = ((uint32_t)atimInitStruct->m_Postscaler << ATIM_TXCTL_TXCKBS0_POS) \
             | (atimInitStruct->m_CounterMode) \
             | ((uint32_t)atimInitStruct->m_Clock) \
             | ((uint32_t)atimInitStruct->m_WorkMode) \
             | ((uint32_t)atimInitStruct->m_EXPulseSync);
    ATIMx->TXCTL = SFR_Config (ATIMx->TXCTL,
                          ~ATIM_TXCTL_INIT_MASK,
                          tmpreg);
}

/**
  * ����  �߼���ʱ��(ATIM)���ã�����Tz��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       atimInitStruct: �߼���ʱ��������Ϣ�ṹ��ָ�롣
  * ����  ��
  */
void ATIM_Z_Configuration(ATIM_SFRmap* ATIMx,
                    ATIM_InitTypeDef* atimInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_POSTSCALER(atimInitStruct->m_Postscaler));
    CHECK_RESTRICTION(CHECK_ATIM_COUNTER_MODE(atimInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_ATIM_CLOCK_CONFIG(atimInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_ATIM_WORK_MODE(atimInitStruct->m_WorkMode));
    CHECK_RESTRICTION(CHECK_ATIM_SYNC_MODE(atimInitStruct->m_EXPulseSync));

    /*---------------- ����TxCNT�Ĵ��� ----------------*/
    ATIMx->TZCNT = (uint32_t)atimInitStruct->m_Counter;

    /*--------------- ����PPx���ڼĴ��� ---------------*/
    ATIMx->TZPPZ = (uint32_t)atimInitStruct->m_Period;

    /*-------------- ����TxԤ��Ƶ�Ĵ��� ---------------*/
    ATIMx->TZPRSC = (uint32_t)atimInitStruct->m_Prescaler;

    /*------------------ ����TXCTL�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Postscaler������TXCKBSλ�� */
    /* ���ݽṹ���Աm_CounterMode������TXCMSλ�� */
    /* ���ݽṹ���Աm_Clock������TXCLKλ�� */
    /* ���ݽṹ���Աm_WorkMode������TXCSλ�� */
    /* ���ݽṹ���Աm_EXPulseSync������TXSYλ�� */
    tmpreg = ((uint32_t)atimInitStruct->m_Postscaler << ATIM_TZCTL_TZCKBS0_POS) \
             | (atimInitStruct->m_CounterMode) \
             | ((uint32_t)atimInitStruct->m_Clock) \
             | ((uint32_t)atimInitStruct->m_WorkMode) \
             | ((uint32_t)atimInitStruct->m_EXPulseSync);
    ATIMx->TZCTL = SFR_Config (ATIMx->TZCTL,
                          ~ATIM_TZCTL_INIT_MASK,
                          tmpreg);
}

/**
  * ����  ��ʼ���߼���ʱ��������Ϣ�ṹ�塣
  * ����  atimInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  ��
  */
void
ATIM_Struct_Init (ATIM_InitTypeDef* atimInitStruct)
{
    /* ����Tx����ֵ */
    atimInitStruct->m_Counter = 0x00000000;
    /* ����Tx����ֵ */
    atimInitStruct->m_Period = 0x0000FFFF;
    /* ����TxԤ��Ƶֵ */
    atimInitStruct->m_Prescaler = 0x00000000;
    /* ����Tx���Ƶֵ */
    atimInitStruct->m_Postscaler = 0x0;
    /* ����Tx����ģʽ */
    atimInitStruct->m_CounterMode = ATIM_COUNT_UP_OF;
    /* ����Tx����ʱ�� */
    atimInitStruct->m_Clock = ATIM_SCLK;
    /* ���ö�ʱ/����ģʽѡ�� */
    atimInitStruct->m_WorkMode = ATIM_TIMER_MODE;
    /* ����Tx����ģʽ�ⲿ������������ͬ������ */
    atimInitStruct->m_EXPulseSync = ATIM_EX_SYNC_MODE;
}

/**
  * ����  ��ʱ��Tx��������ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: ��ʱ��ʹ�ܿ��ƣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL�Ĵ���TXENλ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ��Tx��ʱ�� */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXEN_POS);
    }
    else
    {
        /* ��ֹTx��ʱ�� */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXEN_POS);
    }
}

/**
  * ����  ��ʱ��Tz��������ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: ��ʱ��ʹ�ܿ��ƣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Z_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TZCTL�Ĵ���TZENλ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ��Tz��ʱ�� */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZEN_POS);
    }
    else
    {
        /* ��ֹTz��ʱ�� */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZEN_POS);
    }
}

/**
  * ����  �߼���ʱ��(ATIM)�������ã�����Tx��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       UpdataCounter: �߼���ʱ�����¼�������ֵ��ȡֵ8λ���ݡ�
  *       UpdataOutput: ����������ƼĴ���ʹ�ܿ��ƣ�
  *                       ȡֵΪTRUE��FALSE��
  *       UpdataImmediately: �������¶�ʱ��ʹ�ܿ��ƣ�
  *                            ȡֵΪTRUE��FALSE��
  * ����  ��
  */
void
ATIM_X_Updata_Configuration (ATIM_SFRmap* ATIMx,
                    uint8_t UpdataCounter,
                    uint32_t UpdataOutput,
                    uint32_t UpdataImmediately)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(UpdataOutput));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(UpdataImmediately));

    /*-------------- ����TXUDTIM���¼����Ĵ��� ---------------*/
    ATIMx->TXUDTIM = UpdataCounter;

    /*------------------ ����PXUDCTL�Ĵ��� -----------------*/
    /* ���ݽṹ���ԱUpdataOutput������PXUDEVT0λ�� */
    /* ���ݽṹ���ԱUpdataImmediately������PXUDEVT1λ�� */
    tmpreg = (UpdataOutput << ECCP_PXUDCTL_PXUDEVT0_POS) \
             | (UpdataImmediately << ECCP_PXUDCTL_PXUDEVT1_POS);
    ATIMx->PXUDCTL = SFR_Config (ATIMx->PXUDCTL,
                          ~ATIM_X_PXUDCTL_INIT_MASK,
                          tmpreg);
}

/**
  * ����  �߼���ʱ��(ATIM)�������ã�����Tz��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       UpdataCounter: �߼���ʱ�����¼�������ֵ��ȡֵ8λ���ݡ�
  *       UpdataOutput: ����������ƼĴ���ʹ�ܿ��ƣ�
  *                       ȡֵΪTRUE��FALSE��
  *       UpdataImmediately: �������¶�ʱ��ʹ�ܿ��ƣ�
  *                            ȡֵΪTRUE��FALSE��
  * ����  ��
  */
void
ATIM_Z_Updata_Configuration (ATIM_SFRmap* ATIMx,
                    uint8_t UpdataCounter,
                    uint32_t UpdataOutput,
                    uint32_t UpdataImmediately)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(UpdataOutput));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(UpdataImmediately));

    /*-------------- ����TXUDTIM���¼����Ĵ��� ---------------*/
    ATIMx->TZUDTIM = UpdataCounter;

    /*------------------ ����PXUDCTL�Ĵ��� -----------------*/
    /* ���ݽṹ���ԱUpdataOutput������PXUDEVT2λ�� */
    /* ���ݽṹ���ԱUpdataImmediately������PXUDEVT3λ�� */
    tmpreg = (UpdataOutput << ECCP_PXUDCTL_PXUDEVT2_POS) \
             | (UpdataImmediately << ECCP_PXUDCTL_PXUDEVT3_POS);
    ATIMx->PXUDCTL = SFR_Config (ATIMx->PXUDCTL,
                          ~ATIM_Z_PXUDCTL_INIT_MASK,
                          tmpreg);
}

/**
  * ����  ��ʱ��Tx����ʹ�ܿ��ơ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: ��ʱ��Tx����ʹ�ܿ��ƣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Updata_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_PXUDCTL�Ĵ���PXUDEN0λ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ��Tx��ʱ������ */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN0_POS);
    }
    else
    {
        /* ��ֹTx��ʱ������ */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN0_POS);
    }
}

/**
  * ����  ��ʱ��Tz����ʹ�ܿ��ơ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: ��ʱ��Tz����ʹ�ܿ��ƣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Z_Updata_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_PXUDCTL�Ĵ���PXUDEN1λ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ��Tz��ʱ�� */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN1_POS);
    }
    else
    {
        /* ��ֹTz��ʱ�� */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN1_POS);
    }
}

/**
  * ����  ���ö�ʱ��Tx����ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       Counter: ����ֵ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
ATIM_X_Set_Counter (ATIM_SFRmap* ATIMx, uint16_t Counter)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TXCNT�Ĵ��� ----------------*/
    ATIMx->TXCNT = Counter;
}

/**
  * ����  ���ö�ʱ��Tz����ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       Counter: ����ֵ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
ATIM_Z_Set_Counter (ATIM_SFRmap* ATIMx, uint16_t Counter)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TZCNT�Ĵ��� ----------------*/
    ATIMx->TZCNT = Counter;
}

/**
  * ����  ���ö�ʱ��Tx����ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       Period: ����ֵ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
ATIM_X_Set_Period (ATIM_SFRmap* ATIMx, uint16_t Period)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TXPPX�Ĵ��� ----------------*/
    ATIMx->TXPPX = Period;
}

/**
  * ����  ���ö�ʱ��Tz����ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       Period: ����ֵ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
ATIM_Z_Set_Period (ATIM_SFRmap* ATIMx, uint16_t Period)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TZPPZ�Ĵ��� ----------------*/
    ATIMx->TZPPZ = Period;
}

/**
  * ����  ���ö�ʱ��TxԤ��Ƶֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       Prescaler: Ԥ��Ƶ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
ATIM_X_Set_Prescaler (ATIM_SFRmap* ATIMx, uint16_t Prescaler)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TXPRSC�Ĵ��� ----------------*/
    ATIMx->TXPRSC = Prescaler;
}

/**
  * ����  ���ö�ʱ��TzԤ��Ƶֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       Prescaler: Ԥ��Ƶ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
ATIM_Z_Set_Prescaler (ATIM_SFRmap* ATIMx, uint16_t Prescaler)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TZPRSC�Ĵ��� ----------------*/
    ATIMx->TZPRSC = Prescaler;
}

/**
  * ����  ���ö�ʱ��Tx����ģʽ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       CounterMode: �µļ���ģʽ��
  *                    ȡֵ��ΧΪ��
  *                      ATIM_COUNT_DOWN_UF: ���¼���,��������жϱ�־
  *                      ATIM_COUNT_UP_OF: ���ϼ���,��������жϱ�־
  *                      ATIM_COUNT_UP_DOWN_OF: ����-���¼���,��������жϱ�־
  *                      ATIM_COUNT_UP_DOWN_UF: ����-���¼���,��������жϱ�־
  *                      ATIM_COUNT_UP_DOWN_OUF: ����-���¼���,�������������жϱ�־
  * ����  ��
  */
void
ATIM_X_Counter_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t CounterMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_COUNTER_MODE(CounterMode));

    /*-------------------- ����TXCTL�Ĵ���TXCMSλ --------------------*/
    ATIMx->TXCTL = SFR_Config (ATIMx->TXCTL,
                          ~ATIM_TXCTL_TXCMS,
                          CounterMode);
}

/**
  * ����  ���ö�ʱ��Tz����ģʽ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       CounterMode: �µļ���ģʽ��
  *                    ȡֵ��ΧΪ��
  *                      ATIM_COUNT_DOWN_UF: ���¼���,��������жϱ�־
  *                      ATIM_COUNT_UP_OF: ���ϼ���,��������жϱ�־
  *                      ATIM_COUNT_UP_DOWN_OF: ����-���¼���,��������жϱ�־
  *                      ATIM_COUNT_UP_DOWN_UF: ����-���¼���,��������жϱ�־
  *                      ATIM_COUNT_UP_DOWN_OUF: ����-���¼���,�������������жϱ�־
  * ����  ��
  */
void
ATIM_Z_Counter_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t CounterMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_COUNTER_MODE(CounterMode));

    /*-------------------- ����TZCTL�Ĵ���TZCMSλ --------------------*/
    ATIMx->TZCTL = SFR_Config (ATIMx->TZCTL,
                          ~ATIM_TZCTL_TZCMS,
                          CounterMode);
}

/**
  * ����  ���ö�ʱ��Tx����ʱ�ӡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewClock: ��ʱ��Tx����ʱ�ӣ�
  *                 ȡֵ��ΧΪ��
  *                   ATIM_SCLK: ѡ��SCLKʱ��
  *                   ATIM_HFCLK: ѡ��HFCLKʱ��
  *                   ATIM_LFCLK: ѡ��LFCLKʱ��
  * ����  ��
  */
void
ATIM_X_Clock_Config (ATIM_SFRmap* ATIMx, uint32_t NewClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_CLOCK_CONFIG(NewClock));

    /*-------------------- ����TXCTL�Ĵ���TXCLKλ --------------------*/
    ATIMx->TXCTL = SFR_Config (ATIMx->TXCTL,
                          ~ATIM_TXCTL_TXCLK,
                          NewClock);
}

/**
  * ����  ���ö�ʱ��Tz����ʱ�ӡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewClock: ��ʱ��Tz����ʱ�ӣ�
  *                 ȡֵ��ΧΪ��
  *                   ATIM_SCLK: ѡ��SCLKʱ��
  *                   ATIM_HFCLK: ѡ��HFCLKʱ��
  *                   ATIM_LFCLK: ѡ��LFCLKʱ��
  * ����  ��
  */
void
ATIM_Z_Clock_Config (ATIM_SFRmap* ATIMx, uint32_t NewClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_CLOCK_CONFIG(NewClock));

    /*-------------------- ����TZCTL�Ĵ���TZCLKλ --------------------*/
    ATIMx->TZCTL = SFR_Config (ATIMx->TZCTL,
                          ~ATIM_TZCTL_TZCLK,
                          NewClock);
}

/**
  * ����  ���ö�ʱ��Tx�ĺ��Ƶ����
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewPostscaler: ��ʱ��Tx�ĺ��Ƶ����ȡֵΪ4λ���ݣ���Χ���£�
  *                   ATIM_POSTSCALER_DIV_1: ��Ƶ��Ϊ1:1
  *                   ATIM_POSTSCALER_DIV_2: ��Ƶ��Ϊ1:2
  *                   ATIM_POSTSCALER_DIV_3: ��Ƶ��Ϊ1:3
  *                   ATIM_POSTSCALER_DIV_4: ��Ƶ��Ϊ1:4
  *                   ATIM_POSTSCALER_DIV_5: ��Ƶ��Ϊ1:5
  *                   ATIM_POSTSCALER_DIV_6: ��Ƶ��Ϊ1:6
  *                   ATIM_POSTSCALER_DIV_7: ��Ƶ��Ϊ1:7
  *                   ATIM_POSTSCALER_DIV_8: ��Ƶ��Ϊ1:8
  *                   ATIM_POSTSCALER_DIV_9: ��Ƶ��Ϊ1:9
  *                   ATIM_POSTSCALER_DIV_10: ��Ƶ��Ϊ1:10
  *                   ATIM_POSTSCALER_DIV_11: ��Ƶ��Ϊ1:11
  *                   ATIM_POSTSCALER_DIV_12: ��Ƶ��Ϊ1:12
  *                   ATIM_POSTSCALER_DIV_13: ��Ƶ��Ϊ1:13
  *                   ATIM_POSTSCALER_DIV_14: ��Ƶ��Ϊ1:14
  *                   ATIM_POSTSCALER_DIV_15: ��Ƶ��Ϊ1:15
  *                   ATIM_POSTSCALER_DIV_16: ��Ƶ��Ϊ1:16
  * ����  ��
  */
void
ATIM_X_Postscaler_Config (ATIM_SFRmap* ATIMx, uint32_t NewPostscaler)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_POSTSCALER(NewPostscaler));

    /*-------------------- ����TXCTL�Ĵ���TXCKBSλ --------------------*/
    tmpreg = NewPostscaler << ATIM_TXCTL_TXCKBS0_POS;
    ATIMx->TXCTL = SFR_Config (ATIMx->TXCTL,
                          ~ATIM_TXCTL_TXCKBS,
                          tmpreg);
}

/**
  * ����  ���ö�ʱ��Tz�ĺ��Ƶ����
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewPostscaler: ��ʱ��Tz�ĺ��Ƶ����ȡֵΪ4λ���ݣ���Χ���£�
  *                   ATIM_POSTSCALER_DIV_1: ��Ƶ��Ϊ1:1
  *                   ATIM_POSTSCALER_DIV_2: ��Ƶ��Ϊ1:2
  *                   ATIM_POSTSCALER_DIV_3: ��Ƶ��Ϊ1:3
  *                   ATIM_POSTSCALER_DIV_4: ��Ƶ��Ϊ1:4
  *                   ATIM_POSTSCALER_DIV_5: ��Ƶ��Ϊ1:5
  *                   ATIM_POSTSCALER_DIV_6: ��Ƶ��Ϊ1:6
  *                   ATIM_POSTSCALER_DIV_7: ��Ƶ��Ϊ1:7
  *                   ATIM_POSTSCALER_DIV_8: ��Ƶ��Ϊ1:8
  *                   ATIM_POSTSCALER_DIV_9: ��Ƶ��Ϊ1:9
  *                   ATIM_POSTSCALER_DIV_10: ��Ƶ��Ϊ1:10
  *                   ATIM_POSTSCALER_DIV_11: ��Ƶ��Ϊ1:11
  *                   ATIM_POSTSCALER_DIV_12: ��Ƶ��Ϊ1:12
  *                   ATIM_POSTSCALER_DIV_13: ��Ƶ��Ϊ1:13
  *                   ATIM_POSTSCALER_DIV_14: ��Ƶ��Ϊ1:14
  *                   ATIM_POSTSCALER_DIV_15: ��Ƶ��Ϊ1:15
  *                   ATIM_POSTSCALER_DIV_16: ��Ƶ��Ϊ1:16
  * ����  ��
  */
void
ATIM_Z_Postscaler_Config (ATIM_SFRmap* ATIMx, uint32_t NewPostscaler)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_POSTSCALER(NewPostscaler));

    /*-------------------- ����TZCTL�Ĵ���TZCKBSλ --------------------*/
    tmpreg = NewPostscaler << ATIM_TZCTL_TZCKBS0_POS;
    ATIMx->TZCTL = SFR_Config (ATIMx->TZCTL,
                          ~ATIM_TZCTL_TZCKBS,
                          tmpreg);
}

/**
  * ����  ���ö�ʱ��Tx����ģʽ�ⲿ������������ͬ������λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewClock: ��ʱ��Tx����ģʽ�ⲿ������������ͬ������λʹ��״̬��
  *                 ȡֵ��ΧΪ��
  *                   ATIM_EX_SYNC_MODE: ���ⲿ������������ͬ��
  *                   ATIM_NO_SYNC_MODE: �����ⲿ������������ͬ��
  * ����  ��
  */
void
ATIM_X_External_Pulse_Sync_Config (ATIM_SFRmap* ATIMx, uint32_t PulseSync)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_SYNC_MODE(PulseSync));

    /*-------------------- ����TXCTL�Ĵ���TXSYλ --------------------*/
    if (PulseSync != ATIM_EX_SYNC_MODE)
    {
        /* �����ⲿ������������ͬ�� */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXSY_POS);
    }
    else
    {
        /* ���ⲿ������������ͬ�� */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXSY_POS);
    }
}

/**
  * ����  ���ö�ʱ��Tz����ģʽ�ⲿ������������ͬ������λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewClock: ��ʱ��Tz����ģʽ�ⲿ������������ͬ������λʹ��״̬��
  *                 ȡֵ��ΧΪ��
  *                   ATIM_EX_SYNC_MODE: ���ⲿ������������ͬ��
  *                   ATIM_NO_SYNC_MODE: �����ⲿ������������ͬ��
  * ����  ��
  */
void
ATIM_Z_External_Pulse_Sync_Config (ATIM_SFRmap* ATIMx, uint32_t PulseSync)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_SYNC_MODE(PulseSync));

    /*-------------------- ����TZCTL�Ĵ���TZSYλ --------------------*/
    if (PulseSync != ATIM_EX_SYNC_MODE)
    {
        /* �����ⲿ������������ͬ�� */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZSY_POS);
    }
    else
    {
        /* ���ⲿ������������ͬ�� */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZSY_POS);
    }
}

/**
  * ����  ����Tx��ʱ/����ģʽѡ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: Tx��ʱ/����ģʽѡ��
  *                 ȡֵ��ΧΪ��
  *                   ATIM_TIMER_MODE: ��ʱģʽ
  *                   ATIM_COUNTER_MODE: ����ģʽ
  * ����  ��
  */
void
ATIM_X_Work_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_WORK_MODE(NewState));

    /*-------------------- ����TXCTL�Ĵ���TXCSλ --------------------*/
    if (NewState != ATIM_TIMER_MODE)
    {
        /* ����ģʽ */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXCS_POS);
    }
    else
    {
        /* ��ʱģʽ */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXCS_POS);
    }
}

/**
  * ����  ����Tz��ʱ/����ģʽѡ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: Tz��ʱ/����ģʽѡ��
  *                 ȡֵ��ΧΪ��
  *                   ATIM_TIMER_MODE: ��ʱģʽ
  *                   ATIM_COUNTER_MODE: ����ģʽ
  * ����  ��
  */
void
ATIM_Z_Work_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_WORK_MODE(NewState));

    /*-------------------- ����TZCTL�Ĵ���TZCSλ --------------------*/
    if (NewState != ATIM_TIMER_MODE)
    {
        /* ����ģʽ */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZCS_POS);
    }
    else
    {
        /* ��ʱģʽ */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZCS_POS);
    }
}

/**
  * ����  ��Tx��������
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  * ����  Tx��������0�����£�1�����ϡ�
  */
DIRStatus
ATIM_X_Get_Direction (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ��ȡTXCTL�Ĵ���TXDIRλ --------------------*/
    if (ATIMx->TXCTL & ATIM_TXCTL_TXDIR)
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
  * ����  ��Tz��������
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  * ����  Tz��������0�����£�1�����ϡ�
  */
DIRStatus
ATIM_Z_Get_Direction (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ��ȡTZCTL�Ĵ���TZDIRλ --------------------*/
    if (ATIMx->TZCTL & ATIM_TZCTL_TZDIR)
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
  * ����  ����Tx�����жϴ���ADʹ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: Tx�����жϴ���ADʹ��λ��
  *                 ȡֵΪ��TRUE��FALSE��
  * ����  ��
  */
void
ATIM_X_Overflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL�Ĵ���TXOFS1λ --------------------*/
    if (NewState != FALSE)
    {
    	/* �����ж�ʹ�� */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXOFS1_POS);
    }
    else
    {
        /* �����жϽ�ֹ */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXOFS1_POS);
    }
}

/**
  * ����  ����Tz�����жϴ���ADʹ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: Tz�����жϴ���ADʹ��λ��
  *                 ȡֵΪ��TRUE��FALSE��
  * ����  ��
  */
void
ATIM_Z_Overflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TZCTL�Ĵ���TZOFS1λ --------------------*/
    if (NewState != FALSE)
    {
        /* ����ģʽ */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZOFS1_POS);
    }
    else
    {
        /* ��ʱģʽ */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZOFS1_POS);
    }
}

/**
  * ����  ����Tx�����жϴ���ADʹ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: Tx�����жϴ���ADʹ��λ��
  *                 ȡֵΪ��TRUE��FALSE��
  * ����  ��
  */
void
ATIM_X_Underflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL�Ĵ���TXOFS0λ --------------------*/
    if (NewState != FALSE)
    {
        /* ����ģʽ */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXOFS0_POS);
    }
    else
    {
        /* ��ʱģʽ */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXOFS0_POS);
    }
}

/**
  * ����  ����Tz�����жϴ���ADʹ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: Tz�����жϴ���ADʹ��λ��
  *                 ȡֵΪ��TRUE��FALSE��
  * ����  ��
  */
void
ATIM_Z_Underflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TZCTL�Ĵ���TZOFS0λ --------------------*/
    if (NewState != FALSE)
    {
        /* ����ģʽ */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZOFS0_POS);
    }
    else
    {
        /* ��ʱģʽ */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZOFS0_POS);
    }
}

/**
  * ����  ����Tx����ж��Զ�����AD����λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: Tx����ж��Զ�����AD����λ��
  *                 ȡֵΪ��TRUE��FALSE��
  * ����  ��
  */
void
ATIM_X_TriggerAD_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TXCTL�Ĵ���TXADAUTOλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����ģʽ */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXADAUTO_POS);
    }
    else
    {
        /* ��ʱģʽ */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXADAUTO_POS);
    }
}

/**
  * ����  ����Tz����ж��Զ�����AD����λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: Tz����ж��Զ�����AD����λ��
  *                 ȡֵΪ��TRUE��FALSE��
  * ����  ��
  */
void
ATIM_Z_TriggerAD_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����TZCTL�Ĵ���TZADAUTOλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����ģʽ */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZADAUTO_POS);
    }
    else
    {
        /* ��ʱģʽ */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZADAUTO_POS);
    }
}

/**
  * ����  ���ö�ʱ��Tx����AD�Ĵ�����ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       CompareAD0: Tx����AD�Ĵ���0��ֵ��ȡֵ16λ���ݡ�
  *       CompareAD1: Tx����AD�Ĵ���1��ֵ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
ATIM_X_Set_TriggerAD_Signal (ATIM_SFRmap* ATIMx,
                    uint16_t CompareAD0, uint16_t CompareAD1)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TXCCR0/TXCCR1�Ĵ��� ----------------*/
    ATIMx->TXCCR0 = CompareAD0;
    ATIMx->TXCCR1 = CompareAD1;
}

/**
  * ����  ���ö�ʱ��Tz����AD�Ĵ�����ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       CompareAD: Tz����AD�Ĵ���0��ֵ��ȡֵ16λ���ݡ�
  * ����  ��
  */
void
ATIM_Z_Set_TriggerAD_Signal (ATIM_SFRmap* ATIMx,
                    uint16_t CompareAD)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TZCCR0�Ĵ��� ----------------*/
    ATIMx->TZCCR0 = CompareAD;
}

/**
  * ����  ����Tx�������¿���λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: ��������ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Updata_Immediately_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_PXUDCTL�Ĵ���PXUDEVT1λ --------------------*/
    if (NewState != FALSE)
    {
        /* ���������¼� */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT1_POS);
    }
    else
    {
    	  /* ����ʱ�� UDTIMX Ϊ 0 ʱ������ Tx Ϊʱ����ռ�ձȡ����ڡ�Ԥ��Ƶ�����¼������Ĵ��� */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT1_POS);
    }
}

/**
  * ����  ����Tz�������¿���λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: ��������ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Z_Updata_Immediately_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_PXUDCTL�Ĵ���PXUDEVT3λ --------------------*/
    if (NewState != FALSE)
    {
        /* ���������¼� */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT3_POS);
    }
    else
    {
        /* ����ʱ��UDTIMzΪ0ʱ���� */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT3_POS);
    }
}

/**
  * ����  ����Tx������ƼĴ������¿���λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: ��������ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Updata_Output_Ctl (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_PXUDCTL�Ĵ���PXUDEVT0λ --------------------*/
    if (NewState != FALSE)
    {
        /* ����������TxΪʱ����������ƼĴ��� */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT0_POS);
    }
    else
    {
        /* ����ʱ��UDTIMxΪ0ʱ���� */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT0_POS);
    }
}

/**
  * ����  ����Tz������ƼĴ������¿���λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: ��������ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  *
  */
void
ATIM_Z_Updata_Output_Ctl (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_PXUDCTL�Ĵ���PXUDEVT2λ --------------------*/
    if (NewState != FALSE)
    {
        /* ����������TzΪʱ����������ƼĴ��� */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT2_POS);
    }
    else
    {
        /* ����ʱ��UDTIMzΪ0ʱ���� */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT2_POS);
    }
}

/**
  * ����  ����Tx����ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: ����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Updata_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_PXUDCTL�Ĵ���PXUDEN0λ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܸ��� */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN0_POS);
    }
    else
    {
        /* ��ֹ���� */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN0_POS);
    }
}

/**
  * ����  ����Tz����ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       NewState: ����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Z_Updata_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_PXUDCTL�Ĵ���PXUDEN1λ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܸ��� */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN1_POS);
    }
    else
    {
        /* ��ֹ���� */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN1_POS);
    }
}

/**
  * ����  ���ö�ʱ��Tx���¼�������
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       UpdataCounter: ���¼���ֵ��ȡֵ8λ���ݡ�
  * ����  ��
  */
void
ATIM_X_Set_Updata_Counter (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TXUDTIM�Ĵ��� ----------------*/
    ATIMx->TXUDTIM = UpdataCounter;
}

/**
  * ����  ���ö�ʱ��Tz���¼�������
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       UpdataCounter: ���¼���ֵ��ȡֵ8λ���ݡ�
  * ����  ��
  */
void
ATIM_Z_Set_Updata_Counter (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- ����TZUDTIM�Ĵ��� ----------------*/
    ATIMx->TZUDTIM = UpdataCounter;
}

/**
  * ����  ����Tx��ģʽѡ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       SlaveSelect: ��ģʽѡ��
  *                    ȡֵ��ΧΪ��
  *                      ATIM_SLAVE_FORBIDDEN_MODE: ��ģʽ��ֹ
  *                      ATIM_SLAVE_TRIGGER_MODE: ����ģʽ
  *                      ATIM_SLAVE_GATED_MODE: �ſ�ģʽ
  *                      ATIM_SLAVE_RESET_MODE: ��λģʽ
  *                      ATIM_SLAVE_COUNTER_MODE: ����ģʽ2
  * ����  ��
  */
void
ATIM_X_Slave_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t SlaveMode)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_SLAVE_MODE(SlaveMode));

    /*-------------------- ����ECCP_CTL2�Ĵ���TXSMSλ --------------------*/
    tmpreg = SlaveMode << ECCP_CTL2_TXSMS0_POS;
    ATIMx->ECCPXCTL2 = SFR_Config (ATIMx->ECCPXCTL2,
                          ~ECCP_CTL2_TXSMS,
                          tmpreg);
}

/**
  * ����  ����Tz��ģʽѡ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  *       SlaveSelect: ��ģʽѡ��
  *                    ȡֵ��ΧΪ��
  *                      ATIM_SLAVE_FORBIDDEN_MODE: ��ģʽ��ֹ
  *                      ATIM_SLAVE_TRIGGER_MODE: ����ģʽ
  *                      ATIM_SLAVE_GATED_MODE: �ſ�ģʽ
  *                      ATIM_SLAVE_RESET_MODE: ��λģʽ
  *                      ATIM_SLAVE_COUNTER_MODE: ����ģʽ2
  * ����  ��
  */
void
ATIM_Z_Slave_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t SlaveMode)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_SLAVE_MODE(SlaveMode));

    /*-------------------- ����ECCP_CTL2�Ĵ���TZSMSλ --------------------*/
    tmpreg = SlaveMode << ECCP_CTL2_TZSMS0_POS;
    ATIMx->ECCPXCTL2 = SFR_Config (ATIMx->ECCPXCTL2,
                          ~ECCP_CTL2_TZSMS,
                          tmpreg);
}

/**
  * ����  ����Tx��ģʽѡ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       MasterMode: ��ģʽѡ��ȡֵ��ΧΪ��
  *                     ATIM_MASTER_TXEN_SIGNAL: ʹ���ź�TXEN��Ϊ�������TRGO
  *                     ATIM_MASTER_TXIF_SIGNAL: TXIF�ź���Ϊ�������TRGO����ʱ�����ʱ���ж������źţ�
  *                     ATIM_MASTER_CCPXCH1IF_SIGNAL: ECCPx��CC1IF������Ϊ�������TRGO
  *                     ATIM_MASTER_CCPXCH1_SIGNAL: ECCPxCH1��Ϊ�������TRGO
  *                     ATIM_MASTER_CCPXCH2_SIGNAL: ECCPxCH2 ��Ϊ�������TRGO
  *                     ATIM_MASTER_CCPXCH3_SIGNAL: ECCPxCH3 ��Ϊ�������TRGO
  *                     ATIM_MASTER_CCPXCH4_SIGNAL: ECCPxCH4 ��Ϊ�������TRGO
  * ����  ��
  */
void
ATIM_Master_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t MasterMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_MASTER_MODE(MasterMode));

    /*-------------------- ����ECCP_CTL2�Ĵ���TXMMSλ --------------------*/
    ATIMx->ECCPXCTL2 = SFR_Config (ATIMx->ECCPXCTL2,
                          ~ECCP_CTL2_TXMMS,
                          MasterMode);
}

/**
  * ����  ��������ģʽͬ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: ����ģʽͬ��λ״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Master_Slave_Snyc_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_CTL2�Ĵ���TXMSSYNCλ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ������ģʽͬ�� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXCTL2, ECCP_CTL2_TXMSSYNC_POS);
    }
    else
    {
        /* δʹ�� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXCTL2, ECCP_CTL2_TXMSSYNC_POS);
    }
}

/**
  * ����  ���ô���ѡ��λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       TriggerSelect: ����ѡ��
  *                      ȡֵ��ΧΪ��
  *                        ATIM_TRIGGER_T1
  *                        ATIM_TRIGGER_T20
  *                        ATIM_TRIGGER_T5
  *                        ATIM_TRIGGER_T9
  *                        ATIM_TRIGGER_ECCPXCH1
  *                        ATIM_TRIGGER_ECCPXCH2
  *                        ATIM_TRIGGER_ECCPXCH3
  *                        ATIM_TRIGGER_TXCK
  * ����  ��
  */
void
ATIM_Trigger_Select_Config (ATIM_SFRmap* ATIMx,
                    uint32_t TriggerSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_TRIGGER_MODE(TriggerSelect));

    /*-------------------- ����ECCP_CTL2�Ĵ���TXTSλ --------------------*/
    ATIMx->ECCPXCTL2 = SFR_Config (ATIMx->ECCPXCTL2,
                          ~ECCP_CTL2_TXTS,
                          TriggerSelect);
}

/**
  * ����  ����Tx��Tz����ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  *       NewState: Tx��Tz����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Timer_Unite_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_CTL3�Ĵ���TUNITEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tx��Tz */
        SFR_SET_BIT_ASM(ATIMx->ECCPXCTL3, ECCP_CTL3_TUNITE_POS);
    }
    else
    {
        /* ������ */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXCTL3, ECCP_CTL3_TUNITE_POS);
    }
}

/**
  * ����  ��Tx��ʱ������ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  * ����  Tx����ֵ��16λ
  */
uint16_t
ATIM_X_Get_Counter (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* ����ʱ������ֵTxCNT */
    tmpreg = ATIMx->TXCNT;

    return tmpreg;
}

/**
  * ����  ��Tz��ʱ������ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  * ����  Tz����ֵ��16λ
  */
uint16_t
ATIM_Z_Get_Counter (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* ����ʱ������ֵTzCNT */
    tmpreg = ATIMx->TZCNT;

    return tmpreg;
}

/**
  * ����  ��Tx��ʱ������ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  * ����  Tx����ֵ��16λ
  */
uint16_t
ATIM_X_Get_Period (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* ����ʱ������ֵTxPPX */
    tmpreg = ATIMx->TXPPX;

    return tmpreg;
}

/**
  * ����  ��Tz��ʱ������ֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  * ����  Tz����ֵ��16λ
  */
uint16_t
ATIM_Z_Get_Period (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* ����ʱ������ֵTZPPZ */
    tmpreg = ATIMx->TZPPZ;

    return tmpreg;
}

/**
  * ����  ��Tx��ʱ��Ԥ��Ƶֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  * ����  TxԤ��Ƶֵ��16λ
  */
uint16_t
ATIM_X_Get_Prescaler (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* ����ʱ��Ԥ��ƵֵTxPPX */
    tmpreg = ATIMx->TXPRSC;

    return tmpreg;
}

/**
  * ����  ��Tz��ʱ��Ԥ��Ƶֵ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT6_SFR/T10_SFR��
  * ����  TzԤ��Ƶֵ��16λ
  */
uint16_t
ATIM_Z_Get_Prescaler (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* ����ʱ��Ԥ��ƵֵTxPPX */
    tmpreg = ATIMx->TZPRSC;

    return tmpreg;
}

/**
  *   ##### �߼���ʱ��(ATIM)��ʼ�������ú���������� #####
  */


/**
  *   ##### �߼���ʱ��(ATIM)�жϼ���־���������� #####
  */
/**
  * ����  ����Tx�����¼��ж�ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  *       NewState: Tx�����¼��жϣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Updata_INT_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_IE�Ĵ���TXUIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tx�����ж� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXUIE_POS);
    }
    else
    {
        /* ��ֹTx�����ж� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXUIE_POS);
    }
}

/**
  * ����  ����Tz�����¼��ж�ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT6_SFR/T10_SFR��
  *       NewState: Tz�����¼��жϣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Z_Updata_INT_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_IE�Ĵ���TZUIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tz�����ж� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TZUIE_POS);
    }
    else
    {
        /* ��ֹTz�����ж� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TZUIE_POS);
    }
}

/**
  * ����  ����Tx��������ж�ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  *       NewState: Tx��������жϣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Overflow_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_IE�Ĵ���TXIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tx������������ж� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXIE_POS);
    }
    else
    {
        /* ��ֹTx������������ж� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXIE_POS);
    }
}

/**
  * ����  ����Tz��������ж�ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT6_SFR/T10_SFR��
  *       NewState: Tz��������жϣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Z_Overflow_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_IE�Ĵ���TZIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tz������������ж� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TZIE_POS);
    }
    else
    {
        /* ��ֹTz������������ж� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TZIE_POS);
    }
}

/**
  * ����  ����Tx�����¼��ж�ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  *       NewState: Tx�����¼��жϣ�
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Trigger_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_IE�Ĵ���TXTIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����Tx�����¼������ж� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXTIE_POS);
    }
    else
    {
        /* ��ֹTx�����¼������ж� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXTIE_POS);
    }
}

/**
  * ����  ����Tx�����¼���DMA����ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: Tx�����¼���DMA����
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Updata_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_DE�Ĵ���TXUDEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ��������¼���DMA���� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TXUDE_POS);
    }
    else
    {
        /* ��ֹ�����¼���DMA���� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TXUDE_POS);
    }
}

/**
  * ����  ����Tz�����¼���DMA����ʹ�ܡ�
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP6/ECCP10��
  *       NewState: Tz�����¼���DMA����
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_Z_Updata_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_DE�Ĵ���TZUDEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ��������¼���DMA���� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TZUDE_POS);
    }
    else
    {
        /* ��ֹ�����¼���DMA���� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TZUDE_POS);
    }
}

/**
  * ����  ����Tx�����¼���DMA��������
  * ����  ECCPx: ָ��CCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: Tx�����¼���DMA����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Trigger_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_DE�Ĵ���TXTDEλ --------------------*/
    if (NewState != FALSE)
    {
        /* �������¼���DMA���� */
        SFR_SET_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TXTDE_POS);
    }
    else
    {
        /* ��ֹ�����¼���DMA���� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TXTDE_POS);
    }
}

/**
  * ����  ��ȡTx�����¼��жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ATIM_X_Get_Updata_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ��ȡECCPXEGIF�Ĵ���TXUIFλ --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TXUIF)
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
  * ����  ��ȡTz�����¼��жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT6_SFR/T10_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ATIM_Z_Get_Updata_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ��ȡECCPXEGIF�Ĵ���TZUIFλ --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TZUIF)
    {
        /* ����Tz�����¼��ж� */
        return SET;
    }
    else
    {
        /* δ����Tz�����¼��ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡTx��������жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ATIM_X_Get_Overflow_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ��ȡECCPXEGIF�Ĵ���TXIFλ --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TXIF)
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
  * ����  ��ȡTz��������жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT6_SFR/T10_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ATIM_Z_Get_Overflow_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ��ȡECCPXEGIF�Ĵ���TZIFλ --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TZIF)
    {
        /* ����Tz��������ж� */
        return SET;
    }
    else
    {
        /* δ����Tz��������ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡTx�����¼��жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬ȡֵT5_SFR/T9_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ATIM_X_Get_Trigger_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ��ȡECCPXEGIF�Ĵ���TXTIFλ --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TXTIF)
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
  * ����  ���������¼�����λ��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  *       NewState: ��ʱ��ʹ�ܿ���״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ATIM_X_Generate_Trigger_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_EGIF�Ĵ���TXTRGλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����������Ӧ */
        SFR_SET_BIT_ASM(ATIMx->ECCPXEGIF, ECCP_EGIF_TXTRG_POS);
    }
    else
    {
        /* �޶��� */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXEGIF, ECCP_EGIF_TXTRG_POS);
    }
}

/**
  * ����  ��ȡTx�����¼�DMA�жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ATIM_X_Get_Updata_DMA_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_DF�Ĵ���TXUDFλ --------------------*/
    if (ATIMx->ECCPXDF & ECCP_DF_TXUDF)
    {
        /* ����Tx����DMA�ж� */
        return SET;
    }
    else
    {
        /* δ����Tx����DMA�ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡTz�����¼�DMA�жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT6_SFR/T10_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ATIM_Z_Get_Updata_DMA_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_DF�Ĵ���TZUDFλ --------------------*/
    if (ATIMx->ECCPXDF & ECCP_DF_TZUDF)
    {
        /* ����Tz����DMA�ж� */
        return SET;
    }
    else
    {
        /* δ����Tz����DMA�ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡTx�����¼�����DMA�жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ATIM_X_Get_Trigger_DMA_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_DF�Ĵ���TRIGDFλ --------------------*/
    if (ATIMx->ECCPXDF & ECCP_DF_TRIGDF)
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
  * ����  ���Tx�����¼��жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  * ����  ��
  * ����˵��������ñ�־λ����ʹ�ܶ�Ӧ��ʱ��������½��У�����ᵼ�����ʧ�ܡ�
  */
void
ATIM_X_Clear_Updata_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_SRIC�Ĵ���TXUICλ --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXUIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TXUIF)>>ECCP_EGIF_TXUIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXUIC_POS);
}

/**
  * ����  ���Tz�����¼��жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT6_SFR/T10_SFR��
  * ����  ��
  * ����˵��������ñ�־λ����ʹ�ܶ�Ӧ��ʱ��������½��У�����ᵼ�����ʧ�ܡ�
  */
void
ATIM_Z_Clear_Updata_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_SRIC�Ĵ���TZUICλ --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TZUIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TZUIF)>>ECCP_EGIF_TZUIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TZUIC_POS);
}

/**
  * ����  ���Tx����жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  * ����  ��
  */
void
ATIM_X_Clear_Overflow_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_SRIC�Ĵ���TXICλ --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TXIF)>>ECCP_EGIF_TXIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXIC_POS);
}

/**
  * ����  ���Tz����жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT6_SFR/T10_SFR��
  * ����  ��
  */
void
ATIM_Z_Clear_Overflow_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_SRIC�Ĵ���TZICλ --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TZIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TZIF)>>ECCP_EGIF_TZIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TZIC_POS);
}

/**
  * ����  ���Tx�����¼��жϱ�־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  * ����  ��
  */
void
ATIM_X_Clear_Trigger_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_SRIC�Ĵ���TXTICλ --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXTIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TXTIF)>>ECCP_EGIF_TXTIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXTIC_POS);
}

/**
  *   ##### �߼���ʱ��(ATIM)�жϼ���־������������� #####
  */


/**
  *   ##### ��ǿ�Ͳ�׽/�Ƚ�/PWM����(ECCP)��׽���ܺ������� #####
  */
/**
  * ����  ��ǿ�ͱȽ���(ECCP)���á�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: �Ƚ�ͨ����ȡֵΪ:
  *                ECCP_CHANNEL_1: ͨ��1
  *                ECCP_CHANNEL_2: ͨ��2
  *                ECCP_CHANNEL_3: ͨ��3
  *                ECCP_CHANNEL_4: ͨ��4
  *       CompareMode: �Ƚ���ģʽ��ȡֵΪ:
  *                      ECCP_MODE_RST: �رձȽϹ���
  *                      ECCP_CMP_TOGGLE_LEVEL: �Ƚ�ƥ��ʱ�����ת
  *                      ECCP_CMP_ACTIVE_LEVEL: �Ƚ�ƥ��ʱ����ߵ�ƽ
  *                      ECCP_CMP_INACTIVE_LEVEL: �Ƚ�ƥ��ʱ����͵�ƽ
  *                      ECCP_CMP_SOFT_INT: �Ƚ�ƥ��ʱ�������ж�
  *                      ECCP_CMP_SPECIAL_EVENT: �Ƚ�ƥ��ʱ���������¼�
  *       CompareValue: �ȽϼĴ�����ֵ��ȡֵΪ16λ���ݡ�
  * ����  ��
  */
void
ECCP_Compare_Configuration(ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t CompareMode, uint16_t CompareValue)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CMP_MODE(CompareMode));

    /*------------------ ����ECCPX_CTL1�Ĵ��� -----------------*/
    /* ����Channel��CompareMode������ECCPxCHyMλ�� */
    tmpreg = 4 * Channel;
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << tmpreg),
                          CompareMode << tmpreg);

    /*------------------ ����ECCPXRy�Ĵ��� -----------------*/
    /* ��ȡECCPXRY�Ĵ�����ַ */
    tmpreg1 = (uint32_t)ECCPx;
    tmpreg1 = tmpreg1 + ECCP_RY_OFFSET + tmpreg;
    /* ����CompareValue������ECCPXRyλ�� */
    *(volatile uint32_t*)tmpreg1 = CompareValue;
}

/**
  * ����  ��ǿ�Ͳ�׽ģ��(ECCP)���á�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       eccpInitStruct: ��ǿ�Ͳ�׽ģ��(ECCP)������Ϣ�ṹ��ָ�롣
  * ����  ��
  */
void
ECCP_Capture_Configuration (ECCP_SFRmap* ECCPx,
                    ECCP_CaptureInitTypeDef* eccpInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(eccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CAP_MODE(eccpInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_PWMInput));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_XORMode));

    /*------------------ ����ECCPX_CTL1�Ĵ��� -----------------*/
    /* ����m_Channel��m_Mode������ECCPXCHyMλ�� */
    tmpreg = 4 * eccpInitStruct->m_Channel;
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << tmpreg),
                          eccpInitStruct->m_Mode << tmpreg);

    /*------------------ ����ECCPX_CTL2�Ĵ��� -----------------*/
    /* ����PWMInput������PXPWMIλ�� */
    /* ����m_XORMode������PXXORMλ�� */
    tmpreg = (eccpInitStruct->m_PWMInput << ECCP_CTL2_PXPWMI_POS) \
           | (eccpInitStruct->m_PWMInput << ECCP_CTL2_PXXORM_POS);
    ECCPx->ECCPXCTL2 = SFR_Config (ECCPx->ECCPXCTL2,
                          ~ECCP_CTL2_INIT_MASK,
                          tmpreg);
}

/**
  * ����  ��ʼ����ǿ�Ͳ�׽ģ��(ECCP)������Ϣ�ṹ�塣
  * ����  eccpInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  ��
  */
void
ECCP_Capture_Struct_Init (ECCP_CaptureInitTypeDef* eccpInitStruct)
{
    /* ����PWMͨ�� */
    eccpInitStruct->m_Channel = ECCP_CHANNEL_1;
    /* ����ͨ����ģʽѡ��λ */
    eccpInitStruct->m_Mode = ECCP_MODE_RST;
    /* ����PWM����ģʽ */
    eccpInitStruct->m_PWMInput = FALSE;
    /* �����������ģʽ */
    eccpInitStruct->m_XORMode = FALSE;
}

/**
  * ����  ��ǿ��PWMģ��(ECCP)���á�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       eccpInitStruct: ��ǿ��PWMģ��(ECCP)������Ϣ�ṹ��ָ�롣
  * ����  ��
  */
void
ECCP_PWM_Configuration (ECCP_SFRmap* ECCPx,
                    ECCP_PWMInitTypeDef* eccpInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(eccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_ECCP_PWM_MODE(eccpInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_ECCP_OUTPUT_MODE(eccpInitStruct->m_OutputMode));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL_OUTPUT(eccpInitStruct->m_HOutputCtl));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL_OUTPUT(eccpInitStruct->m_LOutputCtl));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_PhaseMove));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_SinglePWM));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_CloseTimer));

    /*------------------ ����ECCPX_CTL1�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel��m_Mode������ECCPXCHyMλ�� */
    tmpreg = 4 * eccpInitStruct->m_Channel;
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << tmpreg),
                          eccpInitStruct->m_Mode << tmpreg);

    /*------------------ ����ECCPX_Ry�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel����ȡCCPXRY�Ĵ�����ַ */
    tmpreg1 = (uint32_t)ECCPx;
    tmpreg1 = tmpreg1 + ECCP_RY_OFFSET + tmpreg;
    /* ���ݽṹ���Աm_DutyRatio������ECCPXRyλ�� */
    *(volatile uint32_t*)tmpreg1
        = (uint32_t)eccpInitStruct->m_DutyRatio;

    /*------------------ ����PXUDCTL�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_PhaseMove������PXMPENλ�� */
    ECCPx->PXUDCTL = SFR_Config (ECCPx->PXUDCTL,
                        ~ECCP_PXUDCTL_PXMPEN,
                        eccpInitStruct->m_PhaseMove);

    /*------------------ ����PXDTCTL�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel��m_DeadTime������PZDC��PZDCλ�� */
    if (ECCP_CHANNEL_4 == eccpInitStruct->m_Channel)
    {
        tmpreg = ((uint32_t)eccpInitStruct->m_DeadTime \
                    << ECCP_PXDTCTL_PZDC0_POS);
        ECCPx->PXDTCTL = SFR_Config (ECCPx->PXDTCTL,
                            ~ECCP_PXDTCTL_PZDC,
                            tmpreg);
    }
    else
    {
        tmpreg = ((uint32_t)eccpInitStruct->m_DeadTime \
                    << ECCP_PXDTCTL_PXDC0_POS);
        ECCPx->PXDTCTL = SFR_Config (ECCPx->PXDTCTL,
                            ~ECCP_PXDTCTL_PXDC,
                            tmpreg);
    }

    /*------------------ ����PXATRCTL�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel��m_OutputMode������PxCHyMODλ�� */
    tmpreg = eccpInitStruct->m_Channel;
    ECCPx->PXATRCTL = SFR_Config (ECCPx->PXATRCTL,
                          ~(ECCP_PXATRCTL_PXCH1MOD << tmpreg),
                          (uint32_t)eccpInitStruct->m_OutputMode << tmpreg);

    /*------------------ ����PWMXOC�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_Channel��m_HOutputCtl��m_LOutputCtl��
       ����PxOCyH��PxOCyLλ�� */
    tmpreg = 4 * eccpInitStruct->m_Channel;
    tmpreg1 = ((uint32_t)eccpInitStruct->m_HOutputCtl << ECCP_PWMXOC_PXOC1H0_POS) \
            | ((uint32_t)eccpInitStruct->m_LOutputCtl << ECCP_PWMXOC_PXOC1L0_POS);
    ECCPx->PWMXOC = SFR_Config (ECCPx->PWMXOC,
                          ~((ECCP_PWMXOC_PXOC1L | ECCP_PWMXOC_PXOC1H) \
                            << tmpreg),
                          tmpreg1 << tmpreg);

    /*------------------ ����ECCPX_CTL2�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_SinglePWM������PxSPMλ�� */
    tmpreg = (eccpInitStruct->m_SinglePWM << ECCP_CTL2_PXSPM_POS);
    ECCPx->ECCPXCTL2 = SFR_Config (ECCPx->ECCPXCTL2,
                          ~(ECCP_CTL2_PXSPM),
                          tmpreg);

    /*------------------ ����ECCPX_CTL3�Ĵ��� -----------------*/
    /* ���ݽṹ���Աm_CloseTimer������PxSPMSTλ�� */
    tmpreg = (eccpInitStruct->m_CloseTimer << ECCP_CTL3_PXSPMST_POS);
    ECCPx->ECCPXCTL3 = SFR_Config (ECCPx->ECCPXCTL3,
                          ~ECCP_CTL3_PXSPMST,
                          tmpreg);
}

/**
  * ����  ��ʼ����ǿ��PWMģ��(ECCP)������Ϣ�ṹ�塣
  * ����  eccpInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  ��
  */
void
ECCP_PWM_Struct_Init (ECCP_PWMInitTypeDef* eccpInitStruct)
{
    /* ����PWMͨ�� */
    eccpInitStruct->m_Channel = ECCP_CHANNEL_1;
    /* ����ͨ����ģʽѡ��λ */
    eccpInitStruct->m_Mode = ECCP_MODE_RST;
    /* ����PWMռ�ձ� */
    eccpInitStruct->m_DutyRatio = 0x0000;
    /* ������������ */
    eccpInitStruct->m_DeadTime = 0x00;
    /* ����ͨ�����ģʽ */
    eccpInitStruct->m_OutputMode = ECCP_OUTPUT_INDEPENDENT;
    /* ����ͨ��H������� */
    eccpInitStruct->m_HOutputCtl = ECCP_CHANNEL_OUTPUT_PWM_ACTIVE;
    /* ����ͨ��L������� */
    eccpInitStruct->m_LOutputCtl = ECCP_CHANNEL_OUTPUT_PWM_ACTIVE;
    /* ������λ�ƶ�ʹ�� */
    eccpInitStruct->m_PhaseMove = FALSE;
    /* ���õ��������ģʽ */
    eccpInitStruct->m_SinglePWM = FALSE;
    /* ���õ��������ģʽѡ�� */
    eccpInitStruct->m_CloseTimer = FALSE;
}

/**
  * ����  ����ECCP��׽���ܡ�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       EdgeConfig: ƥ����أ�ȡֵ��ΧΪ:
  *                     ECCP_MODE_RST: �رղ�׽����
  *                     ECCP_CAP_RISING_EDGE: ÿ���½��ط�����׽
  *                     ECCP_CAP_FALLING_EDGE: ÿ�������ط�����׽
  *                     ECCP_CAP_4TH_RISING_EDGE: ÿ4�������ط�����׽
  *                     ECCP_CAP_16TH_RISING_EDGE: ÿ16�������ط�����׽
  * ����  ��
  */
void
ECCP_Capture_Mode_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t EdgeConfig)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CAP_MODE(EdgeConfig));

    /*-------------------- ����ECCPX_CTL1�Ĵ��� --------------------*/
    tmpreg = EdgeConfig << (4 * Channel);
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * ����  ����ECCP�ȽϹ��ܡ�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: �Ƚ�ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       CmpConfig: �Ƚ�ƥ��ģʽ��ȡֵ��ΧΪ:
  *                    ECCP_MODE_RST: �رձȽϹ���
  *                    ECCP_CMP_TOGGLE_LEVEL: ƥ��ʱ�����ƽ��ת
  *                    ECCP_CMP_ACTIVE_LEVEL: �Ƚ�ƥ��ʱ����ߵ�ƽ
  *                    ECCP_CMP_INACTIVE_LEVEL: �Ƚ�ƥ��ʱ����͵�ƽ
  *                    ECCP_CMP_SOFT_INT: �Ƚ�ƥ��ʱ��������ж�
  *                    ECCP_CMP_SPECIAL_EVENT: ���������¼�
  * ����  ��
  */
void
ECCP_Compare_Mode_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t CmpConfig)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CMP_MODE(CmpConfig));

    /*-------------------- ����ECCPX_CTL1�Ĵ��� --------------------*/
    tmpreg = CmpConfig << (4 * Channel);
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * ����  ����ECCP PWM����,ģʽѡ�����ɣ�Эͬ����ʱ��ģʽ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       PWMConfig: PWMƥ��ģʽ��ȡֵ��ΧΪ:
  *                    ECCP_MODE_RST: �رձȽϹ���
  *                    ECCP_PWM_FREE: PWM����ģʽ
  *                    ECCP_PWM_COORDINATION: PWMЭͬģʽ
  *                    ECCP_PWM_SINGLE: PWM��ʱ��ģʽ
  * ����  ��
  */
void
ECCP_PWM_Mode_Config (ECCP_SFRmap* ECCPx, uint32_t PWMConfig)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_PWM_MODE(PWMConfig));

    /*-------------------- ����ECCPX_CTL1�Ĵ��� --------------------*/
    tmpreg = PWMConfig << ECCP_CTL1_CH1M0_POS;
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~ECCP_CTL1_CH1M,
                          tmpreg);
}

/**
  * ����  ��ȡECCP��׽�Ĵ�����
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  ��׽�Ĵ�����16λֵ��
  */
uint16_t
ECCP_Get_Capture_Result (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /* ��ȡ�Ĵ������ʵ�ַ */
    tmpreg = (uint32_t)ECCPx;
    tmpreg = tmpreg + ECCP_CY_OFFSET + (4 * Channel);

    /* ͨ��16λ����ָ���ȡ�Ĵ�����16λ��ֵ */
    return (*(volatile const uint16_t*)  tmpreg);
}

/**
  * ����  ��ȡECCP�Ƚ�/EPWMռ�ձȼĴ�����
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *               ȡֵT5_SFR/T9_SFR
  *               ��ECCP5_SFR/ECCP9_SFR
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  �Ƚ�/PWMռ�ձȼĴ�����ֵ��
  */
uint16_t
ECCP_Get_Compare_Result (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
      CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
      CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));


    /* ��ȡ�Ĵ������ʵ�ַ */
    tmpreg = (uint32_t)ECCPx;
    tmpreg = tmpreg + ECCP_RY_OFFSET + (4 * Channel);

    /* ͨ��16λ����ָ���ȡ�Ĵ�����16λ��ֵ */
    return (*(volatile const uint16_t*)  tmpreg);
}



/**
  * ����  ����ECCP�Ƚ�/PWMռ�ձȼĴ�����
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  �ޡ�
  */
void
ECCP_Set_Compare_Result (ECCP_SFRmap* ECCPx, uint32_t Channel, uint16_t Value)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /* ��ȡCCPXRY�Ĵ�����ַ */
    tmpreg = (uint32_t)ECCPx;
    tmpreg = tmpreg + ECCP_RY_OFFSET + (4 * Channel);

    /*-------------------- ����ECCPX_RY�Ĵ��� --------------------*/
    *(volatile uint32_t*)tmpreg = (uint16_t)Value;
}

/**
  * ����  ������׽/�Ƚ��¼�����λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       NewState: ������׽/�Ƚ��¼�����״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ECCP_Generate_Trigger_Config (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------------ ����ECCPX_EGIF�Ĵ���CCyGλ ------------------*/
    if (NewState != FALSE)
    {
        /* ��ͨ��Channel�ϲ���һ����׽/�Ƚ��¼� */
        ECCPx->ECCPXEGIF |= ECCP_EGIF_CC1G << Channel;
    }
    else
    {
        /* �޶��� */
        ECCPx->ECCPXEGIF &= ~(ECCP_EGIF_CC1G << Channel);
    }
}

/**
  * ����  PWM�������ģʽʹ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: PWM�������ģʽʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ECCP_PWM_Input_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_CTL2�Ĵ���PXPWMIλ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PWM�������ģʽ */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXPWMI_POS);
    }
    else
    {
        /* δʹ��PWM�������ģʽ */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXPWMI_POS);
    }
}

/**
  * ����  �������ʹ��λ����
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: �������ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ECCP_Input_XOR_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCP_CTL2�Ĵ���PXXORMλ --------------------*/
    if (NewState != FALSE)
    {
        /* ʹ��PWM�������ģʽ */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXXORM_POS);
    }
    else
    {
        /* δʹ��PWM�������ģʽ */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXXORM_POS);
    }
}

/**
  * ����  ���õ��������ģʽ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: ���������ģʽʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ECCP_Single_Pulse_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_CTL2�Ĵ���PXSPMλ --------------------*/
    if (NewState != FALSE)
    {
        /* ���������ģʽʹ�� */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXSPM_POS);
    }
    else
    {
        /* ���������ģʽδʹ�� */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXSPM_POS);
    }
}

/**
  * ����  ���õ��������ģʽѡ��
  * ����  ECCPx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: ���������ģʽѡ��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ECCP_Single_Pulse_Shut_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_CTL3�Ĵ���PXSPMSTλ --------------------*/
    if (NewState != FALSE)
    {
        /* �����������رն�ʱ��ʹ��λ */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL3, ECCP_CTL3_PXSPMST_POS);
    }
    else
    {
        /* ����������󲻹رն�ʱ��ʹ��λ */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL3, ECCP_CTL3_PXSPMST_POS);
    }
}

/**
  * ����  ����PWMͨ��1/2/3/4����ʹ��λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: PWMͨ��1/2/3/4����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ECCP_PWM_Restart_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_CTL3�Ĵ���PXRSENλ --------------------*/
    if (NewState != FALSE)
    {
        /* �Զ��ر�ʱ��һ���ر��¼���ʧ��PxASEλ���Զ����㣬PWM�Զ����� */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL3, ECCP_CTL3_PXRSEN_POS);
    }
    else
    {
        /* �Զ��ر�ʱ��PxASE��������㣬������PWM */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL3, ECCP_CTL3_PXRSEN_POS);
    }
}

/**
  * ����  ����PWMͨ��������ʱʱ�䡣
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       DeadTime��������ʱʱ�䣬ȡֵΪ8λ���ݡ�
  * ����  ��
  */
void
ECCP_Dead_Time_Config (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint8_t DeadTime)
{
    uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*-------------------- ����ECCPX_PxDTCTL�Ĵ��� --------------------*/
    if (ECCP_CHANNEL_4 == Channel)
    {
        /* ����ͨ��4��������ʱ��ʱ�� */
        tmpreg = (uint32_t)DeadTime << ECCP_PXDTCTL_PZDC0_POS;
        ECCPx->PXDTCTL = SFR_Config (ECCPx->PXDTCTL,
                             ~ECCP_PXDTCTL_PZDC,
                             tmpreg);
    }
    else
    {
        /* ����ͨ��1/2/3��������ʱ��ʱ�� */
        tmpreg = (uint32_t)DeadTime << ECCP_PXDTCTL_PXDC0_POS;
        ECCPx->PXDTCTL = SFR_Config (ECCPx->PXDTCTL,
                             ~ECCP_PXDTCTL_PXDC,
                             tmpreg);
    }
}

/**
  * ����  ����ECCPͨ��������ơ�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       Port: �˿�ѡ��ȡֵ��ΧΪ:
  *               ECCP_PORT_LOW: CHxL�˿�
  *               ECCP_PORT_HIGH: CHxH�˿�
  *       ChannelOutputCtl: ͨ���˿�������ƣ�ȡֵ��ΧΪ:
  *                           ECCP_CHANNEL_OUTPUT_PWM_ACTIVE: PWM���������Ч
  *                           ECCP_CHANNEL_OUTPUT_PWM_INACTIVE: PWM���������Ч
  *                           ECCP_CHANNEL_OUTPUT_INACTIVE: ǿ�Ƶ͵�ƽ���
  *                           ECCP_CHANNEL_OUTPUT_ACTIVE: ǿ�Ƹߵ�ƽ���
  * ����  �ޡ�
  */
void
ECCP_Channel_Output_Control (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t ChannelOutputCtl)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_PORT(Port));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL_OUTPUT(ChannelOutputCtl));

    /*--------------- ����ECCPX_PWMXOC�Ĵ���PXOCyH/PXOCyLλ ---------------*/
    tmpreg = ChannelOutputCtl << (Port + Channel * 4);
    ECCPx->PWMXOC = SFR_Config (ECCPx->PWMXOC,
                          ~(ECCP_PWMXOC_PXOC1L << (Port + Channel * 4)),
                          tmpreg);
}

/**
  * ����  ����ECCPͨ�����ģʽ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       ChannelOutputMode: PWMͨ�����ģʽ��ȡֵ��ΧΪ:
  *                            ECCP_OUTPUT_INDEPENDENT: �������ģʽ
  *                            ECCP_OUTPUT_COMPLEMENTARY: �������ģʽ
  * ����  �ޡ�
  */
void
ECCP_Channel_Output_Mode (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t ChannelOutputMode)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_OUTPUT_MODE(ChannelOutputMode));

    /*----------------- ����ECCPX_PXATRCTL�Ĵ���PXCHyMODλ -----------------*/
    tmpreg = ChannelOutputMode << (ECCP_PXATRCTL_PXCH1MOD_POS + Channel);
    ECCPx->PXATRCTL = SFR_Config (ECCPx->PXATRCTL,
                          ~(ECCP_PXATRCTL_PXCH1MOD << Channel),
                          tmpreg);
}

/**
  * ����  ����ͨ���Զ��ر��¼�״̬λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       WorkingState: ͨ���Զ��ر��¼�״̬��ȡֵ��ΧΪ:
  *                       ECCP_CHANNEL_WORKING_STATE: ͨ����������
  *                       ECCP_CHANNEL_SHUTDOWN_STATE: ͨ���ر�
  * ����  �ޡ�
  */
void
ECCP_Channel_Work_State_Config (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t WorkingState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL_STATE(WorkingState));

    /*-------------------- ����ECCPX_PXASCTL0�Ĵ���PXASEyλ --------------------*/
    tmpreg = WorkingState << (ECCP_PXASCTL0_PXASE1_POS + Channel);
    ECCPx->PXASCTL0 = SFR_Config (ECCPx->PXASCTL0,
                          ~(ECCP_PXASCTL0_PXASE1 << Channel),
                          tmpreg);
}

/**
  * ����  ��ȡͨ���Զ��ر��¼�״̬λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  ͨ���Զ��ر��¼�״̬�������ر��¼�����1��������������0��
  */
FlagStatus
ECCP_Get_Channel_Work_State (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*--------------- ��ȡECCPX_PXASCTL0�Ĵ���PXASEyλ ---------------*/
    tmpreg = ECCP_PXASCTL0_PXASE1 << Channel;
    if (ECCPx->PXASCTL0 & tmpreg)
    {
        /* �����˹ر��¼�,ͨ��Ϊ�ر�״̬ */
        return SET;
    }
    else
    {
        /* ͨ���������� */
        return RESET;
    }
}

/**
  * ����  ����ͨ���Զ��ر�Դѡ��λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       ShutDownSignal: ͨ���Զ��ر�Դ��ȡֵ��ΧΪ:
  *                         ECCP_CHANNEL_SHUTDOWN_FORBID: ��ֹ�Զ��ض�
  *                         ECCP_CHANNEL_CMP2CMP3_ACTIVE: �Ƚ���2/3����ߵ�ƽ
  *                         ECCP_CHANNEL_BKIN_INACTIVE: ECCP_BKIN�����ϵĵ͵�ƽ
  *       ע�⣺�Ƚ���3�����Ϊ ECCP5/9 ͨ�� 1/2/3 �Զ��ضϵĴ����ź�(�ߵ�ƽ�ض�)
  *       		�Ƚ��� 2/3 �������Ϊ ECCP5/9 ͨ��4 �Զ��ضϵĴ����ź�(�ߵ�ƽ�ض�)
  * ����  �ޡ�
  */
void
ECCP_CHANNEL4_Shutdown_SEL (ECCP_SFRmap* ECCPx,
                    uint32_t ShutDownSignal)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_CHANNEL4_SHUTDOWN_SIGNAL(ShutDownSignal));


    if (ShutDownSignal != CMP3_OUTPUT_SHUNT)
    {
        /* �Զ��ر�ʱ��һ���ر��¼���ʧ��PxASEλ���Զ����㣬PWM�Զ����� */
        SFR_SET_BIT_ASM(ECCPx->PXASCTL, ECCP_PXASCTL_PXASS_POS);
    }
    else
    {
        /* �Զ��ر�ʱ��PxASE��������㣬������PWM */
        SFR_CLR_BIT_ASM(ECCPx->PXASCTL, ECCP_PXASCTL_PXASS_POS);
    }
}
/**
  * ����  ����ͨ���Զ��ر�Դѡ��λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       ShutDownSignal: ͨ���Զ��ر�Դ��ȡֵ��ΧΪ:
  *                         ECCP_CHANNEL_SHUTDOWN_FORBID: ��ֹ�Զ��ض�
  *                         ECCP_CHANNEL_CMP2CMP3_ACTIVE: �Ƚ���2/3����ߵ�ƽ
  *                         ECCP_CHANNEL_BKIN_INACTIVE: ECCP_BKIN�����ϵĵ͵�ƽ
  *       ע�⣺�Ƚ���3�����Ϊ ECCP5/9 ͨ�� 1/2/3 �Զ��ضϵĴ����ź�(�ߵ�ƽ�ض�)
  *       		�Ƚ��� 2/3 �������Ϊ ECCP5/9 ͨ��4 �Զ��ضϵĴ����ź�(�ߵ�ƽ�ض�)
  * ����  �ޡ�
  */
void
ECCP_Channel_Shutdown_Signal (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t ShutDownSignal)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_SHUTDOWN_SIGNAL(ShutDownSignal));

    /*-------------------- ����ECCPX_PXASCTL0�Ĵ���PXASSyλ --------------------*/
    tmpreg = ShutDownSignal << (Channel * 2);
    ECCPx->PXASCTL0 = SFR_Config (ECCPx->PXASCTL0,
                          ~(ECCP_PXASCTL0_PXASS1 << (Channel * 2)),
                          tmpreg);
}

/**
  * ����  ��������ECCPxCHyH/ECCPxCHyL�ر�״̬����λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       Port: �˿�ѡ��ȡֵ��ΧΪ:
  *               ECCP_PORT_LOW: CHxL�˿�
  *               ECCP_PORT_HIGH: CHxH�˿�
  *       ChannelPinCtl: ����״̬��ȡֵ��ΧΪ:
  *                        PIN_INACTIVE: ��������Ϊ�͵�ƽ
  *                        PIN_ACTIVE: ��������Ϊ�ߵ�ƽ
  *                        PIN_TRISTATE: ��������Ϊ��̬
  * ����  �ޡ�
  */
void
ECCP_Channel_Pin_Ctl (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t ChannelPinCtl)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_PORT(Port));
    CHECK_RESTRICTION(CHECK_PIN_STATE(ChannelPinCtl));

    /*--------------- ����ECCPX_PXASCTL1�Ĵ���PxSSyH/PxSSyLλ ---------------*/
    tmpreg = ChannelPinCtl << (Port + Channel * 4);
    ECCPx->PXASCTL1 = SFR_Config (ECCPx->PXASCTL1,
                          ~(ECCP_PXASCTL1_PXSS1L << (Port + Channel * 2)),
                          tmpreg);
}

/**
  * ����  ���������ʱ�ӡ�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       ZeroClock: �����ʱ�ӣ�ȡֵ��ΧΪ:
  *                    ECCP_ZERO_CLOCK_DIV_1: Ԥ��Ƶʱ��
  *                    ECCP_ZERO_CLOCK_DIV_2: Ԥ��Ƶʱ�� / 2
  *                    ECCP_ZERO_CLOCK_DIV_4: Ԥ��Ƶʱ�� / 4
  *                    ECCP_ZERO_CLOCK_DIV_8: Ԥ��Ƶʱ�� / 8
  *                    ECCP_ZERO_CLOCK_DIV_16: Ԥ��Ƶʱ�� / 16
  *                    ECCP_ZERO_CLOCK_DIV_32: Ԥ��Ƶʱ�� / 32
  *                    ECCP_ZERO_CLOCK_DIV_64: Ԥ��Ƶʱ�� / 64
  *                    ECCP_ZERO_CLOCK_DIV_128: Ԥ��Ƶʱ�� / 128
  *                    ECCP_ZERO_CLOCK_DIV_256: Ԥ��Ƶʱ�� / 256
  *                    ECCP_ZERO_CLOCK_DIV_512: Ԥ��Ƶʱ�� / 512
  * ����  �ޡ�
  */
void
ECCP_Zero_Clock_Config (ECCP_SFRmap* ECCPx,
                    uint32_t ZeroClock)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_ZERO_CLOCK(ZeroClock));

    /*-------------------- ����ECCPX_ZPDCTL0�Ĵ���ZCLKλ --------------------*/
    ECCPx->ZPDCTL0 = SFR_Config (ECCPx->ZPDCTL0,
                          ~(ECCP_ZPDCTL0_ZCLK),
                          ZeroClock);
}

/**
  * ����  ��������ECCPxCHyH/ECCPxCHyL��̬����λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       Port: �˿�ѡ��ȡֵ��ΧΪ:
  *               ECCP_PORT_LOW: CHxL�˿�
  *               ECCP_PORT_HIGH: CHxH�˿�
  *       PinTristateCtl: ����״̬��ȡֵ��ΧΪ:
  *                        PIN_INACTIVE: ��������Ϊ�͵�ƽ
  *                        PIN_ACTIVE: ��������Ϊ�ߵ�ƽ
  *                        PIN_TRISTATE: ��������Ϊ��̬
  * ����  �ޡ�
  */
void
ECCP_Channel_Pin_Tristate_Enable (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t PinTristateCtl)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_PORT(Port));
    CHECK_RESTRICTION(CHECK_PIN_STATE(PinTristateCtl));

    /*------------- ����ECCPX_ZPDPORT�Ĵ���ZPORTλ -------------*/
    tmpreg = (Port / 2) + (Channel * 2);
    tmpreg1 = PinTristateCtl << tmpreg;
    ECCPx->ZPDPORT = SFR_Config (ECCPx->ZPDPORT,
                          ~(ECCP_ZPDPORT_ZPORT << tmpreg),
                          tmpreg1);
}

/**
  *   ##### ��ǿ�Ͳ�׽/�Ƚ�/PWM����(ECCP)��׽���ܺ���������� #####
  */

/**
  *   ##### ��ǿ�Ͳ�׽/�Ƚ�/PWM����(ECCP)�жϼ���־���������� #####
  */
/**
  * ����  ͨ������/�Ƚϵ��ж�ʹ�����á�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       NewState: ͨ������/�Ƚϵ��ж�ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  �ޡ�
  */
void
ECCP_Channel_INT_Enable (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_IE�Ĵ���CCxIEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����CCPͨ��Channel��׽/�Ƚ��ж� */
        ECCPx->ECCPXIE |= ECCP_IE_CC1IE << Channel;
    }
    else
    {
        /* ��ֹCCPͨ��Channel��׽/�Ƚ��ж� */
        ECCPx->ECCPXIE &= ~(ECCP_IE_CC1IE << Channel);
    }
}

/**
  * ����  �ض��¼���DMA����ʹ��λ
  * ����  ECCPx: ָ��CCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: �ض��¼���DMA����ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  ��
  */
void
ECCP_X_Turn_off_DMA_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_DE�Ĵ���CLSDEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����ض��¼���DMA���� */
        SFR_SET_BIT_ASM(ECCPx->ECCPXDE, ECCP_DE_CLSDE_POS);
    }
    else
    {
        /* ��ֹ�ض��¼���DMA���� */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXDE, ECCP_DE_CLSDE_POS);
    }
}

/**
  * ����  ͨ������/�Ƚϵ�DMA��������ʹ��λ���á�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       NewState: ͨ������/�Ƚϵ�DMA��������ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  �ޡ�
  */
void
ECCP_Channel_DMA_Enable (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_DE�Ĵ���CHxDEλ --------------------*/
    if (NewState != FALSE)
    {
        /* ������/�Ƚ�ͨ��x��DMA���� */
        ECCPx->ECCPXDE |= ECCP_DE_CH1DE << Channel;
    }
    else
    {
        /* ��ֹ����/�Ƚ�ͨ��x��DMA���� */
        ECCPx->ECCPXDE &= ~(ECCP_DE_CH1DE << Channel);
    }
}

/**
  * ����  ��ȡECCPͨ����׽/�Ƚ��жϱ�־��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ECCP_Get_Channel_Trigger_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));

    /*-------------------- ��ȡECCPXEGIF�Ĵ���CCxIFλ --------------------*/
    tmpreg = ECCP_EGIF_CC1IF << Channel;
    if (ECCPx->ECCPXEGIF & tmpreg)
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
  * ����  ��ȡ�ض��¼�����DMA��־��
  * ����  ATIMx: ָ��ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵT5_SFR/T9_SFR��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ECCP_X_Get_Turn_off_DMA_Flag (ATIM_SFRmap* ATIMx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- ����ECCPX_DF�Ĵ���CLSDFλ --------------------*/
    if (ATIMx->ECCPXDF & ECCP_DF_CLSDF)
    {
        /* �����ض��¼�����DMA��־ */
        return SET;
    }
    else
    {
        /* δ�����ض��¼�����DMA��־ */
        return RESET;
    }
}

/**
  * ����  ��ȡECCPͨ����׽/�Ƚ�DMA�жϱ�־��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
ECCP_Get_Trigger_DMA_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));

    /*-------------------- ��ȡECCPXEGIF�Ĵ���CHxDFλ --------------------*/
    tmpreg = ECCP_DF_CH1DF << Channel;
    if (ECCPx->ECCPXDF & tmpreg)
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
  * ����  ���ECCPxͨ����׽/�Ƚ��жϱ�־��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  �ޡ�
  */
void
ECCP_Clear_Channel_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*-------------------- ����ECCPX_SRIC�Ĵ���CCxICλ --------------------*/
    tmpreg = ECCP_SRIC_CC1IC << Channel;
    tmpreg1 = ECCP_EGIF_CC1IF << Channel;
    ECCPx->ECCPXSRIC |= tmpreg;

    while((ECCPx->ECCPXEGIF & tmpreg1)>>(ECCP_EGIF_CC1IF_POS+Channel));

    ECCPx->ECCPXSRIC &= ~tmpreg;
}

/**
  * ����  PWM��λ�ƶ�ʹ��λ���á�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       NewState: PWM��λ�ƶ�ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  �ޡ�
  */
void
ECCP_PWM_Move_Phase_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_PxUDCTL�Ĵ���PxMPENλ --------------------*/
    if (NewState != FALSE)
    {
        /* ����PWM�Զ����ƹ��� */
        SFR_SET_BIT_ASM(ECCPx->PXUDCTL, ECCP_PXUDCTL_PXMPEN_POS);
    }
    else
    {
        /* �ر�PWM�Զ����ƹ��� */
        SFR_CLR_BIT_ASM(ECCPx->PXUDCTL, ECCP_PXUDCTL_PXMPEN_POS);
    }
}

/**
  * ����  ����ͨ���������ʱ����ơ�
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       NewState: ͨ������/�Ƚϵ��ж�ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  �ޡ�
  */
void
ECCP_Channel_Zero_Detect_Sequential_Ctl (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_ZPDCTL0�Ĵ���ZPDyTWINλ --------------------*/
    tmpreg = NewState << (Channel + ECCP_ZPDCTL0_ZPD1TWIN_POS);
    ECCPx->ZPDCTL0 = SFR_Config (ECCPx->ZPDCTL0,
                          ~(ECCP_ZPDCTL0_ZPD1TWIN << Channel),
                          tmpreg);
}

/**
  * ����  ��ȡͨ�������ĸ�Ӧ��ѹ���״̬��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  ͨ�������ĸ�Ӧ��ѹ���״̬��
  *       δ��⵽��Ӧ��ѹ����⵽0�㣩����1��
  *       ��⵽��Ӧ��ѹ��δ��⵽0�㣩����0��
  */
FlagStatus
ECCP_Get_Channel_Zero_Detection_State (ECCP_SFRmap* ECCPx,
                    uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*--------------- ��ȡECCPX_ZPDCTL0�Ĵ���ZPDyFLAGλ ---------------*/
    tmpreg = ECCP_ZPDCTL0_ZPD1FLAG << Channel;
    if (ECCPx->ZPDCTL0 & tmpreg)
    {
        /* δ��⵽��Ӧ��ѹ����⵽0�㣩 */
        return SET;
    }
    else
    {
        /* ��⵽��Ӧ��ѹ��δ��⵽0�㣩 */
        return RESET;
    }
}

/**
  * ����  ����ͨ�������ĸ�Ӧ��ѹ���״̬��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  * ����  �ޡ�
  */
void
ECCP_Clear_Channel_Zero_Detection_State (ECCP_SFRmap* ECCPx,
                    uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*--------------- ����ECCPX_ZPDCTL0�Ĵ���ZPDyFLAGλ ---------------*/
    tmpreg = ECCP_ZPDCTL0_ZPD1FLAG << Channel;
    ECCPx->ZPDCTL0 &= ~tmpreg;
}

/**
  * ����  ����ͨ���������Ƚ���ʹ��λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       NewState: ͨ������/�Ƚϵ��ж�ʹ��״̬��
  *                 ȡֵ��ΧΪ��TRUE �� FALSE
  * ����  �ޡ�
  */
void
ECCP_Channel_Zero_Detect_Enable (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- ����ECCPX_ZPDCTL1�Ĵ���ZPDyENλ --------------------*/
    tmpreg = NewState << (Channel + ECCP_ZPDCTL1_ZPD1EN_POS);
    ECCPx->ZPDCTL1 = SFR_Config (ECCPx->ZPDCTL1,
                          ~(ECCP_ZPDCTL1_ZPD1EN << Channel),
                          tmpreg);
}

/**
  * ����  ����ͨ��������ѹѡ��λ��
  * ����  ECCPx: ָ��ECCP��߼���ʱ���ڴ�ṹ��ָ�룬
  *              ȡֵECCP5_SFR/ECCP9_SFR��
  *       Channel: ��׽ͨ��ѡ��ȡֵ��ΧΪ:
  *                  ECCP_CHANNEL_1: ͨ��1
  *                  ECCP_CHANNEL_2: ͨ��2
  *                  ECCP_CHANNEL_3: ͨ��3
  *                  ECCP_CHANNEL_4: ͨ��4
  *       ZeroDetectVoltage: ͨ��������ѹ��ȡֵ��ΧΪ��
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_15: 0.15V
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_25: 0.25V
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_35: 0.35V
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_45: 0.45V
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_55: 0.55V
  * ����  �ޡ�
  */
void
ECCP_Channel_Zero_Voltage_Config (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t ZeroDetectVoltage)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_ZERO_VOLTAGE(ZeroDetectVoltage));

    /*-------------------- ����ECCPX_ZPDCTL1�Ĵ���ZPDyENλ --------------------*/
    tmpreg = ZeroDetectVoltage \
             << (Channel * 3 + ECCP_ZPDCTL1_ZPD1S0_POS);
    ECCPx->ZPDCTL1 = SFR_Config (ECCPx->ZPDCTL1,
                          ~(ECCP_ZPDCTL1_ZPD1S << (Channel * 3)),
                          tmpreg);
}

/**
  *   ##### ��ǿ�Ͳ�׽/�Ƚ�/PWM����(ECCP)�жϼ���־������������� #####
  */


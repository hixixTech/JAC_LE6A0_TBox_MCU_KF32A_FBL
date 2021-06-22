/**
  ******************************************************************************
  * �ļ���  kf32a_basic_i2c.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ���ڲ����ɵ�·�ӿ�(I2C)���蹦�ܺ�����������
  *          + �ڲ����ɵ�·�ӿ�(I2C)��ʼ������
  *          + �ڲ����ɵ�·�ӿ�(I2C)�������ú���
  *          + �ڲ����ɵ�·�ӿ�(I2C)�жϹ�����
  *
  *********************************************************************
  */

#include "kf32a_basic_i2c.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* I2C˽�ж��� ----------------------------------------------------*/
/* I2C_CTLR�Ĵ��������������� */
#define I2C_CTLR_INIT_MASK              (I2C_CTLR_SMBUS \
                                       | I2C_CTLR_I2CCKS \
                                       | I2C_CTLR_BADR10 \
                                       | I2C_CTLR_SMBT \
                                       | I2C_CTLR_ACKEN \
                                       | I2C_CTLR_ACKDT)


/**
  *   ##### �ڲ����ɵ�·�ӿ�(I2C)��ʼ������ #####
  */
/**
  * ����  I2C���踴λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Reset (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
#ifdef KF32A_Periph_i2c0
    if (I2Cx == I2C0_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_I2C0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_i2c1
    if (I2Cx == I2C1_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_I2C1CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_i2c2
    if (I2Cx == I2C2_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C2RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C2RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_I2C2CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_i2c3
    if (I2Cx == I2C3_SFR)
    {
        RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_I2C3RST, TRUE);
        RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_I2C3RST, FALSE);
        PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_I2C3CLKEN, TRUE);
    }
#endif

}

/**
  * ����  I2C�������á�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       i2cInitStruct: I2Cģ��������Ϣ�ṹ��ָ�롣
  * ����  �ޡ�
  */
void
I2C_Configuration (I2C_SFRmap* I2Cx, I2C_InitTypeDef* i2cInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_MODE(i2cInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_I2C_CLK(i2cInitStruct->m_ClockSource));
    CHECK_RESTRICTION(CHECK_I2C_BUFR_ADDRESS(i2cInitStruct->m_BADR10));
    CHECK_RESTRICTION(CHECK_SMBUS_MODE(i2cInitStruct->m_MasterSlave));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(i2cInitStruct->m_AckEn));
    CHECK_RESTRICTION(CHECK_I2C_ACKDATA(i2cInitStruct->m_AckData));

    /*---------------------------- I2Cx_CTLR�Ĵ�������  -----------------*/
    /* ���ݽṹ���Աm_Mode������SMBUSλ�� */
    /* ���ݽṹ���Աm_ClockSource������I2CCKSλ�� */
    /* ���ݽṹ���Աm_BADR10������BADR10λ�� */
    /* ���ݽṹ���Աm_MasterSlave������SMBTλ�� */
    /* ���ݽṹ���Աm_AckEn������ACKENλ�� */
    /* ���ݽṹ���Աm_AckData������ACKDTλ�� */
	tmpreg = ((i2cInitStruct->m_Mode)    \
			| (i2cInitStruct->m_ClockSource)   \
			| (i2cInitStruct->m_BADR10)  \
			| (i2cInitStruct->m_MasterSlave) \
			| (i2cInitStruct->m_AckEn << I2C_CTLR_ACKEN_POS) \
			| (i2cInitStruct->m_AckData));
    I2Cx->CTLR = SFR_Config (I2Cx->CTLR, ~I2C_CTLR_INIT_MASK, tmpreg);

    /*---------------------------- I2Cx_BRGR�Ĵ�������  -----------------*/
    /* ���ݽṹ���Աm_BaudRateL������I2CBRGLλ�� */
    /* ���ݽṹ���Աm_BaudRateH������I2CBRGHλ�� */
    tmpreg = ((i2cInitStruct->m_BaudRateL << I2C_BRGR_I2CBRGL0_POS) \
            | (i2cInitStruct->m_BaudRateH << I2C_BRGR_I2CBRGH0_POS));
    I2Cx->BRGR = SFR_Config (I2Cx->BRGR,
                        ~(I2C_BRGR_I2CBRGL | I2C_BRGR_I2CBRGH),
                        tmpreg);
}

/**
  * ����  ��ʼ��I2C������Ϣ�ṹ�塣
  * ����  dacInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void I2C_Struct_Init (I2C_InitTypeDef* I2C_InitStruct)
{
    /* ��ʼ�� I2Cģʽѡ��*/
    I2C_InitStruct->m_Mode = I2C_MODE_I2C;

    /* ��ʼ�� I2Cʱ��ѡ�� */
    I2C_InitStruct->m_ClockSource = I2C_CLK_SCLK;

    /* ��ʼ�� I2C��ַѡ�� */
    I2C_InitStruct->m_BADR10 = I2C_BUFRADDRESS_7BIT;

    /* ��ʼ�� SMBus����ѡ�� */
    I2C_InitStruct->m_MasterSlave = I2C_MODE_SMBUSDEVICE;

    /* ��ʼ�� SCL�͵�ƽռ�õ�ʱ�������� */
    I2C_InitStruct->m_BaudRateL = 4;

    /* ��ʼ�� SCL�ߵ�ƽռ�õ�ʱ�������� */
    I2C_InitStruct->m_BaudRateH = 4;

    /* ��ʼ�� I2CӦ��ʹ������ */
    I2C_InitStruct->m_AckEn = FALSE;

    /* ��ʼ�� I2CӦ������λ */
    I2C_InitStruct->m_AckData = I2C_ACKDATA_ACK;
}
/**
  *   ##### �ڲ����ɵ�·�ӿ�(I2C)��ʼ������������� #####
  */


/**
  *   ##### �ڲ����ɵ�·�ӿ�(I2C)�������ú��� #####
  */
/**
  * ����  ����I2Cʹ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2Cʹ��λ������Ϣ��ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Cmd(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���I2CENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_I2CEN_POS);
    }
    else
    {
        /* ��ֹI2C */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_I2CEN_POS);
    }
}

/**
  * ����  ����I2C10λ��ַʹ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2Cʹ��λ������Ϣ��
  *                 ȡֵΪ I2C_BUFRADDRESS_10BIT �� I2C_BUFRADDRESS_7BIT��
  * ����  �ޡ�
  */
void
I2C_Bufr_Address_Config(I2C_SFRmap* I2Cx, uint32_t NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_BUFR_ADDRESS(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���BADR10λ ----------------*/
    if (NewState != I2C_BUFRADDRESS_7BIT)
    {
        /* I2Cģ��ʹ��10λ��ַ */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_BADR10_POS);
    }
    else
    {
        /* I2Cģ��ʹ��7λ��ַ */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_BADR10_POS);
    }
}

/**
  * ����  ����I2C_����ʹ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C����ʹ��λ������Ϣ��ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Generate_START(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���SENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C���� */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SEN_POS);
    }
    else
    {
        /* ��ֹI2C���� */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SEN_POS);
    }
}

/**
  * ����  ����I2Cֹͣ����ʹ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2Cֹͣ����ʹ��λ������Ϣ��ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Generate_STOP(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���PENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2Cֹͣ���� */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_PEN_POS);
    }
    else
    {
        /* ��ֹI2Cֹͣ���� */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_PEN_POS);
    }
}

/**
  * ����  ����I2C_AckӦ��ʹ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2CӦ��ʹ��λ������Ϣ��ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Ack_Config (I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���ACKENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C_Ack */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ACKEN_POS);
    }
    else
    {
        /* ��ֹI2C_Ack */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ACKEN_POS);
    }
}

/**
  * ����  ����I2C_AckӦ������λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2CӦ������λ������Ϣ��ȡֵΪ
  *                   I2C_ACKDATA_ACK:  Ӧ��
  *                   I2C_ACKDATA_NO_ACK: ��Ӧ��
  * ����  �ޡ�
  */
void
I2C_Ack_DATA_Config (I2C_SFRmap* I2Cx, uint32_t NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_ACKDATA(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���ACKDTλ ----------------*/
    if (NewState != I2C_ACKDATA_ACK)
    {
        /* ��Ӧ��*/
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ACKDT_POS);
    }
    else
    {
        /* Ӧ�� */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ACKDT_POS);
    }
}

/**
  * ����  ����I2C_Callʹ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C_Callʹ��λ������Ϣ��ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Call_Cmd (I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���GCENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C_Call */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_GCEM_POS);
    }
    else
    {
        /* ��ֹI2C_Call */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_GCEM_POS);
    }
}

/**
  * ����  ����I2C����ʱ��ѡ��Ĵ�����
  * ����  ClkSource: ʱ��ѡ��ȡֵ��ΧΪ��
  *                    I2C_CLK_SCLK: ѡ��SCLKΪI2C����ʱ��
  *                    I2C_CLK_HFCLK: ѡ��HFCLKΪI2C����ʱ��
  *                    I2C_CLK_LFCLK: ѡ��LFCLKΪI2C����ʱ��
  * ����  �ޡ�
  */
void
I2C_Clock_Config (I2C_SFRmap* I2Cx,uint32_t ClkSource)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_CLK(ClkSource));

    /*---------------- ����USART_CTLR�Ĵ���I2CCKSλ ----------------*/
    I2Cx->CTLR = SFR_Config (I2Cx->CTLR, ~I2C_CTLR_I2CCKS, ClkSource);
}

/**
  * ����  ����I2C��ַ�Ĵ���ƥ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C_��ַ�Ĵ���ƥ��λ��ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_MATCH_ADDRESS_Config (I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USART_CTLR�Ĵ���MTHALLλ ----------------*/
    if (NewState != FALSE)
    {
        /* ����������ܵĵ�ַ�ϲ����жϣ���ʹ������������ϵ�����ͨ���� */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_MTHALL_POS);
    }
    else
    {
        /* ֻ����I2CADDR��ַƥ��ʱ�Ż�������� */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_MTHALL_POS);
    }
}

/**
  * ����  ����I2C SCL���ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C_SCL���ʹ�ܣ�ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_SCL_Enable (I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���ENASCLλ ----------------*/
    if (NewState != FALSE)
    {
        /* ����ʹ��SCL�� */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ENASCL_POS);
    }
    else
    {
        /* SCL��ǿ��Ϊ�� */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ENASCL_POS);
    }
}

/**
  * ����  ����I2C ���ģʽʹ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C_���ģʽʹ��λ��ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_NMENA_Enable(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���NMENAλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܼ��ģʽ */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_NMENA_POS);
    }
    else
    {
        /* ��ʹ�ܼ��ģʽ */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_NMENA_POS);
    }
}

/**
  * ����  ����SMBusģʽʹ���źš�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: SMBusģʽʹ���źţ�ȡֵΪ:
  *                   I2C_MODE_SMBUS: SMBusģʽ
  *                   I2C_MODE_I2C: I2Cģʽ
  * ����  �ޡ�
  */
void
I2C_SMBUS_Enable(I2C_SFRmap* I2Cx, uint32_t NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_MODE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���SMBUSλ ----------------*/
    if (NewState != I2C_MODE_I2C)
    {
        /* 1 = SMBusģʽ */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SMBUS_POS);
    }
    else
    {
        /* 0 = I2Cģʽ */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SMBUS_POS);
    }
}

/**
  * ����  ����SMBUS���͡�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: SMBUS���ͣ�ȡֵΪ:
  *                   I2C_MODE_SMBUSHOST: SMBus����
  *                   I2C_MODE_SMBUSDEVICE: SMBus�豸
  * ����  �ޡ�
  */
void
I2C_SMBT_Config(I2C_SFRmap* I2Cx, uint32_t NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_SMBUS_MODE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���SMBTλ ----------------*/
    if (NewState != I2C_MODE_SMBUSDEVICE)
    {
        /* SMBus���� */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SMBT_POS);
    }
    else
    {
        /* SMBus�豸*/
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SMBT_POS);
    }
}

/**
  * ����  ����SMBus���ѡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: SMBus���ѣ�ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_SMBus_ALERT_Config(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���ALERTλ ----------------*/
    if (NewState != FALSE)
    {
        /* ����SMBALT����ʹ���� */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ALERT_POS);
    }
    else
    {
        /* �ͷ�SMBALT����ʹ����*/
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ALERT_POS);
    }
}

/**
  * ����  I2C�������ݡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR����
  *       Data: д�����ݼĴ�����ֵ��ȡֵΪ10λ���ݡ�
  * ����  �ޡ�
  */
void
I2C_SendData (I2C_SFRmap* I2Cx, uint32_t Data)
{
     /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_BUFR(Data));

    /*---------------- ����I2C_BUFR�Ĵ��� ----------------*/
    I2Cx->BUFR = Data;
}

/**
  * ����  I2C����8λ���ݡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR����
  *       Data: д�����ݼĴ�����ֵ��ȡֵΪ8λ���ݡ�
  * ����  �ޡ�
  */
void
I2C_SendData8 (I2C_SFRmap* I2Cx, uint8_t Data)
{
     /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_BUFR(Data));

    /*---------------- ����I2C_BUFR�Ĵ��� ----------------*/
    I2Cx->BUFR = Data;
}

/**
  * ����  I2C�������ݡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR����
  * ����  32λ�������ݵ�10λ���ݡ�
  */
uint32_t I2C_ReceiveData(I2C_SFRmap* I2Cx)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_BUFR�Ĵ��� ----------------*/
    tmpreg = I2Cx->BUFR;
    tmpreg &= I2C_BUFR_I2CBUF;
    tmpreg >>= I2C_BUFR_I2CBUF0_POS;

    return tmpreg;
}

/**
  * ����  ����I2C_ARPʹ��λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C_ARPʹ��λ������Ϣ��ȡֵΪ TRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_ARP_Enable(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_CTLR�Ĵ���ARPENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C_ARP */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ARPEN_POS);
    }
    else
    {
        /* ��ֹI2C_ARP */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ARPEN_POS);
    }
}

/**
  * ����  ����I2C��ַλ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       AddrSelect: ��ַ�Ĵ���ѡ��ȡֵΪ0x0~0x3��
  *       Data: ��ַλѡ��ȡֵΪ0x0~0x3FF��
  * ����  �ޡ�
  */
void
I2C_ADDR_Config(I2C_SFRmap* I2Cx, uint32_t AddrSelect, uint32_t Data)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_ADDR_REGISTER(AddrSelect));
    CHECK_RESTRICTION(CHECK_I2C_ADDR(Data));

    /*---------------- ����I2C_ADDR�Ĵ���I2CADDλ ----------------*/
    tmpreg = Data << I2C_ADDR_I2CADD0_POS;
    if (0 == AddrSelect)
    {
        tmpaddr = (uint32_t)&(I2Cx->ADDR0);
    }
    else
    {
        tmpaddr = (uint32_t)&(I2Cx->ADDR0) + 4 + (4 * AddrSelect);
    }
    *(volatile uint32_t *)tmpaddr =
                SFR_Config (*(volatile uint32_t *)tmpaddr,
                            ~I2C_ADDR_I2CADD,
                            tmpreg);
//    I2Cx->ADDR0 = SFR_Config (I2Cx->ADDR0, ~I2C_ADDR_I2CADD, tmpreg);
}

/**
  * ����  ����I2C��ַ����λ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       AddrSelect: ��ַ�Ĵ���ѡ��ȡֵΪ0x0~0x3��
  *       DataMask: ��ַλѡ��ȡֵΪ0x0~0x3FF��
  * ����  �ޡ�
  */
void
I2C_MSK_Config(I2C_SFRmap* I2Cx, uint32_t AddrSelect, uint32_t DataMask)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_ADDR_REGISTER(AddrSelect));
    CHECK_RESTRICTION(CHECK_I2C_MSK(DataMask));

    /*---------------- ����I2C_ADDR�Ĵ���λI2CMSK ----------------*/
    tmpreg = DataMask << I2C_ADDR_I2CMSK0_POS;
    if (0 == AddrSelect)
    {
        tmpaddr = (uint32_t)&(I2Cx->ADDR0);
    }
    else
    {
        tmpaddr = (uint32_t)&(I2Cx->ADDR0) + 4 + (4 * AddrSelect);
    }
    *(volatile uint32_t *)tmpaddr =
                SFR_Config (*(volatile uint32_t *)tmpaddr,
                            ~I2C_ADDR_I2CMSK,
                            tmpreg);
}

/**
  * ����  ����I2C SCL�ߵ�ƽռ�õ�ʱ����������
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       Period: SCL�ߵ�ƽռ�õ�ʱ����������ȡֵΪ0x0~0xFFFF��
  * ����  �ޡ�
  */
void
I2C_BRGH_Config (I2C_SFRmap* I2Cx,uint16_t Period)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_ADDR�Ĵ���λI2CMSK ----------------*/
    tmpreg = (uint32_t)Period << I2C_BRGR_I2CBRGH0_POS;
    I2Cx->BRGR = SFR_Config (I2Cx->BRGR, ~I2C_BRGR_I2CBRGH, tmpreg);
}

/**
  * ����  ����I2C SCL�͵�ƽռ�õ�ʱ����������
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       Period: SCL�͵�ƽռ�õ�ʱ����������ȡֵΪ0x0~0xFFFF��
  * ����  �ޡ�
  */
void
I2C_BRGL_Config (I2C_SFRmap* I2Cx,uint16_t Period)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_ADDR�Ĵ���λI2CMSK ----------------*/
    tmpreg = (uint32_t)Period << I2C_BRGR_I2CBRGL0_POS;
    I2Cx->BRGR = SFR_Config (I2Cx->BRGR, ~I2C_BRGR_I2CBRGL, tmpreg);
}
/**
  *   ##### �ڲ����ɵ�·�ӿ�(I2C)�������ú���������� #####
  */


/**
  *   ##### �ڲ����ɵ�·�ӿ�(I2C)�жϹ����� #####
  */
/**
  * ����  ����I2C��ʼ�ź��ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C��ʼ�ź��ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Start_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���SIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C��ʼ�ź��ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_SIE_POS);
    }
    else
    {
        /* ��ֹI2C��ʼ�ź��ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_SIE_POS);
    }
}

/**
  * ����  ����I2Cֹͣ�ź��ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2Cֹͣ�ź��ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Stop_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���PIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2Cֹͣ�ź��ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_PIE_POS);
    }
    else
    {
        /* ��ֹI2Cֹͣ�ź��ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_PIE_POS);
    }
}

/**
  * ����  ����I2CӦ������ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2CӦ������ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Ack_Fail_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���AFIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2CӦ������ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_AFIE_POS);
    }
    else
    {
        /* ��ֹI2CӦ������ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_AFIE_POS);
    }
}

/**
  * ����  ����I2Cʧȥ�ٲ��ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2Cʧȥ�ٲ��ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Arbitration_Lost_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���ARBLIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2Cʧȥ�ٲ��ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_ARBLIE_POS);
    }
    else
    {
        /* ��ֹI2Cʧȥ�ٲ��ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_ARBLIE_POS);
    }
}

/**
  * ����  ����SMBus�����ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: SMBus�����ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_SMBus_Alert_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���SMBAIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��SMBus�����ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_SMBAIE_POS);
    }
    else
    {
        /* ��ֹSMBus�����ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_SMBAIE_POS);
    }
}

/**
  * ����  ����SMBus����ͷϵ���ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: SMBus����ͷϵ���ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_SMBus_HostHead_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���SMBHIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��SMBus����ͷϵ���ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_SMBHIE_POS);
    }
    else
    {
        /* ��ֹSMBus����ͷϵ���ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_SMBHIE_POS);
    }
}

/**
  * ����  ����SMBus�豸Ĭ�ϵ�ַ�ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: SMBus�豸Ĭ�ϵ�ַ�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_SMBus_Device_Defaultaddress_INT_Enable (I2C_SFRmap* I2Cx,
                    FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���SMBDIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��SMBus�豸Ĭ�ϵ�ַ�ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_SMBDIE_POS);
    }
    else
    {
        /* ��ֹSMBus�豸Ĭ�ϵ�ַ�ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_SMBDIE_POS);
    }
}

/**
  * ����  ����I2C�ж��ź�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C�ж��ź�״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_ISIE_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���ISIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C�ж��ź� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_ISIE_POS);
    }
    else
    {
        /* ��ֹI2C�ж��ź� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_ISIE_POS);
    }
}

/**
  * ����  ����I2C����DMA�ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C����DMA�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Receive_DMA_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���IRCDEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C����DMA�ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_IRCDE_POS);
    }
    else
    {
        /* ��ֹI2C����DMA�ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_IRCDE_POS);
    }
}

/**
  * ����  ����I2C����DMA�ж�ʹ�ܡ�
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  *       NewState: I2C����DMA�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
I2C_Transmit_DMA_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����I2C_IER�Ĵ���ITXDEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��I2C����DMA�ж� */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_ITXDE_POS);
    }
    else
    {
        /* ��ֹI2C����DMA�ж� */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_ITXDE_POS);
    }
}

/**
  * ����  ��ȡI2C��ʼ�źű�־λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: �����ϳ�������ʼλ��
  *       0: ������δ��������ʼλ��
  */
FlagStatus
I2C_Get_Start_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���SIFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_SIF)
    {
        /* �����ϳ�������ʼλ */
        return SET;
    }
    else
    {
        /* ������δ��������ʼλ */
        return RESET;
    }
}

/**
  * ����  ����I2C��ʼ�źű�־��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Clear_Start_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_SR�Ĵ���SIFλ ----------------*/
    while(((I2Cx->SR) & I2C_SR_SIF)>>I2C_SR_SIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_SIF_POS);
    }
}

/**
  * ����  ��ȡI2Cֹͣ�źű�־λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: �����ϳ�����ֹͣλ��
  *       0: ������δ������ֹͣλ��
  */
FlagStatus
I2C_Get_Stop_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���PIFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_PIF)
    {
        /* �����ϳ�����ֹͣλ */
        return SET;
    }
    else
    {
        /* ������δ������ֹͣλ */
        return RESET;
    }
}

/**
  * ����  ����I2Cֹͣ�źű�־��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Clear_Stop_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_SR�Ĵ���PIFλ ----------------*/
    while(((I2Cx->SR) & I2C_SR_PIF)>>I2C_SR_PIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_PIF_POS);
    }

}

/**
  * ����  ��ȡI2C��ַƥ��״̬λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: �ӻ��յ�ƥ���ַ��Ӧ�𣻣������ϳ���ֹͣλ��Ӳ�������״̬λ��
  *       0: �ӻ�δ�յ���Ӧ��ַ��
  */
FlagStatus
I2C_Get_Address_Match_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���ADDRλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_ADDR)
    {
        /* �ӻ��յ�ƥ���ַ��Ӧ�� */
        return SET;
    }
    else
    {
        /* �ӻ�δ�յ���Ӧ��ַ */
        return RESET;
    }
}

/**
  * ����  ��ȡI2C��λ��ַ״̬λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: �ϴν��ջ��͵��ֽ��Ǹ�λ��ַ��
  *       0: �ϴν��ջ��͵��ֽڲ��Ǹ�λ��ַ��
  */
FlagStatus
I2C_Get_HighAddress_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���ADD10λ ----------------*/
    if ((I2Cx->SR) & I2C_SR_ADD10)
    {
        /* �ϴν��ջ��͵��ֽ��Ǹ�λ��ַ */
        return SET;
    }
    else
    {
        /* �ϴν��ջ��͵��ֽڲ��Ǹ�λ��ַ*/
        return RESET;
    }
}

/**
  * ����  ��ȡI2C��������״̬λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ��ʾ�ϴν��ջ��͵��ֽ������ݣ������ϳ�����ʼλ�������״̬λ��
  *       0: ��ʾ�ϴν��ջ��͵��ֽ��ǵ�ַ��
  */
FlagStatus
I2C_Get_Data_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���DATAλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_DATA)
    {
        /* ��ʾ�ϴν��ջ��͵��ֽ������� */
        return SET;
    }
    else
    {
        /* ��ʾ�ϴν��ջ��͵��ֽ��ǵ�ַ*/
        return RESET;
    }
}

/**
  * ����  ��ȡI2CӦ������־λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ������Ӧ�����
  *       0: δ����Ӧ�����
  */
FlagStatus
I2C_Get_Ack_Fail_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���AFIFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_AFIF)
    {
        /* ��ʾ�ϴν��ջ��͵��ֽ������� */
        return SET;
    }
    else
    {
        /* ��ʾ�ϴν��ջ��͵��ֽ��ǵ�ַ*/
        return RESET;
    }
}

/**
  * ����  ����I2CӦ������־��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Clear_Ack_Fail_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_SR�Ĵ���AFIFλ ----------------*/
    while(((I2Cx->SR) & I2C_SR_AFIF)>>I2C_SR_AFIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_AFIF_POS);
    }
}

/**
  * ����  ��ȡI2Cʧȥ�ٲñ�־λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: �������ݹ�����ʧȥ�ٲã�
  *       0: �������ݹ�����δʧȥ�ٲá�
  */
FlagStatus
I2C_Get_Arbitration_Lost_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���ARBLIFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_ARBLIF)
    {
        /* �������ݹ�����ʧȥ�ٲ� */
        return SET;
    }
    else
    {
        /* �������ݹ�����δʧȥ�ٲ�*/
        return RESET;
    }
}

/**
  * ����  ����I2Cʧȥ�ٲñ�־��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Clear_Arbitration_Lost_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_SR�Ĵ���ARBLIFλ ----------------*/
    while(((I2Cx->SR) & I2C_SR_ARBLIF)>>I2C_SR_ARBLIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_ARBLIF_POS);
    }
}

/**
  * ����  ��ȡI2C��/ д��Ϣ״̬λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ����Ϣ״̬λ��
  *       0: д��Ϣ״̬λ��
  */
FlagStatus
I2C_Get_Write_Read_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���RNWλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_RNW)
    {
        /* ����Ϣ״̬λ*/
        return SET;
    }
    else
    {
        /* д��Ϣ״̬λ*/
        return RESET;
    }
}

/**
  * ����  ��ȡSMBus�����жϱ�־״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ����Ϣ״̬λ��
  *       0: д��Ϣ״̬λ��
  */
FlagStatus
I2C_Get_SMBus_Alert_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���SMBAIFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_SMBAIF)
    {
        /* ����SMBus�����¼�*/
        return SET;
    }
    else
    {
        /* ��SMBus����*/
        return RESET;
    }
}

/**
  * ����  ����SMBus�����жϱ�־��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Clear_SMBus_Alert_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_SR�Ĵ���SMBAIFλ ----------------*/
    while(((I2Cx->SR) & I2C_SR_SMBAIF)>>I2C_SR_SMBAIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_SMBAIF_POS);
    }
}

/**
  * ����  ��ȡSMBus����ͷϵ���жϱ�־״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ����Ϣ״̬λ��
  *       0: д��Ϣ״̬λ��
  */
FlagStatus
I2C_Get_SMBus_Host_Header_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���SMBHIFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_SMBHIF)
    {
        /* ����SMBus�����¼�*/
        return SET;
    }
    else
    {
        /* ��SMBus����*/
        return RESET;
    }
}

/**
  * ����  ����SMBus����ͷϵ���жϱ�־��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Clear_SMBus_Host_Header_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_SR�Ĵ���SMBHIFλ ----------------*/
    while(((I2Cx->SR) & I2C_SR_SMBHIF)>>I2C_SR_SMBHIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_SMBHIF_POS);
    }
}

/**
  * ����  ��ȡSMBus�豸Ĭ�ϵ�ַ(��ģʽ) ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ����Ϣ״̬λ��
  *       0: д��Ϣ״̬λ��
  */
FlagStatus
I2C_Get_SMBus_Device_Default_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���SMBDIFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_SMBDIF)
    {
        /* ��ARPEN=1ʱ���յ�SMBus�豸��Ĭ�ϵ�ַ*/
        return SET;
    }
    else
    {
        /* δ�յ�SMBus�豸��Ĭ�ϵ�ַ*/
        return RESET;
    }
}

/**
  * ����  ����SMBus�豸Ĭ�ϵ�ַ(��ģʽ)��־��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Clear_SMBus_Device_Default_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_SR�Ĵ���SMBDIFλ ----------------*/
    while(((I2Cx->SR) & I2C_SR_SMBDIF)>>I2C_SR_SMBDIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_SMBDIF_POS);
    }
}

/**
  * ����  ��ȡI2C�ж��źű�־λ״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ������I2C�ж��źű�־λ��
  *       0: δ����I2C�ж��źű�־λ��
  */
FlagStatus
I2C_Get_INTERRUPT_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���ISIFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_ISIF)
    {
        /* ������I2C�ж��źű�־λ*/
        return SET;
    }
    else
    {
        /* δ����I2C�ж��źű�־λ*/
        return RESET;
    }
}

/**
  * ����  ����I2C�ж��źű�־��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  �ޡ�
  */
void
I2C_Clear_INTERRUPT_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ����I2C_SR�Ĵ���ISIFλ ----------------*/
    while(((I2Cx->SR) & I2C_SR_ISIF)>>I2C_SR_ISIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_ISIF_POS);

    }
}

/**
  * ����  ��ȡI2C����BUFFΪ��״̬ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ����BUFFΪ����(�� I2Cx_BUFR �����λ)
  *       0: ����BUFFΪ�ա�
  */
FlagStatus
I2C_Get_Receive_Buff_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���RCBFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_RCBF)
    {
        /* ����BUFFΪ��*/
        return SET;
    }
    else
    {
        /* ����BUFFΪ��*/
        return RESET;
    }
}

/**
  * ����  ��ȡI2C����BUFF״̬λ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: �ȴ�дI2Cx_BUFR��(д I2Cx_BUFR �����λ)
  *       0: ����ҪдI2Cx_BUFR��
  */
FlagStatus
I2C_Get_Transmit_Buff_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���TXBEλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_TXBE)
    {
        /* �ȴ�дI2Cx_BUFR*/
        return SET;
    }
    else
    {
        /* ����ҪдI2Cx_BUFR*/
        return RESET;
    }
}

/**
  * ����  ��ȡI2C����DMA�жϱ�־λ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ������I2C����DMA�жϣ�
  *       0: δ����I2C����DMA�жϡ�
  */
FlagStatus
I2C_Get_Receive_DMA_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���IRCDFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_IRCDF)
    {
        /* ������I2C����DMA�ж�*/
        return SET;
    }
    else
    {
        /* δ����I2C����DMA�ж�*/
        return RESET;
    }
}

/**
  * ����  ��ȡI2C����DMA�жϱ�־λ ��
  * ����  I2Cx: ָ��I2C�ڴ�ṹ��ָ�룬ȡֵΪI2C0_SFR~I2C3_SFR��
  * ����  1: ������I2C����DMA�жϣ�
  *       0: δ����I2C����DMA�жϡ�
  */
FlagStatus
I2C_Get_Transmit_DMA_Flag (I2C_SFRmap* I2Cx)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- ��ȡI2C_SR�Ĵ���ITXDFλ ----------------*/
    if ((I2Cx->SR) & I2C_SR_ITXDF)
    {
        /* ������I2C����DMA�ж�*/
        return SET;
    }
    else
    {
        /* δ����I2C����DMA�ж�*/
        return RESET;
    }
}
/**
  *   ##### �ڲ����ɵ�·�ӿ�(I2C)�жϹ������������ #####
  */

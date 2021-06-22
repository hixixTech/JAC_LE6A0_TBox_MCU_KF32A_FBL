/**
  ******************************************************************************
  * �ļ���  kf32a_basic_exic.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��ģ��ת��ģ��(ADC)���蹦�ܺ�����������
  * ��  ��  ���ļ��ṩ��LDC8080ģ��(EXIC)��صĹ��ܺ�����������
  *          + EXIC��ʼ������
  *          + EXIC�շ�����
  *          + EXIC�жϹ�����
  *
  *********************************************************************
  */
#include "kf32a_basic_exic.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_exic
/**
  *   ##### EXIC�ӿڹ��ܳ�ʼ������ #####
  */
/* EXIC_CTL0�Ĵ�����ʼ���������� */
#define EXIC_CTL0_INIT_MASK		 		(EXIC_CTL0_EXICEN \
                                       | EXIC_CTL0_CSEN \
                                       | EXIC_CTL0_DCSEL \
                                       | EXIC_CTL0_WRITEEN \
                                       | EXIC_CTL0_READEN \
                                       | EXIC_CTL0_TDMAEN \
                                       | EXIC_CTL0_RDMAEN \
                                       | EXIC_CTL0_WENDIN \
                                       | EXIC_CTL0_RENDIE \
                                       | EXIC_CTL0_WRITE1EN \
                                       | EXIC_CTL0_READ1EN \
                                       | EXIC_CTL0_DATATR \
                                       | EXIC_CTL0_MODESEL)
/* EXIC_CTL1�Ĵ�����ʼ���������� */
#define EXIC_CTL1_INIT_MASK		 		(EXIC_CTL1_EXICCS \
                                       | EXIC_CTL1_PULSEWIDTH)
/**
  * ����  ��λEXICģ�顣
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
EXIC_Reset (void)
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_EXICRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_EXICRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_EXICCLKEN, TRUE);
}
/**
  * ����  EXIC�������á�
  * ����  EXICx: EXICInitStruct: EXIC������Ϣ
  * ����  �ޡ�
  */
void
EXIC_Configuration (EXIC_InitTypeDef* EXICInitStruct)
{
    uint32_t tmpreg = 0;

	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_Module_Enable));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_CS_Enable));
	CHECK_RESTRICTION(CHECK_EXIC_TRANSMIT(EXICInitStruct->m_Data_Control_Sel));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_Write_Enable));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_Read_Enable));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_DMA_Write_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_DMA_Read_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_WriteFnish_INT_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_ReadFnish_INT_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_WriteData_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_ReadData_EN));
	CHECK_RESTRICTION(CHECK_DATA_DIRECTION(EXICInitStruct->m_Data_Direction_Sel));
	CHECK_RESTRICTION(CHECK_MODE_SEL(EXICInitStruct->m_Mode_Sel));
	CHECK_RESTRICTION(CHECK_EXIC_CLKSOURCE_SEL(EXICInitStruct->m_Source_Clock_Sel));
	CHECK_RESTRICTION(CHECK_PULSEWIDTH_SEL(EXICInitStruct->m_Pulse_Width_Sel));

	/*---------------------------- EXIC_CTL�Ĵ�������  -----------------*/
	/* ���ݽṹ���Աm_Module_Enable������EXICENλ�� */
	/* ���ݽṹ���Աm_CS_Enable������CSENλ�� */
	/* ���ݽṹ���Աm_Data_Control_Sel������D/CSELλ�� */
	/* ���ݽṹ���Աm_Write_Enable������WRITEENλ�� */
	/* ���ݽṹ���Աm_Read_Enable������READENλ�� */
	/* ���ݽṹ���Աm_DMA_Write_EN������TDMAENλ�� */
	/* ���ݽṹ���Աm_DMA_Read_EN������RDMAENλ�� */
	/* ���ݽṹ���Աm_WriteFnish_INT_EN������WENDIEλ�� */
	/* ���ݽṹ���Աm_ReadFnish_INT_EN������RENDIEλ�� */
	/* ���ݽṹ���Աm_WriteData_EN������WRITE1ENλ�� */
	/* ���ݽṹ���Աm_ReadData_EN������READ1ENλ�� */
	/* ���ݽṹ���Աm_Data_Direction_Sel������DATATRλ�� */
	/* ���ݽṹ���Աm_Mode_Sel������MODESELλ�� */


   tmpreg = ((EXICInitStruct->m_Module_Enable << EXIC_CTL0_EXICEN_POS) | \
		     (EXICInitStruct->m_CS_Enable << EXIC_CTL0_CSEN_POS) | \
		     (EXICInitStruct->m_Data_Control_Sel ) | \
		     (EXICInitStruct->m_Write_Enable << EXIC_CTL0_WRITEEN_POS) | \
		     (EXICInitStruct->m_Read_Enable << EXIC_CTL0_READEN_POS) | \
		     (EXICInitStruct->m_DMA_Write_EN << EXIC_CTL0_TDMAEN_POS) | \
		     (EXICInitStruct->m_DMA_Read_EN << EXIC_CTL0_RDMAEN_POS) | \
		     (EXICInitStruct->m_WriteFnish_INT_EN << EXIC_CTL0_WENDIE_POS) | \
		     (EXICInitStruct->m_ReadFnish_INT_EN << EXIC_CTL0_RENDIE_POS) | \
		     (EXICInitStruct->m_WriteData_EN << EXIC_CTL0_WRITE1EN_POS) | \
		     (EXICInitStruct->m_ReadData_EN << EXIC_CTL0_READ1EN_POS) | \
		     (EXICInitStruct->m_Data_Direction_Sel ) | \
		     (EXICInitStruct->m_Mode_Sel) | \
		     (EXICInitStruct->m_Source_Clock_Sel) | \
		     (EXICInitStruct->m_Pulse_Width_Sel));
   EXIC_SFR->CTL0 = SFR_Config (EXIC_SFR->CTL0,
			   ~EXIC_CTL0_INIT_MASK,
			   tmpreg);
	/*---------------------------- EXIC_CTL1�Ĵ�������  -----------------*/
	/* ���ݽṹ���Աm_Source_Clock_Sel������EXICCSλ�� */
	/* ���ݽṹ���Աm_Pulse_Width_Sel������PULSEWIDTHλ�� */
   tmpreg = ((EXICInitStruct->m_Source_Clock_Sel) | \
		     (EXICInitStruct->m_Pulse_Width_Sel));
   EXIC_SFR->CTL1 = SFR_Config (EXIC_SFR->CTL1,
			   ~EXIC_CTL1_INIT_MASK,
			   tmpreg);
}
/**
  * ����  ��ʼ��EXIC������Ϣ�ṹ�塣
  * ����  EXICInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
EXIC_Struct_Init (EXIC_InitTypeDef* EXICInitStruct)
{
    /* ��ʼ��EXIC�ӿ�ʹ��ѡ�� */
    EXICInitStruct->m_Module_Enable = FALSE;
    /* ��ʼ��EXICƬѡʹ��ѡ�� */
    EXICInitStruct->m_CS_Enable = FALSE;
    /* ��ʼ�� EXIC����/����ѡ�� */
    EXICInitStruct->m_Data_Control_Sel = EXIC_TRANSMIT_DATA;
    /* ��ʼ��EXICд����ʹ�� */
    EXICInitStruct->m_Write_Enable = FALSE;
    /* ��ʼ�� EXIC������ʹ�� */
    EXICInitStruct->m_Read_Enable = FALSE;
    /* ��ʼ��EXIC DMAдʹ��*/
    EXICInitStruct->m_DMA_Write_EN = FALSE;
    /* ��ʼ��EXIC DMA��ʹ�� */
    EXICInitStruct->m_DMA_Read_EN = FALSE;
    /* ��ʼ�� EXIC�����ȼ�ͨ���ⲿ����ת��ģʽʹ�� */
    EXICInitStruct->m_WriteFnish_INT_EN = FALSE;
    /* ��ʼ�� ������ж�ʹ�� */
    EXICInitStruct->m_ReadFnish_INT_EN = FALSE;
    /* ��ʼ�� EXICд����ʹ�� */
    EXICInitStruct->m_WriteData_EN = FALSE;
    /* ��ʼ�� EXIC������ʹ�� */
    EXICInitStruct->m_ReadData_EN = FALSE;
    /* ��ʼ��EXIC���ݷ���ѡ�� */
    EXICInitStruct->m_Data_Direction_Sel = EXIC_DATA_INPUT;
    /* ��ʼ��EXICģʽѡ�� */
	EXICInitStruct->m_Mode_Sel = EXIC_MODE_SEL_W1R1;
	/* ��ʼ��EXIC����ʱ��Դѡ�� */
	EXICInitStruct->m_Source_Clock_Sel = EXIC_CLKSOURCE_SCLK;
	/* ��ʼ�� EXIC��д����ѡ�� */
	EXICInitStruct->m_Pulse_Width_Sel = EXIC_PULSEWIDTH_16Tclk;
}
/**
  * ����  ��ȡEXICд��ɱ�־��
  * ����  :��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
EXIC_Get_Write_Finish_INT_Flag (void)
{
    /*-------------------- ��ȡEXICSTATE�Ĵ���WENFIFλ --------------------*/
	  if (EXIC_STATE & EXIC_STATE_WENDIF)
    {
        /*  EXICд��ɱ�־ */
        return SET;
    }
    else
    {
        /*  EXICδ������ɱ�־ */
        return RESET;
    }
}
/**
  * ����  ����EXICд��ɱ�־��
  * ����  :��
  * ����  ����
  */
void EXIC_Clear_Write_Finish_INT_Flag (void)
{
    /*-------------------- ����EXICSTATE�Ĵ���WENFIFλ --------------------*/
	while((EXIC_STATE & EXIC_STATE_WENDIF)>>EXIC_STATE_WENDIF_POS)
	{
		EXIC_SFR->STATE &= ~EXIC_STATE_WENDIF;
	}
}
/**
  * ����  ��ȡEXIC����ɱ�־��
  * ����  :��
  * ����  �ж�״̬��0��δ�����жϣ�1�������ж�
  */
FlagStatus
EXIC_Get_Read_Finish_INT_Flag (void)
{
    /*-------------------- ��ȡEXICSTATE�Ĵ���RENFIFλ --------------------*/
	if (EXIC_STATE & EXIC_STATE_RENDIF)
    {
        /*  EXIC����ɱ�־ */
        return SET;
    }
    else
    {
        /*  EXICδ��������ɱ�־ */
        return RESET;
    }
}
/**
  * ����  ����EXIC����ɱ�־��
  * ����  :��
  * ����  ����
  */
void EXIC_Clear_Read_Finish_INT_Flag (void)
{
    /*-------------------- ����EXICSTATE�Ĵ���RENFIFλ --------------------*/
	while((EXIC_STATE & EXIC_STATE_RENDIF)>>EXIC_STATE_RENDIF_POS)
	{
		EXIC_SFR->STATE &= ~EXIC_STATE_RENDIF;
	}
}

/**
  * ����  EXICд���ݻ��档
  * ����  :DATAλ����18,��С��ΧΪ(0x00-0x3ffff)
  * ����  :��
  */
void
EXIC_Write_Buff (uint32_t DATA)
{
   EXIC_SFR->WRITEBUF = DATA;
}
/**
  * ����  EXIC�����ݻ��档
  * ����  :��
  * ����  :DATAλ����18,��С��ΧΪ(0x00-0x3ffff)
  */
uint32_t
EXIC_Read_Buff (void)
{

   return(EXIC_SFR->READBUF);

}

#endif  // KF32A_Periph_exic

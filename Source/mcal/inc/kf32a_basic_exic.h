/**
  ******************************************************************************
  * �ļ���  kf32a_basic_exic.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ����    ���ļ�����EXICģ������Ŀ⺯����������غ궨�塣
  *
  *********************************************************************
  */


#ifndef _KF32A_BASIC_EXIC_H_
#define _KF32A_BASIC_EXIC_H_

#include "KF32A_BASIC.h"


/**
  * ����  EXIC�ӿ�ģ��������Ϣ�ṹ��
  */
typedef struct
{
	FunctionalState m_Module_Enable; /* EXIC�ӿ�ʹ��ѡ��
                                            ȡֵΪTRUE��FALSE�� */
	FunctionalState m_CS_Enable; /* EXICƬѡʹ��ѡ��
                                           ȡֵΪTRUE��FALSE��һ���� */
	uint32_t m_Data_Control_Sel; /* EXIC����/����ѡ��
                                           ȡֵΪ��EXIC����/����ѡ�� */
	FunctionalState m_Write_Enable; /* EXICд����ʹ�ܣ�
                                            ȡֵΪTRUE��FALSE�� */
	FunctionalState m_Read_Enable; /* EXIC������ʹ�ܣ�
                                           ȡֵΪTRUE��FALSE�� */
    FunctionalState m_DMA_Write_EN;  /* EXIC DMAдʹ�ܣ�
                                           ȡֵΪTRUE��FALSE��   */
    FunctionalState m_DMA_Read_EN; /* EXIC DMA��ʹ�ܣ�
                                            ȡֵΪTRUE��FALSE�� */
    FunctionalState m_WriteFnish_INT_EN;/* EXIC�����ȼ�ͨ���ⲿ����ת��ģʽʹ�ܣ�
                                           ȡֵΪTRUE��FALSE�� */
    FunctionalState m_ReadFnish_INT_EN; /* ������ж�ʹ�ܣ�
                                           ȡֵΪTRUE��FALSE�� */
    FunctionalState m_WriteData_EN;/* EXICд����ʹ�ܣ�
                                           ȡֵΪTRUE��FALSE�� */
    FunctionalState m_ReadData_EN;  /* EXIC������ʹ�ܣ�
                                           ȡֵΪTRUE��FALSE�� */
    uint32_t m_Data_Direction_Sel;/* EXIC���ݷ���ѡ��
                                            ȡֵΪ�ꡰEXIC ���ݷ���ѡ���е�һ���� */
    uint32_t m_Mode_Sel;/* EXICģʽѡ��
                                           ȡֵΪ�ꡰEXIC ģʽѡ��λ���е�һ���� */
    uint32_t m_Source_Clock_Sel;/* EXIC����ʱ��Դѡ��
                                            ȡֵΪ�ꡰEXIC ����ʱ��Դѡ���е�һ���� */
    uint32_t m_Pulse_Width_Sel; /* EXIC��д����ѡ��
                                            ȡֵΪ�ꡰEXIC ��д����ѡ���е�һ���� */

}EXIC_InitTypeDef;

/**
 * EXIC ����/����ѡ��
 */
#define EXIC_TRANSMIT_DATA           ( (uint32_t) 0x0 << EXIC_CTL0_DCSEL_POS )
#define EXIC_TRANSMIT_COMMAND        ( (uint32_t) 0x1 << EXIC_CTL0_DCSEL_POS )
#define CHECK_EXIC_TRANSMIT(SEL)       (((SEL) == EXIC_TRANSMIT_DATA) \
                                        || ((SEL) == EXIC_TRANSMIT_COMMAND) )
/**
 * EXIC ���ݷ���ѡ��
 */
#define EXIC_DATA_INPUT              ( (uint32_t) 0x1 << EXIC_CTL0_DATATR_POS )
#define EXIC_DATA_OUTPUT             ( (uint32_t) 0x0 << EXIC_CTL0_DATATR_POS )
#define CHECK_DATA_DIRECTION(SEL)      (((SEL) == EXIC_DATA_INPUT) \
                                       || ((SEL) == EXIC_DATA_OUTPUT) )

/**
 * EXIC ģʽѡ��
 */
#define EXIC_MODE_SEL_W1R1            ( (uint32_t) 0x1 << EXIC_CTL0_MODESEL_POS )
#define EXIC_MODE_SEL_WR              ( (uint32_t) 0x0 << EXIC_CTL0_MODESEL_POS )
#define CHECK_MODE_SEL(SEL)             (((SEL) == EXIC_MODE_SEL_W1R1) \
                                        || ((SEL) == EXIC_MODE_SEL_WR) )

/**
 * EXIC ����ʱ��Դѡ��
 */
#define EXIC_CLKSOURCE_SCLK           ( (uint32_t) 0x0 << EXIC_CTL1_EXICCS0_POS )
#define EXIC_CLKSOURCE_HFCLK          ( (uint32_t) 0x1 << EXIC_CTL1_EXICCS0_POS )
#define EXIC_CLKSOURCE_LFCLK          ( (uint32_t) 0x2 << EXIC_CTL1_EXICCS0_POS )
#define CHECK_EXIC_CLKSOURCE_SEL(SEL)   (((SEL) == EXIC_CLKSOURCE_SCLK) \
										|| ((SEL) == EXIC_CLKSOURCE_HFCLK) \
                                        || ((SEL) == EXIC_CLKSOURCE_LFCLK) )
/**
 * EXIC ��д����ѡ��
 */
#define EXIC_PULSEWIDTH_1Tclk           ( (uint32_t) 0x0 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_2Tclk           ( (uint32_t) 0x1 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_4Tclk           ( (uint32_t) 0x2 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_8Tclk           ( (uint32_t) 0x3 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_16Tclk          ( (uint32_t) 0x4 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_32Tclk          ( (uint32_t) 0x5 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_64Tclk          ( (uint32_t) 0x6 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_128Tclk         ( (uint32_t) 0x7 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_256Tclk         ( (uint32_t) 0x8 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_512Tclk         ( (uint32_t) 0x9 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_1024Tclk        ( (uint32_t) 0xA << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_2048Tclk        ( (uint32_t) 0xB << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_4096Tclk        ( (uint32_t) 0xC << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_8192Tclk        ( (uint32_t) 0xD << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_16384Tclk       ( (uint32_t) 0xE << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_32768Tclk       ( (uint32_t) 0xF << EXIC_CTL1_PULSEWIDTH0_POS )
#define CHECK_PULSEWIDTH_SEL(SEL)       (((SEL) == EXIC_PULSEWIDTH_1Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_2Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_4Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_8Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_16Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_32Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_64Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_128Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_256Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_512Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_1024Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_2048Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_4096Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_8192Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_16384Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_32768Tclk) )

#ifdef KF32A_Periph_exic
void EXIC_Struct_Init (EXIC_InitTypeDef* EXICInitStruct);
uint32_t EXIC_Read_Buff (void);
void EXIC_Write_Buff (uint32_t DATA);
void EXIC_Clear_Write_Finish_INT_Flag (void);
void EXIC_Clear_Read_Finish_INT_Flag (void);
FlagStatus EXIC_Get_Read_Finish_INT_Flag (void);
FlagStatus EXIC_Get_Write_Finish_INT_Flag (void);
void EXIC_Configuration (EXIC_InitTypeDef* EXICInitStruct);
void EXIC_Reset (void);

#endif //KF32A_Periph_exic
#endif /* KF32A_BASIC_EXIC_H_ */

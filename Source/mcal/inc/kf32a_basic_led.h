/**
  ********************************************************************
  * �ļ���  kf32a_basic_led.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��������ʾ(LED)��ع��ܺ�����������غ궨�塣
  *********************************************************************
*/

#ifndef _KF32A_BASIC_LED_H_
#define _KF32A_BASIC_LED_H_

#include "KF32A_BASIC.h"


/**
  * ����  LED�ӿ�ģ��������Ϣ�ṹ��
  */
typedef struct
{
	FunctionalState m_Module_Enable;  /* LED��ʾ����ʹ��ѡ��,
                                            ȡֵΪTRUE��FALSE�� */
	uint32_t m_Output_Control_Select; /* LED�����������ѡ��,
                                           ȡֵΪ�ꡰ����������ơ���һ���� */
	uint32_t m_Anodic_Cathodal_Sel;   /* LED��������ѡ��
                                           ȡֵΪ����������ѡ�񡰵�һ���� */
	uint32_t m_Source_Clock_Sel;     /* LED����ʱ��Դѡ��
                                            ȡֵΪ������ʱ��Դѡ�񡰵�һ���� */
	uint32_t m_Clock_Division; /* LEDʱ��Դ��Ƶѡ��
                                           ȡֵΪ��ʱ��Դ��Ƶѡ�󡰵�һ���� */
	uint32_t m_Clock_PreDivision;  /* LEDԤ��Ƶ��ѡ��
                                           ȡֵΪ��ʱ��ԴԤ��Ƶ��ѡ�񡰵�һ����   */

	FunctionalState m_Output_SEG12_15_Enable;  /* LEDͨ��SEG<15:12>���ʹ��,
                                            ȡֵΪTRUE��FALSE�� */
	FunctionalState m_Output_SEG8_11_Enable;  /* LEDͨ��SEG<11:8>���ʹ��,
                                            ȡֵΪTRUE��FALSE�� */
	FunctionalState m_Output_SEG0_7_Enable;  /* LEDͨ��SEG<7:0>���ʹ��,
                                            ȡֵΪTRUE��FALSE�� */
	uint32_t m_COM_Number_Select;  /* LED COM ʹ������ѡ��
                                           ȡֵΪ��ʹ������ѡ�񡰵�һ����   */
	uint32_t m_Lum_Select;  /* LED COM �Զȵ�������ѡ��
                                           ȡֵΪ���Զȵ������á���һ����   */


//	uint32_t m_COMX_SEGDATA_State;  /* LED ��ʾ��������λ��
//                                           ȡֵΪ����ʾ�������ݡ���һ����   */

}LED_InitTypeDef;


/**
 * LED �����������
 */
#define LEDSEG_OUTPUT_DISABLE             ( (uint32_t) 0x1 << LED_CTL0_LEDBR_POS )
#define LEDSEG_LEDCOM_OUTPUT_NORMAL       ( (uint32_t) 0x0 << LED_CTL0_LEDBR_POS )
#define CHECK_OUTPUT_CONTROL_SEL(SEL)     (((SEL) == LEDSEG_OUTPUT_DISABLE) \
                                        || ((SEL) == LEDSEG_LEDCOM_OUTPUT_NORMAL) )
/**
 * LED ��������ѡ��
 */
#define LED_CATHODAL_DIRECT_MODE           ( (uint32_t) 0x0 << LED_CTL0_PNS0_POS )
#define LED_CATHODAL_AUDION_MODE           ( (uint32_t) 0x2 << LED_CTL0_PNS0_POS )
#define LED_ANODIC_DIRECT_MODE     		   ( (uint32_t) 0x3 << LED_CTL0_PNS0_POS )
#define CHECK_WORK_MODE_SEL(SEL)           (((SEL) == LED_CATHODAL_DIRECT_MODE) \
										   || ((SEL) == LED_CATHODAL_AUDION_MODE)\
                                           || ((SEL) == LED_ANODIC_DIRECT_MODE) )
/**
 * LED ����ʱ��Դѡ��
 */
#define LED_CLKSOURCE_SCLK                 ( (uint32_t) 0x0 << LED_CTL0_CS0_POS )
#define LED_CLKSOURCE_HFCLK                ( (uint32_t) 0x1 << LED_CTL0_CS0_POS )
#define LED_CLKSOURCE_LFCLK     	       ( (uint32_t) 0x2 << LED_CTL0_CS0_POS )
#define CHECK_LED_CLKSOURCE_SEL(SEL)       (((SEL) == LED_CLKSOURCE_SCLK) \
										   || ((SEL) == LED_CLKSOURCE_HFCLK)\
                                           || ((SEL) == LED_CLKSOURCE_LFCLK) )
/**
 * LED ʱ��Դ��Ƶ��ѡ��
 */
#define LED_CLK_DIVISION_2                 ( (uint32_t) 0x0 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_4                 ( (uint32_t) 0x2 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_8                 ( (uint32_t) 0x3 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_16                ( (uint32_t) 0x4 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_32                ( (uint32_t) 0x5 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_64                ( (uint32_t) 0x6 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_128               ( (uint32_t) 0x7 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_256               ( (uint32_t) 0x8 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_512               ( (uint32_t) 0x9 << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_1024              ( (uint32_t) 0xA << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_2048              ( (uint32_t) 0xB << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_4096              ( (uint32_t) 0xC << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_8192              ( (uint32_t) 0xD << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_16384             ( (uint32_t) 0xE << LED_CTL0_CKS1_0_POS )
#define LED_CLK_DIVISION_32768             ( (uint32_t) 0xF << LED_CTL0_CKS1_0_POS )
#define CHECK_CLK_DIVISION_SEL(SEL)        (((SEL) == LED_CLK_DIVISION_2) \
										   || ((SEL) == LED_CLK_DIVISION_4)\
										   || ((SEL) == LED_CLK_DIVISION_8)\
										   || ((SEL) == LED_CLK_DIVISION_16)\
										   || ((SEL) == LED_CLK_DIVISION_32)\
										   || ((SEL) == LED_CLK_DIVISION_64)\
										   || ((SEL) == LED_CLK_DIVISION_128)\
										   || ((SEL) == LED_CLK_DIVISION_256)\
										   || ((SEL) == LED_CLK_DIVISION_512)\
										   || ((SEL) == LED_CLK_DIVISION_1024)\
										   || ((SEL) == LED_CLK_DIVISION_2048)\
										   || ((SEL) == LED_CLK_DIVISION_4096)\
										   || ((SEL) == LED_CLK_DIVISION_8192)\
										   || ((SEL) == LED_CLK_DIVISION_16384)\
										   || ((SEL) == LED_CLK_DIVISION_32768) )
/**
 * LED Ԥ��Ƶ��ѡ��
 */
#define LED_CLK_PREDIVISION_16             ( (uint32_t) 0xF << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_15             ( (uint32_t) 0xE << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_14             ( (uint32_t) 0xD << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_13             ( (uint32_t) 0xC << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_12             ( (uint32_t) 0xB << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_11             ( (uint32_t) 0xA << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_10             ( (uint32_t) 0x9 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_9              ( (uint32_t) 0x8 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_8              ( (uint32_t) 0x7 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_7              ( (uint32_t) 0x6 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_6              ( (uint32_t) 0x5 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_5              ( (uint32_t) 0x4 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_4              ( (uint32_t) 0x3 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_3              ( (uint32_t) 0x2 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_2              ( (uint32_t) 0x1 << LED_CTL0_CKS0_0_POS )
#define LED_CLK_PREDIVISION_1              ( (uint32_t) 0x0 << LED_CTL0_CKS0_0_POS )
#define CHECK_CLK_PREDIVISION_SEL(SEL)     (((SEL) == LED_CLK_PREDIVISION_16) \
										   || ((SEL) == LED_CLK_PREDIVISION_15)\
										   || ((SEL) == LED_CLK_PREDIVISION_14)\
										   || ((SEL) == LED_CLK_PREDIVISION_13)\
										   || ((SEL) == LED_CLK_PREDIVISION_12)\
										   || ((SEL) == LED_CLK_PREDIVISION_11)\
										   || ((SEL) == LED_CLK_PREDIVISION_10)\
										   || ((SEL) == LED_CLK_PREDIVISION_9)\
										   || ((SEL) == LED_CLK_PREDIVISION_8)\
										   || ((SEL) == LED_CLK_PREDIVISION_7)\
										   || ((SEL) == LED_CLK_PREDIVISION_6)\
										   || ((SEL) == LED_CLK_PREDIVISION_5)\
										   || ((SEL) == LED_CLK_PREDIVISION_4)\
										   || ((SEL) == LED_CLK_PREDIVISION_3)\
										   || ((SEL) == LED_CLK_PREDIVISION_2)\
										   || ((SEL) == LED_CLK_PREDIVISION_1) )
/**
 * LED λʹ������ѡ��
 */
#define LED_COM_NUM_1                      ( (uint32_t) 0x0 << LED_CTL1_CCOMEN0_POS )
#define LED_COM_NUM_2                      ( (uint32_t) 0x1 << LED_CTL1_CCOMEN0_POS )
#define LED_COM_NUM_3                      ( (uint32_t) 0x2 << LED_CTL1_CCOMEN0_POS )
#define LED_COM_NUM_4                      ( (uint32_t) 0x3 << LED_CTL1_CCOMEN0_POS )
#define LED_COM_NUM_5                      ( (uint32_t) 0x4 << LED_CTL1_CCOMEN0_POS )
#define LED_COM_NUM_6                      ( (uint32_t) 0x5 << LED_CTL1_CCOMEN0_POS )
#define LED_COM_NUM_7                      ( (uint32_t) 0x6 << LED_CTL1_CCOMEN0_POS )
#define LED_COM_NUM_8                      ( (uint32_t) 0x7 << LED_CTL1_CCOMEN0_POS )
#define CHECK_COM_NUMBER_SEL(SEL)          (((SEL) == LED_COM_NUM_1) \
										   || ((SEL) == LED_COM_NUM_2)\
										   || ((SEL) == LED_COM_NUM_3)\
										   || ((SEL) == LED_COM_NUM_4)\
										   || ((SEL) == LED_COM_NUM_5)\
										   || ((SEL) == LED_COM_NUM_6)\
										   || ((SEL) == LED_COM_NUM_7)\
										   || ((SEL) == LED_COM_NUM_8) )


/**
 * LED COM�˿�ѡ��
 */
#define	LED_COM_SEL_0                      ((uint16_t) 0)
#define	LED_COM_SEL_1                      ((uint16_t) 1)
#define LED_COM_SEL_2                      ((uint16_t) 2)
#define LED_COM_SEL_3                      ((uint16_t) 3)
#define LED_COM_SEL_4                      ((uint16_t) 4)
#define LED_COM_SEL_5                      ((uint16_t) 5)
#define LED_COM_SEL_6                      ((uint16_t) 6)
#define LED_COM_SEL_7                      ((uint16_t) 7)
#define CHECK_LED_COM_SEL(SEL)             ((SEL>>3) ==0)

/**
 * LED �Զȵ�������ѡ��
 */
#define LED_COM_LUM_1                      ( (uint32_t) 0x0 << LED_CTL1_LUM0_POS )
#define LED_COM_LUM_14                     ( (uint32_t) 0x1 << LED_CTL1_LUM0_POS )
#define LED_COM_LUM_12                     ( (uint32_t) 0x2 << LED_CTL1_LUM0_POS )
#define LED_COM_LUM_10                     ( (uint32_t) 0x3 << LED_CTL1_LUM0_POS )
#define LED_COM_LUM_8                      ( (uint32_t) 0x4 << LED_CTL1_LUM0_POS )
#define LED_COM_LUM_6                      ( (uint32_t) 0x5 << LED_CTL1_LUM0_POS )
#define LED_COM_LUM_4                      ( (uint32_t) 0x6 << LED_CTL1_LUM0_POS )
#define LED_COM_LUM_2                      ( (uint32_t) 0x7 << LED_CTL1_LUM0_POS )
#define CHECK_LUM_SEL(SEL)                 (((SEL) == LED_COM_LUM_1) \
										   || ((SEL) == LED_COM_LUM_14)\
										   || ((SEL) == LED_COM_LUM_12)\
										   || ((SEL) == LED_COM_LUM_10)\
										   || ((SEL) == LED_COM_LUM_8)\
										   || ((SEL) == LED_COM_LUM_6)\
										   || ((SEL) == LED_COM_LUM_4)\
										   || ((SEL) == LED_COM_LUM_2) )

#ifdef KF32A_Periph_led
void LED_Reset(void);
void LED_Configuration (LED_InitTypeDef* ledInitStruct);
void LED_Struct_Init (LED_InitTypeDef* LEDInitStruct);
INTStatus LED_Get_Frame_Flag (void);
uint16_t LED_Display_Data_Value (uint32_t LED_DATAx);
#endif //KF32A_Periph_led
#endif /* _KF32A_BASIC_LED_H_ */

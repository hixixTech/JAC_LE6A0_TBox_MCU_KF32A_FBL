/**
  ******************************************************************************
  * �ļ���  kf32a_basic_aes.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˼���(AES)��ع��ܺ�����������غ궨�塣
  *********************************************************************
  */

#ifndef _KF32A_BASIC_AES_H_
#define _KF32A_BASIC_AES_H_

#include "KF32A_BASIC.h"


/**
  * AES����ģ�飨AES��ָ�붨��
  */
#define CHECK_AES_PERIPH(PERIPH)     ((PERIPH) == AES_SFR)

/**
 * AES ������Ϣ�ṹ��
 */
typedef struct
{
 uint32_t m_OFB;                 /*AES OFBģʽ�������
                                 ȡֵΪ�ꡰAES OFBģʽ������ơ��е�һ��*/
 uint32_t m_ClockSelect;         /*AES ����ʱ��Դѡ��
                                 ȡֵΪ�ꡰAES ����ʱ��Դѡ���е�һ��*/
 uint32_t m_ClockDiv;            /*AES ʱ�ӷ�Ƶ����
                                 ȡֵΪ�ꡰAES ʱ�ӷ�Ƶ���ơ��е�һ��*/
 uint32_t m_Run;                 /*AES ����ʹ�ܿ���
                                 ȡֵΪ�ꡰAES ����ʹ�ܿ��ơ��е�һ��*/
 uint32_t m_FinishIntEn;         /*AES ��������ж�ʹ��λ
                                 ȡֵΪ�ꡰAES ��������ж�ʹ��λ���е�һ��*/
 uint32_t m_FinishIntClr;        /*AES��������������
                                 ȡֵΪ�ꡰAES�������������ơ��е�һ��*/

} AES_InitTypeDef;



/**
 * AES OFBģʽ�������
 */
#define AES_OFB_ENABLE              ((uint32_t) 0x1 << AES_CTL_OFBEN_POS)
#define AES_OFB_DISABLE             ((uint32_t) 0x0 << AES_CTL_OFBEN_POS)
#define CHECK_AES_OFB(SEL)          (((SEL) == AES_OFB_ENABLE) \
                                    || ((SEL) == AES_OFB_DISABLE) )

/**
 * AES ����ʱ��Դѡ��
 */
#define AES_CLK_SEL_SCLK            ((uint32_t) 0x0 << AES_CTL_AESCS0_POS)
#define AES_CLK_SEL_HFCLK           ((uint32_t) 0x1 << AES_CTL_AESCS0_POS)
#define AES_CLK_SEL_LFCLK           ((uint32_t)0x2 << AES_CTL_AESCS0_POS)
#define CHECK_AES_CLKSOURCE(SEL)    (((SEL) == AES_CLK_SEL_SCLK) \
                                    || ((SEL) == AES_CLK_SEL_HFCLK) \
                                    || ((SEL) == AES_CLK_SEL_LFCLK))

/**
 * AES ʱ�ӷ�Ƶ����
 */
#define AES_CLK_DIV_1               ((uint32_t) 0x0 << AES_CTL_AESDIV0_POS)
#define AES_CLK_DIV_2               ((uint32_t) 0x1 << AES_CTL_AESDIV0_POS)
#define AES_CLK_DIV_4               ((uint32_t) 0x2 << AES_CTL_AESDIV0_POS)
#define AES_CLK_DIV_8               ((uint32_t) 0x3 << AES_CTL_AESDIV0_POS)
#define AES_CLK_DIV_16              ((uint32_t) 0x4 << AES_CTL_AESDIV0_POS)
#define AES_CLK_DIV_32              ((uint32_t) 0x5 << AES_CTL_AESDIV0_POS)
#define AES_CLK_DIV_64              ((uint32_t) 0x6 << AES_CTL_AESDIV0_POS)
#define AES_CLK_DIV_128             ((uint32_t) 0x7 << AES_CTL_AESDIV0_POS)
#define CHECK_AES_CLK_DIV(SEL)      (((SEL) == AES_CLK_DIV_1) \
                                    || ((SEL) == AES_CLK_DIV_2) \
                                    || ((SEL) == AES_CLK_DIV_4) \
                                    || ((SEL) == AES_CLK_DIV_8) \
                                    || ((SEL) == AES_CLK_DIV_16) \
                                    || ((SEL) == AES_CLK_DIV_32) \
                                    || ((SEL) == AES_CLK_DIV_64) \
                                    || ((SEL) == AES_CLK_DIV_128) )

/**
 * AES ����ʹ�ܿ���
 */
#define AES_RUN_ENABLE              ((uint32_t) 0x1 << AES_CTL_AESRUN_POS)
#define AES_RUN_DISABLE             ((uint32_t) 0x0 << AES_CTL_AESRUN_POS)
#define CHECK_AES_RUN(SEL)          (((SEL) == AES_RUN_ENABLE) \
                                    || ((SEL) == AES_RUN_DISABLE) )

/**
 * AES ��������ж�ʹ��λ
 */
#define AES_FINISH_INT_ENABLE       ((uint32_t) 0x1 << AES_CTL_AESFIE_POS)
#define AES_FINISH_INT_DISABLE      ((uint32_t) 0x0 << AES_CTL_AESFIE_POS)
#define CHECK_AES_FINISH_INT(SEL)   (((SEL) == AES_FINISH_INT_ENABLE) \
                                    || ((SEL) == AES_FINISH_INT_DISABLE) )

/**
 * AES��������������
 */
#define AES_FINISH_INT_CLEAR_ENABLE      ((uint32_t) 0x1 << AES_CTL_AESFIC_POS)
#define AES_FINISH_INT_CLEAR_DISABLE     ((uint32_t) 0x0 << AES_CTL_AESFIC_POS)
#define CHECK_AES_FINISH_INT_CLEAR(SEL)  (((SEL) == AES_FINISH_INT_CLEAR_ENABLE) \
                                        || ((SEL) == AES_FINISH_INT_CLEAR_DISABLE) )
#ifdef KF32A_Periph_aes
uint32_t GET_AES_OUTPUT0_DATA (void);
uint32_t GET_AES_OUTPUT1_DATA (void);
uint32_t GET_AES_OUTPUT2_DATA (void);
uint32_t GET_AES_OUTPUT3_DATA (void);
void AES_INPUT0_DATA (uint32_t DATA);
void AES_INPUT1_DATA (uint32_t DATA);
void AES_INPUT2_DATA (uint32_t DATA);
void AES_INPUT3_DATA (uint32_t DATA);
void AES_KEY0_DATA (uint32_t DATA);
void AES_KEY1_DATA (uint32_t DATA);
void AES_KEY2_DATA (uint32_t DATA);
void AES_KEY3_DATA (uint32_t DATA);
void AES_Struct_Init (AES_InitTypeDef* AESInitStruct);
void AES_Configuration (AES_InitTypeDef* AESInitStruct);
void AES_Reset ();
void AES_Start ();
void AES_CLR_Flag_Status ();
FlagStatus AES_Get_Flag_Status (void);
#endif //KF32A_Periph_aes
#endif /* _KF32A_BASIC_AES_H */

/**
  ******************************************************************************
  * 文件名  system_init.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2019-11-16
  * 描  述  该文件提供了外设时钟与系统时钟初始化
  *
  ******************************************************************************
  */
#include "system_init.h"
/**
  * 描述  时钟默认配置。
  * 输入  无。
  * 返回  无。
*/
static void SetSysClock(void)
{
	OSC_Backup_Write_Read_Enable(TRUE);  						//使能备份域读写功能
	SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS); 			//备份域退出复位状态

	PM_EXTHF_PIN_Selection_Config(PM_EXTHF_PIN2_IO_PORT); 		//外部高频晶振PD9与PD10中晶振引脚

#ifdef SYSCLK_FREQ_HSE
	OSC_EXTHF_Software_Enable(TRUE); 							//外部高频晶振使能
	OSC_EXTHF_Start_Delay_Config(EXT_START_DELAY_16);			//延时外部高频晶振控制
	while(OSC_Get_EXTHF_INT_Flag() != SET);						//等待外部高频晶振稳定
	OSC_PLL_Input_Source_Config(PLL_INPUT_EXTHF);				//PLL时钟源选择
#else
	OSC_INTHF_Software_Enable(TRUE);							//内部高频晶振使能
	while(OSC_Get_INTHF_INT_Flag() != SET);						//等待内部高频晶振稳定
	OSC_PLL_Input_Source_Config(PLL_INPUT_INTHF);				//PLL时钟源选择
#endif

	/* PLL倍频时钟选择 */
#ifdef PPL_CLK_48MHz
	OSC_PLL_Multiple_Value_Select(48,2,8);
#elif defined PPL_CLK_60MHz
	OSC_PLL_Multiple_Value_Select(30,2,4);
#elif defined PPL_CLK_72MHz
	OSC_PLL_Multiple_Value_Select(36,2,4);
#elif defined PPL_CLK_96MHz
	OSC_PLL_Multiple_Value_Select(12,1,2);
#elif defined PPL_CLK_120MHz
	OSC_PLL_Multiple_Value_Select(30,2,2);
#endif

	OSC_PLL_Start_Delay_Config(PLL_START_DELAY_1024);		//PPL延时控制
	OSC_PLL_Software_Enable(TRUE);							//PPL使能
	OSC_PLL_RST();											//PPL复位
	while(OSC_Get_PLL_INT_Flag() != SET);					//等待PPL稳定
	
	/* 主时钟配置 */
	OSC_SCK_Source_Config(SCLK_SOURCE_PLL);					//选择PPL时钟作为主时钟源
	OSC_SCK_Division_Config(SCLK_DIVISION_1);
	
	/* 外设高速时钟 */
	OSC_HFCK_Division_Config(HFCK_DIVISION_2);
	OSC_HFCK_Source_Config(SCLK_SOURCE_PLL);
	OSC_HFCK_Enable(TRUE);
}

/**
  * 描述  系统初始化。
  * 输入  无。
  * 返回  无。
*/
void SystemInit(void)
{
	/* 时钟配置 */
	SetSysClock();
	FLASH_CFG=0xC3;//FLASH读取周期4周期
}


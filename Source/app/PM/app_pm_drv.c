/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_drv.c
* Description:  PM模块
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  徐高腾        新建基线
* 2020-08-05   V11.01.01.01.00  徐高腾        因GPIO变更，暂将休眠唤醒GPIO删除
* 2020-08-13   V11.01.01.01.00  徐高腾        休眠唤醒外围电路电源控制
******************************************************************************/


/******************************************************************************
* 头文件
******************************************************************************/
#include "app_pm_drv.h"
#include "ecu_gpio.h"
/******************************************************************************
* 宏定义
******************************************************************************/
#define PM_BAKBATT_UNK      0
#define PM_BAKBATT_USED     1
#define PM_BAKBATT_UNUSED   2

/******************************************************************************
* 电源控制
******************************************************************************/


/******************************************************************************
* 变量定义
******************************************************************************/



/******************************************************************************
* 函数定义
******************************************************************************/

static void ecu_iod_gpio_Can_Int_Wakeup_Enable(void)
{
    // FCLA0CTL0_INTPL = 0x01;
}

static void ecu_iod_gpio_Rtc_Int_Wakeup_Enable(void)
{
    // FCLA0CTL2_INTPL = 0x02;
}

static void DiasBakBatteryProcess(void)
{
    static uint8_t u8BackBat = PM_BAKBATT_UNK;
    static uint16_t u16tCnt = 0;
    // if ( ApiAdcVoltRead(ADC_CHANNEL_2) > 4400 )
    // {
    //     if ( u8BackBat != PM_BAKBATT_UNUSED)
    //     {
    //         u16tCnt++;
    //         if ( u16tCnt > 20)
    //         {
    //             u16tCnt = 0;
    //             u8BackBat = PM_BAKBATT_UNUSED;
    //             ApiGpioOutputLow(GPIO_NMBAT_CHRG);
    //         }
    //     }
    //     else
    //     {
    //         u16tCnt = 0;
    //     }
        
  
    // }
    // else if (( ApiAdcVoltRead(ADC_CHANNEL_2) < 3900 ) && ( ApiAdcVoltRead(ADC_CHANNEL_2) > 1500 ))
    // {
    //     if ( u8BackBat != PM_BAKBATT_USED )
    //     {
    //         u16tCnt++;
    //         if ( u16tCnt > 20)
    //         {
    //             u16tCnt = 0;
    //             u8BackBat = PM_BAKBATT_USED;
    //             ApiGpioOutputHigh(GPIO_NMBAT_CHRG);
    //         }
    //     }
    //     else
    //     {
    //         u16tCnt = 0;
    //     }
        
    // }
}


/* MCU */
void DiasPmDrvTimeout(void)
{
#if DIAS_PM_KL15_CHECK == 1
    ecu_iod_lte_Mcm63mc_KL15_Detc();
#endif
    DiasBakBatteryProcess();
}

void DiasPmWakeupEnable(void)
{
    ecu_iod_gpio_Can_Int_Wakeup_Enable();
    ecu_iod_gpio_Rtc_Int_Wakeup_Enable();
    // ecu_iod_gpio_Mcm63mcINTPMode();
    // ecu_iod_gpio_ACC_INTPMode();
    // ecu_iod_gpio_BtINTPMode();
}

void DiasPmMcuDrvInit(void)
{

}

void DiasPmEnableHwt(void)
{
#ifdef HWT_SWITCH
    (void)ApiWdtHwtEnable();
#endif
}

void DiasPmDisableHwt(void)
{
#ifdef HWT_SWITCH
    (void)ApiWdtHwtDisable();
#endif
}

void DiasPmMcuEnterStop(void)
{
    ApiStbcEnterStop();
}

void DiasPmMcuEnterDeepstop(void)
{
    // ApiStbcEnterDeepstop();
}

void ApiPmPeripheralCtrlMcuRun(void)
{
	(void)ApiGpioOutputLow(GPIO_CAN0_STB);
	(void)ApiGpioOutputLow(GPIO_CAN1_STB);
	(void)ApiGpioOutputLow(GPIO_CAN2_STB);
    (void)ApiGpioOutputHigh(GPIO_NMBAT_CHRG);
    (void)ApiGpioOutputHigh(GPIO_BAT_DET_CTL);
    (void)ApiGpioOutputHigh(GPIO_AUX_BOX_MUTE);
    (void)ApiGpioOutputHigh(GPIO_SPK_BOX_MUTE);
    (void)ApiGpioOutputHigh(GPIO_SOSLED_CTL);
    (void)ApiGpioOutputHigh(GPIO_CODEC_EN);
    (void)ApiGpioOutputHigh(GPIO_4G_VBUS_CNTL);
    //(void)ApiGpioOutputHigh(GPIO_SEC_EN);
    (void)ApiGpioOutputHigh(GPIO_GSENSOR_EN);
    (void)ApiGpioOutputHigh(GPIO_DIV8_EN);
    (void)ApiGpioOutputHigh(GPIO_MIC_EN);
}

void DiasPmPeripheralCtrlMcuStop(void)
{
	(void)ApiGpioOutputHigh(GPIO_CAN0_STB);
	(void)ApiGpioOutputHigh(GPIO_CAN1_STB);
	(void)ApiGpioOutputHigh(GPIO_CAN2_STB);
    (void)ApiGpioOutputLow(GPIO_NMBAT_CHRG);
    (void)ApiGpioOutputLow(GPIO_NMBAT_SW);
    (void)ApiGpioOutputLow(GPIO_BAT_DET_CTL);
    (void)ApiGpioOutputLow(GPIO_AUX_BOX_MUTE);
    (void)ApiGpioOutputLow(GPIO_SPK_BOX_MUTE);
    (void)ApiGpioOutputLow(GPIO_SOSLED_CTL);
    (void)ApiGpioOutputLow(GPIO_CODEC_EN);
    (void)ApiGpioOutputLow(GPIO_4G_VBUS_CNTL);
    //(void)ApiGpioOutputLow(GPIO_SEC_EN);
    (void)ApiGpioOutputLow(GPIO_GSENSOR_EN);
    (void)ApiGpioOutputLow(GPIO_DIV8_EN);
    (void)ApiGpioOutputLow(GPIO_MIC_EN);
}

void DiasPmPeripheralCtrlMcuDeepstop(void)
{

	(void)ApiGpioOutputHigh(GPIO_CAN0_STB);
	(void)ApiGpioOutputHigh(GPIO_CAN1_STB);
	(void)ApiGpioOutputHigh(GPIO_CAN2_STB);
    (void)ApiGpioOutputLow(GPIO_NMBAT_CHRG);
    (void)ApiGpioOutputLow(GPIO_NMBAT_SW);
    (void)ApiGpioOutputLow(GPIO_BAT_DET_CTL);
    (void)ApiGpioOutputLow(GPIO_AUX_BOX_MUTE);
    (void)ApiGpioOutputLow(GPIO_SPK_BOX_MUTE);
    (void)ApiGpioOutputLow(GPIO_SOSLED_CTL);
    (void)ApiGpioOutputLow(GPIO_CODEC_EN);
    (void)ApiGpioOutputLow(GPIO_4G_VBUS_CNTL);
    //(void)ApiGpioOutputLow(GPIO_SEC_EN);
    (void)ApiGpioOutputLow(GPIO_GSENSOR_EN);
    (void)ApiGpioOutputLow(GPIO_DIV8_EN);
    (void)ApiGpioOutputLow(GPIO_MIC_EN);
    (void)ApiGpioOutputLow(GPIO_DOG_EN);

    (void)ApiGpioOutputLow(GPIO_MCUSW_EN);

    // ecu_iod_gpio_ACC_INTPMode();
}

void DiasPmModDrvInit(void)
{
}

VOID DiasPmModPowerUp(VOID)
{
    //(void)ApiGpioOutputLow(GPIO_4G_EN);
}

VOID DiasPmModPowerDown(VOID)
{
    //(void)ApiGpioOutputLow(GPIO_4G_EN);
}

void DiasPmPeripheralCtrlModRun(void)
{
}

void DiasPmPeripheralCtrlModOff(void)
{
}

void DiasPmPeripheralCtrlModSleep(void)
{
}

BOOL DiasPmIsCrashed(void)
{
    return FALSE;
}


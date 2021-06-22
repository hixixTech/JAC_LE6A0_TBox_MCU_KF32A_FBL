/******************************************************************************
* Copyright (C), 2018, DIAS Automotive Electronic Systems Co; Ltd.
* File Name:    app_pm_lte_dcpd_com.c
* Description:  PM模块
* 
* Others:       other description
* 
* Processor:    RH850F1K
* Compiler:     GHS_201517
********************************** 修改历史 ***********************************
* Date            Version       Author     Description                        
* 2020-08-03   V11.01.01.01.00  徐高腾        新建基线
* 2020-08-05   V11.01.01.01.00  徐高腾        增加总线长时间OFF休眠超时功能
* 2020-08-06   V11.01.01.01.00  徐高腾        编译警告修改
* 2020-08-13   V11.01.01.01.00  徐高腾        DCPD消息发送接收方法修改
* 2020-08-13   V11.01.01.01.00  徐高腾        增加休眠失败消息通知及ACK处理
* 2020-08-13   V11.01.01.01.00  徐高腾        增加互斥锁gtPmLock
******************************************************************************/


/******************************************************************************
* 头文件
******************************************************************************/
#include "app_pm_lte_dcpd_com.h"
#include "os_log.h"
#include "app_pm_public_define.h"
#include "app_pm_peripheral_sys_cfg.h"
#include "app_pm_peripheral_sys.h"
#include "spp_generated.h"
// #include "dias_spp_main.h"
#include "spp_code.h"
#include "app_pm_power_mode.h"



/******************************************************************************
* 宏定义
******************************************************************************/


/******************************************************************************
* 类型定义
******************************************************************************/


/******************************************************************************
* 变量定义
******************************************************************************/


/******************************************************************************
* 回调函数声明
******************************************************************************/
static void DiasPmLteIsSleepReadyCallback(UINT16 u16ReceiveEventId, SppPackage_t *ptData, UINT32 u32DataLen);
static void DiasPmLtePowerModeChangeEndCallback(UINT16 u16ReceiveEventId, SppPackage_t *ptData, UINT32 u32DataLen);
static void DiasPmLteResetDeviceCallback(UINT16 u16ReceiveEventId, SppPackage_t *ptData, UINT32 u32DataLen);
static void DiasPmNotifySleepErrAckCallback(UINT16 u16ReceiveEventId, SppPackage_t *ptData, UINT32 u32DataLen);


/******************************************************************************
*  function name | DiasPmLteNotifyPowerMode
*  content       | 电源模式广播通知
*  parameter[1]  | PmPowerModeStatus_e ePowerMode：电源模式
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmLteNotifyPowerMode(PmPowerModeStatus_e ePowerMode)
{
    DcpdPackageToMpuPmStatusUpdate_t tDcpdPmStatusUpdate = {0};
    BOOL bDcpdCommIsReady = ApiDcpdComIsReady();

    /* LTE模组和MPU电源模式通知 */
    if(bDcpdCommIsReady == FALSE)
    {
        return;
    }

    /* 通知电源模式切换 */
    tDcpdPmStatusUpdate.tHeader.u8DstSrvId = 0u;
    tDcpdPmStatusUpdate.tHeader.u8SrcSrvId = 0u;
    tDcpdPmStatusUpdate.tHeader.u16EventId = (UINT16)SPP_USER_DCPD_MSG_TO_MPU_PM_STATUS_UPDATE_EVT;
    tDcpdPmStatusUpdate.PowerMode = (UINT8)ePowerMode;
    tDcpdPmStatusUpdate.u8BattVoltOperASta = (UINT8)ApiPmBatyVolConsGet(INDEX_BATY_CONS_OPERA);
    tDcpdPmStatusUpdate.u8BattVoltOperCANASta = (UINT8)ApiPmBatyVolConsGet(INDEX_BATY_CONS_OPERCANA);
    tDcpdPmStatusUpdate.u8BattVoltOperDiagASta = (UINT8)ApiPmBatyVolConsGet(INDEX_BATY_CONS_DIAG);
    if ( ApiPmIsKeyStateOff() == TRUE )
    {
        tDcpdPmStatusUpdate.u8KL15 = 0;
    }
    else
    {
        tDcpdPmStatusUpdate.u8KL15 = 1;
    }
    tDcpdPmStatusUpdate.u8KL30 = 1;

    ApiSPPAddEvent(SPP_USER_DCPD_MSG_TO_MPU_PM_STATUS_UPDATE_EVT,(UINT8*)(&tDcpdPmStatusUpdate),sizeof(DcpdPackageToMpuPmStatusUpdate_t));
}

/******************************************************************************
*  function name | DiasPmLteRegisterEventListener
*  content       | pm app与DCPD相关回调函数注册
*  parameter[1]  | void
*  notice        | 无
*  return        | void
******************************************************************************/
void DiasPmLteRegisterEventListener(void)
{
    ApiSPPSetListener(SPP_USER_DCPD_MSG_TO_MCU_IS_SLEEP_READY_EVT,DiasPmLteIsSleepReadyCallback);
    ApiSPPSetListener(SPP_USER_DCPD_MSG_TO_MCU_PM_CHANGE_END_EVT,DiasPmLtePowerModeChangeEndCallback);
    //ApiSPPSetListener(SPP_USER_DCPD_MSG_TO_MCU_RESET_TBOX_EVT,DiasPmLteResetDeviceCallback);
    ApiSPPSetListener(SPP_USER_DCPD_MSG_TO_MCU_SLEEP_ERR_ACK_EVT,DiasPmNotifySleepErrAckCallback);
}

/******************************************************************************
*  function name | DiasPmLteIsSleepReadyCallback
*  content       | Lte是否可以进入休眠DCPD消息回调函数
*  parameter     | UINT16 u16ReceiveEventId：事件id
*                | SppPackage_t* ptData：消息
*                | UINT32 u32DataLen：消息长度
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmLteIsSleepReadyCallback(UINT16 u16ReceiveEventId, SppPackage_t *ptData, UINT32 u32DataLen)
{
    DcpdPackageToMcuSleepReady_t *ptSleepReadyData = (DcpdPackageToMcuSleepReady_t *)ptData;
    if ((ptSleepReadyData == NULL) || (u32DataLen != sizeof(DcpdPackageToMcuSleepReady_t)))
    {
        return;
    }

    /* SPP_USER_DCPD_MSG_TO_MCU_IS_SLEEP_READY_EVT消息只有TRUE和FALSE,其他均为无效值 */
    if (ptSleepReadyData->u8Result > 1u)
    {
        return;
    }
    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Lte sleep ready:%d\n", ptSleepReadyData->u8Result);


    // (VOID)GetResource(gtPmLock);
    gatPmSysInfo[DIAS_PM_LTE].u16NoSleepReadyMsgTimer = 0;

    if (ptSleepReadyData->u8Result == TRUE)
    {
        if (gatPmSysInfo[DIAS_PM_LTE].u32NoServiceStartTime == 0u)
        {
            gatPmSysInfo[DIAS_PM_LTE].u32NoServiceStartTime = ApiAuxTaskGetAlarmTimeUtc();
        }
    }
    else
    {
        gatPmSysInfo[DIAS_PM_LTE].u32NoServiceStartTime = 0u;
    }

    if (gatPmSysInfo[DIAS_PM_LTE].bIsSleepReady != (BOOL)ptSleepReadyData->u8Result)
    {
        gatPmSysInfo[DIAS_PM_LTE].bIsSleepReady = (BOOL)ptSleepReadyData->u8Result;
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Lte new sleep ready status:%d\n", ptSleepReadyData->u8Result);
    }

    // (void)ReleaseResource(gtPmLock);

    (void)u16ReceiveEventId;
}

/******************************************************************************
*  function name | DiasPmLtePowerModeChangeEndCallback
*  content       | Lte电源模式切换完成消息回调函数
*  parameter     | uint16_t u16ReceiveEventId：事件id
*                | SppPackage_t* ptData：消息
*                | uint32_t u32DataLen：消息长度
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmLtePowerModeChangeEndCallback(UINT16 u16ReceiveEventId, SppPackage_t *ptData, UINT32 u32DataLen)
{
    DcpdPackageToMcuPmChangeEnd_t *ptPmChangeEndData = (DcpdPackageToMcuPmChangeEnd_t *)ptData;
    if ((ptPmChangeEndData == NULL) || (u32DataLen != sizeof(DcpdPackageToMcuPmChangeEnd_t)))
    {
        return;
    }

    if (ptPmChangeEndData->u8PowerMode >= PM_POWER_MODE_END)
    {
        return;
    }

    // (void)GetResource(gtPmLock);

    if ((UINT8)gatPmSysInfo[DIAS_PM_LTE].ePowerModeStatus != ptPmChangeEndData->u8PowerMode)
    {
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM: Lte power mode change:%d to %d\n", gatPmSysInfo[DIAS_PM_LTE].ePowerModeStatus, ptPmChangeEndData->u8PowerMode);
        gatPmSysInfo[DIAS_PM_LTE].ePowerModeStatus = (PmPowerModeStatus_e)ptPmChangeEndData->u8PowerMode;
    }

    // (void)ReleaseResource(gtPmLock);

    (void)u16ReceiveEventId;
}

/******************************************************************************
*  function name | DiasPmLteResetDeviceCallback
*  content       | 设备重启回调函数
*  parameter     | uint16_t u16ReceiveEventId：事件id
*                | SppPackage_t* ptData：消息
*                | uint32_t u32DataLen：消息长度
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmLteResetDeviceCallback(UINT16 u16ReceiveEventId, SppPackage_t *ptData, UINT32 u32DataLen)
{
    DcpdPackageToMcuResetTboxReq_t *ptResetData = (DcpdPackageToMcuResetTboxReq_t *)ptData;
    if((ptData == NULL) || (u32DataLen != sizeof(DcpdPackageToMcuResetTboxReq_t)))
    {
        return;
    }

    ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "ResetReqType:%d.\n", ptResetData->u8Type);

    // (void)GetResource(gtPmLock);
    if (gbPmResetMcu == TRUE)
    {
        // (void)ReleaseResource(gtPmLock);
        ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "Reset failed as mcu slept.\n");
        return;
    }

    switch(ptResetData->u8Type)
    {
        case 0: //reserve
        {
            break;
        }
        case 1: //modem ps:4G internal control
        {
            break;
        }
        case 2: //MCU
        {
            (void)ApiPmResetMCU();
        }
        case 3: //4G
        {
            if (ApiPmResetMPU() != OK)
            {
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:Cannot reset mpu.\n");
            }
            break;
        }
        case 4: //BLE
        {
            if (ApiPmResetBLE() != OK)
            {
                ApiLogPrintExt(PM_TASK_DEBUG, _LOG_ERROR, "PM:Cannot reset BLE.\n");
            }
            break;
        }
        case 0xFF: //All
        {
            (VOID)ApiPmResetSystem(FALSE);
            ApiLogPrintExt(PM_TASK_DEBUG, _LOG_WARNING, "PM:Start reset all sys by mpu.\n");
            break;
        }
        default:
        {
            break;
        }
    }
    // (void)ReleaseResource(gtPmLock);
    (void)u16ReceiveEventId;
}

/******************************************************************************
*  function name | DiasPmLteNotifySleepFailure
*  content       | 休眠失败通知
*  parameter[1]  | void
*  notice        | 无
*  return        | int32_t：OK(发送成功)/FALSE(发送失败)
******************************************************************************/
int32_t DiasPmLteNotifySleepFailure(void)
{
    DcpdPackageHeader_t tSleepFailureMsg = {0};
    BOOL bDcpdCommIsReady = ApiDcpdComIsReady();
    if(bDcpdCommIsReady == FALSE)
    {
        return ERR;
    }

    /* 通知休眠失败 */
    tSleepFailureMsg.u8DstSrvId = 0u;
    tSleepFailureMsg.u8SrcSrvId = 0u;
    tSleepFailureMsg.u16EventId = (UINT16)SPP_USER_DCPD_MSG_TO_MPU_SLEEP_ERR_EVT;
    ApiSPPAddEvent(SPP_USER_DCPD_MSG_TO_MPU_SLEEP_ERR_EVT,(UINT8*)(&tSleepFailureMsg),sizeof(tSleepFailureMsg));

    return OK;
}

/******************************************************************************
*  function name | DiasPmLteIsSleepReadyCallback
*  content       | Lte是否可以进入休眠DCPD消息回调函数
*  parameter     | UINT16 u16ReceiveEventId：事件id
*                | SppPackage_t* ptData：消息
*                | UINT32 u32DataLen：消息长度
*  notice        | 无
*  return        | void
******************************************************************************/
static void DiasPmNotifySleepErrAckCallback(UINT16 u16ReceiveEventId, SppPackage_t *ptData, UINT32 u32DataLen)
{
    if((ptData == NULL) || (u32DataLen != sizeof(DcpdPackageHeader_t)))
    {
        return;
    }

    // (void)GetResource(gtPmLock);
    if (gatPmSysInfo[DIAS_PM_LTE].ptSleepFailNotifyInfo != NULL)
    {
        if ((gatPmSysInfo[DIAS_PM_LTE].ptSleepFailNotifyInfo->eNotifyStatus == DIAS_PM_WAIT_ACK) ||
            (gatPmSysInfo[DIAS_PM_LTE].ptSleepFailNotifyInfo->eNotifyStatus == DIAS_PM_SENDING))
        {
            gatPmSysInfo[DIAS_PM_LTE].ptSleepFailNotifyInfo->eNotifyStatus = DIAS_PM_NOTIFY_SUCCESS;
        }
    }
    // (void)ReleaseResource(gtPmLock);
}


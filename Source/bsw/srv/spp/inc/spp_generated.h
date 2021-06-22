

#ifndef MPU_SPP_H
#define MPU_SPP_H

/*#include "types.h"*//*by S811*/



#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

#define SPP_USER_DCPD_MSG_TOMPU_START  (110)

#define SPP_USER_DCPD_MSG_TOMCU_START  (10)

typedef enum 
{
    SPP_USER_DCPD_MSG_TO_MCU_IS_SLEEP_READY_EVT = SPP_USER_DCPD_MSG_TOMCU_START,    /*!< __Description:__ 发送mpu是否可以sleep消息 \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_PM \n
                                                                        __RequestData:__ DcpdPackageToMcuSleepReady_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_PM_CHANGE_END_EVT,                   /*!< __Description:__ MPU发送电源状态切换完毕 \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_PM \n
                                                                        __RequestData:__ DcpdPackageToMcuPmChangeEnd_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_EOL_CMD_RESP_EVT,             /*!< __Description:__ EOL指令应答 \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ 参考 @ref   DcpdPackageToMcuEolCmdResp_t  \n
                                                                        __ResponseData:__ None \n
                                                                  */
    SPP_USER_DCPD_MSG_TO_MCU_IMEI_RESP_EVT,                /*!< __Description:__ 返回IMEI信息 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuImei_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_GNSS_EVT,                         /*!< __Description:__ 同步Gnss数据给MCU \n
                                                                        __Sender:__ Mpu_GnssSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuGnss_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_INICFG_REQ_EVT,                   /*!< __Description:__ 查询初始配置信息 \n
                                                                        __Sender:__ Mpu_cfgApp \n
                                                                        __Receiver:__ Mcu_DIAG \n
                                                                        __RequestData:__ None  \n
                                                                        __ResponseData:__   参考 @ref SPP_USER_DCPD_MSG_TO_MPU_INICFG_RESP_EVT \n
                                                                    */  
    SPP_USER_DCPD_MSG_TO_MCU_CFG_SV_REQ_EVT,                   /*!< __Description:__ 请求保存配置信息 \n
                                                                        __Sender:__ Mpu_cfgApp \n
                                                                        __Receiver:__ Mcu_DIAG \n
                                                                        __RequestData:__ 参考 @ref DcpdPackageToMcuCfgSvReq_t  \n
                                                                        __ResponseData:__   参考 @ref SPP_USER_DCPD_MSG_TO_MPU_CFG_SV_RESP_EVT \n
                                                                    */  
    SPP_USER_DCPD_MSG_TO_MCU_ICCID_RESP_EVT,              /*!< __Description:__ 返回ICCID信息 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuIccid_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_IMSI_RESP_EVT,              /*!< __Description:__ 返回IMSI信息 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuImsi_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_NADSTA1_RESP_EVT,            /*!< __Description:__ 返回Nad状态信息 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuMccMnc_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_NADSTA2_RESP_EVT,            /*!< __Description:__ 返回Nad状态信息 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuNadSta2_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_NADIP1_RESP_EVT,             /*!< __Description:__ 返回IP1信息 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuNadIp1_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_NADIP2_RESP_EVT,             /*!< __Description:__ 返回IP2信息 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuNadIp2_t \n
                                                                        __ResponseData:__ None \n
                                                                    */ 
    SPP_USER_DCPD_MSG_TO_MCU_RVC_OTA_MSG_EVT,                   /*!< __Description:__ OTA车控消息 \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_RVC \n
                                                                        __RequestData:__  参考 @ref DcpdPackageToMcuRvcOtaMsgRx_t \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_RVC_OTA_MSG_ACK_EVT  \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_RVC_OTA_TX_RESULT_EVT,             /*!< __Description:__ OTA车控消息发送的结果 \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_RVC \n
                                                                        __RequestData:__  DcpdPackageToMcuOtaTxResult_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_TBOX_REG_ACK_EVT,                  /*!< __Description:__ 终端注册应答 \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None  \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_TBOX_REG_RESULT_EVT,                  /*!< __Description:__ 终端注册结果 \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  DcpdPackageToMcuTboxRegResult_t \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_TBOX_REG_RESULT_ACK_EVT   \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_CFG_UPDATE_IND_ACK_EVT,           /*!< __Description:__ 配置信息更新应答 \n
                                                                        __Sender:__ Mpu_CfgApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None  \n
                                                                    */
                                                                    
    SPP_USER_DCPD_MSG_TO_MCU_WIFI_IND_ACK_EVT,             /*!< __Description:__ 返回socket码应答消息 \n
                                                                        __Sender:__ Mpu_WifiApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  DcpdPackageToMcuWifiIndResp_t \n
                                                                        __ResponseData:__ None  \n
                                                                    */
                                                                    
    SPP_USER_DCPD_MSG_TO_MCU_DTCRD_REQ_ACK_EVT,                   /*!< __Description:__ 请求读取DTC ACK \n
                                                                        __Sender:__ Mpu_FotaRDApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None  \n
                                                                    */  
    SPP_USER_DCPD_MSG_TO_MCU_PRE_RD_REQ_EVT,                   /*!< __Description:__ 请求读取远程诊断预读信息 \n
                                                                        __Sender:__ Mpu_FotaRDApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_PRE_RD_RESP_EVT  \n
                                                                    */ 
    SPP_USER_DCPD_MSG_TO_MCU_PRE_FOTA_REQ_EVT,                   /*!< __Description:__ 请求读取FOTA预读信息 \n
                                                                        __Sender:__ Mpu_FotaRDApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_PRE_FOTA_RESP_EVT\n
                                                                    */ 
    SPP_USER_DCPD_MSG_TO_MCU_FENCE_SAVE_REQ_EVT,                   /*!< __Description:__ 请求保存防护圈信息 \n
                                                                        __Sender:__ Mpu_RvmApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  参考  @ref DcpdPackageToMcuDefenceSvReq_t \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_FENCE_SAVE_RESP_EVT\n
                                                                    */
                                                                    
    SPP_USER_DCPD_MSG_TO_MCU_DEFENCE_ALM_EVT,                   /*!< __Description:__ 电子围栏报警通知 \n
                                                                        __Sender:__ Mpu_RvmApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_DEFENCE_ALM_ACK_EVT\n
                                                                    */
                                                                    
    SPP_USER_DCPD_MSG_TO_MCU_VEH_REPORT_ACK_EVT,                /*!< __Description:__ 车辆状态应答 \n
                                                                        __Sender:__ Mpu_FotaRvmApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_EMMC_STA_EVT,                     /*!< __Description:__ 发送EMMC卡状态信息 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv    \n
                                                                        __RequestData:__  DcpdPackageToMcuEmmcSta_t \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_EMMC_STA_ACK_EVT\n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_WIFI_STA_EVT,                     /*!< __Description:__ 发送WIFI状态信息 \n
                                                                        __Sender:__ Mpu_WifiApp \n
                                                                        __Receiver:__ Mcu_WanSrv    \n
                                                                        __RequestData:__  DcpdPackageToMcuWifiSta_t \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_WIFI_STA_ACK_EVT\n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_TSPCON_STA_EVT,                   /*!< __Description:__ 发送TSP连接状态信息 \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_WanSrv    \n
                                                                        __RequestData:__  DcpdPackageToMcuTspConSta_t \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_TSPCON_STA_ACK_EVT\n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_SLEEP_ERR_ACK_EVT,                /*!< __Description:__ 休眠异常ACK \n
                                                                        __Sender:__ Mpu_RvmApp \n
                                                                        __Receiver:__ Mcu_pm    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_WIN_AUTOCLOSE_ACK_EVT,            /*!< __Description:__ 雨天自动关窗应答 \n
                                                                        __Sender:__ Mpu_FotaRvmApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_SIM_STA_EVT,                          /*!< __Description:__ Dcpd发送SIM卡故障状态 \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__ DcpdPackageToMcuSimSta_t \n
                                                                        __ResponseData:__ 参考 @ref    SPP_USER_DCPD_MSG_TO_MPU_SIM_STA_ACK_EVT  \n
                                                                    */
    SPP_USER_MPU_RES_GNSS_EVT,                                 /*!< __Description:__ Dcpd发送GNSS信息 \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ DcpdPackageEolGnssInfo_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_MPU_RES_WAN_EVT,                                  /*!< __Description:__ Dcpd发送WAN信息 \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ DcpdPackageEolWanConnInfo_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_MPU_RES_UART_EVT,                                 /*!< __Description:__ Dcpd发送WAN信息 \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ DcpdPackageEolUartInfo_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_MPU_RES_PING_EVT,                                 /*!< __Description:__ Dcpd发送WAN信息 \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ DcpdPackageEolPingInfo_t \n
                                                                        __ResponseData:__ None \n
                                                                    */

    SPP_USER_DCPD_MSG_TO_MCU_RDIAG_REQ_EVT,                   /*!< __Description:__ Dcpd发送检测远程诊断请求 \n
                                                                        __Sender:__ Mpu_FotaEApp \n
                                                                        __Receiver:__ Mcu_DIAG \n
                                                                        __RequestData:__  参考 @ref DcpdPackageToMcuRDiagReq_t
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MPU_RDIAG_RESP_EVT \n
                                                                  */

    SPP_USER_DCPD_MSG_TO_MCU_SYS_TIME_REQ_EVT,                 /*!< __Description:__ 请求MCU的系统时间 \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_Auxiliary \n
                                                                        __RequestData:__ None \n
                                                                        __ResponseData:__ 参考 @ref   SPP_USER_DCPD_MSG_TO_MPU_SYS_TIME_RESP_EVT \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MCU_SYS_TIME_RESP_EVT,                 /*!< __Description:__ 发送Mpu的系统时间 \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_Auxiliary \n
                                                                        __RequestData:__ 参考 @ref   DcpdPackageToMcuSysTimeOfMpu_t  \n
                                                                        __ResponseData:__ None \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MCU_RVC_OTA_MSG_ACK_EVT,           /*!< __Description:__ MPU发送收到RVC OTA消息的ACK \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__  Mcu_RVC \n
                                                                        __RequestData:__ 参考 @ref DcpdPackageToMcuRvcOtaTxAck_t \n
                                                                        __ResponseData:__ None \n
                                                                 */
    SPP_USER_DCPD_MSG_TO_MCU_RESET_TBOX_EVT,                /*!< __Description:__ MPU发送复位TBOX \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_PM \n
                                                                        __RequestData:__ DcpdPackageToMcuResetTboxReq_t \n
                                                                        __ResponseData:__ None \n
                                                                    */

    SPP_USER_DCPD_MSG_TO_MCU_PHNUM_RESP_EVT,                /*!< __Description:__ MPU发送电话号码 \n
                                                                            __Sender:__ Mpu_WanSrv \n
                                                                            __Receiver:__ Mcu_WanSrv \n
                                                                            __RequestData:__ DcpdPackageToMcuPhnumResp_t \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_UPDATE_ENGINE_DATA_ACK_EVT,   /*!< __Description:__ MPU发送排放相关信号通知应答 \n
                                                                            __Sender:__ MPU_EmissionApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_KMS_TELE_INFO_ACK_EVENT,      /*!< __Description:__ MPU发送终端消息上报应答 \n
                                                                            __Sender:__ MPU_KMSApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_KMS_HB_ACK_EVENT,              /*!< __Description:__ MPU发送心跳数据上报应答 \n
                                                                            __Sender:__ MPU_KMSApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_APN_STA_EVT,               /*!< __Description:__ MPU发送私网APN连接状态 \n
                                                                            __Sender:__ Mpu_WanSrv \n
                                                                            __Receiver:__ Mcu_WanSrv \n
                                                                            __RequestData:__ DcpdPackageToMcuApnSta_t \n
                                                                            __ResponseData:__ None \n
                                                                        */ 			
    SPP_USER_DCPD_MSG_TO_MCU_APN_CFG_REQ_EVT,               /*!< __Description:__ MPU发送APN配置请求 \n
                                                                            __Sender:__ Mpu_WanSrv \n
                                                                            __Receiver:__ Mcu_WanSrv \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_APN_CFG_RESP_EVT \n
                                                                        */ 	
    SPP_USER_DCPD_MSG_TO_MCU_XCALL_INFO_REQ_EVT,               /*!< __Description:__ MPU发送APN配置请求 \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */ 	
    SPP_USER_DCPD_MSG_TO_MCU_CAN_TRANSMIT,                     /*!< __Description:__ MPU发CAN信号值MCU \n
                                                                            __Sender:__ Mpu_OtaLib \n
                                                                            __Receiver:__ Mcu_DIAG \n
                                                                            __RequestData:__ DcpdPackageCANSignalTransmit_t \n
                                                                            __ResponseData:__ None \n
                                                                        */ 
    SPP_USER_DCPD_MSG_TO_MCU_CTR_VEH_REQ_EVT,                  /*!< __Description:__ MPU发送整车切电请求 \n
                                                                            __Sender:__ Mpu_OtaLib \n
                                                                            __Receiver:__ Mcu_Rvc \n
                                                                            __RequestData:__ DcpdPackageToMcuCtrReq_t \n
                                                                            __ResponseData:__ DcpdPackageToMpuCtrRsp_t\n
                                                                        */
    SPP_USRT_DCPD_MSG_TO_MCU_WKE_NET_REQ_EVT,                    /*!< __Description:__ MPU发送唤醒整车can网络请求 \n
                                                                            __Sender:__ Mpu_OtaLib \n
                                                                            __Receiver:__ Mcu_diag \n
                                                                            __RequestData:__ DcpdPackageToMcuWkeNetReq_t \n
                                                                            __ResponseData:__ DcpdPackageToMPUWkeNetRst_t\n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_BLE_REFRESH_REQ_EVT,               /*!< __Description:__ MPU发送蓝牙刷新请求 \n
                                                                            __Sender:__ Mpu_FotaSrv \n
                                                                            __Receiver:__ Mcu_Ble \n
                                                                            __RequestData:__ DcpdPackageToMcuBleRefReq_t \n
                                                                            __ResponseData:__ DcpdPackageToMPUBleRefRst_t\n
                                                                        */
    
    SPP_USER_DCPD_MSG_TO_MCU_PGN_REQ_EVT,                       /*!< __Description:__ MPU发送PGN请求 \n
                                                                            __Sender:__ Mpu_Emission \n
                                                                            __Receiver:__ Mcu_diag \n
                                                                            __RequestData:__ DcpdPackageToMcuPGNReq_t \n
                                                                            __ResponseData:__ DcpdPackageToMPUPGNRsp_t\n
                                                                        */ 
         

    SPP_USER_DCPD_MSG_TO_MCU_KMS_DM1_ACK_EVENT,               /*!< __Description:__ MCU_KMS \n
                                                                            __Sender:__ MPU_KMSApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */
	SPP_USER_DCPD_MSG_TO_MCU_XCALL_REPORT_ACK_EVT,           /*!< __Description:__ MPU发送XCALL ACK  到ICT \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */ 	

    SPP_USER_DCPD_MSG_TO_MCU_XCALL_DIAL_ACK_EVT,           /*!< __Description:__ MPU收到XCALL按键拨打请求回复ACK \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_PM \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_XCALL_DIAL_FINISH_EVT,           /*!< __Description:__ MPU发送通话结束事件 \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_PM \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_XCALL_CODEC_SWITCH_EVT,           /*!< __Description:__ MPU发送音频通道切换请求事件 \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_PM \n
                                                                            __RequestData:__ INT8 0:DEFAULT 1:AUX 2:独立扬声器 \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_TSP_LOGIN_STATE_EVT,           /*!< __Description:__ MPU发送TSP登录状态事件 \n
                                                                            __Sender:__ Mpu_TspSrv \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ DcpdPackageToMCUTspLoginState_t
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_GET_HMI_UPDATA_EVT,				/*!< __Description:__ MPU发送获取升级条件事件 \n
                                                                            __Sender:__ Mpu_KMSAPP \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */	
    SPP_USER_DCPD_MSG_TO_MCU_HMI_ACK_EVT,					    /*!< __Description:__ MPU发送标定升级之前状态的ACK \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_ICM_UPDATA_EVT,                        /*!< __Description:__ MPU发送标定升级各个条件状态 \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */ 
    SPP_USER_DCPD_MSG_TO_MCU_BTC_OTA_MSG_ACK_EVT,               /*!< __Description:__ MPU发送接收到蓝牙密钥应答 \n
                                                                            __Sender:__ Mpu_TspSrv \n
                                                                            __Receiver:__ Mcu_BTC \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */ 
    SPP_USER_DCPD_MSG_TO_MCU_BTC_OTA_TX_RESULT_EVT,             /*!< __Description:__ MPU发送蓝牙密钥发送成功消息 \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ DcpdPackageToMcuBtcOtaTxResult_t \n
                                                                            __ResponseData:__ None \n
                                                                        */ 
    SPP_USER_DCPD_MSG_TO_MCU_ICT_KMS_PRIVATECAN_STATUS_MSG_EVT,/*!< __Description:__ MPU发送收到KMS私有CAN状态信号 \n
                                                                            __Sender:__ Mpu_KMSApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ DcpdPackageToMcuBtcOtaTxResult_t \n
                                                                            __ResponseData:__ None \n
                                                                        */ 

    SPP_USER_DCPD_MSG_TO_MCU_WIFI_REQ_DVR_VID_EVT,             /*!< __Description:__ 请求mcu唤醒dvr并请求远程视频 \n
                                                                        __Sender:__ Mpu_WifiApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  DcpdPackageToMcWifiDvrVidReq_t \n
                                                                        __ResponseData:__ None  \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_CTR_VEH_RES_ACK_EVT,                   /*!< __Description:__ 切电响应消息ACK \n
                                                                       __Sender:__ Mpu_TspSrv \n
                                                                       __Receiver:__  Mcu_VC\n
                                                                       __RequestData:__ 参考  \n
                                                                       __ResponseData:__  \n
                                                               */ 
    SPP_USER_DCPD_MSG_TOMCU_MAX

} EMpuSendEvent;

typedef enum 
{
    
    SPP_USER_DCPD_MSG_TO_MPU_PM_STATUS_UPDATE_EVT = SPP_USER_DCPD_MSG_TOMPU_START,    /*!< __Description:__ 发送电源管理模式 \n
                                                                                   __Sender:__ Mcu_PM \n
                                                                                   __Receiver:__ Mpu_SysMgrSrv \n
                                                                                   __RequestData:__ DcpdPackageToMpuPmStatusUpdate_t \n
                                                                                   __ResponseData:__ None \n
                                                                                    */
    SPP_USER_DCPD_MSG_TO_MPU_MCULOG_EVT,                   /*!< __Description:__ MCU Log消息 \n
                                                                       __Sender:__ Mcu_ \n
                                                                       __Receiver:__ Mpu_LogSrv \n
                                                                       __RequestData:__ 参考 @ref DcpdPackageToMpuMcuLog_t  \n
                                                                       __ResponseData:__  \n
                                                               */  
    SPP_USER_DCPD_MSG_TO_MPU_EOL_CMD_REQ_EVT,              /*!< __Description:__ EOL测试/认证指令 \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ 参考 @ref DcpdPackageToMpuEolCmdReq_t \n
                                                                       __ResponseData:__ 参考 @ref   SPP_USER_DCPD_MSG_TO_MCU_EOL_CMD_RESP_EVT \n
                                                                */
    SPP_USER_DCPD_MSG_TO_MPU_INICFG_RESP_EVT,              /*!< __Description:__ 返回初始配置信息 \n
                                                                           __Sender:__ Mcu_DIAG \n
                                                                           __Receiver:__ Mpu_CfgApp \n
                                                                           __RequestData:__ 参考 @ref DcpdPackageToMpuIniCfg_t  \n
                                                                           __ResponseData:__  \n
                                                                       */  
    SPP_USER_DCPD_MSG_TO_MPU_CFG_SV_RESP_EVT,                   /*!< __Description:__ 返回配置信息保存结果 \n
                                                                           __Sender:__ Mcu_DIAG \n
                                                                           __Receiver:__ Mpu_CfgApp \n
                                                                           __RequestData:__ 参考 @ref DcpdPackageToMpuCfgSvResp_t  \n
                                                                           __ResponseData:__  \n
                                                                       */  
    SPP_USER_DCPD_MSG_TO_MPU_IMEI_REQ_EVT,                   /*!< __Description:__ 请求IMEI信息   \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_IMEI_RESP_EVT   \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_ICCID_REQ_EVT,                  /*!< __Description:__ 请求ICCID信息  \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_ICCID_RESP_EVT  \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_IMSI_REQ_EVT,                   /*!< __Description:__ 请求IMSI信息   \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_IMSI_RESP_EVT   \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_NADSTA1_REQ_EVT,                /*!< __Description:__ 请求Nad状态信息  \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_NADSTA1_RESP_EVT    \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_NADSTA2_REQ_EVT,                /*!< __Description:__ 请求Nad状态信息  \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_NADSTA2_RESP_EVT  \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_NADIP1_REQ_EVT,              /*!< __Description:__ 请求IP1信息   \n
                                                                       __Sender:__ Mcu_WanSrv  \n
                                                                       __Receiver:__ Mpu_WanSrv    \n
                                                                       __RequestData:__  None   \n
                                                                       __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_NADIP1_RESP_EVT \n
                                                                   */

    SPP_USER_DCPD_MSG_TO_MPU_NADIP2_REQ_EVT,              /*!< __Description:__ 请求IP2信息   \n
                                                                       __Sender:__ Mcu_WanSrv  \n
                                                                       __Receiver:__ Mpu_WanSrv    \n
                                                                       __RequestData:__  None   \n
                                                                       __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_NADIP2_RESP_EVT \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_RVC_OTA_MSG_EVT,                   /*!< __Description:__ 车控Ota消息发送 \n
                                                                       __Sender:__ Mcu_RVC  \n
                                                                       __Receiver:__ Mpu_TspSrv    \n
                                                                       __RequestData:__  参考 @ref DcpdPackageToMpuRvcOtaMsgTx_t   \n
                                                                       __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_RVC_OTA_MSG_ACK_EVT  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_TBOX_REG_REQ_EVT,                  /*!< __Description:__ 请求终端注册 \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_TspSrv    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MCU_TBOX_REG_ACK_EVT  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_TBOX_REG_RESULT_ACK_EVT,          /*!< __Description:__ 请求终端注册结果应答 \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_TspSrv    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_CFG_UPDATE_IND_EVT,          /*!< __Description:__ 配置信息更新通知 \n
                                                                       __Sender:__ Mcu_ \n
                                                                       __Receiver:__ Mpu_cfgApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuCfgUpdateInd_t \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MCU_CFG_UPDATE_IND_ACK_EVT  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_WIFI_IND_EVT,                 /*!< __Description:__ 通知wifi请求事件：WiFi开启请求、socket码请求 \n
                                                                       __Sender:__ Mcu_ICT \n
                                                                       __Receiver:__ Mpu_WifiApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuWifiInd_t \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MCU_WIFI_IND_ACK_EVT  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_DTCRD_REQ_EVT,                /*!< __Description:__ 请求读取DTC \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_FotaRDApp    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MCU_DTCRD_REQ_ACK_EVT  \n
                                                               */
                                                               
    SPP_USER_DCPD_MSG_TO_MPU_PRE_RD_RESP_EVT,              /*!< __Description:__ 返回远程诊断预读信息 \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_FotaRDApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuPreRdResp_t \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_PRE_FOTA_RESP_EVT,              /*!< __Description:__ 返回FOTA预读信息 \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_FotaRDApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuPreFotaResp_t \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_FENCE_SAVE_RESP_EVT,          /*!< __Description:__ 防护圈信息的保存结果 \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuFenceSaveResp_t \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_DEFENCE_ALM_ACK_EVT,          /*!< __Description:__ 电子围栏报警应答 \n
                                                                       __Sender:__ Mcu_ICT \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_VEH_REPORT_EVT,               /*!< __Description:__ 车辆状态通知 \n
                                                                       __Sender:__ Mcu_ICT \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuVehReport_t \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MCU_VEH_REPORT_ACK_EVT  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_EMMC_STA_ACK_EVT,             /*!< __Description:__ 发送EMMC卡状态信息应答 \n
                                                                       __Sender:__ Mcu_WanSrv \n
                                                                       __Receiver:__ Mpu_WanSrv    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_WIFI_STA_ACK_EVT,             /*!< __Description:__ 发送WIFI状态信息应答 \n
                                                                       __Sender:__ Mcu_WanSrv \n
                                                                       __Receiver:__ Mpu_WifiApp    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_TSPCON_STA_ACK_EVT,             /*!< __Description:__ 发送TSP连接状态信息应答 \n
                                                                       __Sender:__ Mcu_WanSrv \n
                                                                       __Receiver:__ Mpu_TspSrv    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_SLEEP_ERR_EVT,               /*!< __Description:__ 休眠异常通知 \n
                                                                       __Sender:__ Mcu_PM \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MCU_SLEEP_ERR_ACK_EVT  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_WIN_AUTOCLOSE_EVT,               /*!< __Description:__ 发送雨天、熄火自动关窗通知 \n
                                                                       __Sender:__ Mcu_ICT \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  参考 @ref DcpdPackageToMpuWinAutoClose_t \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MCU_WIN_AUTOCLOSE_ACK_EVT  \n
                                                               */

    SPP_USER_DCPD_MSG_TO_MPU_SIM_STA_ACK_EVT,                      /*!< __Description:__ 消息  @ref SPP_USER_DCPD_MSG_TO_MCU_SIM_STA_EVT 的应答 \n
                                                                       __Sender:__ Mcu_WanSrv \n
                                                                       __Receiver:__ Mpu_WanSrv \n
                                                                       __RequestData:__ None \n
                                                                       __ResponseData:__ None \n
                                                                   */
    SPP_USER_MCU_REQ_GNSS_EVT,                                 /*!< __Description:__ 请求Gnss信息 \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ DcpdPackageHeader_t \n
                                                                       __ResponseData:__ 参考 @ref SPP_USER_MPU_RES_GNSS_EVT \n
                                                                   */
    SPP_USER_MCU_REQ_WAN_EVT,                                  /*!< __Description:__ 请求Wan信息 \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ DcpdPackageHeader_t \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_MPU_RES_WAN_EVT \n
                                                                   */
    SPP_USER_MCU_REQ_UART_EVT,                                 /*!< __Description:__ 请求UART通路测试 \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ DcpdPackageEolUartInfo_t \n
                                                                       __ResponseData:__ 参考   @ref SPP_USER_MPU_RES_UART_EVT \n
                                                                   */
    SPP_USER_MCU_REQ_PING_EVT,                                 /*!< __Description:__ 请求Ping网络 \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ DcpdPackageHeader_t \n
                                                                       __ResponseData:__ 参考   @ref  SPP_USER_MPU_RES_PING_EVT \n
                                                                   */

    SPP_USER_DCPD_MSG_TO_MPU_RDIAG_RESP_EVT,                    /*!< __Description:__ 返回远程诊断结果 \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_FotaRDSrv \n
                                                                       __RequestData:__ 参考 @ref   DcpdPackageToMpuRDiagResp_t \n
                                                                       __ResponseData:__ None \n
                                                                   */

    SPP_USER_DCPD_MSG_TO_MPU_SYS_TIME_RESP_EVT,            /*!< __Description:__ 发送MCU的系统时间 \n
                                                                       __Sender:__ Mcu_Auxiliary \n
                                                                       __Receiver:__ Mpu_SysMgrSrv \n
                                                                       __RequestData:__ 参考 @ref DcpdPackageToMpuSysTimeOfMcu_t \n
                                                                       __ResponseData:__ 参考 None  \n
                                                                */
    SPP_USER_DCPD_MSG_TO_MPU_SYS_TIME_REQ_EVT,             /*!< __Description:__ 请求MPU  的系统时间 \n
                                                                       __Sender:__ Mcu_Auxiliary \n
                                                                       __Receiver:__ Mpu_SysMgrSrv \n
                                                                       __RequestData:__ None \n
                                                                       __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_ SYS_TIME_RESP_EVT \n
                                                                */
    SPP_USER_DCPD_MSG_TO_MPU_RVC_OTA_MSG_ACK_EVT,           /*!< __Description:__ MCU发送收到RVC OTA消息的ACK \n
                                                                       __Sender:__ Mcu_RVC \n
                                                                       __Receiver:__ Mpu_TspSrv \n
                                                                       __RequestData:__ 参考 @ref DcpdPackageToMpuRxRvcOtaAck_t  \n
                                                                       __ResponseData:__ None \n
                                                                */
    SPP_USER_DCPD_MSG_TO_MPU_PHNUM_REQ_EVT,                /*!< __Description:__ MCU发送请求电话号码 \n
                                                                        __Sender:__ Mcu_WanSrv \n
                                                                        __Receiver:__ Mpu_WanSrv \n
                                                                        __RequestData:__ None \n
                                                                        __ResponseData:__ 参考 @ref SPP_USER_DCPD_MSG_TO_MCU_PHNUM_RESP_EVT \n
                                                                        */   
                                                                                                                                      
    SPP_USER_DCPD_MSG_TO_MPU_UPDATE_ENGINE_DATA_EVT,                /*!< __Description:__ MCU发送发动机数据 \n
                                                                        __Sender:__Mcu_ICT\n
                                                                        __Receiver:__Mpu_EmissionApp \n
                                                                        __RequestData:_参考 @ref   DcpdPackageToMpuEngData_t\n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_KMS_TELE_INFO_EVENT,         /*!< __Description:__ Mcu发送康明斯心跳数据上报 \n
                                                                            __Sender:__ Mcu_ICT  \n
                                                                            __Receiver:__ MPU_KMSApp\n
                                                                            __RequestData:__ DcpdPacketKMSTelemticInfo_t \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_KMS_HB_EVENT,                /*!< __Description:__ Mcu发送康明斯心跳数据上报 \n
                                                                            __Sender:__ Mcu_ICT  \n
                                                                            __Receiver:__ MPU_KMSApp\n
                                                                            __RequestData:__ DcpdPackageKmsHBData_t \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_APN_CFG_RESP_EVT,              /*!< __Description:__ MCU发送APN信息 \n
                                                                        __Sender:__ Mcu_WanSrv \n
                                                                        __Receiver:__ Mpu_WanSrv \n
                                                                        __RequestData:__ DcpdPackageToMpuApnCfg_t \n
                                                                        __ResponseData:__ None \n
                                                                        */    
    SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT,          /*!< __Description:__ MCU发送XCALL信息 \n
                                                                        __Sender:__ Mcu_ICT \n
                                                                        __Receiver:__ Mpu_XcallApp \n
                                                                        __RequestData:__ DcpdPackageToMpuVehReport_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_CAN_TRANSMIT,                      /*!< __Description:__ MCU转发CAN信号 \n
                                                                        __Sender:__ Mcu_DIAG \n
                                                                        __Receiver:__ Mpu_OtaLib \n
                                                                        __RequestData:__ DcpdPackageCANSignalTransmit_t \n
                                                                        __ResponseData:__ None \n
                                                                        */                                             
    SPP_USER_DCPD_MSG_TO_MPU_VEH_CON_DATA_EVT,             /*!< __Description:__ MCU发送fota相关的整车信息 \n
                                                                        __Sender:__ Mcu_DIAG \n
                                                                        __Receiver:__ Mpu_OtaLib \n
                                                                        __RequestData:__ DcpdPackageToMpuVehConData_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_CTR_VEH_RES_EVT,              /*!< __Description:__ MCU发送切电控制响应 \n
                                                                        __Sender:__ Mcu_RVC \n
                                                                        __Receiver:__ Mpu_OtaLib \n
                                                                        __RequestData:__ DcpdPackageToMpuCtrRsp_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USRT_DCPD_MSG_TO_MPU_WKE_NET_RES_EVT,              /*!< __Description:__ MCU发送唤醒网络请求响应 \n
                                                                        __Sender:__ Mcu_Diag \n
                                                                        __Receiver:__ Mpu_OtaLib \n
                                                                        __RequestData:__ DcpdPackageToMPUWkeNetRst_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_BLE_REFRESH_RESP_EVT,         /*!< __Description:__ MCU发送蓝牙刷新请求响应 \n
                                                                        __Sender:__ Mcu_Ble \n
                                                                        __Receiver:__ Mpu_FotaRDSrv \n
                                                                        __RequestData:__ DcpdPackageToMPUBleRefRst_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
                                                                        
    SPP_USER_DCPD_MSG_TO_MPU_PGN_RSP_EVT,                  /*!< __Description:__ MCU发送PGN请求响应 \n
                                                                        __Sender:__ Mcu_Diag \n
                                                                        __Receiver:__ Mpu_Emission \n
                                                                        __RequestData:__ DcpdPackageToMPUPGNRsp_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_PGN_BC_EVT,                  /*!< __Description:__ MCU发送PGN广播 \n
                                                                        __Sender:__ Mcu_Diag \n
                                                                        __Receiver:__ Mpu_Emission \n
                                                                        __RequestData:__ DcpdPackageToMPUPGNBC_t \n
                                                                        __ResponseData:__ None \n
                                                                        */      
    SPP_USER_DCPD_MSG_TO_MPU_KMS_DM1_EVENT,							/*!< __Description:__ Mcu发送康明斯DM1数据上报 \n
                                                                            __Sender:__ Mcu_ICT  \n
                                                                            __Receiver:__ MPU_KMSApp\n
                                                                            __RequestData:__ DcpdPackageKmsHBData_t \n
                                                                            __ResponseData:__ None \n
                                                                        */

    SPP_USER_DCPD_MSG_TO_MPU_XCALL_DIAL_FINISH_ACK_EVT,           /*!< __Description:__ MCU发送通话结束事件回复 \n
                                                                            __Sender:__ Mcu_PM \n
                                                                            __Receiver:__ Mpu_XcallApp \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_XCALL_CODEC_SWITCH_RESP_EVT,           /*!< __Description:__ McU发送音频通道切换成功回复 \n
                                                                            __Sender:__ Mcu_PM \n
                                                                            __Receiver:__ Mpu_XcallApp \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_XCALL_DIAL_REQ_EVT,           /*!< __Description:__ McU发送XCALL（ECALL）电话拨打请求 \n
                                                                            __Sender:__ Mcu_PM \n
                                                                            __Receiver:__ Mpu_XcallApp \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_TSP_LOGIN_ACK_EVT,           /*!< __Description:__ McU发送TSP登录成功消息应答 \n
                                                                                    __Sender:__ Mcu_PM \n
                                                                                    __Receiver:__ Mpu_XcallApp \n
                                                                                    __RequestData:__ None \n
                                                                                    __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                                */
	SPP_USER_DCPD_MSG_TO_MPU_KMS_HMI_UPDATA_EVENT,          /*!< __Description:__ Mcu发送康明斯标定升级条件状态数据上报 \n
                                                                            __Sender:__ Mcu_ICT  \n
                                                                            __Receiver:__ MPU_KMSApp\n
                                                                            __RequestData:__ DcpdPackageKmsHBData_t \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_KMS_HMI_UPDATA_SURE_EVENT,     /*!< __Description:__ HMI发送的仪表数据以及ack回应 \n
                                                                        __Sender:__ Mcu_ICT  \n
                                                                        __Receiver:__ MPU_KMSApp\n
                                                                        __RequestData:__ DcpdPackageKmsHBData_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MPU_BTC_OTA_MSG_EVT,               /*!< __Description:__ 发送蓝牙密钥到MPU \n
                                                                        __Sender:__ Mcu_btc  \n
                                                                        __Receiver:__ MPU_TspSrv\n
                                                                        __RequestData:__ DcpdPackageToMpuBtcOta_t \n
                                                                        __ResponseData:__ 参考   @ref SPP_USER_DCPD_MSG_TO_MCU_BTC_OTA_MSG_ACK_EVT \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MPU_BTC_OTA_TX_RESULT_ACK_EVT,     /*!< __Description:__ 应答蓝牙密钥发送结果 \n
                                                                        __Sender:__ Mcu_BTC  \n
                                                                        __Receiver:__ MPU_TspSrv\n
                                                                        __RequestData:__ None \n
                                                                        __ResponseData:__ None \n
                                                                    */

	SPP_USER_DCPD_MSG_TO_MPU_ICT_KMS_PRIVATECAN_STATUS_MSG_EVT, /*!< __Description:__ 发送MPUKMS私有CAN状态 \n
                                                                        __Sender:__ Mcu_ICT  \n
                                                                        __Receiver:__ MPU_KMSAPP\n
                                                                        __RequestData:__ None \n
                                                                        __ResponseData:__ None \n
                                                                    */

    SPP_USER_DCPD_MSG_TO_MPU_WIFI_DVR_VID_ACK_EVT,             /*!< __Description:__ mcu唤醒dvr并请求远程视频应答 \n
                                                                    __Sender:__ Mpu_ICT \n
                                                                    __Receiver:__ Mpu_WifiApp    \n
                                                                    __RequestData:__  DcpdPackageToMpuWifiDvrVidResp_t \n
                                                                    __ResponseData:__ None  \n
                                                                */
	SPP_USER_DCPD_MSG_TO_MPU_CAN_LOG_EVT,                   /*!< __Description:__ MCU CAN Log消息 \n
                                                                       __Sender:__ Mcu_ \n
                                                                       __Receiver:__ Mpu_RemoteLogSrv \n
                                                                       __RequestData:__ 参考 @ref DcpdPackageToMpuMcuLog_t  \n
                                                                       __ResponseData:__  \n
                                                               */ 
    SPP_USER_DCPD_MSG_TO_MPU_CTR_VEH_REQ_ACK_EVT,              /*!< __Description:__ 切电请求消息ACK \n
                                                                       __Sender:__ Mcu_VC \n
                                                                       __Receiver:__ Mpu_TspSrv \n
                                                                       __RequestData:__   \n
                                                                       __ResponseData:__  \n
                                                               */ 
    SPP_USER_DCPD_MSG_TO_MPU_RESET_TBOX_ACK_EVT,                 /*!< __Description:__ 复位请求ACK \n
                                                                       __Sender:__ Mpu_Cfg \n
                                                                       __Receiver:__  Mcu_PM\n
                                                                       __RequestData:__   \n
                                                                       __ResponseData:__  \n
                                                               */                                           
    SPP_USER_DCPD_MSG_TOMPU_MAX
} EMcuSendEvent;


/*!
typedef enum \n
{ \n
    VEH_REPORT_TYPE_PERIOD,     \n
    VEH_REPORT_TYPE_NEW_ENERGY,  \n
    VEH_REPORT_TYPE_ALER,        \n
    VEH_REPORT_TYPE_POWER_ON,    \n
    VEH_REPORT_TYPE_POWER_OFF,   \n
    VEH_REPORT_TYPE_VEH_CHANGE,   \n
    VEH_REPORT_TYPE_FAIL_CODE,    \n
    VEH_REPORT_TYPE_CLOSE_WIN,   \n
}DcpdPackageVehReportType_e;     \n
*/
typedef enum
{
    VEH_REPORT_TYPE_PERIOD,            /*!< period report*/
    VEH_REPORT_TYPE_NEW_ENERGY,        /*!< new energy data report*/
    VEH_REPORT_TYPE_ALER,              /*!< alert data report*/
    VEH_REPORT_TYPE_POWER_ON,          /*!< power on report*/
    VEH_REPORT_TYPE_POWER_OFF,         /*!< power off report*/
    VEH_REPORT_TYPE_VEH_CHANGE,        /*!< vehicle changed report*/
    VEH_REPORT_TYPE_FAIL_CODE,         /*!< fail code report*/
    VEH_REPORT_TYPE_CLOSE_WIN,         /*!< push data report*/
    VEH_REPORT_TYPE_ADAS_WARNING,
    VEH_REPORT_TYPE_SPP_FAILURE_CLASSIFICATION,
    VEH_REPORT_TYPE_BAT_PREWARN_DATA,
    VEH_REPORT_TYPE_ADAS_FUNC_DATA,
    VEH_REPORT_TYPE_GEAR_BOX_DATA
}DcpdPackageVehReportType_e;


/*!
typedef struct \n
{   \n
> UINT8 u8SatelliteInViewId; \n
> UINT8 u8Elevation;         \n
> UINT16 u16Azimuth;         \n
> UINT8 u8Snr;               \n
> UINT8 u8IsUsed;            \n
} DcpdPackageSatelliteDetail_t; \n
*/
typedef struct
{
    UINT8 u8SatelliteInViewId;  /*!< Satellite in view Id
                                - Range: 1 to 32*/
    UINT8 u8Elevation;          /*!< Satellite elevation
                                -# Units: 1 Degree
                                -# Range: 0 to 90
                                -# Res: 10 */
    UINT16 u16Azimuth;          /*!< Satellite azimuth
                                - Units: 1 Degree
                                - Range: 0 to 359*/
    UINT8 u8Snr;                /*!< Satellite SNR
                                - Units: 1 dbHz
                                - Range: 0 to 99*/
    UINT8 u8IsUsed;             /*!< Satellite in used or not
                                - 0: not in use
                                - 1: in used*/
} DcpdPackageSatelliteDetail_t;

/*!
typedef struct  \n
{   \n
DcpdPackageSatelliteDetail_t tSatellitesDetail[24]; \n
} DcpdPackageSatelliteDetail_tt; \n
*/
typedef struct
{
    DcpdPackageSatelliteDetail_t tSatellitesDetail[24]; /*!< Satellite detail info in FmcGnssSatelliteDetail_t
                                                struct.
                                                - MAX 24 satellites detail*/
} DcpdPackageSatelliteDetail_tt;

/*!
typedef struct                 \n
{
UINT8 u8InfoType;           \n
INT32 i32Longitude;         \n
INT32 i32Latitude;          \n
INT32 i32Altitude;          \n
INT16 i16Declination;       \n
UINT16 u16T_direction;      \n
UINT16 u16M_direction;      \n
UINT16 u16Speed;            \n
UINT16 u16PDOP;             \n
UINT16 u16HDOP;             \n
UINT16 u16VDOP;             \n
UINT8 u8FixStatus;          \n
UINT8 u8GnssLocateType;     \n
UINT8 u8VisibleSatellites;  \n
UINT8 u8UsingSatellites;    \n
UINT8 u8TimeStamp[12];      \n
FmcGnssSatelliteDetail_t tSatellitesDetail[24]; \n
} DcpdPackageNmeaInfo_t;*/
typedef struct
{
    UINT8 u8InfoType;           /*!< Gnss data info type, only support struct binary format*/
    INT32 i32Longitude;         /*!< Longitude (specified in WGS84 datum)
                                    - Units: 0.000001 Degrees
                                    - Range: -180000000 to 180000000(-180.000000 to
                                    180.000000 *1000000)
                                    - Positive values indicate eastern longitude
                                    - Negative values indicate western longitude*/
    INT32 i32Latitude;          /*!< Latitude (specified in WGS84 datum)
                                    - Units: 0.000001 Degrees
                                    - Range: -90000000 to 90000000(-90.000000 to
                                    90.000000 *1000000)
                                    - Positive values indicate northern latitude
                                    - Negative values indicate southern latitude*/
    INT32 i32Altitude;          /*!< Altitude with respect to the WGS84 ellipsoid
                                    - Units: 0.1 Meters
                                    - Range: -99999 to 999999(-9999.9 to 99999.9 *10)
                                    - Positive = height
                                    - Negative = depth*/
    INT16 i16Declination;       /*!< Magnetic declination
                                    - Units: 0.01 Degrees
                                    - Range: -18000 to 18000(-180 to 180 *100)*/
    UINT16 u16T_direction;      /*!< True north azimuth angle
                                    - Units: 0.01 Degrees
                                    - Range: 0 to 36000(0 to 360 *100)*/
    UINT16 u16M_direction;      /*!< Magnetic north azimuth angle
                                    - Units: 0.01 Degrees
                                    - Range: 00 to 36000(0 to 360 *100)*/
    UINT16 u16Speed;            /*!< Horizontal speed
                                    - Units: 0.01 km/hr*/
    UINT16 u16PDOP;             /*!< Position dilution of precision
                                    - Units: 0.1
                                    - Range: 5 to 999(0.5 to 99.9 *10)*/
    UINT16 u16HDOP;             /*!< Horizontal dilution of precision
                                    - Units: 0.1
                                    - Range: 5 to 999(0.5 to 99.9 *10)*/
    UINT16 u16VDOP;             /*!< Vertical dilution of precision
                                    - Units: 0.1
                                    - Range: 5 to 999(0.5 to 99.9 *10)*/
    UINT8 u8FixStatus;          /*!< Gnss fix status
                                    - 0: Gnss location not fixed
                                    - 1: Gnss location fixed*/
    UINT8 u8GnssLocateType;     /*!< Gnss location type
                                    - 1: not fixed
                                    - 2: 2D located
                                    - 3: 3D located*/
    UINT8 u8VisibleSatellites;  /*!< Visible satellites counter
                                    - Range: 0 to 24*/
    UINT8 u8UsingSatellites;    /*!< Using satellites counter
                                    - Range: 0 to 12*/
    UINT8 u8TimeStamp[12];      /*!< Gnss time stamp in UTC
                                    - YYMMDDHHMMSS time format*/
    DcpdPackageSatelliteDetail_tt ttSatellitesDetail; /*!< Satellite detail info in FmcGnssSatelliteDetail_t
                                                    struct.
                                                    - MAX 24 satellites detail*/
} DcpdPackageNmeaInfo_t;


/*!
typedef struct  \n
{   \n
    UINT8 u8SrcSrvId;   \n
    UINT8 u8DstSrvId;   \n
    UINT16 u16MsgId;    \n
} DcpdPackageHeader_t;  \n
*/
typedef struct
{
    UINT8 u8SrcSrvId;   /*!< Destinate Service Id 参考 @ref DcpdPackageAppSrvId_e*/
    UINT8 u8DstSrvId;   /*!< Destinate Service Id 参考 @ref DcpdPackageAppSrvId_e*/
    UINT16 u16EventId;    /*!< Event Id 参考 @ref DcpdPackageAppSrvId_e*/
} DcpdPackageHeader_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    DcpdPackageNmeaInfo_t   tGnssData;  \n
} DcpdPackageToMcuGnss_t;   \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    DcpdPackageNmeaInfo_t   tGnssData;  /*!< Gnss数据*/
} DcpdPackageToMcuGnss_t;


/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    CHAR   strImeiB004[24];  \n
} DcpdPackageToMcuImei_t;   \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    CHAR   strImeiB004[24];  /*!<  IMEI*/
} DcpdPackageToMcuImei_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    CHAR   strIccid[20];  \n
} DcpdPackageToMcuIccid_t;  \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    CHAR   strIccid[20];  /*!<  ICCID*/
} DcpdPackageToMcuIccid_t;


/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    CHAR   strIMSI[20];  \n
} DcpdPackageToMcuImsi_t;  \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    CHAR   strIMSI[20];  /*!<  IMSI*/
} DcpdPackageToMcuImsi_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;   \n
    CHAR   strMcc[3];  \n
    CHAR   strMnc[3];  \n
} DcpdPackageToMcuMccMnc_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    CHAR   strMcc[3];  /*!<  MCC*/
    CHAR   strMnc[3];  /*!<  MNC*/
} DcpdPackageToMcuMccMnc_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    UINT16   u16CSQ;  \n
    INT16   i16NadRssiB181;  \n
    UINT8   u8TechTypeB181;  \n
    UINT8   u8SimUsed;  \n
    UINT8   u8NadReg;  \n
} DcpdPackageToMcuNadSta2_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT16   u16CSQ;  /*!<  信号强度 CSQ，参考Part4文档*/
    INT16   i16NadRssi;  /*!<  RSSI，参考Part4文档*/
    UINT8   u8TechType;  /*!<  网络制式，参考Part4文档:
                                - 00=Not Registered
                                - 01=CDMA
                                - 02=GSM
                                - 03=CDMA2000
                                - 04=WCDMA
                                - 05=LTEFDD
                                - 06=LTETDD
                                - 07=LTE
                                - 08=Reserved.
                            */
    UINT8   u8SimUsed;  /*!<  使用的SIM卡，参考Part4文档:
                            - 00-Internal
                            - 01-External 
                        */
    UINT8   u8NadReg;  /*!<  注网信息，参考Part4文档:
                            - 00=No Service
                            - 01=Seraching
                            - 02=Registered
                            - 03=Reg SPP_FAILURE / Denied
                            - 04=NotUsed
                            - 05=RegisteredRoaming
                            - 06-FF=Reserved
                        */
} DcpdPackageToMcuNadSta2_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    CHAR   strNadIp1[16];  \n
} DcpdPackageToMcuNadIp1_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    CHAR   strNadIp1[16];  /*!<  Tbox APN1 IP1*/
} DcpdPackageToMcuNadIp1_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    CHAR   strNadIp2[16];  \n
} DcpdPackageToMcuNadIp2_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    CHAR   strNadIp2[16];  /*!<  Tbox APN2 IP2，参考Part4文档*/
} DcpdPackageToMcuNadIp2_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;        \n
    UINT8 u8Type;        \n
} DcpdPackageToMcuResetTboxReq_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 u8Type;       /*!<  u8Type:请求复位的模块:
                                            - 0: 保留
                                            - 1: 复位Modem
                                            - 2: 复位MCU
                                            - 3: 复位MPU
                                            - 4: 复位BLE
                                          */
} DcpdPackageToMcuResetTboxReq_t;


/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader; 
    UINT8 PowerMode;           \n
    UINT8 u8BattVoltOperASta;  \n
    UINT8 u8BattVoltOperCANASta;  \n
    UINT8 u8BattVoltOperDiagASta; \n
    UINT8  u8KeySta;          \n
    UINT8  u8KL15;            \n
    UINT8  u8KL30;            \n    
} DcpdPackageToMpuPmStatusUpdate_t;   \n
*/
typedef struct 
{ 
 DcpdPackageHeader_t tHeader; 
 UINT8 PowerMode;           /*!<  TBOX电源管理模式；
                                    - 0x00：预留；
                                    - 0x01：RUNNING模式；
                                    - 0x02：SLEEPL1模式；
                                    - 0x03：SLEEPL2模式；
                                    - 0x04：SLEEPL3模式；
                                    - 0x05：BACKBAT模式；
                            */
UINT8 u8BattVoltOperASta;   /*!<  普通功能电压状态；
                                    - 0x00：正常；
                                    - 0x01：低压；
                                    - 0x02：高压；
                                    - 0x03：无效；
                            */
UINT8 u8BattVoltOperCANASta;  /*!<  CAN网络通信功能电压状态；
                                    - 0x00：正常；
                                    - 0x01：低压；
                                    - 0x02：高压；
                                    - 0x03：无效；
                           */
UINT8 u8BattVoltOperDiagASta;  /*!<  A级网络诊断功能电压状态(预留)；
                                    - 0x00：正常；
                                    - 0x01：低压；
                                    - 0x02：高压；
                                    - 0x03：无效；
                           */
UINT8  u8KeySta;           /*!< 点火钥匙状态：
                                    - 0x00：OFF
                                    - 0x01：ACC
                                    - 0x02：RUN
                                    - 0x03：CRANK
                            */
UINT8  u8KL15;           /*!< KL15状态：
                                    - 0x00：OFF
                                    - 0x01：ON
                            */
UINT8  u8KL30;           /*!< KL30状态：
                                    - 0x00：OFF
                                    - 0x01：ON
                            */

} DcpdPackageToMpuPmStatusUpdate_t;


/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8ExtFailReason;   \n
    UINT32 u32CanID;               \n
    UINT16 u16CanRespMsgLen;       \n
    UINT8 u8ArrDiagRspMsg[255];    \n
} DcpdPackageToMpuRDiagResp_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT8 u8ExtFailReason;    /*!< 除诊断负反馈之外的其它失败原因:
                                - 0x00  无失败原因
                                - 0x01  外部诊断工具连接
                                - 0x02  发动机启动
                                - 0x03  点火钥匙处在Crank
                                - 0x04  车速大于4KM/H
                                - 0x05  功能电压异常
                              */
    UINT8  u8Num;                /*!< 当前包*/
    UINT8  u8TotalNum;           /*!< 总包数*/
    UINT32 u32CanID;                /*!< 诊断的CAN ID    */
    UINT16 u16CanRespMsgLen;        /*!< 响应的诊断报文有效长度          */
    UINT16 u16PayloadLen;           /*!< 当前包长度*/
    UINT8 u8ArrDiagRspMsg[255];     /*!< 响应的诊断报文，不足部分填0x00*/

} DcpdPackageToMpuRDiagResp_t;

/*!
typedef struct \n
{ \n 
    DcpdPackageHeader_t tHeader;  \n
    UINT16 u16Cmd;             \n
    UINT8 u8ArrPara[200];     \n
} DcpdPackageToMpuEolCmdReq_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT16 u16Cmd;              /*!< 指令 :
                                    - 0 查询MPU是否收到Sleep信号
                                    - 1 查询MPU是否收到Wakeup信号
                                    - 2 请求MPU发出唤醒MCU信号
                                    - 3 请求MPU打开音频回环
                                    - 4 请求MPU关闭音频回环
                                    - 5 请求MPU查询SIM卡状态
                                    - 6 查询Tbox的5码信息（IMEI+IMSI+MEID+ICCID+SIM卡电话号码）
                                    - 7 进入飞行模式
                                    - 8 退出飞行模式
                                */
    UINT8 u8ArrPara[200];     /*!< 指令对应的参数：
                                    - CMD=0 NA
                                    - CMD=1 NA
                                    - CMD=2 NA
                                    - CMD=3 NA
                                    - CMD=4 NA
                                    - CMD=5 NA
                                    - CMD=6 NA
                                    - CMD=7 NA
                                    - CMD=8 NA
                              */
} DcpdPackageToMpuEolCmdReq_t;



/*! 
typedef struct \n
{\n
    DcpdPackageHeader_t tHeader;  \n
    UINT32 u32McuSysTime;         \n
} DcpdPackageToMpuSysTimeOfMcu_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT32 u32McuSysTime;         /*!< 0xFFFFFFFF表示MCU系统时间无效            */
} DcpdPackageToMpuSysTimeOfMcu_t;

/*! 
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT32 u32EventID;         \n
} DcpdPackageToMpuRxRvcOtaAck_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT32 u32EventID;         /*!< u32EventID，事件ID，预留*/
} DcpdPackageToMpuRxRvcOtaAck_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;   \n
    UINT8 u8PowerMode;                 \n
} DcpdPackageToMcuSleepReady_t;   \n
*/
typedef struct 
{ 
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT8 u8Result;                 /*!<    MPU是否可以休眠；
                                            - 0x00：休眠条件不满足，不可以休眠；
                                            - 0x01：休眠条件满足，可以休眠；
                                    */
} DcpdPackageToMcuSleepReady_t;


/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;   \n
    UINT8 u8PowerMode;                 \n
} DcpdPackageToMcuPmChangeEnd_t;   \n
*/
typedef struct 
{ 
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT8 u8PowerMode;             /*!<  TBOX电源管理模式；
                                    - 0x00：预留；
                                    - 0x01：RUNNING模式；
                                    - 0x02：SLEEP1模式；
                                    - 0x03：SLEEP2模式；
                                    - 0x04：SLEEP3模式；
                                    - 0x05：BATBACK模式；
                                    */
} DcpdPackageToMcuPmChangeEnd_t;


/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;   \n
    UINT8 u8SimSta;       \n
    UINT8 u8SimFltSta;    \n
    UINT32 u32SimExtInfo;  \n
} DcpdPackageToMcuSimSta_t;  \n
*/
typedef struct 
{ 
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT8 u8SimSta;       /*!<  SIM卡状态 :
                                - 0x00：SIM卡存在
                                - 0x01：SIM卡不存在
                         */
    UINT8 u8SimFltSta;    /*!<  SIM卡故障状态：
                                - 0x00：SIM卡无故障
                                - 0x01：SIM卡有故障
                          */
    UINT32 u32SimExtInfo;    /*!<  SIM卡信息：此字段目前填0，预留*/
} DcpdPackageToMcuSimSta_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;   \n
    UINT8 u8ReqSrc;             \n
    UINT32 u32CanID;            \n
    UINT16 u16DiagMsgLen;       \n
    UINT8 u8ArrDiagMsg[255];    \n
} DcpdPackageToMcuRDiagReq_t;   \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT8 u8ReqSrc;              /*!< 请求来源:
                                    - 0x00: FOTA(FOTA请求时不做诊断条件判断)
                                    - 0x01: Remote Diag
                                 */
    UINT8  u8Num;                /*!< 当前包*/
    UINT8  u8TotalNum;           /*!< 总包数*/
    UINT32 u32CanID;             /*!< 诊断CAN ID */
    UINT16 u16DiagMsgLen;        /*!< 诊断报文长度          */
    UINT16 u16PayloadLen;        /*!< 本包长度          */
    UINT8 u8ArrDiagMsg[255];     /*!< 诊断报文，不足部分填0x00*/
} DcpdPackageToMcuRDiagReq_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader; \n
    UINT16 u16Cmd;             \n
    UINT8 u8ArrPara[200];     \n
} DcpdPackageToMcuEolCmdResp_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT16 u16Cmd;              /*!< 指令 :
                                    - 0 查询MPU是否收到Sleep信号
                                    - 1 查询MPU是否收到Wakeup信号
                                    - 2 请求MPU发出唤醒MCU信号
                                    - 3 请求MPU打开音频回环
                                    - 4 请求MPU关闭音频回环
                                    - 5 请求MPU查询SIM卡状态
                                    - 6 查询Tbox的5码信息（ICCID+IMEI+IMSI+MEID+SIM卡电话号码）
                                    - 7 进入飞行模式
                                    - 8 退出飞行模式
                                */
    UINT8 u8ArrPara[200];     /*!< 指令对应的参数：
                                    - CMD=0 参考 @ref DcpdPackageToMcuEolSleepSigResp_t
                                    - CMD=1 参考 @ref DcpdPackageToMcuEolWakeupSigResp_t
                                    - CMD=2 参考 @ref DcpdPackageToMcuEolWakeupMcuResp_t
                                    - CMD=3 参考 @ref DcpdPackageToMcuEolOpenAudioLpResp_t
                                    - CMD=4 参考 @ref DcpdPackageToMcuEolCloseAudioLpResp_t
                                    - CMD=5 参考 @ref DcpdPackageToMcuEolSimStaResp_t
                                    - CMD=6 参考 @ref DcpdPackageToMcuEol5CodeResp_t
                                    - CMD=7 参考 @ref DcpdPackageToMcuEolEnterFlightModeResp_t
                                    - CMD=8 参考 @ref DcpdPackageToMcuEolExitFlightModeResp_t
                              */
} DcpdPackageToMcuEolCmdResp_t;

/*! 
typedef struct \n
{\n
    DcpdPackageHeader_t tHeader;  \n
    UINT32 u32MpuSysTime;         \n
} DcpdPackageToMcuSysTimeOfMpu_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< Dcpd Package Header*/
    UINT32 u32MpuSysTime;         /*!< 0xFFFFFFFF表示MPU系统时间无效            */
} DcpdPackageToMcuSysTimeOfMpu_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;    \n
    UINT32 u32ReqId;  \n
} DcpdPackageToMcuRvcOtaTxAck_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT32 u32ReqId;  /*!<  u32ReqId，请求ID，RVC据此确认所发送的消失是否已送达*/
} DcpdPackageToMcuRvcOtaTxAck_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    BOOL bIsSms;                    \n
    UINT8 u8TotalNum;               \n
    UINT8 u8Num;                    \n
    UINT8 u8ArrPara[256];           \n
} DcpdPackageToMcuRvcOtaMsgRx_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT16 u16SerNum;               /*!< 流水号*/
    BOOL bIsSms;                    /*!<  TRUE:通过SMS接收；FALSE:通过Socket接收*/
    UINT8 u8TotalNum;               /*!< 请求车控的参数总个数*/
    UINT8 u8Num;                    /*!< 本次传输的参数个数*/
    UINT8 u8ArrPara[256];           /*!< DID1+DID1Para+……+DIDn+DIDnPara*/
} DcpdPackageToMcuRvcOtaMsgRx_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    UINT32 u32DcpdReqId;            \n
    BOOL bIsSms;  \n
    UINT8 u8Num;                \n
    UINT8 u8ArrResult[256];     \n
} DcpdPackageToMpuRvcOtaMsgTx_t; \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT16 u16SerNum;               /*!< 流水号*/
    UINT32 u32DcpdReqId;            /*!< 请求ID，MPU返回发送结果时，使用这个ID来标识*/
    BOOL bIsSms;                /*!<  TRUE:请求使用SMS发送*/
    UINT8 u8Num;                /*!<  请求参数个数*/
    UINT8 u8ArrResult[256];     /*!<  DID1+DID1Result+……+DIDn+DIDnResult*/
} DcpdPackageToMpuRvcOtaMsgTx_t;


/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    UINT32 u32ReqId;  \n
    UINT8 u8Result;  \n
} DcpdPackageToMcuOtaTxResult_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT32 u32ReqId;  /*!<  发送请求的ID*/
    UINT8 u8Result;  /*!<  发送结果:
                        -  0:发送成功
                        -  其它：发送失败
                     */
} DcpdPackageToMcuOtaTxResult_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    UINT8 u8FixStatus;  \n
    INT32 i32Longitude; \n
    INT32 i32Latitude;  \n
    UINT8 u8TimeStamp[12];  \n
} DcpdPackageEolGnssInfo_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 u8FixStatus;  /*!< 定位状态 :
                                -  0:未定位
                                -  1:定位成功
                        */
    INT32 i32Longitude; /*!< 纬度;   0.000001 Degrees ; -180000000 to 180000000;正为东经；负为西经*/
    INT32 i32Latitude;  /*!< 经度;  0.000001 Degrees ; -90000000 to 90000000; 正为北纬；负为南纬 */
    UINT8 u8TimeStamp[12];  /*!< 时间: YYMMDDHHMMSS */
} DcpdPackageEolGnssInfo_t;


/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    UINT8 u8WanConnectState;    \n
    UINT8 u8PublicIp[16];   \n
    UINT8 u8Csq;            \n
} DcpdPackageEolWanConnInfo_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 u8WanConnectState;    /*!< Wan连接状态:
                                     -  1:connecting
                                     -  2:2G
                                     -  3:3G
                                     -  4:4G
                                */
    UINT8 u8PublicIp[16];   /*!< 公网IP*/
    UINT8 u8Csq;            /*!< CSQ值*/
} DcpdPackageEolWanConnInfo_t;


/*!
typedef struct  \n
{   \n
        DcpdPackageHeader_t tHeader;    \n
        UINT8 tDataBuff[8]; \n
} DcpdPackageEolUartInfo_t;    \n
*/
typedef struct
{
        DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
        UINT8 tDataBuff[8]; /*!< 8个字节的Buffer MPU收到后原样返回*/
} DcpdPackageEolUartInfo_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n
    UINT8 u8State;                  \n
} DcpdPackageEolPingInfo_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 u8State;                  /*!<  PING网结果:
                                        -  1:成功
                                        -  其它：失败
                                    */
} DcpdPackageEolPingInfo_t;

/*!
typedef struct \n
{ \n
    UINT8  u8Sta;           \n
}DcpdPackageToMcuEolSleepSigResp_t;    \n
*/
typedef struct
{
    UINT8  u8Sta;           /*!< MPU是否收到Sleep信号标志:
                                - 0 没有收到Sleep信号
                                - 1 收到Sleep信息
                            */
}DcpdPackageToMcuEolSleepSigResp_t;

/*!
typedef struct \n
{\n
    UINT8  u8Sta;           \n
}DcpdPackageToMcuEolWakeupSigResp_t;    \n
*/
typedef struct
{
    UINT8  u8Sta;           /*!< MPU是否收到Wakeup信号标志:
                                - 0 没有收到Wakeup信号
                                - 1 收到Wakeup信息
                            */
}DcpdPackageToMcuEolWakeupSigResp_t;

/*!
typedef struct \n
{ \n
    UINT8  u8Sta;           \n
}DcpdPackageToMcuEolWakeupMcuResp_t;    \n
*/
typedef struct
{
    UINT8  u8Sta;           /*!< MPU是否发出唤醒MCU信号:
                                - 0 没有发出
                                - 1 已发出
                            */
}DcpdPackageToMcuEolWakeupMcuResp_t;

/*!
typedef struct \n
{ \n 
    UINT8  u8Sta;           \n
}DcpdPackageToMcuEolOpenAudioLpResp_t;    \n
*/
typedef struct
{
    UINT8  u8Sta;           /*!< MPU是否打开音频回环:
                                - 0 打开失败
                                - 1 打开成功
                            */
}DcpdPackageToMcuEolOpenAudioLpResp_t;

/*!
typedef struct \n
{ \n
    UINT8  u8Sta;           \n
}DcpdPackageToMcuEolCloseAudioLpResp_t;    \n
*/
typedef struct
{
    UINT8  u8Sta;           /*!< MPU是否关闭音频回环:
                                - 0 关闭失败
                                - 1 关闭成功
                            */
}DcpdPackageToMcuEolCloseAudioLpResp_t;

/*!
typedef struct \n
{ \n
    UINT8  u8Sta;           \n
}DcpdPackageToMcuEolSimStaResp_t;    \n
*/
typedef struct
{
    UINT8  u8Sta;           /*!< SIM卡状态:
                                - 0 SIM卡已连接，正常
                                - 1 SIM卡已连接，但是状态异常
                                - 2 SIM卡未连接
                            */
}DcpdPackageToMcuEolSimStaResp_t;

/*!
typedef struct \n
{ \n
    UINT8  u8IccidLen;          \n
    UINT8 u8ArrIccid[21];       \n
    UINT8  u8ImeiLen;           \n
    UINT8 u8ArrImei[33];        \n
    UINT8  u8ImsiLen;           \n
    UINT8 u8ArrImsi[33];        \n
    UINT8  u8MeidLen;           \n
    UINT8 u8ArrMeid[33];        \n
    UINT8  u8SimPhNoLen;        \n
    UINT8 u8ArrSimPhNo[33];     \n
}DcpdPackageToMcuEol5CodeResp_t;    \n
*/
typedef struct
{
    UINT8  u8IccidLen;           /*!< ICCID有效长度     */
    UINT8 u8ArrIccid[21];        /*!< ICCID，不足填充0x00     */
    UINT8  u8ImeiLen;           /*!< ICCID有效长度     */
    UINT8 u8ArrImei[33];        /*!< IMEI，不足填充0x00     */
    UINT8  u8ImsiLen;           /*!< IMSI有效长度     */
    UINT8 u8ArrImsi[33];        /*!< IMSI，不足填充0x00     */
    UINT8  u8MeidLen;           /*!< MEID有效长度     */
    UINT8 u8ArrMeid[33];        /*!< MEID，不足填充0x00     */
    UINT8  u8SimPhNoLen;           /*!< 电话号码有效长度     */
    UINT8 u8ArrSimPhNo[33];        /*!< 电话号码，不足填充0x00     */
}DcpdPackageToMcuEol5CodeResp_t;


/*!
typedef struct \n
{ \n
    UINT8  u8Result;           \n
}DcpdPackageToMcuEolEnterFlightModeResp_t;    \n
*/
typedef struct
{
    UINT8  u8Result;           /*!< 进入飞行模式的结果：
                                    - 0    成功;  
                                    - 其它 失败码     */
}DcpdPackageToMcuEolEnterFlightModeResp_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader; \n
    UINT8 u8DidNum;             \n
    UINT8 u8ArrDid[512];         \n
}DcpdPackageToMcuCfgSvReq_t;       \n
*/
typedef struct {
    DcpdPackageHeader_t tHeader;    //!n.a
    UINT8 u8DidNum;                 /*!< 本次DID的个数n     */
    UINT8 u8ArrDid[512];            /*!< DID1+DIDData+……+DIDn+DIDDatan*/
} DcpdPackageToMcuCfgSvReq_t;

/*!
typedef struct \n
{ \n
    UINT8  u8Result;           \n
}DcpdPackageToMcuEolExitCFunResp_t;    \n
*/
typedef struct
{
    UINT8  u8Result;           /*!< 退出飞行模式的结果：
                                - 0    成功;
                                - 其它 失败码     */
}DcpdPackageToMcuEolExitFlightModeResp_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;           \n
    UINT8 strLogBuf[512];             \n
}DcpdPackageToMpuMcuLog_t;    \n
*/
typedef struct {
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 strLogBuf[512];   /*!< buffer */
} DcpdPackageToMpuMcuLog_t;


/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader; \n
    UINT8 u8DidTotalNum;         \n
    UINT8 u8DidNum;;             \n
    UINT8 u8ArrDid[512];         \n
}DcpdPackageToMpuIniCfg_t;       \n
*/
typedef struct {
    DcpdPackageHeader_t tHeader;    //!n.a
    UINT8 u8DidTotalNum;            /*!< DID的总个数n     */
    UINT8 u8DidNum;                 /*!< 本次DID的个数n     */
    UINT8 u8ArrDid[512];            /*!< DID1+DIDData+……+DIDn+DIDDatan*/
} DcpdPackageToMpuIniCfg_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;     \n
    UINT8 u8DidNum;                 \n
    UINT8 u8ArrDid[512];            \n
}DcpdPackageToMpuCfgUpdateInd_t;    \n
*/
typedef struct 
{ 
    DcpdPackageHeader_t tHeader;     
    UINT8 u8DidNum;                 /*!< 本次DID的个数n     */
    UINT8 u8ArrDid[512];            /*!< DID1+DIDData+……+DIDn+DIDDatan*/
}DcpdPackageToMpuCfgUpdateInd_t;  


/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;     \n
    UINT8 u8ReqType;                 \n
}DcpdPackageToMpuWifiReq_t;    \n
*/
typedef struct 
{ 
    DcpdPackageHeader_t tHeader;     
    UINT8 u8ReqType;                 /*!< 请求类型：
                                        - 0 关闭WIFI
                                        - 1 开启WIFI
                                        - 2 请求Socket码
                                        - 3 请求Psw/SSID
                                    */
    UINT8 u8SrcType;                
            /*!< wifi请求来源:
              - 0 DVR请求
              - 1 MP5请求
            */
}DcpdPackageToMpuWifiReq_t; 


/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8  u8Result;           \n
    UINT8 u8ArrResult[128];    \n
}DcpdPackageToMpuCfgSvResp_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8DidNum;                 /*!< 本次DID的个数n     */
    UINT8 u8ArrResult[128];           /*!< DID1+DID1Result+……+DIDn+DIDResult*/
}DcpdPackageToMpuCfgSvResp_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8KL15;                  \n
    UINT32 u32DID6009;             \n
    UINT8 u8DID6001;               \n
    UINT8 u8DID6002;               \n
    UINT8 u8DID6003;               \n
    UINT8 u8DID6004;               \n
    UINT32 u32DID6005;             \n
}DcpdPackageToMpuPreRdResp_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8KL15;                   /*!< KL15状态:
                                            - 0    OFF;
                                            - 1    ON   
                                    */
    UINT32 u32DID6009;              /*!< DID_0x6009 */
    UINT8 u8DID6001;                /*!< DID_0x6001 */
    UINT8 u8DID6002;                /*!< DID_0x6002 */
    UINT8 u8DID6003;                /*!< DID_0x6003 */
    UINT8 u8DID6004;                /*!< DID_0x6004 */
    UINT32 u32DID6005;              /*!< DID_0x6005 */
}DcpdPackageToMpuPreRdResp_t;


/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8KL15;                  \n
    UINT32 u32Vbat;                \n
    UINT8 u8EpbSt;                 \n
    UINT16 u16EngSpd;              \n
    UINT8 u8SecSt;                 \n
}DcpdPackageToMpuPreFotaResp_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8KL15;                   /*!< KL15状态:
                                            - 0    OFF;
                                            - 1    ON   
                                    */
    UINT32 u32Vbat;                 /*!< 电池电压(单位: 0.1v)     */
    UINT8 u8EpbSt;                  /*!< 手刹状态 */
    UINT16 u16EngSpd;               /*!< 发动机转速(1rpm)*/
    UINT8 u8SecSt;                  /*!< 设防状态*/
	UINT8 u8Result;                 /*!< 判断结果：bit0:bit4置1分别表示对应不满足条件              */
}DcpdPackageToMpuPreFotaResp_t;



/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8Result;                  \n
}DcpdPackageToMpuDefenceSvResp_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8Result;                   /*!< 保存结果:
                                         - 0    失败;
                                         - 1    成功   
                                    */
}DcpdPackageToMpuDefenceSvResp_t;

/*!
typedef struct \n
{ \n 
    DcpdPackageHeader_t tHeader;    \n
    UINT8  u8Result;           \n
    UINT8 u8AuthCodeLen;       \n
    UINT8 u8AuthCode[16];      \n
    UINT8 u8WorkMd;            \n
}DcpdPackageToMcuTboxRegResult_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8  u8Result;           /*!< 终端注册结果:
                                - 0 打开失败
                                - 1 打开成功
                            */
    UINT8 u8AuthCodeLen;        /*!<鉴权码有效长度*/
    UINT8 u8AuthCode[16];       /*!<鉴权码，最长16字节，不足部分填充0x00*/
    UINT8 u8WorkMd;             /*!<工作模式 0：测试期；1：正式期*/
}DcpdPackageToMcuTboxRegResult_t;


/*!
typedef struct \n
{ \n 
    DcpdPackageHeader_t tHeader;    \n
    UINT8  u8Type;           \n
    UINT8 u8OpResult;       \n
    UINT8 u8SocketCode[8];      \n
}DcpdPackageToMcuWifiIndResp_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8  u8Type;           /*!< 类型:
                                - 0 关闭WIFI的结果
                                - 1 打开WIFI的结果
                                - 2 SOCKET码请求结果
                                - 3 请求ssid/psw
                            */
    UINT8 u8OpResult;        /*!<WIFI开关操作结果
                                - TRUE 成功
                                - FALSE 失败
                            */
    UINT8 u8SocketCode[8];   /*!<u8Type为2时有效*/

	UINT8 u8WifiSSID[16];    /*Wifi名称*/
	
	UINT8 u8WifiPsw[16];	 /*Wifi密码*/
}DcpdPackageToMcuWifiIndResp_t;

/*!
typedef struct \n
{ \n 
    DcpdPackageHeader_t tHeader;    \n
    UINT8  u8Type;          \n
    UINT8 u8Attr[34];       \n
}DcpdPackageToMcuDefenceSvReq_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8  u8Type;           /*!< 防护圈类型:
                                - 0 圆形
                                - 1 矩形
                            */
    UINT8 u8Attr[34];       /*!<防护圈参数，根据u8Type解析，不足部分填充0x00*/
}DcpdPackageToMcuDefenceSvReq_t;

/*!
typedef struct \n
{ \n 
    DcpdPackageHeader_t tHeader;    \n
    DcpdPackageVehReportType_e eReportType;   \n
    UINT8  UINT8 u8ArrTrig[8];   \n
    UINT8 u8DidNum;             \n
    UINT16 u16DataLen;          \n
    UINT8 u8DidData[512];       \n
}DcpdPackageToMpuVehReport_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 eReportType;  /*!< report type*/
    UINT8 u8ArrTrig[8];   /*!< 触发源:
                                    - bit0-bit63对应 "表 3.6.29.3 1 车辆上报触发表"  */
    UINT8 u8DidNum;             /*!<DID总数*/
    UINT16 u16DataLen;          /*!<DID Data Len*/
    UINT8 u8DidData[512];       /*!<DID+DIDData……，不足部分填充0x00*/
}DcpdPackageToMpuVehReport_t;


/*!
typedef struct \n
{ \n 
    DcpdPackageHeader_t tHeader;    \n
    UINT8  UINT8 u8ArrDID603D[8];   \n
}DcpdPackageToMpuWinAutoClose_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 u8ArrDID603D[8];   /*!< 关窗等信息，参考“江淮汽车TLV键值对表格”  */
}DcpdPackageToMpuWinAutoClose_t;


/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8Sta;                  \n
}DcpdPackageToMcuEmmcSta_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8EmmcSta;                   /*!< Emmc状态:
                                         - 0    Emmc存在且无故障;
                                         - 1    Emmc存在且有故障
                                         - 2    Emmc不存在
                                    */
}DcpdPackageToMcuEmmcSta_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8Sta;                  \n
}DcpdPackageToMcuWifiSta_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8WifiSta;                   /*!< Wifi状态:
                                         - 0    连接;
                                         - 1    未连接;   
                                    */
}DcpdPackageToMcuWifiSta_t;

    
/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8Sta;                  \n
}DcpdPackageToMcuTspConSta_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8TspSta;                   /*!< Tsp连接状态:
                                         - 0    连接;
                                         - 1    未连接   
                                    */
}DcpdPackageToMcuTspConSta_t;
    

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8Len;                  \n
    UINT8 u8PhNumArry[32];                  \n
}DcpdPackageToMcuPhnumResp_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8Len;                   /*!< 电话号码长度 */
    UINT8 u8PhNumArry[32];           /*!< 电话号码信息 */
}DcpdPackageToMcuPhnumResp_t;


/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n
    UINT8 u8Type;                  \n
}DcpdPackageToMpuWifiInd_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8Type;                   /*!< wifi请求事件:
                                      - 0 请求关闭WIFI
                                      - 1 请求打开WIFI
                                      - 2 请求socket码
                                      - 3 请求SSID、Psw
                                    */
    UINT8 u8SrcType;                /*!< wifi请求来源:
                                      - 0 DVR请求
                                      - 1 MP5请求
                                    */
}DcpdPackageToMpuWifiInd_t;

/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;  \n   
	UINT16 u16Spd;         \n
	UINT8 u8AirPre;        \n
	UINT8 u8EngTorque;     \n 
	UINT8 u8FrictionTor;  \n
	UINT16 u16EngSpd;      \n
	UINT16 u16EngFuelFlow; \n
	UINT16 u16NOxSensorUp; \n
    UINT16 u16NOxSensorDown;    \n
    UINT8  u8Reagentresidual;   \n
    UINT16 u16AirInflow;        \n
    UINT16 u16SCRTmpIN;         \n
    UINT16 u16SCRTmpOUT;        \n
    UINT16 u16DPF;              \n
    UINT8 u8EngCoolantTmp;      \n
    UINT8 u8FuelLevel;          \n
    UINT32 u32TotalMileage;     \n
}DcpdPackageToMpuEngData_t;    \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/   
	UINT16 u16Spd;         /*!< 车速  */
	UINT8 u8AirPre;        /*!< 大气压力   */
	UINT8 u8EngTorque;     /*!< 发动机净输出扭矩   */  
	UINT8 u8FrictionTor;  /*!< 摩擦扭矩   */
	UINT16 u16EngSpd;      /*!< 发动机转速   */
	UINT16 u16EngFuelFlow; /*!< 发动机燃料流量   */
	UINT16 u16NOxSensorUp; /*!< SCR上游Nox传感器输出值  */
    UINT16 u16NOxSensorDown;    /*!< SCR下游Nox传感器输出值  */
    UINT8  u8Reagentresidual;   /*!< 反应剂余量   */
    UINT16 u16AirInflow;        /*!< 进气量  */
    UINT16 u16SCRTmpIN;         /*!< SCR入口温度    */
    UINT16 u16SCRTmpOUT;        /*!< SCR出口温度 */
    UINT16 u16DPF;              /*!< DPF压差  */
    UINT8 u8EngCoolantTmp;      /*!< 发动机冷却液温度  */
    UINT8 u8FuelLevel;          /*!< 油箱液位*/
    UINT32 u32TotalMileage;     /*!< 累计里程*/
}DcpdPackageToMpuEngData_t;

/*!
typedef struct  \n
{  \n
    DcpdPackageHeader_t tHeader;    \n  
    UINT8 u8HarWaredNumLen;     \n
    UINT8 pu8HarWaredNum[15];   \n
    UINT8 u8SolftWaredNumLen;    \n
    UINT8 pu8SoftWareNum[15];   \n
    UINT8 u8TBOXIDLen;        \n
    UINT8 pu8TBOXID[20];      \n
    UINT8 u8FMSta;            \n
    UINT8 u8HMIType;           \n
    UINT32  u32VehDistance;     \n
    UINT8 u8ProtocalType;       \n
    UINT8   u8ReCalibEnable;    \n
    UINT8  pu8ReCalibECMCode[10];   \n
    UINT8  pu8RmCalibECMMake[5];    \n
    UINT8  pu8RmCalibECMVIN[17];    \n
    UINT8  pu8RmCalibECMSn[8];      \n   
}DcpdPacketKMSTelemticInfo_t;   \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/   
    UINT8 u8HarWaredNumLen;     /*!< 硬件号长度*/
    UINT8 pu8HarWaredNum[15];   /*!< 硬件号*/
    UINT8 u8SolftWaredNumLen;    /*!< 软件号长度*/
    UINT8 pu8SoftWareNum[15];   /*!< 软件号*/
    UINT8 u8TBOXIDLen;         /*!< 终端盒子ID长度*/
    UINT8 pu8TBOXID[26];        /*!< 终端盒子ID*/
    UINT8 u8FMSta;               /*!< 故障状态*/
    UINT8 u8HMIType;            /*!<HMI类型*/
    UINT32  u32VehDistance;     /*!< 车辆总里程*/
    UINT8 u8ProtocalType;       /*!< 数据通信协议，默认0*/
    UINT8   u8ReCalibEnable;/*!< 发动机远程标定功能启动控制开关*/
    UINT8  pu8ReCalibECMCode[10];/*!< 发动机远程标定 ECM Code*/
    UINT8  pu8RmCalibECMMake[5];/*!< 发动机远程标定 ECM Make*/
    UINT8  pu8RmCalibECMVIN[17];/*!< 发动机远程标定 ECM VIN*/
    UINT8  pu8RmCalibECMSn[8];/*!< 发动机远程标定 ECM SN*/
	UINT32 u32ECMSourceAddress; /*ECM源地址型*/
}DcpdPacketKMSTelemticInfo_t;

/*!
typedef struct  \n
{   \n
    DcpdPackageHeader_t tHeader;    \n   
    UINT8 pu8FMinFor[183]; \n
    UINT8 pu8HBNotSample[800]; \n
}DcpdPackageKmsHBData_t;   \n
*/
typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/   
    UINT8 pu8FMinFor[183]   ; /*!< 故障信息(按照0x604C读取) 
                                - 消息ID 0x604C (16BIT)
                                - 消息数据长WORD(16BIT)
                                - 发动机远程标定 ECM VIN(8BIT * 17)
                                - 发动机远程标定 ECM SN(8BIT * 8)
                                - 故障节点个数(8BIT)
                                - 故障节点1源地址(8BIT)
                                - 故障节点1故障码属性(8BIT)
                                - 故障节点1故障灯信息(16BIT)
                                - 故障节点1故障码个数(8BIT)
                                
                                - 故障节点1故障码1 SPN(24BIT)
                                - 故障节点1故障码1 FMI (8BIT)
                                - 故障节点1故障码1 OC&CM (8BIT)
                                - 故障节点1故障码1 属性(8BIT)
                                - 故障节点1故障码2...
                                */
    UINT8 pu8HBNotSample[800]; /*!< 康明斯心跳数据
                                - 参数个数(16BIT)
                                - 参数1 SPNID(32BIT)
                                - 参数1 SPN数据长度(8BIT)
                                - 参数1 SPN数据(N)
                                - 参数N SPNID
                                - 参数N SPN数据长度
                                - 参数N SPN数据*/ 

}DcpdPackageKmsHBData_t;

/*!
typedef struct   \n
{   \n
	DcpdPackageHeader_t tHeader;  \n 
	UINT8 u8McuChkResult;   \n
	UINT16 u16Vspd;     \n
	UINT8 u8ShiftLvr;	\n
	UINT8 u8EngRun;     \n
	UINT8 u8MotCmddOn;	\n
	UINT8 u8ExtDiagToolSta;     \n
	UINT8 u8BattVoltOperASta;	\n
	UINT8 u8DTCD50062;	\n
	UINT8 u8KeySta; \n
	UINT16 u16SOC;	\n
} DcpdPackageToMpuFotaCondChkResp_t;  \n
*/
typedef struct
{
	DcpdPackageHeader_t tHeader;	//!n.a
	UINT8 u8McuChkResult;	//!n.a
	UINT16 u16Vspd; //!n.a
	UINT8 u8ShiftLvr;	//!n.a
	UINT8 u8EngRun; //!n.a
	UINT8 u8MotCmddOn;	//!n.a
	UINT8 u8ExtDiagToolSta; //!n.a
	UINT8 u8BattVoltOperASta;	//!n.a
	UINT8 u8DTCD50062;	//!n.a
	UINT8 u8KeySta; //!n.a
	UINT16 u16SOC;	//!n.a
} DcpdPackageToMpuFotaCondChkResp_t;

/*!
typedef struct \n
{  \n
	DcpdPackageHeader_t tHeader;    \n
	UINT8 u8Result; \n
} DcpdPackageToMpuFblRes_t;   \n
*/
typedef struct 
{
	DcpdPackageHeader_t tHeader;	//!n.a
	UINT8 u8Result; //!n.a
} DcpdPackageToMpuFblRes_t;


/*!
typedef struct \n
{ \n
    DcpdPackageHeader_t tHeader;    \n
    SPP_UInt8 u8Apn16Sta;   \n
    SPP_UInt8 u8WkpCnt1;    \n
    SPP_UInt8 u8WkpCnt2;    \n
} DcpdPackageToMcuApnSta_t; \n
*/
typedef struct 
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8Apn16Sta;   /*!< 私网1的连接状态 */
    UINT8 u8WkpCnt1;    /*!< 30min内的唤醒次数 */
    UINT8 u8WkpCnt2;    /*!< 总的唤醒次数 */
} DcpdPackageToMcuApnSta_t;

/*!
typedef struct \n
{  \n
    DcpdPackageHeader_t tHeader;  \n  
    UINT8 au8TspApnAddr[32];   \n
    UINT8 au8TspApnUsr[24];    \n
    UINT8 au8TspApnPsw[24];    \n
    UINT8 au8Mp5ApnAddr[32];   \n
    UINT8 au8Mp5ApnUsr[24];    \n
    UINT8 au8Mp5ApnPsw[24];    \n
    UINT8 au8OtaApnAddr[32];   \n
    UINT8 au8OtaApnUsr[24];    \n
    UINT8 au8OtaApnPsw[24];    \n
    UINT8 au8TspIp[32];        \n
    UINT8 au8OtaIp[32];     \n
} DcpdPackageToMpuApnCfg_t; \n
*/
    
typedef struct 
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 au8TspApnAddr[32];        /*!< Tsp服务器APN信息*/
    UINT8 au8TspApnUsr[24];         /*!< Tsp服务器APN用户名*/
    UINT8 au8TspApnPsw[24];         /*!< Tsp服务器APN密码*/
    UINT8 au8Mp5ApnAddr[32];        /*!< MP5主机上网APN信息*/
    UINT8 au8Mp5ApnUsr[24];         /*!< MP5主机上网APN用户名*/
    UINT8 au8Mp5ApnPsw[24];         /*!< MP5主机上网APN密码*/
    UINT8 au8OtaApnAddr[32];        /*!< OTA上网APN信息*/
    UINT8 au8OtaApnUsr[24];         /*!< OTA上网APN用户名*/
    UINT8 au8OtaApnPsw[24];         /*!< OTA上网APN密码*/
    UINT8 au8TspIp[32];             /*!< Tsp服务器地址*/
    UINT8 au8OtaIp[32];             /*!< Ota服务器地址*/
} DcpdPackageToMpuApnCfg_t;

/*!
typedef struct  \n
{  \n
    DcpdPackageHeader_t tHeader;    \n
    UINT8 u8ReportType;             \n
} DcpdPackageToMcuVehReportAck_t;  \n
*/
typedef struct 
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8ReportType;             /*!< 上报类型 */
} DcpdPackageToMcuVehReportAck_t;



typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 u8SignalNum;            /*!< signal num*/      
    UINT8 u8Channel;          /*!<CAN Channel */
    UINT8 u8FrameIndex;         /*!< CANFrame Index*/
    UINT8 u8TranType;        /*!< transmit type 
                                - 0 : cycle
                                - 1 : spontaneous*/  
    UINT8 pu8SignalData[50];      /*!< SignalIndex[one byte] SignalLen[one byte] SingnalData[n bytes] ...*/
}DcpdPackageCANSignalTransmit_t;



typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */
    UINT16 u16VehSpeed;  /*!< 车速*/
    BOOL bVehGearStatus; /*!< 档位状态*/
    BOOL bVehEpbStatus; /*!< 电子手刹状态*/
    BOOL bVehBatteryStatus;/*!< 电池状态*（新能源）*/
    UINT8 u8VehBatteryLevel; /*!< 电池组电量百分比（新能源）*/
    UINT8 u8VehAccBatteryLevel;/*!< 蓄电池电量百分比*/
    UINT8 u8VehAccVoltage;/*!< 电池电压*/
    BOOL bEngRunSta;/*!< 发动机运行状态*/
    UINT8 u8KeySta; /*!< 钥匙状态*/
	UINT8 u8SecSta; /*!< 设防状态*/
    UINT32 u32VehConDaValid;  /*!< 信号有效性*/    
}DcpdPackageToMpuVehConData_t;


typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */  
    UINT8 u8CtrType;                 /*!< 控制类型
                                        -0：下电
                                        -1：上电*/
    UINT8 u8CtrRst;          			/*!<切电控制结果
                                       -0：控制成功
                                       -非0：控制失败码（RVC定义）
             							0x1:车辆不在OFF电或车辆未设防
										0x2:ESCL解锁失败
										0x3:PEPS未配置
										0x7:其他*/
                                       
}DcpdPackageToMpuCtrRsp_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;
    UINT8 u8CtrType;
    UINT8 u8Reserve;
}DcpdPackageToMcuCtrReq_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;
    UINT8 u8WkeNetRst;
    UINT8 u8WkeNetSource;
}DcpdPackageToMPUWkeNetRst_t;
typedef struct
{
    DcpdPackageHeader_t tHeader;
    UINT8 u8WkeNetSr;
}DcpdPackageToMcuWkeNetReq_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;
    UINT8 u8CommandId;
    UINT8 u8DataLen;
    UINT8 u8Data[256];
}DcpdPackageToMPUBleRefRst_t;

typedef struct
{
	DcpdPackageHeader_t tHeader;
	UINT8 u8CommandId;
	UINT8 u8DataLen;
	UINT8 u8Data[256];
}DcpdPackageToMcuBleRefReq_t;

typedef struct
{
	DcpdPackageHeader_t tHeader; /*!< tHeader */ 
	UINT8 u8ReqSrc; 			 /*!< PGN请求源 */ 
	UINT32 u32PGN;				 /*!< PGN */ 
}DcpdPackageToMcuPGNReq_t;
	
typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
	UINT8 u8ReqSrc; 			   /*!< PGN请求源 */
	UINT8 u8Result; 			   /*!< 响应结果*/
	UINT16 u16DataLen;			   /*!< PGN数据长度*/
	UINT32 u32PGN;				   /*!< PGN*/
	UINT8 pu8Data[255]; 		   /*!< 数据响应*/
}DcpdPackageToMPUPGNRsp_t;
	
	
typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
	UINT16 u16DataLen;			   /*!< PGN数据长度*/
	UINT32 u32PGN;				   /*!< PGN*/
	UINT8 pu8Data[255]; 		   /*!< 数据响应*/
}DcpdPackageToMPUPGNBC_t;

typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
	UINT8 LoninState; 		   /*!< 0:Not login 1:has login */
}DcpdPackageToMCUTspLoginState_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */	
    UINT8 u8CodecSwt;               /*!< Codec开关*/              
}DcpdPackageToMcuCodecSwt_t;

/****************HMI  data****************************/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */	
    UINT8   u8BatteryVoltage;               //电池电压
    UINT16  u16Speed;                       //车速
    UINT16  u16EngineSpeed;                 //发动机转速
    UINT8   u8ParkingBrake;                 //驻车制动结合状态
    UINT8   u8KeySwitch;                    //钥匙开关
}DcpdHMIUpdataCondition_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */	
    UINT8 u8SelectResults;                  //用户的选择结果
    UINT8 u8InstallationProgress;           //升级安装进度应答
    UINT8 u8InstallationResults;            //升级的结果应答
    UINT8 u8EquipmentIdentification;        //HMI设备标识
}DcpdHMIReceiveData_t;
typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
	UINT8   u8SPN1;                     //标定更新(1)
    UINT8   u8SPN3;                     //选择结果应答(3)
    UINT8   u8SPN5;                     //安装进度(5)
    UINT8   u8SPN8;                     //安装结果(8)
    UINT8   u8SPN6;                    //安装进度(6)
    UINT8   u8SPN4;                     //升级条件(4)
    UINT8   u8SPN10;                    //车辆停止(10)
    UINT8   u8SPN11;                    //驻车制动(11)
    UINT8   u8SPN12;                    //发动机停机(12)
    UINT8   u8SPN13;                    //钥匙开关(13)
    UINT8   u8SPN14;                    //电池电压(14)
    UINT8   u8SPN15;                    //卫星信号(15)
}DcpdHMISendData_t;


typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */
    UINT16 u16DataLen;
    UINT8 au8BtcKey[256];
}DcpdPackageToMpuBtcOta_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */
    UINT8 u8Result;       /*!< 0：成功，其它：失败 */
}DcpdPackageToMcuBtcOtaTxResult_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8OpReq;                   /*!< 远程DVR视频请求:
                                      - reserve, 保留
                                    */
}DcpdPackageToMcWifiDvrVidReq_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 u8OpResult;        /*!< 操作结果
                                - TRUE 成功
                                - FALSE 失败
                            */
}DcpdPackageToMpuWifiDvrVidResp_t;

typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
    // UINT32 WarningInfo1_timestamp;
	UINT16 ICT_Warning_FAS_LaneTrackingStatusRightSide;                     //车道左偏离
    UINT16 ICT_Warning_FAS_LaneTrackingStatusLeftSide;                      //车道右偏离
    UINT16 ICT_Warning_FAS_DSM_Warning;                                     //司机疲劳预警
    UINT16 ICT_Warning_FAS_HW_Status;                                       //车距监控预警
    UINT16 ICT_Warning_FCW_Alarm_Status;                                    //FCW碰撞预警
    UINT16 ICT_Warning_OGP_Forward_Collision_Warning;                       //
    UINT16 ICT_Warning_OGP_Advanced_emergency_braking_system_state;         //AEBS点刹预警
    UINT16 ICT_Warning_OGP_Advanced_emergency_braking_system_state2;        //AEBS紧急制动
}DcpdWarningInfo1_t;//（预警监控、2020.4.25提交）中通卡嘉定制策略（一期）0x6058

typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */
    // UINT32 WarningInfo2_timestamp;	
	UINT8 ICT_Warning_AEBS_Switch;                  //AEBS开关
    UINT8 ICT_Warning_LDW_Main_Switch;              //车道偏离预警功能开关
    UINT8 ICT_Warning_FCW_Main_Switch;              //前碰撞预警功能开关
    UINT8 ICT_Warning_DSM_Main_Switch;              //疲劳预警功能开关
    UINT8 ICT_Warning_ACCUsageDemand;		        //ACC开关
}DcpdWarningInfo2_t;//（开关状态监控、2020.10.8提交）中通卡嘉定制策略（二期）无TypeID(WORD)

#ifdef __cplusplus
}
#endif

#pragma pack()


#endif /*MPU_SPP_H*/



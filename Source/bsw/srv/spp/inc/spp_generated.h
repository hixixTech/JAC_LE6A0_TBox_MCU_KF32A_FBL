

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
    SPP_USER_DCPD_MSG_TO_MCU_IS_SLEEP_READY_EVT = SPP_USER_DCPD_MSG_TOMCU_START,    /*!< __Description:__ ����mpu�Ƿ����sleep��Ϣ \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_PM \n
                                                                        __RequestData:__ DcpdPackageToMcuSleepReady_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_PM_CHANGE_END_EVT,                   /*!< __Description:__ MPU���͵�Դ״̬�л���� \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_PM \n
                                                                        __RequestData:__ DcpdPackageToMcuPmChangeEnd_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_EOL_CMD_RESP_EVT,             /*!< __Description:__ EOLָ��Ӧ�� \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ �ο� @ref   DcpdPackageToMcuEolCmdResp_t  \n
                                                                        __ResponseData:__ None \n
                                                                  */
    SPP_USER_DCPD_MSG_TO_MCU_IMEI_RESP_EVT,                /*!< __Description:__ ����IMEI��Ϣ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuImei_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_GNSS_EVT,                         /*!< __Description:__ ͬ��Gnss���ݸ�MCU \n
                                                                        __Sender:__ Mpu_GnssSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuGnss_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_INICFG_REQ_EVT,                   /*!< __Description:__ ��ѯ��ʼ������Ϣ \n
                                                                        __Sender:__ Mpu_cfgApp \n
                                                                        __Receiver:__ Mcu_DIAG \n
                                                                        __RequestData:__ None  \n
                                                                        __ResponseData:__   �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_INICFG_RESP_EVT \n
                                                                    */  
    SPP_USER_DCPD_MSG_TO_MCU_CFG_SV_REQ_EVT,                   /*!< __Description:__ ���󱣴�������Ϣ \n
                                                                        __Sender:__ Mpu_cfgApp \n
                                                                        __Receiver:__ Mcu_DIAG \n
                                                                        __RequestData:__ �ο� @ref DcpdPackageToMcuCfgSvReq_t  \n
                                                                        __ResponseData:__   �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_CFG_SV_RESP_EVT \n
                                                                    */  
    SPP_USER_DCPD_MSG_TO_MCU_ICCID_RESP_EVT,              /*!< __Description:__ ����ICCID��Ϣ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuIccid_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_IMSI_RESP_EVT,              /*!< __Description:__ ����IMSI��Ϣ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuImsi_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_NADSTA1_RESP_EVT,            /*!< __Description:__ ����Nad״̬��Ϣ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuMccMnc_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_NADSTA2_RESP_EVT,            /*!< __Description:__ ����Nad״̬��Ϣ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuNadSta2_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_NADIP1_RESP_EVT,             /*!< __Description:__ ����IP1��Ϣ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuNadIp1_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_NADIP2_RESP_EVT,             /*!< __Description:__ ����IP2��Ϣ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__  DcpdPackageToMcuNadIp2_t \n
                                                                        __ResponseData:__ None \n
                                                                    */ 
    SPP_USER_DCPD_MSG_TO_MCU_RVC_OTA_MSG_EVT,                   /*!< __Description:__ OTA������Ϣ \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_RVC \n
                                                                        __RequestData:__  �ο� @ref DcpdPackageToMcuRvcOtaMsgRx_t \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_RVC_OTA_MSG_ACK_EVT  \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_RVC_OTA_TX_RESULT_EVT,             /*!< __Description:__ OTA������Ϣ���͵Ľ�� \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_RVC \n
                                                                        __RequestData:__  DcpdPackageToMcuOtaTxResult_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_TBOX_REG_ACK_EVT,                  /*!< __Description:__ �ն�ע��Ӧ�� \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None  \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_TBOX_REG_RESULT_EVT,                  /*!< __Description:__ �ն�ע���� \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  DcpdPackageToMcuTboxRegResult_t \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_TBOX_REG_RESULT_ACK_EVT   \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_CFG_UPDATE_IND_ACK_EVT,           /*!< __Description:__ ������Ϣ����Ӧ�� \n
                                                                        __Sender:__ Mpu_CfgApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None  \n
                                                                    */
                                                                    
    SPP_USER_DCPD_MSG_TO_MCU_WIFI_IND_ACK_EVT,             /*!< __Description:__ ����socket��Ӧ����Ϣ \n
                                                                        __Sender:__ Mpu_WifiApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  DcpdPackageToMcuWifiIndResp_t \n
                                                                        __ResponseData:__ None  \n
                                                                    */
                                                                    
    SPP_USER_DCPD_MSG_TO_MCU_DTCRD_REQ_ACK_EVT,                   /*!< __Description:__ �����ȡDTC ACK \n
                                                                        __Sender:__ Mpu_FotaRDApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None  \n
                                                                    */  
    SPP_USER_DCPD_MSG_TO_MCU_PRE_RD_REQ_EVT,                   /*!< __Description:__ �����ȡԶ�����Ԥ����Ϣ \n
                                                                        __Sender:__ Mpu_FotaRDApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_PRE_RD_RESP_EVT  \n
                                                                    */ 
    SPP_USER_DCPD_MSG_TO_MCU_PRE_FOTA_REQ_EVT,                   /*!< __Description:__ �����ȡFOTAԤ����Ϣ \n
                                                                        __Sender:__ Mpu_FotaRDApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_PRE_FOTA_RESP_EVT\n
                                                                    */ 
    SPP_USER_DCPD_MSG_TO_MCU_FENCE_SAVE_REQ_EVT,                   /*!< __Description:__ ���󱣴����Ȧ��Ϣ \n
                                                                        __Sender:__ Mpu_RvmApp \n
                                                                        __Receiver:__ Mcu_DIAG    \n
                                                                        __RequestData:__  �ο�  @ref DcpdPackageToMcuDefenceSvReq_t \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_FENCE_SAVE_RESP_EVT\n
                                                                    */
                                                                    
    SPP_USER_DCPD_MSG_TO_MCU_DEFENCE_ALM_EVT,                   /*!< __Description:__ ����Χ������֪ͨ \n
                                                                        __Sender:__ Mpu_RvmApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_DEFENCE_ALM_ACK_EVT\n
                                                                    */
                                                                    
    SPP_USER_DCPD_MSG_TO_MCU_VEH_REPORT_ACK_EVT,                /*!< __Description:__ ����״̬Ӧ�� \n
                                                                        __Sender:__ Mpu_FotaRvmApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_EMMC_STA_EVT,                     /*!< __Description:__ ����EMMC��״̬��Ϣ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv    \n
                                                                        __RequestData:__  DcpdPackageToMcuEmmcSta_t \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_EMMC_STA_ACK_EVT\n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_WIFI_STA_EVT,                     /*!< __Description:__ ����WIFI״̬��Ϣ \n
                                                                        __Sender:__ Mpu_WifiApp \n
                                                                        __Receiver:__ Mcu_WanSrv    \n
                                                                        __RequestData:__  DcpdPackageToMcuWifiSta_t \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_WIFI_STA_ACK_EVT\n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_TSPCON_STA_EVT,                   /*!< __Description:__ ����TSP����״̬��Ϣ \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__ Mcu_WanSrv    \n
                                                                        __RequestData:__  DcpdPackageToMcuTspConSta_t \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_TSPCON_STA_ACK_EVT\n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_SLEEP_ERR_ACK_EVT,                /*!< __Description:__ �����쳣ACK \n
                                                                        __Sender:__ Mpu_RvmApp \n
                                                                        __Receiver:__ Mcu_pm    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_WIN_AUTOCLOSE_ACK_EVT,            /*!< __Description:__ �����Զ��ش�Ӧ�� \n
                                                                        __Sender:__ Mpu_FotaRvmApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  None \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_SIM_STA_EVT,                          /*!< __Description:__ Dcpd����SIM������״̬ \n
                                                                        __Sender:__ Mpu_WanSrv \n
                                                                        __Receiver:__ Mcu_WanSrv \n
                                                                        __RequestData:__ DcpdPackageToMcuSimSta_t \n
                                                                        __ResponseData:__ �ο� @ref    SPP_USER_DCPD_MSG_TO_MPU_SIM_STA_ACK_EVT  \n
                                                                    */
    SPP_USER_MPU_RES_GNSS_EVT,                                 /*!< __Description:__ Dcpd����GNSS��Ϣ \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ DcpdPackageEolGnssInfo_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_MPU_RES_WAN_EVT,                                  /*!< __Description:__ Dcpd����WAN��Ϣ \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ DcpdPackageEolWanConnInfo_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_MPU_RES_UART_EVT,                                 /*!< __Description:__ Dcpd����WAN��Ϣ \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ DcpdPackageEolUartInfo_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_MPU_RES_PING_EVT,                                 /*!< __Description:__ Dcpd����WAN��Ϣ \n
                                                                        __Sender:__ Mpu_AuxSrv \n
                                                                        __Receiver:__ Mcu_EOL \n
                                                                        __RequestData:__ DcpdPackageEolPingInfo_t \n
                                                                        __ResponseData:__ None \n
                                                                    */

    SPP_USER_DCPD_MSG_TO_MCU_RDIAG_REQ_EVT,                   /*!< __Description:__ Dcpd���ͼ��Զ��������� \n
                                                                        __Sender:__ Mpu_FotaEApp \n
                                                                        __Receiver:__ Mcu_DIAG \n
                                                                        __RequestData:__  �ο� @ref DcpdPackageToMcuRDiagReq_t
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MPU_RDIAG_RESP_EVT \n
                                                                  */

    SPP_USER_DCPD_MSG_TO_MCU_SYS_TIME_REQ_EVT,                 /*!< __Description:__ ����MCU��ϵͳʱ�� \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_Auxiliary \n
                                                                        __RequestData:__ None \n
                                                                        __ResponseData:__ �ο� @ref   SPP_USER_DCPD_MSG_TO_MPU_SYS_TIME_RESP_EVT \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MCU_SYS_TIME_RESP_EVT,                 /*!< __Description:__ ����Mpu��ϵͳʱ�� \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_Auxiliary \n
                                                                        __RequestData:__ �ο� @ref   DcpdPackageToMcuSysTimeOfMpu_t  \n
                                                                        __ResponseData:__ None \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MCU_RVC_OTA_MSG_ACK_EVT,           /*!< __Description:__ MPU�����յ�RVC OTA��Ϣ��ACK \n
                                                                        __Sender:__ Mpu_TspSrv \n
                                                                        __Receiver:__  Mcu_RVC \n
                                                                        __RequestData:__ �ο� @ref DcpdPackageToMcuRvcOtaTxAck_t \n
                                                                        __ResponseData:__ None \n
                                                                 */
    SPP_USER_DCPD_MSG_TO_MCU_RESET_TBOX_EVT,                /*!< __Description:__ MPU���͸�λTBOX \n
                                                                        __Sender:__ Mpu_SysMgrSrv \n
                                                                        __Receiver:__ Mcu_PM \n
                                                                        __RequestData:__ DcpdPackageToMcuResetTboxReq_t \n
                                                                        __ResponseData:__ None \n
                                                                    */

    SPP_USER_DCPD_MSG_TO_MCU_PHNUM_RESP_EVT,                /*!< __Description:__ MPU���͵绰���� \n
                                                                            __Sender:__ Mpu_WanSrv \n
                                                                            __Receiver:__ Mcu_WanSrv \n
                                                                            __RequestData:__ DcpdPackageToMcuPhnumResp_t \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_UPDATE_ENGINE_DATA_ACK_EVT,   /*!< __Description:__ MPU�����ŷ�����ź�֪ͨӦ�� \n
                                                                            __Sender:__ MPU_EmissionApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_KMS_TELE_INFO_ACK_EVENT,      /*!< __Description:__ MPU�����ն���Ϣ�ϱ�Ӧ�� \n
                                                                            __Sender:__ MPU_KMSApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_KMS_HB_ACK_EVENT,              /*!< __Description:__ MPU�������������ϱ�Ӧ�� \n
                                                                            __Sender:__ MPU_KMSApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_APN_STA_EVT,               /*!< __Description:__ MPU����˽��APN����״̬ \n
                                                                            __Sender:__ Mpu_WanSrv \n
                                                                            __Receiver:__ Mcu_WanSrv \n
                                                                            __RequestData:__ DcpdPackageToMcuApnSta_t \n
                                                                            __ResponseData:__ None \n
                                                                        */ 			
    SPP_USER_DCPD_MSG_TO_MCU_APN_CFG_REQ_EVT,               /*!< __Description:__ MPU����APN�������� \n
                                                                            __Sender:__ Mpu_WanSrv \n
                                                                            __Receiver:__ Mcu_WanSrv \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_APN_CFG_RESP_EVT \n
                                                                        */ 	
    SPP_USER_DCPD_MSG_TO_MCU_XCALL_INFO_REQ_EVT,               /*!< __Description:__ MPU����APN�������� \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */ 	
    SPP_USER_DCPD_MSG_TO_MCU_CAN_TRANSMIT,                     /*!< __Description:__ MPU��CAN�ź�ֵMCU \n
                                                                            __Sender:__ Mpu_OtaLib \n
                                                                            __Receiver:__ Mcu_DIAG \n
                                                                            __RequestData:__ DcpdPackageCANSignalTransmit_t \n
                                                                            __ResponseData:__ None \n
                                                                        */ 
    SPP_USER_DCPD_MSG_TO_MCU_CTR_VEH_REQ_EVT,                  /*!< __Description:__ MPU���������е����� \n
                                                                            __Sender:__ Mpu_OtaLib \n
                                                                            __Receiver:__ Mcu_Rvc \n
                                                                            __RequestData:__ DcpdPackageToMcuCtrReq_t \n
                                                                            __ResponseData:__ DcpdPackageToMpuCtrRsp_t\n
                                                                        */
    SPP_USRT_DCPD_MSG_TO_MCU_WKE_NET_REQ_EVT,                    /*!< __Description:__ MPU���ͻ�������can�������� \n
                                                                            __Sender:__ Mpu_OtaLib \n
                                                                            __Receiver:__ Mcu_diag \n
                                                                            __RequestData:__ DcpdPackageToMcuWkeNetReq_t \n
                                                                            __ResponseData:__ DcpdPackageToMPUWkeNetRst_t\n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_BLE_REFRESH_REQ_EVT,               /*!< __Description:__ MPU��������ˢ������ \n
                                                                            __Sender:__ Mpu_FotaSrv \n
                                                                            __Receiver:__ Mcu_Ble \n
                                                                            __RequestData:__ DcpdPackageToMcuBleRefReq_t \n
                                                                            __ResponseData:__ DcpdPackageToMPUBleRefRst_t\n
                                                                        */
    
    SPP_USER_DCPD_MSG_TO_MCU_PGN_REQ_EVT,                       /*!< __Description:__ MPU����PGN���� \n
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
	SPP_USER_DCPD_MSG_TO_MCU_XCALL_REPORT_ACK_EVT,           /*!< __Description:__ MPU����XCALL ACK  ��ICT \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */ 	

    SPP_USER_DCPD_MSG_TO_MCU_XCALL_DIAL_ACK_EVT,           /*!< __Description:__ MPU�յ�XCALL������������ظ�ACK \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_PM \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_XCALL_DIAL_FINISH_EVT,           /*!< __Description:__ MPU����ͨ�������¼� \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_PM \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_XCALL_CODEC_SWITCH_EVT,           /*!< __Description:__ MPU������Ƶͨ���л������¼� \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_PM \n
                                                                            __RequestData:__ INT8 0:DEFAULT 1:AUX 2:���������� \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_TSP_LOGIN_STATE_EVT,           /*!< __Description:__ MPU����TSP��¼״̬�¼� \n
                                                                            __Sender:__ Mpu_TspSrv \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ DcpdPackageToMCUTspLoginState_t
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MCU_GET_HMI_UPDATA_EVT,				/*!< __Description:__ MPU���ͻ�ȡ���������¼� \n
                                                                            __Sender:__ Mpu_KMSAPP \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */	
    SPP_USER_DCPD_MSG_TO_MCU_HMI_ACK_EVT,					    /*!< __Description:__ MPU���ͱ궨����֮ǰ״̬��ACK \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_ICM_UPDATA_EVT,                        /*!< __Description:__ MPU���ͱ궨������������״̬ \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */ 
    SPP_USER_DCPD_MSG_TO_MCU_BTC_OTA_MSG_ACK_EVT,               /*!< __Description:__ MPU���ͽ��յ�������ԿӦ�� \n
                                                                            __Sender:__ Mpu_TspSrv \n
                                                                            __Receiver:__ Mcu_BTC \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ None \n
                                                                        */ 
    SPP_USER_DCPD_MSG_TO_MCU_BTC_OTA_TX_RESULT_EVT,             /*!< __Description:__ MPU����������Կ���ͳɹ���Ϣ \n
                                                                            __Sender:__ Mpu_XcallApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ DcpdPackageToMcuBtcOtaTxResult_t \n
                                                                            __ResponseData:__ None \n
                                                                        */ 
    SPP_USER_DCPD_MSG_TO_MCU_ICT_KMS_PRIVATECAN_STATUS_MSG_EVT,/*!< __Description:__ MPU�����յ�KMS˽��CAN״̬�ź� \n
                                                                            __Sender:__ Mpu_KMSApp \n
                                                                            __Receiver:__ Mcu_ICT \n
                                                                            __RequestData:__ DcpdPackageToMcuBtcOtaTxResult_t \n
                                                                            __ResponseData:__ None \n
                                                                        */ 

    SPP_USER_DCPD_MSG_TO_MCU_WIFI_REQ_DVR_VID_EVT,             /*!< __Description:__ ����mcu����dvr������Զ����Ƶ \n
                                                                        __Sender:__ Mpu_WifiApp \n
                                                                        __Receiver:__ Mcu_ICT    \n
                                                                        __RequestData:__  DcpdPackageToMcWifiDvrVidReq_t \n
                                                                        __ResponseData:__ None  \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MCU_CTR_VEH_RES_ACK_EVT,                   /*!< __Description:__ �е���Ӧ��ϢACK \n
                                                                       __Sender:__ Mpu_TspSrv \n
                                                                       __Receiver:__  Mcu_VC\n
                                                                       __RequestData:__ �ο�  \n
                                                                       __ResponseData:__  \n
                                                               */ 
    SPP_USER_DCPD_MSG_TOMCU_MAX

} EMpuSendEvent;

typedef enum 
{
    
    SPP_USER_DCPD_MSG_TO_MPU_PM_STATUS_UPDATE_EVT = SPP_USER_DCPD_MSG_TOMPU_START,    /*!< __Description:__ ���͵�Դ����ģʽ \n
                                                                                   __Sender:__ Mcu_PM \n
                                                                                   __Receiver:__ Mpu_SysMgrSrv \n
                                                                                   __RequestData:__ DcpdPackageToMpuPmStatusUpdate_t \n
                                                                                   __ResponseData:__ None \n
                                                                                    */
    SPP_USER_DCPD_MSG_TO_MPU_MCULOG_EVT,                   /*!< __Description:__ MCU Log��Ϣ \n
                                                                       __Sender:__ Mcu_ \n
                                                                       __Receiver:__ Mpu_LogSrv \n
                                                                       __RequestData:__ �ο� @ref DcpdPackageToMpuMcuLog_t  \n
                                                                       __ResponseData:__  \n
                                                               */  
    SPP_USER_DCPD_MSG_TO_MPU_EOL_CMD_REQ_EVT,              /*!< __Description:__ EOL����/��ָ֤�� \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ �ο� @ref DcpdPackageToMpuEolCmdReq_t \n
                                                                       __ResponseData:__ �ο� @ref   SPP_USER_DCPD_MSG_TO_MCU_EOL_CMD_RESP_EVT \n
                                                                */
    SPP_USER_DCPD_MSG_TO_MPU_INICFG_RESP_EVT,              /*!< __Description:__ ���س�ʼ������Ϣ \n
                                                                           __Sender:__ Mcu_DIAG \n
                                                                           __Receiver:__ Mpu_CfgApp \n
                                                                           __RequestData:__ �ο� @ref DcpdPackageToMpuIniCfg_t  \n
                                                                           __ResponseData:__  \n
                                                                       */  
    SPP_USER_DCPD_MSG_TO_MPU_CFG_SV_RESP_EVT,                   /*!< __Description:__ ����������Ϣ������ \n
                                                                           __Sender:__ Mcu_DIAG \n
                                                                           __Receiver:__ Mpu_CfgApp \n
                                                                           __RequestData:__ �ο� @ref DcpdPackageToMpuCfgSvResp_t  \n
                                                                           __ResponseData:__  \n
                                                                       */  
    SPP_USER_DCPD_MSG_TO_MPU_IMEI_REQ_EVT,                   /*!< __Description:__ ����IMEI��Ϣ   \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_IMEI_RESP_EVT   \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_ICCID_REQ_EVT,                  /*!< __Description:__ ����ICCID��Ϣ  \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_ICCID_RESP_EVT  \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_IMSI_REQ_EVT,                   /*!< __Description:__ ����IMSI��Ϣ   \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_IMSI_RESP_EVT   \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_NADSTA1_REQ_EVT,                /*!< __Description:__ ����Nad״̬��Ϣ  \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_NADSTA1_RESP_EVT    \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_NADSTA2_REQ_EVT,                /*!< __Description:__ ����Nad״̬��Ϣ  \n
                                                                           __Sender:__ Mcu_WanSrv  \n
                                                                           __Receiver:__ Mpu_WanSrv    \n
                                                                           __RequestData:__  None   \n
                                                                           __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_NADSTA2_RESP_EVT  \n
                                                                       */
    SPP_USER_DCPD_MSG_TO_MPU_NADIP1_REQ_EVT,              /*!< __Description:__ ����IP1��Ϣ   \n
                                                                       __Sender:__ Mcu_WanSrv  \n
                                                                       __Receiver:__ Mpu_WanSrv    \n
                                                                       __RequestData:__  None   \n
                                                                       __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_NADIP1_RESP_EVT \n
                                                                   */

    SPP_USER_DCPD_MSG_TO_MPU_NADIP2_REQ_EVT,              /*!< __Description:__ ����IP2��Ϣ   \n
                                                                       __Sender:__ Mcu_WanSrv  \n
                                                                       __Receiver:__ Mpu_WanSrv    \n
                                                                       __RequestData:__  None   \n
                                                                       __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_NADIP2_RESP_EVT \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_RVC_OTA_MSG_EVT,                   /*!< __Description:__ ����Ota��Ϣ���� \n
                                                                       __Sender:__ Mcu_RVC  \n
                                                                       __Receiver:__ Mpu_TspSrv    \n
                                                                       __RequestData:__  �ο� @ref DcpdPackageToMpuRvcOtaMsgTx_t   \n
                                                                       __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_RVC_OTA_MSG_ACK_EVT  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_TBOX_REG_REQ_EVT,                  /*!< __Description:__ �����ն�ע�� \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_TspSrv    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MCU_TBOX_REG_ACK_EVT  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_TBOX_REG_RESULT_ACK_EVT,          /*!< __Description:__ �����ն�ע����Ӧ�� \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_TspSrv    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_CFG_UPDATE_IND_EVT,          /*!< __Description:__ ������Ϣ����֪ͨ \n
                                                                       __Sender:__ Mcu_ \n
                                                                       __Receiver:__ Mpu_cfgApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuCfgUpdateInd_t \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MCU_CFG_UPDATE_IND_ACK_EVT  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_WIFI_IND_EVT,                 /*!< __Description:__ ֪ͨwifi�����¼���WiFi��������socket������ \n
                                                                       __Sender:__ Mcu_ICT \n
                                                                       __Receiver:__ Mpu_WifiApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuWifiInd_t \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MCU_WIFI_IND_ACK_EVT  \n
                                                                   */
    SPP_USER_DCPD_MSG_TO_MPU_DTCRD_REQ_EVT,                /*!< __Description:__ �����ȡDTC \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_FotaRDApp    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MCU_DTCRD_REQ_ACK_EVT  \n
                                                               */
                                                               
    SPP_USER_DCPD_MSG_TO_MPU_PRE_RD_RESP_EVT,              /*!< __Description:__ ����Զ�����Ԥ����Ϣ \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_FotaRDApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuPreRdResp_t \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_PRE_FOTA_RESP_EVT,              /*!< __Description:__ ����FOTAԤ����Ϣ \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_FotaRDApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuPreFotaResp_t \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_FENCE_SAVE_RESP_EVT,          /*!< __Description:__ ����Ȧ��Ϣ�ı����� \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuFenceSaveResp_t \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_DEFENCE_ALM_ACK_EVT,          /*!< __Description:__ ����Χ������Ӧ�� \n
                                                                       __Sender:__ Mcu_ICT \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_VEH_REPORT_EVT,               /*!< __Description:__ ����״̬֪ͨ \n
                                                                       __Sender:__ Mcu_ICT \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  DcpdPackageToMpuVehReport_t \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MCU_VEH_REPORT_ACK_EVT  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_EMMC_STA_ACK_EVT,             /*!< __Description:__ ����EMMC��״̬��ϢӦ�� \n
                                                                       __Sender:__ Mcu_WanSrv \n
                                                                       __Receiver:__ Mpu_WanSrv    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_WIFI_STA_ACK_EVT,             /*!< __Description:__ ����WIFI״̬��ϢӦ�� \n
                                                                       __Sender:__ Mcu_WanSrv \n
                                                                       __Receiver:__ Mpu_WifiApp    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_TSPCON_STA_ACK_EVT,             /*!< __Description:__ ����TSP����״̬��ϢӦ�� \n
                                                                       __Sender:__ Mcu_WanSrv \n
                                                                       __Receiver:__ Mpu_TspSrv    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ None  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_SLEEP_ERR_EVT,               /*!< __Description:__ �����쳣֪ͨ \n
                                                                       __Sender:__ Mcu_PM \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  None \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MCU_SLEEP_ERR_ACK_EVT  \n
                                                               */
    SPP_USER_DCPD_MSG_TO_MPU_WIN_AUTOCLOSE_EVT,               /*!< __Description:__ �������졢Ϩ���Զ��ش�֪ͨ \n
                                                                       __Sender:__ Mcu_ICT \n
                                                                       __Receiver:__ Mpu_RvmApp    \n
                                                                       __RequestData:__  �ο� @ref DcpdPackageToMpuWinAutoClose_t \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MCU_WIN_AUTOCLOSE_ACK_EVT  \n
                                                               */

    SPP_USER_DCPD_MSG_TO_MPU_SIM_STA_ACK_EVT,                      /*!< __Description:__ ��Ϣ  @ref SPP_USER_DCPD_MSG_TO_MCU_SIM_STA_EVT ��Ӧ�� \n
                                                                       __Sender:__ Mcu_WanSrv \n
                                                                       __Receiver:__ Mpu_WanSrv \n
                                                                       __RequestData:__ None \n
                                                                       __ResponseData:__ None \n
                                                                   */
    SPP_USER_MCU_REQ_GNSS_EVT,                                 /*!< __Description:__ ����Gnss��Ϣ \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ DcpdPackageHeader_t \n
                                                                       __ResponseData:__ �ο� @ref SPP_USER_MPU_RES_GNSS_EVT \n
                                                                   */
    SPP_USER_MCU_REQ_WAN_EVT,                                  /*!< __Description:__ ����Wan��Ϣ \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ DcpdPackageHeader_t \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_MPU_RES_WAN_EVT \n
                                                                   */
    SPP_USER_MCU_REQ_UART_EVT,                                 /*!< __Description:__ ����UARTͨ·���� \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ DcpdPackageEolUartInfo_t \n
                                                                       __ResponseData:__ �ο�   @ref SPP_USER_MPU_RES_UART_EVT \n
                                                                   */
    SPP_USER_MCU_REQ_PING_EVT,                                 /*!< __Description:__ ����Ping���� \n
                                                                       __Sender:__ Mcu_EOL \n
                                                                       __Receiver:__ Mpu_AuxSrv \n
                                                                       __RequestData:__ DcpdPackageHeader_t \n
                                                                       __ResponseData:__ �ο�   @ref  SPP_USER_MPU_RES_PING_EVT \n
                                                                   */

    SPP_USER_DCPD_MSG_TO_MPU_RDIAG_RESP_EVT,                    /*!< __Description:__ ����Զ����Ͻ�� \n
                                                                       __Sender:__ Mcu_DIAG \n
                                                                       __Receiver:__ Mpu_FotaRDSrv \n
                                                                       __RequestData:__ �ο� @ref   DcpdPackageToMpuRDiagResp_t \n
                                                                       __ResponseData:__ None \n
                                                                   */

    SPP_USER_DCPD_MSG_TO_MPU_SYS_TIME_RESP_EVT,            /*!< __Description:__ ����MCU��ϵͳʱ�� \n
                                                                       __Sender:__ Mcu_Auxiliary \n
                                                                       __Receiver:__ Mpu_SysMgrSrv \n
                                                                       __RequestData:__ �ο� @ref DcpdPackageToMpuSysTimeOfMcu_t \n
                                                                       __ResponseData:__ �ο� None  \n
                                                                */
    SPP_USER_DCPD_MSG_TO_MPU_SYS_TIME_REQ_EVT,             /*!< __Description:__ ����MPU  ��ϵͳʱ�� \n
                                                                       __Sender:__ Mcu_Auxiliary \n
                                                                       __Receiver:__ Mpu_SysMgrSrv \n
                                                                       __RequestData:__ None \n
                                                                       __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_ SYS_TIME_RESP_EVT \n
                                                                */
    SPP_USER_DCPD_MSG_TO_MPU_RVC_OTA_MSG_ACK_EVT,           /*!< __Description:__ MCU�����յ�RVC OTA��Ϣ��ACK \n
                                                                       __Sender:__ Mcu_RVC \n
                                                                       __Receiver:__ Mpu_TspSrv \n
                                                                       __RequestData:__ �ο� @ref DcpdPackageToMpuRxRvcOtaAck_t  \n
                                                                       __ResponseData:__ None \n
                                                                */
    SPP_USER_DCPD_MSG_TO_MPU_PHNUM_REQ_EVT,                /*!< __Description:__ MCU��������绰���� \n
                                                                        __Sender:__ Mcu_WanSrv \n
                                                                        __Receiver:__ Mpu_WanSrv \n
                                                                        __RequestData:__ None \n
                                                                        __ResponseData:__ �ο� @ref SPP_USER_DCPD_MSG_TO_MCU_PHNUM_RESP_EVT \n
                                                                        */   
                                                                                                                                      
    SPP_USER_DCPD_MSG_TO_MPU_UPDATE_ENGINE_DATA_EVT,                /*!< __Description:__ MCU���ͷ��������� \n
                                                                        __Sender:__Mcu_ICT\n
                                                                        __Receiver:__Mpu_EmissionApp \n
                                                                        __RequestData:_�ο� @ref   DcpdPackageToMpuEngData_t\n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_KMS_TELE_INFO_EVENT,         /*!< __Description:__ Mcu���Ϳ���˹���������ϱ� \n
                                                                            __Sender:__ Mcu_ICT  \n
                                                                            __Receiver:__ MPU_KMSApp\n
                                                                            __RequestData:__ DcpdPacketKMSTelemticInfo_t \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_KMS_HB_EVENT,                /*!< __Description:__ Mcu���Ϳ���˹���������ϱ� \n
                                                                            __Sender:__ Mcu_ICT  \n
                                                                            __Receiver:__ MPU_KMSApp\n
                                                                            __RequestData:__ DcpdPackageKmsHBData_t \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_APN_CFG_RESP_EVT,              /*!< __Description:__ MCU����APN��Ϣ \n
                                                                        __Sender:__ Mcu_WanSrv \n
                                                                        __Receiver:__ Mpu_WanSrv \n
                                                                        __RequestData:__ DcpdPackageToMpuApnCfg_t \n
                                                                        __ResponseData:__ None \n
                                                                        */    
    SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT,          /*!< __Description:__ MCU����XCALL��Ϣ \n
                                                                        __Sender:__ Mcu_ICT \n
                                                                        __Receiver:__ Mpu_XcallApp \n
                                                                        __RequestData:__ DcpdPackageToMpuVehReport_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_CAN_TRANSMIT,                      /*!< __Description:__ MCUת��CAN�ź� \n
                                                                        __Sender:__ Mcu_DIAG \n
                                                                        __Receiver:__ Mpu_OtaLib \n
                                                                        __RequestData:__ DcpdPackageCANSignalTransmit_t \n
                                                                        __ResponseData:__ None \n
                                                                        */                                             
    SPP_USER_DCPD_MSG_TO_MPU_VEH_CON_DATA_EVT,             /*!< __Description:__ MCU����fota��ص�������Ϣ \n
                                                                        __Sender:__ Mcu_DIAG \n
                                                                        __Receiver:__ Mpu_OtaLib \n
                                                                        __RequestData:__ DcpdPackageToMpuVehConData_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_CTR_VEH_RES_EVT,              /*!< __Description:__ MCU�����е������Ӧ \n
                                                                        __Sender:__ Mcu_RVC \n
                                                                        __Receiver:__ Mpu_OtaLib \n
                                                                        __RequestData:__ DcpdPackageToMpuCtrRsp_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USRT_DCPD_MSG_TO_MPU_WKE_NET_RES_EVT,              /*!< __Description:__ MCU���ͻ�������������Ӧ \n
                                                                        __Sender:__ Mcu_Diag \n
                                                                        __Receiver:__ Mpu_OtaLib \n
                                                                        __RequestData:__ DcpdPackageToMPUWkeNetRst_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_BLE_REFRESH_RESP_EVT,         /*!< __Description:__ MCU��������ˢ��������Ӧ \n
                                                                        __Sender:__ Mcu_Ble \n
                                                                        __Receiver:__ Mpu_FotaRDSrv \n
                                                                        __RequestData:__ DcpdPackageToMPUBleRefRst_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
                                                                        
    SPP_USER_DCPD_MSG_TO_MPU_PGN_RSP_EVT,                  /*!< __Description:__ MCU����PGN������Ӧ \n
                                                                        __Sender:__ Mcu_Diag \n
                                                                        __Receiver:__ Mpu_Emission \n
                                                                        __RequestData:__ DcpdPackageToMPUPGNRsp_t \n
                                                                        __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_PGN_BC_EVT,                  /*!< __Description:__ MCU����PGN�㲥 \n
                                                                        __Sender:__ Mcu_Diag \n
                                                                        __Receiver:__ Mpu_Emission \n
                                                                        __RequestData:__ DcpdPackageToMPUPGNBC_t \n
                                                                        __ResponseData:__ None \n
                                                                        */      
    SPP_USER_DCPD_MSG_TO_MPU_KMS_DM1_EVENT,							/*!< __Description:__ Mcu���Ϳ���˹DM1�����ϱ� \n
                                                                            __Sender:__ Mcu_ICT  \n
                                                                            __Receiver:__ MPU_KMSApp\n
                                                                            __RequestData:__ DcpdPackageKmsHBData_t \n
                                                                            __ResponseData:__ None \n
                                                                        */

    SPP_USER_DCPD_MSG_TO_MPU_XCALL_DIAL_FINISH_ACK_EVT,           /*!< __Description:__ MCU����ͨ�������¼��ظ� \n
                                                                            __Sender:__ Mcu_PM \n
                                                                            __Receiver:__ Mpu_XcallApp \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_XCALL_CODEC_SWITCH_RESP_EVT,           /*!< __Description:__ McU������Ƶͨ���л��ɹ��ظ� \n
                                                                            __Sender:__ Mcu_PM \n
                                                                            __Receiver:__ Mpu_XcallApp \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_XCALL_DIAL_REQ_EVT,           /*!< __Description:__ McU����XCALL��ECALL���绰�������� \n
                                                                            __Sender:__ Mcu_PM \n
                                                                            __Receiver:__ Mpu_XcallApp \n
                                                                            __RequestData:__ None \n
                                                                            __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_TSP_LOGIN_ACK_EVT,           /*!< __Description:__ McU����TSP��¼�ɹ���ϢӦ�� \n
                                                                                    __Sender:__ Mcu_PM \n
                                                                                    __Receiver:__ Mpu_XcallApp \n
                                                                                    __RequestData:__ None \n
                                                                                    __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MPU_XCALL_INFO_RESP_EVT \n
                                                                                */
	SPP_USER_DCPD_MSG_TO_MPU_KMS_HMI_UPDATA_EVENT,          /*!< __Description:__ Mcu���Ϳ���˹�궨��������״̬�����ϱ� \n
                                                                            __Sender:__ Mcu_ICT  \n
                                                                            __Receiver:__ MPU_KMSApp\n
                                                                            __RequestData:__ DcpdPackageKmsHBData_t \n
                                                                            __ResponseData:__ None \n
                                                                        */
    SPP_USER_DCPD_MSG_TO_MPU_KMS_HMI_UPDATA_SURE_EVENT,     /*!< __Description:__ HMI���͵��Ǳ������Լ�ack��Ӧ \n
                                                                        __Sender:__ Mcu_ICT  \n
                                                                        __Receiver:__ MPU_KMSApp\n
                                                                        __RequestData:__ DcpdPackageKmsHBData_t \n
                                                                        __ResponseData:__ None \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MPU_BTC_OTA_MSG_EVT,               /*!< __Description:__ ����������Կ��MPU \n
                                                                        __Sender:__ Mcu_btc  \n
                                                                        __Receiver:__ MPU_TspSrv\n
                                                                        __RequestData:__ DcpdPackageToMpuBtcOta_t \n
                                                                        __ResponseData:__ �ο�   @ref SPP_USER_DCPD_MSG_TO_MCU_BTC_OTA_MSG_ACK_EVT \n
                                                                    */
    SPP_USER_DCPD_MSG_TO_MPU_BTC_OTA_TX_RESULT_ACK_EVT,     /*!< __Description:__ Ӧ��������Կ���ͽ�� \n
                                                                        __Sender:__ Mcu_BTC  \n
                                                                        __Receiver:__ MPU_TspSrv\n
                                                                        __RequestData:__ None \n
                                                                        __ResponseData:__ None \n
                                                                    */

	SPP_USER_DCPD_MSG_TO_MPU_ICT_KMS_PRIVATECAN_STATUS_MSG_EVT, /*!< __Description:__ ����MPUKMS˽��CAN״̬ \n
                                                                        __Sender:__ Mcu_ICT  \n
                                                                        __Receiver:__ MPU_KMSAPP\n
                                                                        __RequestData:__ None \n
                                                                        __ResponseData:__ None \n
                                                                    */

    SPP_USER_DCPD_MSG_TO_MPU_WIFI_DVR_VID_ACK_EVT,             /*!< __Description:__ mcu����dvr������Զ����ƵӦ�� \n
                                                                    __Sender:__ Mpu_ICT \n
                                                                    __Receiver:__ Mpu_WifiApp    \n
                                                                    __RequestData:__  DcpdPackageToMpuWifiDvrVidResp_t \n
                                                                    __ResponseData:__ None  \n
                                                                */
	SPP_USER_DCPD_MSG_TO_MPU_CAN_LOG_EVT,                   /*!< __Description:__ MCU CAN Log��Ϣ \n
                                                                       __Sender:__ Mcu_ \n
                                                                       __Receiver:__ Mpu_RemoteLogSrv \n
                                                                       __RequestData:__ �ο� @ref DcpdPackageToMpuMcuLog_t  \n
                                                                       __ResponseData:__  \n
                                                               */ 
    SPP_USER_DCPD_MSG_TO_MPU_CTR_VEH_REQ_ACK_EVT,              /*!< __Description:__ �е�������ϢACK \n
                                                                       __Sender:__ Mcu_VC \n
                                                                       __Receiver:__ Mpu_TspSrv \n
                                                                       __RequestData:__   \n
                                                                       __ResponseData:__  \n
                                                               */ 
    SPP_USER_DCPD_MSG_TO_MPU_RESET_TBOX_ACK_EVT,                 /*!< __Description:__ ��λ����ACK \n
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
    UINT8 u8SrcSrvId;   /*!< Destinate Service Id �ο� @ref DcpdPackageAppSrvId_e*/
    UINT8 u8DstSrvId;   /*!< Destinate Service Id �ο� @ref DcpdPackageAppSrvId_e*/
    UINT16 u16EventId;    /*!< Event Id �ο� @ref DcpdPackageAppSrvId_e*/
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
    DcpdPackageNmeaInfo_t   tGnssData;  /*!< Gnss����*/
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
    UINT16   u16CSQ;  /*!<  �ź�ǿ�� CSQ���ο�Part4�ĵ�*/
    INT16   i16NadRssi;  /*!<  RSSI���ο�Part4�ĵ�*/
    UINT8   u8TechType;  /*!<  ������ʽ���ο�Part4�ĵ�:
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
    UINT8   u8SimUsed;  /*!<  ʹ�õ�SIM�����ο�Part4�ĵ�:
                            - 00-Internal
                            - 01-External 
                        */
    UINT8   u8NadReg;  /*!<  ע����Ϣ���ο�Part4�ĵ�:
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
    CHAR   strNadIp2[16];  /*!<  Tbox APN2 IP2���ο�Part4�ĵ�*/
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
    UINT8 u8Type;       /*!<  u8Type:����λ��ģ��:
                                            - 0: ����
                                            - 1: ��λModem
                                            - 2: ��λMCU
                                            - 3: ��λMPU
                                            - 4: ��λBLE
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
 UINT8 PowerMode;           /*!<  TBOX��Դ����ģʽ��
                                    - 0x00��Ԥ����
                                    - 0x01��RUNNINGģʽ��
                                    - 0x02��SLEEPL1ģʽ��
                                    - 0x03��SLEEPL2ģʽ��
                                    - 0x04��SLEEPL3ģʽ��
                                    - 0x05��BACKBATģʽ��
                            */
UINT8 u8BattVoltOperASta;   /*!<  ��ͨ���ܵ�ѹ״̬��
                                    - 0x00��������
                                    - 0x01����ѹ��
                                    - 0x02����ѹ��
                                    - 0x03����Ч��
                            */
UINT8 u8BattVoltOperCANASta;  /*!<  CAN����ͨ�Ź��ܵ�ѹ״̬��
                                    - 0x00��������
                                    - 0x01����ѹ��
                                    - 0x02����ѹ��
                                    - 0x03����Ч��
                           */
UINT8 u8BattVoltOperDiagASta;  /*!<  A��������Ϲ��ܵ�ѹ״̬(Ԥ��)��
                                    - 0x00��������
                                    - 0x01����ѹ��
                                    - 0x02����ѹ��
                                    - 0x03����Ч��
                           */
UINT8  u8KeySta;           /*!< ���Կ��״̬��
                                    - 0x00��OFF
                                    - 0x01��ACC
                                    - 0x02��RUN
                                    - 0x03��CRANK
                            */
UINT8  u8KL15;           /*!< KL15״̬��
                                    - 0x00��OFF
                                    - 0x01��ON
                            */
UINT8  u8KL30;           /*!< KL30״̬��
                                    - 0x00��OFF
                                    - 0x01��ON
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
    UINT8 u8ExtFailReason;    /*!< ����ϸ�����֮�������ʧ��ԭ��:
                                - 0x00  ��ʧ��ԭ��
                                - 0x01  �ⲿ��Ϲ�������
                                - 0x02  ����������
                                - 0x03  ���Կ�״���Crank
                                - 0x04  ���ٴ���4KM/H
                                - 0x05  ���ܵ�ѹ�쳣
                              */
    UINT8  u8Num;                /*!< ��ǰ��*/
    UINT8  u8TotalNum;           /*!< �ܰ���*/
    UINT32 u32CanID;                /*!< ��ϵ�CAN ID    */
    UINT16 u16CanRespMsgLen;        /*!< ��Ӧ����ϱ�����Ч����          */
    UINT16 u16PayloadLen;           /*!< ��ǰ������*/
    UINT8 u8ArrDiagRspMsg[255];     /*!< ��Ӧ����ϱ��ģ����㲿����0x00*/

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
    UINT16 u16Cmd;              /*!< ָ�� :
                                    - 0 ��ѯMPU�Ƿ��յ�Sleep�ź�
                                    - 1 ��ѯMPU�Ƿ��յ�Wakeup�ź�
                                    - 2 ����MPU��������MCU�ź�
                                    - 3 ����MPU����Ƶ�ػ�
                                    - 4 ����MPU�ر���Ƶ�ػ�
                                    - 5 ����MPU��ѯSIM��״̬
                                    - 6 ��ѯTbox��5����Ϣ��IMEI+IMSI+MEID+ICCID+SIM���绰���룩
                                    - 7 �������ģʽ
                                    - 8 �˳�����ģʽ
                                */
    UINT8 u8ArrPara[200];     /*!< ָ���Ӧ�Ĳ�����
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
    UINT32 u32McuSysTime;         /*!< 0xFFFFFFFF��ʾMCUϵͳʱ����Ч            */
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
    UINT32 u32EventID;         /*!< u32EventID���¼�ID��Ԥ��*/
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
    UINT8 u8Result;                 /*!<    MPU�Ƿ�������ߣ�
                                            - 0x00���������������㣬���������ߣ�
                                            - 0x01�������������㣬�������ߣ�
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
    UINT8 u8PowerMode;             /*!<  TBOX��Դ����ģʽ��
                                    - 0x00��Ԥ����
                                    - 0x01��RUNNINGģʽ��
                                    - 0x02��SLEEP1ģʽ��
                                    - 0x03��SLEEP2ģʽ��
                                    - 0x04��SLEEP3ģʽ��
                                    - 0x05��BATBACKģʽ��
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
    UINT8 u8SimSta;       /*!<  SIM��״̬ :
                                - 0x00��SIM������
                                - 0x01��SIM��������
                         */
    UINT8 u8SimFltSta;    /*!<  SIM������״̬��
                                - 0x00��SIM���޹���
                                - 0x01��SIM���й���
                          */
    UINT32 u32SimExtInfo;    /*!<  SIM����Ϣ�����ֶ�Ŀǰ��0��Ԥ��*/
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
    UINT8 u8ReqSrc;              /*!< ������Դ:
                                    - 0x00: FOTA(FOTA����ʱ������������ж�)
                                    - 0x01: Remote Diag
                                 */
    UINT8  u8Num;                /*!< ��ǰ��*/
    UINT8  u8TotalNum;           /*!< �ܰ���*/
    UINT32 u32CanID;             /*!< ���CAN ID */
    UINT16 u16DiagMsgLen;        /*!< ��ϱ��ĳ���          */
    UINT16 u16PayloadLen;        /*!< ��������          */
    UINT8 u8ArrDiagMsg[255];     /*!< ��ϱ��ģ����㲿����0x00*/
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
    UINT16 u16Cmd;              /*!< ָ�� :
                                    - 0 ��ѯMPU�Ƿ��յ�Sleep�ź�
                                    - 1 ��ѯMPU�Ƿ��յ�Wakeup�ź�
                                    - 2 ����MPU��������MCU�ź�
                                    - 3 ����MPU����Ƶ�ػ�
                                    - 4 ����MPU�ر���Ƶ�ػ�
                                    - 5 ����MPU��ѯSIM��״̬
                                    - 6 ��ѯTbox��5����Ϣ��ICCID+IMEI+IMSI+MEID+SIM���绰���룩
                                    - 7 �������ģʽ
                                    - 8 �˳�����ģʽ
                                */
    UINT8 u8ArrPara[200];     /*!< ָ���Ӧ�Ĳ�����
                                    - CMD=0 �ο� @ref DcpdPackageToMcuEolSleepSigResp_t
                                    - CMD=1 �ο� @ref DcpdPackageToMcuEolWakeupSigResp_t
                                    - CMD=2 �ο� @ref DcpdPackageToMcuEolWakeupMcuResp_t
                                    - CMD=3 �ο� @ref DcpdPackageToMcuEolOpenAudioLpResp_t
                                    - CMD=4 �ο� @ref DcpdPackageToMcuEolCloseAudioLpResp_t
                                    - CMD=5 �ο� @ref DcpdPackageToMcuEolSimStaResp_t
                                    - CMD=6 �ο� @ref DcpdPackageToMcuEol5CodeResp_t
                                    - CMD=7 �ο� @ref DcpdPackageToMcuEolEnterFlightModeResp_t
                                    - CMD=8 �ο� @ref DcpdPackageToMcuEolExitFlightModeResp_t
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
    UINT32 u32MpuSysTime;         /*!< 0xFFFFFFFF��ʾMPUϵͳʱ����Ч            */
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
    UINT32 u32ReqId;  /*!<  u32ReqId������ID��RVC�ݴ�ȷ�������͵���ʧ�Ƿ����ʹ�*/
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
    UINT16 u16SerNum;               /*!< ��ˮ��*/
    BOOL bIsSms;                    /*!<  TRUE:ͨ��SMS���գ�FALSE:ͨ��Socket����*/
    UINT8 u8TotalNum;               /*!< ���󳵿صĲ����ܸ���*/
    UINT8 u8Num;                    /*!< ���δ���Ĳ�������*/
    UINT8 u8ArrPara[256];           /*!< DID1+DID1Para+����+DIDn+DIDnPara*/
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
    UINT16 u16SerNum;               /*!< ��ˮ��*/
    UINT32 u32DcpdReqId;            /*!< ����ID��MPU���ط��ͽ��ʱ��ʹ�����ID����ʶ*/
    BOOL bIsSms;                /*!<  TRUE:����ʹ��SMS����*/
    UINT8 u8Num;                /*!<  �����������*/
    UINT8 u8ArrResult[256];     /*!<  DID1+DID1Result+����+DIDn+DIDnResult*/
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
    UINT32 u32ReqId;  /*!<  ���������ID*/
    UINT8 u8Result;  /*!<  ���ͽ��:
                        -  0:���ͳɹ�
                        -  ����������ʧ��
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
    UINT8 u8FixStatus;  /*!< ��λ״̬ :
                                -  0:δ��λ
                                -  1:��λ�ɹ�
                        */
    INT32 i32Longitude; /*!< γ��;   0.000001 Degrees ; -180000000 to 180000000;��Ϊ��������Ϊ����*/
    INT32 i32Latitude;  /*!< ����;  0.000001 Degrees ; -90000000 to 90000000; ��Ϊ��γ����Ϊ��γ */
    UINT8 u8TimeStamp[12];  /*!< ʱ��: YYMMDDHHMMSS */
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
    UINT8 u8WanConnectState;    /*!< Wan����״̬:
                                     -  1:connecting
                                     -  2:2G
                                     -  3:3G
                                     -  4:4G
                                */
    UINT8 u8PublicIp[16];   /*!< ����IP*/
    UINT8 u8Csq;            /*!< CSQֵ*/
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
        UINT8 tDataBuff[8]; /*!< 8���ֽڵ�Buffer MPU�յ���ԭ������*/
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
    UINT8 u8State;                  /*!<  PING�����:
                                        -  1:�ɹ�
                                        -  ������ʧ��
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
    UINT8  u8Sta;           /*!< MPU�Ƿ��յ�Sleep�źű�־:
                                - 0 û���յ�Sleep�ź�
                                - 1 �յ�Sleep��Ϣ
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
    UINT8  u8Sta;           /*!< MPU�Ƿ��յ�Wakeup�źű�־:
                                - 0 û���յ�Wakeup�ź�
                                - 1 �յ�Wakeup��Ϣ
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
    UINT8  u8Sta;           /*!< MPU�Ƿ񷢳�����MCU�ź�:
                                - 0 û�з���
                                - 1 �ѷ���
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
    UINT8  u8Sta;           /*!< MPU�Ƿ����Ƶ�ػ�:
                                - 0 ��ʧ��
                                - 1 �򿪳ɹ�
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
    UINT8  u8Sta;           /*!< MPU�Ƿ�ر���Ƶ�ػ�:
                                - 0 �ر�ʧ��
                                - 1 �رճɹ�
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
    UINT8  u8Sta;           /*!< SIM��״̬:
                                - 0 SIM�������ӣ�����
                                - 1 SIM�������ӣ�����״̬�쳣
                                - 2 SIM��δ����
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
    UINT8  u8IccidLen;           /*!< ICCID��Ч����     */
    UINT8 u8ArrIccid[21];        /*!< ICCID���������0x00     */
    UINT8  u8ImeiLen;           /*!< ICCID��Ч����     */
    UINT8 u8ArrImei[33];        /*!< IMEI���������0x00     */
    UINT8  u8ImsiLen;           /*!< IMSI��Ч����     */
    UINT8 u8ArrImsi[33];        /*!< IMSI���������0x00     */
    UINT8  u8MeidLen;           /*!< MEID��Ч����     */
    UINT8 u8ArrMeid[33];        /*!< MEID���������0x00     */
    UINT8  u8SimPhNoLen;           /*!< �绰������Ч����     */
    UINT8 u8ArrSimPhNo[33];        /*!< �绰���룬�������0x00     */
}DcpdPackageToMcuEol5CodeResp_t;


/*!
typedef struct \n
{ \n
    UINT8  u8Result;           \n
}DcpdPackageToMcuEolEnterFlightModeResp_t;    \n
*/
typedef struct
{
    UINT8  u8Result;           /*!< �������ģʽ�Ľ����
                                    - 0    �ɹ�;  
                                    - ���� ʧ����     */
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
    UINT8 u8DidNum;                 /*!< ����DID�ĸ���n     */
    UINT8 u8ArrDid[512];            /*!< DID1+DIDData+����+DIDn+DIDDatan*/
} DcpdPackageToMcuCfgSvReq_t;

/*!
typedef struct \n
{ \n
    UINT8  u8Result;           \n
}DcpdPackageToMcuEolExitCFunResp_t;    \n
*/
typedef struct
{
    UINT8  u8Result;           /*!< �˳�����ģʽ�Ľ����
                                - 0    �ɹ�;
                                - ���� ʧ����     */
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
    UINT8 u8DidTotalNum;            /*!< DID���ܸ���n     */
    UINT8 u8DidNum;                 /*!< ����DID�ĸ���n     */
    UINT8 u8ArrDid[512];            /*!< DID1+DIDData+����+DIDn+DIDDatan*/
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
    UINT8 u8DidNum;                 /*!< ����DID�ĸ���n     */
    UINT8 u8ArrDid[512];            /*!< DID1+DIDData+����+DIDn+DIDDatan*/
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
    UINT8 u8ReqType;                 /*!< �������ͣ�
                                        - 0 �ر�WIFI
                                        - 1 ����WIFI
                                        - 2 ����Socket��
                                        - 3 ����Psw/SSID
                                    */
    UINT8 u8SrcType;                
            /*!< wifi������Դ:
              - 0 DVR����
              - 1 MP5����
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
    UINT8 u8DidNum;                 /*!< ����DID�ĸ���n     */
    UINT8 u8ArrResult[128];           /*!< DID1+DID1Result+����+DIDn+DIDResult*/
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
    UINT8 u8KL15;                   /*!< KL15״̬:
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
    UINT8 u8KL15;                   /*!< KL15״̬:
                                            - 0    OFF;
                                            - 1    ON   
                                    */
    UINT32 u32Vbat;                 /*!< ��ص�ѹ(��λ: 0.1v)     */
    UINT8 u8EpbSt;                  /*!< ��ɲ״̬ */
    UINT16 u16EngSpd;               /*!< ������ת��(1rpm)*/
    UINT8 u8SecSt;                  /*!< ���״̬*/
	UINT8 u8Result;                 /*!< �жϽ����bit0:bit4��1�ֱ��ʾ��Ӧ����������              */
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
    UINT8 u8Result;                   /*!< ������:
                                         - 0    ʧ��;
                                         - 1    �ɹ�   
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
    UINT8  u8Result;           /*!< �ն�ע����:
                                - 0 ��ʧ��
                                - 1 �򿪳ɹ�
                            */
    UINT8 u8AuthCodeLen;        /*!<��Ȩ����Ч����*/
    UINT8 u8AuthCode[16];       /*!<��Ȩ�룬�16�ֽڣ����㲿�����0x00*/
    UINT8 u8WorkMd;             /*!<����ģʽ 0�������ڣ�1����ʽ��*/
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
    UINT8  u8Type;           /*!< ����:
                                - 0 �ر�WIFI�Ľ��
                                - 1 ��WIFI�Ľ��
                                - 2 SOCKET��������
                                - 3 ����ssid/psw
                            */
    UINT8 u8OpResult;        /*!<WIFI���ز������
                                - TRUE �ɹ�
                                - FALSE ʧ��
                            */
    UINT8 u8SocketCode[8];   /*!<u8TypeΪ2ʱ��Ч*/

	UINT8 u8WifiSSID[16];    /*Wifi����*/
	
	UINT8 u8WifiPsw[16];	 /*Wifi����*/
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
    UINT8  u8Type;           /*!< ����Ȧ����:
                                - 0 Բ��
                                - 1 ����
                            */
    UINT8 u8Attr[34];       /*!<����Ȧ����������u8Type���������㲿�����0x00*/
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
    UINT8 u8ArrTrig[8];   /*!< ����Դ:
                                    - bit0-bit63��Ӧ "�� 3.6.29.3 1 �����ϱ�������"  */
    UINT8 u8DidNum;             /*!<DID����*/
    UINT16 u16DataLen;          /*!<DID Data Len*/
    UINT8 u8DidData[512];       /*!<DID+DIDData���������㲿�����0x00*/
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
    UINT8 u8ArrDID603D[8];   /*!< �ش�����Ϣ���ο�����������TLV��ֵ�Ա��  */
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
    UINT8 u8EmmcSta;                   /*!< Emmc״̬:
                                         - 0    Emmc�������޹���;
                                         - 1    Emmc�������й���
                                         - 2    Emmc������
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
    UINT8 u8WifiSta;                   /*!< Wifi״̬:
                                         - 0    ����;
                                         - 1    δ����;   
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
    UINT8 u8TspSta;                   /*!< Tsp����״̬:
                                         - 0    ����;
                                         - 1    δ����   
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
    UINT8 u8Len;                   /*!< �绰���볤�� */
    UINT8 u8PhNumArry[32];           /*!< �绰������Ϣ */
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
    UINT8 u8Type;                   /*!< wifi�����¼�:
                                      - 0 ����ر�WIFI
                                      - 1 �����WIFI
                                      - 2 ����socket��
                                      - 3 ����SSID��Psw
                                    */
    UINT8 u8SrcType;                /*!< wifi������Դ:
                                      - 0 DVR����
                                      - 1 MP5����
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
	UINT16 u16Spd;         /*!< ����  */
	UINT8 u8AirPre;        /*!< ����ѹ��   */
	UINT8 u8EngTorque;     /*!< �����������Ť��   */  
	UINT8 u8FrictionTor;  /*!< Ħ��Ť��   */
	UINT16 u16EngSpd;      /*!< ������ת��   */
	UINT16 u16EngFuelFlow; /*!< ������ȼ������   */
	UINT16 u16NOxSensorUp; /*!< SCR����Nox���������ֵ  */
    UINT16 u16NOxSensorDown;    /*!< SCR����Nox���������ֵ  */
    UINT8  u8Reagentresidual;   /*!< ��Ӧ������   */
    UINT16 u16AirInflow;        /*!< ������  */
    UINT16 u16SCRTmpIN;         /*!< SCR����¶�    */
    UINT16 u16SCRTmpOUT;        /*!< SCR�����¶� */
    UINT16 u16DPF;              /*!< DPFѹ��  */
    UINT8 u8EngCoolantTmp;      /*!< ��������ȴҺ�¶�  */
    UINT8 u8FuelLevel;          /*!< ����Һλ*/
    UINT32 u32TotalMileage;     /*!< �ۼ����*/
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
    UINT8 u8HarWaredNumLen;     /*!< Ӳ���ų���*/
    UINT8 pu8HarWaredNum[15];   /*!< Ӳ����*/
    UINT8 u8SolftWaredNumLen;    /*!< ����ų���*/
    UINT8 pu8SoftWareNum[15];   /*!< �����*/
    UINT8 u8TBOXIDLen;         /*!< �ն˺���ID����*/
    UINT8 pu8TBOXID[26];        /*!< �ն˺���ID*/
    UINT8 u8FMSta;               /*!< ����״̬*/
    UINT8 u8HMIType;            /*!<HMI����*/
    UINT32  u32VehDistance;     /*!< ���������*/
    UINT8 u8ProtocalType;       /*!< ����ͨ��Э�飬Ĭ��0*/
    UINT8   u8ReCalibEnable;/*!< ������Զ�̱궨�����������ƿ���*/
    UINT8  pu8ReCalibECMCode[10];/*!< ������Զ�̱궨 ECM Code*/
    UINT8  pu8RmCalibECMMake[5];/*!< ������Զ�̱궨 ECM Make*/
    UINT8  pu8RmCalibECMVIN[17];/*!< ������Զ�̱궨 ECM VIN*/
    UINT8  pu8RmCalibECMSn[8];/*!< ������Զ�̱궨 ECM SN*/
	UINT32 u32ECMSourceAddress; /*ECMԴ��ַ��*/
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
    UINT8 pu8FMinFor[183]   ; /*!< ������Ϣ(����0x604C��ȡ) 
                                - ��ϢID 0x604C (16BIT)
                                - ��Ϣ���ݳ�WORD(16BIT)
                                - ������Զ�̱궨 ECM VIN(8BIT * 17)
                                - ������Զ�̱궨 ECM SN(8BIT * 8)
                                - ���Ͻڵ����(8BIT)
                                - ���Ͻڵ�1Դ��ַ(8BIT)
                                - ���Ͻڵ�1����������(8BIT)
                                - ���Ͻڵ�1���ϵ���Ϣ(16BIT)
                                - ���Ͻڵ�1���������(8BIT)
                                
                                - ���Ͻڵ�1������1 SPN(24BIT)
                                - ���Ͻڵ�1������1 FMI (8BIT)
                                - ���Ͻڵ�1������1 OC&CM (8BIT)
                                - ���Ͻڵ�1������1 ����(8BIT)
                                - ���Ͻڵ�1������2...
                                */
    UINT8 pu8HBNotSample[800]; /*!< ����˹��������
                                - ��������(16BIT)
                                - ����1 SPNID(32BIT)
                                - ����1 SPN���ݳ���(8BIT)
                                - ����1 SPN����(N)
                                - ����N SPNID
                                - ����N SPN���ݳ���
                                - ����N SPN����*/ 

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
    UINT8 u8Apn16Sta;   /*!< ˽��1������״̬ */
    UINT8 u8WkpCnt1;    /*!< 30min�ڵĻ��Ѵ��� */
    UINT8 u8WkpCnt2;    /*!< �ܵĻ��Ѵ��� */
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
    UINT8 au8TspApnAddr[32];        /*!< Tsp������APN��Ϣ*/
    UINT8 au8TspApnUsr[24];         /*!< Tsp������APN�û���*/
    UINT8 au8TspApnPsw[24];         /*!< Tsp������APN����*/
    UINT8 au8Mp5ApnAddr[32];        /*!< MP5��������APN��Ϣ*/
    UINT8 au8Mp5ApnUsr[24];         /*!< MP5��������APN�û���*/
    UINT8 au8Mp5ApnPsw[24];         /*!< MP5��������APN����*/
    UINT8 au8OtaApnAddr[32];        /*!< OTA����APN��Ϣ*/
    UINT8 au8OtaApnUsr[24];         /*!< OTA����APN�û���*/
    UINT8 au8OtaApnPsw[24];         /*!< OTA����APN����*/
    UINT8 au8TspIp[32];             /*!< Tsp��������ַ*/
    UINT8 au8OtaIp[32];             /*!< Ota��������ַ*/
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
    UINT8 u8ReportType;             /*!< �ϱ����� */
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
    UINT16 u16VehSpeed;  /*!< ����*/
    BOOL bVehGearStatus; /*!< ��λ״̬*/
    BOOL bVehEpbStatus; /*!< ������ɲ״̬*/
    BOOL bVehBatteryStatus;/*!< ���״̬*������Դ��*/
    UINT8 u8VehBatteryLevel; /*!< ���������ٷֱȣ�����Դ��*/
    UINT8 u8VehAccBatteryLevel;/*!< ���ص����ٷֱ�*/
    UINT8 u8VehAccVoltage;/*!< ��ص�ѹ*/
    BOOL bEngRunSta;/*!< ����������״̬*/
    UINT8 u8KeySta; /*!< Կ��״̬*/
	UINT8 u8SecSta; /*!< ���״̬*/
    UINT32 u32VehConDaValid;  /*!< �ź���Ч��*/    
}DcpdPackageToMpuVehConData_t;


typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */  
    UINT8 u8CtrType;                 /*!< ��������
                                        -0���µ�
                                        -1���ϵ�*/
    UINT8 u8CtrRst;          			/*!<�е���ƽ��
                                       -0�����Ƴɹ�
                                       -��0������ʧ���루RVC���壩
             							0x1:��������OFF�����δ���
										0x2:ESCL����ʧ��
										0x3:PEPSδ����
										0x7:����*/
                                       
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
	UINT8 u8ReqSrc; 			 /*!< PGN����Դ */ 
	UINT32 u32PGN;				 /*!< PGN */ 
}DcpdPackageToMcuPGNReq_t;
	
typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
	UINT8 u8ReqSrc; 			   /*!< PGN����Դ */
	UINT8 u8Result; 			   /*!< ��Ӧ���*/
	UINT16 u16DataLen;			   /*!< PGN���ݳ���*/
	UINT32 u32PGN;				   /*!< PGN*/
	UINT8 pu8Data[255]; 		   /*!< ������Ӧ*/
}DcpdPackageToMPUPGNRsp_t;
	
	
typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
	UINT16 u16DataLen;			   /*!< PGN���ݳ���*/
	UINT32 u32PGN;				   /*!< PGN*/
	UINT8 pu8Data[255]; 		   /*!< ������Ӧ*/
}DcpdPackageToMPUPGNBC_t;

typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
	UINT8 LoninState; 		   /*!< 0:Not login 1:has login */
}DcpdPackageToMCUTspLoginState_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */	
    UINT8 u8CodecSwt;               /*!< Codec����*/              
}DcpdPackageToMcuCodecSwt_t;

/****************HMI  data****************************/
typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */	
    UINT8   u8BatteryVoltage;               //��ص�ѹ
    UINT16  u16Speed;                       //����
    UINT16  u16EngineSpeed;                 //������ת��
    UINT8   u8ParkingBrake;                 //פ���ƶ����״̬
    UINT8   u8KeySwitch;                    //Կ�׿���
}DcpdHMIUpdataCondition_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;   /*!< tHeader */	
    UINT8 u8SelectResults;                  //�û���ѡ����
    UINT8 u8InstallationProgress;           //������װ����Ӧ��
    UINT8 u8InstallationResults;            //�����Ľ��Ӧ��
    UINT8 u8EquipmentIdentification;        //HMI�豸��ʶ
}DcpdHMIReceiveData_t;
typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
	UINT8   u8SPN1;                     //�궨����(1)
    UINT8   u8SPN3;                     //ѡ����Ӧ��(3)
    UINT8   u8SPN5;                     //��װ����(5)
    UINT8   u8SPN8;                     //��װ���(8)
    UINT8   u8SPN6;                    //��װ����(6)
    UINT8   u8SPN4;                     //��������(4)
    UINT8   u8SPN10;                    //����ֹͣ(10)
    UINT8   u8SPN11;                    //פ���ƶ�(11)
    UINT8   u8SPN12;                    //������ͣ��(12)
    UINT8   u8SPN13;                    //Կ�׿���(13)
    UINT8   u8SPN14;                    //��ص�ѹ(14)
    UINT8   u8SPN15;                    //�����ź�(15)
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
    UINT8 u8Result;       /*!< 0���ɹ���������ʧ�� */
}DcpdPackageToMcuBtcOtaTxResult_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< tHeader */
    UINT8 u8OpReq;                   /*!< Զ��DVR��Ƶ����:
                                      - reserve, ����
                                    */
}DcpdPackageToMcWifiDvrVidReq_t;

typedef struct
{
    DcpdPackageHeader_t tHeader;    /*!< Dcpd Package Header*/
    UINT8 u8OpResult;        /*!< �������
                                - TRUE �ɹ�
                                - FALSE ʧ��
                            */
}DcpdPackageToMpuWifiDvrVidResp_t;

typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */	
    // UINT32 WarningInfo1_timestamp;
	UINT16 ICT_Warning_FAS_LaneTrackingStatusRightSide;                     //������ƫ��
    UINT16 ICT_Warning_FAS_LaneTrackingStatusLeftSide;                      //������ƫ��
    UINT16 ICT_Warning_FAS_DSM_Warning;                                     //˾��ƣ��Ԥ��
    UINT16 ICT_Warning_FAS_HW_Status;                                       //������Ԥ��
    UINT16 ICT_Warning_FCW_Alarm_Status;                                    //FCW��ײԤ��
    UINT16 ICT_Warning_OGP_Forward_Collision_Warning;                       //
    UINT16 ICT_Warning_OGP_Advanced_emergency_braking_system_state;         //AEBS��ɲԤ��
    UINT16 ICT_Warning_OGP_Advanced_emergency_braking_system_state2;        //AEBS�����ƶ�
}DcpdWarningInfo1_t;//��Ԥ����ء�2020.4.25�ύ����ͨ���ζ��Ʋ��ԣ�һ�ڣ�0x6058

typedef struct
{
	DcpdPackageHeader_t tHeader;   /*!< tHeader */
    // UINT32 WarningInfo2_timestamp;	
	UINT8 ICT_Warning_AEBS_Switch;                  //AEBS����
    UINT8 ICT_Warning_LDW_Main_Switch;              //����ƫ��Ԥ�����ܿ���
    UINT8 ICT_Warning_FCW_Main_Switch;              //ǰ��ײԤ�����ܿ���
    UINT8 ICT_Warning_DSM_Main_Switch;              //ƣ��Ԥ�����ܿ���
    UINT8 ICT_Warning_ACCUsageDemand;		        //ACC����
}DcpdWarningInfo2_t;//������״̬��ء�2020.10.8�ύ����ͨ���ζ��Ʋ��ԣ����ڣ���TypeID(WORD)

#ifdef __cplusplus
}
#endif

#pragma pack()


#endif /*MPU_SPP_H*/



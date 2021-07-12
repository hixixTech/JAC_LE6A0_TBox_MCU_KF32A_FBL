/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    mem_ee_id.h
 * DESCRIPTION:
 * 
 * DATE BEGUN:   2021/06/01
 * BY:           feifei.xu
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

#ifndef __MEM_EE_ID_H__
#define __MEM_EE_ID_H__

/*****************************************************************************
**#include 
*****************************************************************************/




/*****************************************************************************
**  typedef
*****************************************************************************/

/*****************************************************************************
** Constant Macro Definition
*****************************************************************************/
#define EE_WR_STEPS     (8) /*****Double Word*****/

/****************************************************
 *      EEPROM ID and Length
 ****************************************************/
#define EEPID_DIAGDID_0xF186            0x0001 /* （1）Active Diag Session 当前诊断会话模式 +（2）CRC*/
#define EEPID_DIAGDID_0xF186_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF187            0x0002 /* （12）JAC Spare Part Number JAC零件号码 +（2）CRC */
#define EEPID_DIAGDID_0xF187_LEN        2*EE_WR_STEPS

#define EEPID_DIAGDID_0xF188            0x0003 /* （15）JAC ECU Software Number JAC零部件软件号 +（2）CRC */
#define EEPID_DIAGDID_0xF188_LEN        3*EE_WR_STEPS

#define EEPID_DIAGDID_0xF189            0x0004 /* （5）JAC ECU Software Version JAC零部件软件版本号 +（2）CRC */
#define EEPID_DIAGDID_0xF189_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF18A            0x0005 /* （5）ECU Supplier Identifier ECU供应商号码 +（2）CRC */
#define EEPID_DIAGDID_0xF18A_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF18B            0x0006 /* （4）ECU Manufacture Date ECU生产日期 +（2）CRC */
#define EEPID_DIAGDID_0xF18B_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF18C            0x0007 /* （26）ECU Serial Number ECU序列号+（2）CRC */
#define EEPID_DIAGDID_0xF18C_LEN        4*EE_WR_STEPS

#define EEPID_DIAGDID_0xF190            0x0008 /* （17）Vehicle Identification Number车辆识别编号 +（2）CRC */
#define EEPID_DIAGDID_0xF190_LEN        3*EE_WR_STEPS

#define EEPID_DIAGDID_0xF192            0x0009 /* （15）Supplier ECU Hardware Number 供应商ECU硬件号 +（2）CRC */
#define EEPID_DIAGDID_0xF192_LEN        3*EE_WR_STEPS

#define EEPID_DIAGDID_0xF193            0x000A /* （5）Supplier ECU Hardware Version 供应商ECU硬件版本号 +（2）CRC */
#define EEPID_DIAGDID_0xF193_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF194            0x000B /* （15）Supplier ECU Software Number 供应商ECU软件号 +（2）CRC */
#define EEPID_DIAGDID_0xF194_LEN        3*EE_WR_STEPS

#define EEPID_DIAGDID_0xF195            0x000C /* （5）Supplier ECU Software Version 供应商ECU软件版本号 +（2）CRC */
#define EEPID_DIAGDID_0xF195_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF197            0x000D /* （6）System Name or Engine Type 系统名称或发动机类型 +（2）CRC */
#define EEPID_DIAGDID_0xF197_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF198            0x000E /* （3）Tester Serial Number诊断仪连续号 +（2）CRC */
#define EEPID_DIAGDID_0xF198_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF199            0x000F /* （4）Programming Date 编程日期 +（2）CRC */
#define EEPID_DIAGDID_0xF199_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF19D            0x0014 /* （4）ECU Installation Date ECU安装日期 +（2）CRC */
#define EEPID_DIAGDID_0xF19D_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF1A8            0x0016 /* （20）Vehicle Feature Code车辆特征代码 +（2）CRC */
#define EEPID_DIAGDID_0xF1A8_LEN        3*EE_WR_STEPS

#define EEPID_FLHVLD                    0x0017 /* （1）F1FA 软件完整性状态 +（1）F1FB 软件兼容性状态 +（1）F1FC 软件有效标志 +（64）flashing validation area（bootloader内部使用）*/
#define EEPID_FLHVLD_LEN                9*EE_WR_STEPS

/* 0x0018~0x02FF，此ID段供DID-Internal使用 */

#define EEPID_NVID_0xE001               0x0018 /* （20）终端软件版本号，总软件包版本号 +（2）CRC */
#define EEPID_NVID_0xE001_LEN           3*EE_WR_STEPS

#define EEPID_NVID_0xE002               0x0019 /* （20）参数版本号 +（2）CRC */
#define EEPID_NVID_0xE002_LEN           3*EE_WR_STEPS

#define EEPID_NVID_0xE003               0x001A /* （16）鉴权码 +（2）CRC */
#define EEPID_NVID_0xE003_LEN           3*EE_WR_STEPS

#define EEPID_NVID_0xE004               0x001B /* （20）终端MCU软件版本号 +（2）CRC */
#define EEPID_NVID_0xE004_LEN           3*EE_WR_STEPS

#define EEPID_NVID_0xE005               0x001C /* （20）MPU软件版本号 +（2）CRC */
#define EEPID_NVID_0xE005_LEN           3*EE_WR_STEPS

#define EEPID_NVID_0xE006               0x001D /*（17）预留地址 +（2）CRC*/
#define EEPID_NVID_0xE006_LEN           3*EE_WR_STEPS

#define EEPID_NVID_0xE007               0x001E /*（26）预留地址 +（2）CRC*/
#define EEPID_NVID_0xE007_LEN           4*EE_WR_STEPS

#define EEPID_NVID_0xE008               0x001F /*（20）预留地址 +（2）CRC*/
#define EEPID_NVID_0xE008_LEN           3*EE_WR_STEPS

#define EEPID_NVID_0xE009               0x0020 /*（4）预留地址 +（2）CRC*/
#define EEPID_NVID_0xE009_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE00A               0x0021 /*（4）预留地址 +（2）CRC*/
#define EEPID_NVID_0xE00A_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE00B               0x0022 /*（4）预留地址 +（2）CRC*/
#define EEPID_NVID_0xE00B_LEN           1*EE_WR_STEPS

#define EEPID_CFGDID_0x1001             0x0023 /*（2）终端心跳发送间隔 + （2）CRC*/
#define EEPID_CFGDID_0x1001_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1002             0x0024 /*（2）TCP 消息应答超时时间 + （2）CRC */
#define EEPID_CFGDID_0x1002_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1003             0x0025 /*（64）ECall号码 + （2）CRC*/
#define EEPID_CFGDID_0x1003_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1004             0x0026 /*64）BCall号码(最大长度63BYTE) + （2）CRC*/
#define EEPID_CFGDID_0x1004_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1005             0x0027 /*（64）TSP 使用 URL 1（使用808主服务器地址IP或域名） + （2）CRC*/
#define EEPID_CFGDID_0x1005_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1006             0x0028 /*（2）TSP 使用 URL 1（使用808服务器TCP端口）+ （2）CRC*/
#define EEPID_CFGDID_0x1006_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1007             0x0029 /*（2）定时上报时间间隔 + （2）CRC*/
#define EEPID_CFGDID_0x1007_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1008             0x002A /* （32）Wifi 用户名+ （2）CRC*/
#define EEPID_CFGDID_0x1008_LEN         5*EE_WR_STEPS

#define EEPID_CFGDID_0x1009             0x002B /*（64）Wifi 密码 + （2）CRC*/
#define EEPID_CFGDID_0x1009_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1010             0x002C /*（1）Wifi 开关 + （2）CRC*/
#define EEPID_CFGDID_0x1010_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1012             0x002D /*（64）TSP 使用 URL 2（使用808主服务器地址IP或域名） + （2）CRC*/
#define EEPID_CFGDID_0x1012_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1013             0x002E /*（2）TSP 使用 URL 2（使用808服务器TCP端口）+ （2）CRC*/
#define EEPID_CFGDID_0x1013_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1014             0x002F /*（64）TSP服务器APN信息 + （2）CRC*/
#define EEPID_CFGDID_0x1014_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1015             0x0030 /*（64）MP5主机上网APN信息 + （2）CRC*/
#define EEPID_CFGDID_0x1015_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1016             0x0031 /*（64）OTA上网APN信息+ （2）CRC*/
#define EEPID_CFGDID_0x1016_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1018             0x0032 /*（64）ICall号码 + （2）CRC*/
#define EEPID_CFGDID_0x1018_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1019             0x0033 /*（64）短信中心号码 + （2）CRC*/
#define EEPID_CFGDID_0x1019_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1023             0x0034 /*（64）数据上传FTP服务器地址 + （2）CRC*/
#define EEPID_CFGDID_0x1023_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1024             0x0035 /*（2）数据上传FTP服务器端口 + （2）CRC*/
#define EEPID_CFGDID_0x1024_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1025             0x0036 /*（64）数据上传FTP服务器用户名 + （2）CRC*/
#define EEPID_CFGDID_0x1025_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1026             0x0037 /*（64）数据上传FTP服务器密码 + （2）CRC*/
#define EEPID_CFGDID_0x1026_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1027             0x0038 /*（1）定时上报状态 + （2）CRC*/
#define EEPID_CFGDID_0x1027_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1028             0x0039 /*（1）休眠上报状态 + （2）CRC*/
#define EEPID_CFGDID_0x1028_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1029             0x003A /*（1）大数据上报状态+ （2）CRC*/
#define EEPID_CFGDID_0x1029_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1030             0x003B /*（1）车辆变化上报状态+ （2）CRC*/
#define EEPID_CFGDID_0x1030_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1031             0x003C /*（1）故障灯上报状态 + （2）CRC*/
#define EEPID_CFGDID_0x1031_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1032             0x003D /*（1）故障码上报状态 + （2）CRC*/
#define EEPID_CFGDID_0x1032_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1034             0x003E /*（1）拖车报警状态 + （2）CRC*/
#define EEPID_CFGDID_0x1034_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1035             0x003F /*（1）碰撞报警状态 + （2）CRC*/
#define EEPID_CFGDID_0x1035_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1036             0x0040 /*（1）事件触发式CAN采集上报 + （2）CRC*/
#define EEPID_CFGDID_0x1036_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1040             0x0041 /*（64）MCU软件版本信息 + （2）CRC*/
#define EEPID_CFGDID_0x1040_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1041             0x0042 /*（64）应用软件版本信息 + （2）CRC*/
#define EEPID_CFGDID_0x1041_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1042             0x0043 /*（64）通讯模块版本信息 + （2）CRC*/
#define EEPID_CFGDID_0x1042_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1043             0x0044 /*（64）硬件版本信息 + （2）CRC*/
#define EEPID_CFGDID_0x1043_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1049             0x0045 /*（64）事件触发式CAN数据采集 + （2）CRC*/
#define EEPID_CFGDID_0x1049_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1050             0x0046 /*（78）管理员触发式CAN数据采集 +（2）CRC*/
#define EEPID_CFGDID_0x1050_LEN         10*EE_WR_STEPS

#define EEPID_CFGDID_0x1051             0x047 /*（78）周期性CAN数据采集 +（2）CRC*/
#define EEPID_CFGDID_0x1051_LEN         10*EE_WR_STEPS

#define EEPID_CFGDID_0x1052             0x0048 /*（64）LOG数据文件路径 + （2）CRC*/
#define EEPID_CFGDID_0x1052_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1054             0x0049 /*（1）TBOX工作模式 + （2）CRC*/
#define EEPID_CFGDID_0x1054_LEN         1*EE_WR_STEPS

//#define EEPID_CFGDID_0x1055             0x04A /*（N）诊断数据读取+ （2）CRC*/
//#define EEPID_CFGDID_0x1055_LEN        N

#define EEPID_CFGDID_0x1056             0x004B /*（1）通讯模块复位 + （2）CRC*/
#define EEPID_CFGDID_0x1056_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1057             0x004C /*（1）TBOX复位 + （2）CRC*/
#define EEPID_CFGDID_0x1057_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1058             0x004D /*（1）TBOX工作模式 + （2）CRC*/
#define EEPID_CFGDID_0x1058_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1059             0x004E /*（1）TBOX流量控制 + （2）CRC*/
#define EEPID_CFGDID_0x1059_LEN         1*EE_WR_STEPS

//#define EEPID_CFGDID_0x1060             0x04F /*（N）车辆状态信息配置 + （2）CRC*/
//#define EEPID_CFGDID_0x1060_LEN        N

#define EEPID_CFGDID_0x1061             0x0050 /*（1）静默升级 + （2）CRC*/
#define EEPID_CFGDID_0x1061_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1062             0x0051 /*（1）大数据定时上报设置 + （2）CRC*/
#define EEPID_CFGDID_0x1062_LEN         1*EE_WR_STEPS

//#define EEPID_CFGDID_0x1064             0x0052 /*（N）查询车辆信息 +（2）CRC*/
//#define EEPID_CFGDID_0x1064_LEN        N

#define EEPID_CFGDID_0x1065             0x0053 /*（1）远程视频请求 +（2）CRC*/
#define EEPID_CFGDID_0x1065_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1066             0x0054 /*（1）电子围栏 + （2）CRC*/
#define EEPID_CFGDID_0x1066_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1067             0x0055 /*（96）远程log提取 + （2）CRC*/
#define EEPID_CFGDID_0x1067_LEN         13*EE_WR_STEPS

//#define EEPID_CFGDID_0x1068             0x0056 /*（N）道路运输证IC卡认证主服务器 + （2）CRC*/
//#define EEPID_CFGDID_0x1068_LEN        N

#define EEPID_CFGDID_0x1069             0x0057 /*（10）商用车基础信息  + （2）CRC*/
#define EEPID_CFGDID_0x1069_LEN         2*EE_WR_STEPS

//#define EEPID_CFGDID_0x106A             0x0058 /*（N）商用车电话信息 + （2）CRC*/
//#define EEPID_CFGDID_0x106A_LEN        N

#define EEPID_CFGDID_0x106B             0x0059 /*（20）商用车报警信息 + （2）CRC*/
#define EEPID_CFGDID_0x106B_LEN         3*EE_WR_STEPS

#define EEPID_CFGDID_0x106C             0x005A /*（36）商用车报警条件信息 + （2）CRC*/
#define EEPID_CFGDID_0x106C_LEN         5*EE_WR_STEPS

//#define EEPID_CFGDID_0x106D             0x005B /*（N）商用车车辆信息 + （2）CRC*/
//#define EEPID_CFGDID_0x106D_LEN        N

#define EEPID_CFGDID_0x106E             0x005C /*（12）商用车GPS设置信息 + （2）CRC*/
#define EEPID_CFGDID_0x106E_LEN         2*EE_WR_STEPS

#define EEPID_CFGDID_0x106F             0x005D /*（28）商用车CAN数据采集 + （2）CRC*/
#define EEPID_CFGDID_0x106F_LEN         4*EE_WR_STEPS

#define EEPID_CFGDID_0x1070             0x005E /*（1）商用车车辆型号+ （2）CRC*/
#define EEPID_CFGDID_0x1070_LEN         1*EE_WR_STEPS

//#define EEPID_CFGDID_0x1071             0x005F /*（N）商用车车架号 + （2）CRC*/
//#define EEPID_CFGDID_0x1071_LEN        N

#define EEPID_CFGDID_0x1072             0x0060 /*（1）商用车发送机信息 + （2）CRC*/
#define EEPID_CFGDID_0x1072_LEN         1*EE_WR_STEPS

//#define EEPID_CFGDID_0x1073             0x0061 /*（N）商用车终端属性 + （2）CRC*/
//#define EEPID_CFGDID_0x1073_LEN        N

#define EEPID_CFGDID_0x1074             0x0062 /*（96）MP5 LOG远程提取 + （2）CRC*/
#define EEPID_CFGDID_0x1074_LEN         13*EE_WR_STEPS

#define EEPID_CFGDID_0x1075             0x0063 /*（1）AEB数据采集 + （2）CRC*/
#define EEPID_CFGDID_0x1075_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1076             0x0064 /*（1）康明斯发动机远程标定使能标志+ （2）CRC*/
#define EEPID_CFGDID_0x1076_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1077             0x0065 /*（2）新能源商用车新能源数据发送间隔 + （2）CRC*/
#define EEPID_CFGDID_0x1077_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x1078             0x0066 /*（67）新能源商用车第三方平台设置 + （2）CRC*/
#define EEPID_CFGDID_0x1078_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x1079             0x0067 /*（2）排放终端属性 + （2）CRC*/
#define EEPID_CFGDID_0x1079_LEN         1*EE_WR_STEPS

//#define EEPID_CFGDID_0x1080             0x0068 /*（N）蓝牙根密钥查询 + （2）CRC*/
//#define EEPID_CFGDID_0x1080_LEN        N

#define EEPID_CFGDID_0x6000             0x0069 /*（25）GPS信息 +（2）CRC*/
#define EEPID_CFGDID_0x6000_LEN         4*EE_WR_STEPS

#define EEPID_CFGDID_0x6001             0x006A /*（1）油门踏板状态 +（2）CRC*/
#define EEPID_CFGDID_0x6001_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6002             0x006B /*（1）车锁状态 +（2）CRC*/
#define EEPID_CFGDID_0x6002_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6003             0x006C /*（1）车门状态 +（2）CRC*/
#define EEPID_CFGDID_0x6003_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6004             0x006D /*（1）车窗状态 +（2）CRC*/
#define EEPID_CFGDID_0x6004_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6005             0x006E /*（3）车辆工作状态 +（2）CRC*/
#define EEPID_CFGDID_0x6005_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6006             0x006F /*（16）胎压信息 +（2）CRC*/
#define EEPID_CFGDID_0x6006_LEN         3*EE_WR_STEPS

#define EEPID_CFGDID_0x6007             0x0070 /*（18）车辆行驶信息 +（2）CRC*/
#define EEPID_CFGDID_0x6007_LEN         3*EE_WR_STEPS

#define EEPID_CFGDID_0x6008             0x0071 /*（1）只记录行程信息 +（2）CRC*/
#define EEPID_CFGDID_0x6008_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6009             0x0072 /*（5）车辆故障灯信息 +（2）CRC*/
#define EEPID_CFGDID_0x6009_LEN         2*EE_WR_STEPS

// #define EEPID_CFGDID_0x600A             0x0073 /*（7+n1+n2+nn）车辆故障信息 +（2）CRC*/
// #define EEPID_CFGDID_0x600A_LEN         4*EE_WR_STEPS

// #define EEPID_CFGDID_0x600B             0x0074 /*（3+m+n）TBOX状态信息 + （2）CRC*/
// #define EEPID_CFGDID_0x600B_LEN         17*EE_WR_STEPS

#define EEPID_CFGDID_0x600C             0x0075 /*（4）空调信息 + （2）CRC*/
#define EEPID_CFGDID_0x600C_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x600D             0x0076 /*（2）座椅信息 + （2）CRC*/
#define EEPID_CFGDID_0x600D_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x600E             0x0077 /*（4）车窗位置状态 +（2）CRC*/
#define EEPID_CFGDID_0x600E_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x600F             0x0078 /*（3）空气净化器信息 +（2）CRC*/
#define EEPID_CFGDID_0x600F_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6010             0x0079 /*（1）转向灯配置信息 +（2）CRC*/
#define EEPID_CFGDID_0x6010_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6011             0x007A /*（2）车辆报警状态 +（2）CRC*/
#define EEPID_CFGDID_0x6011_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6013             0x007B /*（1）X-CALL状态 +（2）CRC*/
#define EEPID_CFGDID_0x6013_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6014             0x007C /*（1）下线状态 +（2）CRC*/
#define EEPID_CFGDID_0x6014_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6015             0x007D /*（30）圆形防护圈信息 +（2）CRC*/
#define EEPID_CFGDID_0x6015_LEN         4*EE_WR_STEPS

#define EEPID_CFGDID_0x6016             0x007E /*（34）矩形防护圈信息 +（2）CRC*/
#define EEPID_CFGDID_0x6016_LEN         5*EE_WR_STEPS

#define EEPID_CFGDID_0x6017             0x007F /*（1）防护圈属性 +（2）CRC*/
#define EEPID_CFGDID_0x6017_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6018             0x0080 /*（64）空中升级FTP服务器地址 +（2）CRC*/
#define EEPID_CFGDID_0x6018_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x6019             0x0081 /*（64）FTP服务器用户名 +（2）CRC*/
#define EEPID_CFGDID_0x6019_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x601A             0x0082 /*（64）FTP服务器密码 +（2）CRC*/
#define EEPID_CFGDID_0x601A_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x601B             0x0083 /*（64）升级文件路径 +（2）CRC*/
#define EEPID_CFGDID_0x601B_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x601C             0x0084 /*（64）升级文件文件名 +（2）CRC*/
#define EEPID_CFGDID_0x601C_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x601D             0x0085 /*（8）休眠时间 + （2）CRC*/
#define EEPID_CFGDID_0x601D_LEN         2*EE_WR_STEPS

#define EEPID_CFGDID_0x601E             0x0086 /*（1）SendToCar + （2）CRC*/
#define EEPID_CFGDID_0x601E_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x601F             0x0087 /*（66）OTA升级服务器信息 + （2）CRC*/
#define EEPID_CFGDID_0x601F_LEN         9*EE_WR_STEPS

#define EEPID_CFGDID_0x6020             0x0088 /*（256）视频URL地址 + （2）CRC*/
#define EEPID_CFGDID_0x6020_LEN         33*EE_WR_STEPS

#define EEPID_CFGDID_0x6021             0x0089 /*（20）整车数据 + （2）CRC*/
#define EEPID_CFGDID_0x6021_LEN         3*EE_WR_STEPS

#define EEPID_CFGDID_0x6022             0x008A/*（12）驱动电机 + （2）CRC*/
#define EEPID_CFGDID_0x6022_LEN         2*EE_WR_STEPS

//#define EEPID_CFGDID_0x6023             0x008B /*（18+N）燃料电池 + （2）CRC*/
//#define EEPID_CFGDID_0x6023_LEN        N

#define EEPID_CFGDID_0x6024             0x008C /*（5）发动机数据 + （2）CRC*/
#define EEPID_CFGDID_0x6024_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6025             0x008D /*（5）车辆位置 + （2）CRC*/
#define EEPID_CFGDID_0x6025_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6026             0x008E /*（14）极值数据 + （2）CRC*/
#define EEPID_CFGDID_0x6026_LEN         2*EE_WR_STEPS

//#define EEPID_CFGDID_0x6027             0x008F /*（9 + N*4 + N2*4 + N3*4 +N?*4）报警数据 + （2）CRC*/
//#define EEPID_CFGDID_0x6027_LEN        N

//#define EEPID_CFGDID_0x6028             0x0090 /*可充电储能装置电压 +（2）CRC*/
//#define EEPID_CFGDID_0x6028_LEN        N

//#define EEPID_CFGDID_0x6029             0x0091 /**/
//#define EEPID_CFGDID_0x6029_LEN        N

#define EEPID_CFGDID_0x602A             0x0092 /*（25）GPS-商用车 + （2）CRC*/
#define EEPID_CFGDID_0x602A_LEN         4*EE_WR_STEPS

#define EEPID_CFGDID_0x602B             0x0093 /*（37）商用车附加信息 + （2）CRC*/
#define EEPID_CFGDID_0x602B_LEN         5*EE_WR_STEPS

#define EEPID_CFGDID_0x602D             0x0094 /*（8）车辆ECU列表 + （2）CRC*/
#define EEPID_CFGDID_0x602D_LEN         2*EE_WR_STEPS

#define EEPID_CFGDID_0x602E             0x0095 /*（17）车辆辅助信息 +（2）CRC*/
#define EEPID_CFGDID_0x602E_LEN         3*EE_WR_STEPS

//#define EEPID_CFGDID_0x602F             0x0096 /*（3+N1+N2+N3）TBOX版本信息 +（2）CRC*/
//#define EEPID_CFGDID_0x602F_LEN        N

#define EEPID_CFGDID_0x6030             0x0097 /*（4）报警信息1 +（2）CRC*/
#define EEPID_CFGDID_0x6030_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6031             0x0098 /*（17）GPS信息-(精简信息) +（2）CRC*/
#define EEPID_CFGDID_0x6031_LEN         3*EE_WR_STEPS

#define EEPID_CFGDID_0x6032             0x0099 /*（372）iHev_Signal+（2）CRC*/
#define EEPID_CFGDID_0x6032_LEN         47*EE_WR_STEPS

#define EEPID_CFGDID_0x6033             0x009A /* （146）重商信息 +（2）CRC*/
#define EEPID_CFGDID_0x6033_LEN         19*EE_WR_STEPS

#define EEPID_CFGDID_0x6034             0x009B /*（33）1939总线数据信息 +（2）CRC*/
#define EEPID_CFGDID_0x6034_LEN         5*EE_WR_STEPS

#define EEPID_CFGDID_0x6035             0x009C /*（54）11898总线数据信息 +（2）CRC*/
#define EEPID_CFGDID_0x6035_LEN         7*EE_WR_STEPS

//#define EEPID_CFGDID_0x6036             0x09D /*（6+N1+N2）网关地址信息 +（2）CRC*/
//#define EEPID_CFGDID_0x6036_LEN        N

//#define EEPID_CFGDID_0x6037             0x009E /*（N）异常信息 +（2）CRC*/
//#define EEPID_CFGDID_0x6037_LEN        N

#define EEPID_CFGDID_0x6038             0x009F /*（1）车型信息 +（2）CRC*/
#define EEPID_CFGDID_0x6038_LEN         1*EE_WR_STEPS

//#define EEPID_CFGDID_0x6039             0x0100 /*（30+N） 终端登入信息+（2）CRC*/
//#define EEPID_CFGDID_0x6039_LEN        N

#define EEPID_CFGDID_0x603A             0x0101 /*（8）终端登出信息 +（2）CRC*/
#define EEPID_CFGDID_0x603A_LEN         2*EE_WR_STEPS

#define EEPID_CFGDID_0x603B             0x0102 /*（33）车辆行驶大数据信息1s周期 +（2）CRC*/
#define EEPID_CFGDID_0x603B_LEN         5*EE_WR_STEPS

#define EEPID_CFGDID_0x603C             0x0103 /* （21）DTC信息（S7重混）+（2）CRC*/
#define EEPID_CFGDID_0x603C_LEN         3*EE_WR_STEPS

#define EEPID_CFGDID_0x603D             0x0104 /*（8）推送信息 +（2）CRC*/
#define EEPID_CFGDID_0x603D_LEN         2*EE_WR_STEPS

//#define EEPID_CFGDID_0x603F             0x0105 /*（N）ECU版本信息+（2）CRC*/
//#define EEPID_CFGDID_0x603F_LEN        N

//#define EEPID_CFGDID_0x6040             0x0106 /*（N）氢燃料汽车信息（包括前面6023部分） +（2）CRC*/
//#define EEPID_CFGDID_0x6040_LEN        N

#define EEPID_CFGDID_0x6041             0x0107 /*（19）车辆行驶大数据信息+（2）CRC*/
#define EEPID_CFGDID_0x6041_LEN         3*EE_WR_STEPS

#define EEPID_CFGDID_0x6043             0x0108 /*（40）车辆配置信息重卡 + （2）CRC*/
#define EEPID_CFGDID_0x6043_LEN         6*EE_WR_STEPS

//#define EEPID_CFGDID_0x6045             0x0109 /*（N）新能源附加信息 +（2）CRC*/
//#define EEPID_CFGDID_0x6045_LEN        N

#define EEPID_CFGDID_0x6046             0x010A /*（2）刹车压力值 +（2）CRC*/
#define EEPID_CFGDID_0x6046_LEN         1*EE_WR_STEPS


//#define EEPID_CFGDID_0x6047             0x010B /*（N）轻卡1939自主发动机诊断数据 +（2）CRC*/
//#define EEPID_CFGDID_0x6047_LEN        N

//#define EEPID_CFGDID_0x6048             0x010C /*（N）康明斯监控数据 +（2）CRC*/
//#define EEPID_CFGDID_0x6048_LEN        N

//#define EEPID_CFGDID_0x6049             0x010D /*（N）康明斯trip数据 +（2）CRC*/
//#define EEPID_CFGDID_0x6049_LEN        N

//#define EEPID_CFGDID_0x604A             0x010E /*（N）康明斯trim设置数据 +（2）CRC*/
//#define EEPID_CFGDID_0x604A_LEN        N

//#define EEPID_CFGDID_0x604B             0x010F /*（N）康明斯trim数据 +（2）CRC*/
//#define EEPID_CFGDID_0x604B_LEN        N

//#define EEPID_CFGDID_0x604C             0x0110 /*（N）康明斯DM1诊断数据 +（2）CRC*/
//#define EEPID_CFGDID_0x604C_LEN        N

//#define EEPID_CFGDID_0x604D             0x0111 /*（N）康明斯终端数据 +（2）CRC*/
//#define EEPID_CFGDID_0x604D_LEN        N

//#define EEPID_CFGDID_0x604E             0x0112 /*（N）发动机远程标定密钥请求数据 +（2）CRC*/
//#define EEPID_CFGDID_0x604E_LEN        N

#define EEPID_CFGDID_0x604F             0x0113 /*（77）发动机远程标定密钥数据 +（2）CRC*/
#define EEPID_CFGDID_0x604F_LEN         10*EE_WR_STEPS

//#define EEPID_CFGDID_0x6050             0x0114 /*（N）康明斯发动机远程标定属性 +（2）CRC*/
//#define EEPID_CFGDID_0x6050_LEN        N

#define EEPID_CFGDID_0x6051             0x0115 /*（2）多包数据应答 +（2）CRC*/
#define EEPID_CFGDID_0x6051_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6052             0x0116 /*（9）康明斯trim数据应答 +（2）CRC*/
#define EEPID_CFGDID_0x6052_LEN         2*EE_WR_STEPS

//#define EEPID_CFGDID_0x6053             0x0117 /*（N）大数据流上报 +（2）CRC*/
//#define EEPID_CFGDID_0x6053_LEN        N

#define EEPID_CFGDID_0x6054             0x0118 /*（5）大数据流上报应答 +（2）CRC*/
#define EEPID_CFGDID_0x6054_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x6055             0x0119 /*（24）胎压信息(六轮) +（2）CRC*/
#define EEPID_CFGDID_0x6055_LEN         4*EE_WR_STEPS

//#define EEPID_CFGDID_0x6056             0x011A /*（N）蓝牙根密钥 +（2）CRC*/
//#define EEPID_CFGDID_0x6056_LEN        N

#define EEPID_CFGDID_0x2008             0x011B /*（1）远程寻车 + （2）CRC*/
#define EEPID_CFGDID_0x2008_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x200A             0x011C /*（4）远程开启自动空调 + （2）CRC*/
#define EEPID_CFGDID_0x200A_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x200C             0x011D /*（2）远程座椅加热 + （2）CRC*/
#define EEPID_CFGDID_0x200C_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x200D             0x011E /*（2）远程座椅通风 + （2）CRC*/
#define EEPID_CFGDID_0x200D_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x2010             0x011F /*（1）远程控制天窗位置 +（2）CRC*/
#define EEPID_CFGDID_0x2010_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x2011             0x0120 /*（4）远程控制车窗位置 + （2）CRC*/
#define EEPID_CFGDID_0x2011_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x2012             0x0121 /*（1）远程控制后备箱 + （2）CRC*/
#define EEPID_CFGDID_0x2012_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x2013             0x0122 /*（5）远程开启电动空调 + （2）CRC*/
#define EEPID_CFGDID_0x2013_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x201A             0x0123 /*（1）清除故障码 +（2）CRC*/
#define EEPID_CFGDID_0x201A_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x201D             0x0124 /*（2）信贷锁闭锁 +（2）CRC*/
#define EEPID_CFGDID_0x201D_LEN         1*EE_WR_STEPS

#define EEPID_NVID_0xE00C               0x0125 /*（2）锁车命令发送请求 +（2）CRC*/
#define EEPID_NVID_0xE00C_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE00D               0x0126 /*（1）重卡信贷锁车失败次数 +（2） CRC*/
#define EEPID_NVID_0xE00D_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE00E               0x0127 /*（1）TBOX激活状态 +（2） CRC*/
#define EEPID_NVID_0xE00E_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE00F               0x0128 /*（2）TBOX激活BCM状态 + （2）CRC*/
#define EEPID_NVID_0xE00F_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE010               0x0129 /*（5）预留地址 +（2）CRC*/
#define EEPID_NVID_0xE010_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE011               0x012A /*（15）预留地址 +（2）CRC*/
#define EEPID_NVID_0xE011_LEN           3*EE_WR_STEPS

#define EEPID_NVID_0xE012               0x012B /*（32）INDEX_PM_MODE_BEFORE_RESET */
#define EEPID_NVID_0xE012_LEN           4*EE_WR_STEPS

#define EEPID_DIAGDID_0xB000            0x012C /*（1）ECU Power Supply Voltage ECU供电电压 +（2）CRC*/
#define EEPID_DIAGDID_0xB000_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xB010            0x012D /*（1）Device Status 设备状态 +（2）CRC*/
#define EEPID_DIAGDID_0xB010_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xB011            0x012E /*（1）4G Network Status 4G网络状态 +（2）CRC*/
#define EEPID_DIAGDID_0xB011_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xB02A            0x012F /*（15）IMSI Number +（2）CRC*/
#define EEPID_DIAGDID_0xB02A_LEN        3*EE_WR_STEPS

#define EEPID_DIAGDID_0xB02B            0x0130 /*（11）SIM Phone Number SIM卡中电话号码 +（2）CRC*/
#define EEPID_DIAGDID_0xB02B_LEN        2*EE_WR_STEPS

#define EEPID_DIAGDID_0xB300            0x0131 /*（1）TBOX Status +（2）CRC*/
#define EEPID_DIAGDID_0xB300_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xB301            0x0132 /*（8）CAN配置 +（2）CRC */
#define EEPID_DIAGDID_0xB301_LEN        2*EE_WR_STEPS

#define EEPID_DIAGDID_0xB302            0x0133 /*（4）Security Constant安全常量 +（2）CRC */
#define EEPID_DIAGDID_0xB302_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xB303            0x0134 /*（2）Reset Source重置源 +（2）CRC */
#define EEPID_DIAGDID_0xB303_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xB0B2            0x0135 /*（4）SecurityCode 安全代码 +（2）CRC*/
#define EEPID_DIAGDID_0xB0B2_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF201            0x0136 /*（5）OFF电1h后网络管理状态发送非12/32的节点 +（2）CRC*/
#define EEPID_DIAGDID_0xF201_LEN        1*EE_WR_STEPS

#define EEPID_DIAGDID_0xF202            0x0137 /*（15）OFF电1h后网络管理状态发送非12/32出现次数 +（2）CRC*/
#define EEPID_DIAGDID_0xF202_LEN        3*EE_WR_STEPS

#define EEPID_NVID_0xE013               0x0138 /*（1）NM_BUFFER_LEN 网络管理报文接收缓存数 +（2）CRC*/
#define EEPID_NVID_0xE013_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE014               0x0139 /*（1）NM_CONFIGLIST_LEN 网络管理配置列表数 +（2）CRC*/
#define EEPID_NVID_0xE014_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE015               0x013A /*（1）TX_LIMIT 发送网络管理报文失败计数器限制 +（2）CRC*/
#define EEPID_NVID_0xE015_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE016               0x013B /*（1）RX_LIMIT 接收网络管理报文失败计数器限制 +（2）CRC*/
#define EEPID_NVID_0xE016_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE017               0x013C /*（4）T_TYP 环报文的典型延迟时间 +（2）CRC*/
#define EEPID_NVID_0xE017_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE018               0x013D /*（4）T_MAX 两个环报文之间最大的允许间隔时间 +（2）CRC*/
#define EEPID_NVID_0xE018_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE019               0x013E /*（4）T_ERROR 两个跛行回家报文之间的时间间隔 +（2）CRC*/
#define EEPID_NVID_0xE019_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE01A               0x013F /*（4）T_WAIT_BUS_SLEEP 发送或者接收睡眠应答报文到转移到总线非激活（总线睡眠）的时间间隔+（2）CRC*/
#define EEPID_NVID_0xE01A_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE01B               0x0140 /*（64）NM_ID_BASE 网络管理报文的基地址 +（2）CRC*/
#define EEPID_NVID_0xE01B_LEN           9*EE_WR_STEPS

#define EEPID_NVID_0xE01C               0x0141 /*（64）NM_SELF_ID 本节点的ID编号 +（2）CRC*/
#define EEPID_NVID_0xE01C_LEN           9*EE_WR_STEPS

#define EEPID_NVID_0xE01D               0x0142 /*（64）NM_BASE_ADDRESS 网络管理基地址 +（2）CRC*/
#define EEPID_NVID_0xE01D_LEN           9*EE_WR_STEPS

#define EEPID_NVID_0xE01E               0x0143 /*（4）T_BAT_REQ 上电后释放网络时间 +（2）CRC*/
#define EEPID_NVID_0xE01E_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE01F               0x0144 /*（4）T_LIMPHOME_LOG 跛行回家故障记录时间 +（2）CRCC*/
#define EEPID_NVID_0xE01F_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE020               0x0145 /*（（4）T_NMACTIVE 网路激活时间 +（2）CRC*/
#define EEPID_NVID_0xE020_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE021               0x0146 /*（8） PM_SupVolt 供电系统配置字（12V系统/24V系统）+（2）CRC*/
#define EEPID_NVID_0xE021_LEN           2*EE_WR_STEPS

#define EEPID_NVID_0xE022               0x0147 /*（4）PM_PowerTimer11 一级休眠的唤醒周期（默认4分钟）+（2）CRC*/
#define EEPID_NVID_0xE022_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE023               0x0148/*（4）PM_PowerTimer12 一级休眠转二级休眠的时间参数（默认连续3天）+（2）CRC*/
#define EEPID_NVID_0xE023_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE024               0x0149 /*（4）PM_PowerTimer2 二级休眠转三级休眠的时间参数（默认连续11天）+（2）CRC*/
#define EEPID_NVID_0xE024_LEN           1*EE_WR_STEPS

#define EEPID_NVID_0xE025               0x014A /*（8）SC32参数（诊断工具标定，鉴权使用） +（2）CRC*/
#define EEPID_NVID_0xE025_LEN           2*EE_WR_STEPS

#define EEPID_NVID_0xE026               0x014B /*（8）BCM_SK64（EOL学习是写入，BCM鉴权使用） +（2）CRC*/
#define EEPID_NVID_0xE026_LEN           2*EE_WR_STEPS

#define EEPID_NVID_0xE027               0x014C /*（8）PEPS_SK64（EOL学习是写入，PEPS鉴权使用） +（2）CRC*/
#define EEPID_NVID_0xE027_LEN           2*EE_WR_STEPS

#define EEPID_NVID_0xE028               0x014D /*（4）Tinfo定时上报周期 +（2）CRC*/
#define EEPID_NVID_0xE028_LEN           1*EE_WR_STEPS

#define EEPID_CFGDID_0x6012             0x014E /*TBD */
#define EEPID_CFGDID_0x6012_LEN         1*EE_WR_STEPS

#define EEPID_CFGDID_0x602C             0x014F /*TBD*/
#define EEPID_CFGDID_0x602C_LEN         5*EE_WR_STEPS

/* 0x0300~0x03FF，此ID段供DTC相关内容使用 */
#define EEPID_DTCDATA                      0x0301 /* DTC data*/
#define EEPID_DTCDATA_LEN                  128*EE_WR_STEPS

#define EEPID_SECURITY_ACCESSS_FLAG        0x0302 /* (1)安全访问标志*/
#define EEPID_SECURITY_ACCESSS_FLAG_LEN    1*EE_WR_STEPS


/* 0x0400~0x04FF，此ID段供EOL相关内容使用 */
#define EEPID_EOL_FLAG                  0x0400 /* EOL标志位*/
#define EEPID_EOL_FLAG_LEN              1*EE_WR_STEPS

#define EEPID_EOL_LOCK_FLAG             0x0401 /* EOL锁标志位*/
#define EEPID_EOL_LOCK_FLAG_LEN         1*EE_WR_STEPS

#define EEPID_EOL_VBAT_GAIN             0x0402 /* 电压增益*/
#define EEPID_EOL_VBAT_GAIN_LEN         1*EE_WR_STEPS

#define EEPID_EOL_VBAT_OFFSET           0x0403 /* 电压偏移*/
#define EEPID_EOL_VBAT_OFFSET_LEN       1*EE_WR_STEPS

#define EEPID_EOL_CRCDATA               0x0404 /* 校验*/
#define EEPID_EOL_CRCDATA_LEN           1*EE_WR_STEPS

#define EEPID_EOL_Trace_Num             0x0405 /* 产线ECU追溯号*/
#define EEPID_EOL_Trace_Num_LEN         2*EE_WR_STEPS

/* 0x0500~0x07FF，此ID段供其他功能相关内容使用 */

#define EEPID_AUX_SLEEPTIME             0x0500 /* (6)保存休眠时当前时间+(2)CRC*/
#define EEPID_AUX_SLEEPTIME_LEN         1*EE_WR_STEPS

#define EEPID_AUX_SLEEPTIMEBACKUP       0x0501 /* (6)保存休眠时当前时间+(2)CRC(当0x0500CRC校验不过时，使用0x0501)*/
#define EEPID_AUX_SLEEPTIMEBACKUP_LEN   1*EE_WR_STEPS

#define EEPID_USERID                    0x0502 /* (14)蓝牙钥匙解锁后TSP给用户的ID+(2)CRC */
#define EEPID_USERID_LEN                2*EE_WR_STEPS

#define EEPID_USERIDBACKUP             0x0503 /* (14)蓝牙钥匙解锁后TSP给用户的ID+(2)CRC(当0x0502CRC校验不过时，使用0x0503) */
#define EEPID_USERIDBACKUP_LEN          2*EE_WR_STEPS

#define EEPID_TTOKEN                    0x0504 /* (32)配置更新时TSP给TBOX的一个TOKEN+(2)CRC */
#define EEPID_TTOKEN_LEN                5*EE_WR_STEPS

#define EEPID_TTOKENBACKUP              0x0505 /* (32)配置更新时TSP给TBOX的一个TOKEN+(2)CRC(当0x0504CRC校验不过时，使用0x0505) */
#define EEPID_TTOKENBACKUP_LEN          5*EE_WR_STEPS

#define EEPID_AUX_ALARMTIMEUTC          0x0506 /* (4)TBOX下电时设置的Alarm唤醒时间+(2)CRC */
#define EEPID_AUX_ALARMTIMEUTC_LEN      1*EE_WR_STEPS

#define EEPID_AUX_ALARMTIMEUTCBACKUP        0x0507 /* (4)TBOX下电时设置的Alarm唤醒时间+(2)CRC(当0x0504CRC校验不过时，使用0x0506) */
#define EEPID_AUX_ALARMTIMEUTCBACKUP_LEN    1*EE_WR_STEPS

#define EEPID_IMMODATA                  0x0508 /* (16)ImmoData+(2)CRC*/
#define EEPID_IMMODATA_LEN              3*EE_WR_STEPS      

#define EEPID_IMMODATABACKUP            0x0509 /* (16)ImmoData备份+(2)CRC(当0x0235CRC校验不过时，使用0x0509)*/
#define EEPID_IMMODATABACKUP_LEN        3*EE_WR_STEPS

#define EEPID_NXP_KEYK1                 0x050A /* (20)NXPKey+(2)CRC*/
#define EEPID_NXP_KEYK1_LEN             3*EE_WR_STEPS      

#define EEPID_NXP_KEYK1BACKUP           0x050B /* (20))NXPKey备份+(2)CRC(当0x050ACRC校验不过时，使用0x50B)*/
#define EEPID_NXP_KEYK1BACKUP_LEN       3*EE_WR_STEPS

#define EEPID_USERCERTIFICATE           0x050C /* (176)用户证书+(2)CRC*/
#define EEPID_USERCERTIFICATE_LEN       23*EE_WR_STEPS       

#define EEPID_USERCERTIFICATEBACKUP     0x050D /* (176)用户证书备份+(2)CRC(当0x050C CRC校验不过时，使用0x50D)*/
#define EEPID_USERCERTIFICATEBACKUP_LEN 23*EE_WR_STEPS   

#define EEPID_PUBLICKEY_X                0x050E /* (32)公钥X+(2)CRC*/
#define EEPID_PUBLICKEY_X_LEN            5*EE_WR_STEPS      

#define EEPID_PUBLICKEY_X_BACKUP          0x050F /* (32)公钥X备份+(2)CRC(当0x050E CRC校验不过时，使用0x50F)*/
#define EEPID_PUBLICKEY_X_BACKUP_LEN      5*EE_WR_STEPS 

#define EEPID_PUBLICKEY_Y                0x0510 /* (32)公钥Y+(2)CRC*/
#define EEPID_PUBLICKEY_Y_LEN            5*EE_WR_STEPS        

#define EEPID_PUBLICKEY_Y_BACKUP          0x0511 /* (32)公钥Y备份+(2)CRC(当0x0510CRC校验不过时，使用0x511)*/
#define EEPID_PUBLICKEY_Y_BACKUP_LEN      5*EE_WR_STEPS 

#define EEPID_SECRETKEY                 0x0512 /* (32)私钥+(2)CRC*/
#define EEPID_SECRETKEY_LEN             5*EE_WR_STEPS        

#define EEPID_SECRETKEYBACKUP           0x0513 /* (32)私钥备份+(2)CRC(当0x0512CRC校验不过时，使用0x513)*/
#define EEPID_SECRETKEYBACKUP_LEN       5*EE_WR_STEPS 

#define EEPID_RVSREQUESTID              0x0514 /* (10)存储远程车控TBOXRequestID*/
#define EEPID_RVSREQUESTID_LEN          2*EE_WR_STEPS 

/********0x0800~0x0810***********/
#define EEPID_WAKEUPSOURCE              0x0800 /* (8) 唤醒源*/
#define EEPID_WAKEUPSOURCE_LEN          1*EE_WR_STEPS 
/*****************************************************************************
** System Macro Definition
*****************************************************************************/


/*****************************************************************************
** Action Macro Definition
*****************************************************************************/


/*****************************************************************************
** Config Macro Definition
*****************************************************************************/


/*****************************************************************************
** Task Macro Definition
*****************************************************************************/


/*****************************************************************************
** Variables
*****************************************************************************/


/*****************************************************************************
** Constants
*****************************************************************************/


/*****************************************************************************
** Function prototypeseb
*****************************************************************************/


/*****************************************************************************
** other
*****************************************************************************/


/****************************************************************************/

#endif	//__MEM_EE_ID_H__

/*****************************************************************************
** End File
*****************************************************************************/


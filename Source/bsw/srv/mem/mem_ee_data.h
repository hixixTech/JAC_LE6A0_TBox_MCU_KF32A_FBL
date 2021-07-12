/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    mem_ee_data.h
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

#ifndef __MEM_EE_DATA_H__
#define __MEM_EE_DATA_H__

/*****************************************************************************
**#include 
*****************************************************************************/
#include "type.h"
#include "mem_ee_id.h"
/*****************************************************************************
**  typedef
*****************************************************************************/
typedef struct
{
    UINT32 u32_eeprom_id;
    UINT16 u16_eeprom_len;
    UINT16 u16_eeprom_offset;
    UINT32 u32_init_data;
}MemEeprom_Map;

typedef struct
{
    UINT8 u8_DiagIdF186Data[EEPID_DIAGDID_0xF186_LEN];
    UINT8 u8_DiagIdF187Data[EEPID_DIAGDID_0xF187_LEN];
    UINT8 u8_DiagIdF188Data[EEPID_DIAGDID_0xF188_LEN];
    UINT8 u8_DiagIdF189Data[EEPID_DIAGDID_0xF189_LEN];
    UINT8 u8_DiagIdF18AData[EEPID_DIAGDID_0xF18A_LEN];
    UINT8 u8_DiagIdF18BData[EEPID_DIAGDID_0xF18B_LEN];
    UINT8 u8_DiagIdF18CData[EEPID_DIAGDID_0xF18C_LEN];
    UINT8 u8_DiagIdF190Data[EEPID_DIAGDID_0xF190_LEN];
    UINT8 u8_DiagIdF192Data[EEPID_DIAGDID_0xF192_LEN];
	UINT8 u8_DiagIdF193Data[EEPID_DIAGDID_0xF193_LEN];
    UINT8 u8_DiagIdF194Data[EEPID_DIAGDID_0xF194_LEN];
    UINT8 u8_DiagIdF195Data[EEPID_DIAGDID_0xF195_LEN];
    UINT8 u8_DiagIdF197Data[EEPID_DIAGDID_0xF197_LEN];
    UINT8 u8_DiagIdF198Data[EEPID_DIAGDID_0xF198_LEN];
    UINT8 u8_DiagIdF199Data[EEPID_DIAGDID_0xF199_LEN];
    UINT8 u8_DiagIdF19DData[EEPID_DIAGDID_0xF19D_LEN];
    UINT8 u8_DiagIdF1A8Data[EEPID_DIAGDID_0xF1A8_LEN];
    UINT8 u8_FlhvldData[EEPID_FLHVLD_LEN];

    /* 0x0018~0x02FF，此ID段供DID-Internal使用 */

    UINT8 u8_NvIdE001Data[EEPID_NVID_0xE001_LEN];
    UINT8 u8_NvIdE002Data[EEPID_NVID_0xE002_LEN];
    UINT8 u8_NvIdE003Data[EEPID_NVID_0xE003_LEN];
    UINT8 u8_NvIdE004Data[EEPID_NVID_0xE004_LEN];
    UINT8 u8_NvIdE005Data[EEPID_NVID_0xE005_LEN];
    UINT8 u8_NvIdE006Data[EEPID_NVID_0xE006_LEN];
    UINT8 u8_NvIdE007Data[EEPID_NVID_0xE007_LEN];
    UINT8 u8_NvIdE008Data[EEPID_NVID_0xE008_LEN];
    UINT8 u8_NvIdE009Data[EEPID_NVID_0xE009_LEN];
    UINT8 u8_NvIdE00AData[EEPID_NVID_0xE00A_LEN];
    UINT8 u8_NvIdE00BData[EEPID_NVID_0xE00B_LEN];

    UINT8 u8_CfgId1001Data[EEPID_CFGDID_0x1001_LEN];
    UINT8 u8_CfgId1002Data[EEPID_CFGDID_0x1002_LEN];
    UINT8 u8_CfgId1003Data[EEPID_CFGDID_0x1003_LEN];
    UINT8 u8_CfgId1004Data[EEPID_CFGDID_0x1004_LEN];
    UINT8 u8_CfgId1005Data[EEPID_CFGDID_0x1005_LEN];
    UINT8 u8_CfgId1006Data[EEPID_CFGDID_0x1006_LEN];
    UINT8 u8_CfgId1007Data[EEPID_CFGDID_0x1007_LEN];
    UINT8 u8_CfgId1008Data[EEPID_CFGDID_0x1008_LEN];
    UINT8 u8_CfgId1009Data[EEPID_CFGDID_0x1009_LEN];
    UINT8 u8_CfgId1010Data[EEPID_CFGDID_0x1010_LEN];
    UINT8 u8_CfgId1012Data[EEPID_CFGDID_0x1012_LEN];
    UINT8 u8_CfgId1013Data[EEPID_CFGDID_0x1013_LEN];
    UINT8 u8_CfgId1014Data[EEPID_CFGDID_0x1014_LEN];
    UINT8 u8_CfgId1015Data[EEPID_CFGDID_0x1015_LEN];
    UINT8 u8_CfgId1016Data[EEPID_CFGDID_0x1016_LEN];
    UINT8 u8_CfgId1018Data[EEPID_CFGDID_0x1018_LEN];
    UINT8 u8_CfgId1019Data[EEPID_CFGDID_0x1019_LEN];
    UINT8 u8_CfgId1023Data[EEPID_CFGDID_0x1023_LEN];
    UINT8 u8_CfgId1024Data[EEPID_CFGDID_0x1024_LEN];
    UINT8 u8_CfgId1025Data[EEPID_CFGDID_0x1025_LEN];
    UINT8 u8_CfgId1026Data[EEPID_CFGDID_0x1026_LEN];
    UINT8 u8_CfgId1027Data[EEPID_CFGDID_0x1027_LEN];
    UINT8 u8_CfgId1028Data[EEPID_CFGDID_0x1028_LEN];
    UINT8 u8_CfgId1029Data[EEPID_CFGDID_0x1029_LEN];
    UINT8 u8_CfgId1030Data[EEPID_CFGDID_0x1030_LEN];
    UINT8 u8_CfgId1031Data[EEPID_CFGDID_0x1031_LEN];
    UINT8 u8_CfgId1032Data[EEPID_CFGDID_0x1032_LEN];
    UINT8 u8_CfgId1034Data[EEPID_CFGDID_0x1034_LEN];
    UINT8 u8_CfgId1035Data[EEPID_CFGDID_0x1035_LEN];
    UINT8 u8_CfgId1036Data[EEPID_CFGDID_0x1036_LEN];
    UINT8 u8_CfgId1040Data[EEPID_CFGDID_0x1040_LEN];
    UINT8 u8_CfgId1041Data[EEPID_CFGDID_0x1041_LEN];
    UINT8 u8_CfgId1042Data[EEPID_CFGDID_0x1042_LEN];
    UINT8 u8_CfgId1043Data[EEPID_CFGDID_0x1043_LEN];
    UINT8 u8_CfgId1049Data[EEPID_CFGDID_0x1049_LEN];
    UINT8 u8_CfgId1050Data[EEPID_CFGDID_0x1050_LEN];
    UINT8 u8_CfgId1051Data[EEPID_CFGDID_0x1051_LEN];
    UINT8 u8_CfgId1052Data[EEPID_CFGDID_0x1052_LEN];
    UINT8 u8_CfgId1054Data[EEPID_CFGDID_0x1054_LEN];
    UINT8 u8_CfgId1056Data[EEPID_CFGDID_0x1056_LEN];
    UINT8 u8_CfgId1057Data[EEPID_CFGDID_0x1057_LEN];
    UINT8 u8_CfgId1058Data[EEPID_CFGDID_0x1058_LEN];
    UINT8 u8_CfgId1059Data[EEPID_CFGDID_0x1059_LEN];
    UINT8 u8_CfgId1061Data[EEPID_CFGDID_0x1061_LEN];
    UINT8 u8_CfgId1062Data[EEPID_CFGDID_0x1062_LEN];
    UINT8 u8_CfgId1065Data[EEPID_CFGDID_0x1065_LEN];
    UINT8 u8_CfgId1066Data[EEPID_CFGDID_0x1066_LEN];
    UINT8 u8_CfgId1067Data[EEPID_CFGDID_0x1067_LEN];
    UINT8 u8_CfgId1069Data[EEPID_CFGDID_0x1069_LEN];
    UINT8 u8_CfgId106BData[EEPID_CFGDID_0x106B_LEN];
    UINT8 u8_CfgId106CData[EEPID_CFGDID_0x106C_LEN];
    UINT8 u8_CfgId106EData[EEPID_CFGDID_0x106E_LEN];
    UINT8 u8_CfgId106FData[EEPID_CFGDID_0x106F_LEN];
    UINT8 u8_CfgId1070Data[EEPID_CFGDID_0x1070_LEN];
    UINT8 u8_CfgId1072Data[EEPID_CFGDID_0x1072_LEN];
    UINT8 u8_CfgId1074Data[EEPID_CFGDID_0x1074_LEN];
	UINT8 u8_CfgId1075Data[EEPID_CFGDID_0x1075_LEN];
    UINT8 u8_CfgId1076Data[EEPID_CFGDID_0x1076_LEN];
    UINT8 u8_CfgId1077Data[EEPID_CFGDID_0x1077_LEN];
    UINT8 u8_CfgId1078Data[EEPID_CFGDID_0x1078_LEN];
    UINT8 u8_CfgId1079Data[EEPID_CFGDID_0x1079_LEN];

    UINT8 u8_CfgId6000Data[EEPID_CFGDID_0x6000_LEN];
    UINT8 u8_CfgId6001Data[EEPID_CFGDID_0x6001_LEN];
    UINT8 u8_CfgId6002Data[EEPID_CFGDID_0x6002_LEN];
    UINT8 u8_CfgId6003Data[EEPID_CFGDID_0x6003_LEN];
    UINT8 u8_CfgId6004Data[EEPID_CFGDID_0x6004_LEN];
    UINT8 u8_CfgId6005Data[EEPID_CFGDID_0x6005_LEN];
    UINT8 u8_CfgId6006Data[EEPID_CFGDID_0x6006_LEN];
    UINT8 u8_CfgId6007Data[EEPID_CFGDID_0x6007_LEN];
    UINT8 u8_CfgId6008Data[EEPID_CFGDID_0x6008_LEN];
    UINT8 u8_CfgId6009Data[EEPID_CFGDID_0x6009_LEN];
    UINT8 u8_CfgId600CData[EEPID_CFGDID_0x600C_LEN];
    UINT8 u8_CfgId600DData[EEPID_CFGDID_0x600D_LEN];
    UINT8 u8_CfgId600EData[EEPID_CFGDID_0x600E_LEN];
    UINT8 u8_CfgId600FData[EEPID_CFGDID_0x600F_LEN];
    UINT8 u8_CfgId6010Data[EEPID_CFGDID_0x6010_LEN];
    UINT8 u8_CfgId6011Data[EEPID_CFGDID_0x6011_LEN];
    UINT8 u8_CfgId6013Data[EEPID_CFGDID_0x6013_LEN];
    UINT8 u8_CfgId6014Data[EEPID_CFGDID_0x6014_LEN];
    UINT8 u8_CfgId6015Data[EEPID_CFGDID_0x6015_LEN];
    UINT8 u8_CfgId6016Data[EEPID_CFGDID_0x6016_LEN];
    UINT8 u8_CfgId6017Data[EEPID_CFGDID_0x6017_LEN];
    UINT8 u8_CfgId6018Data[EEPID_CFGDID_0x6018_LEN];
    UINT8 u8_CfgId6019Data[EEPID_CFGDID_0x6019_LEN];
    UINT8 u8_CfgId601AData[EEPID_CFGDID_0x601A_LEN];
    UINT8 u8_CfgId601BData[EEPID_CFGDID_0x601B_LEN];
    UINT8 u8_CfgId601CData[EEPID_CFGDID_0x601C_LEN];
    UINT8 u8_CfgId601DData[EEPID_CFGDID_0x601D_LEN];
    UINT8 u8_CfgId601EData[EEPID_CFGDID_0x601E_LEN];
    UINT8 u8_CfgId601FData[EEPID_CFGDID_0x601F_LEN];
    UINT8 u8_CfgId6020Data[EEPID_CFGDID_0x6020_LEN];
    UINT8 u8_CfgId6021Data[EEPID_CFGDID_0x6021_LEN];
    UINT8 u8_CfgId6022Data[EEPID_CFGDID_0x6022_LEN];
    UINT8 u8_CfgId6024Data[EEPID_CFGDID_0x6024_LEN];
    UINT8 u8_CfgId6025Data[EEPID_CFGDID_0x6025_LEN];
    UINT8 u8_CfgId6026Data[EEPID_CFGDID_0x6026_LEN];
    UINT8 u8_CfgId602AData[EEPID_CFGDID_0x602A_LEN];
    UINT8 u8_CfgId602BData[EEPID_CFGDID_0x602B_LEN];
    UINT8 u8_CfgId602DData[EEPID_CFGDID_0x602D_LEN];
    UINT8 u8_CfgId602EData[EEPID_CFGDID_0x602E_LEN];
    UINT8 u8_CfgId6030Data[EEPID_CFGDID_0x6030_LEN];
    UINT8 u8_CfgId6031Data[EEPID_CFGDID_0x6031_LEN];
    UINT8 u8_CfgId6032Data[EEPID_CFGDID_0x6032_LEN];
    UINT8 u8_CfgId6033Data[EEPID_CFGDID_0x6033_LEN];
    UINT8 u8_CfgId6034Data[EEPID_CFGDID_0x6034_LEN];
    UINT8 u8_CfgId6035Data[EEPID_CFGDID_0x6035_LEN];
    UINT8 u8_CfgId6038Data[EEPID_CFGDID_0x6038_LEN];
    UINT8 u8_CfgId603AData[EEPID_CFGDID_0x603A_LEN];
    UINT8 u8_CfgId603BData[EEPID_CFGDID_0x603B_LEN];
    UINT8 u8_CfgId603CData[EEPID_CFGDID_0x603C_LEN];
    UINT8 u8_CfgId603DData[EEPID_CFGDID_0x603D_LEN];
    UINT8 u8_CfgId6040Data[EEPID_CFGDID_0x6041_LEN];
    UINT8 u8_CfgId6043Data[EEPID_CFGDID_0x6043_LEN];
    UINT8 u8_CfgId6046Data[EEPID_CFGDID_0x6046_LEN];
    UINT8 u8_CfgId604FData[EEPID_CFGDID_0x604F_LEN];
    UINT8 u8_CfgId6051Data[EEPID_CFGDID_0x6051_LEN];
    UINT8 u8_CfgId6052Data[EEPID_CFGDID_0x6052_LEN];
    UINT8 u8_CfgId6054Data[EEPID_CFGDID_0x6054_LEN];
    UINT8 u8_CfgId6055Data[EEPID_CFGDID_0x6055_LEN];

    UINT8 u8_CfgId2008Data[EEPID_CFGDID_0x2008_LEN];
    UINT8 u8_CfgId200AData[EEPID_CFGDID_0x200A_LEN];
    UINT8 u8_CfgId200CData[EEPID_CFGDID_0x200C_LEN];
    UINT8 u8_CfgId200DData[EEPID_CFGDID_0x200D_LEN];
    UINT8 u8_CfgId2010Data[EEPID_CFGDID_0x2010_LEN];
    UINT8 u8_CfgId2011Data[EEPID_CFGDID_0x2011_LEN];
    UINT8 u8_CfgId2012Data[EEPID_CFGDID_0x2012_LEN];
    UINT8 u8_CfgId2013Data[EEPID_CFGDID_0x2013_LEN];
    UINT8 u8_CfgId201AData[EEPID_CFGDID_0x201A_LEN];
    UINT8 u8_CfgId201DData[EEPID_CFGDID_0x201D_LEN];

    UINT8 u8_NvIdE00CData[EEPID_NVID_0xE00C_LEN];
	UINT8 u8_NvIdE00DData[EEPID_NVID_0xE00D_LEN];
    UINT8 u8_NvIdE00EData[EEPID_NVID_0xE00E_LEN];
    UINT8 u8_NvIdE00FData[EEPID_NVID_0xE00F_LEN];
    UINT8 u8_NvIdE010Data[EEPID_NVID_0xE010_LEN];
    UINT8 u8_NvIdE011Data[EEPID_NVID_0xE011_LEN];
    UINT8 u8_NvIdE012Data[EEPID_NVID_0xE012_LEN];

    UINT8 u8_DiagIdB000Data[EEPID_DIAGDID_0xB000_LEN];
    UINT8 u8_DiagIdB010Data[EEPID_DIAGDID_0xB010_LEN];
    UINT8 u8_DiagIdB011Data[EEPID_DIAGDID_0xB011_LEN];
    UINT8 u8_DiagIdB02AData[EEPID_DIAGDID_0xB02A_LEN];
    UINT8 u8_DiagIdB02BData[EEPID_DIAGDID_0xB02B_LEN];
    UINT8 u8_DiagIdB300Data[EEPID_DIAGDID_0xB300_LEN];
    UINT8 u8_DiagIdB301Data[EEPID_DIAGDID_0xB301_LEN];
    UINT8 u8_DiagIdB302Data[EEPID_DIAGDID_0xB302_LEN];
    UINT8 u8_DiagIdB303Data[EEPID_DIAGDID_0xB303_LEN];
    UINT8 u8_DiagIdB0B2Data[EEPID_DIAGDID_0xB0B2_LEN];
	
    UINT8 u8_DiagIdF201Data[EEPID_DIAGDID_0xF201_LEN];
    UINT8 u8_DiagIdF202Data[EEPID_DIAGDID_0xF202_LEN];

    UINT8 u8_NvIdE013Data[EEPID_NVID_0xE013_LEN];
    UINT8 u8_NvIdE014Data[EEPID_NVID_0xE014_LEN];
    UINT8 u8_NvIdE015Data[EEPID_NVID_0xE015_LEN];
    UINT8 u8_NvIdE016Data[EEPID_NVID_0xE016_LEN];
    UINT8 u8_NvIdE017Data[EEPID_NVID_0xE017_LEN];
    UINT8 u8_NvIdE018Data[EEPID_NVID_0xE018_LEN];
    UINT8 u8_NvIdE019Data[EEPID_NVID_0xE019_LEN];
    UINT8 u8_NvIdE01AData[EEPID_NVID_0xE01A_LEN];
    UINT8 u8_NvIdE01BData[EEPID_NVID_0xE01B_LEN];
    UINT8 u8_NvIdE01CData[EEPID_NVID_0xE01C_LEN];
    UINT8 u8_NvIdE01DData[EEPID_NVID_0xE01D_LEN];
    UINT8 u8_NvIdE01EData[EEPID_NVID_0xE01E_LEN];
    UINT8 u8_NvIdE01FData[EEPID_NVID_0xE01F_LEN];
    UINT8 u8_NvIdE020Data[EEPID_NVID_0xE020_LEN];
    UINT8 u8_NvIdE021Data[EEPID_NVID_0xE021_LEN];
    UINT8 u8_NvIdE022Data[EEPID_NVID_0xE022_LEN];
    UINT8 u8_NvIdE023Data[EEPID_NVID_0xE023_LEN];
    UINT8 u8_NvIdE024Data[EEPID_NVID_0xE024_LEN];
    UINT8 u8_NvIdE025Data[EEPID_NVID_0xE025_LEN];
    UINT8 u8_NvIdE026Data[EEPID_NVID_0xE026_LEN];
    UINT8 u8_NvIdE027Data[EEPID_NVID_0xE027_LEN];
    UINT8 u8_NvIdE028Data[EEPID_NVID_0xE028_LEN];

    UINT8 u8_CfgId6012Data[EEPID_CFGDID_0x6012_LEN];
    UINT8 u8_CfgId602CData[EEPID_CFGDID_0x602C_LEN];

    UINT8 u8_DCTData[EEPID_DTCDATA_LEN];
    UINT8 u8_SecurityAccessFlagData[EEPID_SECURITY_ACCESSS_FLAG_LEN];

    UINT8 u8_EolFlagData[EEPID_EOL_FLAG_LEN];
    UINT8 u8_EolLockFlagData[EEPID_EOL_LOCK_FLAG_LEN];
    UINT8 u8_EolVbatGainData[EEPID_EOL_VBAT_GAIN_LEN];
    UINT8 u8_EolVbatOffsetData[EEPID_EOL_VBAT_OFFSET_LEN];
    UINT8 u8_EolCrcData[EEPID_EOL_CRCDATA_LEN];
    UINT8 u8_EolTraceNumData[EEPID_EOL_Trace_Num_LEN];

    UINT8 u8_AuxSleepTimeData[EEPID_AUX_SLEEPTIME_LEN];
    UINT8 u8_AuxSleepTimeBackupData[EEPID_AUX_SLEEPTIMEBACKUP_LEN];
    UINT8 u8_UserIdData[EEPID_USERID_LEN];
    UINT8 u8_UserIdBackupData[EEPID_USERIDBACKUP_LEN];
    UINT8 u8_TTokenData[EEPID_TTOKEN_LEN];
    UINT8 u8_TTokenBackupData[EEPID_TTOKENBACKUP_LEN];
    UINT8 u8_AuxAlarmTimeUtcData[EEPID_AUX_ALARMTIMEUTC_LEN];
    UINT8 u8_AuxAlarmTimeUtcBackupData[EEPID_AUX_ALARMTIMEUTCBACKUP_LEN];
    UINT8 u8_ImmoData[EEPID_IMMODATA_LEN];
    UINT8 u8_ImmoBackupData[EEPID_IMMODATABACKUP_LEN];
    UINT8 u8_NxpKeyk1Data[EEPID_NXP_KEYK1_LEN];
    UINT8 u8_NxpKeyk1BackupData[EEPID_NXP_KEYK1BACKUP_LEN];
    UINT8 u8_UserCertificateData[EEPID_USERCERTIFICATE_LEN];
    UINT8 u8_UserCertificateBackupData[EEPID_USERCERTIFICATEBACKUP_LEN];
    UINT8 u8_PublicKeyXData[EEPID_PUBLICKEY_X_LEN];
    UINT8 u8_PublicKeyXBackuData[EEPID_PUBLICKEY_X_BACKUP_LEN];
    UINT8 u8_PublicKeyYData[EEPID_PUBLICKEY_Y_LEN];
    UINT8 u8_PublicKeyYBackupData[EEPID_PUBLICKEY_Y_BACKUP_LEN];
    UINT8 u8_SecretKeyData[EEPID_SECRETKEY_LEN];
    UINT8 u8_SecretKeyBackupData[EEPID_SECRETKEYBACKUP_LEN];
    UINT8 u8_RVSRequestIDData[EEPID_RVSREQUESTID_LEN];
    UINT8 u8_WakeUpSourceData[EEPID_WAKEUPSOURCE_LEN];
}MemEeprom_Data; 

/*****************************************************************************
** Constant Macro Definition
*****************************************************************************/


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
extern const UINT16 u16_e2_data_num;
extern const MemEeprom_Map c_st_e2DataMap[];

extern const MemEeprom_Data *const p_e2_Data;
extern const UINT8* const p_e2_ram;

extern const UINT32 const u32_e2_byte_num;
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

#endif	//__MEM_EE_DATA_H__

/*****************************************************************************
** End File
*****************************************************************************/


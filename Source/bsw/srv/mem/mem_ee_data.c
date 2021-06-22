/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    mem_ee_data.c
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

/*****************************************************************************
** #include 
*****************************************************************************/
#include "mem_ee_data.h"
#include <stddef.h>
/*****************************************************************************
** #define
*****************************************************************************/
#define FillFF                      0xFFFFFFFF
#define Fill00                      0x00000000

#define OFFSET_DIAGDID_0xF186        (offsetof(MemEeprom_Data,u8_DiagIdF186Data))
#define OFFSET_DIAGDID_0xF187        (offsetof(MemEeprom_Data,u8_DiagIdF187Data))
#define OFFSET_DIAGDID_0xF188        (offsetof(MemEeprom_Data,u8_DiagIdF188Data))
#define OFFSET_DIAGDID_0xF189        (offsetof(MemEeprom_Data,u8_DiagIdF189Data))
#define OFFSET_DIAGDID_0xF18A        (offsetof(MemEeprom_Data,u8_DiagIdF18AData))
#define OFFSET_DIAGDID_0xF18B        (offsetof(MemEeprom_Data,u8_DiagIdF18BData))
#define OFFSET_DIAGDID_0xF18C        (offsetof(MemEeprom_Data,u8_DiagIdF18CData))
#define OFFSET_DIAGDID_0xF190        (offsetof(MemEeprom_Data,u8_DiagIdF190Data))
#define OFFSET_DIAGDID_0xF192        (offsetof(MemEeprom_Data,u8_DiagIdF192Data))
#define OFFSET_DIAGDID_0xF193        (offsetof(MemEeprom_Data,u8_DiagIdF193Data))
#define OFFSET_DIAGDID_0xF194        (offsetof(MemEeprom_Data,u8_DiagIdF194Data))
#define OFFSET_DIAGDID_0xF195        (offsetof(MemEeprom_Data,u8_DiagIdF195Data))
#define OFFSET_DIAGDID_0xF197        (offsetof(MemEeprom_Data,u8_DiagIdF197Data))
#define OFFSET_DIAGDID_0xF198        (offsetof(MemEeprom_Data,u8_DiagIdF198Data))
#define OFFSET_DIAGDID_0xF199        (offsetof(MemEeprom_Data,u8_DiagIdF199Data))
#define OFFSET_DIAGDID_0xF19D        (offsetof(MemEeprom_Data,u8_DiagIdF19DData))
#define OFFSET_DIAGDID_0xF1A8        (offsetof(MemEeprom_Data,u8_DiagIdF1A8Data))
#define OFFSET_FLHVLD        		 (offsetof(MemEeprom_Data,u8_FlhvldData))

#define OFFSET_NVID_0xE001        (offsetof(MemEeprom_Data,u8_NvIdE001Data))
#define OFFSET_NVID_0xE002        (offsetof(MemEeprom_Data,u8_NvIdE002Data))
#define OFFSET_NVID_0xE003        (offsetof(MemEeprom_Data,u8_NvIdE003Data))
#define OFFSET_NVID_0xE004        (offsetof(MemEeprom_Data,u8_NvIdE004Data))
#define OFFSET_NVID_0xE005        (offsetof(MemEeprom_Data,u8_NvIdE005Data))
#define OFFSET_NVID_0xE006        (offsetof(MemEeprom_Data,u8_NvIdE006Data))
#define OFFSET_NVID_0xE007        (offsetof(MemEeprom_Data,u8_NvIdE007Data))
#define OFFSET_NVID_0xE008        (offsetof(MemEeprom_Data,u8_NvIdE008Data))
#define OFFSET_NVID_0xE009        (offsetof(MemEeprom_Data,u8_NvIdE009Data))
#define OFFSET_NVID_0xE00A        (offsetof(MemEeprom_Data,u8_NvIdE00AData))
#define OFFSET_NVID_0xE00B        (offsetof(MemEeprom_Data,u8_NvIdE00BData))

#define OFFSET_CFGDID_0x1001        (offsetof(MemEeprom_Data,u8_CfgId1001Data))
#define OFFSET_CFGDID_0x1002        (offsetof(MemEeprom_Data,u8_CfgId1002Data))
#define OFFSET_CFGDID_0x1003        (offsetof(MemEeprom_Data,u8_CfgId1003Data))
#define OFFSET_CFGDID_0x1004        (offsetof(MemEeprom_Data,u8_CfgId1004Data))
#define OFFSET_CFGDID_0x1005        (offsetof(MemEeprom_Data,u8_CfgId1005Data))
#define OFFSET_CFGDID_0x1006        (offsetof(MemEeprom_Data,u8_CfgId1006Data))
#define OFFSET_CFGDID_0x1007        (offsetof(MemEeprom_Data,u8_CfgId1007Data))
#define OFFSET_CFGDID_0x1008        (offsetof(MemEeprom_Data,u8_CfgId1008Data))
#define OFFSET_CFGDID_0x1009        (offsetof(MemEeprom_Data,u8_CfgId1009Data))
#define OFFSET_CFGDID_0x1010        (offsetof(MemEeprom_Data,u8_CfgId1010Data))
#define OFFSET_CFGDID_0x1012        (offsetof(MemEeprom_Data,u8_CfgId1012Data))
#define OFFSET_CFGDID_0x1013        (offsetof(MemEeprom_Data,u8_CfgId1013Data))
#define OFFSET_CFGDID_0x1014        (offsetof(MemEeprom_Data,u8_CfgId1014Data))
#define OFFSET_CFGDID_0x1015        (offsetof(MemEeprom_Data,u8_CfgId1015Data))
#define OFFSET_CFGDID_0x1016        (offsetof(MemEeprom_Data,u8_CfgId1016Data))
#define OFFSET_CFGDID_0x1018        (offsetof(MemEeprom_Data,u8_CfgId1018Data))
#define OFFSET_CFGDID_0x1019        (offsetof(MemEeprom_Data,u8_CfgId1019Data))
#define OFFSET_CFGDID_0x1023        (offsetof(MemEeprom_Data,u8_CfgId1023Data))
#define OFFSET_CFGDID_0x1024        (offsetof(MemEeprom_Data,u8_CfgId1024Data))
#define OFFSET_CFGDID_0x1025        (offsetof(MemEeprom_Data,u8_CfgId1025Data))
#define OFFSET_CFGDID_0x1026        (offsetof(MemEeprom_Data,u8_CfgId1026Data))
#define OFFSET_CFGDID_0x1027        (offsetof(MemEeprom_Data,u8_CfgId1027Data))
#define OFFSET_CFGDID_0x1028        (offsetof(MemEeprom_Data,u8_CfgId1028Data))
#define OFFSET_CFGDID_0x1029        (offsetof(MemEeprom_Data,u8_CfgId1029Data))
#define OFFSET_CFGDID_0x1030        (offsetof(MemEeprom_Data,u8_CfgId1030Data))
#define OFFSET_CFGDID_0x1031        (offsetof(MemEeprom_Data,u8_CfgId1031Data))
#define OFFSET_CFGDID_0x1032        (offsetof(MemEeprom_Data,u8_CfgId1032Data))
#define OFFSET_CFGDID_0x1034        (offsetof(MemEeprom_Data,u8_CfgId1034Data))
#define OFFSET_CFGDID_0x1035        (offsetof(MemEeprom_Data,u8_CfgId1035Data))
#define OFFSET_CFGDID_0x1036        (offsetof(MemEeprom_Data,u8_CfgId1036Data))
#define OFFSET_CFGDID_0x1040        (offsetof(MemEeprom_Data,u8_CfgId1040Data))
#define OFFSET_CFGDID_0x1041        (offsetof(MemEeprom_Data,u8_CfgId1041Data))
#define OFFSET_CFGDID_0x1042        (offsetof(MemEeprom_Data,u8_CfgId1042Data))
#define OFFSET_CFGDID_0x1043        (offsetof(MemEeprom_Data,u8_CfgId1043Data))
#define OFFSET_CFGDID_0x1049        (offsetof(MemEeprom_Data,u8_CfgId1049Data))
#define OFFSET_CFGDID_0x1050        (offsetof(MemEeprom_Data,u8_CfgId1050Data))
#define OFFSET_CFGDID_0x1051        (offsetof(MemEeprom_Data,u8_CfgId1051Data))
#define OFFSET_CFGDID_0x1052        (offsetof(MemEeprom_Data,u8_CfgId1052Data))
#define OFFSET_CFGDID_0x1054        (offsetof(MemEeprom_Data,u8_CfgId1054Data))
#define OFFSET_CFGDID_0x1056        (offsetof(MemEeprom_Data,u8_CfgId1056Data))
#define OFFSET_CFGDID_0x1057        (offsetof(MemEeprom_Data,u8_CfgId1057Data))
#define OFFSET_CFGDID_0x1058        (offsetof(MemEeprom_Data,u8_CfgId1058Data))
#define OFFSET_CFGDID_0x1059        (offsetof(MemEeprom_Data,u8_CfgId1059Data))
#define OFFSET_CFGDID_0x1061        (offsetof(MemEeprom_Data,u8_CfgId1061Data))
#define OFFSET_CFGDID_0x1062        (offsetof(MemEeprom_Data,u8_CfgId1062Data))
#define OFFSET_CFGDID_0x1065        (offsetof(MemEeprom_Data,u8_CfgId1065Data))
#define OFFSET_CFGDID_0x1066        (offsetof(MemEeprom_Data,u8_CfgId1066Data))
#define OFFSET_CFGDID_0x1067        (offsetof(MemEeprom_Data,u8_CfgId1067Data))
#define OFFSET_CFGDID_0x1069        (offsetof(MemEeprom_Data,u8_CfgId1069Data))
#define OFFSET_CFGDID_0x106B        (offsetof(MemEeprom_Data,u8_CfgId106BData))
#define OFFSET_CFGDID_0x106C        (offsetof(MemEeprom_Data,u8_CfgId106CData))
#define OFFSET_CFGDID_0x106E        (offsetof(MemEeprom_Data,u8_CfgId106EData))
#define OFFSET_CFGDID_0x106F        (offsetof(MemEeprom_Data,u8_CfgId106FData))
#define OFFSET_CFGDID_0x1070        (offsetof(MemEeprom_Data,u8_CfgId1070Data))
#define OFFSET_CFGDID_0x1072        (offsetof(MemEeprom_Data,u8_CfgId1072Data))
#define OFFSET_CFGDID_0x1074        (offsetof(MemEeprom_Data,u8_CfgId1074Data))
#define OFFSET_CFGDID_0x1075        (offsetof(MemEeprom_Data,u8_CfgId1075Data))
#define OFFSET_CFGDID_0x1076        (offsetof(MemEeprom_Data,u8_CfgId1076Data))
#define OFFSET_CFGDID_0x1077        (offsetof(MemEeprom_Data,u8_CfgId1077Data))
#define OFFSET_CFGDID_0x1078        (offsetof(MemEeprom_Data,u8_CfgId1078Data))
#define OFFSET_CFGDID_0x1079        (offsetof(MemEeprom_Data,u8_CfgId1079Data))

#define OFFSET_CFGDID_0x6000        (offsetof(MemEeprom_Data,u8_CfgId6000Data))
#define OFFSET_CFGDID_0x6001        (offsetof(MemEeprom_Data,u8_CfgId6001Data))
#define OFFSET_CFGDID_0x6002        (offsetof(MemEeprom_Data,u8_CfgId6002Data))
#define OFFSET_CFGDID_0x6003        (offsetof(MemEeprom_Data,u8_CfgId6003Data))
#define OFFSET_CFGDID_0x6004        (offsetof(MemEeprom_Data,u8_CfgId6004Data))
#define OFFSET_CFGDID_0x6005        (offsetof(MemEeprom_Data,u8_CfgId6005Data))
#define OFFSET_CFGDID_0x6006        (offsetof(MemEeprom_Data,u8_CfgId6006Data))
#define OFFSET_CFGDID_0x6007        (offsetof(MemEeprom_Data,u8_CfgId6007Data))
#define OFFSET_CFGDID_0x6008        (offsetof(MemEeprom_Data,u8_CfgId6008Data))
#define OFFSET_CFGDID_0x6009        (offsetof(MemEeprom_Data,u8_CfgId6009Data))
#define OFFSET_CFGDID_0x600C        (offsetof(MemEeprom_Data,u8_CfgId600CData))
#define OFFSET_CFGDID_0x600D        (offsetof(MemEeprom_Data,u8_CfgId600DData))
#define OFFSET_CFGDID_0x600E        (offsetof(MemEeprom_Data,u8_CfgId600EData))
#define OFFSET_CFGDID_0x600F        (offsetof(MemEeprom_Data,u8_CfgId600FData))
#define OFFSET_CFGDID_0x6010        (offsetof(MemEeprom_Data,u8_CfgId6010Data))
#define OFFSET_CFGDID_0x6011        (offsetof(MemEeprom_Data,u8_CfgId6011Data))
#define OFFSET_CFGDID_0x6013        (offsetof(MemEeprom_Data,u8_CfgId6013Data))
#define OFFSET_CFGDID_0x6014        (offsetof(MemEeprom_Data,u8_CfgId6014Data))
#define OFFSET_CFGDID_0x6015        (offsetof(MemEeprom_Data,u8_CfgId6015Data))
#define OFFSET_CFGDID_0x6016        (offsetof(MemEeprom_Data,u8_CfgId6016Data))
#define OFFSET_CFGDID_0x6017        (offsetof(MemEeprom_Data,u8_CfgId6017Data))
#define OFFSET_CFGDID_0x6018        (offsetof(MemEeprom_Data,u8_CfgId6018Data))
#define OFFSET_CFGDID_0x6019        (offsetof(MemEeprom_Data,u8_CfgId6019Data))
#define OFFSET_CFGDID_0x601A        (offsetof(MemEeprom_Data,u8_CfgId601AData))
#define OFFSET_CFGDID_0x601B        (offsetof(MemEeprom_Data,u8_CfgId601BData))
#define OFFSET_CFGDID_0x601C        (offsetof(MemEeprom_Data,u8_CfgId601CData))
#define OFFSET_CFGDID_0x601D        (offsetof(MemEeprom_Data,u8_CfgId601DData))
#define OFFSET_CFGDID_0x601E        (offsetof(MemEeprom_Data,u8_CfgId601EData))
#define OFFSET_CFGDID_0x601F        (offsetof(MemEeprom_Data,u8_CfgId601FData))
#define OFFSET_CFGDID_0x6020        (offsetof(MemEeprom_Data,u8_CfgId6020Data))
#define OFFSET_CFGDID_0x6021        (offsetof(MemEeprom_Data,u8_CfgId6021Data))
#define OFFSET_CFGDID_0x6022        (offsetof(MemEeprom_Data,u8_CfgId6022Data))
#define OFFSET_CFGDID_0x6024        (offsetof(MemEeprom_Data,u8_CfgId6024Data))
#define OFFSET_CFGDID_0x6025        (offsetof(MemEeprom_Data,u8_CfgId6025Data))
#define OFFSET_CFGDID_0x6026        (offsetof(MemEeprom_Data,u8_CfgId6026Data))
#define OFFSET_CFGDID_0x602A        (offsetof(MemEeprom_Data,u8_CfgId602AData))
#define OFFSET_CFGDID_0x602B        (offsetof(MemEeprom_Data,u8_CfgId602BData))
#define OFFSET_CFGDID_0x602D        (offsetof(MemEeprom_Data,u8_CfgId602DData))
#define OFFSET_CFGDID_0x602E        (offsetof(MemEeprom_Data,u8_CfgId602EData))
#define OFFSET_CFGDID_0x6030        (offsetof(MemEeprom_Data,u8_CfgId6030Data))
#define OFFSET_CFGDID_0x6031        (offsetof(MemEeprom_Data,u8_CfgId6031Data))
#define OFFSET_CFGDID_0x6032        (offsetof(MemEeprom_Data,u8_CfgId6032Data))
#define OFFSET_CFGDID_0x6033        (offsetof(MemEeprom_Data,u8_CfgId6033Data))
#define OFFSET_CFGDID_0x6034        (offsetof(MemEeprom_Data,u8_CfgId6034Data))
#define OFFSET_CFGDID_0x6035        (offsetof(MemEeprom_Data,u8_CfgId6035Data))
#define OFFSET_CFGDID_0x6038        (offsetof(MemEeprom_Data,u8_CfgId6038Data))
#define OFFSET_CFGDID_0x603A        (offsetof(MemEeprom_Data,u8_CfgId603AData))
#define OFFSET_CFGDID_0x603B        (offsetof(MemEeprom_Data,u8_CfgId603BData))
#define OFFSET_CFGDID_0x603C        (offsetof(MemEeprom_Data,u8_CfgId603CData))
#define OFFSET_CFGDID_0x603D        (offsetof(MemEeprom_Data,u8_CfgId603DData))
#define OFFSET_CFGDID_0x6041        (offsetof(MemEeprom_Data,u8_CfgId6040Data))
#define OFFSET_CFGDID_0x6043        (offsetof(MemEeprom_Data,u8_CfgId6043Data))
#define OFFSET_CFGDID_0x6046        (offsetof(MemEeprom_Data,u8_CfgId6046Data))
#define OFFSET_CFGDID_0x604F        (offsetof(MemEeprom_Data,u8_CfgId604FData))
#define OFFSET_CFGDID_0x6051        (offsetof(MemEeprom_Data,u8_CfgId6051Data))
#define OFFSET_CFGDID_0x6052        (offsetof(MemEeprom_Data,u8_CfgId6052Data))
#define OFFSET_CFGDID_0x6054        (offsetof(MemEeprom_Data,u8_CfgId6054Data))
#define OFFSET_CFGDID_0x6055        (offsetof(MemEeprom_Data,u8_CfgId6055Data))

#define OFFSET_CFGDID_0x2008        (offsetof(MemEeprom_Data,u8_CfgId2008Data))
#define OFFSET_CFGDID_0x200A        (offsetof(MemEeprom_Data,u8_CfgId200AData))
#define OFFSET_CFGDID_0x200C        (offsetof(MemEeprom_Data,u8_CfgId200CData))
#define OFFSET_CFGDID_0x200D        (offsetof(MemEeprom_Data,u8_CfgId200DData))
#define OFFSET_CFGDID_0x2010        (offsetof(MemEeprom_Data,u8_CfgId2010Data))
#define OFFSET_CFGDID_0x2011        (offsetof(MemEeprom_Data,u8_CfgId2011Data))
#define OFFSET_CFGDID_0x2012        (offsetof(MemEeprom_Data,u8_CfgId2012Data))
#define OFFSET_CFGDID_0x2013        (offsetof(MemEeprom_Data,u8_CfgId2013Data))
#define OFFSET_CFGDID_0x201A        (offsetof(MemEeprom_Data,u8_CfgId201AData))
#define OFFSET_CFGDID_0x201D        (offsetof(MemEeprom_Data,u8_CfgId201DData))


#define OFFSET_NVID_0xE00C        (offsetof(MemEeprom_Data,u8_NvIdE00CData))
#define OFFSET_NVID_0xE00D        (offsetof(MemEeprom_Data,u8_NvIdE00DData))
#define OFFSET_NVID_0xE00E        (offsetof(MemEeprom_Data,u8_NvIdE00EData))
#define OFFSET_NVID_0xE00F        (offsetof(MemEeprom_Data,u8_NvIdE00FData))
#define OFFSET_NVID_0xE010        (offsetof(MemEeprom_Data,u8_NvIdE010Data))
#define OFFSET_NVID_0xE011        (offsetof(MemEeprom_Data,u8_NvIdE011Data))
#define OFFSET_NVID_0xE012        (offsetof(MemEeprom_Data,u8_NvIdE012Data))

#define OFFSET_DIAGDID_0xB000        (offsetof(MemEeprom_Data,u8_DiagIdB000Data))
#define OFFSET_DIAGDID_0xB010        (offsetof(MemEeprom_Data,u8_DiagIdB010Data))
#define OFFSET_DIAGDID_0xB011        (offsetof(MemEeprom_Data,u8_DiagIdB011Data))
#define OFFSET_DIAGDID_0xB02A        (offsetof(MemEeprom_Data,u8_DiagIdB02AData))
#define OFFSET_DIAGDID_0xB02B        (offsetof(MemEeprom_Data,u8_DiagIdB02BData))
#define OFFSET_DIAGDID_0xB300        (offsetof(MemEeprom_Data,u8_DiagIdB300Data))
#define OFFSET_DIAGDID_0xB301        (offsetof(MemEeprom_Data,u8_DiagIdB301Data))
#define OFFSET_DIAGDID_0xB302        (offsetof(MemEeprom_Data,u8_DiagIdB302Data))
#define OFFSET_DIAGDID_0xB303        (offsetof(MemEeprom_Data,u8_DiagIdB303Data))
#define OFFSET_DIAGDID_0xB0B2        (offsetof(MemEeprom_Data,u8_DiagIdB0B2Data))

#define OFFSET_DIAGDID_0xF201        (offsetof(MemEeprom_Data,u8_CfgId6012Data))
#define OFFSET_DIAGDID_0xF202        (offsetof(MemEeprom_Data,u8_CfgId602CData))

#define OFFSET_NVID_0xE013        (offsetof(MemEeprom_Data,u8_NvIdE013Data))
#define OFFSET_NVID_0xE014        (offsetof(MemEeprom_Data,u8_NvIdE014Data))
#define OFFSET_NVID_0xE015        (offsetof(MemEeprom_Data,u8_NvIdE015Data))
#define OFFSET_NVID_0xE016        (offsetof(MemEeprom_Data,u8_NvIdE016Data))
#define OFFSET_NVID_0xE017        (offsetof(MemEeprom_Data,u8_NvIdE017Data))
#define OFFSET_NVID_0xE018        (offsetof(MemEeprom_Data,u8_NvIdE018Data))
#define OFFSET_NVID_0xE019        (offsetof(MemEeprom_Data,u8_NvIdE019Data))
#define OFFSET_NVID_0xE01A        (offsetof(MemEeprom_Data,u8_NvIdE01AData))
#define OFFSET_NVID_0xE01B        (offsetof(MemEeprom_Data,u8_NvIdE01BData))
#define OFFSET_NVID_0xE01C        (offsetof(MemEeprom_Data,u8_NvIdE01CData))
#define OFFSET_NVID_0xE01D        (offsetof(MemEeprom_Data,u8_NvIdE01DData))
#define OFFSET_NVID_0xE01E        (offsetof(MemEeprom_Data,u8_NvIdE01EData))
#define OFFSET_NVID_0xE01F        (offsetof(MemEeprom_Data,u8_NvIdE01FData))
#define OFFSET_NVID_0xE020        (offsetof(MemEeprom_Data,u8_NvIdE020Data))
#define OFFSET_NVID_0xE021        (offsetof(MemEeprom_Data,u8_NvIdE021Data))
#define OFFSET_NVID_0xE022        (offsetof(MemEeprom_Data,u8_NvIdE022Data))
#define OFFSET_NVID_0xE023        (offsetof(MemEeprom_Data,u8_NvIdE023Data))
#define OFFSET_NVID_0xE024        (offsetof(MemEeprom_Data,u8_NvIdE024Data))
#define OFFSET_NVID_0xE025        (offsetof(MemEeprom_Data,u8_NvIdE025Data))
#define OFFSET_NVID_0xE026        (offsetof(MemEeprom_Data,u8_NvIdE026Data))
#define OFFSET_NVID_0xE027        (offsetof(MemEeprom_Data,u8_NvIdE027Data))
#define OFFSET_NVID_0xE028        (offsetof(MemEeprom_Data,u8_NvIdE028Data))

#define OFFSET_CFGDID_0x6012        (offsetof(MemEeprom_Data,u8_CfgId6012Data))
#define OFFSET_CFGDID_0x602C        (offsetof(MemEeprom_Data,u8_CfgId602CData))

#define OFFSET_DTCDATA        			(offsetof(MemEeprom_Data,u8_DCTData))
#define OFFSET_SECURITY_ACCESSS_FLAG     (offsetof(MemEeprom_Data,u8_SecurityAccessFlagData))

#define OFFSET_EOL_FLAG              	(offsetof(MemEeprom_Data,u8_EolFlagData))
#define OFFSET_EOL_LOCK_FLAG         	(offsetof(MemEeprom_Data,u8_EolLockFlagData))
#define OFFSET_EOL_VBAT_GAIN         	(offsetof(MemEeprom_Data,u8_EolVbatGainData))
#define OFFSET_EOL_VBAT_OFFSET      	(offsetof(MemEeprom_Data,u8_EolVbatOffsetData))
#define OFFSET_EOL_CRCDATA          	 (offsetof(MemEeprom_Data,u8_EolCrcData))
#define OFFSET_EOL_Trace_Num        	(offsetof(MemEeprom_Data,u8_EolTraceNumData))


#define OFFSET_AUX_SLEEPTIME              (offsetof(MemEeprom_Data,u8_AuxSleepTimeData))
#define OFFSET_AUX_SLEEPTIMEBACKUP       (offsetof(MemEeprom_Data,u8_AuxSleepTimeBackupData))
#define OFFSET_USERID                     (offsetof(MemEeprom_Data,u8_UserIdData))
#define OFFSET_USERIDBACKUP               (offsetof(MemEeprom_Data,u8_UserIdBackupData))
#define OFFSET_TTOKEN                     (offsetof(MemEeprom_Data,u8_TTokenData))
#define OFFSET_TTOKENBACKUP               (offsetof(MemEeprom_Data,u8_TTokenBackupData))
#define OFFSET_AUX_ALARMTIMEUTC           (offsetof(MemEeprom_Data,u8_AuxAlarmTimeUtcData))
#define OFFSET_AUX_ALARMTIMEUTCBACKUP        (offsetof(MemEeprom_Data,u8_AuxAlarmTimeUtcBackupData))
#define OFFSET_IMMODATA                   (offsetof(MemEeprom_Data,u8_ImmoData))
#define OFFSET_IMMODATABACKUP             (offsetof(MemEeprom_Data,u8_ImmoBackupData))
#define OFFSET_NXP_KEYK1                  (offsetof(MemEeprom_Data,u8_NxpKeyk1Data))
#define OFFSET_NXP_KEYK1BACKUP           (offsetof(MemEeprom_Data,u8_NxpKeyk1BackupData))
#define OFFSET_USERCERTIFICATE            (offsetof(MemEeprom_Data,u8_UserCertificateData))
#define OFFSET_USERCERTIFICATEBACKUP        (offsetof(MemEeprom_Data,u8_UserCertificateBackupData))
#define OFFSET_PUBLICKEY_X                (offsetof(MemEeprom_Data,u8_PublicKeyXData))
#define OFFSET_PUBLICKEY_X_BACKUP         (offsetof(MemEeprom_Data,u8_PublicKeyXBackuData))
#define OFFSET_PUBLICKEY_Y               (offsetof(MemEeprom_Data,u8_PublicKeyYData))
#define OFFSET_PUBLICKEY_Y_BACKUP        (offsetof(MemEeprom_Data,u8_PublicKeyYBackupData))
#define OFFSET_SECRETKEY               (offsetof(MemEeprom_Data,u8_SecretKeyData))
#define OFFSET_SECRETKEYBACKUP            (offsetof(MemEeprom_Data,u8_SecretKeyBackupData))
#define OFFSET_RVSREQUESTID             (offsetof(MemEeprom_Data,u8_RVSRequestIDData))

/*****************************************************************************
** typedef
*****************************************************************************/
static MemEeprom_Data e2_Data;

/*****************************************************************************
** global variable
*****************************************************************************/
const MemEeprom_Map c_st_e2DataMap[] = 
{
    {EEPID_DIAGDID_0xF186,  EEPID_DIAGDID_0xF186_LEN,   OFFSET_DIAGDID_0xF186,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF187,  EEPID_DIAGDID_0xF187_LEN,   OFFSET_DIAGDID_0xF187,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF188,  EEPID_DIAGDID_0xF188_LEN,   OFFSET_DIAGDID_0xF188,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF189,  EEPID_DIAGDID_0xF189_LEN,   OFFSET_DIAGDID_0xF189,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF18A,  EEPID_DIAGDID_0xF18A_LEN,   OFFSET_DIAGDID_0xF18A,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF18B,  EEPID_DIAGDID_0xF18B_LEN,   OFFSET_DIAGDID_0xF18B,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF18C,  EEPID_DIAGDID_0xF18C_LEN,   OFFSET_DIAGDID_0xF18C,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF190,  EEPID_DIAGDID_0xF190_LEN,   OFFSET_DIAGDID_0xF190,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF192,  EEPID_DIAGDID_0xF192_LEN,   OFFSET_DIAGDID_0xF192,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF193,  EEPID_DIAGDID_0xF193_LEN,   OFFSET_DIAGDID_0xF193,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF194,  EEPID_DIAGDID_0xF194_LEN,   OFFSET_DIAGDID_0xF194,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF195,  EEPID_DIAGDID_0xF195_LEN,   OFFSET_DIAGDID_0xF195,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF197,  EEPID_DIAGDID_0xF197_LEN,   OFFSET_DIAGDID_0xF197,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF198,  EEPID_DIAGDID_0xF198_LEN,   OFFSET_DIAGDID_0xF198,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF199,  EEPID_DIAGDID_0xF199_LEN,   OFFSET_DIAGDID_0xF199,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF19D,  EEPID_DIAGDID_0xF19D_LEN,   OFFSET_DIAGDID_0xF19D,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF1A8,  EEPID_DIAGDID_0xF1A8_LEN,   OFFSET_DIAGDID_0xF1A8,   (UINT32)Fill00},
    {EEPID_FLHVLD,          EEPID_FLHVLD_LEN,           OFFSET_FLHVLD        ,   (UINT32)Fill00},

    {EEPID_NVID_0xE001,  EEPID_NVID_0xE001_LEN,   OFFSET_NVID_0xE001,   (UINT32)Fill00},
    {EEPID_NVID_0xE002,  EEPID_NVID_0xE002_LEN,   OFFSET_NVID_0xE002,   (UINT32)Fill00},
    {EEPID_NVID_0xE003,  EEPID_NVID_0xE003_LEN,   OFFSET_NVID_0xE003,   (UINT32)Fill00},
    {EEPID_NVID_0xE004,  EEPID_NVID_0xE004_LEN,   OFFSET_NVID_0xE004,   (UINT32)Fill00},
    {EEPID_NVID_0xE005,  EEPID_NVID_0xE005_LEN,   OFFSET_NVID_0xE005,   (UINT32)Fill00},
    {EEPID_NVID_0xE006,  EEPID_NVID_0xE006_LEN,   OFFSET_NVID_0xE006,   (UINT32)Fill00},
    {EEPID_NVID_0xE007,  EEPID_NVID_0xE007_LEN,   OFFSET_NVID_0xE007,   (UINT32)Fill00},
    {EEPID_NVID_0xE008,  EEPID_NVID_0xE008_LEN,   OFFSET_NVID_0xE008,   (UINT32)Fill00},
    {EEPID_NVID_0xE009,  EEPID_NVID_0xE009_LEN,   OFFSET_NVID_0xE009,   (UINT32)Fill00},
    {EEPID_NVID_0xE00A,  EEPID_NVID_0xE00A_LEN,   OFFSET_NVID_0xE00A,   (UINT32)Fill00},
    {EEPID_NVID_0xE00B,  EEPID_NVID_0xE00B_LEN,   OFFSET_NVID_0xE00B,   (UINT32)Fill00},

    {EEPID_CFGDID_0x1001,  EEPID_CFGDID_0x1001_LEN,   OFFSET_CFGDID_0x1001,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1002,  EEPID_CFGDID_0x1002_LEN,   OFFSET_CFGDID_0x1002,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1003,  EEPID_CFGDID_0x1003_LEN,   OFFSET_CFGDID_0x1003,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1004,  EEPID_CFGDID_0x1004_LEN,   OFFSET_CFGDID_0x1004,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1005,  EEPID_CFGDID_0x1005_LEN,   OFFSET_CFGDID_0x1005,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1006,  EEPID_CFGDID_0x1006_LEN,   OFFSET_CFGDID_0x1006,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1007,  EEPID_CFGDID_0x1007_LEN,   OFFSET_CFGDID_0x1007,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1008,  EEPID_CFGDID_0x1008_LEN,   OFFSET_CFGDID_0x1008,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1009,  EEPID_CFGDID_0x1009_LEN,   OFFSET_CFGDID_0x1009,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1010,  EEPID_CFGDID_0x1010_LEN,   OFFSET_CFGDID_0x1010,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1012,  EEPID_CFGDID_0x1012_LEN,   OFFSET_CFGDID_0x1012,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1013,  EEPID_CFGDID_0x1013_LEN,   OFFSET_CFGDID_0x1013,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1014,  EEPID_CFGDID_0x1014_LEN,   OFFSET_CFGDID_0x1014,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1015,  EEPID_CFGDID_0x1015_LEN,   OFFSET_CFGDID_0x1015,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1016,  EEPID_CFGDID_0x1016_LEN,   OFFSET_CFGDID_0x1016,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1018,  EEPID_CFGDID_0x1018_LEN,   OFFSET_CFGDID_0x1018,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1019,  EEPID_CFGDID_0x1019_LEN,   OFFSET_CFGDID_0x1019,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1023,  EEPID_CFGDID_0x1023_LEN,   OFFSET_CFGDID_0x1023,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1024,  EEPID_CFGDID_0x1024_LEN,   OFFSET_CFGDID_0x1024,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1025,  EEPID_CFGDID_0x1025_LEN,   OFFSET_CFGDID_0x1025,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1026,  EEPID_CFGDID_0x1026_LEN,   OFFSET_CFGDID_0x1026,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1027,  EEPID_CFGDID_0x1027_LEN,   OFFSET_CFGDID_0x1027,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1028,  EEPID_CFGDID_0x1028_LEN,   OFFSET_CFGDID_0x1028,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1029,  EEPID_CFGDID_0x1029_LEN,   OFFSET_CFGDID_0x1029,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1030,  EEPID_CFGDID_0x1030_LEN,   OFFSET_CFGDID_0x1030,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1031,  EEPID_CFGDID_0x1031_LEN,   OFFSET_CFGDID_0x1031,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1032,  EEPID_CFGDID_0x1032_LEN,   OFFSET_CFGDID_0x1032,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1034,  EEPID_CFGDID_0x1034_LEN,   OFFSET_CFGDID_0x1034,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1035,  EEPID_CFGDID_0x1035_LEN,   OFFSET_CFGDID_0x1035,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1036,  EEPID_CFGDID_0x1036_LEN,   OFFSET_CFGDID_0x1036,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1040,  EEPID_CFGDID_0x1040_LEN,   OFFSET_CFGDID_0x1040,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1041,  EEPID_CFGDID_0x1041_LEN,   OFFSET_CFGDID_0x1041,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1042,  EEPID_CFGDID_0x1042_LEN,   OFFSET_CFGDID_0x1042,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1043,  EEPID_CFGDID_0x1043_LEN,   OFFSET_CFGDID_0x1043,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1049,  EEPID_CFGDID_0x1049_LEN,   OFFSET_CFGDID_0x1049,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1050,  EEPID_CFGDID_0x1050_LEN,   OFFSET_CFGDID_0x1050,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1051,  EEPID_CFGDID_0x1051_LEN,   OFFSET_CFGDID_0x1051,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1052,  EEPID_CFGDID_0x1052_LEN,   OFFSET_CFGDID_0x1052,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1054,  EEPID_CFGDID_0x1054_LEN,   OFFSET_CFGDID_0x1054,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1056,  EEPID_CFGDID_0x1056_LEN,   OFFSET_CFGDID_0x1056,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1057,  EEPID_CFGDID_0x1057_LEN,   OFFSET_CFGDID_0x1057,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1058,  EEPID_CFGDID_0x1058_LEN,   OFFSET_CFGDID_0x1058,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1059,  EEPID_CFGDID_0x1059_LEN,   OFFSET_CFGDID_0x1059,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1061,  EEPID_CFGDID_0x1061_LEN,   OFFSET_CFGDID_0x1061,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1062,  EEPID_CFGDID_0x1062_LEN,   OFFSET_CFGDID_0x1062,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1065,  EEPID_CFGDID_0x1065_LEN,   OFFSET_CFGDID_0x1065,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1066,  EEPID_CFGDID_0x1066_LEN,   OFFSET_CFGDID_0x1066,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1067,  EEPID_CFGDID_0x1067_LEN,   OFFSET_CFGDID_0x1067,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1069,  EEPID_CFGDID_0x1069_LEN,   OFFSET_CFGDID_0x1069,   (UINT32)Fill00},
    {EEPID_CFGDID_0x106B,  EEPID_CFGDID_0x106B_LEN,   OFFSET_CFGDID_0x106B,   (UINT32)Fill00},
    {EEPID_CFGDID_0x106C,  EEPID_CFGDID_0x106C_LEN,   OFFSET_CFGDID_0x106C,   (UINT32)Fill00},
    {EEPID_CFGDID_0x106E,  EEPID_CFGDID_0x106E_LEN,   OFFSET_CFGDID_0x106E,   (UINT32)Fill00},
    {EEPID_CFGDID_0x106F,  EEPID_CFGDID_0x106F_LEN,   OFFSET_CFGDID_0x106F,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1070,  EEPID_CFGDID_0x1070_LEN,   OFFSET_CFGDID_0x1070,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1072,  EEPID_CFGDID_0x1072_LEN,   OFFSET_CFGDID_0x1072,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1074,  EEPID_CFGDID_0x1074_LEN,   OFFSET_CFGDID_0x1074,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1075,  EEPID_CFGDID_0x1075_LEN,   OFFSET_CFGDID_0x1075,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1076,  EEPID_CFGDID_0x1076_LEN,   OFFSET_CFGDID_0x1076,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1077,  EEPID_CFGDID_0x1077_LEN,   OFFSET_CFGDID_0x1077,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1078,  EEPID_CFGDID_0x1078_LEN,   OFFSET_CFGDID_0x1078,   (UINT32)Fill00},
    {EEPID_CFGDID_0x1079,  EEPID_CFGDID_0x1079_LEN,   OFFSET_CFGDID_0x1079,   (UINT32)Fill00},
    
    {EEPID_CFGDID_0x6000,  EEPID_CFGDID_0x6000_LEN,   OFFSET_CFGDID_0x6000,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6001,  EEPID_CFGDID_0x6001_LEN,   OFFSET_CFGDID_0x6001,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6002,  EEPID_CFGDID_0x6002_LEN,   OFFSET_CFGDID_0x6002,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6003,  EEPID_CFGDID_0x6003_LEN,   OFFSET_CFGDID_0x6003,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6004,  EEPID_CFGDID_0x6004_LEN,   OFFSET_CFGDID_0x6004,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6005,  EEPID_CFGDID_0x6005_LEN,   OFFSET_CFGDID_0x6005,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6006,  EEPID_CFGDID_0x6006_LEN,   OFFSET_CFGDID_0x6006,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6007,  EEPID_CFGDID_0x6007_LEN,   OFFSET_CFGDID_0x6007,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6008,  EEPID_CFGDID_0x6008_LEN,   OFFSET_CFGDID_0x6008,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6009,  EEPID_CFGDID_0x6009_LEN,   OFFSET_CFGDID_0x6009,   (UINT32)Fill00},
    {EEPID_CFGDID_0x600C,  EEPID_CFGDID_0x600C_LEN,   OFFSET_CFGDID_0x600C,   (UINT32)Fill00},
    {EEPID_CFGDID_0x600D,  EEPID_CFGDID_0x600D_LEN,   OFFSET_CFGDID_0x600D,   (UINT32)Fill00},
    {EEPID_CFGDID_0x600E,  EEPID_CFGDID_0x600E_LEN,   OFFSET_CFGDID_0x600E,   (UINT32)Fill00},
    {EEPID_CFGDID_0x600F,  EEPID_CFGDID_0x600F_LEN,   OFFSET_CFGDID_0x600F,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6010,  EEPID_CFGDID_0x6010_LEN,   OFFSET_CFGDID_0x6010,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6011,  EEPID_CFGDID_0x6011_LEN,   OFFSET_CFGDID_0x6011,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6013,  EEPID_CFGDID_0x6013_LEN,   OFFSET_CFGDID_0x6013,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6014,  EEPID_CFGDID_0x6014_LEN,   OFFSET_CFGDID_0x6014,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6015,  EEPID_CFGDID_0x6015_LEN,   OFFSET_CFGDID_0x6015,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6016,  EEPID_CFGDID_0x6016_LEN,   OFFSET_CFGDID_0x6016,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6017,  EEPID_CFGDID_0x6017_LEN,   OFFSET_CFGDID_0x6017,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6018,  EEPID_CFGDID_0x6018_LEN,   OFFSET_CFGDID_0x6018,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6019,  EEPID_CFGDID_0x6019_LEN,   OFFSET_CFGDID_0x6019,   (UINT32)Fill00},
    {EEPID_CFGDID_0x601A,  EEPID_CFGDID_0x601A_LEN,   OFFSET_CFGDID_0x601A,   (UINT32)Fill00},
    {EEPID_CFGDID_0x601B,  EEPID_CFGDID_0x601B_LEN,   OFFSET_CFGDID_0x601B,   (UINT32)Fill00},
    {EEPID_CFGDID_0x601C,  EEPID_CFGDID_0x601C_LEN,   OFFSET_CFGDID_0x601C,   (UINT32)Fill00},
    {EEPID_CFGDID_0x601D,  EEPID_CFGDID_0x601D_LEN,   OFFSET_CFGDID_0x601D,   (UINT32)Fill00},
    {EEPID_CFGDID_0x601E,  EEPID_CFGDID_0x601E_LEN,   OFFSET_CFGDID_0x601E,   (UINT32)Fill00},
    {EEPID_CFGDID_0x601F,  EEPID_CFGDID_0x601F_LEN,   OFFSET_CFGDID_0x601F,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6020,  EEPID_CFGDID_0x6020_LEN,   OFFSET_CFGDID_0x6020,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6021,  EEPID_CFGDID_0x6021_LEN,   OFFSET_CFGDID_0x6021,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6022,  EEPID_CFGDID_0x6022_LEN,   OFFSET_CFGDID_0x6022,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6024,  EEPID_CFGDID_0x6024_LEN,   OFFSET_CFGDID_0x6024,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6025,  EEPID_CFGDID_0x6025_LEN,   OFFSET_CFGDID_0x6025,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6026,  EEPID_CFGDID_0x6026_LEN,   OFFSET_CFGDID_0x6026,   (UINT32)Fill00},
    {EEPID_CFGDID_0x602A,  EEPID_CFGDID_0x602A_LEN,   OFFSET_CFGDID_0x602A,   (UINT32)Fill00},
    {EEPID_CFGDID_0x602B,  EEPID_CFGDID_0x602B_LEN,   OFFSET_CFGDID_0x602B,   (UINT32)Fill00},
    {EEPID_CFGDID_0x602D,  EEPID_CFGDID_0x602D_LEN,   OFFSET_CFGDID_0x602D,   (UINT32)Fill00},
    {EEPID_CFGDID_0x602E,  EEPID_CFGDID_0x602E_LEN,   OFFSET_CFGDID_0x602E,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6030,  EEPID_CFGDID_0x6030_LEN,   OFFSET_CFGDID_0x6030,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6031,  EEPID_CFGDID_0x6031_LEN,   OFFSET_CFGDID_0x6031,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6032,  EEPID_CFGDID_0x6032_LEN,   OFFSET_CFGDID_0x6032,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6033,  EEPID_CFGDID_0x6033_LEN,   OFFSET_CFGDID_0x6033,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6034,  EEPID_CFGDID_0x6034_LEN,   OFFSET_CFGDID_0x6034,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6035,  EEPID_CFGDID_0x6035_LEN,   OFFSET_CFGDID_0x6035,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6038,  EEPID_CFGDID_0x6038_LEN,   OFFSET_CFGDID_0x6038,   (UINT32)Fill00},
    {EEPID_CFGDID_0x603A,  EEPID_CFGDID_0x603A_LEN,   OFFSET_CFGDID_0x603A,   (UINT32)Fill00},
    {EEPID_CFGDID_0x603B,  EEPID_CFGDID_0x603B_LEN,   OFFSET_CFGDID_0x603B,   (UINT32)Fill00},
    {EEPID_CFGDID_0x603C,  EEPID_CFGDID_0x603C_LEN,   OFFSET_CFGDID_0x603C,   (UINT32)Fill00},
    {EEPID_CFGDID_0x603D,  EEPID_CFGDID_0x603D_LEN,   OFFSET_CFGDID_0x603D,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6041,  EEPID_CFGDID_0x6041_LEN,   OFFSET_CFGDID_0x6041,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6043,  EEPID_CFGDID_0x6043_LEN,   OFFSET_CFGDID_0x6043,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6046,  EEPID_CFGDID_0x6046_LEN,   OFFSET_CFGDID_0x6046,   (UINT32)Fill00},
    {EEPID_CFGDID_0x604F,  EEPID_CFGDID_0x604F_LEN,   OFFSET_CFGDID_0x604F,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6051,  EEPID_CFGDID_0x6051_LEN,   OFFSET_CFGDID_0x6051,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6052,  EEPID_CFGDID_0x6052_LEN,   OFFSET_CFGDID_0x6052,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6054,  EEPID_CFGDID_0x6054_LEN,   OFFSET_CFGDID_0x6054,   (UINT32)Fill00},
    {EEPID_CFGDID_0x6055,  EEPID_CFGDID_0x6055_LEN,   OFFSET_CFGDID_0x6055,   (UINT32)Fill00},

    {EEPID_CFGDID_0x2008,  EEPID_CFGDID_0x2008_LEN,   OFFSET_CFGDID_0x2008,   (UINT32)Fill00},
    {EEPID_CFGDID_0x200A,  EEPID_CFGDID_0x200A_LEN,   OFFSET_CFGDID_0x200A,   (UINT32)Fill00},
    {EEPID_CFGDID_0x200C,  EEPID_CFGDID_0x200C_LEN,   OFFSET_CFGDID_0x200C,   (UINT32)Fill00},
    {EEPID_CFGDID_0x200D,  EEPID_CFGDID_0x200D_LEN,   OFFSET_CFGDID_0x200D,   (UINT32)Fill00},
    {EEPID_CFGDID_0x2010,  EEPID_CFGDID_0x2010_LEN,   OFFSET_CFGDID_0x2010,   (UINT32)Fill00},
    {EEPID_CFGDID_0x2011,  EEPID_CFGDID_0x2011_LEN,   OFFSET_CFGDID_0x2011,   (UINT32)Fill00},
    {EEPID_CFGDID_0x2012,  EEPID_CFGDID_0x2012_LEN,   OFFSET_CFGDID_0x2012,   (UINT32)Fill00},
    {EEPID_CFGDID_0x2013,  EEPID_CFGDID_0x2013_LEN,   OFFSET_CFGDID_0x2013,   (UINT32)Fill00},
    {EEPID_CFGDID_0x201A,  EEPID_CFGDID_0x201A_LEN,   OFFSET_CFGDID_0x201A,   (UINT32)Fill00},
    {EEPID_CFGDID_0x201D,  EEPID_CFGDID_0x201D_LEN,   OFFSET_CFGDID_0x201D,   (UINT32)Fill00},

    {EEPID_NVID_0xE00C,  EEPID_NVID_0xE00C_LEN,   OFFSET_NVID_0xE00C,   (UINT32)Fill00},
    {EEPID_NVID_0xE00D,  EEPID_NVID_0xE00D_LEN,   OFFSET_NVID_0xE00D,   (UINT32)Fill00},
    {EEPID_NVID_0xE00E,  EEPID_NVID_0xE00E_LEN,   OFFSET_NVID_0xE00E,   (UINT32)Fill00},
    {EEPID_NVID_0xE00F,  EEPID_NVID_0xE00F_LEN,   OFFSET_NVID_0xE00F,   (UINT32)Fill00},
    {EEPID_NVID_0xE010,  EEPID_NVID_0xE010_LEN,   OFFSET_NVID_0xE010,   (UINT32)Fill00},
    {EEPID_NVID_0xE011,  EEPID_NVID_0xE011_LEN,   OFFSET_NVID_0xE011,   (UINT32)Fill00},
    {EEPID_NVID_0xE012,  EEPID_NVID_0xE012_LEN,   OFFSET_NVID_0xE012,   (UINT32)Fill00},

    {EEPID_DIAGDID_0xB000,  EEPID_DIAGDID_0xB000_LEN,   OFFSET_DIAGDID_0xB000,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB010,  EEPID_DIAGDID_0xB010_LEN,   OFFSET_DIAGDID_0xB010,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB011,  EEPID_DIAGDID_0xB011_LEN,   OFFSET_DIAGDID_0xB011,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB02A,  EEPID_DIAGDID_0xB02A_LEN,   OFFSET_DIAGDID_0xB02A,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB02B,  EEPID_DIAGDID_0xB02B_LEN,   OFFSET_DIAGDID_0xB02B,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB300,  EEPID_DIAGDID_0xB300_LEN,   OFFSET_DIAGDID_0xB300,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB301,  EEPID_DIAGDID_0xB301_LEN,   OFFSET_DIAGDID_0xB301,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB302,  EEPID_DIAGDID_0xB302_LEN,   OFFSET_DIAGDID_0xB302,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB303,  EEPID_DIAGDID_0xB303_LEN,   OFFSET_DIAGDID_0xB303,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xB0B2,  EEPID_DIAGDID_0xB0B2_LEN,   OFFSET_DIAGDID_0xB0B2,   (UINT32)Fill00},

    {EEPID_DIAGDID_0xF201,  EEPID_DIAGDID_0xF201_LEN,   OFFSET_DIAGDID_0xF201,   (UINT32)Fill00},
    {EEPID_DIAGDID_0xF202,  EEPID_DIAGDID_0xF202_LEN,   OFFSET_DIAGDID_0xF202,   (UINT32)Fill00},
    
    {EEPID_NVID_0xE013,  EEPID_NVID_0xE013_LEN,   OFFSET_NVID_0xE013,   (UINT32)Fill00},
    {EEPID_NVID_0xE014,  EEPID_NVID_0xE014_LEN,   OFFSET_NVID_0xE014,   (UINT32)Fill00},
    {EEPID_NVID_0xE015,  EEPID_NVID_0xE015_LEN,   OFFSET_NVID_0xE015,   (UINT32)Fill00},
    {EEPID_NVID_0xE016,  EEPID_NVID_0xE016_LEN,   OFFSET_NVID_0xE016,   (UINT32)Fill00},
    {EEPID_NVID_0xE017,  EEPID_NVID_0xE017_LEN,   OFFSET_NVID_0xE017,   (UINT32)Fill00},
    {EEPID_NVID_0xE018,  EEPID_NVID_0xE018_LEN,   OFFSET_NVID_0xE018,   (UINT32)Fill00},
    {EEPID_NVID_0xE019,  EEPID_NVID_0xE019_LEN,   OFFSET_NVID_0xE019,   (UINT32)Fill00},
    {EEPID_NVID_0xE01A,  EEPID_NVID_0xE01A_LEN,   OFFSET_NVID_0xE01A,   (UINT32)Fill00},
    {EEPID_NVID_0xE01B,  EEPID_NVID_0xE01B_LEN,   OFFSET_NVID_0xE01B,   (UINT32)Fill00},
    {EEPID_NVID_0xE01C,  EEPID_NVID_0xE01C_LEN,   OFFSET_NVID_0xE01C,   (UINT32)Fill00},
    {EEPID_NVID_0xE01D,  EEPID_NVID_0xE01D_LEN,   OFFSET_NVID_0xE01D,   (UINT32)Fill00},
    {EEPID_NVID_0xE01E,  EEPID_NVID_0xE01E_LEN,   OFFSET_NVID_0xE01E,   (UINT32)Fill00},
    {EEPID_NVID_0xE01F,  EEPID_NVID_0xE01F_LEN,   OFFSET_NVID_0xE01F,   (UINT32)Fill00},
    {EEPID_NVID_0xE020,  EEPID_NVID_0xE020_LEN,   OFFSET_NVID_0xE020,   (UINT32)Fill00},
    {EEPID_NVID_0xE021,  EEPID_NVID_0xE021_LEN,   OFFSET_NVID_0xE021,   (UINT32)Fill00},
    {EEPID_NVID_0xE022,  EEPID_NVID_0xE022_LEN,   OFFSET_NVID_0xE022,   (UINT32)Fill00},
    {EEPID_NVID_0xE023,  EEPID_NVID_0xE023_LEN,   OFFSET_NVID_0xE023,   (UINT32)Fill00},
    {EEPID_NVID_0xE024,  EEPID_NVID_0xE024_LEN,   OFFSET_NVID_0xE024,   (UINT32)Fill00},
    {EEPID_NVID_0xE025,  EEPID_NVID_0xE025_LEN,   OFFSET_NVID_0xE025,   (UINT32)Fill00},
    {EEPID_NVID_0xE026,  EEPID_NVID_0xE026_LEN,   OFFSET_NVID_0xE026,   (UINT32)Fill00},
    {EEPID_NVID_0xE027,  EEPID_NVID_0xE027_LEN,   OFFSET_NVID_0xE027,   (UINT32)Fill00},
    {EEPID_NVID_0xE028,  EEPID_NVID_0xE028_LEN,   OFFSET_NVID_0xE028,   (UINT32)Fill00},

    {EEPID_CFGDID_0x6012,  EEPID_CFGDID_0x6012_LEN,   OFFSET_CFGDID_0x6012,   (UINT32)Fill00},
    {EEPID_CFGDID_0x602C,  EEPID_CFGDID_0x602C_LEN,   OFFSET_CFGDID_0x602C,   (UINT32)Fill00},

    {EEPID_DTCDATA,                EEPID_DTCDATA_LEN,                OFFSET_DTCDATA,	   			(UINT32)Fill00},
    {EEPID_SECURITY_ACCESSS_FLAG,  EEPID_SECURITY_ACCESSS_FLAG_LEN,  OFFSET_SECURITY_ACCESSS_FLAG,  (UINT32)Fill00},

    {EEPID_EOL_FLAG,        EEPID_EOL_FLAG_LEN,         OFFSET_EOL_FLAG,            (UINT32)Fill00},
    {EEPID_EOL_LOCK_FLAG,   EEPID_EOL_LOCK_FLAG_LEN,    OFFSET_EOL_LOCK_FLAG,       (UINT32)Fill00},
    {EEPID_EOL_VBAT_GAIN,   EEPID_EOL_VBAT_GAIN_LEN,    OFFSET_EOL_VBAT_GAIN,       (UINT32)Fill00},
    {EEPID_EOL_VBAT_OFFSET, EEPID_EOL_VBAT_OFFSET_LEN,  OFFSET_EOL_VBAT_OFFSET,     (UINT32)Fill00},
    {EEPID_EOL_CRCDATA,     EEPID_EOL_CRCDATA_LEN,      OFFSET_EOL_CRCDATA,         (UINT32)Fill00},
    {EEPID_EOL_Trace_Num,   EEPID_EOL_Trace_Num_LEN,    OFFSET_EOL_Trace_Num,       (UINT32)Fill00},

    {EEPID_AUX_SLEEPTIME,           EEPID_AUX_SLEEPTIME_LEN,              OFFSET_AUX_SLEEPTIME,         (UINT32)Fill00},
    {EEPID_AUX_SLEEPTIMEBACKUP,     EEPID_AUX_SLEEPTIMEBACKUP_LEN,        OFFSET_AUX_SLEEPTIMEBACKUP,   (UINT32)Fill00},
    {EEPID_USERID,                  EEPID_USERID_LEN,                     OFFSET_USERID,                (UINT32)Fill00},
    {EEPID_USERIDBACKUP,            EEPID_USERIDBACKUP_LEN,               OFFSET_USERIDBACKUP,          (UINT32)Fill00},
    {EEPID_TTOKEN,                  EEPID_TTOKEN_LEN,                     OFFSET_TTOKEN,                (UINT32)Fill00},
    {EEPID_TTOKENBACKUP,            EEPID_TTOKENBACKUP_LEN,               OFFSET_TTOKENBACKUP,          (UINT32)Fill00},
    {EEPID_AUX_ALARMTIMEUTC,        EEPID_AUX_ALARMTIMEUTC_LEN,           OFFSET_AUX_ALARMTIMEUTC,      (UINT32)Fill00},
    {EEPID_AUX_ALARMTIMEUTCBACKUP,  EEPID_AUX_ALARMTIMEUTCBACKUP_LEN, OFFSET_AUX_ALARMTIMEUTCBACKUP,    (UINT32)Fill00},
    {EEPID_IMMODATA,                EEPID_IMMODATA_LEN,               OFFSET_IMMODATA,                  (UINT32)Fill00},
    {EEPID_IMMODATABACKUP,          EEPID_IMMODATABACKUP_LEN,         OFFSET_IMMODATABACKUP,            (UINT32)Fill00},
    {EEPID_NXP_KEYK1,               EEPID_NXP_KEYK1_LEN,              OFFSET_NXP_KEYK1,                 (UINT32)Fill00},
    {EEPID_NXP_KEYK1BACKUP,         EEPID_NXP_KEYK1BACKUP_LEN,        OFFSET_NXP_KEYK1BACKUP,           (UINT32)Fill00},
    {EEPID_USERCERTIFICATE,         EEPID_USERCERTIFICATE_LEN,        OFFSET_USERCERTIFICATE,           (UINT32)Fill00},
    {EEPID_USERCERTIFICATEBACKUP,   EEPID_USERCERTIFICATEBACKUP_LEN,  OFFSET_USERCERTIFICATEBACKUP,     (UINT32)Fill00},
    {EEPID_PUBLICKEY_X,             EEPID_PUBLICKEY_X_LEN,            OFFSET_PUBLICKEY_X,               (UINT32)Fill00},
    {EEPID_PUBLICKEY_X_BACKUP,      EEPID_PUBLICKEY_X_BACKUP_LEN,     OFFSET_PUBLICKEY_X_BACKUP,        (UINT32)Fill00},
    {EEPID_PUBLICKEY_Y,             EEPID_PUBLICKEY_Y_LEN,            OFFSET_PUBLICKEY_Y,               (UINT32)Fill00},
    {EEPID_PUBLICKEY_Y_BACKUP,      EEPID_PUBLICKEY_Y_BACKUP_LEN,     OFFSET_PUBLICKEY_Y_BACKUP,        (UINT32)Fill00},
    {EEPID_SECRETKEY,               EEPID_SECRETKEY_LEN,              OFFSET_SECRETKEY,                 (UINT32)Fill00},
    {EEPID_SECRETKEYBACKUP,         EEPID_SECRETKEYBACKUP_LEN,        OFFSET_SECRETKEYBACKUP,           (UINT32)Fill00},
    {EEPID_RVSREQUESTID,            EEPID_RVSREQUESTID_LEN,           OFFSET_RVSREQUESTID,              (UINT32)Fill00},
};


const UINT16 u16_e2_data_num = sizeof(c_st_e2DataMap) / sizeof(c_st_e2DataMap[0]);

const UINT8* const p_e2_ram = ( UINT8* )&e2_Data;
const MemEeprom_Data *const p_e2_Data = &e2_Data;

const UINT32 const u32_e2_byte_num = sizeof(e2_Data)/sizeof(UINT8);
/*****************************************************************************
** static variables
*****************************************************************************/


/*****************************************************************************
** static constants
*****************************************************************************/


/*****************************************************************************
** static function prototypes
*****************************************************************************/


/*****************************************************************************
** function prototypes
*****************************************************************************/


/****************************************************************************/


/*****************************************************************************
** End File
*****************************************************************************/


/******************************************************************************
 * (C) Copyright 2020 YuChiZhiNeng-Automotive
 * FILE NAME:    
 * DESCRIPTION:
 * 
 * DATE BEGUN:   
 * BY:           
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

/*****************************************************************************
** #include 
*****************************************************************************/
//FreeRTOS相关
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "type.h"

/*****************************************************************************
** #define
*****************************************************************************/
#define RUNTASK                for(;;)

#define CAN_TASK_STK_SIZE                     (2048/4)
#define CAN_TASK_ALARM_TIME                   1U  
#define CAN_TASK_PRD_MS                       5U      

#define DCPD_TASK_STK_SIZE                    (2048/4) 
#define DCPD_TASK_ALARM_TIME                  3U
#define DCPD_TASK_PRD_MS                      10U

#define DTC_TASK_STK_SIZE                     (2048/4)
#define DTC_TASK_ALARM_TIME                   4U
#define DTC_TASK_PRD_MS                       50U

#define BLE_TASK_STK_SIZE                     (2048/4)
#define BLE_TASK_ALARM_TIME                   5U
#define BLE_TASK_PRD_MS                       50U

#define APP_TASK_STK_SIZE                     (2048/4)
#define APP_TASK_ALARM_TIME                   7U
#define APP_TASK_PRD_MS                       10U

#define SYS_MGR_TASK_STK_SIZE                 (1024/4)
#define SYS_MGR_ALARM_TIME                    8U
#define SYS_MGR_PRD_MS                        100U

#define AUX_TASK_STK_SIZE                     (1536/4)
#define AUX_TASK_ALARM_TIME                   9U
#define AUX_TASK_PRD_MS                       200U

#define NVRAM_TASK_STK_SIZE                   (1536/4)
#define NVRAM_TASK_ALARM_TIME                 10U
#define NVRAM_TASK_PRD_MS                     100U


/*****************************************************************************
** typedef
*****************************************************************************/
#define FM_NORMAL  0
#define FM_QUIET   1
#define FM_SILENT  2

/*****************************************************************************
** global variable
*****************************************************************************/
extern UINT8 u8gCanDataTx[8];
extern UINT8 u8gCanDataRx[8];
extern UINT8 u8gCanBusoff;
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
void TaskInit(void);

void CAN_task(void);
void DCPD_task(void);
void DTC_task(void);
void BLE_task(void);
void APP_task(void);
void SYS_task(void);
void AUX_task(void);
void NVRAM_task(void);
BOOL ApiComSetFrameMode(UINT8 u8Fmd);
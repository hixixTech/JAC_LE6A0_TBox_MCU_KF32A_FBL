/**
  ********************************************************************
  * 文件名  ymodem.h
  * 作  者   ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2018-3-15
  * 描  述  该文件提供了ymodem.c需要的宏定义以及函数声明。
  *
  *********************************************************************
*/
#ifndef _YMODEM_H_
#define _YMODEM_H_

/* Includes ------------------------------------------------------------------*/
#include "system_init.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (256)

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define MAX_ERRORS              (5)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t Ymodem_Receive (uint8_t *);

#endif  /* _YMODEM_H_ */

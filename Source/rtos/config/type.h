/******************************************************************************
 * (C) Copyright 2020 Anhui Domain Compute Co.Ltd
 * FILE NAME:    type.h
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

#ifndef __TYPE_H__
#define __TYPE_H__

#define SWAP2(s) ((((s) >> 8) & 0xFFu) | (((s) << 8) & 0xFF00u))

#define SWAP4(x) ((((x) >> 24) & 0xFFu) | ((((x) & 0xFF0000UL) >> 8) & 0xFF00u) \
                      | (((x) & 0xFF00UL) << 8) | ((x) << 24))

#define GET_UINT16(addr)        ((UINT16)((UINT16)(*(const UINT8*)(addr))<<8)|(UINT16)(*((const UINT8*)(addr)+1u)))
#define GET_UINT32(addr)        ((((UINT32)(*((const UINT8*)(addr)+0u)))<<24)|\
                                 (((UINT32)(*((const UINT8*)(addr)+1u)))<<16)|\
                                 (((UINT32)(*((const UINT8*)(addr)+2u)))<< 8)|\
                                 (((UINT32)(*((const UINT8*)(addr)+3u)))    ) )

#define MSB(value)       ((UINT8)((value)>>8))
#define LSB(value)       ((UINT8)(value))

#define LONIBBLE(value)  ((UINT8)((UINT8)(value)&(UINT8)0x0f))
#define HINIBBLE(value)  ((UINT8)(((UINT8)(value)<<4)&(UINT8)0xf0))

/*****************************************************************************
**#include 
*****************************************************************************/



/*****************************************************************************
**  typedef
*****************************************************************************/


/*****************************************************************************
** Constant Macro Definition
*****************************************************************************/
typedef   	char 					CHAR;
typedef   	unsigned char 		    UCHAR;
typedef     unsigned char 	        BOOL;
typedef     unsigned char 	        bool;
typedef     signed char             INT8;  
typedef     unsigned char           UINT8;  
typedef     short                   INT16;  
typedef     unsigned short          UINT16;  
typedef     long                    INT32;  
typedef     unsigned long           UINT32;  
typedef     long long               INT64;  
typedef     unsigned long long      UINT64;  
typedef     float                   FLOAT;  
typedef     double                  DOUBLE;  
typedef     void                    VOID;

typedef signed char                 int8_t;
typedef unsigned char               uint8_t;
typedef short                       int16_t;
typedef unsigned short              uint16_t;
typedef int                         int32_t;
typedef unsigned int                uint32_t;
typedef long long                   int64_t;
typedef unsigned long long          uint64_t;

#ifndef OK
#define OK           0
#endif

#ifndef ERROR
#define ERROR       -1
#endif

#ifndef NG
#define NG          1
#endif

#ifndef TRUE
#define TRUE        1
#endif

#ifndef true
#define true        1
#endif

#ifndef FALSE
#define FALSE       0
#endif

#ifndef false
#define false       0
#endif

#ifndef NULL
#define NULL        (void*)0
#endif

#ifndef ON
#define ON          1
#endif

#ifndef OFF
#define OFF         0
#endif

#ifndef ENABLE
#define ENABLE          1
#endif

#ifndef DISABLE
#define DISABLE         0
#endif

#ifndef LOW
#define LOW          0
#endif

#ifndef HIGH
#define HIGH         1
#endif
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

#endif	//__TYPE_H__

/*****************************************************************************
** End File
*****************************************************************************/


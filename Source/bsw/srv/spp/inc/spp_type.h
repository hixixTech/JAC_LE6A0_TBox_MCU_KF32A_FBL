#ifndef _DIAS_SPP_TYPE_H_
#define _DIAS_SPP_TYPE_H_


#include "type.h"
#include "os_log.h"

typedef signed char              INT8;
typedef unsigned char             UINT8;
typedef unsigned char             UCHAR;
typedef unsigned char             BYTE;
typedef unsigned char             BOOL;
typedef signed short             INT16;
typedef unsigned short            UINT16;
typedef signed long             INT32;
typedef unsigned long            UINT32;
typedef unsigned long            DWORD;
typedef signed long long             INT64;
typedef unsigned long long            UINT64;
typedef void                VOID;
typedef float               FLOAT;
typedef double              DOUBLE;

//#define DIAS_SPP_TYPE_MPU
#define DIAS_SPP_HEART_ADD

#define	LOG_EMERG          0 /* system is unusable               */
#define	LOG_ALERT          1 /* action must be taken immediately */
#define	LOG_CRIT           2 /* critical conditions              */
#define	LOG_ERR            3 /* error conditions                 */
#define	LOG_WARNING        4 /* warning conditions               */
#define	LOG_NOTICE         5 /* normal but significant condition */
#define	LOG_INFO           6 /* informational                    */
#define	LOG_DEBUG          7 /* debug-level messages             */


#ifdef	DIAS_SPP_TYPE_MPU
    #define DiasDcpdLog(priority,...) DiasLog(priority, DIAS_DCPD_APP_NAME,__VA_ARGS__)
#else
    
    #define DiasDcpdLog(priority,...) 
#endif

typedef enum
{
    DIAS_FAILURE                     = -1,
    DIAS_SUCCESS                     = 0,

    DIAS_COMMON_ERROR_CODE_MIN       = 0x00000001,
    DIAS_COMMON_ERROR_CODE_MAX       = 0x00001000,
    
    DIAS_DAEMON_ERROR_CODE_MIN       = 0x00001000,
    DIAS_DAEMON_ERROR_CODE_MAX       = 0x00002000,

    DIAS_LOG_ERROR_CODE_MIN          = 0x00002000,
    DIAS_LOG_ERROR_CODE_MAX          = 0x00003000,

    DIAS_NV_ERROR_CODE_MIN           = 0x00003001,
    DIAS_NV_ERROR_CODE_BAD_LENGTH    = 0x00003002,
    DIAS_NV_ERROR_CODE_MAX           = 0x00004000,
}DiasErrorCode_e;


#endif


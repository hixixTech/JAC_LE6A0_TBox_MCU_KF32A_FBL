#ifndef _SPP_TYPE_H_
#define _SPP_TYPE_H_


/*#include "types.h"*//*by S811*/
#include "os_log.h"



//#define SPP_TYPE_MPU
#define SPP_HEART_ADD

#define	LOG_EMERG          0 /* system is unusable               */
#define	LOG_ALERT          1 /* action must be taken immediately */
#define	LOG_CRIT           2 /* critical conditions              */
#define	LOG_ERR            3 /* error conditions                 */
#define	LOG_WARNING        4 /* warning conditions               */
#define	LOG_NOTICE         5 /* normal but significant condition */
#define	LOG_INFO           6 /* informational                    */
#define	LOG_DEBUG          7 /* debug-level messages             */


#ifdef	SPP_TYPE_MPU
    #define DiasDcpdLog(priority,...) DiasLog(priority, DCPD_APP_NAME,__VA_ARGS__)
#else
    
    #define DiasDcpdLog(priority,...) //yynt(_LOG_ENTRY,##__VA_ARGS__);
#endif

typedef enum
{
    SPP_FAILURE                 = -1,
    SPP_SUCCESS                 = 0,

    COMMON_ERROR_CODE_MIN       = 0x00000001,
    COMMON_ERROR_CODE_MAX       = 0x00001000,
    
    DAEMON_ERROR_CODE_MIN       = 0x00001000,
    DAEMON_ERROR_CODE_MAX       = 0x00002000,

    LOG_ERROR_CODE_MIN          = 0x00002000,
    LOG_ERROR_CODE_MAX          = 0x00003000,

    NV_ERROR_CODE_MIN           = 0x00003001,
    NV_ERROR_CODE_BAD_LENGTH    = 0x00003002,
    NV_ERROR_CODE_MAX           = 0x00004000,
}SPPErrorCode_e;


#endif


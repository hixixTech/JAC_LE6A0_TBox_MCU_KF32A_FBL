/*****************************************************************************
|                      	       	Copyright
|-----------------------------------------------------------------------------
|    Copyright (c) 2018 SAIC Motor Co. LTD.  All rights reserved.
|
|    This software is furnished under a license and may be used and copied 
|    only in accordance with the terms of such license and with the inclusion
|    of the above copyright notice. This software or any other copies thereof 
|    may not be provided or otherwise made available to any other person.
|    No title to and ownership of the software is hereby transferred.
|
|    The information in this software is subject to change without notice 
|    and should not be constructed as a commitment by SAIC Motor Co. LTD.
|
|    SAIC Motor Co. LTD. assumes no responsibility for the use or reliability
|    of its Software on equipment which is not supported by SAIC Motor Co. LTD.
|-----------------------------------------------------------------------------
******************************************************************************/
#ifndef SPP_CORE_TYPES_H
#define SPP_CORE_TYPES_H

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// #define SPP_SUCCESS (1u)
// #define SPP_FAILURE (0u)

typedef unsigned char          SPP_UInt8;                 /*!< Unsigned 8 bit data type */
typedef unsigned short         SPP_UInt16;                /*!< Unsigned 16 bit data type */
typedef unsigned int           SPP_UInt32;                /*!< Unsigned 32 bit data type */
typedef unsigned long long     SPP_UInt64;                /*!< unsigned 64 bit data type*/
typedef signed char            SPP_Int8;                  /*!< signed 8 bit data type */
typedef signed short           SPP_Int16;                 /*!< signed 16 bit data type */
typedef signed int             SPP_Int32;                 /*!< signed 32 bit data type */
typedef signed long long       SPP_Int64;                 /*!< signed 64 bit data type */
typedef unsigned char          SPP_BOOL;                  /*!< SPP_BOOL data type */

typedef SPP_BOOL SPP_STATUS;
/**
* @brief SPP_BOOLEAN definition
*/
typedef enum
{
    SPP_FALSE = 0,
    SPP_TRUE  = 1,
}SPP_BOOLEAN;

/**
* @brief ErrorCode definition
*/
typedef enum
{
    SPP_TIMEOUT = 1,
}SPP_ERRORCODE;

/**
* @brief Connection State
// */
// typedef enum
// {
//     SPP_STATE_UNKNOW = 0,
//     SPP_STATE_INIT,
//     SPP_STATE_RUNNING,
//     SPP_STATE_WARNING,
//     SPP_STATE_ERROR,
//     SPP_STATE_SLEEP
// }SppState_ENUM;

// typedef SppState_ENUM SPP_STATE;

/**
* @brief Connection State
*/
typedef enum
{
    CS_UNKNOWN = 0,
    CS_INIT,
    CS_CONNECTING,
    CS_SYS_VERSION_MISMATCH_ERROR,
    CS_USER_VERSION_MISMATCH_ERROR,
    CS_SIGNATURE_VERSION_MISMATCH_ERROR,
    CS_CONNECTED,
    CS_DISCONNECTING,
    CS_DISCONNECTED,
}SPP_CONN_STATE;

/**
* @brief Log Level Enums
*/
typedef enum
{
    LL_CLOSED = 0,  /*!< no log output */
    LL_DEBUG,       /*!< debugging logs */
    LL_INFO,        /*!< information logs */
    LL_WARNING,     /*!< warning logs */
    LL_ERROR,       /*!< error logs */
}SPP_LOGLEVEL;

/**
* @brief Version Structure
*/
typedef struct
{
    SPP_Int16 major;  /*!< the version should not be compatible if major version changes. */
    SPP_Int16 minor;  /*!< the version should be compatible if only minor version changes. */
    SPP_Int16 patch;  /*!< the patch number */
}SppVersion;

/**
* @brief Externsion of Version Structure
*/
typedef struct
{
    SppVersion sys_version;  /*!< the system version, normally means spp core library. */ 
    SppVersion user_version; /*!< the user version, normally means version of sppm file, definited in EAST editor. */
    SPP_Int8 md5[9];             /*!< the user version's md5 value (only the last 8 bytes), it's genereated
                                  by EAST editor, the users are not allowed to modify it*/
}SppVersionEx;

/**
*  @brief the logging callback function
*/
typedef void (*SPP_Logging_Callback)(SPP_LOGLEVEL log_level, const char* msg);

/**
*  @brief the connection state callback function
*/
typedef void (*SPP_ConnectionStateChange_Callback)(SPP_CONN_STATE cs);

/**
*  @brief the user event acknowledge callback function
*/
typedef void (*SPP_UserEventAck_Callback)(SPP_UInt16 event_id);

/**
*  @brief the user event error acknowledge callback function
*/
typedef void (*SPP_UserEventAckError_Callback)(SPP_UInt16 event_id, SPP_ERRORCODE ec);

/**
*  @brief the user event acknowledge callback function in case to capture all of user event
*/
typedef void (*SPP_EventRecv_Callback)(SPP_UInt16 event_id);

typedef SPP_UInt32 (*SPP_SerialPortInit_Callback)(SPP_UInt8* data_buffer, SPP_UInt16 length);
typedef SPP_BOOL (*SPP_SerialPortSend_Callback)(unsigned char* data, int length);
typedef SPP_UInt16 (*SPP_SerialPortRecv_Callback)(void);
typedef SPP_BOOL (*SPP_SerialPortClose_Callback)(void);

#ifdef __cplusplus
}
#endif

#endif /*SPP_CORE_TYPES_H*/

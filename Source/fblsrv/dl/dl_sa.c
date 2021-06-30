/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    dl_sa.c                                                     */
/* Description:  Security Access service implementation                      */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠        基线创建                         */
/* 2020-02-24   V11.01.01.01     陈惠        增加CVTC安全算法                 */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/

#include "dl_engine.h"
#include "dl_service.h"
#include "dl_config_pc.h"

#include "bl_timer.h"
#include "bl_mem.h"
#include "ll_target.h"

// #include "mcu_drv_dflash.h"
#include "mem_ee.h"

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define SA_SK_LEN           (0x05u)
#define SA_RS_LEN           (0x01u)
#define SA_MSG_MIN_LENGTH       (1u)

#define DEFAULT_SEC_CONST   (0xFFFFFFFFul)
#define ROR3(x) ((UINT8)((UINT8)((x) >> 3u) | (UINT8)((x) << 5u)))


/*---------------------------------------------------------------------------*/
/* 常量定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 变量定义                                                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* 外部参照变量                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 内部函数声明                                                              */
/*---------------------------------------------------------------------------*/
static bool DlCheckKey(UINT32 u32Key, UINT32 u32Seed);
static UINT32 DlJacSecu(UINT32 u32wSeed);
static void DlEncipher(UINT16 u16NumRounds, UINT32 u32V[2u], UINT32 const u32Key[4u]);


/******************************************************************************
*  function name | DlCheckKey
*  content       | 校验key是否正确.
*  parameter     | key:钥匙; seed:种子
*  return        | bool
*  notice        | 无
******************************************************************************/
static bool DlCheckKey(UINT32 u32Key, UINT32 u32Seed)
{
    return (DlJacSecu(u32Seed)== u32Key) ? true : false;
}


/******************************************************************************
*  function name | DlJacSecu
*  content       | 江淮密钥算法.
*  parameter     | key:钥匙; seed:种子
*  return        | bool
*  notice        | 无
******************************************************************************/
static UINT32 DlJacSecu(UINT32 u32wSeed)
{
    UINT32  u32Key[4u];
    UINT32  u32V[2u];
    UINT8   u8SecConstBuffer[SA_CONST_BUFFER];
    UINT8   i;
	
    /* Read the const from non-volatile memory */
    if(false == ApiNvramReadSyncInd(EEPID_DIAGDID_0xF18C,4,22,&u8SecConstBuffer[0]))
    {
        /* Error during sec const read - use the default */
        for (i=0u; i<4u; i++)
        {
            u8SecConstBuffer[i] = (UINT8)DEFAULT_SEC_CONST;
        }
    }
    else
    {
    /*	u8SecConstBuffer[0] = (UINT8)DEFAULT_SEC_CONST;
		u8SecConstBuffer[1] = (UINT8)DEFAULT_SEC_CONST;
		u8SecConstBuffer[2] = (UINT8)DEFAULT_SEC_CONST;
		u8SecConstBuffer[3] = (UINT8)DEFAULT_SEC_CONST;*/
        /* successful read */
    }

    /* copy uint8 array into uint32 array
    E.g. SA constant in EEPROM is 0x11,0x22,0x33,0x44, then argument in encipher
    function uint32 k[4] will be (0x00000011,0x00000022,0x00000033,0x00000044).
    */
    for(i=0u; i<4u; i++)
    {
        /* calculate security level 2 constant, initialize cipher registers */
        u32Key[i] = (UINT32)(ROR3(u8SecConstBuffer[i]));
    }

    /*
    The input parameter for the XTEA is defined as:
    V[0] = {seed0; seed1; seed2; seed3]} (seed0 as the MSB).
    V[1] = {~(seed0);~(seed1);~(seed2);~(seed3)}.
    */
    u32V[0] = u32wSeed;
    u32V[1] = (UINT32)~u32wSeed;

    /* apply alghorithm */
    DlEncipher(2u, u32V, u32Key);

    /* After the calculation, the key is read from V[0] */
    return u32V[0];
}


/******************************************************************************
*  function name | DlEncipher
*  content       | 
*  parameter     | 
*  return        | void
*  notice        | 无
******************************************************************************/
static void DlEncipher(UINT16 u16NumRounds, UINT32 u32V[2u], UINT32 const u32Key[4u])
{
    UINT16 i;
    UINT32 u32V0=u32V[0], u32V1=u32V[1u], u32Sum=(UINT32)0uL, u32Delta=(UINT32)0x9E3779B9uL;
    for (i=0u; i < u16NumRounds; i++)
    {
        u32V0 += (((u32V1 << 4u) ^ (u32V1 >> 5u)) + u32V1) ^ (u32Sum + u32Key[u32Sum & 3u]);
        u32Sum += u32Delta;
        u32V1 += (((u32V0 << 4u) ^ (u32V0 >> 5u)) + u32V0) ^ (u32Sum + u32Key[(u32Sum>>11u) & 3u]);
    }
    u32V[0]=u32V0; u32V[1u]=u32V1;
}


/******************************************************************************
*  function name | securityaccess
*  content       | It is the main command handler to handle security access.
*  parameter     | data:数据; length:长度
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiDlSecurityAccess(const UINT8 *pu8Data,UINT16 u16Length)
{
    static UINT32 u32SaSeed;
    UINT8 *pu8Response;
	UINT8 u8Sa[64];
	INT32 s32Ret = 0;

    /* abort all operations and do actions or return without action */
    if (ApiDlAbortPendingOperations(ALL) == false)
    {
        return;
    }

    /* At least the first param should be arrived*/
    if (u16Length < SA_MSG_MIN_LENGTH)
    {
        (void)ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);
        return;
    }

    if(pu8Data[0] == requestSeed)           /*请求种子*/
    {
        /*if a seed has been sent before */
        if(false != tSemaphores.bSaSeedSent)
        {
            /*invalidate the previous seed*/
            tSemaphores.bSaSeedSent = false;
        }
        if(u16Length != SA_RS_LEN)
        {
            (void)ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);
            return;
        }
        /* if the previous SA has been interrupted or invalid key received*/

		s32Ret = ApiNvramReadSyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, 0, &u8Sa[0]);  /*读取上一次密钥交换是否失败*/
		
        if ((s32Ret == OK) && ((u8Sa[7] != false) && (u8Sa[7] != 0xFF)))   /*如果数据读取正确且上次密钥交换失败*/
        {
            /* delay timer hasn't expired (but running)*/
            bool bIsTimerStopped = ApiIsTimerStopped(securityDelay_timer_handle);
            if((not_expired == ApiCheckTimer(securityDelay_timer_handle)) && (false == bIsTimerStopped))   /*如果延时未结束*/
            {
            //    (void)ApiDlSendNegativeResponse((UINT16)requiredTimeDelayNotExpired); /*回复37延时要求不满足*/
            //    return;
            }
            else               /*延时结束*/
            {
                /* requesting a seed is valid . So go on! */
            }
        }
        else                    /*如果上次密钥交换成功，停止计数*/
        {
            /*stop the timer if the security access is allowed*/
            /*this have to be done, because if this access is not successful the
             * SA_MUTE will be true and next time it would end in a negative response*/
            ApiStopTimer(securityDelay_timer_handle);
        }

        /*get response buffer from the diagnostic layer */
        pu8Response = ApiDlGetResponseBuffer();
        pu8Response[0] = requestSeed;

        if(tSemaphores.bLocked != false)            /*如果未解锁*/
        {

            u32SaSeed = ApiLlGetSecuritySeed(u32SaSeed);  /*获取随机种子*/

            /* MSB of the seed must go the bus first */
            pu8Response[1] = (UINT8)(u32SaSeed >> 24);
            pu8Response[2] = (UINT8)(u32SaSeed >> 16);
            pu8Response[3] = (UINT8)(u32SaSeed >> 8);
            pu8Response[4] = (UINT8)u32SaSeed;

            tSemaphores.bSaSeedSent = true;
        }
        else
        {
            /*if already in the unlocked state, the SA was successful */
             /*REQ: BL_SAIC057 - SA returns 0 as a seed if the ECU is already unlocked */
            pu8Response[1] = 0u;
            pu8Response[2] = 0u;
            pu8Response[3] = 0u;
            pu8Response[4] = 0u;
        }
        (void)ApiDlSendPositiveResponse(pu8Response,5);
    }
    else if(pu8Data[0] == sendKey)             /*发送key*/
    {

        if(u16Length != SA_SK_LEN)
        {
            (void)ApiDlSendNegativeResponse((UINT16)incorrectMessageLengthOrInvalidFormat);

        }
        /*if there wasn't previous RequestSeed request*/
        else if(tSemaphores.bSaSeedSent == false)
        {
            (void)ApiDlSendNegativeResponse((UINT16)requestSequenceError);

        }
        else if(DlCheckKey(GET_UINT32(&pu8Data[1]), u32SaSeed) != false)   /*如果上位机发送的key和FBL计算的key一致*/
        {
            pu8Response = ApiDlGetResponseBuffer();
            pu8Response[0] = sendKey;
            (void)ApiDlSendPositiveResponse(pu8Response,1);

            tSemaphores.bLocked = false;
            /*if sa was successful clear sa_mute */
            ApiNvramReadSyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, 0, &u8Sa[0]);  /*如果密钥交换成功，清除失败标志位*/
			u8Sa[7] = 0;
			ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8Sa[0]);
        }
        else
        {
            /* an invalid key requires the client to start from SA-requestSeed (ISO-14229)*/
            tSemaphores.bSaSeedSent = false;

			s32Ret = ApiNvramReadSyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, 0, &u8Sa[0]);
			
            if ((s32Ret != TRUE) && ((u8Sa[7] != false) &&(u8Sa[7] != 0xff)))
            {
                /* request should be rejected */
               (void)ApiDlSendNegativeResponse((UINT16)exceededNumberOfAttempts); 

                /* start the delay timer */
                ApiResetTimer(securityDelay_timer_handle);           /*重置密钥交换延时时间*/
            }
            else                     /*如果是第一次、上电或者复位情况下*/
            {
                /* Set security mute in case of power up/ECU reset happen*/

				ApiNvramReadSyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, 0, &u8Sa[0]);
				u8Sa[7] = 1;
				ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8Sa[0]);

                (void)ApiDlSendNegativeResponse((UINT16)invalidKey);
            }
        }
    }
    else
    {
        (void)ApiDlSendNegativeResponse((UINT16)subFunctionNotSupportedInActiveSession);
    }
}





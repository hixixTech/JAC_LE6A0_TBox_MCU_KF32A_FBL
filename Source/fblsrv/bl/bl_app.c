/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    bl_app.c                                                    */
/* Description:  Application validation using Validation Table               */
/*                     Sets a ValidFlag if the application is valid.         */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** �޸���ʷ *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     �»�        ���ߴ���                              */
/* 2019-10-24   V11.01.01.01     �»�        ����ι������                              */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* ͷ�ļ�                                                                    */
/*---------------------------------------------------------------------------*/
#include "dl_engine.h"
#include "bl_app.h"
#include "bl_config_pc.h"
// #include "mcu_drv_flash.h"
// #include "mcu_drv_dflash.h"
#include "ecu_can.h"
#include "mem_ee.h"
#include "ll_target.h"
#include "bl_crc32.h"
#include "dl_service.h"
#include "bl_timer.h"
#include "bl_mem.h"
#include "os_log.h"
// #include "mcu_drv_wdg.h"
extern JumpToApp();

/*---------------------------------------------------------------------------*/
/* ���Ͷ���                                                                  */
/*---------------------------------------------------------------------------*/
typedef enum 
{
    reset_or_updated = 0,
    clear_requested,
    cleared
} InvalidationSateTyp_e;

/* Validation Table software block informations */
typedef struct 
{
    UINT32 u32BlockStart;
    UINT32 u32BlockEnd;
    UINT32 u32BlockCrc;
}ValBlockType_t;

typedef struct 
{
    UINT32 u32MemoryAddress;
    UINT32 u32MemorySize;
    UINT32 u32StoredCrc;
    UINT32 u32Crc;
    MemType_e eMemType;
    bool bIsProcessing;
}CrcType_t;

/*---------------------------------------------------------------------------*/
/* �궨��                                                                    */
/*---------------------------------------------------------------------------*/
#define DEFAULT_SIS_SCS     (UINT8)(0xFFu)
#define DEBUG_BITS          (UINT8)(0xF4u)  /* not used but all must be cleared when SIS/SCS valid */
#define COMDEBUG_BITS       (UINT8)(0xF6u)

#define ROUTINE_FINISHED    (UINT8)(1u<<3)  /* means SIS or SCS state updated */
#define APP_VALID_MASK      (UINT8)(1u<<0)  /* if bit is 1, module is invalid Ӧ�����������״̬*/
#define CFG_VALID_MASK      (UINT8)(1u<<1)  /* if bit is 1, module is invalid ��������������״̬*/
#define CAL_VALID_MASK      (UINT8)(1u<<2)  /* if bit is 1, module is invalid �궨����������״̬*/
#define ALL_MODULE_VALID    (UINT8)(0u)     /* all bit '0', all module valid  ȫ0���������Լ��ɹ�*/

#define APP_CRC_BUFFER_SIZE 128

#ifndef ENDIAN_CONV32 /* big endian target does not need any conversion */
    #define ENDIAN_CONV32(param) do { } while (0)
#endif

#ifndef LL_TARGET_SPECIFIC_JUMP
    #define LL_TARGET_SPECIFIC_JUMP(addr) do { } while (0)
#endif

#define COMPARE_LOWER_WORD(param1,param2)  ((UINT16)(param1) == (UINT16)(param2))
#define COMPARE_UPPER_WORD(param1,param2)  ((((param1) ^ (param2)) >> 16) == 0)

#define CODE_VALIDATION     (0u)   /* Application program */
#define CONF_VALIDATION     (1u)   /* EOL   */

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* ��������                                                                  */
/*---------------------------------------------------------------------------*/
static InvalidationSateTyp_e  eInvalidationState = reset_or_updated;
static UINT8  u8ActualBlock       = 0u;
static bool   bBlockValid        = true;
static bool   bProgrammingStarted = false;

static UINT32 u32BlockAddress    = VAL_TABLE_ADDR_APP;
static bool   bInitShadow        = false;
static UINT8  u8SISShadow        = 0u;
static UINT8  u8SCSShadow        = 0u;
static UINT8  u8InvalidateTarget = 0u;


/*---------------------------------------------------------------------------*/
/* �ⲿ���ձ���                                                              */
/*---------------------------------------------------------------------------*/
UINT8 gu8InvalidateFlag = 0;

/*---------------------------------------------------------------------------*/
/* �ڲ���������                                                              */
/*---------------------------------------------------------------------------*/
static void BlUpdateSisScs(void);


/******************************************************************************
*  function name | BlUpdateSisScs
*  content       | 
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
static void BlUpdateSisScs(void)
{
    UINT8 u8Prg[64];
	INT32 s32Ret; 
	
	s32Ret = ApiNvramReadSyncInd(EEPID_FLHVLD,EEPID_FLHVLD_LEN,&u8Prg[0]); 			
	if(ERROR != s32Ret)
	{
		u8Prg[2] = u8SCSShadow;
		u8Prg[1] = u8SISShadow;
		ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8Prg[0]);
	}
}


/******************************************************************************
*  function name | ApiApplicationStart
*  content       | ��תapp����
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiApplicationStart(void)           /*��תapp����*/
{
	UINT8 u8AppCheck[4] = {0x00};

	UINT8 u8Fill[64] = {DATA_0xFF_64bytes};
    /*if the startup timeout has expired*/
    if(ApiCheckTimer(startup_timer_handle) == expired)
    {
		INT32                s32Result;

		s32Result = ApiNvramReadSyncInd(EEPID_FLHVLD,4,&u8AppCheck[0]);

		if(FALSE == s32Result)
		{
			ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8Fill[0]);
            
			// ApiCanDeinit();
			// ApiWdtSwtFeedDog();         /*����ι��*/
			// ApiWdtHwtFeedDog();
            ApiLogPrint(_LOG_DEBUG,"FBL:READ FLHVLD FAIL��!\n");
			ApiLlApplicationPrestart();                                 /*�ر�taub*/
			JumpToApp();
            // asm("mov 0x00038000,r7 ");
			// asm( "jmp [r7]" );
		}

		/* check stored integrity, compatibility */
		if ( (s32Result == TRUE) && ( (0x01 == u8AppCheck[3]) || (0xff == u8AppCheck[3]) ) )  /*������Ϊ�꣬�������ͨ������������ȫff����app*/
		{	
			// ApiCanDeinit();
			// ApiWdtSwtFeedDog();         /*����ι��*/
			// ApiWdtHwtFeedDog();
			ApiLlApplicationPrestart();
            
            ApiLogPrint(_LOG_DEBUG,"FBL:about to jump��!\n");
            ApiLogProcess();
            JumpToApp();
			// asm("mov 0x00038000,r7 ");   /*������Ϊ��LL_TARGET_SPECIFIC_JUMP*/
			// asm( "jmp [r7]" );
		}
		else
		{
            /* application is invalid (integrity on compatibility error) */
			// ApiWdtSwtFeedDog();         /*����ι��*/
            ApiLogPrint(_LOG_DEBUG,"FBL:CAN NOT jump��!u8AppCheck[3]=%d\n",u8AppCheck[3]);
			gu8InvalidateFlag = 1;
			ApiStopTimer(startup_timer_handle);
		}
    }
}


/******************************************************************************
*  function name | ApiApplCompatibilityChk
*  content       | �����Լ��
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiApplCompatibilityChk(void)                      /*�����Լ��*/
{
    UINT8 u8SwCompatibilityStatus = DEFAULT_SIS_SCS & (UINT8)~(COMDEBUG_BITS | ROUTINE_FINISHED);

	UINT8 u8AppComCheck[64] = {0x00};
	INT32 s32Ret =0;

    eInvalidationState = reset_or_updated;  /* updated */

    if(ALL_MODULE_VALID == ApiGetSwIntegrityStatus())     /*�ж������ԣ���������Բ������������Ϊ����*/
    {
        /* check compatibility */
		UINT32 comp_bytes_cod, comp_bytes_cfg, comp_bytes_cal;

		ApiLlCopyBuffer((UINT8 *)&comp_bytes_cod,(UINT8 *)COM_TABLE_ADDR_APP,4);	/*��ȡapp��fbl������*/

        ApiLlCopyBuffer((UINT8 *)&comp_bytes_cfg,(UINT8 *)COM_TABLE_ADDR_APP2,4);	/*��ȡapp��eol�����Ե�ַ*/

		ApiLlCopyBuffer((UINT8 *)&comp_bytes_cal,(UINT8 *)COM_TABLE_ADDR_EOL,4);	/*��ȡeol��app�����Ե�ַ*/

		/* check compatibility among modules */
		ENDIAN_CONV32(comp_bytes_cod);
		ENDIAN_CONV32(comp_bytes_cfg);
		ENDIAN_CONV32(comp_bytes_cal);
			
        if((APP_FBL == comp_bytes_cod) && (comp_bytes_cfg == comp_bytes_cal))
        {
            u8SwCompatibilityStatus &= (UINT8)~APP_VALID_MASK;
        }
    } /* else SIS invalid */

	u8SCSShadow = u8SwCompatibilityStatus;

	s32Ret = ApiNvramReadSyncInd(EEPID_FLHVLD,EEPID_FLHVLD_LEN,&u8AppComCheck[0]); 			
	if(ERROR != s32Ret)
	{
		u8AppComCheck[2] = u8SwCompatibilityStatus;
		ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8AppComCheck[0]);	   /*���¼����Ա�־λ*/
	}
    tSemaphores.bAppValidDone = true;
}


/******************************************************************************
*  function name | ApiApplIntegrityChk
*  content       | �����Լ��
*  parameter     | void
*  return        | bool
*  notice        | ��
******************************************************************************/
bool ApiApplIntegrityChk(void)                     /*�����Լ��*/
{
    static CrcType_t tCrc;
    static UINT8 u8ValidationTable = CODE_VALIDATION;
    static UINT8 u8OngoingSIS       = DEFAULT_SIS_SCS;
    bool    bValidationFinished   = false;

    ValBlockType_t tBlock;
    UINT32  u32TableAddr;
    INT16   s16Result;
    UINT8   u8CrcBuffer[APP_CRC_BUFFER_SIZE];
    MemType_e   eTableMemtype;
    bool    bIsTimerStopped;
    bool    bRetValue = false;

	UINT8 u8AppIntCheck[64] = {0x00};
	INT32 s32Ret;
	static RcMemory_t tAppInt;

    if(false == tCrc.bIsProcessing)
    {
    	ApiDlGetRDLParam(&tAppInt.u32MemoryAddress, &tAppInt.u32MemorySize, &tAppInt.eMemType);
		
        if (CODE_VALIDATION == u8ValidationTable)    /*��Ч�Լ���ַΪapp*/
        {
            eInvalidationState = reset_or_updated; /* update is ongoing */
            /* store SIS in RAM until validation is ongoing */
            u8OngoingSIS    = DEFAULT_SIS_SCS & (UINT8)~(DEBUG_BITS | ROUTINE_FINISHED);
            eTableMemtype = VAL_TABLE_MEMTYPE_COD;
            u32TableAddr    = VAL_TABLE_ADDR_APP;
			u8ActualBlock = 1;
        }
        else if(CONF_VALIDATION == u8ValidationTable)
        {
            eTableMemtype = VAL_TABLE_MEMTYPE_CFG;
            u32TableAddr    = VAL_TABLE_ADDR_EOL;
        }
		else
		{

		}

        /* this timer has to be started in the service fg task!!! */
        bIsTimerStopped = ApiIsTimerStopped(app_valid_timer_handle);
        if ((ApiCheckTimer(app_valid_timer_handle) == expired)
             || (false != bIsTimerStopped))
        {
            /*send wait before start the checksum calculation*/
            (void) ApiDlSendNegativeResponse((UINT16)responsePending);
            ApiInitTimer(app_valid_timer_handle,P2xCAN_TO);
            /*signal to the upper layer that a responsePending has
             * been sent out */
            bRetValue = true;
        }
        else
        {/* Do nothing */
        }

        /* This block has to be processed*/
        if(u8ActualBlock < TABLE_NUM)                 /*����У���block������app��eol��bt��������Ϊ��*/
        {
        	if(VAL_TABLE_MEMTYPE_COD == eTableMemtype)
        	{
        		ApiLlCopyBuffer((UINT8 *)&tBlock,(UINT8 *)u32TableAddr,sizeof(tBlock));  /*��ȡУ���������*/
        	}
			else
			{

			}

            /* Little endian MCU-s needs a little conversion */
            ENDIAN_CONV32(tBlock.u32BlockStart);                  /*flash��ַ��ȡ����������Ҫ��offset����*/
            ENDIAN_CONV32(tBlock.u32BlockEnd);
            ENDIAN_CONV32(tBlock.u32BlockCrc);

            tCrc.u32MemoryAddress = tBlock.u32BlockStart;              /*��flash�ж�ȡ��ʼ��ַ*/
            tCrc.u32MemorySize = tBlock.u32BlockEnd - tBlock.u32BlockStart +1;   /*���㳤��*/
            tCrc.u32StoredCrc = tBlock.u32BlockCrc;     /*��flash�ж�ȡcrcֵ�����Լ������ֵ���Ա�*/
            //block_address +=sizeof(block);              /*��ȡ����У��ĵ�ַ����ַ��Ч�Լ��*/
			u32BlockAddress = u32BlockAddress + CRC_OFFSET;   /*ʵ�ʵ�crc�洢��ַ��ƫ��8�ֽڵ�*/

            tCrc.eMemType = ApiValidateFlashRead(tCrc.u32MemoryAddress,tCrc.u32MemorySize);  /*��ַ��Ч�Լ��*/

            if(Invalid != tCrc.eMemType)
            {	
				tCrc.u32Crc = 0xFFFFFFFFu; 
				tCrc.bIsProcessing = true;
            }
            else
            {
                /*invalid block*/
                bBlockValid = false;
            }
        }
        else
        {
           /*we are at the end of the block list - evaluate result*/
           bValidationFinished = true;
        }

    }
    else /* crc_struct.is_processing */
    {
		if(tAppInt.eMemType == INTFLASH)
		{
		    s16Result = ApiFlashRead(&u8CrcBuffer[0],tCrc.u32MemoryAddress,
			                     (UINT16)((tCrc.u32MemorySize>=APP_CRC_BUFFER_SIZE) ? APP_CRC_BUFFER_SIZE : tCrc.u32MemorySize));
				
		}
		else
		{

		}
	
        if(s16Result<0)
        {
            /*read failed*/
            bBlockValid = false;
        }
        else
        {
            /* Note on explicit cast of read_result: int16 -> uint32
             * The if case checked whether read_result is negative,
             * so in the else case it is assured to be non-negative. */
            tCrc.u32MemoryAddress+= (UINT32) s16Result;
            tCrc.u32MemorySize-= (UINT32) s16Result;
			
			tCrc.u32Crc = ApiCrc32Calc(tCrc.u32Crc,&u8CrcBuffer[0],(UINT16)s16Result);
			
            if(0u == tCrc.u32MemorySize)                  /*���У�鳤��Ϊ0����crcУ�����*/
            {
                tCrc.bIsProcessing = false;             /*crcУ����������־λ*/
                /*step to the next block*/
                u8ActualBlock++;                               /*У����һ��ģ��*/

                if(tCrc.u32Crc != tCrc.u32StoredCrc)   /*��s19�ļ��е�crcֵ�Ƿ�һ��*/
                {
                    /*invalid crc*/
                    bBlockValid = false;                     /*��һ����У��ʧ��*/
                }
                else
                {
                    /*block is valid, so go on to the next*/
                }
				bValidationFinished = true;
            }
            if ((ApiCheckTimer(app_valid_timer_handle) == expired))
            {
                /*send wait before start the next block of checksum calculation*/
                 (void) ApiDlSendNegativeResponse((UINT16)responsePending);          /*��ʱ������PENDING״̬*/
                ApiInitTimer(app_valid_timer_handle,P2xCAN_TO);
                /* signal to the upper layer that a responsePending has been sent out */
                bRetValue = true;
            }
            else
            {/* Do nothing */
            }
        }
    }
    /*if validation failed or we are at the end of the validation table */
	
    if((false == bBlockValid) || (false != bValidationFinished))       /*У��ʧ�ܻ�������ģ��У�����*/
    {
        if ( CODE_VALIDATION == u8ValidationTable )
        {
            /* code part done */
            if (false != bBlockValid)
            {
                u8OngoingSIS &= (UINT8)~APP_VALID_MASK;
            }
            /* See data part next */
            u8ValidationTable = CONF_VALIDATION;
            u32BlockAddress = VAL_TABLE_ADDR_EOL;
        }
        else
        {
            UINT8 oldStoredSIS = DEFAULT_SIS_SCS;   /* we hope we can read it */
            /* calibration part done */
            if (false != bBlockValid)
            {
            	if(CONF_VALIDATION == u8ValidationTable)
            	{
            		u8OngoingSIS &= (UINT8)~CFG_VALID_MASK;
            	}
				else
				{
					u8OngoingSIS = (UINT8)ALL_MODULE_VALID;
				}
                
            }

			u8SISShadow = u8OngoingSIS;

			s32Ret = ApiNvramReadSyncInd(EEPID_FLHVLD,EEPID_FLHVLD_LEN,&u8AppIntCheck[0]); 			
			if(ERROR != s32Ret)
			{
				u8AppIntCheck[1] = u8OngoingSIS;
				ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8AppIntCheck[0]);	   /*���������Ա�־λ*/
			}
	
            /* now it is ready, both part checked. Next is code part again */
            u8ValidationTable = CODE_VALIDATION;
            u32BlockAddress = VAL_TABLE_ADDR_APP;
            tSemaphores.bAppIntegrityDone = true;
        }
        /* initialize block valid for the next validation process*/
        u8ActualBlock = 0;
        bBlockValid = true;
    }
    return bRetValue;
}


/******************************************************************************
*  function name | ApiApplicationValidate
*  content       | app��Ч��
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
bool ApiApplicationValidate(void)
{
	UINT8 u8ReadBuf[1];
    if(bInitShadow != true)
    {
        // //TODO:OFFSET:1&2
		// (void)ApiNvramReadSyncInd(EEPID_FLHVLD, 1, 1, &u8ReadBuf[0]);
        // u8SISShadow = u8ReadBuf[0];
		// (void)ApiNvramReadSyncInd(EEPID_FLHVLD, 1, 2, &u8ReadBuf[0]);
        // u8SCSShadow = u8ReadBuf[0];
        (void)ApiNvramReadSyncInd(EEPID_FLHVLD, 3, &u8ReadBuf[0]);
        u8SISShadow = u8ReadBuf[1];
        u8SCSShadow = u8ReadBuf[2];
        bInitShadow = true;
    }

    if(true != tSemaphores.bAppIntegrityDone)
    {
        return ApiApplIntegrityChk();
    }
    else if(true != tSemaphores.bAppValidDone)
    {
        ApiApplCompatibilityChk();
        bInitShadow = false;
    }
    else
    {
        bInitShadow = false;
    }
    BlUpdateSisScs();
    return false;
}


/******************************************************************************
*  function name | ApiApplicationInvalidate
*  content       | app��Ч��
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiApplicationInvalidate(void)
{
	INT32     s32Ret;
	UINT8     u8App[64] = {0x00};


    if (clear_requested != eInvalidationState)
    {
        return; /* clear not requested */
    }

	s32Ret = ApiNvramReadSyncInd(EEPID_FLHVLD,EEPID_FLHVLD_LEN,&u8App[0]);			
	if(ERROR != s32Ret)
	{
		u8App[1] = 0x01;
		u8App[2] = 0x01;
		u8App[3] = 0x00;
		ApiNvramWritAsyncInd(EEPID_FLHVLD, EEPID_FLHVLD_LEN, &u8App[0]);	   /*�����������Ч�Եȱ�־λ*/
	}

    /* Initialize block valid for the next validation process */
    u8ActualBlock = 0;
    bBlockValid = true;
    u32BlockAddress = VAL_TABLE_ADDR_APP;

    tSemaphores.bAppIntegrityDone = false;
    tSemaphores.bAppValidDone = false;

    eInvalidationState = cleared; /* no clear needed until RC validation */
}


/******************************************************************************
*  function name | ApiGetSwCompatibilityStatus
*  content       | ��Ч��״̬
*  parameter     | void
*  return        | uint8
*  notice        | ��
******************************************************************************/
UINT8 ApiGetSwCompatibilityStatus(void)
{
    UINT8 u8SwCompatibilityStatus[3] = {0,0,DEFAULT_SIS_SCS};

	INT32 s32Result;

	if(bInitShadow != true)
	{
	s32Result = ApiNvramReadSyncInd(EEPID_FLHVLD,3,&u8SwCompatibilityStatus[0]);   /*��ȡ�����Լ��*/

    if(OK == s32Result)
	    {
        return (u8SwCompatibilityStatus[2]);
	    }
	    else
	    {
	        return DEFAULT_SIS_SCS;
	    }
	}
	else
	{
		return u8SCSShadow;
	}
}


/******************************************************************************
*  function name | ApiGetSwIntegrityStatus
*  content       | ������״̬
*  parameter     | void
*  return        | uint8
*  notice        | ��
******************************************************************************/
UINT8 ApiGetSwIntegrityStatus(void)
{
	UINT8 u8SwIntegrityStatus[2] = {0,DEFAULT_SIS_SCS};  /*��ֵ����Ϊ0x00����0xff*/
	INT32 s32Result = 0;

	if(bInitShadow != true)
	{
	    s32Result = ApiNvramReadSyncInd(EEPID_FLHVLD,2,&u8SwIntegrityStatus[0]);   /*��ȡ�����Լ��*/

        if(OK == s32Result)
	    {
            return (u8SwIntegrityStatus[1]);
	    }
	    else
	    {
	        return DEFAULT_SIS_SCS;
	    }	
	}
	else
	{
		return u8SISShadow;
	}
}


/******************************************************************************
*  function name | ApiSetInvalidationTarget
*  content       | ������Ч״̬
*  parameter     | void
*  return        | void
*  notice        | ��
******************************************************************************/
void ApiSetInvalidationTarget(void)
{
    /* for DID related services */
    bProgrammingStarted = true;

    if (reset_or_updated == eInvalidationState)
    {
        eInvalidationState = clear_requested;
    }
    else
    {
        /* already cleared or already clear_requested */
    }
}


/******************************************************************************
*  function name | ApiIsProgrammingStarted
*  content       | ��ȡˢ��״̬
*  parameter     | void
*  return        | bool
*  notice        | ��
******************************************************************************/
bool ApiIsProgrammingStarted(void)
{
    return bProgrammingStarted;
}



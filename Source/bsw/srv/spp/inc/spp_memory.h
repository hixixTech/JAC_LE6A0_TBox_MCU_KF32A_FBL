#ifndef _DIAS_SPP_MEMORY_H_
#define _DIAS_SPP_MEMORY_H_

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
//#include "common_error_code.h"


#define DIAS_SPP_MEM_MAX_COUNT 192
#define DIAS_SPP_MEM_UINT_SIZE (sizeof(DIAS_SPP_MEM_TYPE))
#define DIAS_SPP_MEM_BUF_SIZE (DIAS_SPP_MEM_UINT_SIZE * DIAS_SPP_MEM_MAX_COUNT)

typedef enum
{
    DIAS_SPP_MEM_UNUSABLE = 0,
    DIAS_SPP_MEM_USABLE = 1,
}DiasSppMemStatus_e;	

typedef DiasSppListNode_t DIAS_SPP_MEM_TYPE;

DiasErrorCode_e DiasSppMemInit(void);
DiasErrorCode_e DiasSppMemUninit(void);
DIAS_SPP_MEM_TYPE * DiasSppMemAddNode(DIAS_SPP_MEM_TYPE* ptMemList, DIAS_SPP_MEM_TYPE* ptNodeToAdd);
DIAS_SPP_MEM_TYPE * DiasSppMemFindNode(DIAS_SPP_MEM_TYPE* ptMemList, UINT16 u16Cmdid, UINT8 u8CmdSplitedNumCur);
DIAS_SPP_MEM_TYPE* DiasSppMemDeleteNodebyCmdid(DIAS_SPP_MEM_TYPE* ptMemList, UINT16 u16Cmdid);
DIAS_SPP_MEM_TYPE* DiasSppMemRemoveNotHighNode(DIAS_SPP_MEM_TYPE* ptMemList);
DIAS_SPP_MEM_TYPE* DiasSppMemRemoveOneNode(DIAS_SPP_MEM_TYPE* ptMemList, DIAS_SPP_MEM_TYPE* ptNodetoRemove);
DIAS_SPP_MEM_TYPE * DiasSppMemGetFirstNode(DIAS_SPP_MEM_TYPE* ptMemList);
DiasErrorCode_e DiasSppMemFreeOneNode(DIAS_SPP_MEM_TYPE* ptNode);
UINT32 DiasSppMemGetListNum(DIAS_SPP_MEM_TYPE* ptMemList);

#endif

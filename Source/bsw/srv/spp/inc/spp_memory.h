#ifndef _SPP_MEMORY_H_
#define _SPP_MEMORY_H_

#include <stdio.h>
#include <stdlib.h>
/*#include "types.h"*//*by S811*/
//#include "common_error_code.h"


#define SPP_MEM_MAX_COUNT 80
#define SPP_MEM_UINT_SIZE (sizeof(SppMemType_t))
#define SPP_MEM_BUF_SIZE (SPP_MEM_UINT_SIZE * SPP_MEM_MAX_COUNT)

typedef enum
{
    SPP_MEM_UNUSABLE = 0,
    SPP_MEM_USABLE = 1,
}SppMemStatus_e;	

typedef SppListNode_t SppMemType_t;

SPPErrorCode_e SppMemInit(void);
SPPErrorCode_e SppMemUninit(void);
SppMemType_t * SppMemAddNode(SppMemType_t* ptMemList, SppMemType_t* ptNodeToAdd);
SppMemType_t* SppMemRemoveOneNode(SppMemType_t* ptMemList, SppMemType_t* ptNodetoRemove);
SppMemType_t* SppMemDeleteNodebySN(SppMemType_t* ptMemList, UINT8 u8Sn);

SppMemType_t * SppMemGetFirstNode(SppMemType_t* ptMemList);
SPPErrorCode_e SppMemFreeOneNode(SppMemType_t* ptNode);
UINT32 SppMemGetListNum(SppMemType_t* ptMemList);
SppMemType_t* SppMemFindLastNode(SppMemType_t* ptMemList);
UINT32 SppMemGetFreeSpace(void);


#endif

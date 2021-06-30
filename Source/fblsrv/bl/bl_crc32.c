/*****************************************************************************/
/* Copyright (C), 2019, DIAS Automotive Electronic Systems Co; Ltd.          */
/* File Name:    bl_crc32.c                                                  */
/* Description:  Calculates CRC32 of the specified memory range              */
/*                                                                           */
/* Others:       other description                                           */
/*                                                                           */
/* Processor:    RH850F1L                                                    */
/* Compiler:     GHS_201517                                                  */
/********************************** 修改历史 *********************************/
/* Date            Version       Author      Description                     */
/* 2019-07-05   V11.01.01.00     陈惠        基线创建                              */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* 头文件                                                                    */
/*---------------------------------------------------------------------------*/
#include "bl_crc32.h"
#include "bl_config_pc.h"

/*---------------------------------------------------------------------------*/
/* 类型定义                                                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* 宏定义                                                                    */
/*---------------------------------------------------------------------------*/
#define CRC32_POLY      ((UINT32) 0x04C11DB7u)
#define CRC32_REVPOLY   ((UINT32) 0xEDB88320u)
#define CRC32_INIT      ((UINT32) 0xFFFFFFFFu)

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
#ifdef CRC32_FAST
static const UINT32 CRC32Tab[] =
{
	0x00000000ul, 0x77073096ul, 0xEE0E612Cul, 0x990951BAul, 0x076DC419ul, 0x706AF48Ful, 0xE963A535ul, 0x9E6495A3ul,
	0x0EDB8832ul, 0x79DCB8A4ul, 0xE0D5E91Eul, 0x97D2D988ul, 0x09B64C2Bul, 0x7EB17CBDul, 0xE7B82D07ul, 0x90BF1D91ul,
	0x1DB71064ul, 0x6AB020F2ul, 0xF3B97148ul, 0x84BE41DEul, 0x1ADAD47Dul, 0x6DDDE4EBul, 0xF4D4B551ul, 0x83D385C7ul,
	0x136C9856ul, 0x646BA8C0ul, 0xFD62F97Aul, 0x8A65C9ECul, 0x14015C4Ful, 0x63066CD9ul, 0xFA0F3D63ul, 0x8D080DF5ul,
	0x3B6E20C8ul, 0x4C69105Eul, 0xD56041E4ul, 0xA2677172ul, 0x3C03E4D1ul, 0x4B04D447ul, 0xD20D85FDul, 0xA50AB56Bul,
	0x35B5A8FAul, 0x42B2986Cul, 0xDBBBC9D6ul, 0xACBCF940ul, 0x32D86CE3ul, 0x45DF5C75ul, 0xDCD60DCFul, 0xABD13D59ul,
	0x26D930ACul, 0x51DE003Aul, 0xC8D75180ul, 0xBFD06116ul, 0x21B4F4B5ul, 0x56B3C423ul, 0xCFBA9599ul, 0xB8BDA50Ful,
	0x2802B89Eul, 0x5F058808ul, 0xC60CD9B2ul, 0xB10BE924ul, 0x2F6F7C87ul, 0x58684C11ul, 0xC1611DABul, 0xB6662D3Dul,
	0x76DC4190ul, 0x01DB7106ul, 0x98D220BCul, 0xEFD5102Aul, 0x71B18589ul, 0x06B6B51Ful, 0x9FBFE4A5ul, 0xE8B8D433ul,
	0x7807C9A2ul, 0x0F00F934ul, 0x9609A88Eul, 0xE10E9818ul, 0x7F6A0DBBul, 0x086D3D2Dul, 0x91646C97ul, 0xE6635C01ul,
	0x6B6B51F4ul, 0x1C6C6162ul, 0x856530D8ul, 0xF262004Eul, 0x6C0695EDul, 0x1B01A57Bul, 0x8208F4C1ul, 0xF50FC457ul,
	0x65B0D9C6ul, 0x12B7E950ul, 0x8BBEB8EAul, 0xFCB9887Cul, 0x62DD1DDFul, 0x15DA2D49ul, 0x8CD37CF3ul, 0xFBD44C65ul,
	0x4DB26158ul, 0x3AB551CEul, 0xA3BC0074ul, 0xD4BB30E2ul, 0x4ADFA541ul, 0x3DD895D7ul, 0xA4D1C46Dul, 0xD3D6F4FBul,
	0x4369E96Aul, 0x346ED9FCul, 0xAD678846ul, 0xDA60B8D0ul, 0x44042D73ul, 0x33031DE5ul, 0xAA0A4C5Ful, 0xDD0D7CC9ul,
	0x5005713Cul, 0x270241AAul, 0xBE0B1010ul, 0xC90C2086ul, 0x5768B525ul, 0x206F85B3ul, 0xB966D409ul, 0xCE61E49Ful,
	0x5EDEF90Eul, 0x29D9C998ul, 0xB0D09822ul, 0xC7D7A8B4ul, 0x59B33D17ul, 0x2EB40D81ul, 0xB7BD5C3Bul, 0xC0BA6CADul,
	0xEDB88320ul, 0x9ABFB3B6ul, 0x03B6E20Cul, 0x74B1D29Aul, 0xEAD54739ul, 0x9DD277AFul, 0x04DB2615ul, 0x73DC1683ul,
	0xE3630B12ul, 0x94643B84ul, 0x0D6D6A3Eul, 0x7A6A5AA8ul, 0xE40ECF0Bul, 0x9309FF9Dul, 0x0A00AE27ul, 0x7D079EB1ul,
	0xF00F9344ul, 0x8708A3D2ul, 0x1E01F268ul, 0x6906C2FEul, 0xF762575Dul, 0x806567CBul, 0x196C3671ul, 0x6E6B06E7ul,
	0xFED41B76ul, 0x89D32BE0ul, 0x10DA7A5Aul, 0x67DD4ACCul, 0xF9B9DF6Ful, 0x8EBEEFF9ul, 0x17B7BE43ul, 0x60B08ED5ul,
	0xD6D6A3E8ul, 0xA1D1937Eul, 0x38D8C2C4ul, 0x4FDFF252ul, 0xD1BB67F1ul, 0xA6BC5767ul, 0x3FB506DDul, 0x48B2364Bul,
	0xD80D2BDAul, 0xAF0A1B4Cul, 0x36034AF6ul, 0x41047A60ul, 0xDF60EFC3ul, 0xA867DF55ul, 0x316E8EEFul, 0x4669BE79ul,
	0xCB61B38Cul, 0xBC66831Aul, 0x256FD2A0ul, 0x5268E236ul, 0xCC0C7795ul, 0xBB0B4703ul, 0x220216B9ul, 0x5505262Ful,
	0xC5BA3BBEul, 0xB2BD0B28ul, 0x2BB45A92ul, 0x5CB36A04ul, 0xC2D7FFA7ul, 0xB5D0CF31ul, 0x2CD99E8Bul, 0x5BDEAE1Dul,
	0x9B64C2B0ul, 0xEC63F226ul, 0x756AA39Cul, 0x026D930Aul, 0x9C0906A9ul, 0xEB0E363Ful, 0x72076785ul, 0x05005713ul,
	0x95BF4A82ul, 0xE2B87A14ul, 0x7BB12BAEul, 0x0CB61B38ul, 0x92D28E9Bul, 0xE5D5BE0Dul, 0x7CDCEFB7ul, 0x0BDBDF21ul,
	0x86D3D2D4ul, 0xF1D4E242ul, 0x68DDB3F8ul, 0x1FDA836Eul, 0x81BE16CDul, 0xF6B9265Bul, 0x6FB077E1ul, 0x18B74777ul,
	0x88085AE6ul, 0xFF0F6A70ul, 0x66063BCAul, 0x11010B5Cul, 0x8F659EFFul, 0xF862AE69ul, 0x616BFFD3ul, 0x166CCF45ul,
	0xA00AE278ul, 0xD70DD2EEul, 0x4E048354ul, 0x3903B3C2ul, 0xA7672661ul, 0xD06016F7ul, 0x4969474Dul, 0x3E6E77DBul,
	0xAED16A4Aul, 0xD9D65ADCul, 0x40DF0B66ul, 0x37D83BF0ul, 0xA9BCAE53ul, 0xDEBB9EC5ul, 0x47B2CF7Ful, 0x30B5FFE9ul,
	0xBDBDF21Cul, 0xCABAC28Aul, 0x53B39330ul, 0x24B4A3A6ul, 0xBAD03605ul, 0xCDD70693ul, 0x54DE5729ul, 0x23D967BFul,
	0xB3667A2Eul, 0xC4614AB8ul, 0x5D681B02ul, 0x2A6F2B94ul, 0xB40BBE37ul, 0xC30C8EA1ul, 0x5A05DF1Bul, 0x2D02EF8Dul
};

/*static const UINT32 CRC32Tab[] = {
  0x00000000L,  0x04c11db7L,  0x09823b6eL,  0x0d4326d9L,  0x130476dcL,  0x17c56b6bL,  0x1a864db2L, 
  0x1e475005L,  0x2608edb8L,  0x22c9f00fL,  0x2f8ad6d6L,  0x2b4bcb61L,  0x350c9b64L,  0x31cd86d3L,  
  0x3c8ea00aL,  0x384fbdbdL,  0x4c11db70L,  0x48d0c6c7L,  0x4593e01eL,  0x4152fda9L,  0x5f15adacL, 
  0x5bd4b01bL,  0x569796c2L,  0x52568b75L,  0x6a1936c8L,  0x6ed82b7fL,  0x639b0da6L,  0x675a1011L,  
  0x791d4014L,  0x7ddc5da3L,  0x709f7b7aL,  0x745e66cdL,  0x9823b6e0L,  0x9ce2ab57L,  0x91a18d8eL, 
  0x95609039L,  0x8b27c03cL,  0x8fe6dd8bL,  0x82a5fb52L,  0x8664e6e5L,  0xbe2b5b58L,  0xbaea46efL,  
  0xb7a96036L,  0xb3687d81L,  0xad2f2d84L,  0xa9ee3033L,  0xa4ad16eaL,  0xa06c0b5dL,  0xd4326d90L,  
  0xd0f37027L,  0xddb056feL,  0xd9714b49L,  0xc7361b4cL,  0xc3f706fbL,  0xceb42022L,  0xca753d95L,  
  0xf23a8028L,  0xf6fb9d9fL,  0xfbb8bb46L,  0xff79a6f1L,  0xe13ef6f4L,  0xe5ffeb43L,  0xe8bccd9aL,  
  0xec7dd02dL,  0x34867077L,  0x30476dc0L,  0x3d044b19L,  0x39c556aeL,  0x278206abL,  0x23431b1cL,  
  0x2e003dc5L,  0x2ac12072L,  0x128e9dcfL,  0x164f8078L,  0x1b0ca6a1L,  0x1fcdbb16L,  0x018aeb13L,  
  0x054bf6a4L,  0x0808d07dL,  0x0cc9cdcaL,  0x7897ab07L,  0x7c56b6b0L,  0x71159069L,  0x75d48ddeL,  
  0x6b93dddbL,  0x6f52c06cL,  0x6211e6b5L,  0x66d0fb02L,  0x5e9f46bfL,  0x5a5e5b08L,  0x571d7dd1L,  
  0x53dc6066L,  0x4d9b3063L,  0x495a2dd4L,  0x44190b0dL,  0x40d816baL,  0xaca5c697L,  0xa864db20L,  
  0xa527fdf9L,  0xa1e6e04eL,  0xbfa1b04bL,  0xbb60adfcL,  0xb6238b25L,  0xb2e29692L,  0x8aad2b2fL,  
  0x8e6c3698L,  0x832f1041L,  0x87ee0df6L,  0x99a95df3L,  0x9d684044L,  0x902b669dL,  0x94ea7b2aL,  
  0xe0b41de7L,  0xe4750050L,  0xe9362689L,  0xedf73b3eL,  0xf3b06b3bL,  0xf771768cL,  0xfa325055L,  
  0xfef34de2L,  0xc6bcf05fL,  0xc27dede8L,  0xcf3ecb31L,  0xcbffd686L,  0xd5b88683L,  0xd1799b34L,  
  0xdc3abdedL,  0xd8fba05aL,  0x690ce0eeL,  0x6dcdfd59L,  0x608edb80L,  0x644fc637L,  0x7a089632L,  
  0x7ec98b85L,  0x738aad5cL,  0x774bb0ebL,  0x4f040d56L,  0x4bc510e1L,  0x46863638L,  0x42472b8fL,  
  0x5c007b8aL,  0x58c1663dL,  0x558240e4L,  0x51435d53L,  0x251d3b9eL,  0x21dc2629L,  0x2c9f00f0L,  
  0x285e1d47L,  0x36194d42L,  0x32d850f5L,  0x3f9b762cL,  0x3b5a6b9bL,  0x0315d626L,  0x07d4cb91L,  
  0x0a97ed48L,  0x0e56f0ffL,  0x1011a0faL,  0x14d0bd4dL,  0x19939b94L,  0x1d528623L,  0xf12f560eL,  
  0xf5ee4bb9L,  0xf8ad6d60L,  0xfc6c70d7L,  0xe22b20d2L,  0xe6ea3d65L,  0xeba91bbcL,  0xef68060bL,  
  0xd727bbb6L,  0xd3e6a601L,  0xdea580d8L,  0xda649d6fL,  0xc423cd6aL,  0xc0e2d0ddL,  0xcda1f604L,  
  0xc960ebb3L,  0xbd3e8d7eL,  0xb9ff90c9L,  0xb4bcb610L,  0xb07daba7L,  0xae3afba2L,  0xaafbe615L,  
  0xa7b8c0ccL,  0xa379dd7bL,  0x9b3660c6L,  0x9ff77d71L,  0x92b45ba8L,  0x9675461fL,  0x8832161aL,  
  0x8cf30badL,  0x81b02d74L,  0x857130c3L,  0x5d8a9099L,  0x594b8d2eL,  0x5408abf7L,  0x50c9b640L,  
  0x4e8ee645L,  0x4a4ffbf2L,  0x470cdd2bL,  0x43cdc09cL,  0x7b827d21L,  0x7f436096L,  0x7200464fL, 
  0x76c15bf8L,  0x68860bfdL,  0x6c47164aL,  0x61043093L,  0x65c52d24L,  0x119b4be9L,  0x155a565eL, 
  0x18197087L,  0x1cd86d30L,  0x029f3d35L,  0x065e2082L,  0x0b1d065bL,  0x0fdc1becL,  0x3793a651L, 
  0x3352bbe6L,  0x3e119d3fL,  0x3ad08088L,  0x2497d08dL,  0x2056cd3aL,  0x2d15ebe3L,  0x29d4f654L,  
  0xc5a92679L,  0xc1683bceL,  0xcc2b1d17L,  0xc8ea00a0L,  0xd6ad50a5L,  0xd26c4d12L,  0xdf2f6bcbL,  
  0xdbee767cL,  0xe3a1cbc1L,  0xe760d676L,  0xea23f0afL,  0xeee2ed18L,  0xf0a5bd1dL,  0xf464a0aaL, 
  0xf9278673L,  0xfde69bc4L,  0x89b8fd09L,  0x8d79e0beL,  0x803ac667L,  0x84fbdbd0L,  0x9abc8bd5L,  
  0x9e7d9662L,  0x933eb0bbL,  0x97ffad0cL,  0xafb010b1L,  0xab710d06L,  0xa6322bdfL,  0xa2f33668L,
  0xbcb4666dL,  0xb8757bdaL,  0xb5365d03L,  0xb1f740b4L };
*/
#else
static UINT32 CRC32Tab[256];

/******************************************************************************
*  function name | ApiCrc32Init
*  content       | 初始化crc16，获得crc16的table
*  parameter     | void
*  return        | void
*  notice        | 无
******************************************************************************/
void ApiCrc32Init(void)
{
	UINT16 i, j;
	UINT32 c;
	for(i = 0; i < 256; i++)
	{
		for(c = i, j = 0; j < 8; j++)
		{
		   c = (c & 1) ? ((c >> 1) ^ CRC32_REVPOLY) : (c >> 1);
		}
		CRC32Tab[i] = c;
	}
}
#endif


/******************************************************************************
*  function name | ApiCrc32Calc
*  content       | crc32计算
*  parameter     | addr：计算地址，size：计算大小，u32StartCrc：crc32初始值
*  return        | UINT32
*  notice        | 无
******************************************************************************/
UINT32 ApiCrc32Calc(UINT32 u32StartCrc, const UINT8 *u8Addr, UINT16 u16Size)
{
	UINT32 i;
	u32StartCrc = u32StartCrc ^ 0xFFFFFFFFul;
	for(i = 0u; i < u16Size; i++)
	{
	    u32StartCrc = CRC32Tab[(UINT8)u32StartCrc ^ u8Addr[i]] ^ (u32StartCrc >> 8u);
	}
	/*invert the result*/
	u32StartCrc = u32StartCrc ^ 0xFFFFFFFFul;
	return u32StartCrc;
}





/**
  ******************************************************************************
  * 文件名  ymodem.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2019-11-16
  * 描  述  该文件提供了基于外设库的最小代码
  *
  *********************************************************************
  */
#include "system_init.h"
#include "ymodem.h"

#define EVAL_COM USART0_SFR

uint8_t file_name[FILE_NAME_LENGTH];
uint32_t FlashDestination = 0x38000; /* APP程序区的首地址，最好设置为整页的首地址 ，1页 =1K byte*/
uint32_t test[256] ;

/**
  * 描述   uart接收一个字符
  * 输入   c: 单个字符接收的缓存指针
  * 返回   0：字符接收成功
  * 	 -1：接收超时
  *
  * 注意	 timeout: 超时系数，用于设置超时的时长
  */
static  int32_t Receive_Byte (uint8_t *c )
{
  uint32_t timeout =0x50000;//超时时间必须足够长，一般要求必须大于30ms，否则上位机来不及响应

  while (timeout-- > 1)
  {
	  if( EVAL_COM->STR & 0x01) //溢出错误处理
	  {
		  EVAL_COM->STR |= 0x10000; //OVFEIC =1
		  while( EVAL_COM->STR & (uint32_t)0x01);
		  EVAL_COM->STR &= 0xfffeffff; //OVFEIC =0
	  }

	  if( EVAL_COM->STR & 0x800)  //RDRIF =1；接收缓存有数据
	  {
		  *c = (uint8_t)EVAL_COM->RBUFR; //读出接收数据
		  return 0;
	  }
  }
  return -1;
}

/**
  * 描述   Uart发送一个字符
  * 输入   c:待发送的字符
  * 返回  无
  */
static void Send_Byte (uint8_t c)
{
	USART_SendData(EVAL_COM,c);//填写Uart的发送缓存将会自动触发uart的发送行为
	while(!USART_Get_Transmitter_Empty_Flag(EVAL_COM));//发送完成标志
}

/**
  * 描述   接收一个数据包
  * 输入   data: 接收的缓存指针
  * 	 length：已接收数据包的长度
  * 	 -1：接收超时
  *返回   0：接收数据包成功
  *		1：上位机放弃传输
  *		-1：超时或数据错误
  */
static int32_t Receive_Packet (uint8_t *data, int32_t *length)
{
  volatile uint16_t i, packet_size =0;
  uint8_t c;
  *length = 0; //写入初始的数据包长度
  if (Receive_Byte(&c ) != 0) //接收第一个字符
  {
    return -1;
  }
  switch (c) //判断是哪一种数据包，并且确定数据包的长度
  {
    case SOH:
      packet_size = PACKET_SIZE; //起始包，包括文件名和文件长度，内容长度是128
      break;
    case STX:
      packet_size = PACKET_1K_SIZE; //数据包，内容长度是1024
      break;
    case EOT: //结束包
      return 0;
    default: //如果收到的第一个数据，不是标准数据包的第一个数据，直接返回不再处理剩余的数据
      return -1;
  }
  *data = c; //保存包的第一个字节
  for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++) //接收当前数据包的剩余字节
  {
    if (Receive_Byte(data + i ) != 0)
    {
      return -1;
    }
  }
  if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) //校验包的序号和反码是否正确
  {
    return -1;
  }
  *length = packet_size; //写入当前数据包长度
  return 0;
}

/**
  * 描述   接收Ymodem协议传来的文件并写入Flash
  * 输入   buf: 数据包接收的缓存指针
  *	返回   100：文件成功接收并顺利写入Flash
  *		 其他：接收数据失败
  */
int32_t Ymodem_Receive (uint8_t *buf)
{
  uint8_t  *file_ptr;
  int32_t i, packet_length,  errors =0, session_begin=0;
  uint32_t packets_received =0 ,k=0;

    while(1) //除非遇到协议传输中断或者严重错误，否者会一直在这里等待接收
    {
      if(!Receive_Packet(buf, &packet_length ))//正确的接收完一个数据包
      {
          Send_Byte(321); //PB10翻转,LED2熄灭或点亮
          errors = 0;
            switch (packet_length) //处理不同长度的数据包
            {
              case 0:    //接收到结束标志
                Send_Byte(ACK);
                return 100;
              default: //正常的数据包处理
                if (buf[PACKET_SEQNO_INDEX] != (packets_received & 0xff)) //检查当前数据包内的序号和实际收到的数据包序号的低8位是否一致
                {
                  Send_Byte(NAK); //如果不对，发送NACK
                }
                else //数据包序号验证正确
                {
/*第一个数据包处理，包含是文件名，实际上对Bootloader没有用处，实际代码可以删除该部分*/
                  if (packets_received == 0)//如果是第一个数据包
                  {
                    if (buf[PACKET_HEADER] != 0)//检查文件名内容的第一个字符，确认不是空的
                    {
                      for (i = 0, file_ptr = buf + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);) //取出文件名
                      {
                        file_name[i++] = *file_ptr++;
                      }
                      file_name[i++] = '\0'; //后面用空格结束
                      Send_Byte(ACK); //发送应答
                      Send_Byte(CRC16); //要求继续发送文件内容，发完文件头，接收器必须发C才能继续
                    }
                    else //文件名是空的，
                    {
                      Send_Byte(ACK);
                      return 0; //直接返回文件的长度就是0
                    }
                  }
/*文件内容数据包处理，包含的实际数据，单个数据包的有效长度是128byte*/
                  else //数据包处理
                  {
                  	k = (packets_received-1) % 8;
                  	memcpy ( (uint8_t *)test + k*128, buf+ PACKET_HEADER, 128);//把收到的数据按照顺序存放在test[]中
                  	if( k ==7) //每1K个字节
                  	{
                          //程序区整页写入数据
                          FLASH_PageWrite_fun( FlashDestination, test, 128);     //把一页的数据写入0x8000 所在程序区的flash PAGE
                          FlashDestination +=0x400; //每写1K，地址累加1K
                  	}
                    Send_Byte(ACK); //给个响应

                  }

                  packets_received ++; //接收包数量自增
                  if(packets_received> 80) //如果超过80 /8 *1K byte未遇到结束符，则停止接收数据
                  {
                  	Send_Byte(NAK); //给个响应
                      return 100;
                  }
                  session_begin = 1; //表示开始接收了
                }//数据包序号验证正确
            } //switch
      }
      else//字符接收超时或者内容错误处理
      {
          if (session_begin > 0) //接收过程中，错误计数
          {
            errors ++;
          }
          if (errors > MAX_ERRORS) //如果错误超过一定次数，终止接收
          {
        	Send_Byte(NAK); //给个响应
            return 0;
          }
          Send_Byte(CRC16);  //发送‘C’，要求内容重发,超时后的C都是这里发出的
      }
    } //while(1)
  return 100;
}

/**
  * 描述   计算单个字节数据的CRC16值
  * 输入   crcIn: CRC的初值
  * 	 byte：待计算CRC值的字符
  *	返回   CRC16计算的结果
  *	使用的多项式0x1021
  */
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
 uint32_t crc = crcIn;
 uint32_t in = byte|0x100;
 do
 {
 crc <<= 1;
 in <<= 1;
 if(in&0x100)
 ++crc;
 if(crc&0x10000)
 crc ^= 0x1021;
 }
 while(!(in&0x10000));
 return crc&0xffffu;
}

/**
  * 描述   计算数据包的CRC16值
  * 输入   data: 数据包的指针
  * 	 size：数据包的长度
  *	返回   CRC16计算的结果
  */
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
 uint32_t crc = 0;
 const uint8_t* dataEnd = data+size;
 while(data<dataEnd)
  crc = UpdateCRC16(crc,*data++);
 
 crc = UpdateCRC16(crc,0);
 crc = UpdateCRC16(crc,0);
 return crc&0xffffu;
}

/**
  * 描述   计算数据包的Checksum值
  * 输入   data: 数据包的指针
  * 	 size：数据包的长度
  *	返回   Checksum计算的结果
  */
uint8_t CalChecksum(const uint8_t* data, uint32_t size)
{
 uint32_t sum = 0;
 const uint8_t* dataEnd = data+size;
 while(data < dataEnd )
   sum += *data++;
 return sum&0xffu;
}


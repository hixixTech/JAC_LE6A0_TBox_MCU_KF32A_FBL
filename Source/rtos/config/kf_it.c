/****************************************************************************************
 *
 * 文件名 kf_it.c
 * 作  者	ChipON_AE/FAE_Group
 * 版  本	V2.1
 * 日  期	2019-11-16
 * 描  述  该文件提供了中断入口地址,在CAN1中断处理RAM报文
 *
 ****************************************************************************************/
#include"system_init.h"



// void __attribute__((interrupt))_CAN0_exception (void)
// {
// 	volatile uint8_t i,j;
// 	uint32_t tmprmc,tmprmd;
// 	uint32_t bus_temp;
// 	static uint8_t Receice_addr=0x00;//接收RAM偏移地址
// 	CAN_MessageTypeDef CAN_MessageStructrue;//接收报文结构体

// 	/* 判断是否为CAN发送标志 */
// 	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_TRANSMIT) != RESET)
// 	{
// 		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_TRANSMIT);

// 		CAN0_SFR->CTLR &= ~0x300;
// 	}

// 	/* 判断总线错误标志 */
// 	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_BUS_ERROR) != RESET)
// 	{
// 		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_BUS_ERROR);
// 		CAN0_SFR->CTLR &= ~0x300; //总线错误不重发
// 	}
// 	/* 判断仲裁标志 */
// 	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_ARBITRATION_LOST) != RESET)
// 	{
// 		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_ARBITRATION_LOST);
// 		bus_temp = CAN0_SFR->RCR;//读取RCR标志位，作用下一次仲裁标志位
// 		CAN0_SFR->CTLR = CAN0_SFR->CTLR | 0x300;//仲裁失败重发
// 	}

// 	/* 判断是否为CAN接收标志 */
// 	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_RECEIVE) != RESET)
// 	{
// 		GPIO_Toggle_Output_Data_Config (GPIOB_SFR,GPIO_PIN_MASK_8);
// 		tmprmc = (CAN0_SFR->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS;//当前邮箱报文邮箱计数
// 		tmprmd =CAN0_SFR->RCR >>16;                //仅A02版本有此功能，读取下一个邮箱指针地址
// 		if(tmprmd >tmprmc)
// 			Receice_addr =(tmprmd -tmprmc)*0x10;
// 		else
// 			Receice_addr =(tmprmd +16 -tmprmc)*0x10;

// 		/* 接收RAM数据 */
// 		CAN_Receive_Message_Configuration(CAN0_SFR,Receice_addr,&CAN_MessageStructrue);

// 		/* 释放一次计数器 */

// 		j=30;
//    		SFR_SET_BIT_ASM(CAN0_SFR->CTLR, CAN_CTLR_RELRX_POS);
//    		while((tmprmc - ((CAN0_SFR->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS) != 1)&j)
//    			j--;       //等待计数标志位释放完成及超时退出
//    		SFR_CLR_BIT_ASM(CAN0_SFR->CTLR, CAN_CTLR_RELRX_POS);

// 		if(CAN_MessageStructrue.m_RemoteTransmit != CAN_DATA_FRAME)//远程帧
// 		{
// 			//用户代码
// 			ReceiveData[0]=0xAA;
// 			USART_Send(USART2_SFR,ReceiveData,1);
// 		}
// 		else  //数据帧
// 		{
// 			/* 获取数据 */
// 			for(i=0;i<CAN_MessageStructrue.m_DataLength;i++)
// 			{
// 				ReceiveData[i] = CAN_MessageStructrue.m_Data[i];
// 			}
// 			/* 打印数据 */
// 			USART_Send(USART2_SFR,ReceiveData,CAN_MessageStructrue.m_DataLength);
// 		}
// 	}

// }

//*****************************************************************************************
//                              CAN1中断函数
//*****************************************************************************************	//

// void __attribute__((interrupt))_CAN1_exception (void)
// {
// 	volatile uint8_t i,j;
// 	uint32_t tmprmc,tmprmd;
// 	uint32_t bus_temp;
// 	static uint8_t Receice_addr=0x00;//接收RAM偏移地址
// 	CAN_MessageTypeDef CAN_MessageStructrue;//接收报文结构体

// 	/* 判断是否为CAN发送标志 */
// 	if(CAN_Get_INT_Flag(CAN1_SFR,CAN_INT_TRANSMIT) != RESET)
// 	{
// 		CAN_Clear_INT_Flag(CAN1_SFR,CAN_INT_TRANSMIT);

// 		CAN1_SFR->CTLR &= ~0x300;
// 	}

// 	/* 判断总线错误标志 */
// 	if(CAN_Get_INT_Flag(CAN1_SFR,CAN_INT_BUS_ERROR) != RESET)
// 	{
// 		CAN_Clear_INT_Flag(CAN1_SFR,CAN_INT_BUS_ERROR);
// 		CAN1_SFR->CTLR &= ~0x300; //总线错误不重发
// 	}
// 	/* 判断仲裁标志 */
// 	if(CAN_Get_INT_Flag(CAN1_SFR,CAN_INT_ARBITRATION_LOST) != RESET)
// 	{
// 		CAN_Clear_INT_Flag(CAN1_SFR,CAN_INT_ARBITRATION_LOST);
// 		bus_temp = CAN1_SFR->RCR;//读取RCR标志位，作用下一次仲裁标志位
// 		CAN1_SFR->CTLR = CAN1_SFR->CTLR | 0x300;//仲裁失败重发
// 	}

// 	/* 判断是否为CAN接收标志 */
// 	if(CAN_Get_INT_Flag(CAN1_SFR,CAN_INT_RECEIVE) != RESET)
// 	{
// 		GPIO_Toggle_Output_Data_Config (GPIOB_SFR,GPIO_PIN_MASK_8);
// 		tmprmc = (CAN1_SFR->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS;//当前邮箱报文邮箱计数
// 		tmprmd =CAN1_SFR->RCR >>16;                //仅A02版本有此功能，读取下一个邮箱指针地址
// 		if(tmprmd >tmprmc)
// 			Receice_addr =(tmprmd -tmprmc)*0x10;
// 		else
// 			Receice_addr =(tmprmd +16 -tmprmc)*0x10;

// 		/* 接收RAM数据 */
// 		CAN_Receive_Message_Configuration(CAN1_SFR,Receice_addr,&CAN_MessageStructrue);

// 		/* 释放一次计数器 */

// 		j=30;
//    		SFR_SET_BIT_ASM(CAN1_SFR->CTLR, CAN_CTLR_RELRX_POS);
//    		while((tmprmc - ((CAN1_SFR->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS) != 1)&j)
//    			j--;       //等待计数标志位释放完成及超时退出
//    		SFR_CLR_BIT_ASM(CAN1_SFR->CTLR, CAN_CTLR_RELRX_POS);

// 		if(CAN_MessageStructrue.m_RemoteTransmit != CAN_DATA_FRAME)//远程帧
// 		{
// 			//用户代码
// 			ReceiveData[0]=0xAA;
// 			USART_Send(USART2_SFR,ReceiveData,1);
// 		}
// 		else  //数据帧
// 		{
// 			/* 获取数据 */
// 			for(i=0;i<CAN_MessageStructrue.m_DataLength;i++)
// 			{
// 				ReceiveData[i] = CAN_MessageStructrue.m_Data[i];
// 			}
// 			/* 打印数据 */
// 			USART_Send(USART2_SFR,ReceiveData,CAN_MessageStructrue.m_DataLength);
// 		}
// 	}

// }


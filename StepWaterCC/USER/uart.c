
#include <includes.h>
//extern OS_EVENT *OSSemUart3;
//extern OS_EVENT *OSSemUart2;
//extern OS_EVENT *OSSemUart1;
#if CONFIG_UART1
uchar Uart1Sending;
uchar Uart1RecvBuf[UART1_RECV_BUF_SIZE+4];	//串口1接收缓冲区
_loopbuf Uart1RecvStruct;
uchar Uart1SendBuf[UART1_SEND_BUF_SIZE+4];	//串口1发缓冲区
_loopbuf Uart1SendStruct;
#endif

#if CONFIG_UART2
uchar Uart2Sending;
uchar Uart2RecvBuf[UART2_RECV_BUF_SIZE+4];	//串口2接收缓冲区
_loopbuf Uart2RecvStruct;
uchar Uart2SendBuf[UART2_SEND_BUF_SIZE+4];	//串口2发缓冲区
_loopbuf Uart2SendStruct;
#endif

#if CONFIG_UART3
uchar Uart3Sending;
uchar Uart3RecvBuf[UART3_RECV_BUF_SIZE+4];	//串口1接收缓冲区
_loopbuf Uart3RecvStruct;
uchar Uart3SendBuf[UART3_SEND_BUF_SIZE+4];	//串口1发缓冲区
_loopbuf Uart3SendStruct;
#endif

#if CONFIG_UART4
uchar Uart4Sending;
uchar Uart4RecvBuf[UART4_RECV_BUF_SIZE+4];	//串口2接收缓冲区
_loopbuf Uart4RecvStruct;
uchar Uart4SendBuf[UART4_SEND_BUF_SIZE+4];	//串口2发缓冲区
_loopbuf Uart4SendStruct;
#endif

//////////////////////////////////////////////////////////////////
#if CONFIG_UART5
uchar Uart5Sending;
uchar Uart5RecvBuf[UART5_RECV_BUF_SIZE+4];	//串口2接收缓冲区
_loopbuf Uart5RecvStruct;
uchar Uart5SendBuf[UART5_SEND_BUF_SIZE+4];	//串口2发缓冲区
_loopbuf Uart5SendStruct;
#endif
/*****************************************************************************/

///////////////////
//*************************************



#if CONFIG_UART1
/****************************************************************************
* 名称：void  Uart1Init(ulong baud, uchar parity)
* 功能：初始化串口1。设置其工作模式及波特率。
* 入口参数：baud                波特率
*   parity          0:无校验  1：奇校验  2：偶校验
****************************************************************************/
void  Uart1BufInit(void)
{  
CPU_SR         cpu_sr;
   OS_ENTER_CRITICAL();	//ucos2
   Uart1Sending = 0;
   ClearLoopBuf(&Uart1SendStruct,UART1_SEND_BUF_SIZE);				//清除环型缓冲区
	ClearLoopBuf(&Uart1RecvStruct,UART1_RECV_BUF_SIZE);
   
	OS_EXIT_CRITICAL();          //ucos2  
}


///////////////////


////////////
uchar GetOneFromUart1(uchar *buf)
{
	uchar flag;
	CPU_SR         cpu_sr;
	
	OS_ENTER_CRITICAL();	//ucos2   
	
	if(Uart1RecvStruct.len)
		{
		GetLoopbuf(&Uart1RecvStruct,Uart1RecvBuf,UART1_RECV_BUF_SIZE,buf);
		flag= 1;
		}
	else 	flag=  0;
	
	OS_EXIT_CRITICAL();
	
	return flag;
}


uint16 Uart1CharSend(uchar x)
{
	
	uchar temp;
	uint16 fail=0;
	uint32 t;
	CPU_SR         cpu_sr;
	
    if(Uart1SendStruct.len>3)
	    	{//full
	    	t=10;
	    	while((Uart1SendStruct.addTail == Uart1SendStruct.outTail)&&(t>0))
	    		{
	    		OSTimeDly(2);	//延时10毫秒		
	    		t--;
	    		}
	    	}
	    if(t==0)
	    	fail=1;
	    	
	 OS_ENTER_CRITICAL();	//ucos2  
	AddLoopBuf(&Uart1SendStruct,Uart1SendBuf,UART1_SEND_BUF_SIZE,x);	//加入到环型缓冲区
	OS_EXIT_CRITICAL();          //ucos2  	
	/*
	if(Uart1Sending!=0)	//准备继续发送
		{
		if(U1LSR&BIT5)//发送保持寄存器空
	   		{
	   		Uart1Sending = 0;
	   		}
		}
		//20100915
	*/
	if(Uart1Sending==0)
	             {
	              //Un485Send=FALSE;  //485允许发送
	              //U1MCR=0x03;     //DTR_232=1,RTS_232=1
	              OS_ENTER_CRITICAL();	//ucos2
	              
	              if(GetLoopbuf(&Uart1SendStruct,Uart1SendBuf,UART1_SEND_BUF_SIZE,&temp))
	              	{
	              		Uart1Sending = 1;
	              		SetEn485_1;	//SetPin_DTR1;	//uart1_485
										USART_SendData(USART1, temp);
	              		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	              	//	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	        			}
	        OS_EXIT_CRITICAL();          //ucos2  	
	             }
	return (!fail);
}








/************************************************************************/
/************************************************************************/
/*     uart1中断方式发送字符串，调用方法:				*/
/* 1.SendString_UART1("ATZ\r",sizeof("ATZ\r"));				     */
/* 2.SendString_UART1(&XYZ[0],length);					     */
/* 3.如果时间超过而没有发送完成，则返回剩余的未发送字节数，如果成功则返回0 */
/************************************************************************/
void SendString_UART1(unsigned char *StrData,uint16 len)
{
uint16 i;
	for(i=0;i<len;i++)
	  {
	  Uart1CharSend(*(StrData+i));
	  }
  
}

/*************************************************************/

void SendText_UART1(uchar *StrData)
{

	uchar i;
	for(i=0;i<255;i++)
	  {
	  if(*(StrData+i)==0x00)break;
	   Uart1CharSend(*(StrData+i));
	  }
}
#endif
///////////////////
#if CONFIG_UART2
void  Uart2BufInit(void)
{  
	CPU_SR         cpu_sr;
	
	OS_ENTER_CRITICAL();	//ucos2
	
  Uart2Sending = 0;
  ClearLoopBuf(&Uart2SendStruct,UART2_SEND_BUF_SIZE);				//清除环型缓冲区
	ClearLoopBuf(&Uart2RecvStruct,UART2_RECV_BUF_SIZE);

OS_EXIT_CRITICAL();          //ucos2  

}
///////////////////



///////////////////
uchar GetOneFromUart2(uchar *buf)
{
	uchar flag;
	CPU_SR         cpu_sr;
	
	OS_ENTER_CRITICAL();	//ucos2   
	
	if(Uart2RecvStruct.len)
		{
		GetLoopbuf(&Uart2RecvStruct,Uart2RecvBuf,UART2_RECV_BUF_SIZE,buf);
		flag= 1;
		}
	else 	flag=  0;
	
	OS_EXIT_CRITICAL();
	
	return flag;
}
///////////////////
uint16 Uart2CharSend(uchar x)
{
	
	uchar temp;
	uint16 fail=0;
	uint32 t;
	CPU_SR         cpu_sr;	
	
    if(Uart2SendStruct.len>3)
	    	{//full
	    	t=10;
	    	while((Uart2SendStruct.addTail == Uart2SendStruct.outTail)&&(t>0))
	    		{
	    		//OSTimeDly(OS_TICKS_PER_SEC / 100);	//延时10毫秒	
	    		OSTimeDly(2);	//延时10毫秒		
	    		t--;
	    		}
	    	}
	    if(t==0)
	    	fail=1;
		
	
	
	OS_ENTER_CRITICAL();	//ucos2   
	AddLoopBuf(&Uart2SendStruct,Uart2SendBuf,UART2_SEND_BUF_SIZE,x);	//加入到环型缓冲区
	OS_EXIT_CRITICAL();          //ucos2  	
	///
//	if(Uart2Sending)
//		{
//		if(U0LSR&BIT5)//发送保持寄存器空
//	   		{
//	   		 Uart2Sending = 0;
//	   		}
//		}
	///
	if(!Uart2Sending)
	             {

	              OS_ENTER_CRITICAL();	//ucos2
	              if(GetLoopbuf(&Uart2SendStruct,Uart2SendBuf,UART2_SEND_BUF_SIZE,&temp))
	              		{
	              		Uart2Sending = 1;	
	              			SetEn485_2;
										USART_SendData(USART2, temp);
	              	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	              	//	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
	        		}
	        	OS_EXIT_CRITICAL();          //ucos2  	
	             }
	 
	return (!fail);
}
///////////////////
void SendText_UART2(uchar *StrData)
{
	//	 INT8U err;
	uchar i;

		for(i=0;i<255;i++)
		  {
		  if(*(StrData+i)==0x00)break;
		   Uart2CharSend(*(StrData+i));
		  }
 
	  
}
///////////////////
void SendString_UART2(uchar *StrData,uint16 len)
{
 //  INT8U err;
	uint16 i;

	
		for(i=0;i<len;i++)
		  {
		  Uart2CharSend(*(StrData+i));
		  }

}
///////////////////


#endif
///////////////////

#if CONFIG_UART3
/****************************************************************************
* 名称：void  Uart3Init(ulong baud, uchar parity)
* 功能：初始化串口1。设置其工作模式及波特率。
* 入口参数：baud                波特率
*   parity          0:无校验  1：奇校验  2：偶校验
****************************************************************************/
void  Uart3BufInit(void)
{  
CPU_SR         cpu_sr;
   OS_ENTER_CRITICAL();	//ucos2
   Uart3Sending = 0;
   ClearLoopBuf(&Uart3SendStruct,UART3_SEND_BUF_SIZE);				//清除环型缓冲区
	ClearLoopBuf(&Uart3RecvStruct,UART3_RECV_BUF_SIZE);
   
	OS_EXIT_CRITICAL();          //ucos2  
}


///////////////////


////////////
uchar GetOneFromUart3(uchar *buf)
{
	uchar flag;
	CPU_SR         cpu_sr;
	
	OS_ENTER_CRITICAL();	//ucos2   
	
	if(Uart3RecvStruct.len)
		{
		GetLoopbuf(&Uart3RecvStruct,Uart3RecvBuf,UART3_RECV_BUF_SIZE,buf);
		flag= 1;
		}
	else 	flag=  0;
	
	OS_EXIT_CRITICAL();
	
	return flag;
}


uint16 Uart3CharSend(uchar x)
{
	
	uchar temp;
	uint16 fail=0;
	uint32 t;
	CPU_SR         cpu_sr;	
	
    if(Uart3SendStruct.len>3)
	    	{//full
	    	t=10;
	    	while((Uart3SendStruct.addTail == Uart3SendStruct.outTail)&&(t>0))
	    		{
	    		OSTimeDly(2);	//延时10毫秒		
	    		t--;
	    		}
	    	}
	    if(t==0)
	    	fail=1;
	    	
	 OS_ENTER_CRITICAL();	//ucos2  
	AddLoopBuf(&Uart3SendStruct,Uart3SendBuf,UART3_SEND_BUF_SIZE,x);	//加入到环型缓冲区
	OS_EXIT_CRITICAL();          //ucos2  	
	/*
	if(Uart3Sending!=0)	//准备继续发送
		{
		if(U1LSR&BIT5)//发送保持寄存器空
	   		{
	   		Uart3Sending = 0;
	   		}
		}
		//20100915
	*/
	if(Uart3Sending==0)
	             {
	              //Un485Send=FALSE;  //485允许发送
	              //U1MCR=0x03;     //DTR_232=1,RTS_232=1
	              OS_ENTER_CRITICAL();	//ucos2
	              
	              if(GetLoopbuf(&Uart3SendStruct,Uart3SendBuf,UART3_SEND_BUF_SIZE,&temp))
	              	{
	              		Uart3Sending = 1;
	              		SetEn485_3;
										USART_SendData(USART3, temp);
										USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	              	//	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	              		
	        			}
	        OS_EXIT_CRITICAL();          //ucos2  	
	             }
	return (!fail);
}







/************************************************************************/
/************************************************************************/
/*     uart1中断方式发送字符串，调用方法:				*/
/* 1.SendString_UART3("ATZ\r",sizeof("ATZ\r"));				     */
/* 2.SendString_UART3(&XYZ[0],length);					     */
/* 3.如果时间超过而没有发送完成，则返回剩余的未发送字节数，如果成功则返回0 */
/************************************************************************/
void SendString_UART3(unsigned char *StrData,uint16 len)
{
uint16 i;
	for(i=0;i<len;i++)
	  {
	  Uart3CharSend(*(StrData+i));
	  }
  
}

/*************************************************************/

void SendText_UART3(uchar *StrData)
{

	uchar i;
	for(i=0;i<255;i++)
	  {
	  if(*(StrData+i)==0x00)break;
	   Uart3CharSend(*(StrData+i));
	  }
}
#endif
//////////////////////////////////////////////////////////////////

#if CONFIG_UART4
void  Uart4BufInit(void)
{  
CPU_SR         cpu_sr;
   OS_ENTER_CRITICAL();	//ucos2
   Uart4Sending = 0;
   ClearLoopBuf(&Uart4SendStruct,UART4_SEND_BUF_SIZE);				//清除环型缓冲区
	ClearLoopBuf(&Uart4RecvStruct,UART4_RECV_BUF_SIZE);
   
	OS_EXIT_CRITICAL();          //ucos2  
}


///////////////////

//void Uart4_RxHandle (uchar chr)
//{
//
//	//  接收  
//
//           		AddLoopBuf(&Uart4RecvStruct,Uart4RecvBuf,UART4_RECV_BUF_SIZE,chr);
//}
////////////////////////////
//uchar Uart4_TxHandle (void) 
//{    		
//	//  发送 
//	unsigned char temp;
//	     		if (0 == Uart4SendStruct.len)
//	     			{
//	     			Uart4Sending = 0;	
//	     			//Clr_En485;//485允许发送结束
//	     			}
//			else{
//	         	
//					 		GetLoopbuf(&Uart4SendStruct,Uart4SendBuf,UART4_SEND_BUF_SIZE,&temp);
//					 		USART_SendData(UART4, temp);
//				}
//
//}
////////////
uchar GetOneFromUart4(uchar *buf)
{
	
	uchar flag;
	CPU_SR         cpu_sr;
	
	OS_ENTER_CRITICAL();	//ucos2   
	
	if(Uart4RecvStruct.len)
		{
		GetLoopbuf(&Uart4RecvStruct,Uart4RecvBuf,UART4_RECV_BUF_SIZE,buf);
		flag= 1;
		}
	else 	flag=  0;
	
	OS_EXIT_CRITICAL();
	
	return flag;	
	
}


uint16 Uart4CharSend(uchar x)
{
	
	uchar temp;
	uint16 fail=0;
	uint32 t;
	CPU_SR         cpu_sr;
	
    if(Uart4SendStruct.len>3)
	    	{//full
	    	t=10;
	    	while((Uart4SendStruct.addTail == Uart4SendStruct.outTail)&&(t>0))
	    		{
	    		OSTimeDly(2);	//延时10毫秒		
	    		t--;
	    		}
	    	}
	    if(t==0)
	    	fail=1;
	    	
	 OS_ENTER_CRITICAL();	//ucos2  
	AddLoopBuf(&Uart4SendStruct,Uart4SendBuf,UART4_SEND_BUF_SIZE,x);	//加入到环型缓冲区
	OS_EXIT_CRITICAL();          //ucos2  	
	/*
	if(Uart4Sending!=0)	//准备继续发送
		{
		if(U4LSR&BIT5)//发送保持寄存器空
	   		{
	   		Uart4Sending = 0;
	   		}
		}
		//20100915
	*/
	if(Uart4Sending==0)
	             {
	              OS_ENTER_CRITICAL();	//ucos2
	              
	              if(GetLoopbuf(&Uart4SendStruct,Uart4SendBuf,UART4_SEND_BUF_SIZE,&temp))
	              	{
	              		Uart4Sending = 1;
	              		SetEn485_4;
										USART_SendData(UART4, temp);
	              		USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
	              	//	USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
	        			}
	        OS_EXIT_CRITICAL();          //ucos2  	
	             }
	return (!fail);
}








/************************************************************************/
/************************************************************************/
/*     uart1中断方式发送字符串，调用方法:				*/
/* 1.SendString_UART4("ATZ\r",sizeof("ATZ\r"));				     */
/* 2.SendString_UART4(&XYZ[0],length);					     */
/************************************************************************/
void SendString_UART4(unsigned char *StrData,uint16 len)
{
uint16 i;
	for(i=0;i<len;i++)
	  {
	  Uart4CharSend(*(StrData+i));
	  }
  
}

/*************************************************************/

void SendText_UART4(uchar *StrData)
{

	uchar i;
	for(i=0;i<255;i++)
	  {
	  if(*(StrData+i)==0x00)break;
	   Uart4CharSend(*(StrData+i));
	  }
}

#endif

#if CONFIG_UART5
void  Uart5BufInit(void)
{  
CPU_SR         cpu_sr;
   OS_ENTER_CRITICAL();	//ucos2
   Uart5Sending = 0;
   ClearLoopBuf(&Uart5SendStruct,UART5_SEND_BUF_SIZE);				//清除环型缓冲区
	ClearLoopBuf(&Uart5RecvStruct,UART5_RECV_BUF_SIZE);
   
	OS_EXIT_CRITICAL();          //ucos2  
}


///////////////////

//void Uart5_RxHandle (uchar chr)
//{
//
//	//  接收  
//
//           		AddLoopBuf(&Uart5RecvStruct,Uart5RecvBuf,UART5_RECV_BUF_SIZE,chr);
//}
////////////////////////////
//uchar Uart5_TxHandle (void) 
//{    		
//	//  发送 
//	unsigned char temp;
//	     		if (0 == Uart5SendStruct.len)
//	     			{
//	     			Uart5Sending = 0;	
//	     			//Clr_En485;//485允许发送结束
//	     			}
//			else{
//	         	
//					 		GetLoopbuf(&Uart5SendStruct,Uart5SendBuf,UART5_SEND_BUF_SIZE,&temp);
//					 		USART_SendData(UART5, temp);
//				}
//
//}
////////////
uchar GetOneFromUart5(uchar *buf)
{
	
	uchar flag;
	CPU_SR         cpu_sr;
	
	OS_ENTER_CRITICAL();	//ucos2   
	
	if(Uart5RecvStruct.len)
		{
		GetLoopbuf(&Uart5RecvStruct,Uart5RecvBuf,UART5_RECV_BUF_SIZE,buf);
		flag= 1;
		}
	else 	flag=  0;
	
	OS_EXIT_CRITICAL();
	
	return flag;	
	
}


uint16 Uart5CharSend(uchar x)
{
	
	uchar temp;
	uint16 fail=0;
	uint32 t;
	CPU_SR         cpu_sr;
	
    if(Uart5SendStruct.len>3)
	    	{//full
	    	t=10;
	    	while((Uart5SendStruct.addTail == Uart5SendStruct.outTail)&&(t>0))
	    		{
	    		OSTimeDly(2);	//延时10毫秒		
	    		t--;
	    		}
	    	}
	    if(t==0)
	    	fail=1;
	    	
	 OS_ENTER_CRITICAL();	//ucos2  
	AddLoopBuf(&Uart5SendStruct,Uart5SendBuf,UART5_SEND_BUF_SIZE,x);	//加入到环型缓冲区
	OS_EXIT_CRITICAL();          //ucos2  	
	/*
	if(Uart5Sending!=0)	//准备继续发送
		{
		if(U4LSR&BIT5)//发送保持寄存器空
	   		{
	   		Uart5Sending = 0;
	   		}
		}
		//20100915
	*/
	if(Uart5Sending==0)
	             {
	              OS_ENTER_CRITICAL();	//ucos2
	              
	              if(GetLoopbuf(&Uart5SendStruct,Uart5SendBuf,UART5_SEND_BUF_SIZE,&temp))
	              	{
	              		Uart5Sending = 1;
	              		SetEn485_5;
										USART_SendData(UART5, temp);
	              		USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
	              	//	USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
	        			}
	        OS_EXIT_CRITICAL();          //ucos2  	
	             }
	return (!fail);
}








/************************************************************************/
/************************************************************************/
/*     uart1中断方式发送字符串，调用方法:				*/
/* 1.SendString_UART5("ATZ\r",sizeof("ATZ\r"));				     */
/* 2.SendString_UART5(&XYZ[0],length);					     */
/************************************************************************/
void SendString_UART5(unsigned char *StrData,uint16 len)
{
uint16 i;
	for(i=0;i<len;i++)
	  {
	  Uart5CharSend(*(StrData+i));
	  }
  
}

/*************************************************************/

void SendText_UART5(uchar *StrData)
{

	uchar i;
	for(i=0;i<255;i++)
	  {
	  if(*(StrData+i)==0x00)break;
	   Uart5CharSend(*(StrData+i));
	  }
}

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/

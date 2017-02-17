//Test.c
#include "includes.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define USERCharSend			Uart1CharSend
#define SendText_USER 		SendText_UART1
#define OSSemUartUSER			OSSemUart1
extern OS_EVENT *OSSemUartUSER;


void SendTestBufHex(uchar *p,uint16 size)	//发送一段缓冲区内容信息
{
INT8U err;
uint16 i;
if(1)	//测试方式	
	{
	if(OSRunning)
			{
	#if CONFIG_UART1
			OSSemPend(OSSemUartUSER,0,&err);
			for(i=0;i<size;i++)
				{

			USERCharSend(HexToAsc(*(p+i)>>4));
			USERCharSend(HexToAsc(*(p 
			+i)));
			USERCharSend(0x20);

		
				}
				
SendText_USER("\r\n");

			OSSemPost(OSSemUartUSER);
	#endif			
			}
	}
}



void SendTestResoult(uchar ok,uchar *p1,uchar *p2)	//发送判断测试信息
{
INT8U err;
if(1)	//测试方式	
	{
	if(OSRunning)
			{
				#if CONFIG_UART1
			OSSemPend(OSSemUartUSER,0,&err);
			if(ok)SendText_USER(p1);
			else
				SendText_USER(p2);
			SendText_USER("\r\n");
			OSSemPost(OSSemUartUSER);	
			#endif
			}
	}
}	


void SendTestGroup5(uchar *p1,uchar *p2,uchar *p3,uchar *p4,uchar *p5)	//发送测试信息组
{
INT8U err;
if(1)	//测试方式	
	{
	if(OSRunning)
			{
				#if CONFIG_UART1
			OSSemPend(OSSemUartUSER,0,&err);
			SendText_USER(p1);
			SendText_USER(p2);	
			SendText_USER(p3);
			SendText_USER(p4);	
			SendText_USER(p5);
			//SendText_USER("\r\n");
			OSSemPost(OSSemUartUSER);	
			#endif
			}
	}
}
////
void SendTestStatu(uchar *p)	//发送测试信息不带参数
{
INT8U err;
if(1)	//测试方式
 	{
 	if(OSRunning)
			{
				#if CONFIG_UART1
			OSSemPend(OSSemUartUSER,0,&err);
		 	SendText_USER(p);
		 	SendText_USER("\r\n");
		 	OSSemPost(OSSemUartUSER);
		 	#endif
			}
	}
 }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendTestStatuAsc16(uchar *p,uint16 x)	//发送测试信息带16位参数
{
INT8U err;
uchar out[12];
if(1)	//测试方式
 	{
 	if(OSRunning)
			{
				#if CONFIG_UART1
			OSSemPend(OSSemUartUSER,0,&err);
		 	SendText_USER(p);
		 	MakeValAsc16("",x,"\r\n",out);
		 	SendText_USER(out);
		 	OSSemPost(OSSemUartUSER);
		 	#endif
			}
	}
 	
 }

 ///////////////////////////////
void SendTestStatuAsc32(uchar *p,uint32 x)	//发送测试信息带32位参数
{
INT8U err;
uchar out[24];
if(1)	//测试方式
 	{
 	if(OSRunning)
			{
				#if CONFIG_UART1
			OSSemPend(OSSemUartUSER,0,&err);
		 	SendText_USER(p);
		 	MakeValAsc32("",x,"\r\n",out);
		 	SendText_USER(out);
		 	OSSemPost(OSSemUartUSER);
		 	#endif
			}
	}
 	
 }

 ///////////////////////////////
void SendTestStatuHEX16(uchar *p,uint16 x)	//发送测试信息带16位参数
{
INT8U err;
uchar out[12];	
if(1)	//测试方式
 	{
	 if(OSRunning)
		{
			#if CONFIG_UART1
		OSSemPend(OSSemUartUSER,0,&err);
		SendText_USER(p);
		MakeValHex16("",x,"\r\n",out);	//8位变量转换为HEX的ASC码，有前后缀
	 	SendText_USER(out);
		OSSemPost(OSSemUartUSER);
		#endif
		}
	}
 	
 }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////













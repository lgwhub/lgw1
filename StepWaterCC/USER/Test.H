//  #include "Test.h" 
#ifndef __TEST_H
#define __TEST_H


//检查堆栈
#define FLAG_TEST_STK_SIZE 		(0)


/////////////////////////////////////////////////////////////////////////////////////////////
//定义测试项
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
void SendTestBufHex(uchar *p,uint16 size);	//发送一段缓冲区内容信息
void SendTestResoult(uchar ok,uchar *p1,uchar *p2);	//发送判断测试信息
void SendTestGroup5(uchar *p1,uchar *p2,uchar *p3,uchar *p4,uchar *p5);	//发送测试信息组
void SendTestStatu(uchar *p);	//发送测试信息不带参数
void SendTestStatuAsc16(uchar *p,uint16 x);	//发送测试信息带16位参数
void SendTestStatuHEX16(uchar *p,uint16 x);	//发送测试信息带16位参数
void SendTestStatuAsc32(uchar *p,uint32 x);	//发送测试信息带32位参数
#endif

//#include "config.h"

#ifndef _MEGA_CONFIG_H
#define _MEGA_CONFIG_H





/*
mega16 熔丝位
熔丝位高字节
熔丝高字节		位号			描述																默认值
OCDEN(4) 			7 				使能OCD 														1 ( 未编程， OCD 禁用)
JTAGEN(5) 		6 				使能 JTAG 													0 ( 编程， JTAG 使能)
SPIEN(1) 			5 				使能串行程序和数据下载							0 ( 被编程， SPI 编程使能)
CKOPT(2) 			4 				振荡器选项													1 ( 未编程)
EESAVE 				3 				执行芯片擦除时EEPROM 的内容保留			1 ( 未被编程)，EEPROM 内容不保留
BOOTSZ1 			2 				选择 Boot 区大小										0 ( 被编程)(3)
BOOTSZ0 			1 				选择 Boot 区大小										0 ( 被编程)(3)
BOOTRST 			0 				选择复位向量												1 ( 未被编程) 

Notes: 
1. 在SPI 串行编程模式下SPIEN 熔丝位不可访问。
2. CKOPT 熔丝位功能由CKSEL 位设置决定，详见 P23 “ 时钟源” 。
3. BOOTSZ1..0 默认值为最大Boot 大小，详见 P244Table 100。
4. 不论锁位与JTAGEN熔丝位设置为什么，产品出厂时不对OCDEN编程。对OCDEN熔
丝位编程后会使能系统时钟的某些部分在所有的休眠模式下运行。这会增加功耗。
5. 如果没有连接JTAG 接口，应尽可能取消JTAGEN 熔丝位的编程状态，以消除存在于
JTAG 接口之TDO 引脚的静态电流。
*/


/*
熔丝位低位字节
熔丝位低位字节		位号		描述								默认值
BODLEVEL 					7 			BOD 触发电平				1 ( 未被编程)
BODEN 						6 			BOD 使能						1 ( 未被编程， BOD 禁用)
SUT1 							5 			选择启动时间				1 ( 未被编程)(1)
SUT0 							4				选择启动时间				0 ( 被编程)(1)
CKSEL3 						3 			选择时钟源					0 ( 被编程)(2)
CKSEL2 						2 			选择时钟源					0 ( 被编程)(2)
CKSEL1 						1 			选择时钟源					0 ( 被编程)(2)
CKSEL0 						0 			选择时钟源					1 ( 未被编程)(2)

Notes: 
1. 对于默认时钟源，SUT1..0 的默认值给出最大的起动时间。
2. CKSEL3..0的默认设置导致了片内RC振荡器运行于1 MHz。
熔丝位的状态不受芯片擦除命令的影响。如果锁定位1(LB1) 被编程则熔丝位被锁定。在
编程锁定位前先编程熔丝位。
*/

#define MEGA16H_OCDEN			BIT7
#define MEGA16H_JTAGEN		BIT6
#define MEGA16H_SPIEN			BIT5
#define MEGA16H_CKOPT			BIT4
#define MEGA16H_EESAVE		BIT3
#define MEGA16H_BOOTSZ1		BIT2
#define MEGA16H_BOOTSZ0		BIT1
#define MEGA16H_BOOTRST		BIT0

#define MEGA16L_BODLEVEL	BIT7
#define MEGA16L_BODEN			BIT6
#define MEGA16L_SUT1			BIT5
#define MEGA16L_SUT0			BIT4
#define MEGA16L_CKSEL3		BIT3
#define MEGA16L_CKSEL2		BIT2
#define MEGA16L_CKSEL1		BIT1
#define MEGA16L_CKSEL0		BIT0


//#define MEGA16L_NO				(	MEGA16L_BODLEVEL 	|	\
//														MEGA16L_BODEN 		|	\		
//														MEGA16L_SUT1 			|	\	
//														MEGA16L_SUT0			|	\
//														MEGA16L_CKSEL3		|	\
//														MEGA16L_CKSEL2		|	\
//														MEGA16L_CKSEL1		|	\
//														MEGA16L_CKSEL0		\
//													)
//#define MEGA16H_NO		{
//													MEGA16H_OCDEN		|	\
//													MEGA16H_JTAGEN	|	\
//													MEGA16H_SPIEN		|	\
//													MEGA16H_CKOPT		|	\
//													MEGA16H_EESAVE	|	\
//													MEGA16H_BOOTSZ1	|	\
//													MEGA16H_BOOTSZ0	|	\
//													MEGA16H_BOOTRST	|	\
//													}

#define MEGA16L_DEFAULT		(	MEGA16L_BODLEVEL 	|	MEGA16L_BODEN		|	MEGA16L_SUT1	| MEGA16L_CKSEL0	)
												
													
#define MEGA16H_DEFAULT		(	MEGA16H_OCDEN		|	MEGA16H_CKOPT	|	MEGA16H_EESAVE	|	MEGA16H_BOOTRST		)

											
#define MEGA16L_RCEV12M		(	MEGA16L_BODLEVEL 	|	MEGA16L_BODEN |	MEGA16L_SUT1	|	MEGA16L_CKSEL1	|	MEGA16L_CKSEL0	)													
#define MEGA16H_RCEV12M		(	MEGA16H_OCDEN	|	MEGA16H_JTAGEN	|	MEGA16H_CKOPT	|	MEGA16H_BOOTRST		)

//外部 4MHZ		
#define MEGA16L_RCEV13		(	MEGA16L_BODLEVEL 	|	MEGA16L_BODEN |	MEGA16L_SUT1 	|	MEGA16L_SUT0	|	MEGA16L_CKSEL3	|	MEGA16L_CKSEL2	|	MEGA16L_CKSEL1	|	MEGA16L_CKSEL0	)													
#define MEGA16H_RCEV13		(	MEGA16H_OCDEN		|	MEGA16H_JTAGEN	|	MEGA16H_CKOPT		|	MEGA16H_BOOTRST	)

//外部 4MHZ		
#define MEGA16L_RCEV14		(	MEGA16L_BODLEVEL 	|	MEGA16L_BODEN |	MEGA16L_SUT1 	|	MEGA16L_SUT0	|	MEGA16L_CKSEL3	|		MEGA16L_CKSEL2	|	MEGA16L_CKSEL0	)													
#define MEGA16H_RCEV14		(	MEGA16H_OCDEN		|	MEGA16H_JTAGEN	|	MEGA16H_BOOTRST	)

////////////////////////////////


/////////////////////////////////////////////////////////////////
/*
mega48
熔丝高字节		位号			描述																默认值
RSTDISBL(1) 	7 				外部复位禁用										1 ( 未被编程)
DWEN 					6 				调试线使能											1 ( 未被编程)
SPIEN(2) 			5 				使能串行程序和数据下载					0 ( 被编程， SPI 编程使能)
WDTON(3) 			4 				看门狗定时器一直启用						1 ( 未被编程)
EESAVE 				3 				执行芯片擦除时EEPROM的内容保留	1 ( 未被编程)， EEPROM内容不保留
BODLEVEL2(4) 	2 				BOD 触发电平										1 ( 未被编程)
BODLEVEL1(4) 	1 				BOD 触发电平										1 ( 未被编程)
BODLEVEL0(4) 	0 				BOD 触发电平										1 ( 未被编程)

熔丝位低位字节
熔丝位低位字节	位号		描述							默认值
CKDIV8(4) 			7 			时钟8 分频				0 ( 被编程)
CKOUT(3) 				6 			时钟输出					1 ( 未被编程)
SUT1 						5 			选择启动时间			1 ( 未被编程)(1)
SUT0 						4 			选择启动时间			0 ( 被编程)(1)
CKSEL3 					3 			选择时钟源				0 ( 被编程)(2)
CKSEL2 					2 			选择时钟源				0 ( 被编程)(2)
CKSEL1 					1 			选择时钟源				1 ( 未被编程)(2)
CKSEL0 					0 			选择时钟源				0 ( 被编程)(2)


*/

#define MEGA48H_RSTDISBL		BIT7
#define MEGA48H_DWEN				BIT6
#define MEGA48H_SPIEN				BIT5
#define MEGA48H_WDTON				BIT4
#define MEGA48H_EESAVE			BIT3
#define MEGA48H_BODLEVEL2		BIT2
#define MEGA48H_BODLEVEL1		BIT1
#define MEGA48H_BODLEVEL0		BIT0

#define MEGA48L_CKDIV8		BIT7
#define MEGA48L_CKOUT			BIT6
#define MEGA48L_SUT1			BIT5
#define MEGA48L_SUT0			BIT4
#define MEGA48L_CKSEL3		BIT3
#define MEGA48L_CKSEL2		BIT2
#define MEGA48L_CKSEL1		BIT1
#define MEGA48L_CKSEL0		BIT0



#define MEGA48H_DEFAULT		(	MEGA48H_RSTDISBL	|	MEGA48H_DWEN	|	MEGA48H_WDTON	|	MEGA48H_EESAVE	|	MEGA48H_BODLEVEL2	|	MEGA48H_BODLEVEL1	|	MEGA48H_BODLEVEL0	)
#define MEGA48L_DEFAULT		(	MEGA48L_CKOUT			|	MEGA48L_SUT1	|	MEGA48L_CKSEL1	)

//外部 4MHZ		
#define MEGA88H_PUSH5		(	MEGA48H_RSTDISBL	|	MEGA48H_DWEN	|	MEGA48H_WDTON	|	0*MEGA48H_EESAVE	|	MEGA48H_BODLEVEL2	|	0*MEGA48H_BODLEVEL1	|	MEGA48H_BODLEVEL0	)
#define MEGA88L_PUSH5		(	MEGA48L_CKOUT			|	MEGA48L_SUT1	|	MEGA48L_SUT0	|	MEGA48L_CKSEL3 | MEGA48L_CKSEL2	|	0*MEGA48L_CKSEL1	|	MEGA48L_CKSEL0	)

//外部 4MHZ	 监狱定位接收器	
#define MEGA48H_POLICE		(	MEGA48H_RSTDISBL	|	MEGA48H_DWEN	|	MEGA48H_WDTON	|	MEGA48H_EESAVE	|	MEGA48H_BODLEVEL2	|	MEGA48H_BODLEVEL1	|	MEGA48H_BODLEVEL0	)
#define MEGA48L_POLICE		(	MEGA48L_CKOUT			|	MEGA48L_SUT1	|	MEGA48L_SUT0	|	MEGA48L_CKSEL3 | MEGA48L_CKSEL2	|	0*MEGA48L_CKSEL1	|	MEGA48L_CKSEL0	)

//LOCK_BYTE
#define BLB12		BIT5
#define BLB11		BIT4
#define BLB02		BIT3
#define BLB01		BIT2
#define LB2			BIT1
#define LB1			BIT0

#endif



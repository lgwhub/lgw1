/*****************************************************************************
Copyright: Hangzhouzhongan Elec. Co., Ltd.
File name: ADS8328控制程序
Description: 
Author: chaijunbiao
Version: v1.0
Date: 2015-10-29
History: 修改历史记录列表， 每条修改记录应包括修改日期、修改者及修改内容简述。

*****************************************************************************/
//#include "SysTick/systick.h"

//#include "user_inc/hzzadz_Macro_conf.h"

#include <includes.h>   


#define DELAY_US_100    delay_us(100)
#define DELAY_US_2      delay_us(2)
#define DELAY_US_1      ;


void delay_us(unsigned short int temp)  //实际上1.5us
{
	unsigned short int i;
	while(temp>0)
	{
	for(i=0;i<8;i++)
	   {
	   }
	 temp--;
  }
	
}

///////////////////////////////////////////////////////////////////
/////////////////////模数转换(AD)//////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////ADS8328///////////////
//向ADC8328写入一个字，主要是写入命令寄存器
//0x0000 = 选择通道0
//0x1000 = 选择通道1
//0xE000 = 在CFR寄存器中写入参数
void ADS8328_WriteWord(unsigned short int temp)     //写字
{
	 unsigned short int temp1;
	 unsigned int i;
	 ADS8328_CS1(0);//nADS8328_CS1=0
	 for (i=0;i<16;i++)
	  {
		   temp1=temp&0x8000;
		   if(temp1==0)		
		    ADS8328_DIN(0);//MOSI=0
		   else
		    ADS8328_DIN(1);//MOSI=1
		   DELAY_US_1;
		   ADS8328_SCK(1);//SCLK =1  
		   DELAY_US_1;
		   ADS8328_SCK(0);//SCLK=0
		   DELAY_US_1;
		   temp<<=1;
	  }    
	 ADS8328_CS1(1);//nCS=1
	 ADS8328_SCK(0);//SCLK=0
	 ADS8328_DIN(1);//MOSI =1
}
//根据命令号，从ADC8328中读出16位数据，
//0xC000 = 读出CFR
//0xD000 = 读出数据
unsigned short int ADS8328_ReadWord(unsigned short int command)//命令寄存器
{
	 unsigned short int temp,temp1,temp2;
	 unsigned char i;
	 
	 temp=0;	   	   
	 ADS8328_CS1(0);//nADS8328_CS1=0
	 DELAY_US_2;
	 for(i=0; i<16; i++)
	 {
		  temp2 = command&0x8000;
		  if(temp2 == 0)		
		   ADS8328_DIN(0);//MOSI=0
		  else
		   ADS8328_DIN(1);//MOSI=1
		  command<<=1;
		  DELAY_US_1;
		  ADS8328_SCK(1);//SCLK =1  
		  DELAY_US_1;
		
			temp1= ADS8328_DOUT;
		
		  ADS8328_SCK(0);//SCLK=0
		  DELAY_US_1;
		  temp<<=1;
		  if(temp1!=0) 
		   temp|=0x0001;
		  else
		   temp&=0xfffe;       
		  DELAY_US_1;     
	 }	
	 ADS8328_DIN(1);//MOSI=1
	 DELAY_US_1;
	 ADS8328_CS1(1);//nCS=1
	 DELAY_US_2;
	 return temp;
}

//选择ADS8328通道号，1 = 通道0 ，2 = 通道1
void  ADS8328_SelectChannel(unsigned char channel)
{
	if(	channel == 1)
	{
		ADS8328_WriteWord(0x0000);//选择模拟通道0	
	}
	else
	{
		ADS8328_WriteWord(0x1000);//选择模拟通道1		
	}
}

//ADS8328数据转换函数，sample_times= 采样次数，	data_ptr = 采样结果存储的位置，返回数值为本次数据是否有效
unsigned char ADS8328_DataConvert(unsigned char sample_times, unsigned long int *data_ptr)
{
	unsigned short int temp1;
	unsigned short int i;
	unsigned long int temp2;
	unsigned short int sample_delay_count;
	unsigned short int sample_max,sample_min;

	sample_max = 0;
	sample_min = 0xFFFF;
	sample_times += 2;    //因为需要减掉最大最小值，所以采样次数加2
	temp2 = 0;
	 for(i=0; i < sample_times ; i++)
	 {
	  
		ADS8328_CNV(0);//nCONVST=0
		sample_delay_count = 0;  
		do
		{
		 	ADS8328_SCK(0);//SCLK=0
		    DELAY_US_1;
		  	ADS8328_SCK(1);//SCLK =1  
		  	DELAY_US_1;
			temp1 = ADS8328_EOC ;
			sample_delay_count ++;

	    } while((temp1 == 0)&&(sample_delay_count < 10000)); 
		ADS8328_CNV(1);//nCONVST=1
	    DELAY_US_2;
		if(	sample_delay_count < 10000)
		{
			temp1 = ADS8328_ReadWord(0xd000);//读数据
			if( temp1 > sample_max )
			{
				sample_max = temp1;	
			}

			if( temp1 < sample_min )
			{
			 	sample_min = temp1;	
			}

			temp2 += temp1;
  	    	//delay_us(60);
		}
		else
		{
			return (0);
		}
		
	  }
	  temp2 -=  sample_max;
	  temp2 -= 	sample_min;	 //减掉最大最小值

	  *data_ptr =  temp2 ;
	  return (1);
	  
	  	
}

//
void ADS8328_Init(void)			//ADS8328初始化
{
	unsigned short int temp1;

 #define CFR_11  (0 << 11 )    //Channel select mode, 0 =`Manual, 1 = Auto;
 #define CFR_10	 (1 << 10 )	  //Conversion clock (CCLK) source select,	1: Conversion clock (CCLK) = Internal OSC,0=SCK/2
 #define CFR_9	 (1 << 9 )	  //Trigger (conversion start) select, 0=Auto trigger，1 =Manual trigger
 #define CFR_8	 (0 << 8 )	  //Don't care
 #define CFR_7	 (1 << 7 )	  //Pin 10 polarity select when used as an output (EOC/INT)，0= high，1= low
 #define CFR_6	 (1 << 6 )	  //Pin 10 function select when used as an output (EOC/INT)，0 =INT，1=EOC
 #define CFR_5	 (1 << 5 )	  //Pin 10 I/O select for chain mode operation ，0 =CDI，1=EOC/INT output
 #define CFR_4	 (1 << 4 )	  //Auto nap power-down enable/disable ,0= enable,1= disable
 #define CFR_3	 (1 << 3 )	  //Nap power-down,0=Enable, 1= disable
 #define CFR_2	 (1 << 2 )	  //Deep power-down.0=Enable, 1= disable
 #define CFR_1	 (0 << 1 )	  //TAG bit enable.0: TAG bit disabled; 1: TAG bit output enabled
 #define CFR_0	 (0 << 0 )	  //Reset,0 = Reset ,1 =Normal operation  

  temp1 =  CFR_0 + CFR_1 + CFR_2 + CFR_3 +
  		   CFR_4 + CFR_5 + CFR_6 + CFR_7 +
		   CFR_8 + CFR_9 + CFR_10+ CFR_11 ;

 temp1 += 0xE000;	   		//命令寄存器位置数值		   	
 DELAY_US_100;
 ADS8328_WriteWord(temp1);	//系统复位
 DELAY_US_100;
 ADS8328_WriteWord(temp1);	//系统复位
 DELAY_US_100;
 ADS8328_WriteWord(temp1 + 1);//写CFR寄存器0xe6fd
 DELAY_US_100;
 ADS8328_WriteWord(temp1 + 1);//写CFR寄存器0xe6fd
 //手动通道选择使能，内部时钟，nCONVST开始转换，NC
 //P10=EOC,EOC=low,P10=EOC输出,1  
}


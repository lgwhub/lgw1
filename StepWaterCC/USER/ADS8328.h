/*****************************************************************************
Copyright: Hangzhouzhongan Elec. Co., Ltd.
File name: ADS8328控制程序
Description: 
Author: chaijunbiao
Version: v1.0
Date: 2015-10-29
History: 修改历史记录列表， 每条修改记录应包括修改日期、修改者及修改内容简述。


*****************************************************************************/
#ifndef __ADS8328_H
#define __ADS8328_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
/* Private define ------------------------------------------------------------*/

void delay_us(unsigned short int temp);

/* Private function prototypes -----------------------------------------------*/
void ADS8328_Init(void);			//ADS8328初始化
unsigned char ADS8328_DataConvert(unsigned char  sample_times, unsigned long int *data_ptr); //ADS8328数据转换函数，sample_times= 采样次数，	data_ptr = 采样结果存储的位置，返回数值为本次数据是否有效
void  ADS8328_SelectChannel(unsigned char  channel);				//选择ADS8328通道号，1 = 通道1 ，2 = 通道2
unsigned short int ADS8328_ReadWord(unsigned short int command);						//根据命令号，从ADC8328中读出16位数据，
void ADS8328_WriteWord(unsigned short int temp);						//向ADC8328写入一个字，主要是写入命令寄存器

#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

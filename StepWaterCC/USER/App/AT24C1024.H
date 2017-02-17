/* 24c256 seeprom */

/*   sla 从地址0XA0  */
/*  适合24c32 24c64  24c128 24c256 24c512 */
//sbit SDA=P1^7;  /**????***/
//sbit SCL=P1^6;  /**????***/

#ifndef __AT24C1024_H__
#define __AT24C1024_H__



#define EEPROM_BIG  1
 
#define AT24C16_ADR					0X04

void SaveEE(uint16 ee_adr,uchar x);
uchar ReadByteEE(uint16 start_adr);

/*
void PollDownPortI2C(void);

bool i2SendChar(uchar sla,uint16 suba,uchar c);   //有子地址写单字节数据 

#if EEPROM_BIG
bool i2RcvStr(uchar sla,uint16 suba,uchar n,uchar *s);    //有子地址读n字节数据 
#endif
uchar i2RcvChar(uchar sla,uint16 suba);    //有子地址读单字节数据
void ReadEE(uint16 start_adr,uchar len,uchar *buf);
uint16 ExMemWordRead(uint16 start_adr);
void ExMemWordWrite(uint16 ee_adr,uint16 x);	//保存16位整型
*/

#endif


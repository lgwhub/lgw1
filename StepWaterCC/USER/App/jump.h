//jump.h

#ifndef _JUMP_APP_ADR_H
#define _JUMP_APP_ADR_H

//自动加锁定时器
extern unsigned short int TimeAutoLock1;
extern unsigned short int TimeAutoLock2;
extern unsigned short int TimeAutoLock3;

#define    CMND_RESET_21      21
#define    CMND_ISP_22        22
//除modbus外加另外通讯，比如读写存储器
#define    CMND_EXTREN_33     33  
#define    CMND_MEMERY_45      45


void IspMarkLoad(void);
void IspMarkWrite(uint16_t ix);	
void SoftReset(void);  
  
////////////////////////////////////////
void JumpAppAdr(unsigned long int adr);

void ModbusCommand1(void);//收到参数时执行一下

#endif




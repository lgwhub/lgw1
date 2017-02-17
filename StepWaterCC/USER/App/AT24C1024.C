
#include "Nomal.h"
#include "includes.h"

#if 1


/*  save.c  */
/*   sla 从地址  */
/*  EEPROM_BIG 适合24c32 24c64  24c128 24c256 24c512 */

/////#include <Nomal.h>
/////#define Nop nop_()
/////bit ack;
//sbit SDA=P1^7;  /**????***/
//sbit SCL=P1^6;  /**????***/


/*  AT24C01     128byte    page:4byte      1chip     use_adr:none
    AT24C01A    128byte    page:8byte      8chip     use_adr:A2,A1,A0
    AT24C02     256byte    page:8byte      8chip     use_adr:A2,A1,A0
    AT24C04     512byte    page:16byte     4chip     use_adr:A2,A1
    AT24C08     1024byte    page:16byte     2chip     use_adr:A1
    AT24C16     2048byte    page:16byte     1chip     use_adr:none
    AT24C164    2048byte    page:16byte     8chip     use_adr:A2,A1,A0
    AT24C32     4096byte    page:32byte     8chip     use_adr:A2,A1,A0
    AT24C64     8192byte    page:32byte     8chip     use_adr:A2,A1,A0
    AT24C128    16384byte   page:64byte     4chip     use_adr:A1,A0
    AT24C256    32768byte   page:64byte     4chip     use_adr:A1,A0
    AT24C512    65536byte   page:128byte    4chip     use_adr:A1,A0
    AT24C1024   131072byte  page:256byte    2chip     use_adr:A1
*/    
    
/***************/

#define Nop nop_()
bool ack;

//	#define bSDA		(IO0PIN&V_SDA)
//	#define SetbSDDA	IO0SET=V_SDA
//	#define Clr_SDDA	IO0CLR=V_SDA
//	#define SetbSCCL		IO0SET=V_SCL
//	#define Clr_SCCL		IO0CLR=V_SCL

#define DisableOut_SDDA		;
#define DisableOut_SCCL		;
#define EnableOut_SCCL		;
#define EnableOut_SDDA		;


//#define Delay_2Ms		OSTimeDly(OS_TICKS_PER_SEC/500);
//#define Delay_12Ms	OSTimeDly(OS_TICKS_PER_SEC/83);
#define Delay_2Ms		OSTimeDly(OS_TICKS_PER_SEC/500);
#define Delay_12Ms	OSTimeDly(OS_TICKS_PER_SEC/83);


   void nop_(void)
	{
	unsigned short int t;
	for(t=0;t<880;t++)//for(t=0;t<88;t++)
 		{
 		}
	}
//#endif


/***************/
void Start(void)
{

Clr_SCCL;  //准备
EnableOut_SCCL;
EnableOut_SDDA;
/////////////
nop_();
SetbSDDA;
nop_();
SetbSCCL;
nop_();
Clr_SDDA;  //下降沿
nop_();
Clr_SCCL;
nop_();
}
/***************/
void Stop(void)
{
EnableOut_SDDA;
	/////////////////
nop_();
nop_();
Clr_SDDA;
nop_();
SetbSCCL;
nop_();
SetbSDDA;  //上升沿
nop_();
}

void PollDownPortI2C(void)
{
Clr_SCCL;
nop_();	
Clr_SDDA;	
nop_();
//////////////
DisableOut_SDDA;
DisableOut_SCCL;
}

/***************/

void SendByte(uchar c)   //发送1个字符
{
	uchar i;
	EnableOut_SDDA;
	for(i=0;i<8;i++)
	   {
	    if(c&0x80)
	      SetbSDDA;
	    else Clr_SDDA;
	    c+=c;
	    nop_();
	    SetbSCCL;
	    nop_();
	    Clr_SCCL;
	    nop_();
	   };
	
	   nop_();
	   DisableOut_SDDA;
	   SetbSDDA;  /*  准备接收应答位   */
	   nop_();
	   SetbSCCL;
	   nop_();
	   if(bSDA){ack=FALSE;}else{ack=TRUE;};
	   nop_();
	   Clr_SCCL;
	   nop_();
}

/***************/
uchar RcvByte(void)   //接收1个字符
{
		uchar temp,i;
	 nop_();
	 temp=0;
	 Clr_SCCL;
	 nop_();
	 DisableOut_SDDA;
	 SetbSDDA;
	 for(i=0;i<8;i++)
	    {
	     nop_();
	     SetbSCCL;
	     nop_();
	     temp<<=1;
	     if(bSDA)temp++;
	     nop_();
	     nop_();
	     Clr_SCCL;
	    };
	nop_();
	return temp;
}
/***************/
void SendAck(bool a)   //a=1,表示应答，输出低电平
{
	EnableOut_SDDA;
		if(a){Clr_SDDA;}else{SetbSDDA;};  /* 同其它程序相反  */
		nop_();
		SetbSCCL;
		nop_();
		Clr_SCCL;
		nop_();
}

/***************/

bool i2SendChar(uchar sla,uint16 suba,uchar c)   /*   有子地址写单字节数据   */
{
		bool btemp;
		btemp=TRUE;
		Start();
		SendByte(sla);    /*   sla 写从地址  */
		
		#if EEPROM_BIG
		if(!ack)btemp=FALSE;
		SendByte(suba/0x100);    /*   子地址  */
		#endif
		
		if(!ack)btemp=FALSE;
		SendByte(suba%0x100);    /*   子地址  */
		if(!ack)btemp=FALSE;
		SendByte(c);  /*  数据  */
		if(!ack)btemp=FALSE;
		Stop();
		return btemp;
}
//////////////////////////////////////////////////
bool i2SendChar_NO_sub(uchar sla,uchar c)   /*   无子地址写单字节数据   */
{
		bool btemp;
		btemp=TRUE;
		Start();
		SendByte(sla);    /*   sla 写从地址  */
		
		if(!ack)btemp=FALSE;
		
		SendByte(c);  /*  数据  */
		if(!ack)btemp=FALSE;
		Stop();
		return btemp;
}
///////////////////////////////
bool i2SendChar_one_sub(uchar sla,uint16 suba,uchar c)   /*   有子地址写单字节数据   */
{
		bool btemp;
		btemp=TRUE;
		Start();
		SendByte(sla);    /*   sla 写从地址  */
		/*
		#if EEPROM_BIG
		if(!ack)btemp=FALSE;
		SendByte(suba/0x100);    //子地址
		#endif
		*/
		if(!ack)btemp=FALSE;
		SendByte(suba%0x100);    /*   子地址  */
		if(!ack)btemp=FALSE;
		SendByte(c);  /*  数据  */
		if(!ack)btemp=FALSE;
		Stop();
		return btemp;
}
//////////////////////////////////////////////////
#if EEPROM_BIG
bool i2Send_Pol(uchar sla)   /*   应答检测   */
{uchar i;
		for(i=0;i<100;i++)
		   {
		   	Start();
		    SendByte(sla);    /* 0xA1  sla 写从地址  */
		    if(ack)
		       {
		       	Stop();
		        i=255;
		        return TRUE;
		       }
		   Delay_2Ms;    
		       
		   }        
		Stop();
		return FALSE;
}
#endif
/***************/
/*   有子地址写n字节数据   */
bool i2SendStr(uchar sla,uchar n,uint16 suba,uchar *s)
{
		uchar i;
		Start();
		SendByte(sla);
		#if EEPROM_BIG
		
		if(!ack)return(FALSE);
		SendByte(suba/0x100);
		
		#endif
		if(!ack)return(FALSE);
		SendByte(suba%0x100);
		if(!ack)return(FALSE);
		
		for(i=0;i<n;i++)
		    {SendByte(*s);
		     if(!ack)return(FALSE);
		     s++;
		     
		     };
		Stop();
		return(TRUE);
}

/***************/

/***************/

/*   有子地址读n字节数据   */
bool i2RcvStr(uchar sla,uint16 suba,uchar n,uchar *s)
{
	uchar i;
	Start();
	SendByte(sla);
	
	#if EEPROM_BIG
	if(!ack)return(FALSE);
	SendByte(suba/0x100);
	#endif
	
	if(!ack)return(FALSE);
	SendByte(suba%0x100);
	Start();
	SendByte(sla+1);
	if(!ack)return(FALSE);
	if(n>1)
	   {for(i=0;i<n-1;i++)
	      {*s=RcvByte();
	       s++;
	       SendAck(TRUE);
	      };
	    
	   }   
	*s=RcvByte();
	SendAck(FALSE);
	Stop();
	return(TRUE);
}


/***************/
uchar i2RcvChar(uchar sla,uint16 suba)    /*   有子地址读单字节数据   */
{
		uchar temp;
		bool btemp;
		btemp=TRUE;
		Start();
		SendByte(sla);    /*   sla 从地址  */
		
		#if EEPROM_BIG
		if(!ack)btemp=FALSE;
		SendByte(suba/0x100);    /*   子地址  */
		#endif
		
		if(!ack)btemp=FALSE;
		SendByte(suba%0x100);    /*   子地址  */
		if(!ack)btemp=FALSE;
		Start();
		SendByte(sla+1);    /*   sla读 从地址  */
		if(!ack)btemp=FALSE;
		temp=RcvByte();  /*  读数据  */
		SendAck(FALSE);
		Stop();
		return temp;
}
/***************/

#if EEPROM_BIG
uchar GetSlaAdr(ulong start_adr)   //get slave address
{
uchar sla;	
#if AT24C256    	
    sla=((((uint16)(start_adr/0x100))<<1)/0x100)%0x100;
    sla<<=1;	//器件地址  24C256    1 0 1 0 0 A16 A15 R/W     * A14 A13 A12 A11 A10 A9 A8   
#else    
	//24C512
    sla=((start_adr/0x10000)%0x100)<<1;
#endif
return sla;  
}
#endif
/************/
/************/








/*  适合24c32 24c64  24c128 24c256 24c512 */
#if EEPROM_BIG
void SaveEE(uint16 ee_adr,uchar x)
{
i2SendChar(GetSlaAdr(ee_adr)+0xA0,ee_adr,x);
    
Delay_12Ms;

}

bool SavePageEE(ulong start_adr,uchar *ch,uchar len)
{
	 i2SendStr(GetSlaAdr(start_adr)+0xA0,len,start_adr%0x10000,ch);
 
Delay_12Ms;
Delay_12Ms;
Delay_12Ms;

 	i2Send_Pol(GetSlaAdr(start_adr)+0xA0);   /*   应答检测   */
return TRUE; 
}


uchar ReadByteEE(uint16 start_adr)
{uchar temp;
temp=i2RcvChar(GetSlaAdr(start_adr)+0xA0,start_adr);    /*   有子地址读单字节数据   */	
return temp;
}


void ReadEE(uint16 start_adr,uchar len,uchar *buf)
{i2RcvStr(GetSlaAdr(start_adr)+0xA0,start_adr,len,buf);  //连续读
    
}


#else
/* 24c04 seeprom */
void SaveEE(uint16 ee_adr,uchar x)
{
uchar sla2404;
uchar suba2404;

sla2404=((((uchar)(ee_adr/0x100))<<1)&0x0E)+0xA0;
suba2404=(uchar)(ee_adr%0x100);
//i2SendChar(sla2404,suba2404,x);
i2SendChar_one_sub(sla2404,suba2404,x);
Delay_12Ms;

}


/************************************************************************/
uchar ReadByteEE(uint16 start_adr)
{uchar temp;
	uchar sla2404;
	uchar suba2404;
	sla2404=(((uchar)((start_adr)/0x100)<<1)&0x0E)+0xA0;
	suba2404=(uchar)((start_adr)%0x100);
	temp=i2RcvChar(sla2404,suba2404);
	
	return temp;
}

void ReadEE(uint16 start_adr,uchar len,uchar *buf)
{
	uchar sla2404;
	uchar suba2404;
	uchar i;
	for(i=0;i<len;i++)
	   {sla2404=(((uchar)((start_adr+i)/0x100)<<1)&0x0E)+0xA0;
	    suba2404=(uchar)((start_adr+i)%0x100);
	    *(buf+i)=i2RcvChar(sla2404,suba2404);
	    
	   };
}
#endif





uint16 ExMemWordRead(uint16 start_adr)
{uchar temp1,temp2;
temp1=ReadByteEE(start_adr);
temp2=ReadByteEE(start_adr+1);

	return (uint16)temp1*0x100+temp2;
}
///////////////////////////////////////////////////////////

void ExMemWordWrite(uint16 ee_adr,uint16 x)	//保存16位整型
{
SaveEE(ee_adr,x>>8);
SaveEE(ee_adr+1,x&0xff);
}
///////////////////////////////////////////////////////////

void ClrEeprom(uint16 startadr,uint16 len)
{uint16 i;
	for(i=0;i<len;i++)
		{
		SaveEE(startadr+i,0x00);
			
		}
}

///////////////////////////////////////////////////////////
#endif


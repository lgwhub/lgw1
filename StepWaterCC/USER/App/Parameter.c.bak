#include "includes.h"


//#define Max_MemBuf  400
unsigned char cMemBufA[Max_MemBuf+2];
unsigned char cMemBufB[Max_MemBuf+2];

unsigned short int TimeForSaveParam;

_param_water8  Coldw;

/*
uchar FlagRuning[SUB_NMB_CHNALL];
uchar FlagVoltInEnough[SUB_NMB_CHNALL];
uchar WaitRunTime[SUB_NMB_CHNALL];


 _forget_para_total     gData[SUB_NMB_CHNALL];
 _memory_para_total     gMemory[SUB_NMB_CHNALL];                          
 _public_para_total     gPublic; 
 */
 
//系统参数

//unsigned char InitMemoFlag;		//参数已经初始化标记
//unsigned short int  MemoChecksum;    //校验码
//存储器参数
unsigned short int Stm32IdSum6;   


//modbus通讯缓冲区

//float ModBusParaBuf[32];   //modbus 参数 拷贝缓冲区
//unsigned char  ModBusParaByte[32*4];   //modbus 参数 拷贝缓冲区

//float ModByte
////////////////////////////////////////////////////////////////////////
//void SwapFourByte(unsigned char *in,unsigned char *out)  //交换4字节次序
//{
//  #if MODBUS_FLOAT_HIGHBYTE_FORWORD
//  //浮点型和整型数据小端模式，低位在前，而modbus是高位在前
//  unsigned char temp;	
//  //temp     =    *out;
//  *out     =    *(in+3);
//  //*(in+3)  =    temp;
//			         
//  //temp     =   *(out+1)
//  *(out+1) =   *(in+2);   
//  //*(in+2)  =   temp;
//			       
// // temp     =   *(out+2);
//  *(out+2) =   *(in+1); 
//  //*(in+1)  =   temp;
//  
// // temp     =   *(out+3)			    
//  *(out+3) =   *in;   
//  //*in      =    temp;   
//
//   #else
//			        
//   *out=*in; 
//   *(out+1)=*(in+1);   
//   *(out+2)=*(in+2); 
//   *(out+3)=*(in+3); 
//   #endif
//
//}
//////////////////////////////////////////////////////////////////////////
//void MovBlock_4byte(unsigned char *sou,unsigned char *dest,unsigned short int block_len)
//{
//	//len/=4;
//	
//	while(block_len>0)
//     {
//     SwapFourByte(sou,dest);
//     dest+=4;
//     sou+=4;
//     block_len--;
//     }
//	
//}
////////////////////////////////////////////////////////////////////////
//void _Copy_float(float *sou,float *dest,unsigned char len)
//{
//	while(len>0)
//     {
//     *dest=*sou;
//     dest++;
//     sou++;
//     len--;
//     }
//	
//}

///////////////////////////////////////////////////////////////////////////
//void CopyToModbusBuf_Moni(unsigned char channl)   //调试监视用的参数   虚拟寄存器地址  PARAM_ADR_MONI       0X400 
//{
//
//      MovBlock_4byte( (unsigned char *)&gData[channl].PidBufnI.SetPoint , &ModBusParaByte[0] , 8);     //.PidBufnV //一般高速电流PID临时变量缓冲器 
//      MovBlock_4byte( (unsigned char *)&gData[channl].PidBufaI.SetPoint , &ModBusParaByte[8*4] , 8);	//PidBufStruct;  //临时变量
//
//      MovBlock_4byte( (unsigned char *)&gData[channl].simpleI , &ModBusParaByte[16*4] , 1);		//采样电流  //快速采样值，不精确
//      MovBlock_4byte( (unsigned char *)&gData[channl].simpleV , &ModBusParaByte[17*4] , 1);		//采样电压
//      MovBlock_4byte( (unsigned char *)&gData[channl].simpleP , &ModBusParaByte[18*4] , 1);		//采样功率	
//      MovBlock_4byte( (unsigned char *)&gData[channl].simpleR , &ModBusParaByte[19*4] , 1);		//采样模拟电阻	
//      
//
//}
//
//
//
//void CopyFormModbusBuf_Read(unsigned char channl)   //经常读的参数    虚拟寄存器地址 PARAM_ADR_READ       0
//{//最后结果，精确
//
//  MovBlock_4byte( &ModBusParaByte[0]   , (unsigned char *)&gData[channl].ResoultI , 1);		//当前电流
//  MovBlock_4byte( &ModBusParaByte[1*4] , (unsigned char *)&gData[channl].ResoultV , 1);		//当前电压
//  MovBlock_4byte( &ModBusParaByte[2*4] , (unsigned char *)&gData[channl].ResoultP , 1);		//当前功率	
//  MovBlock_4byte( &ModBusParaByte[3*4] , (unsigned char *)&gData[channl].ResoultR , 1);		//当前模拟电阻	
//                                               
//  MovBlock_4byte( &ModBusParaByte[4*4] , (unsigned char *)&gData[channl].HighI , 1);    //4字节   高压电流
//  MovBlock_4byte( &ModBusParaByte[5*4] , (unsigned char *)&gData[channl].HighVolt , 1);    //4字节   高压电压
//   
//}
//////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////

void Default_ParamInit(void)
{

	unsigned char i;
	
                                           //2字节
    Coldw.SoftVer = SOFT_VER;  

    Coldw.CMD          = 0;     //命令寄存器
    Coldw.WATCHING     = 0;     //监视状态
    Coldw.BOARD_STATE  = 1;     //插板状态
    Coldw.device_type  = 2;     //设备型号
    Coldw.Ts1_AMP      = 1.001; //温度采样放大倍数1
    Coldw.Ts1_BIAS     = 0.01;  //温度采样偏移系数1
    Coldw.Ts2_AMP      = 1.002; //温度采样放大倍数2
    Coldw.Ts2_BIAS     = 0.02;  //温度采样偏移系数2
    Coldw.Ts3_AMP      = 1.003; //温度采样放大倍数1
    Coldw.Ts3_BIAS     = 0.03;  //温度采样偏移系数1
    Coldw.Ts4_AMP      = 1.004; //温度采样放大倍数2
    Coldw.Ts4_BIAS     = 0.04;  //温度采样偏移系数2
    Coldw.Ts5_AMP      = 1.005; //温度采样放大倍数1
    Coldw.Ts5_BIAS     = 0.05;  //温度采样偏移系数1
    Coldw.Ts6_AMP      = 1.006; //温度采样放大倍数2
    Coldw.Ts6_BIAS     = 0.06;  //温度采样偏移系数2
    Coldw.Ts7_AMP      = 1.007; //温度采样放大倍数1
    Coldw.Ts7_BIAS     = 0.07;  //温度采样偏移系数1
    Coldw.Ts8_AMP      = 1.008; //温度采样放大倍数2
    Coldw.Ts8_BIAS     = 0.08;  //温度采样偏移系数2







    Coldw.PID_P1 = 3;                //水冷增量PID
    Coldw.PID_I1 = 1;//0.01;
    Coldw.PID_D1 = 4;
    
    Coldw.PID_P2 = 1.01;                //电加热PID
    Coldw.PID_I2 = 1.02;
    Coldw.PID_D2 = 1.03;    
    
    Coldw.MONI_PX1 = 0.11;                //
    Coldw.MONI_IX1 = 0.12;
    Coldw.MONI_DX1 = 0.13;
    Coldw.MONI_QX1 = 0.14;
    
    Coldw.MONI_PX2 = 0.21;                //
    Coldw.MONI_IX2 = 0.22;
    Coldw.MONI_DX2 = 0.23;
    Coldw.MONI_QX2 = 0.24;    
    //float               NC[22];               //

    Coldw.Burnin_type  =  1;          //老化类型
    Coldw.T_set = 37;                //温度设置值
    Coldw.T_bias_set = 0 ;           //温度偏差设置值
    Coldw.T_upper_set = 90;          //
    Coldw.T_down_set = 40;           //

    Coldw.Cycle_times_set = 0;      //
    
    Coldw.TC_sx = 125;                //温度上限设置值
    
        //
    Coldw.TC_duty_set = 0;          //
    Coldw.FAN_duty_set = 0;         //
    Coldw.Moter_step_set = 0;       //强制走几步
    Coldw.Moter_direction = 0;      //方向
    Coldw.one_unit_flag = 0;        //
    Coldw.one_onoff_flag = 0;       //
    
    
    for(i=0;i<8;i++)
    {
	Coldw.unit_onof_flag[i]=1;					   
    Coldw.limit_recode[i] = 0;
    Coldw.Ts[i] = (0.1)*i;                //
    Coldw.TC_duty[i] = 0;           //
    Coldw.FAN_duty[i] = 0 ;          //
    }
     

    
    
    Coldw.Clcle_times = 0;          //
    Coldw.ADC_error_count = 0;      //
    
    Coldw.lBackupRestore = 0;
    Coldw.SubAdr = 247;
    Coldw.SoftVer = 0;     
    Coldw.lSysSwitch_reserve =0;          //默认开机 系统开关
    Coldw.lstats=0;  //统计信息  
    
    
    for(i=0;i<MAX_TEMPRATURE_CHNALL;i++)
    {
     Coldw.Counter_MaxOpen[i]=0;				//温度超过设定值15度以上，不使用PID,强制打开水冷，开n次后暂停，然后等温度到正常范围
     Coldw.Counter_MaxClose[i]=0;				//温度低于设定值15度以上，不使用PID,强制关闭水冷，关n次后暂停，然后等温度到正常范围
    }
 
 
 
 PID_ParaInit();
 //PID_Inc_ParaInit();
 for(i=0;i<MAX_TEMPRATURE_CHNALL;i++)
    {
     PID_BufInit(&HeatPidBuf[i]);//电加热控制
     PID_BufInit(&StepPidBuf[i]);//水冷增量PID
    }
 
 
 
 
 
	#if CONFIG_UART1
	ClearLoopBuf(&Uart1RecvStruct,UART1_RECV_BUF_SIZE);	//清除环型缓冲区
	#endif
	#if CONFIG_UART2
	ClearLoopBuf(&Uart2RecvStruct,UART2_RECV_BUF_SIZE);	//清除环型缓冲区
	#endif
	#if CONFIG_UART3
	ClearLoopBuf(&Uart3RecvStruct,UART3_RECV_BUF_SIZE);	//清除环型缓冲区
	#endif	
	
}

//////////////////////////
unsigned char _GetParamCheck(unsigned char *p,unsigned short int len)
{
unsigned short int i;
unsigned char Sum;	

			
			Sum=0;
			for(i=0;i<len;i++)
				{
				Sum+=*(p+i);//校验码
				}

return Sum;
}
//////////////////////////
void _copy_4byte(unsigned char type,unsigned char *a,unsigned char *b,unsigned char len)
{
	unsigned char i;
	
	if(type == 0)  //  type = 0    a--> b
		{
			for(i=0;i<len;i++)
			  {
			  *(b+i)=*(a+i);
			
			  }
		}
		
else{  //  type = 1    a--> b
	
			for(i=0;i<len;i++)
			  {
			  *(a+i)=*(b+i);
			
			  }	
    }		
	
}

unsigned short int _Param_SwapMemery(unsigned char type,unsigned char *p)  //type = 0   Param --> memery_buf
{ 

	unsigned char  i;
unsigned short int len;	

len=0;
	

		
//	_copy_4byte(type,(unsigned char *)&gPublic.ampHighI             , p , 4);		//高压电流放大倍数    4byte  0x2540                                         
//  p+=4;len+=4;
//  _copy_4byte(type,(unsigned char *)&gPublic.biasHighI            , p , 4);		//高压电流偏移	                                         
//  p+=4;len+=4; 
//  _copy_4byte(type,(unsigned char *)&gPublic.ampHighV             , p , 4);		//高压电压放大倍数                                         
//  p+=4;len+=4;
//  _copy_4byte(type,(unsigned char *)&gPublic.biasHighV            , p , 4);		//高压电压偏移	  
//	p+=4;len+=4;
//  	
//		
//
//  _copy_4byte(type,(unsigned char *)&Stm32IdSum6                   , p , 2);                                                   //2字节
//  p+=2;len+=2;  

return len;	
	
}




//////////////////////////

void InitSaveParam(void)
{
	
}


//////////////////////////
unsigned char Write_Param(void)
{
	
  unsigned short int i;
  unsigned char numb,flag,sucess;	
  unsigned char 	checksum;
  
	sucess = 1;
	

	//check
	if( cMemBufB[Max_MemBuf-2] ==  FlagParamInitnized)
          {
           checksum = _GetParamCheck( cMemBufA , Max_MemBuf-1);  //产生校验码
           cMemBufA[ Max_MemBuf-1] = (~checksum)+1;
          }
  else {
  	    sucess = 0;  //初始化标记不对
  	    return 0;
  	   } 
  	   
  	   
  	         
  for(i=0;i<Max_MemBuf;i++)
    {
    
    flag = 0 ;
    for(numb=0;numb<3;numb++)
    			{	
    				 
    				
           //if(ReadByteEE(AT24C16_ADR+i) != cMemBufB[i])
            if(cMemBufA[i] != cMemBufB[i])	
    	         {
    	         	cMemBufB[i] = cMemBufA[i];
                
                SaveEE(AT24C16_ADR+i, cMemBufB[i]);
                if(ReadByteEE(AT24C16_ADR+i) == cMemBufB[i])
                	{
                		flag = 1; //sucess
            	      break;
                	}
               }
            else{ 
            	   flag = 1; //sucess
            	   break;
            	  }
    			}
    
    if(flag == 0)
    			{
    				sucess = 0; //写入错误
    			}	
    }

return sucess;
}
//////////////////////////

unsigned char Load_Param(void)
{
  unsigned short int i;
  unsigned char numb,flag;


  flag = 1;

  for(numb=0;numb<3;numb++)
      {		
      for(i=0;i<Max_MemBuf;i++)
           {
           cMemBufB[i]=ReadByteEE(AT24C16_ADR+i);  //开始地址
           cMemBufA[i]=cMemBufB[i];
           }	
      //check
      if( cMemBufB[Max_MemBuf-2] ==  FlagParamInitnized)
          {
           flag = _GetParamCheck( cMemBufA , Max_MemBuf);
          }
      
      if(  flag != 0)
          {//
           return 0;
          }
      //copy
      _Param_SwapMemery(1,cMemBufA);   //cMemBufB ---> parameter
      
          
      }
  return 1;
}





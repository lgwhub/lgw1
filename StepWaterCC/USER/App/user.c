//user.c

#include "includes.h"
//extern 




//OS_EVENT *OSSemblinkOut2;
//OS_EVENT *OSSemblinkOut3;
OS_EVENT *OSSemMotors;
OS_EVENT *OSSemUart1;
//OS_EVENT *OSSemUart2;


OS_EVENT *OSSemTimePid_StepMotor;
OS_EVENT *OSSemTimePid_Heat;

uchar LenFromUser;	
uchar BufFromUser[252];

//2400步在8分频时是300步步进电机的极限行程，开阀门
//如果温度太高，超过设定值15度，则不用PID,直接打开240步，最多后退打开冷水次数为10+2次
#define   STEPER_MOTOR_MAX_COUNT_OPEN     10
//如果温度太低，低于设定值15度，则不用PID,直接关闭32步，最多前进关闭冷水次数为75+15次
#define   STEPER_MOTOR_MAX_COUNT_CLOSE    75
//这样就不会一直在极限处动作了

unsigned char lAddressKey;  //拨码开关PB12-PB15，值，最低4位

unsigned char FlagKey;
unsigned char EventTimeBuz=0;
unsigned char EventTimeLed=0;



float wAdcResoult[MAX_TEMPRATURE_CHNALL];  //MAX_ADC_CH 

/////////////////////////////////////////////




//#define MOTOR_STATUS_OFF				0
//#define MOTOR_STATUS_FORWORD		1
//#define MOTOR_STATUS_BACKWORD		2

/////////////////////////////////////////////

//#define MAX_TEMPRATURE_CHNALL   8
signed short int PowerOutBuf[2+(MAX_TEMPRATURE_CHNALL)];
signed short int TestOutBuf[2+(MAX_TEMPRATURE_CHNALL)];




///////////////////////////////////////////////////////////////////////
//电压采样，温度探头为AD590，电阻为10K
//-23度时为250uA，此时电压值为(273-23)*10K = 2.5V
// 0度时为273uA，此时电压值为273uA*10K=2.73V,	 与2.5V差分后对应采样值为(2.73V-2.5V)*65535/2.5= 6029
//27度时为300uA，此时电压值为300uA*10K=3V,	 与2.5V差分后对应采样值为(3V-2.5V)*65535/2.5= 13107
//150度时为 423uA，此时电压值为423uA*10K=4.23V，与2.5V差分后对应采样值为(4.23V-2.5V)*65535/2.5= 45350
//num表示采样次数，temp_TC_bias表示温度偏差值，data_ptr表示数据指针
void CHECK_AD590OneTCCheckUseADS8328(unsigned int num,float data_amp,float data_bias,float temp_TC_bias,float *data_ptr)
{
	  unsigned long int temp_tt;
	  unsigned long int temp1;
	  float temp2;
	  unsigned int  temp3;
	  unsigned char temp4;
	  unsigned long int temp_max,temp_min;

	  temp_tt=0;
	  temp_max=0;
	  temp_min=0xffffff;
	  for(temp3=0;temp3< num+2;temp3++)
	    {
	     temp4=ADS8328_DataConvert(5,&temp1);
		   temp1 /= 5;
	 	 
	 	  if(temp4 == 0)		 //FAIL
		     {
		     	Coldw.ADC_error_count++;
			    return;
		     }
	     temp_tt += temp1;
	     if(temp1>temp_max)
	         {
	          temp_max=temp1;
	         }
	     if(temp1<temp_min)
	         {
	          temp_min=temp1;
	         }
	     }
	     
	  temp_tt-=temp_max;
	  temp_tt-=temp_min;
	  temp_tt/= num;



	  if(temp_tt > 6029)  //有温度探头的,温度大于0度的
	     {
	  	  temp_tt -= 6029;
		    temp2 =( float) temp_tt;
	      temp2 = temp2 / 262;
	      temp2 += temp_TC_bias;
	     }
	  else  //没有温度探头
	    {
	     temp2 = 0;
	     }

	     
	  temp2 *= data_amp;  //默认 1.000
	  temp2 += data_bias; //默认 0.000
	  
	  *data_ptr = temp2;
}

///////////////////////////////////////////////////////////////////////

//8选1选择通道，A,B,C
void CHECK_SelectChannel(unsigned char channel)
{
	
	//ABC  ...  PC5 PB0  PB1
unsigned char Tab4051[8]=
{
	4,//对应温度输入端1
	2,//对应温度输入端2
	1,//对应温度输入端3
	0,//对应温度输入端4
	5,//对应温度输入端5
	3,//对应温度输入端6
	6,//对应温度输入端7
	7,//对应温度输入端8		
};

	  //A信号
	  if((Tab4051[channel]&0x01) == 0x01)
	  {
		 Set4051A;
	  }
	  else
	  {
	    Clr4051A;
	  }
	  //B信号
	  if((Tab4051[channel]&0x02) == 0x02)
	  {
		 Set4051B;
	  }
	  else
	  {
	    Clr4051B;
	  }
	  //C信号
	  if((Tab4051[channel]&0x04) == 0x04)
	  {
		 Set4051C;
	  }
	  else
	  {
	    Clr4051C;
	  }
}
///////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////
void ViewTestStatu(uchar *text)
{
#if (OS_VIEW_MODULE == DEF_ENABLED)
			OSView_TxStr(text, 1);
#else
			SendTestStatu(text);
#endif
}
//////////////////////////////////////////////


void VirtualPwmOutPin(unsigned char ch , unsigned char High)
{
	switch(ch)
	{
		case 0:
			if(High != 0)
			  {//输出有效电平
			  	SetHeat1;
			  }
			else{//关闭有效电平
				  ClrHeat1;
			  }
	  break;
	  
		case 1:
			if(High != 0)
			  {//输出有效电平
			  	SetHeat2;
			  }
			else{//关闭有效电平
				   ClrHeat2;
			  }
	  break;	
	  
		case 2:
			if(High != 0)
			  {//输出有效电平
			  	SetHeat3;
			  }
			else{//关闭有效电平
				  ClrHeat3;
			  }
	  break;
	  
		case 3:
			if(High != 0)
			  {//输出有效电平
			  	SetHeat4;
			  }
			else{//关闭有效电平
				   ClrHeat4;
			  }
	  break;	    

		case 4:
			if(High != 0)
			  {//输出有效电平
			  	SetHeat5;
			  }
			else{//关闭有效电平
				  ClrHeat5;
			  }
	  break;
	  
		case 5:
			if(High != 0)
			  {//输出有效电平
			  	SetHeat6;
			  }
			else{//关闭有效电平
				   ClrHeat6;
			  }
	  break;
	  
		case 6:
			if(High != 0)
			  {//输出有效电平
			  	SetHeat7;
			  }
			else{//关闭有效电平
				  ClrHeat7;
			  }
	  break;
	  
		case 7:
			if(High != 0)
			  {//输出有效电平
			  	SetHeat8;
			  }
			else{//关闭有效电平
				   ClrHeat8;
			  }
	  break;	  		
	}
	
}
//////////////////////////////////////////////
void TaskVirPwm(void * pdata)  //Virtual
{
INT8U err;
unsigned char i;
unsigned char curDuty[8];//占空比
unsigned char curCycle;//周期

pdata = pdata;                          	 	// 避免编译警告		

//#if CONFIG_ADC
//ADC_Configuration();
//#endif
//
//#if CONFIG_PWM
//GPIO_Configuration_PWM();
//PWM_Configuration();  
//#endif


curCycle = 200;//周期

OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.1秒
for(;;)
		{
			//电加热 软件 PWM
			for(i=0;i<8;i++)
			    {
			     if(curDuty[i] > 0)
			     	  {
			     		
			     		 curDuty[i] --;
			     	  }
			     else{//关输出
			     	
			     	   VirtualPwmOutPin(i,0);
			         }
			     }
			
			  if(curCycle > 0)
			     	{
			     		curCycle --;
			     	}
			  else{
			     	 curCycle = 200 ;//周期到
                for(i=0;i<8;i++)
			            {
                   curDuty[i] = Coldw.TC_duty[i]       ;//刷新PWM值
			     	       //开输出
			     	       if(Coldw.unit_onof_flag[i]>0)
			     	       	   {
			     	       		  VirtualPwmOutPin(i,1);
			     	       	   }
			     	       
			     	       }			     	    
			       }
			        
			
		  OSTimeDly(OS_TICKS_PER_SEC/1000);	    //延时0.001秒

		}		
}

////////////////////////////////////////
void Test_Adc(void)
{
///////////////////////////////////////////////////////////////////////
//		  #if CONFIG_ADC
					
					  
//					#if CONFIG_UART1
//					AdcFilter();
//						 	#if   ( OS_VIEW_MODULE != DEF_ENABLED )|| (OS_VIEW_USART_NUMBER != 1) 
//
//											OSSemPend(OSSemUart1,0,&err);
//											
//											SendText_UART1("....");
//											for(i=0;i<MAX_ADC_CH;i++)
//														{
//														MakeValAsc16("",wAdcResoult[i]*330/4096,"V,",buf);
//														
//														SendText_UART1(buf);
//														
//														}
//														
//											SendText_UART1("\r\n");
//											
//											OSSemPost(OSSemUart1);
//											
//							#endif
//					#endif
					
					
//					#if CONFIG_UART1	== 0
//					//ViewTestStatu
//					//AdcFilter();
//					ViewTestStatu("....");//SendText_UART1("....");
//											for(i=0;i<MAX_TEMPRATURE_CHNALL;i++)
//														{
//														MakeValAsc16("",(uint16)(wAdcResoult[i]*10),"(0.1C),",buf);
//														
//														ViewTestStatu(buf);//SendText_UART1(buf);
//														
//														}
//					ViewTestStatu("\r\n");
//					#endif
//					
//			#endif
//			
//			
//
//			OSTimeDly(OS_TICKS_PER_SEC);
}

////////////////////////////////////////
void Test_Uart3(void)
{

		#if   ( OS_VIEW_MODULE != DEF_ENABLED )|| (OS_VIEW_USART_NUMBER !=3)
					#if CONFIG_UART3
							if(GetOneFromUart3(&temp))
									{
										Uart3CharSend(temp);
									}
					#endif
		#endif
}
////////////////////////////////////////////////////////////////////
void ModbusCommand2(void)
{
	  INT8U err;
	  signed short int  steps;
	  unsigned char i;
	  
	  if(Coldw.Moter_step_set == 0)return;
	  	
	  if(  Coldw.Moter_direction  == 0 )      //方向
	  	  {
	  	  	  
	  	     	if(Coldw.Moter_step_set>10000)       //强制走几步
	  	     		  {
	  	     			 steps = 10000;
	  	     		  }
	  	     	else{
	  	     		  steps = (signed short int)Coldw.Moter_step_set;
	  	     	    }	
	  	    	

	  	  }
	  else if(	 Coldw.Moter_direction  == 1 )      //方向  
	  	 {
	  	 	     
	        		if(Coldw.Moter_step_set>10000)       //强制走几步
	        			   {
	        				  steps = -10000;
	        			   }
	        		else{
	        			  steps = - ((signed short int)Coldw.Moter_step_set);
	        		    }	
	        			
     	 }
	for( i=0; i<MAX_TEMPRATURE_CHNALL ; i++)
	              {  		
	  	     	    OSSemPend(OSSemMotors,0,&err);	
	  	     	    MotorsRun(i,steps);
                OSSemPost(OSSemMotors);
	  	          }	  	 
Coldw.Moter_step_set =0;
}
////////////////////////////////////////////////////////////////////
void CheckAdrressKey(void)  //分机地址设置拨码开关
{
	static unsigned char old1,old2,old3,old4,old5;
	
	old1 = GPIO_ReadInputData(GPIOC); //PC0123 ; 
	
	if((old2==old1)&&(old3==old2)&&(old4==old3)&&(old5==old4))//滤波
		{
				lAddressKey=old1;  //拨码开关PB12-PB15，值，最低4位
		}
	old5=old4;
	old4=old3;	
	old3=old2;
	old2=old1;
}
/////////////////////////////////////////////////////////
void TaskModbus(void * pdata)
{
INT8U err;
pdata = pdata;                          	 	// 避免编译警告		

lAddressKey=0;  //拨码开关PB12-PB15，值，最低4位

TimeForSaveParam = 0;
 Default_ParamInit();    //?????????问题
 /*
  if(Load_Param()==0)		//取设定值
   {
    
    gData[0].AnyError|=8;
    gData[1].AnyError|=8;
    gData[2].AnyError|=8;
    gData[3].AnyError|=8;  
    
      
    //自动恢复默认值，不允许
     _Param_SwapMemery(0,cMemBufA);   //parameter ---> cMemBufB
    cMemBufB[Max_MemBuf-2] =  FlagParamInitnized; 
    Write_Param();
   }
*/
#if CONFIG_CHECK_DEVICE_ID
		if(GetStm32F103_DeviceId_Sum6() == gpParam->Stm32IdSum6)
				{
					STM32DeviceId.Checked=1;
				}
		else{
			STM32DeviceId.Checked=0;
				}
#endif


OSTimeDly(OS_TICKS_PER_SEC/100);	    //延时0.01秒

ModbusInit();


//#if TEST_MODBUS_78
// TestModbus();
//#endif

OSTimeDly(OS_TICKS_PER_SEC/5);	    //延时0.2秒

for(;;)
		{
     

    OSTimeDly(OS_TICKS_PER_SEC/500);	    //延时2毫秒

    CheckAdrressKey();//分机地址设置拨码开关
    Coldw.SubAdr = 80 + ( lAddressKey & 0x0f ) ;//          地址设置
	
		ModbusTimeHandler();	
		
		
		GetModbusPark();  //慢点也可以
		
		}
}
////////////////////
void TaskTimePr(void * pdata)
{
  INT8U err;
  uchar  i;  
  unsigned short int  timer1,timer2;

  pdata = pdata;                   // 避免编译警告		
  OSTimeDly(OS_TICKS_PER_SEC/2);	    //延时0.5秒
  for(;;)
     {

      OSTimeDly(OS_TICKS_PER_SEC/100);	    //延时0.01秒
      timer1++;
      if(timer1 >= 20)  //200ms
           {
            timer1  = 0 ;
            OSSemPost(OSSemTimePid_Heat);   //电加热PID控制周期
           }

       timer2++;
       if(timer2 >= 200 ) //2s
           {
            timer2 = 0;
            OSSemPost(OSSemTimePid_StepMotor);   //水冷PID控制周期
           }
       }
}
////////////////////


////////////////////
void TaskHeat(void * pdata)
{
INT8U err;
//signed short int Sval16;
//unsigned short int Uval16;
unsigned char i;

pdata = pdata;                          	 	// 避免编译警告		


OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.5秒
for(;;)
		{
			
			OSSemPend(OSSemTimePid_Heat,0,&err);  //   OSSemTimePid_Heat
			//电加热PID
			
			for(i=0;i<8;i++)
			    {
          //PID_Calc(PID_ParaStruct *types, PidBufStruct *pidch, float NowPoint)
          
          HeatPidBuf[i].SetPoint = Coldw.T_set; //基本上多余
			    PID_Calc(PidParam, &HeatPidBuf[i], ( HeatPidBuf[i].SetPoint - wAdcResoult [ i ] )); //一般是error = SetPoint - NewPoint ,这里反过来
			    
			    
			    
			    HeatPidBuf[0].Qx = 200 ;
			    
			    Coldw.TC_duty[i] = (unsigned short int)HeatPidBuf[i].Qx;
			    //输出
			    
			     }
			     
			     
    Coldw.MONI_PX2 = HeatPidBuf[0].Px;  	   
    Coldw.MONI_IX2 = HeatPidBuf[0].Ix;  	       
    Coldw.MONI_DX2 = HeatPidBuf[0].Dx;  	        
    Coldw.MONI_QX2 = HeatPidBuf[0].Qx; //0.14;
    
		

		  OSTimeDly(OS_TICKS_PER_SEC/100);	    //延时0.1秒

		}
}
/////////////////////////////////////////
void TaskStatus(void * pdata)
{
INT8U err;
uchar i;
pdata = pdata;                          	 	// 避免编译警告		

		for(i=0;i<10;i++)
					{
					LED1_ON;
					LED2_OFF;
					LED3_ON;
					LED4_OFF;
					
					OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒
					LED1_OFF;
					LED2_ON;
					LED3_OFF;
					LED4_ON;
					OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒
					}
			LED1_OFF;		
			LED2_OFF;
			LED3_OFF;
			LED4_OFF;
for(;;)
		{
		if(1)
					{
					Led_Status_Off;
					OSTimeDly(OS_TICKS_PER_SEC/4);	    //延时0.05秒
		
					Led_Status_On;
					OSTimeDly(OS_TICKS_PER_SEC/4);	    //延时0.05秒
					}
			else{
					Led_Status_On;
					OSTimeDly(OS_TICKS_PER_SEC/2);	    //延时0.1秒
					}	

		if(EventTimeBuz>0)
			{
				EventTimeBuz--;
				BUZ_ON;
			}
		else{
			BUZ_OFF;
			
				}

		if(EventTimeLed>0)
			{
				EventTimeLed--;
				Led_Event_On;
			}
		else{
				Led_Event_Off;
				}

		}
}
////////////////////


////////////////////
void TaskTs(void * pdata)
{
INT8U err;
 
unsigned char i; 
 

	//  MAX_TEMPRATURE_CHNALL
float *p_amp[8];	   
float *p_bias[8];


	pdata = pdata;                          	 	// 避免编译警告	
	   
p_amp[0]   = &Coldw.Ts1_AMP;				//温度采样放大倍数1
p_bias[0]  = &Coldw.Ts1_BIAS;				//温度采样偏移系数1  
p_amp[1]   = &Coldw.Ts2_AMP;				//温度采样放大倍数2
p_bias[1]  = &Coldw.Ts2_BIAS;				//温度采样偏移系数2
p_amp[2]   = &Coldw.Ts3_AMP;				//温度采样放大倍数3
p_bias[2]  = &Coldw.Ts3_BIAS;				//温度采样偏移系数3  
p_amp[3]   = &Coldw.Ts4_AMP;				//温度采样放大倍数4
p_bias[3]  = &Coldw.Ts4_BIAS;				//温度采样偏移系数4
p_amp[4]   = &Coldw.Ts5_AMP;				//温度采样放大倍数5
p_bias[4]  = &Coldw.Ts5_BIAS;				//温度采样偏移系数5  
p_amp[5]   = &Coldw.Ts6_AMP;				//温度采样放大倍数6
p_bias[5]  = &Coldw.Ts6_BIAS;				//温度采样偏移系数6
p_amp[6]   = &Coldw.Ts7_AMP;				//温度采样放大倍数7
p_bias[6]  = &Coldw.Ts7_BIAS;				//温度采样偏移系数7  
p_amp[7]   = &Coldw.Ts8_AMP;				//温度采样放大倍数8
p_bias[7]  = &Coldw.Ts8_BIAS;				//温度采样偏移系数8



OSTimeDly(OS_TICKS_PER_SEC);	    //延时0.5秒
ADS8328_Init();			//ADS8328初始化
ADS8328_SelectChannel(1);				//选择ADS8328通道号，1 = 通道1 ，2 = 通道2
//////
	for(;;)
				{
				//OS_ENTER_CRITICAL();
				//OS_EXIT_CRITICAL();
       

Led_Test_Adc_On1;

//delay_us(5000);//5ms

 

         for ( i = 0 ; i < MAX_TEMPRATURE_CHNALL ; i++ )
            {
	           CHECK_SelectChannel( i );  //TC温度采样 通道切换
	           
	           OSTimeDly(OS_TICKS_PER_SEC/500);	    //延时2ms		
	           //CHECK_AD590OneTCCheckUseADS8328( 5 , 1.000  , 0.000  ,  0.1 ,  &( wAdcResoult [ i ] ));  //大约0.8ms
             CHECK_AD590OneTCCheckUseADS8328( 5 , *p_amp[i]  , *p_bias[i]  ,  Coldw.T_bias_set ,  &( wAdcResoult [ i ] ));  //大约0.8ms
           
             Coldw.Ts[i]=wAdcResoult [ i ];
            }
		
		    Led_Test_Adc_Off1;
				   
				OSTimeDly(OS_TICKS_PER_SEC/100);	    //延时10ms			

					}
}
/////////////////////////////


//uint32 NumberHexStringToInt32(uchar *p)	//最多8位HEX码的数字字符串转换为32位无符号整型
//{
//uint32 word2;
//uchar i;
//word2=0;
//for(i=0;i<8;i++)
//	{
//	if(IsHexAsc(*p))
//        	{
//        	word2=word2*0X10+AscToHex(*p);
//        	}
//        else break;
//        p++;
//        };
//
//return  word2;    	
//}

////////////////////////////////////


void ReceivedPowerOut(uchar type1,uchar *p)
{
		  INT8U err;
	uint16 temp16;
	uchar buf[MAX_TEMPRATURE_CHNALL+1][8];
	uchar 	num=0; 		//项目字符串记数。
	uchar 	j=0;    	//逗号个数
	uchar 	i=0;			//整个字符串位置计数
	
	
	num=0; 		//项目字符串记数。
	j=0;    	//逗号个数
	i=0;			//整个字符串位置计数
	while((*(p+i))&&(i<250)&&(j<MAX_TEMPRATURE_CHNALL))
		{

		if(*(p+i)==',' )
			{
			num=0;  //项目字符串开始记数。
			j++;    //逗号个数
			}
			

			if(*(p+i)!=',')
				{
				if(num<6)
					{
					buf[j][num]=*(p+i);  // 不包括，号
					buf[j][num+1]=0;
					num++;
					}
				}

		 i++;	
		 }
		 
for(j=0;j<MAX_TEMPRATURE_CHNALL;j++)
		{		 
		temp16=NumberAscStringToInt16(&buf[j][0]);	
		//if(temp16>255)temp16=255;
		if(type1=='P')
					{
					PowerOutBuf[j]=temp16;
					}
		else if(type1=='D')
					{
					TestOutBuf[j]=temp16;
					}
		}	
		 
		if(type1=='P')
					{

					//StepMotRun1(PowerOutBuf[0]+RelativeOrigin-StepMot1.Position16);
					

					
					}
		else if(type1=='D')
					{

					//StepMotRun1(TestOutBuf[0]);
					}		 
}
////////////////////

//void ResponeTempratur(void)
//{				 INT8U err;
//	uchar buf[10+6*(MAX_TEMPRATURE_CHNALL)];
//	uchar i,n;
//
//	uint16 temp16;
//	
//	n=PutString("^T",buf,5);
//
//	
//	for(i=0;i<MAX_TEMPRATURE_CHNALL;i++)
//				{
//
//				if( wAdcResoult[ i ] < 0 )
//					{
//						temp16=0;
//          }
//				else temp16=(uint16)( 10* wAdcResoult[ i ] );//单位0.1度
//					
//				n+=MakeValAsc16("",temp16,",",&buf[n]);//单位0.1度
//
//				}
//	n+=PutString("\r\n",&buf[n],5);
//	
//	#if CONFIG_UART1
//		OSSemPend(OSSemUart1,0,&err);
//
//			SendText_UART1(buf);
//		OSSemPost(OSSemUart1);
//	#endif
//}
///////////////////////////////////////



///////////////////////////////////////
//void ResponePower(void)
//{		INT8U err;
//	uchar buf[10+6*(MAX_TEMPRATURE_CHNALL)];
//	uchar n,i;
//	n=PutString("^P",buf,5);
//
//	
//	for(i=0;i<MAX_TEMPRATURE_CHNALL;i++)
//				{
//				n+=MakeValAsc16("",PowerOutBuf[i],",",&buf[n]);
//				}
//	n+=PutString("\r\n",&buf[n],5);
//	#if CONFIG_UART1
//		OSSemPend(OSSemUart1,0,&err);
//
//			SendText_UART1(buf);
//		OSSemPost(OSSemUart1);
//	#endif
//	
//}

//电脑下传的控制量绝对位置   %P32,33,89,....\r\n
//电脑下传的走几步					%D655,32,77,...\r\n
//请求查询温度  		%RTA\r\n
//单片机上传的温度值   ^T1234,1235,1236,1058,....\r\n

////////////////////
//void ProcessRecvUser(uchar *p,uchar len)
//{
//
//INT8U err;	
//	
//		uint8_t *flash;
//		
//	uint32_t adress;
//	uchar i;
//	
//	if(*p!='%')
//		{
//			return;	
//		}
//
//	
//switch (*(p+1))
//		{
//		case 'R':	//请求查询温度
//
//		//wAdcResoult[MAX_TEMPRATURE_CHNALL];    //用来存放求平均值之后的结果
//	
//		
//		ResponeTempratur();
//		break;	
//			
//		case 'P':  //控制值
//			ReceivedPowerOut(*(p+1),p+2);
//			ResponePower();
//
//		break;	
//		
//		case 'D':  //测试值，前后走几步，有负数
//			ReceivedPowerOut(*(p+1),p+2);
//			ResponePower();
//
//		break;			
//		
//		case 'F':  //读FLASH	
//		adress=NumberHexStringToInt32(p+2);
//		flash=(uchar *)adress;
//
//		//SendTestBufHex(flash,16);
//
//		#if CONFIG_UART1
//		OSSemPend(OSSemUart1,0,&err);
//				SendText_UART1("...");
//				for(i=0;i<16;i++)
//								{
//									Uart1CharSend(HexToAsc(*(flash+i)>>4));
//									Uart1CharSend(HexToAsc(*(flash+i)));
//									Uart1CharSend(0x20);
//								}
//				SendText_UART1("\r\n");
//		OSSemPost(OSSemUart1);
//		#endif
//		break;
//		
//		
//		
//		case 0:
//		default:	
//		break;
//		
//	
//	
//		}
//		
//
//
//	
//}

////////////////////
//void RecvFromUser(uchar temp)
//{
//	
//	switch(temp)
//		{
//			case '%':	
//			BufFromUser[0]=temp;	//'*' or ':';	
//			LenFromUser=1;
//			
//			break;
//			
//			case '\r':
//			BufFromUser[LenFromUser]=0;
//			//处理
//			ProcessRecvUser(BufFromUser,LenFromUser);
//			break;
//			
//			default:
//				if(LenFromUser<250)
//					{
//						BufFromUser[LenFromUser]=temp;
//						LenFromUser++;
//					}
//				else LenFromUser=0;
//			break;	
//			
//			
//		}
//	
//}

/////////////////////////////
void TaskRecv(void * pdata)
{
	INT8U err;
	
//	uchar timeout,temp;
//	uchar flag_recv=0;
	
 	pdata = pdata;                          	 	// 避免编译警告	   


OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.1秒

/*
//ProcessRecvUser(":R08020000\r\n",8);

OSSemPend(OSSemUart1,0,&err);
SendText_UART1(":R08010000\r\n");
OSSemPost(OSSemUart1);

ProcessRecvUser(":R08010000\r\n",8);



//产品唯一身份标识寄存器（96位）
//基地址0X1FFF F7E8
//位15:0
//地址偏移0x02
//位31:16
//地址偏移0x04
//位63:32
//地址偏移0x06
//位95:64


OSSemPend(OSSemUart1,0,&err);
SendText_UART1(":R1FFFF7E8\r\n");
OSSemPost(OSSemUart1);
ProcessRecvUser(":R1FFFF7E8\r\n",8);
*/






//BufFromUser[0]=0;	//'*' or ':';	
//LenFromUser=0;

//////
	for(;;)
				{
				//OS_ENTER_CRITICAL();
				//OS_EXIT_CRITICAL();
/*
						
						timeout=0;
						while(timeout<5)
								{
									#if CONFIG_UART1
								OSSemPend(OSSemUart1,0,&err);
								flag_recv=GetOneFromUart1(&temp);
								OSSemPost(OSSemUart1);
								
								
									if(flag_recv)
										{
											EventTimeLed=2;
										RecvFromUser(temp);
										
										timeout=0;
										}
								   #endif
									timeout++;
								}

*/

    //Coldw.Moter_step_set;       //强制走几步
    //Coldw.Moter_direction;      //方向

	OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.01秒

					}
}
////////////////////
//正数 前进2400步，在8分频时是300步步进电机的极限行程，前进到极限，关闭阀门	
//StepMotRun1( 2400);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
//负数 后退2400步，在8分频时是300步步进电机的极限行程，退到最后，全开阀门
//StepMotRun1(-2400);//MOTOR_BACK_STEP);
void MotorsRun(unsigned char ch, signed short int steps)
{
	switch(ch)
	{
		case 0:
		StepMotRun1(steps);	
		break;
			
	  case 1:		
		StepMotRun2(steps);	
		break;
		
		case 2:
			StepMotRun3(steps);			
			
		break;	
	  case 3:		
					StepMotRun4(steps);	
		break;		
		case 4:
					StepMotRun5(steps);	
			
		break;	
	  case 5:		
					StepMotRun6(steps);	
		break;		
		
	}
	
}
////////////////////
void TaskStepMotor(void * pdata)
{
INT8U err;

uchar i;

pdata = pdata;                          	 	// 避免编译警告		
OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.1秒


  StepMotInit();
  
  OSSemMotors = OSSemCreate(1);
  
  /*

//后退2400步，在8分频时是300步步进电机的极限行程，退到最后，全开阀门
	StepMotRun1(-2400);//MOTOR_BACK_STEP);
	StepMotRun2(-2400);//MOTOR_TEST_STEP);
	
	OSTimeDly(OS_TICKS_PER_SEC*6);	    //延时6秒
//前进2400步，在8分频时是300步步进电机的极限行程，前进到极限，关闭阀门	
	StepMotRun1( 2400);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
	StepMotRun2( 2400);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
						
	//延时等待步进电机完成步数,2400*20*100us	= 4.8S				
	OSTimeDly(OS_TICKS_PER_SEC*6);	    //延时6秒
								
								
	//正常工作服务在关闭前的2/3mm，就是200步，还有最后的80步都是关闭的，
	//设打开最大处为0，则关闭最大处为2400步，实际范围是2200-2320，我们取2105-2360
	//0点取2104，则结束点为255，关闭最大处为255+40  步则这个范围为这个255作为关到底的初始值							
  //StepMot1.Position16=(255+40);  //步进电机脉冲表示绝对位置，带符号


RelativeOrigin=2104;    //相对坐标原点
StepMot1.Position16=2400;
//改进:
//或者使用相对位置，这里继续设置为2400.。。。输入点加2105再输出到MOTOR
//StepMot1.Position16用FPROM记录，下次开电时知道位置了，然后先走到0，然后向前走2400步，再校对为0
//温度单位改为0.5度


*/
/*
for(;;)
		{
    for ( i = 0 ; i < MAX_TEMPRATURE_CHNALL ; i++ )
             {
    	        MotorsRun(i,2400);
    	        
						 }
		OSTimeDly(OS_TICKS_PER_SEC*7);
    for ( i = 0 ; i < MAX_TEMPRATURE_CHNALL ; i++ )
             {							 
							MotorsRun(i,-2400);

              }
		OSTimeDly(OS_TICKS_PER_SEC*7);		
							
    }
*/

for(;;)
		{

     OSSemPend(OSSemTimePid_StepMotor,0,&err);  //OSSemPost(OSSemTimePid_StepMotor);   OSSemTimePid_Heat
     
     for ( i = 0 ; i < MAX_TEMPRATURE_CHNALL ; i++ )
            {

				
				
				    if( ( wAdcResoult [ i ] -  Coldw.T_set ) > 15  )
				    	  {//超
				    		
				    		if( Coldw.Counter_MaxOpen[i] < ( STEPER_MOTOR_MAX_COUNT_OPEN + 2 ))				//温度超过设定值15度以上，不使用PID,强制打开水冷，开n次后暂停，然后等温度到正常范围
				    				{//打开 (10 + 2 )x 240步  = 2880
				             //后退200/2400步，2400步在8分频时是300步步进电机的极限行程，开阀门
	                   OSSemPend(OSSemMotors,0,&err);	
	                   MotorsRun(i, - ( MOTOR_MAX_STEP / STEPER_MOTOR_MAX_COUNT_OPEN  ) );//后退240步
                     OSSemPost(OSSemMotors);			    					
				    					
				    				Coldw.Counter_MaxOpen[i]++;
				    				}
				    				
				    		Coldw.Counter_MaxClose[i]  =  0;
				    		
				    	  }
						else if( ( Coldw.T_set -  wAdcResoult [ i ] ) > 15  )
							{//太低
								if( Coldw.Counter_MaxClose[i]	< (STEPER_MOTOR_MAX_COUNT_CLOSE+15) )			//温度低于设定值15度以上，不使用PID,强制关闭水冷，关n次后暂停，然后等温度到正常范围
										{//关闭 (75+15) x 32步 2880
										//前进2400步，在8分频时是300步步进电机的极限行程，前进到极限，关闭阀门	

	                  OSSemPend(OSSemMotors,0,&err);	
										MotorsRun(i,MOTOR_MAX_STEP / STEPER_MOTOR_MAX_COUNT_CLOSE );
										OSSemPost(OSSemMotors);
										
							      Coldw.Counter_MaxClose[i]++;
									  }
								
								Coldw.Counter_MaxOpen[i]   =  0;	  
							}
						else{//正常范围，需要PID控制	
									//超范围计数器清零
									Coldw.Counter_MaxOpen[i]   =  0;				//温度超过设定值15度以上，不使用PID,强制打开水冷，开n次后暂停，然后等温度到正常范围
            			Coldw.Counter_MaxClose[i]  =  0;				//温度低于设定值15度以上，不使用PID,强制关闭水冷，关n次后暂停，然后等温度到正常范围
           				
           				PID_Inc_Calc( &StepPidBuf[i], Coldw.T_set +0.2 -  wAdcResoult [ i ]);		//水冷加高0.2度
           				
           				//输出 FOLT ->>>  INT
           				Coldw.FAN_duty[i] = StepPidBuf[i].Qx;
           					
           				OSSemPend(OSSemMotors,0,&err);	
                  MotorsRun(i,Coldw.FAN_duty[i]);	
                  OSSemPost(OSSemMotors);
							}
					
					  }
					  
		Coldw.MONI_PX1 = StepPidBuf[0].Px;//0.11;                //
    Coldw.MONI_IX1 = StepPidBuf[0].Ix;//0.12;
    Coldw.MONI_DX1 = StepPidBuf[0].Dx;//0.13;
    Coldw.MONI_QX1 = StepPidBuf[0].Qx; //0.14;
    
    //Coldw.MONI_PX2 = 0.21;                //
      OSTimeDly(OS_TICKS_PER_SEC/500);  //补充延时

		}
}
/////////////////////////////


void ProcessKey(uchar kb,uchar kc)
{
	

			if(kb&BIT0)
					{//开步进电机
						
						
						
						if(kb!=kc)
							{
						//		StepMotRun1(  500);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
						//		StepMotRun2( 500);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
						


							}
					}
		if(kb&BIT1)
					{//开步进电机  反向
						
						
						if(kb!=kc)
							{
								
					//	StepMotRun1(-500);//MOTOR_BACK_STEP);
					//	StepMotRun2(-500);//MOTOR_TEST_STEP);		

	
							}
					}
					
		if(kb&BIT2)
					{//关步进电机
						
						StepMotStop1();
						
						StepMotStop2();
								
						

					}				
										
}
void TaskKey(void * pdata)
{
//INT8U err;
 
	uchar ka,kb,kc,kd;

	pdata = pdata;     // 避免编译警告	   
	FlagKey=0;
	ka=0;
	kb=0;
	kc=0;
	kd=0;


		for(;;)
						{
						//OS_ENTER_CRITICAL();
						//OS_EXIT_CRITICAL();
						OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒		
						
					if(InPin_K3)
							{//没有按键
								ka&=(~BIT0);		//

							}
					else {
								ka|=BIT0;

								}

					if(InPin_K4)
							{//没有按键
								ka&=(~BIT1);		//
							}
					else {
								ka|=BIT1;
								}

					if(InPin_K5)
							{//没有按键
								ka&=(~BIT2);		//
							}
					else {
								ka|=BIT2;
								}		
								
											
						if(kb==ka)
							{//滤波kb
								
								ProcessKey(kb,kc);

								if(kc!=kb)
										{//有变化
											kd=kc^kb;		//变化
										EventTimeLed=4;
										}
								kc=kb;			//键盘值	
							}
						else{//滤波kb
								kb=ka;
							}
							
		
				}	
}



////////////////////////////////////////////////
void TaskInput1(void * pdata)
{
//INT8U err;
 
 
	pdata = pdata;                          	 	// 避免编译警告	   


	for(;;)
				{
				//OS_ENTER_CRITICAL();
				//OS_EXIT_CRITICAL();


					
			OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒		

	
					}
}
///////////////////////////









/////////////////



//////////////






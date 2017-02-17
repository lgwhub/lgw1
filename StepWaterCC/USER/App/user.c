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

//2400����8��Ƶʱ��300����������ļ����г̣�������
//����¶�̫�ߣ������趨ֵ15�ȣ�����PID,ֱ�Ӵ�240���������˴���ˮ����Ϊ10+2��
#define   STEPER_MOTOR_MAX_COUNT_OPEN     10
//����¶�̫�ͣ������趨ֵ15�ȣ�����PID,ֱ�ӹر�32�������ǰ���ر���ˮ����Ϊ75+15��
#define   STEPER_MOTOR_MAX_COUNT_CLOSE    75
//�����Ͳ���һֱ�ڼ��޴�������

unsigned char lAddressKey;  //���뿪��PB12-PB15��ֵ�����4λ

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
//��ѹ�������¶�̽ͷΪAD590������Ϊ10K
//-23��ʱΪ250uA����ʱ��ѹֵΪ(273-23)*10K = 2.5V
// 0��ʱΪ273uA����ʱ��ѹֵΪ273uA*10K=2.73V,	 ��2.5V��ֺ��Ӧ����ֵΪ(2.73V-2.5V)*65535/2.5= 6029
//27��ʱΪ300uA����ʱ��ѹֵΪ300uA*10K=3V,	 ��2.5V��ֺ��Ӧ����ֵΪ(3V-2.5V)*65535/2.5= 13107
//150��ʱΪ 423uA����ʱ��ѹֵΪ423uA*10K=4.23V����2.5V��ֺ��Ӧ����ֵΪ(4.23V-2.5V)*65535/2.5= 45350
//num��ʾ����������temp_TC_bias��ʾ�¶�ƫ��ֵ��data_ptr��ʾ����ָ��
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



	  if(temp_tt > 6029)  //���¶�̽ͷ��,�¶ȴ���0�ȵ�
	     {
	  	  temp_tt -= 6029;
		    temp2 =( float) temp_tt;
	      temp2 = temp2 / 262;
	      temp2 += temp_TC_bias;
	     }
	  else  //û���¶�̽ͷ
	    {
	     temp2 = 0;
	     }

	     
	  temp2 *= data_amp;  //Ĭ�� 1.000
	  temp2 += data_bias; //Ĭ�� 0.000
	  
	  *data_ptr = temp2;
}

///////////////////////////////////////////////////////////////////////

//8ѡ1ѡ��ͨ����A,B,C
void CHECK_SelectChannel(unsigned char channel)
{
	
	//ABC  ...  PC5 PB0  PB1
unsigned char Tab4051[8]=
{
	4,//��Ӧ�¶������1
	2,//��Ӧ�¶������2
	1,//��Ӧ�¶������3
	0,//��Ӧ�¶������4
	5,//��Ӧ�¶������5
	3,//��Ӧ�¶������6
	6,//��Ӧ�¶������7
	7,//��Ӧ�¶������8		
};

	  //A�ź�
	  if((Tab4051[channel]&0x01) == 0x01)
	  {
		 Set4051A;
	  }
	  else
	  {
	    Clr4051A;
	  }
	  //B�ź�
	  if((Tab4051[channel]&0x02) == 0x02)
	  {
		 Set4051B;
	  }
	  else
	  {
	    Clr4051B;
	  }
	  //C�ź�
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
			  {//�����Ч��ƽ
			  	SetHeat1;
			  }
			else{//�ر���Ч��ƽ
				  ClrHeat1;
			  }
	  break;
	  
		case 1:
			if(High != 0)
			  {//�����Ч��ƽ
			  	SetHeat2;
			  }
			else{//�ر���Ч��ƽ
				   ClrHeat2;
			  }
	  break;	
	  
		case 2:
			if(High != 0)
			  {//�����Ч��ƽ
			  	SetHeat3;
			  }
			else{//�ر���Ч��ƽ
				  ClrHeat3;
			  }
	  break;
	  
		case 3:
			if(High != 0)
			  {//�����Ч��ƽ
			  	SetHeat4;
			  }
			else{//�ر���Ч��ƽ
				   ClrHeat4;
			  }
	  break;	    

		case 4:
			if(High != 0)
			  {//�����Ч��ƽ
			  	SetHeat5;
			  }
			else{//�ر���Ч��ƽ
				  ClrHeat5;
			  }
	  break;
	  
		case 5:
			if(High != 0)
			  {//�����Ч��ƽ
			  	SetHeat6;
			  }
			else{//�ر���Ч��ƽ
				   ClrHeat6;
			  }
	  break;
	  
		case 6:
			if(High != 0)
			  {//�����Ч��ƽ
			  	SetHeat7;
			  }
			else{//�ر���Ч��ƽ
				  ClrHeat7;
			  }
	  break;
	  
		case 7:
			if(High != 0)
			  {//�����Ч��ƽ
			  	SetHeat8;
			  }
			else{//�ر���Ч��ƽ
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
unsigned char curDuty[8];//ռ�ձ�
unsigned char curCycle;//����

pdata = pdata;                          	 	// ������뾯��		

//#if CONFIG_ADC
//ADC_Configuration();
//#endif
//
//#if CONFIG_PWM
//GPIO_Configuration_PWM();
//PWM_Configuration();  
//#endif


curCycle = 200;//����

OSTimeDly(OS_TICKS_PER_SEC/10);	    //��ʱ0.1��
for(;;)
		{
			//����� ��� PWM
			for(i=0;i<8;i++)
			    {
			     if(curDuty[i] > 0)
			     	  {
			     		
			     		 curDuty[i] --;
			     	  }
			     else{//�����
			     	
			     	   VirtualPwmOutPin(i,0);
			         }
			     }
			
			  if(curCycle > 0)
			     	{
			     		curCycle --;
			     	}
			  else{
			     	 curCycle = 200 ;//���ڵ�
                for(i=0;i<8;i++)
			            {
                   curDuty[i] = Coldw.TC_duty[i]       ;//ˢ��PWMֵ
			     	       //�����
			     	       if(Coldw.unit_onof_flag[i]>0)
			     	       	   {
			     	       		  VirtualPwmOutPin(i,1);
			     	       	   }
			     	       
			     	       }			     	    
			       }
			        
			
		  OSTimeDly(OS_TICKS_PER_SEC/1000);	    //��ʱ0.001��

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
	  	
	  if(  Coldw.Moter_direction  == 0 )      //����
	  	  {
	  	  	  
	  	     	if(Coldw.Moter_step_set>10000)       //ǿ���߼���
	  	     		  {
	  	     			 steps = 10000;
	  	     		  }
	  	     	else{
	  	     		  steps = (signed short int)Coldw.Moter_step_set;
	  	     	    }	
	  	    	

	  	  }
	  else if(	 Coldw.Moter_direction  == 1 )      //����  
	  	 {
	  	 	     
	        		if(Coldw.Moter_step_set>10000)       //ǿ���߼���
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
void CheckAdrressKey(void)  //�ֻ���ַ���ò��뿪��
{
	static unsigned char old1,old2,old3,old4,old5;
	
	old1 = GPIO_ReadInputData(GPIOC); //PC0123 ; 
	
	if((old2==old1)&&(old3==old2)&&(old4==old3)&&(old5==old4))//�˲�
		{
				lAddressKey=old1;  //���뿪��PB12-PB15��ֵ�����4λ
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
pdata = pdata;                          	 	// ������뾯��		

lAddressKey=0;  //���뿪��PB12-PB15��ֵ�����4λ

TimeForSaveParam = 0;
 Default_ParamInit();    //?????????����
 /*
  if(Load_Param()==0)		//ȡ�趨ֵ
   {
    
    gData[0].AnyError|=8;
    gData[1].AnyError|=8;
    gData[2].AnyError|=8;
    gData[3].AnyError|=8;  
    
      
    //�Զ��ָ�Ĭ��ֵ��������
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


OSTimeDly(OS_TICKS_PER_SEC/100);	    //��ʱ0.01��

ModbusInit();


//#if TEST_MODBUS_78
// TestModbus();
//#endif

OSTimeDly(OS_TICKS_PER_SEC/5);	    //��ʱ0.2��

for(;;)
		{
     

    OSTimeDly(OS_TICKS_PER_SEC/500);	    //��ʱ2����

    CheckAdrressKey();//�ֻ���ַ���ò��뿪��
    Coldw.SubAdr = 80 + ( lAddressKey & 0x0f ) ;//          ��ַ����
	
		ModbusTimeHandler();	
		
		
		GetModbusPark();  //����Ҳ����
		
		}
}
////////////////////
void TaskTimePr(void * pdata)
{
  INT8U err;
  uchar  i;  
  unsigned short int  timer1,timer2;

  pdata = pdata;                   // ������뾯��		
  OSTimeDly(OS_TICKS_PER_SEC/2);	    //��ʱ0.5��
  for(;;)
     {

      OSTimeDly(OS_TICKS_PER_SEC/100);	    //��ʱ0.01��
      timer1++;
      if(timer1 >= 20)  //200ms
           {
            timer1  = 0 ;
            OSSemPost(OSSemTimePid_Heat);   //�����PID��������
           }

       timer2++;
       if(timer2 >= 200 ) //2s
           {
            timer2 = 0;
            OSSemPost(OSSemTimePid_StepMotor);   //ˮ��PID��������
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

pdata = pdata;                          	 	// ������뾯��		


OSTimeDly(OS_TICKS_PER_SEC/10);	    //��ʱ0.5��
for(;;)
		{
			
			OSSemPend(OSSemTimePid_Heat,0,&err);  //   OSSemTimePid_Heat
			//�����PID
			
			for(i=0;i<8;i++)
			    {
          //PID_Calc(PID_ParaStruct *types, PidBufStruct *pidch, float NowPoint)
          
          HeatPidBuf[i].SetPoint = Coldw.T_set; //�����϶���
			    PID_Calc(PidParam, &HeatPidBuf[i], ( HeatPidBuf[i].SetPoint - wAdcResoult [ i ] )); //һ����error = SetPoint - NewPoint ,���ﷴ����
			    
			    
			    
			    HeatPidBuf[0].Qx = 200 ;
			    
			    Coldw.TC_duty[i] = (unsigned short int)HeatPidBuf[i].Qx;
			    //���
			    
			     }
			     
			     
    Coldw.MONI_PX2 = HeatPidBuf[0].Px;  	   
    Coldw.MONI_IX2 = HeatPidBuf[0].Ix;  	       
    Coldw.MONI_DX2 = HeatPidBuf[0].Dx;  	        
    Coldw.MONI_QX2 = HeatPidBuf[0].Qx; //0.14;
    
		

		  OSTimeDly(OS_TICKS_PER_SEC/100);	    //��ʱ0.1��

		}
}
/////////////////////////////////////////
void TaskStatus(void * pdata)
{
INT8U err;
uchar i;
pdata = pdata;                          	 	// ������뾯��		

		for(i=0;i<10;i++)
					{
					LED1_ON;
					LED2_OFF;
					LED3_ON;
					LED4_OFF;
					
					OSTimeDly(OS_TICKS_PER_SEC/20);	    //��ʱ0.05��
					LED1_OFF;
					LED2_ON;
					LED3_OFF;
					LED4_ON;
					OSTimeDly(OS_TICKS_PER_SEC/20);	    //��ʱ0.05��
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
					OSTimeDly(OS_TICKS_PER_SEC/4);	    //��ʱ0.05��
		
					Led_Status_On;
					OSTimeDly(OS_TICKS_PER_SEC/4);	    //��ʱ0.05��
					}
			else{
					Led_Status_On;
					OSTimeDly(OS_TICKS_PER_SEC/2);	    //��ʱ0.1��
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


	pdata = pdata;                          	 	// ������뾯��	
	   
p_amp[0]   = &Coldw.Ts1_AMP;				//�¶Ȳ����Ŵ���1
p_bias[0]  = &Coldw.Ts1_BIAS;				//�¶Ȳ���ƫ��ϵ��1  
p_amp[1]   = &Coldw.Ts2_AMP;				//�¶Ȳ����Ŵ���2
p_bias[1]  = &Coldw.Ts2_BIAS;				//�¶Ȳ���ƫ��ϵ��2
p_amp[2]   = &Coldw.Ts3_AMP;				//�¶Ȳ����Ŵ���3
p_bias[2]  = &Coldw.Ts3_BIAS;				//�¶Ȳ���ƫ��ϵ��3  
p_amp[3]   = &Coldw.Ts4_AMP;				//�¶Ȳ����Ŵ���4
p_bias[3]  = &Coldw.Ts4_BIAS;				//�¶Ȳ���ƫ��ϵ��4
p_amp[4]   = &Coldw.Ts5_AMP;				//�¶Ȳ����Ŵ���5
p_bias[4]  = &Coldw.Ts5_BIAS;				//�¶Ȳ���ƫ��ϵ��5  
p_amp[5]   = &Coldw.Ts6_AMP;				//�¶Ȳ����Ŵ���6
p_bias[5]  = &Coldw.Ts6_BIAS;				//�¶Ȳ���ƫ��ϵ��6
p_amp[6]   = &Coldw.Ts7_AMP;				//�¶Ȳ����Ŵ���7
p_bias[6]  = &Coldw.Ts7_BIAS;				//�¶Ȳ���ƫ��ϵ��7  
p_amp[7]   = &Coldw.Ts8_AMP;				//�¶Ȳ����Ŵ���8
p_bias[7]  = &Coldw.Ts8_BIAS;				//�¶Ȳ���ƫ��ϵ��8



OSTimeDly(OS_TICKS_PER_SEC);	    //��ʱ0.5��
ADS8328_Init();			//ADS8328��ʼ��
ADS8328_SelectChannel(1);				//ѡ��ADS8328ͨ���ţ�1 = ͨ��1 ��2 = ͨ��2
//////
	for(;;)
				{
				//OS_ENTER_CRITICAL();
				//OS_EXIT_CRITICAL();
       

Led_Test_Adc_On1;

//delay_us(5000);//5ms

 

         for ( i = 0 ; i < MAX_TEMPRATURE_CHNALL ; i++ )
            {
	           CHECK_SelectChannel( i );  //TC�¶Ȳ��� ͨ���л�
	           
	           OSTimeDly(OS_TICKS_PER_SEC/500);	    //��ʱ2ms		
	           //CHECK_AD590OneTCCheckUseADS8328( 5 , 1.000  , 0.000  ,  0.1 ,  &( wAdcResoult [ i ] ));  //��Լ0.8ms
             CHECK_AD590OneTCCheckUseADS8328( 5 , *p_amp[i]  , *p_bias[i]  ,  Coldw.T_bias_set ,  &( wAdcResoult [ i ] ));  //��Լ0.8ms
           
             Coldw.Ts[i]=wAdcResoult [ i ];
            }
		
		    Led_Test_Adc_Off1;
				   
				OSTimeDly(OS_TICKS_PER_SEC/100);	    //��ʱ10ms			

					}
}
/////////////////////////////


//uint32 NumberHexStringToInt32(uchar *p)	//���8λHEX��������ַ���ת��Ϊ32λ�޷�������
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
	uchar 	num=0; 		//��Ŀ�ַ���������
	uchar 	j=0;    	//���Ÿ���
	uchar 	i=0;			//�����ַ���λ�ü���
	
	
	num=0; 		//��Ŀ�ַ���������
	j=0;    	//���Ÿ���
	i=0;			//�����ַ���λ�ü���
	while((*(p+i))&&(i<250)&&(j<MAX_TEMPRATURE_CHNALL))
		{

		if(*(p+i)==',' )
			{
			num=0;  //��Ŀ�ַ�����ʼ������
			j++;    //���Ÿ���
			}
			

			if(*(p+i)!=',')
				{
				if(num<6)
					{
					buf[j][num]=*(p+i);  // ����������
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
//				else temp16=(uint16)( 10* wAdcResoult[ i ] );//��λ0.1��
//					
//				n+=MakeValAsc16("",temp16,",",&buf[n]);//��λ0.1��
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

//�����´��Ŀ���������λ��   %P32,33,89,....\r\n
//�����´����߼���					%D655,32,77,...\r\n
//�����ѯ�¶�  		%RTA\r\n
//��Ƭ���ϴ����¶�ֵ   ^T1234,1235,1236,1058,....\r\n

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
//		case 'R':	//�����ѯ�¶�
//
//		//wAdcResoult[MAX_TEMPRATURE_CHNALL];    //���������ƽ��ֵ֮��Ľ��
//	
//		
//		ResponeTempratur();
//		break;	
//			
//		case 'P':  //����ֵ
//			ReceivedPowerOut(*(p+1),p+2);
//			ResponePower();
//
//		break;	
//		
//		case 'D':  //����ֵ��ǰ���߼������и���
//			ReceivedPowerOut(*(p+1),p+2);
//			ResponePower();
//
//		break;			
//		
//		case 'F':  //��FLASH	
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
//			//����
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
	
 	pdata = pdata;                          	 	// ������뾯��	   


OSTimeDly(OS_TICKS_PER_SEC/10);	    //��ʱ0.1��

/*
//ProcessRecvUser(":R08020000\r\n",8);

OSSemPend(OSSemUart1,0,&err);
SendText_UART1(":R08010000\r\n");
OSSemPost(OSSemUart1);

ProcessRecvUser(":R08010000\r\n",8);



//��ƷΨһ��ݱ�ʶ�Ĵ�����96λ��
//����ַ0X1FFF F7E8
//λ15:0
//��ַƫ��0x02
//λ31:16
//��ַƫ��0x04
//λ63:32
//��ַƫ��0x06
//λ95:64


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

    //Coldw.Moter_step_set;       //ǿ���߼���
    //Coldw.Moter_direction;      //����

	OSTimeDly(OS_TICKS_PER_SEC/10);	    //��ʱ0.01��

					}
}
////////////////////
//���� ǰ��2400������8��Ƶʱ��300����������ļ����г̣�ǰ�������ޣ��رշ���	
//StepMotRun1( 2400);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
//���� ����2400������8��Ƶʱ��300����������ļ����г̣��˵����ȫ������
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

pdata = pdata;                          	 	// ������뾯��		
OSTimeDly(OS_TICKS_PER_SEC/10);	    //��ʱ0.1��


  StepMotInit();
  
  OSSemMotors = OSSemCreate(1);
  
  /*

//����2400������8��Ƶʱ��300����������ļ����г̣��˵����ȫ������
	StepMotRun1(-2400);//MOTOR_BACK_STEP);
	StepMotRun2(-2400);//MOTOR_TEST_STEP);
	
	OSTimeDly(OS_TICKS_PER_SEC*6);	    //��ʱ6��
//ǰ��2400������8��Ƶʱ��300����������ļ����г̣�ǰ�������ޣ��رշ���	
	StepMotRun1( 2400);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
	StepMotRun2( 2400);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
						
	//��ʱ�ȴ����������ɲ���,2400*20*100us	= 4.8S				
	OSTimeDly(OS_TICKS_PER_SEC*6);	    //��ʱ6��
								
								
	//�������������ڹر�ǰ��2/3mm������200������������80�����ǹرյģ�
	//������Ϊ0����ر����Ϊ2400����ʵ�ʷ�Χ��2200-2320������ȡ2105-2360
	//0��ȡ2104���������Ϊ255���ر����Ϊ255+40  ���������ΧΪ���255��Ϊ�ص��׵ĳ�ʼֵ							
  //StepMot1.Position16=(255+40);  //������������ʾ����λ�ã�������


RelativeOrigin=2104;    //�������ԭ��
StepMot1.Position16=2400;
//�Ľ�:
//����ʹ�����λ�ã������������Ϊ2400.������������2105�������MOTOR
//StepMot1.Position16��FPROM��¼���´ο���ʱ֪��λ���ˣ�Ȼ�����ߵ�0��Ȼ����ǰ��2400������У��Ϊ0
//�¶ȵ�λ��Ϊ0.5��


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
				    	  {//��
				    		
				    		if( Coldw.Counter_MaxOpen[i] < ( STEPER_MOTOR_MAX_COUNT_OPEN + 2 ))				//�¶ȳ����趨ֵ15�����ϣ���ʹ��PID,ǿ�ƴ�ˮ�䣬��n�κ���ͣ��Ȼ����¶ȵ�������Χ
				    				{//�� (10 + 2 )x 240��  = 2880
				             //����200/2400����2400����8��Ƶʱ��300����������ļ����г̣�������
	                   OSSemPend(OSSemMotors,0,&err);	
	                   MotorsRun(i, - ( MOTOR_MAX_STEP / STEPER_MOTOR_MAX_COUNT_OPEN  ) );//����240��
                     OSSemPost(OSSemMotors);			    					
				    					
				    				Coldw.Counter_MaxOpen[i]++;
				    				}
				    				
				    		Coldw.Counter_MaxClose[i]  =  0;
				    		
				    	  }
						else if( ( Coldw.T_set -  wAdcResoult [ i ] ) > 15  )
							{//̫��
								if( Coldw.Counter_MaxClose[i]	< (STEPER_MOTOR_MAX_COUNT_CLOSE+15) )			//�¶ȵ����趨ֵ15�����ϣ���ʹ��PID,ǿ�ƹر�ˮ�䣬��n�κ���ͣ��Ȼ����¶ȵ�������Χ
										{//�ر� (75+15) x 32�� 2880
										//ǰ��2400������8��Ƶʱ��300����������ļ����г̣�ǰ�������ޣ��رշ���	

	                  OSSemPend(OSSemMotors,0,&err);	
										MotorsRun(i,MOTOR_MAX_STEP / STEPER_MOTOR_MAX_COUNT_CLOSE );
										OSSemPost(OSSemMotors);
										
							      Coldw.Counter_MaxClose[i]++;
									  }
								
								Coldw.Counter_MaxOpen[i]   =  0;	  
							}
						else{//������Χ����ҪPID����	
									//����Χ����������
									Coldw.Counter_MaxOpen[i]   =  0;				//�¶ȳ����趨ֵ15�����ϣ���ʹ��PID,ǿ�ƴ�ˮ�䣬��n�κ���ͣ��Ȼ����¶ȵ�������Χ
            			Coldw.Counter_MaxClose[i]  =  0;				//�¶ȵ����趨ֵ15�����ϣ���ʹ��PID,ǿ�ƹر�ˮ�䣬��n�κ���ͣ��Ȼ����¶ȵ�������Χ
           				
           				PID_Inc_Calc( &StepPidBuf[i], Coldw.T_set +0.2 -  wAdcResoult [ i ]);		//ˮ��Ӹ�0.2��
           				
           				//��� FOLT ->>>  INT
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
      OSTimeDly(OS_TICKS_PER_SEC/500);  //������ʱ

		}
}
/////////////////////////////


void ProcessKey(uchar kb,uchar kc)
{
	

			if(kb&BIT0)
					{//���������
						
						
						
						if(kb!=kc)
							{
						//		StepMotRun1(  500);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
						//		StepMotRun2( 500);//MOTOR_FORWORD_STEP);//MOTOR_TEST_STEP);
						


							}
					}
		if(kb&BIT1)
					{//���������  ����
						
						
						if(kb!=kc)
							{
								
					//	StepMotRun1(-500);//MOTOR_BACK_STEP);
					//	StepMotRun2(-500);//MOTOR_TEST_STEP);		

	
							}
					}
					
		if(kb&BIT2)
					{//�ز������
						
						StepMotStop1();
						
						StepMotStop2();
								
						

					}				
										
}
void TaskKey(void * pdata)
{
//INT8U err;
 
	uchar ka,kb,kc,kd;

	pdata = pdata;     // ������뾯��	   
	FlagKey=0;
	ka=0;
	kb=0;
	kc=0;
	kd=0;


		for(;;)
						{
						//OS_ENTER_CRITICAL();
						//OS_EXIT_CRITICAL();
						OSTimeDly(OS_TICKS_PER_SEC/20);	    //��ʱ0.05��		
						
					if(InPin_K3)
							{//û�а���
								ka&=(~BIT0);		//

							}
					else {
								ka|=BIT0;

								}

					if(InPin_K4)
							{//û�а���
								ka&=(~BIT1);		//
							}
					else {
								ka|=BIT1;
								}

					if(InPin_K5)
							{//û�а���
								ka&=(~BIT2);		//
							}
					else {
								ka|=BIT2;
								}		
								
											
						if(kb==ka)
							{//�˲�kb
								
								ProcessKey(kb,kc);

								if(kc!=kb)
										{//�б仯
											kd=kc^kb;		//�仯
										EventTimeLed=4;
										}
								kc=kb;			//����ֵ	
							}
						else{//�˲�kb
								kb=ka;
							}
							
		
				}	
}



////////////////////////////////////////////////
void TaskInput1(void * pdata)
{
//INT8U err;
 
 
	pdata = pdata;                          	 	// ������뾯��	   


	for(;;)
				{
				//OS_ENTER_CRITICAL();
				//OS_EXIT_CRITICAL();


					
			OSTimeDly(OS_TICKS_PER_SEC/20);	    //��ʱ0.05��		

	
					}
}
///////////////////////////









/////////////////



//////////////






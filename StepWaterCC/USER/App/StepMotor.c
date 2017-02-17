/**
  ******************************************************************************
  * @file    StepMotor.c
  * @author  Liguowei  厉国伟
  * @version V0.0.1
  * @date    2/15/2017
  * @brief   This is Steper Motor Driver functions.
  ******************************************************************************
*/

#include "includes.h"

struct step_motor StepMot1;
struct step_motor StepMot2;
struct step_motor StepMot3;
struct step_motor StepMot4;
struct step_motor StepMot5;
struct step_motor StepMot6;

unsigned int RelativeOrigin=2104;    //相对坐标原点


/////////////////////////////////////////////
//CONFIG_HD_TIM
//void TIM2_IRQHandler(void)
void StepMotInit(void)
{
//步进电机脉冲输出初始化

StepMot1.Position16=0;  //步进电机脉冲表示绝对位置，带符号
ClrBitEN1;
ClrBitCW1;
StepMot1.PulseCircleSet=20;//20;		//20*50us  = 0.001s
StepMot1.PulseCircleCount=0;//StepMot1.PulseCircleSet;
StepMot1.PulseCount=0;
StepMot1.Enable=0;
StepMot1.ClkStatus=0;
ClrBitCLK1;
StepMot1.Direction=MOTOR_STATUS_OFF;  //方向
/////////////////////////
StepMot2.Position16=0;  //步进电机脉冲表示绝对位置，带符号
ClrBitEN2;
ClrBitCW2;
StepMot2.PulseCircleSet=20;//20;		//20*50us  = 0.001s
StepMot2.PulseCircleCount=0;//StepMot1.PulseCircleSet;
StepMot2.PulseCount=0;
StepMot2.Enable=0;
StepMot2.ClkStatus=0;
ClrBitCLK2;
StepMot2.Direction=MOTOR_STATUS_OFF;  //方向
/////////////////////////
StepMot3.Position16=0;  //步进电机脉冲表示绝对位置，带符号
ClrBitEN3;
ClrBitCW3;
StepMot3.PulseCircleSet=20;//20;		//20*50us  = 0.001s
StepMot3.PulseCircleCount=0;//StepMot1.PulseCircleSet;
StepMot3.PulseCount=0;
StepMot3.Enable=0;
StepMot3.ClkStatus=0;
ClrBitCLK3;
StepMot3.Direction=MOTOR_STATUS_OFF;  //方向
/////////////////////////
StepMot4.Position16=0;  //步进电机脉冲表示绝对位置，带符号
ClrBitEN4;
ClrBitCW4;
StepMot4.PulseCircleSet=20;//20;		//20*50us  = 0.001s
StepMot4.PulseCircleCount=0;//StepMot1.PulseCircleSet;
StepMot4.PulseCount=0;
StepMot4.Enable=0;
StepMot4.ClkStatus=0;
ClrBitCLK4;
StepMot4.Direction=MOTOR_STATUS_OFF;  //方向
/////////////////////////
StepMot5.Position16=0;  //步进电机脉冲表示绝对位置，带符号
ClrBitEN5;
ClrBitCW5;
StepMot5.PulseCircleSet=20;//20;		//20*50us  = 0.001s
StepMot5.PulseCircleCount=0;//StepMot1.PulseCircleSet;
StepMot5.PulseCount=0;
StepMot5.Enable=0;
StepMot5.ClkStatus=0;
ClrBitCLK5;
StepMot5.Direction=MOTOR_STATUS_OFF;  //方向
/////////////////////////
StepMot6.Position16=0;  //步进电机脉冲表示绝对位置，带符号
ClrBitEN6;
ClrBitCW6;
StepMot6.PulseCircleSet=20;//20;		//20*50us  = 0.001s
StepMot6.PulseCircleCount=0;//StepMot1.PulseCircleSet;
StepMot6.PulseCount=0;
StepMot6.Enable=0;
StepMot6.ClkStatus=0;
ClrBitCLK6;
StepMot6.Direction=MOTOR_STATUS_OFF;  //方向


#if CONFIG_HD_TIM
	Init_Tim3 ();		//步进电机脉冲输出用
#endif

}
//////////////////////////////////////////////

void StepMoterPulseHandler(void)
{		
		if((StepMot1.Enable)&&(StepMot1.PulseCount>0))
			{
				if(StepMot1.PulseCircleCount>0)//   脉冲输出速度控制
					{
						StepMot1.PulseCircleCount--;
					}
				else{
						StepMot1.PulseCircleCount=StepMot1.PulseCircleSet;//   脉冲输出速度控制
						
						if(StepMot1.ClkStatus==0)
							{
								StepMot1.ClkStatus=1;
								SetBitCLK1;
							}
						else{
								StepMot1.ClkStatus=0;
								ClrBitCLK1;
								StepMot1.PulseCount--;
								
								if(StepMot1.Direction==MOTOR_STATUS_FORWORD)		//前
											{
												StepMot1.Position16++;
											}
								else{
											StepMot1.Position16--;
											}
								}	
							
						
						}
			}

	////////////////	
		if((StepMot2.Enable)&&(StepMot2.PulseCount>0))
				{
					if(StepMot2.PulseCircleCount>0)//   脉冲输出速度控制
						{
							StepMot2.PulseCircleCount--;
						}
					else{
							StepMot2.PulseCircleCount=StepMot2.PulseCircleSet;//   脉冲输出速度控制
							
									
										if(StepMot2.ClkStatus==0)
											{
												StepMot2.ClkStatus=1;
												SetBitCLK2;
												//Led_Status_On;
											}
										else{
												StepMot2.ClkStatus=0;
												ClrBitCLK2;
												//Led_Status_Off;
												StepMot2.PulseCount--;
												if(StepMot2.Direction==MOTOR_STATUS_FORWORD)		//前  //方向
														{
															StepMot2.Position16++;
														}
												else{
															StepMot2.Position16--;
															}
												}	
							
							}
				}	
  ///////////////		
		if((StepMot3.Enable)&&(StepMot3.PulseCount>0))
				{
					if(StepMot3.PulseCircleCount>0)//   脉冲输出速度控制
						{
							StepMot3.PulseCircleCount--;
						}
					else{
							StepMot3.PulseCircleCount=StepMot3.PulseCircleSet;//   脉冲输出速度控制
							
									
										if(StepMot3.ClkStatus==0)
											{
												StepMot3.ClkStatus=1;
												SetBitCLK3;
												//Led_Status_On;
											}
										else{
												StepMot3.ClkStatus=0;
												ClrBitCLK3;
												//Led_Status_Off;
												StepMot3.PulseCount--;
												if(StepMot3.Direction==MOTOR_STATUS_FORWORD)		//前  //方向
														{
															StepMot3.Position16++;
														}
												else{
															StepMot3.Position16--;
															}
												}	
							
							}
				}				
  ///////////////				
		if((StepMot4.Enable)&&(StepMot4.PulseCount>0))
				{
					if(StepMot4.PulseCircleCount>0)//   脉冲输出速度控制
						{
							StepMot4.PulseCircleCount--;
						}
					else{
							StepMot4.PulseCircleCount=StepMot4.PulseCircleSet;//   脉冲输出速度控制
							
									
										if(StepMot4.ClkStatus==0)
											{
												StepMot4.ClkStatus=1;
												SetBitCLK4;
												//Led_Status_On;
											}
										else{
												StepMot4.ClkStatus=0;
												ClrBitCLK4;
												//Led_Status_Off;
												StepMot4.PulseCount--;
												if(StepMot4.Direction==MOTOR_STATUS_FORWORD)		//前  //方向
														{
															StepMot4.Position16++;
														}
												else{
															StepMot4.Position16--;
															}
												}	
							
							}
				}
  ///////////////
		if((StepMot5.Enable)&&(StepMot5.PulseCount>0))
				{
					if(StepMot5.PulseCircleCount>0)//   脉冲输出速度控制
						{
							StepMot5.PulseCircleCount--;
						}
					else{
							StepMot5.PulseCircleCount=StepMot5.PulseCircleSet;//   脉冲输出速度控制
							
									
										if(StepMot5.ClkStatus==0)
											{
												StepMot5.ClkStatus=1;
												SetBitCLK5;
												//Led_Status_On;
											}
										else{
												StepMot5.ClkStatus=0;
												ClrBitCLK5;
												//Led_Status_Off;
												StepMot5.PulseCount--;
												if(StepMot5.Direction==MOTOR_STATUS_FORWORD)		//前  //方向
														{
															StepMot5.Position16++;
														}
												else{
															StepMot5.Position16--;
															}
												}	
							
							}
				}							
  ///////////////
		if((StepMot6.Enable)&&(StepMot6.PulseCount>0))
				{
					if(StepMot6.PulseCircleCount>0)//   脉冲输出速度控制
						{
							StepMot6.PulseCircleCount--;
						}
					else{
							StepMot6.PulseCircleCount=StepMot6.PulseCircleSet;//   脉冲输出速度控制
							
									
										if(StepMot6.ClkStatus==0)
											{
												StepMot6.ClkStatus=1;
												SetBitCLK6;
												//Led_Status_On;
											}
										else{
												StepMot6.ClkStatus=0;
												ClrBitCLK6;
												//Led_Status_Off;
												StepMot6.PulseCount--;
												if(StepMot6.Direction==MOTOR_STATUS_FORWORD)		//前  //方向
														{
															StepMot6.Position16++;
														}
												else{
															StepMot6.Position16--;
															}
												}	
							
							}
				}


}

////////////////////////////////////////////////////////
//关步进电机1#
void StepMotStop1(void)
{

ClrBitEN1;
ClrBitCW1;

StepMot1.PulseCircleCount=0;//StepMot1.PulseCircleSet;

StepMot1.PulseCount=0;
StepMot1.Enable=0;
StepMot1.ClkStatus=0;
ClrBitCLK1;

StepMot1.Direction=MOTOR_STATUS_OFF;  //方向
}


//关步进电机2#
void StepMotStop2(void)
{
ClrBitEN2;
ClrBitCW2;

StepMot2.PulseCircleCount=0;//StepMot1.PulseCircleSet;

StepMot2.PulseCount=0;
StepMot2.Enable=0;
StepMot2.ClkStatus=0;
ClrBitCLK2;

StepMot2.Direction=MOTOR_STATUS_OFF;  //方向
}




//开步进电机1#
//step_count>0  --->  向前 MOTOR_STATUS_FORWORD   前进到极限，关闭阀门
//step_count<0  --->  向后 MOTOR_STATUS_BACKWORD  退到最后，全开阀门
//2400在8分频时是300步步进电机的极限行程
void StepMotRun1(signed int step_count)
{
	unsigned  int  step16;
	
StepMot1.Enable=0;
//-- ClrBitEN1;
ClrBitCLK1;


	if(step_count>0)//cw==MOTOR_STATUS_FORWORD)		//前
		{
			StepMot1.Direction=MOTOR_STATUS_FORWORD;  //方向
		ClrBitCW1;
		step16=(unsigned int)step_count;
		}
	else{
		StepMot1.Direction=MOTOR_STATUS_BACKWORD;
			SetBitCW1;
			step16=(unsigned int)(-step_count);
			}
			
//OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒

StepMot1.PulseCircleCount=0;//StepMot1.PulseCircleSet;


StepMot1.PulseCount=step16;
StepMot1.ClkStatus=0;
SetBitEN1;

//OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒

StepMot1.Enable=1;
}

//开步进电机2#
//step_count>0  --->  向前 MOTOR_STATUS_FORWORD   前进到极限，关闭阀门
//step_count<0  --->  向后 MOTOR_STATUS_BACKWORD  退到最后，全开阀门
//2400在8分频时是300步步进电机的极限行程
void StepMotRun2(signed int step_count)
{
unsigned int  step16;
StepMot2.Enable=0;
//-- ClrBitEN2;
ClrBitCLK2;
	if(step_count>0)		//前
		{
			StepMot2.Direction=MOTOR_STATUS_FORWORD;  //方向
		ClrBitCW2;
		step16=(unsigned int)step_count;
		}
	else{
		StepMot2.Direction=MOTOR_STATUS_BACKWORD;  //方向
			SetBitCW2;
			step16=(unsigned int)(-step_count);	
			}
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot2.PulseCircleCount=0;//StepMot2.PulseCircleSet;
StepMot2.PulseCount=step16;
StepMot2.ClkStatus=0;
SetBitEN2;
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot2.Enable=1;
}

//开步进电机3#
//step_count>0  --->  向前 MOTOR_STATUS_FORWORD   前进到极限，关闭阀门
//step_count<0  --->  向后 MOTOR_STATUS_BACKWORD  退到最后，全开阀门
//2400在8分频时是300步步进电机的极限行程
void StepMotRun3(signed int step_count)
{
unsigned int  step16;
StepMot3.Enable=0;
//-- ClrBitEN3;
ClrBitCLK3;
	if(step_count>0)		//前
		{
			StepMot3.Direction=MOTOR_STATUS_FORWORD;  //方向
		ClrBitCW3;
		step16=(unsigned int)step_count;
		}
	else{
		StepMot3.Direction=MOTOR_STATUS_BACKWORD;  //方向
			SetBitCW3;
			step16=(unsigned int)(-step_count);	
			}
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot3.PulseCircleCount=0;//StepMot3.PulseCircleSet;
StepMot3.PulseCount=step16;
StepMot3.ClkStatus=0;
SetBitEN3;
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot3.Enable=1;
}


//开步进电机4#
//step_count>0  --->  向前 MOTOR_STATUS_FORWORD   前进到极限，关闭阀门
//step_count<0  --->  向后 MOTOR_STATUS_BACKWORD  退到最后，全开阀门
//2400在8分频时是300步步进电机的极限行程
void StepMotRun4(signed int step_count)
{
unsigned int  step16;
StepMot4.Enable=0;
//-- ClrBitEN4;
ClrBitCLK4;
	if(step_count>0)		//前
		{
			StepMot4.Direction=MOTOR_STATUS_FORWORD;  //方向
		ClrBitCW4;
		step16=(unsigned int)step_count;
		}
	else{
		StepMot4.Direction=MOTOR_STATUS_BACKWORD;  //方向
			SetBitCW4;
			step16=(unsigned int)(-step_count);	
			}
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot4.PulseCircleCount=0;//StepMot4.PulseCircleSet;
StepMot4.PulseCount=step16;
StepMot4.ClkStatus=0;
SetBitEN4;
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot4.Enable=1;
}

//开步进电机5#
//step_count>0  --->  向前 MOTOR_STATUS_FORWORD   前进到极限，关闭阀门
//step_count<0  --->  向后 MOTOR_STATUS_BACKWORD  退到最后，全开阀门
//2400在8分频时是300步步进电机的极限行程
void StepMotRun5(signed int step_count)
{
unsigned int  step16;
StepMot5.Enable=0;
//-- ClrBitEN5;
ClrBitCLK5;
	if(step_count>0)		//前
		{
			StepMot5.Direction=MOTOR_STATUS_FORWORD;  //方向
		ClrBitCW5;
		step16=(unsigned int)step_count;
		}
	else{
		StepMot5.Direction=MOTOR_STATUS_BACKWORD;  //方向
			SetBitCW5;
			step16=(unsigned int)(-step_count);	
			}
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot5.PulseCircleCount=0;//StepMot5.PulseCircleSet;
StepMot5.PulseCount=step16;
StepMot5.ClkStatus=0;
SetBitEN5;
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot5.Enable=1;
}

//开步进电机6#
//step_count>0  --->  向前 MOTOR_STATUS_FORWORD   前进到极限，关闭阀门
//step_count<0  --->  向后 MOTOR_STATUS_BACKWORD  退到最后，全开阀门
//2400在8分频时是300步步进电机的极限行程
void StepMotRun6(signed int step_count)
{
unsigned int  step16;
StepMot6.Enable=0;
//-- ClrBitEN6;
ClrBitCLK6;
	if(step_count>0)		//前
		{
			StepMot6.Direction=MOTOR_STATUS_FORWORD;  //方向
		ClrBitCW6;
		step16=(unsigned int)step_count;
		}
	else{
		StepMot6.Direction=MOTOR_STATUS_BACKWORD;  //方向
			SetBitCW6;
			step16=(unsigned int)(-step_count);	
			}
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot6.PulseCircleCount=0;//StepMot6.PulseCircleSet;
StepMot6.PulseCount=step16;
StepMot6.ClkStatus=0;
SetBitEN6;
//OSTimeDly(OS_TICKS_PER_SEC/10);	    //延时0.05秒
StepMot6.Enable=1;
}

//步进电机脉冲速度调整
//void StepMotAdjSpeed1(uint32 cycle)
//{
//if(	cycle>60000)cycle=60000;	//脉冲周期最长3+3秒
//	
////StepMot1.PulseCircleSet=2000;		//2000*50us  = 0.1s
//StepMot1.PulseCircleSet=cycle;
//}
//
//void StepMotAdjSpeed2(uint32 cycle)
//{
//	if(	cycle>60000)cycle=60000;	//脉冲周期最长3+3秒
//		
////StepMot2.PulseCircleSet=2000;		//2000*50us  = 0.1s
//StepMot2.PulseCircleSet=cycle;
//}

////步进电机方向设定 1通电
//void StepMotDirection1(uchar cw)
//{
//	
//	StepMot1.Direction=cw;  //方向
//	if(cw==MOTOR_STATUS_FORWORD)		//前
//		{
//
//		ClrBitCW1;
//		
//		}
//	else if(cw==MOTOR_STATUS_BACKWORD)		//
//			{
//			SetBitCW1;
//				
//			}
//	
//			
//}
////步进电机方向设定 1通电
//void StepMotDirection2(uchar cw)
//{
//	StepMot2.Direction=cw;  //方向
//	if(cw==MOTOR_STATUS_FORWORD)		//前
//		{
//
//		ClrBitCW2;
//		
//		}
//	else if(cw==MOTOR_STATUS_BACKWORD)		//
//		   {
//			SetBitCW2;
//				
//			}
//}

////开步进电机1#
//void StepMotStart1(uchar cw,uint32 step_count)
//{
//StepMot1.Enable=0;
//ClrBitEN1;
//ClrBitCLK1;
//
//StepMot1.Direction=cw;  //方向
//	if(cw==MOTOR_STATUS_FORWORD)		//前
//		{
//		ClrBitCW1;
//		}
//	else{
//			SetBitCW1;
//			}
//			
//OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒
//
//StepMot1.PulseCircleCount=0;//StepMot1.PulseCircleSet;
//
//StepMot1.PulseCount=step_count;
//StepMot1.ClkStatus=0;
//SetBitEN1;
//
//OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒
//
//StepMot1.Enable=1;
//}
//
////开步进电机2#
//void StepMotStart2(uchar cw,uint32 step_count)
//{
//StepMot2.Enable=0;
//ClrBitEN2;
//ClrBitCLK2;
//
//StepMot2.Direction=cw;  //方向
//	if(cw==MOTOR_STATUS_FORWORD)		//前
//		{
//		ClrBitCW2;
//		}
//	else{
//			SetBitCW2;
//			}
//			
//OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒
//
//StepMot2.PulseCircleCount=0;//StepMot2.PulseCircleSet;
//
//StepMot2.PulseCount=step_count;
//StepMot2.ClkStatus=0;
//SetBitEN2;
//
//OSTimeDly(OS_TICKS_PER_SEC/20);	    //延时0.05秒
//
//StepMot2.Enable=1;
//}

//



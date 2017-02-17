    
#include "includes.h"

#if CONFIG_PWM    
    
    /*
    TIM1 TIM8是高级定时器
    高级定时器输入时钟72MHZ,利用TIM1产生3个互补输出的信号，频率1HZ,占空比25%，50%,75%,分辨率1/10000，并加入死区时间。
    
    预分配寄存器的值=72000000/10000  -1 =7199
    比较寄存器1的值=10000x25%=2500
		比较寄存器2的值=10000x50%=5000
		比较寄存器3的值=10000x75%=7500    
    */
    
void PWM_Configuration(void)
{
    
    TIM_TimeBaseInitTypeDef  		TIM_TimeBaseStructure;
		TIM_OCInitTypeDef						TIM_OCInitStructure;
		TIM_BDTRInitTypeDef					TIM_BDTRInitStructure;


    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		
    //TIM_TimeBaseStructure.TIM_Prescaler     = 7199;		//预分配寄存器
   	TIM_TimeBaseStructure.TIM_Prescaler     = 72000000/4000/1000-1;			//预分配寄存器   4khz    
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Down;//TIM_CounterMode_Up;
    //TIM_TimeBaseStructure.TIM_Period        = 10000;
    TIM_TimeBaseStructure.TIM_Period        = 1000;		//  1/1000
    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;


    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    

		//设置通道1,2,3运行在PWM模式
		TIM_OCInitStructure.TIM_OCMode				=	TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState		=	TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState	=	TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse					=	200;//2500;
    TIM_OCInitStructure.TIM_OCPolarity		=	TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity		=	TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState		=	TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState	=	TIM_OCNIdleState_Set;
    
    TIM_OC1Init(TIM1,&TIM_OCInitStructure);
    
//    TIM_OCInitStructure.TIM_Pulse					=	5000;
//    TIM_OC2Init(TIM1,&TIM_OCInitStructure);
//    
//    TIM_OCInitStructure.TIM_Pulse					=	7500;
//    TIM_OC3Init(TIM1,&TIM_OCInitStructure);   
    
    
    //输出允许，刹车信号允许，设置死区时间，锁定配置
    TIM_BDTRInitStructure.TIM_OSSRState		=		TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState		=		TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel		=		TIM_LOCKLevel_1;
    TIM_BDTRInitStructure.TIM_DeadTime			=		20;		//死区时间2us
    //TIM_BDTRInitStructure.TIM_Break				=		TIM_Break_Enable;
    //TIM_BDTRInitStructure.TIM_BreakPolarity		=		TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput	=		TIM_AutomaticOutput_Enable;
    
    TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
    
//    TIM_SetCounter(TIM2, 0);
//    TIM_PrescalerConfig(TIM2, 256, TIM_PSCReloadMode_Immediate);
		
		//TIM1允许计数
    TIM_Cmd(TIM1, ENABLE);
    //控制输出允许
    TIM_CtrlPWMOutputs(TIM1,ENABLE);
    
}

////////////////////////    

void GPIO_Configuration_PWM(void)
{
		GPIO_InitTypeDef	GPIO_InitStructure;
		
		//GPIOA通道1,2,3			PA8  PA9  PA10
		GPIO_InitStructure.GPIO_Pin		=		GPIO_Pin_8;// | GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode	=		GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed	=		GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		//GPIOB通道1N,2N,3N		PB13 PB14 PB15
		GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_13;// | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		//GPIOB刹车管脚   PB12
		GPIO_InitStructure.GPIO_Pin		=		GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode	=		GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
}

#endif


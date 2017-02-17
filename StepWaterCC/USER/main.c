/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.blkeji.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The UCOSII application function
**
**--------------------------------------------------------------------------------------------------------

** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];



/* Private function prototypes -----------------------------------------------*/
#if (OS_VIEW_MODULE == DEF_ENABLED)
extern void  App_OSViewTaskCreate (void);
#endif

static  void  App_TaskCreate		(void);
static  void  App_TaskStart			(void		*p_arg);  
extern  void  App_BlinkTaskCreate   (void);
static  void  GPIO_Configuration    (void);



/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/
INT32S main (void)
{
    CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */

IspMarkLoad();


	/* Note:  由于使用UCOS, 在OS运行之前运行,注意别使能任何中断. */
	CPU_IntDis();                    /* Disable all ints until we are ready to accept them.  */

    OSInit();                        /* Initialize "uC/OS-II, The Real-Time Kernel".         */
	

	os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	
#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

	OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

	return (0);
}


/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/	  
static  void  App_TaskStart (void *p_arg)
{   
	(void)p_arg;
	
	/***************  Init hardware ***************/
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		//中断分组方案
GPIO_Configuration();
InitGpio();
    
    OS_CPU_SysTickInit();                                    /* Initialize the SysTick.                              */


//	InitPara();		//针对不保存的变量
//	FirstPara();		//ParaBlockSave();
//	InitPara2();
//
//	if( ParaBlockLoad()==0 )
//		{
////			if(g_sUartFormat.current==UART_USER_FORMAT_TEST)	//测试方式时可以自动更新设置值
////				TimeSaveParaBlock=20;	//20秒后保存
//		}
//		

//StepMotInit();

//#if CONFIG_HD_TIM
//	Init_Tim3 ();		//步进电机脉冲输出用
//#endif


	#if CONFIG_UART1	
		InitUSART1(9600,0);
		SendText_UART1("UART1 OK\r\n");
	#endif
	
	#if CONFIG_UART2
		InitUSART2(115200,0);//DefaultModemBaud;
		SendText_UART2("UART2 OK\r\n");
	#endif	
	
	#if CONFIG_UART3	
InitUSART3(9600,0);
SendText_UART3("UART3 OK\r\n");
	#endif	

	#if CONFIG_UART4
	InitUART4(9600,0);
	SendText_UART4("UART4 OK\r\n");
	#endif	

	#if CONFIG_UART5
	InitUART5(9600,0);
	SendText_UART5("UART5 OK\r\n");
	#endif	

	//StartLog();












#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                            /* Determine CPU capacity.                              */
#endif

    App_TaskCreate();                                        /* Create application tasks.                            */
  OSTimeDlyHMSM(0, 0, 0, 100);                                      /* Create application tasks.                            */
	//AllTaskCreate();
	for(;;)
   	{

      	OSTimeDlyHMSM(0, 1, 0, 0);							 /* Delay One minute */
    }	
}


/*
*********************************************************************************************************
*                                            App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskStart().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
#if (OS_VIEW_MODULE == DEF_ENABLED)
	App_OSViewTaskCreate();
#endif	
	
	App_BlinkTaskCreate();	

}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GPIO_Configuration(void)
{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE); 						 
///**
// *	LED1 -> PE0   LED2 -> PE1
// */					 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//  GPIO_Init(GPIOE, &GPIO_InitStructure);

 
  
  
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

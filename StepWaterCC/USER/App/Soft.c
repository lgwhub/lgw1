/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.blkeji.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_Blink.c
** Descriptions:            The uctsk_Blink application function
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




//extern OS_EVENT *OSSemblinkOut2;
//extern OS_EVENT *OSSemblinkOut3;
extern OS_EVENT *OSSemUart1;
//extern OS_EVENT *OSSemUart2;
extern OS_EVENT *OSSemTimePid_StepMotor;
extern OS_EVENT *OSSemTimePid_Heat;


/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskBlinkStk[APP_TASK_BLINK_STK_SIZE];

static  OS_STK         MyTASKStk[MyTASKStk_SIZE];
static  OS_STK         TaskRecvStk[TaskRecvStk_SIZE];
static  OS_STK         TaskTsStk[TaskTsStk_SIZE];
static  OS_STK         TaskInput1Stk[TaskInput1Stk_SIZE];
static  OS_STK         TaskSyncUpStk[TaskSyncUpStk_SIZE];

static  OS_STK         TaskVirPwmStk[TaskVirPwmStk_SIZE];
static  OS_STK         TaskModbusStk[TaskModbusStk_SIZE];
static  OS_STK         TaskStepMotorStk[TaskStepMotorStk_SIZE];
static  OS_STK         TaskTimePrStk[TaskTimePrStk_SIZE];
static  OS_STK         TaskHeatStk[TaskHeatStk_SIZE];
static  OS_STK         TaskStatusStk[ TaskStatusStk_SIZE];


/* Private function prototypes -----------------------------------------------*/
static void uctsk_Blink (void);
static void MyTASK (void);
//extern void TaskRecv(void);
//extern void TaskTs(void);
//extern void TaskInput1(void * pdata);
 
void  App_BlinkTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */
	
#if CONFIG_TEST_LED_BLINK
	os_err = OSTaskCreate((void (*)(void *)) uctsk_Blink,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskBlinkStk[APP_TASK_BLINK_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_BLINK_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_BLINK_PRIO, "Task LED Blink", &os_err);
	#endif
#endif



//OSSemblinkOut2 = OSSemCreate(0);
//OSSemblinkOut3 = OSSemCreate(0);

OSSemUart1 = OSSemCreate(1);
//OSSemUart2 = OSSemCreate(1);
			
OSSemTimePid_StepMotor = OSSemCreate(0);
OSSemTimePid_Heat = OSSemCreate(0);
//MyStk
//	os_err = OSTaskCreate((void (*)(void *)) MyTASK,				
//                          (void          * ) 0,							
//                          (OS_STK        * )&MyTASKStk[MyTASKStk_SIZE - 1],		
//                          (INT8U           ) MyTASK_PRIO  );	

    os_err = OSTaskCreateExt((void (*)(void *)) MyTASK,
                             (void          * ) 0,
                             (OS_STK        * )&MyTASKStk[MyTASKStk_SIZE - 1],
                             (INT8U           ) MyTASK_PRIO,
                             (INT16U          ) MyTASK_PRIO,
                             (OS_STK        * )&MyTASKStk[0],
                             (INT32U          ) MyTASKStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

    os_err = OSTaskCreateExt((void (*)(void *)) TaskRecv,
                             (void          * ) 0,
                             (OS_STK        * )&TaskRecvStk[TaskRecvStk_SIZE - 1],
                             (INT8U           ) TaskRecv_PRIO,
                             (INT16U          ) TaskRecv_PRIO,
                             (OS_STK        * )&TaskRecvStk[0],
                             (INT32U          ) TaskRecvStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));



    os_err = OSTaskCreateExt((void (*)(void *)) TaskTs,
                             (void          * ) 0,
                             (OS_STK        * )&TaskTsStk[TaskTsStk_SIZE - 1],
                             (INT8U           ) TaskTs_PRIO,
                             (INT16U          ) TaskTs_PRIO,
                             (OS_STK        * )&TaskTsStk[0],
                             (INT32U          ) TaskTsStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    
    os_err = OSTaskCreateExt((void (*)(void *)) TaskSyncUp,
                             (void          * ) 0,
                             (OS_STK        * )&TaskSyncUpStk[TaskSyncUpStk_SIZE - 1],
                             (INT8U           ) TaskSyncUp_PRIO,
                             (INT16U          ) TaskSyncUp_PRIO,
                             (OS_STK        * )&TaskSyncUpStk[0],
                             (INT32U          ) TaskSyncUpStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));


    os_err = OSTaskCreateExt((void (*)(void *)) TaskInput1,
                             (void          * ) 0,
                             (OS_STK        * )&TaskInput1Stk[TaskInput1Stk_SIZE - 1],
                             (INT8U           ) TaskInput1_PRIO,
                             (INT16U          ) TaskInput1_PRIO,
                             (OS_STK        * )&TaskInput1Stk[0],
                             (INT32U          ) TaskInput1Stk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));


    os_err = OSTaskCreateExt((void (*)(void *)) TaskVirPwm,
                             (void          * ) 0,
                             (OS_STK        * )&TaskVirPwmStk[TaskVirPwmStk_SIZE - 1],
                             (INT8U           ) TaskVirPwm_PRIO,
                             (INT16U          ) TaskVirPwm_PRIO,
                             (OS_STK        * )&TaskVirPwmStk[0],
                             (INT32U          ) TaskVirPwmStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

    os_err = OSTaskCreateExt((void (*)(void *)) TaskModbus,
                             (void          * ) 0,
                             (OS_STK        * )&TaskModbusStk[TaskModbusStk_SIZE - 1],
                             (INT8U           ) TaskModbus_PRIO,
                             (INT16U          ) TaskModbus_PRIO,
                             (OS_STK        * )&TaskModbusStk[0],
                             (INT32U          ) TaskModbusStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

    os_err = OSTaskCreateExt((void (*)(void *)) TaskStepMotor,
                             (void          * ) 0,
                             (OS_STK        * )&TaskStepMotorStk[TaskStepMotorStk_SIZE - 1],
                             (INT8U           ) TaskStepMotor_PRIO,
                             (INT16U          ) TaskStepMotor_PRIO,
                             (OS_STK        * )&TaskStepMotorStk[0],
                             (INT32U          ) TaskStepMotorStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
                             
    os_err = OSTaskCreateExt((void (*)(void *)) TaskTimePr,
                             (void          * ) 0,
                             (OS_STK        * )&TaskTimePrStk[TaskTimePrStk_SIZE - 1],
                             (INT8U           ) TaskTimePr_PRIO,
                             (INT16U          ) TaskTimePr_PRIO,
                             (OS_STK        * )&TaskTimePrStk[0],
                             (INT32U          ) TaskTimePrStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));                         
                             
     os_err = OSTaskCreateExt((void (*)(void *)) TaskHeat,
                             (void          * ) 0,
                             (OS_STK        * )&TaskHeatStk[TaskHeatStk_SIZE - 1],
                             (INT8U           ) TaskHeat_PRIO,
                             (INT16U          ) TaskHeat_PRIO,
                             (OS_STK        * )&TaskHeatStk[0],
                             (INT32U          ) TaskHeatStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));                                
                             
      os_err = OSTaskCreateExt((void (*)(void *)) TaskStatus,
                             (void          * ) 0,
                             (OS_STK        * )&TaskStatusStk[TaskStatusStk_SIZE - 1],
                             (INT8U           ) TaskStatus_PRIO,
                             (INT16U          ) TaskStatus_PRIO,
                             (OS_STK        * )&TaskStatusStk[0],
                             (INT32U          ) TaskStatusStk_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));                               
                             
                             
	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(MyTASK_PRIO, "MyTASK", &os_err);

    	OSTaskNameSet(TaskTs_PRIO, "TaskTs", &os_err);
			OSTaskNameSet(TaskRecv_PRIO, "TaskRecv", &os_err);
    	OSTaskNameSet(TaskSyncUp_PRIO, "TaskSyncUp", &os_err);			
    	OSTaskNameSet(TaskInput1_PRIO, "TaskInput1", &os_err);
    	
    	OSTaskNameSet(TaskVirPwm_PRIO, "TaskVirPwm", &os_err);
    	
    	OSTaskNameSet(TaskModbus_PRIO, "TaskModbus", &os_err);
    	OSTaskNameSet(TaskStepMotor_PRIO, "TaskStepMotor", &os_err);
     	OSTaskNameSet(TaskTimePr_PRIO, "TaskTimePr", &os_err);
    	
    	OSTaskNameSet(TaskHeat_PRIO, "TaskHeat", &os_err);
    	OSTaskNameSet(TaskStatus_PRIO, "TaskStatus", &os_err);   	
    	
    	
	#endif
}
/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
//////////////
static void MyTASK (void)
{
	
	for(;;)
	{
		
		
		OSTimeDlyHMSM(0, 0,1, 100);	 /* 1 seconds  */
		//ClrPowJspOut1;
		//ClrResumMegnet;
		//GPIO_ResetBits(GPIOE , GPIO_Pin_1);
		//LED1_ON;
		//LED2_OFF;
		//GPIO_ResetBits(GPIOE,GPIO_Pin_1);
		//Delay(60000);
		OSTimeDlyHMSM(0, 0, 1, 100);
		//SetPowJspOut1;
		//SetResumMegnet;
		//LED2_ON;
		//LED1_OFF;
//GPIO_SetBits(GPIOE , GPIO_Pin_1);
	
	
	
		}
}


#if CONFIG_TEST_LED_BLINK
static void uctsk_Blink (void) {                 
  
   
   	for(;;)
   	{   
	    //Delay( rand()%0xfffff ); 	
	    #if CONFIG_TEST_LED_BLINK
//LED1_ON;	    
//LED2_ON;
//GPIO_SetBits(GPIOE , GPIO_Pin_0);
#endif
//OSTimeDly(OS_TICKS_PER_SEC);
OSTimeDlyHMSM(0, 0, 0,50);	//50ms
		//OSTimeDlyHMSM(0, 0, 1, 0);	 /* 1 seconds  */
	 	/* LED1-ON LED2-OFF */
	//	GPIO_SetBits(GPIOE , GPIO_Pin_0);
		//GPIO_ResetBits(GPIOE , GPIO_Pin_1);

	    //Delay( rand()%0xfffff ); 
	    #if CONFIG_TEST_LED_BLINK
//LED1_OFF;
//LED2_OFF;
//GPIO_ResetBits(GPIOE , GPIO_Pin_0);
#endif
//OSTimeDly(OS_TICKS_PER_SEC);
OSTimeDlyHMSM(0, 0, 0,50);	//50ms
      	
	    /* LED1-OFF LED2-ON */
		//GPIO_ResetBits(GPIOE , GPIO_Pin_0);
		//GPIO_SetBits(GPIOE , GPIO_Pin_1);
   }
}
#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

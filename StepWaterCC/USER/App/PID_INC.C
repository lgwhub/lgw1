
#include <includes.h>
//增量式PID

PidBufStruct StepPidBuf[MAX_TEMPRATURE_CHNALL];





//////////////////////////////////////////////////////////////////////

void PIDInitxxx (PidBufStruct *pp)                     //PID参数初始化，都置0

   {                                           

     memset ( pp,0,sizeof(PidBufStruct));


//memset()的函数， 它可以一字节一字节地把整个数组设置为一个指定的值。

// memset()函数在mem.h头文件中声明，它把数组的起始地址作为其第一个参数，

//第二个参数是设置数组每个字节的值，第三个参数是数组的长度(字节数，不是元素个数)。

//其函数原型为：　void *memset(void*，int，unsigned)；

//头文件<string.h>

   }






void PID_Inc_Calc( PidBufStruct *ch, float Error)
{

    float dError,pError;
    
    float total;
    
    //Error          =  pidch->SetPoint  - NowPoint   ;              
    
    pError   =     Error   -   ch->LastError;                             //比例偏差  e0-e1
    dError   =     pError  -   ( ch->LastError  -  ch->PreError );     		//微分偏差  e1-e2
    
    //更新
    ch->PreError  =    ch->LastError;              							  //上次偏差赋给上上次偏差  e2=e1
    ch->LastError =    Error;                 										//当前偏差赋给上次偏差    e1=e0
     
    
    ch->Px =   Coldw.PID_P1 *  pError;	//P部分Proportion
    ch->Ix =   Coldw.PID_I1 *  Error; 	//I部分Integral
    
    ///抗饱和///
    if( ( ch->Ix ) >  MAX_PID_INTEGRAL_1)
		    {
         ch->Ix  =   MAX_PID_INTEGRAL_1  ;
		    }
    else if( ( ch->Ix )  < (MIN_PID_INTEGRAL_1) )
		    {
        ch->Ix  =  MIN_PID_INTEGRAL_1  ;
		    }    
      
    ch->Dx =   Coldw.PID_D1 *  dError;	//D部分Derivative
   
   
    total     =  ( ch->Px + ch->Ix + ch->Dx);
 
 
    if(total < MIN_PID_INTEGRAL_1)
    	{   ch->Qx  =  (signed long int)MIN_PID_INTEGRAL_1;
    	}
    else if(total > MAX_PID_INTEGRAL_1 )
    	{
    		ch->Qx   = (signed long int)MAX_PID_INTEGRAL_1;
    	}
    else {
    	ch->Qx   =(signed long int) total;
         } 
    
	
}


//void PID_Inc_BufInit(PidBufStruct *pidch)
//{
////水冷增量PID
//
//
//PidBufStruct StepPidBuf[MAX_TEMPRATURE_CHNALL];
//}

void PID_Inc_ParaInit(void)
{
	//	    Coldw.PID_P1 = 2;                //水冷增量PID
//    Coldw.PID_I1 = 0.01;
//    Coldw.PID_D1 = 0.1;
	
}


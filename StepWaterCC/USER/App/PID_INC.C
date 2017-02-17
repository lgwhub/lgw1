
#include <includes.h>
//����ʽPID

PidBufStruct StepPidBuf[MAX_TEMPRATURE_CHNALL];





//////////////////////////////////////////////////////////////////////

void PIDInitxxx (PidBufStruct *pp)                     //PID������ʼ��������0

   {                                           

     memset ( pp,0,sizeof(PidBufStruct));


//memset()�ĺ����� ������һ�ֽ�һ�ֽڵذ�������������Ϊһ��ָ����ֵ��

// memset()������mem.hͷ�ļ��������������������ʼ��ַ��Ϊ���һ��������

//�ڶ�����������������ÿ���ֽڵ�ֵ������������������ĳ���(�ֽ���������Ԫ�ظ���)��

//�亯��ԭ��Ϊ����void *memset(void*��int��unsigned)��

//ͷ�ļ�<string.h>

   }






void PID_Inc_Calc( PidBufStruct *ch, float Error)
{

    float dError,pError;
    
    float total;
    
    //Error          =  pidch->SetPoint  - NowPoint   ;              
    
    pError   =     Error   -   ch->LastError;                             //����ƫ��  e0-e1
    dError   =     pError  -   ( ch->LastError  -  ch->PreError );     		//΢��ƫ��  e1-e2
    
    //����
    ch->PreError  =    ch->LastError;              							  //�ϴ�ƫ������ϴ�ƫ��  e2=e1
    ch->LastError =    Error;                 										//��ǰƫ����ϴ�ƫ��    e1=e0
     
    
    ch->Px =   Coldw.PID_P1 *  pError;	//P����Proportion
    ch->Ix =   Coldw.PID_I1 *  Error; 	//I����Integral
    
    ///������///
    if( ( ch->Ix ) >  MAX_PID_INTEGRAL_1)
		    {
         ch->Ix  =   MAX_PID_INTEGRAL_1  ;
		    }
    else if( ( ch->Ix )  < (MIN_PID_INTEGRAL_1) )
		    {
        ch->Ix  =  MIN_PID_INTEGRAL_1  ;
		    }    
      
    ch->Dx =   Coldw.PID_D1 *  dError;	//D����Derivative
   
   
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
////ˮ������PID
//
//
//PidBufStruct StepPidBuf[MAX_TEMPRATURE_CHNALL];
//}

void PID_Inc_ParaInit(void)
{
	//	    Coldw.PID_P1 = 2;                //ˮ������PID
//    Coldw.PID_I1 = 0.01;
//    Coldw.PID_D1 = 0.1;
	
}

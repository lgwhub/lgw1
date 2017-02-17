#ifndef    _PID_H   
#define    _PID_H


//ˮ��  ��������
//#define MAX_PID_INTEGRAL_1   100
//#define MIN_PID_INTEGRAL_1   (-100)

//2400��8��Ƶʱ��300����������ļ����г�   MOTOR_MAX_STEP == 2400
#define MAX_PID_INTEGRAL_1   (MOTOR_MAX_STEP/24)
#define MIN_PID_INTEGRAL_1   (-MAX_PID_INTEGRAL_1)


extern PidBufStruct StepPidBuf[MAX_TEMPRATURE_CHNALL];
/********************************************************
* PID���ƽṹ������
*********************************************************/
//typedef struct 
//{
//    float Proportion;          //��������
//    float Integral;            //���ֳ���
//    float Derivative;          //΢�ֳ���
//    float Max;    
//    float Min;    
//
//}PID_ParaStruct;
//
//
//
//
//typedef struct 
//{
//    float SetPoint;            //�趨Ŀ��
//    
//    float LastError;           //�ϴ�ƫ��
//    float PreError;            //���ϴ�ƫ��
//    float SumError;            //�ۻ�ƫ��
//
//
//
//    float Px;									//P����
//    float Ix;        					//I����  ��� Px+Ix+Dx
//    float Dx;        					//D����
//
//   
//    signed long int Qx;                 //���������
//    
//}PidBufStruct;  //��ʱ����
//


/********************************************************
* Function:    PID_INC_Calc
* Description: PID����
* Input:       pPIDxxx  ��Ҫ�����PID���ƽṹ��ָ��
*              NowPoint         ʵ�ʷ���ֵ
* Output:      float           PID������
* Calls:       ��
* Called by:   CtrlTemp
* Others:
*********************************************************/
//float

//void PID_Inc_BufInit(PidBufStruct *pidch);
void PID_Inc_ParaInit(void);
void PID_Inc_Calc( PidBufStruct *ch, float Error);
#endif


/**
  ******************************************************************************
  * @file    StepMotor.h
  * @author  Liguowei  厉国伟
  * @version V0.0.1
  * @date    2/15/2017
  * @brief   This is Steper Motor Driver functions.
  ******************************************************************************
*/


#ifndef _STEP_MOTOR_H
#define _STEP_MOTOR_H


#define MOTOR_STATUS_OFF				0
#define MOTOR_STATUS_FORWORD		1
#define MOTOR_STATUS_BACKWORD		2

//2400在8分频时是300步步进电机的极限行程
#define MOTOR_MAX_STEP   2400


//步进电机
struct step_motor{
signed  int Position16;   //步进电机脉冲表示绝对位置，带符号
unsigned int PulseCircleSet;		//2000*50us  = 0.1s   脉冲输出速度控制 设置值
unsigned int PulseCircleCount;//StepMot1.PulseCircleSet;   脉冲输出速度控制
unsigned int PulseCount;
unsigned char Enable;
unsigned char ClkStatus;

unsigned char Direction;  //方向  1先前  或者脉冲为正数

};

extern struct step_motor StepMot1;
extern struct step_motor StepMot2;
extern struct step_motor StepMot3;
extern struct step_motor StepMot4;
extern struct step_motor StepMot5;
extern struct step_motor StepMot6;


extern unsigned int RelativeOrigin;    //相对坐标原点
/////////////////////////
void StepMotInit(void);
void StepMoterPulseHandler(void);

//开步进电机1#
//step_count>0  --->  向前 MOTOR_STATUS_FORWORD   前进到极限，关闭阀门
//step_count<0  --->  向后 MOTOR_STATUS_BACKWORD  退到最后，全开阀门
//2400在8分频时是300步步进电机的极限行程
void StepMotRun1(signed int step_count);
void StepMotRun2(signed int step_count);
void StepMotRun3(signed int step_count);
void StepMotRun4(signed int step_count);
void StepMotRun5(signed int step_count);
void StepMotRun6(signed int step_count);

void StepMotStop1(void);
void StepMotStop2(void);


#endif

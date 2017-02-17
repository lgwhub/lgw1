/**
  ******************************************************************************
  * @file    StepMotor.h
  * @author  Liguowei  ����ΰ
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

//2400��8��Ƶʱ��300����������ļ����г�
#define MOTOR_MAX_STEP   2400


//�������
struct step_motor{
signed  int Position16;   //������������ʾ����λ�ã�������
unsigned int PulseCircleSet;		//2000*50us  = 0.1s   ��������ٶȿ��� ����ֵ
unsigned int PulseCircleCount;//StepMot1.PulseCircleSet;   ��������ٶȿ���
unsigned int PulseCount;
unsigned char Enable;
unsigned char ClkStatus;

unsigned char Direction;  //����  1��ǰ  ��������Ϊ����

};

extern struct step_motor StepMot1;
extern struct step_motor StepMot2;
extern struct step_motor StepMot3;
extern struct step_motor StepMot4;
extern struct step_motor StepMot5;
extern struct step_motor StepMot6;


extern unsigned int RelativeOrigin;    //�������ԭ��
/////////////////////////
void StepMotInit(void);
void StepMoterPulseHandler(void);

//���������1#
//step_count>0  --->  ��ǰ MOTOR_STATUS_FORWORD   ǰ�������ޣ��رշ���
//step_count<0  --->  ��� MOTOR_STATUS_BACKWORD  �˵����ȫ������
//2400��8��Ƶʱ��300����������ļ����г�
void StepMotRun1(signed int step_count);
void StepMotRun2(signed int step_count);
void StepMotRun3(signed int step_count);
void StepMotRun4(signed int step_count);
void StepMotRun5(signed int step_count);
void StepMotRun6(signed int step_count);

void StepMotStop1(void);
void StepMotStop2(void);


#endif

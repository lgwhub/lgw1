

#ifndef _USER_H
#define _USER_H




//产品唯一身份标识寄存器（96位）
//基地址0X1FFF F7E8
//位15:0
//地址偏移0x02
//位31:16
//地址偏移0x04
//位63:32
//地址偏移0x06
//位95:64





//通道数
#define MAX_TEMPRATURE_CHNALL   8






///////////////////////////////

extern unsigned char FlagKey;
extern unsigned char EventTimeBuz;
extern unsigned char EventTimeLed;


#define Led_Test_Key_On1		;//LED1_ON
#define Led_Test_Key_Off1		;//LED1_OFF

#define Led_Test_Adc_On1		;//LED1_ON
#define Led_Test_Adc_Off1		;//LED1_OFF

#define Led_Status_On		LED1_ON
#define Led_Status_Off		LED1_OFF

#define Led_Event_On		;//	LED4_ON
#define Led_Event_Off		;//LED4_OFF


void MotorsRun(unsigned char ch, signed short int steps);

void TaskVirPwm(void * pdata);
void TaskModbus(void * pdata);
void TaskStepMotor(void * pdata);
void TaskTimePr(void * pdata);
void TaskHeat(void * pdata);
void TaskStatus(void * pdata);



void TaskTs(void * pdata);
void TaskRecv(void * pdata);
void TaskInput1(void * pdata);
void TaskSyncUp(void * pdata);

void ModbusCommand2(void);

#endif

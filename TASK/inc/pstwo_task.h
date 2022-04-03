#ifndef _PSTWO_TASK_H_
#define _PSTWO_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"
#include "usart4.h"

typedef struct
{
	uint16_t ch1;
	uint16_t ch2;
	uint16_t ch3;
	uint16_t ch4;
	uint16_t ch5;
	uint16_t ch6;
	uint16_t ch7;
	uint16_t ch8;
	uint16_t ch9;
	uint16_t ch10;
}Rc_Data_t;

extern struct rt_semaphore remoter_get_semaphore;

void Pstwo_Thread_Init(void);
rt_thread_t Get_Pstwo_Thread_Object(void);
const Rc_Data_t* Get_Rc_Data(void);
int16_t Joystick_Raw_To_Normal_Data(uint16_t data);

#endif //_PSTWO_TASK_H_

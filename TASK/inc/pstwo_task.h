#ifndef _PSTWO_TASK_H_
#define _PSTWO_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"
#include "pstwo.h"
#include "led.h"
#include "usart4.h"

void Pstwo_Thread_Init(void);
rt_thread_t Get_Pstwo_Thread_Object(void);
PS2 *Get_Remoter_Data(void);

#endif //_PSTWO_TASK_H_

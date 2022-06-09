#ifndef __OLED_TASK_H_
#define __OLED_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"

void Oled_Thread_Init(void);
rt_thread_t Get_Oled_Thread_Object(void);

#endif //__OLED_TASK_H_

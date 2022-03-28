#ifndef __CHASSIS_TASK_H_
#define __CHASSIS_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"

void Chassis_Thread_Init(void);
rt_thread_t Get_Chassis_Thread_Object(void);

#endif // __CHASSIS_TASK_H_

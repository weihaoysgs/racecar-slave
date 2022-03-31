#ifndef _AHRC_TASK_H_
#define _AHRC_TASK_H_

#include "mpu9250.h"
#include "stm32f10x.h"
#include "rthw.h"
#include "usart4.h"
#include "rtthread.h"

void Ahrs_Thread_Init(void);
rt_thread_t Get_Ahrs_Thread_Object(void);

#endif //_AHRC_TASK_H_

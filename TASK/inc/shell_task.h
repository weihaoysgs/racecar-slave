#ifndef __SHELL_TASK_H_
#define __SHELL_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"
#include "string.h"
#include "usart5.h"

void Shell_Thread_Init(void);
rt_thread_t Get_Shell_Thread_Object(void);

#endif //__SHELL_TASK_H_

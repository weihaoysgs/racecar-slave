#ifndef _UPLOAD_TASK_H_
#define _UPLOAD_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"
#include "usart1.h"
#include "upload_message.h"

void Upload_Thread_Init(void);
rt_thread_t Get_Upload_Thread_Object(void);

#endif //_UPLOAD_TASK_H_

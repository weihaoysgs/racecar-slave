#ifndef __CHASSIS_TASK_H_
#define __CHASSIS_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"
#include "usart4.h"
#include "led.h"
#include "pstwo_task.h"
#include "motor.h"
#include "pid.h"
#include "usart4.h"
#include "motor_speed_timer.h"
#include "usart4.h"
#include "usart1.h"

void Chassis_Thread_Init(void);
rt_thread_t Get_Chassis_Thread_Object(void);
void Set_Chassis_Motor_Speed(float left_motor_speed, float right_motor_speed);
void Int16_Constrain(uint16_t *data, const uint16_t min_value, const uint16_t max_value);

#endif // __CHASSIS_TASK_H_

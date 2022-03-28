#ifndef __MOTOR_SPEED_TIMER_H_
#define __MOTOR_SPEED_TIMER_H_

#include "stm32f10x.h"
#include <rtthread.h>

void Motor_Speed_Clc_Timer_Init(void);
rt_timer_t Get_Motor_Speed_Clc_Timer_Object(void);
int32_t GetMotorLeftSpeed(void);
int32_t GetMotorRightSpeed(void);

#endif

#ifndef __CHASSIS_TASK_H_
#define __CHASSIS_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"
#include "usart4.h"
#include "led.h"
#include "pstwo_task.h"
#include "rosmsg_task.h"
#include "motor.h"
#include "pid.h"
#include "usart4.h"
#include "motor_speed_timer.h"
#include "usart4.h"
#include "usart1.h"
#include "upload_message.h"

typedef struct Servo_Construction_Value_t
{
    uint16_t min_;
    uint16_t middle;
    uint16_t max_;
}Servo_Construction_Value_t;

void Set_Racecar_Direction(uint16_t servo_pulse);
void Chassis_Thread_Init(void);
void Send_Chessis_Encoder2Ros(uint8_t *buffer,const uint8_t size);
rt_thread_t Get_Chassis_Thread_Object(void);
void Set_Chassis_Motor_Speed(float left_motor_speed, float right_motor_speed);
void Int16_Constrain(int16_t *data, const int16_t min_value, const int16_t max_value);
void Uint16_Constrain(uint16_t *data, const uint16_t min_value, const uint16_t max_value);
void Float_Constrain(float *data, const float min_value, const float max_value);

#endif // __CHASSIS_TASK_H_

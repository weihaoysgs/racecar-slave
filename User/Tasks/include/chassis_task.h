#ifndef _CHASSIS_TASK_H
#define _CHASSIS_TASK_H

#include "cmsis_os.h"
#include "uart4.h"
#include "tim8.h"
#include "pid.h"
#include "motor_speed_timer.h"
#include "uart4.h"
#include "motor.h"
#include "ps_two.h"
#include "pstwo_task.h"

// p, i, d, limit_i, limit_output, is_is_spare, begin_i, stop_grow_i
static Pid_Position_t motor_bl_speed_pid = NEW_POSITION_PID(2, 0, 0, 200, 7200, 0, 10, 500);
static Pid_Position_t motor_br_speed_pid = NEW_POSITION_PID(2, 0, 0, 200, 7200, 0, 10, 500);

void Info_Chassis_Task_Get_Control_Data(void);
void Set_ChassisMotor_Speed(float speed_fl, float speed_fr);
uint16_t Servo_Pwm_Limit(int16_t *input);

#endif //_CHASSIS_TASK_H

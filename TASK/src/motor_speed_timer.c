#include "motor_speed_timer.h"
#include "encoder.h"

static volatile int32_t motor_left_speed = 0;
static volatile int32_t motor_right_speed = 0;

static volatile int32_t motor_left_total_count = 0;
static volatile int32_t motor_right_total_count = 0;

static struct rt_timer motor_speed_clc_timer;

void motorSpeedClcTimerCallback(void* parameter);

rt_timer_t Get_Motor_Speed_Clc_Timer_Object(void) {
    return &motor_speed_clc_timer;
}

void Motor_Speed_Clc_Timer_Init(void) {
    rt_timer_init(&motor_speed_clc_timer,
                  "timer1",                   /* 定时器名字是 timer1 */
                  motorSpeedClcTimerCallback, /* 超时时回调的处理函数 */
                  RT_NULL,                    /* 超时函数的入口参数 */
                  70,                         /* 定时长度，以 OS Tick 为单位，即 10 个 OS Tick */
                  RT_TIMER_FLAG_PERIODIC);    /* 周期性定时器 */
}

void motorSpeedClcTimerCallback(void* parameter)
{
    motor_left_speed = GetTim2EncoderChangedValue();
    motor_left_total_count = motor_left_total_count + motor_left_speed;
    motor_right_speed = GetTim3EncoderChangedValue();
    motor_right_total_count = motor_right_total_count + motor_right_speed;
}

/**
 * @brief Get the Motor Left Speed
 * 
 * @return int32_t 电机速度
 */
int32_t GetMotorLeftSpeed(void) {
    return motor_left_speed;
}

/**
 * @brief Get the Motor Right Speed
 * 
 * @return int32_t 电机速度
 */
int32_t GetMotorRightSpeed(void) {
    return motor_right_speed;
}

/**
 * @brief Get the Motor Left cnt
 * 
 * @return int32_t 总脉冲
 */
int32_t GetMotorLeftCount(void) {
    return motor_left_total_count;
}

/**
 * @brief Get the Motor Right cnt
 * 
 * @return int32_t 总脉冲
 */
int32_t GetMotorRightCount(void) {
    return motor_right_total_count;
}

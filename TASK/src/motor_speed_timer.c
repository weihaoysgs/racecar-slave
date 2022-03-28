#include "motor_speed_timer.h"
#include "encoder.h"

extern osTimerId motorSpeedCalcTimerHandle;
static int32_t motor_left_speed = 0;
static int32_t motor_right_speed = 0;
static const uint16_t servo_init_pulse = 200;

void StartTimer2Timer3AndStartMotorSpeedCalcTimer(void)
{
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
    LL_TIM_ClearFlag_UPDATE(TIM2);
    LL_TIM_EnableIT_UPDATE(TIM2);
    LL_TIM_EnableCounter(TIM2);
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH2);
    LL_TIM_ClearFlag_UPDATE(TIM3);
    LL_TIM_EnableIT_UPDATE(TIM3);
    LL_TIM_EnableCounter(TIM3);
    osTimerStart(motorSpeedCalcTimerHandle, 80);
}

void motorSpeedClcTimerCallback(void const *argument)
{
    motor_left_speed = GetTim2EncoderChangedValue();
    motor_right_speed = GetTim3EncoderChangedValue();
}

int32_t GetMotorLeftSpeed(void)
{
    return motor_left_speed;
}

int32_t GetMotorRightSpeed(void)
{
    return motor_right_speed;
}

void StartServoTimer(void)
{
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1); //! Enable capture/compare channels.
    LL_TIM_EnableAllOutputs(TIM1);                     //! Enable all output
    LL_TIM_EnableCounter(TIM1);                        //! Enable timer counter.
    // LL_TIM_OC_SetCompareCH1(TIM1, servo_init_pulse);
}

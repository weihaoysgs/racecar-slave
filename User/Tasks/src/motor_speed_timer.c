#include "motor_speed_timer.h"
#include "encoder.h"

extern osTimerId motorSpeedCalcTimerHandle;
static int32_t tim3Speed = 0;

void StartMotorSpeedCalcTimer(void) {
    osTimerStart(motorSpeedCalcTimerHandle, 500);
}

void motorSpeedClcTimerCallback(void const * argument) {
    tim3Speed = GetTim3EncoderChangedValue();
}

int32_t GetMotorRightSpeed(void) {
    return tim3Speed;
}

#ifndef __MOTOR_SPEED_TIMER_H_
#define __MOTOR_SPEED_TIMER_H_

#include "cmsis_os.h"

void StartTimer2Timer3AndStartMotorSpeedCalcTimer(void);
int32_t GetMotorLeftSpeed(void);
int32_t GetMotorRightSpeed(void);
void StartServoTimer(void);

#endif

#ifndef __MOTOR_SPEED_TIMER_H_
#define __MOTOR_SPEED_TIMER_H_

#include "cmsis_os.h"

void StartMotorSpeedCalcTimer(void);

int32_t GetMotorRightSpeed(void);

#endif

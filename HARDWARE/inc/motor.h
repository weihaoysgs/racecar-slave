#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f10x.h"

void SetMotorLeftPower(int32_t power);
void SetMotorRightPower(int32_t power);
int16_t Output_Smoothing(int16_t output);

#endif // __MOTOR_H_

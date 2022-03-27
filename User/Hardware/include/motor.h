#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f1xx.h"
#include "stm32f1xx_ll_tim.h"

void SetMotorLeftPower(int32_t power);
void SetMotorRightPower(int32_t power);

#endif // __MOTOR_H_

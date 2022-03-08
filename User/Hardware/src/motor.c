#include "motor.h"

// U1
// PC6 - IN2 - TIM8_CH1
// PC7 - IN1 - TIM8_CH2

// U2
// PC8 - IN2 - TIM8_CH3
// PC9 - IN1 - TIM8_CH4

void SetMotorLeftPower(int32_t power) {
	if(power > 0) {
		TIM2->CCR1 = 0;
		TIM2->CCR2 = power;
	} else {
		TIM2->CCR1 = power;
		TIM2->CCR2 = 0;
	}
}

void SetMotorRightPower(uint16_t power) {
	if(power > 0) {
		TIM2->CCR3 = 0;
		TIM2->CCR4 = power;
	} else {
		TIM2->CCR3 = power;
		TIM2->CCR4 = 0;
	}
}
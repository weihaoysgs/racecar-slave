#include "motor.h"

// U1
// PC6 - IN2 - TIM8_CH1
// PC7 - IN1 - TIM8_CH2

// U2
// PC8 - IN2 - TIM8_CH3
// PC9 - IN1 - TIM8_CH4

void SetMotorLeftPower(int32_t power) {
	if(power == 0){
		TIM8->CCR1 = 0;
		TIM8->CCR2 = 0;
	}
	else if (power > 0) {
		TIM8->CCR1 = 7200;
		TIM8->CCR2 = 7200 - power;
	}
	else {
		TIM8->CCR2 = 7200;
		TIM8->CCR1 = 7200 + power;
	}
}

void SetMotorRightPower(int32_t power) {
	if(power == 0){
		TIM8->CCR3 = 0;
		TIM8->CCR4 = 0;
	}
	else if (power > 0) {
		TIM8->CCR3 = 7200;
		TIM8->CCR4 = 7200 - power;
	}
	else {
		TIM8->CCR4 = 7200;
		TIM8->CCR3 = 7200 + power;
	}
}

int16_t Output_Smoothing(int16_t output)
{
	int16_t true_output = 0;
	if (output == 0)
	{
		true_output = 0;
	}
	else if (output > 0)
	{
		true_output = (output * (7200 - 500) / 7200) + 500;
	}
	else if (output < 0)
	{
		true_output = (output * (7200 + 500) / 7200) - 500;
	}
	return true_output;
}

#ifndef _DEFAULT_TASK_H_
#define _DEFAULT_TASK_H_

#include "stm32f1xx.h"
#include "cmsis_os.h"
#include "stm32f1xx_ll_gpio.h"

#define LED_GREEN_ON() LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13)
#define LED_GREEN_OFF() LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13)

#endif //_DEFAULT_TASK_H_

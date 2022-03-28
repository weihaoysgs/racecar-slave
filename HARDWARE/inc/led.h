#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"

#define LED_ON() GPIOB->BSRRL = GPIO_Pin_13
#define LED_OFF() GPIOB->BSRRH = GPIO_Pin_13
#define LED_TOGGLE() GPIOB->ODR ^= GPIO_Pin_13

void Led_Init(void);

#endif

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f1xx.h"
#include "stm32f1xx_ll_tim.h"

int32_t GetTim3EncoderChangedValue(void);
void Tim3InterruptCallback(void);

#endif //_ENCODER_H_

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f10x.h"

int32_t GetTim2EncoderChangedValue(void);
void Tim2InterruptCallback(void);

int32_t GetTim3EncoderChangedValue(void);
void Tim3InterruptCallback(void);

#endif //_ENCODER_H_

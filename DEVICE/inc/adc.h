#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"

void Adc_Init(void);
u16  Get_Adc(u8 ch);
int  Get_battery_volt(void);    

#endif

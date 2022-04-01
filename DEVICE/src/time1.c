#include "time1.h"
// Time1 PA8 -> ch1
void Time1_Pwm_Init(uint32_t arr, uint32_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                          

    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseStructure.TIM_Period = arr;                      
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);                      

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);  
}

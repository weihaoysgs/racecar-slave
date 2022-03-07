#include "tim8.h"

void TimeEightInit(void)
{
    LL_TIM_CC_EnableChannel(TIM8, LL_TIM_CHANNEL_CH1); // Enable capture/compare channels.
    LL_TIM_CC_EnableChannel(TIM8, LL_TIM_CHANNEL_CH2);
    LL_TIM_CC_EnableChannel(TIM8, LL_TIM_CHANNEL_CH3);
    LL_TIM_CC_EnableChannel(TIM8, LL_TIM_CHANNEL_CH4);
    LL_TIM_EnableAllOutputs(TIM8);    // Enable the outputs (set the MOE bit in TIMx_BDTR register).
    LL_TIM_EnableCounter(TIM8);       // Enable timer counter.
    LL_TIM_OC_SetCompareCH1(TIM8, 0); // Set compare value for output channel 1 (TIMx_CCR1).
}


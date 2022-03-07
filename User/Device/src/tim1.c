#include "tim1.h"

/**
 * @brief Init tim1
 * 
 */
void TimeOneInit(void)
{
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1); // Enable capture/compare channels.
    LL_TIM_EnableAllOutputs(TIM1);                     // Enable the outputs (set the MOE bit in TIMx_BDTR register).
    LL_TIM_EnableCounter(TIM1);                        // Enable timer counter.
    LL_TIM_OC_SetCompareCH1(TIM1, 0);                  // Set compare value for output channel 1 (TIMx_CCR1).
}

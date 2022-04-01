#include "encoder.h"

/* --- 左电机编码器 BEGIN --- */
// static int8_t tim2_update_direction = 0;
static int32_t tim2_last_cnt_value;

/**
 * @brief 计算编码器改变的数据
 * 
 * @return int32_t 改变的数据
 */
int32_t GetTim2EncoderChangedValue(void) {
	int32_t encoder_changed_value = 0;
	int32_t tim2_this_cnt_value = TIM2->CNT;
	// encoder_changed_value = tim2_this_cnt_value-tim2_last_cnt_value + 3300*tim2_update_direction;
	encoder_changed_value = tim2_this_cnt_value-tim2_last_cnt_value;
	if (encoder_changed_value < -1650) {
		encoder_changed_value += 3300;
	}
	if (encoder_changed_value > 1650) {
		encoder_changed_value -= 3300;
	}
	// tim2_update_direction = 0;
	tim2_last_cnt_value = tim2_this_cnt_value;
	return encoder_changed_value;
}

void TIM2_IRQHandler(void) {
	// !!! Do Nothing
	// if(TIM_GetITStatus(TIM2, TIM_IT_Update)) {
	// 	if(0u == ((TIM2->CR1) & ((0x1UL << (4U))))) {
	// 		tim2_update_direction++;
	// 	} else {
	// 		tim2_update_direction--;
	// 	}
	// 	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	// }
}
/* --- 左电机编码器 END --- */


/* --- 右电机编码器 BEGIN --- */
// static int8_t tim3_update_direction = 0;
static int32_t tim3_last_cnt_value;

/**
 * @brief 计算编码器改变的数据
 * 
 * @return int32_t 改变的数据
 */
int32_t GetTim3EncoderChangedValue(void) {
	int32_t encoder_changed_value = 0;
	int32_t tim3_this_cnt_value = TIM3->CNT;
	// encoder_changed_value = tim3_this_cnt_value-tim3_last_cnt_value + 3300*tim3_update_direction;
	encoder_changed_value = tim3_this_cnt_value-tim3_last_cnt_value;
	if (encoder_changed_value < -2500) {
		encoder_changed_value += 3300;
	}
	if (encoder_changed_value > 2500) {
		encoder_changed_value -= 3300;
	}
	// tim3_update_direction = 0;
	tim3_last_cnt_value = tim3_this_cnt_value;
	return encoder_changed_value;
}

void TIM3_IRQHandler(void) {
	// !!! Do Nothing
	// if(TIM_GetITStatus(TIM3, TIM_IT_Update)) {
	// 	if(0u == ((TIM3->CR1) & ((0x1UL << (4U))))) {
	// 		tim3_update_direction++;
	// 	} else {
	// 		tim3_update_direction--;
	// 	}
	// 	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	// }
}
/* --- 右电机编码器 END --- */

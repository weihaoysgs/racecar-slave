#include "encoder.h"
#include "uart4.h"

/* --- 左电机编码器 BEGIN --- */
static int8_t tim2_update_direction = 0;
static int32_t tim2_last_cnt_value;

int32_t GetTim2EncoderChangedValue(void) {
	int32_t encoder_changed_value = 0;
	int32_t tim2_this_cnt_value = TIM2->CNT;
	encoder_changed_value = tim2_this_cnt_value-tim2_last_cnt_value + 3300*tim2_update_direction;
	tim2_update_direction = 0;
	tim2_last_cnt_value = tim2_this_cnt_value;
	return encoder_changed_value;
}

void Tim2InterruptCallback(void) {
	if(LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
		if(LL_TIM_COUNTERDIRECTION_UP == LL_TIM_GetDirection(TIM2)) {
			tim2_update_direction++;
		} else {
			tim2_update_direction--;
		}
		LL_TIM_ClearFlag_UPDATE(TIM2);
	}
}
/* --- 左电机编码器 END --- */


/* --- 右电机编码器 BEGIN --- */
static int8_t tim3_update_direction = 0;
static int32_t tim3_last_cnt_value;

int32_t GetTim3EncoderChangedValue(void) {
	int32_t encoder_changed_value = 0;
	int32_t tim3_this_cnt_value = TIM3->CNT;
	encoder_changed_value = tim3_this_cnt_value-tim3_last_cnt_value + 3300*tim3_update_direction;
	tim3_update_direction = 0;
	tim3_last_cnt_value = tim3_this_cnt_value;
	return encoder_changed_value;
}

void Tim3InterruptCallback(void) {
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
		if(LL_TIM_COUNTERDIRECTION_UP == LL_TIM_GetDirection(TIM3)) {
			tim3_update_direction++;
		} else {
			tim3_update_direction--;
		}
		LL_TIM_ClearFlag_UPDATE(TIM3);
	}
}
/* --- 右电机编码器 END --- */

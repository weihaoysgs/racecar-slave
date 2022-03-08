#include "encoder.h"

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

void TIM3_IRQHandler(void) {
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
		if(LL_TIM_COUNTERDIRECTION_UP == LL_TIM_GetDirection(TIM3)) {
			tim3_update_direction++;
		} else {
			tim3_update_direction--;
		}
		LL_TIM_ClearFlag_UPDATE(TIM3);
	}
}

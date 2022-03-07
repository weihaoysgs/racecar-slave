#include "default_task.h"

/**
 * @brief Led flask task
 *
 * @param argument
 */
void StartDefaultTask(void const *argument)
{
    for (;;)
    {
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
        osDelay(500);
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
        osDelay(500);
    }
}

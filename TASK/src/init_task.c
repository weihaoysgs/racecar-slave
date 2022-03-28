#include "init_task.h"
#include "led.h"

/**
 * @brief �����ʼ��
 * 
 */
void Peripheral_Init(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    Led_Init();
}

#include "pstwo_task.h"

PS2 remoter;

void StartPstwoTask(void const *argument)
{
    // PS2_Init();
    // PS2_SetInit();
    for (;;)
    {
        /*更新遥控数据*/
        Update_Ps2_Data();
        Get_Rc_Data(&remoter);

        ___printf("key: %d\t", remoter.key);
        ___printf("ch0: %d\t", remoter.ch0);
        ___printf("ch1: %d\t", remoter.ch1);
        ___printf("ch2: %d\t", remoter.ch2);
        ___printf("ch3: %d\r\n", remoter.ch3);

        osDelay(10);
    }
}

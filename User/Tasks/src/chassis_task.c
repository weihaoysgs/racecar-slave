#include "chassis_task.h"

static const int32_t chassis_get_control_data_sig = 0x0001;
extern osThreadId chassisTaskHandle;

void StartChassisTask(void const *argument)
{
    Uart4_Rx_Init();
    Uart4_Tx_Init();
    //osEvent chassis_control_event;

    for (;;)
    {
//        ___printf("hello\r\n");
        // chassis_control_event = osSignalWait(chassis_get_control_data_sig, osWaitForever);
        // if (chassis_control_event.status == osEventSignal)
        // {
        //     if (chassis_control_event.value.signals == chassis_get_control_data_sig)
        //     {
        //     }
        // }
        osDelay(50);
    }
}

void Info_Chassis_Task_Get_Control_Data(void)
{
    osSignalSet(chassisTaskHandle, chassis_get_control_data_sig);
}

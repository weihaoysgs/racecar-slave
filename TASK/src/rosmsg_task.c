#include "rosmsg_task.h"
#include "usart5.h"

/* relate to thread BEGIN */
ALIGN(RT_ALIGN_SIZE)
static char rosmsg_thread_stack[512];
static struct rt_thread rosmsg_thread_object;
static void Rosmsg_Thread(void *param);

void Rosmsg_Thread_Init(void)
{
    rt_thread_init(&rosmsg_thread_object,
                   "rosmsg",
                   Rosmsg_Thread,
                   RT_NULL,
                   &rosmsg_thread_stack[0],
                   sizeof(rosmsg_thread_stack),
                   3,
                   20);
}

rt_thread_t Get_Rosmsg_Thread_Object(void)
{
    return &rosmsg_thread_object;
}
/* relate to thread END */

static uint8_t ros_message_status = 0;
struct rt_semaphore rosmsg_get_semaphore; //收到一帧ROS数据信号量
static Ros_message_t ros_message;

static void Rosmsg_Thread(void *param)
{
    rt_sem_init(&rosmsg_get_semaphore,
                "rosmsg_get",
                0,
                RT_IPC_FLAG_PRIO);
    rt_thread_delay(1000);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

    for (;;)
    {
        if(RT_EOK == rt_sem_take(&rosmsg_get_semaphore, 200))
		{
            if(*Get_Usart1_Rxd_Len() == 12)
            {
                // 帧头、id检测
                if(Get_Usart1_Rxd_Buffer()[0] == 0xA5 && Get_Usart1_Rxd_Buffer()[1] == 0x22)
                {
                    // CRC校验
                    if(1)
                    {
                        memcpy(&ros_message, Get_Usart1_Rxd_Buffer(), *Get_Usart1_Rxd_Len());
                    }
                }
            }
            ros_message_status = 1;
        }
        else
        {
            ros_message_status = 0;
        }
    }
}

const Ros_message_t* Get_Ros_Message(void)
{
    return &ros_message;
}

uint8_t Get_Ros_Message_Status(void)
{
    return ros_message_status;
}

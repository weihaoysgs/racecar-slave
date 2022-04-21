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

struct rt_semaphore rosmsg_get_semaphore; //收到一帧ROS数据信号量
static Ros_message_t ros_message;

static void Rosmsg_Thread(void *param)
{
    rt_sem_init(&rosmsg_get_semaphore,
                "rosmsg_get",
                0,
                RT_IPC_FLAG_PRIO);
    rt_thread_delay(1000);
    USART_ITConfig(USART1,USART_IT_IDLE, ENABLE);

    for (;;)
    {
        if(RT_EOK == rt_sem_take(&rosmsg_get_semaphore, 200))
		{
            // printf("Len%d\t", *Get_Usart1_Rxd_Len());
            // for(int i=0; i<*Get_Usart1_Rxd_Len(); i++)
            //     printf("%x ", Get_Usart1_Rxd_Buffer()[i]);
            // printf("\r\n");
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

            printf("aa %.2f %.2f\r\n", ros_message.data.speed, ros_message.data.angle);
        }
        else
        {
            ;
        }
    }
}

const Ros_message_t* Get_Ros_Message(void)
{
    return &ros_message;
}

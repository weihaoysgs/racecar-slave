#ifndef __ROSMSG_TASK_H_
#define __ROSMSG_TASK_H_

#include "stm32f10x.h"
#include "rthw.h"
#include "rtthread.h"
#include "usart1.h"
#include "string.h"

typedef __packed struct
{
    // 帧头
    __packed struct
    {
        uint8_t package_head; ///< 0xA5
        uint8_t cmd_id; ///< 0x22
        uint8_t count;
    } frame_head;

    // 数据段
    __packed struct
    {
        float speed;
        float angle;
    } data;

    //帧尾 crc8校验
    uint8_t crc8;
} Ros_message_t;

extern struct rt_semaphore rosmsg_get_semaphore; //收到一帧ROS数据信号量

void Rosmsg_Thread_Init(void);
rt_thread_t Get_Rosmsg_Thread_Object(void);

#endif // __ROSMSG_TASK_H_

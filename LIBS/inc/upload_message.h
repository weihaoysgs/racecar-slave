#ifndef __UPLOAD_MESSAGE_H_
#define __UPLOAD_MESSAGE_H_

#include "stm32f10x.h"

typedef __packed struct
{
    // 帧头
    __packed struct
    {
        uint8_t package_head;
        uint8_t cmd_id;
        uint8_t count;
    } frame_head;

    // 数据段
    __packed struct
    {
        int16_t left_speed;
        int16_t right_speed;

        int32_t left_count;
        int32_t right_count;

        uint16_t servo_angle;

        float yaw_angle;
    } data;

    //帧尾 crc8校验
    uint8_t crc8;
} Send_message_t;

void Set_Ros_Send_Data_MSG(void);
uint32_t Get_Ros_Send_Data_Len(void);
uint8_t* Get_Ros_Send_Buffer(void);

#endif //__UPLOAD_MESSAGE_H_

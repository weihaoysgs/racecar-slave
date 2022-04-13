#include "upload_message.h"
#include "motor_speed_timer.h"

static Send_message_t send_message;

void Set_Ros_Send_Data_MSG(void)
{
    send_message.frame_head.package_head = 0xA5;
    send_message.frame_head.cmd_id = 0x11;
    send_message.frame_head.count++;

    send_message.data.left_speed = GetMotorLeftSpeed();
    send_message.data.right_speed = GetMotorRightSpeed();
    send_message.data.left_count = GetMotorLeftCount();
    send_message.data.right_count = GetMotorRightCount();
    send_message.data.servo_angle = TIM1->CCR1;
    send_message.data.yaw_angle = 0.0f;

    send_message.crc8 = 0xFE;
}

uint32_t Get_Ros_Send_Data_Len(void)
{
    return (sizeof(send_message));
}

uint8_t* Get_Ros_Send_Buffer(void)
{
    return (uint8_t*)&send_message;
}

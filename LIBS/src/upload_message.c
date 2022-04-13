#include "upload_message.h"
#include "motor_speed_timer.h"

static Send_message_t send_message;

void Set_Ros_Send_Data_MSG(void)
{
    send_message.package_head = 0xA5;
    send_message.cmd_id = 0x11;
    send_message.count++;

    send_message.left_speed = GetMotorLeftSpeed();
    send_message.right_speed = GetMotorRightSpeed();
    send_message.left_count = GetMotorLeftCount();
    send_message.right_count = GetMotorRightCount();
    send_message.servo_angle = TIM1->CCR1;
    send_message.yaw_angle = 0.0f;

    send_message.crc8_itu = 0xFE;
}

uint32_t Get_Ros_Send_Data_Len(void)
{
    return (sizeof(send_message));
}

uint8_t* Get_Ros_Send_Buffer(void)
{
    return (uint8_t*)&send_message;
}

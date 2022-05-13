#include "upload_message.h"
#include "motor_speed_timer.h"
#include "string.h"

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

    send_message.data.mpu_raw_data.ax = Get_Mpu_Data_pt()->ax;
    send_message.data.mpu_raw_data.ay = Get_Mpu_Data_pt()->ay;
    send_message.data.mpu_raw_data.az = Get_Mpu_Data_pt()->az;
    send_message.data.mpu_raw_data.gx = Get_Mpu_Data_pt()->gx;
    send_message.data.mpu_raw_data.gy = Get_Mpu_Data_pt()->gy;
    send_message.data.mpu_raw_data.gz = Get_Mpu_Data_pt()->gz;
    send_message.data.mpu_raw_data.mx = Get_Mpu_Data_pt()->mx;
    send_message.data.mpu_raw_data.my = Get_Mpu_Data_pt()->my;
    send_message.data.mpu_raw_data.mz = Get_Mpu_Data_pt()->mz;

    send_message.data.mpu_raw_data.ax_offset = Get_Mpu_Data_pt()->ax_offset;
    send_message.data.mpu_raw_data.ay_offset = Get_Mpu_Data_pt()->ay_offset;
    send_message.data.mpu_raw_data.az_offset = Get_Mpu_Data_pt()->az_offset;

    send_message.data.mpu_raw_data.gx_offset = Get_Mpu_Data_pt()->gx_offset;
    send_message.data.mpu_raw_data.gy_offset = Get_Mpu_Data_pt()->gy_offset;
    send_message.data.mpu_raw_data.gz_offset = Get_Mpu_Data_pt()->gz_offset;
    send_message.data.mpu_raw_data.temp = Get_Mpu_Data_pt()->temp;
    // memcpy((uint8_t *)&send_message.data.mpu_raw_data,(uint8_t *)Get_Mpu_Data_pt(), sizeof(Mpu_Data_t));

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

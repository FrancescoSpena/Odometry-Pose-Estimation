/**
 * beth_globals.h
*/

#pragma once 
#include "beth_packets.h"
#include "packet_operations.h"

extern MotorControlPacket motor1_control;
extern MotorStatusPacket motor1_status;
extern MotorParamsPacket motor1_params;
extern MotorControlPacket motor2_control;
extern MotorStatusPacket motor2_status;
extern MotorParamsPacket motor2_params;
extern SystemStatusPacket system_status;

extern DifferentialDriveParamPacket drive_params;
extern DifferentialDriveControlPacket drive_control;
extern DifferentialDriveStatusPacket drive_status;

//Operations
extern PacketOperation motor_control_op;
extern PacketOperation motor_status_op;
extern PacketOperation motor_params_op;
extern PacketOperation system_status_op;
extern PacketOperation diff_drive_params_op;
extern PacketOperation diff_drive_control_op;
extern PacketOperation diff_drive_status_op;

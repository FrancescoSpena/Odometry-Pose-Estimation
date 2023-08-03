/**
 * beth_globals.c
*/

#include "beth_globals.h"


MotorControlPacket motor1_control = {
    //header
    {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(motor1_control),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .mode=Direct,
    .speed=0 
};

MotorStatusPacket motor1_status = {
    //header
    {
        .id=ID_MOTOR_STATUS_PACKET,
        .size=sizeof(motor1_status),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .encoder_ticks=0,
    .desired_speed=0,
    .measured_speed=0
};

MotorParamsPacket motor1_params = {
    //header
    {
        .id=ID_MOTOR_PARAMS_PACKET,
        .size=sizeof(motor1_params),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .dir_a_pin=5, 
    .dir_b_pin=6,
    .pwm_pin=4,
    //PID
    .kp=5,
    .ki=30,
    .kd=0,

    .max_i=255,
    .max_out=255,
    .dt=0.01,
    .idt=100
};

MotorControlPacket motor2_control = {
    //header
    {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(motor2_control),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .mode=Direct,
    .speed=0
};

MotorStatusPacket motor2_status = {
    //header
    {
        .id=ID_MOTOR_STATUS_PACKET,
        .size=sizeof(motor2_status),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .encoder_ticks=0,
    .desired_speed=0,
    .measured_speed=0
};

MotorParamsPacket motor2_params = {
    //header
    {
        .id=ID_MOTOR_PARAMS_PACKET,
        .size=sizeof(motor2_params),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .dir_a_pin=7, 
    .dir_b_pin=8,
    .pwm_pin=9,
 
    .kp=5,
    .ki=30,
    .kd=0,

    .max_i=255,
    .max_out=255,
    .dt=0.01,
    .idt=100
};

SystemStatusPacket system_status = {
    {
        .id=ID_SYSTEM_STATUS_PACKET,
        .size=sizeof(system_status),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .rx_packets=0,
    .rx_errors=0,
    .tx_packets=0,
    .idle_cycles=0
};

DifferentialDriveParamPacket drive_params = {
    {
        .id=DIFFERENTIAL_DRIVE_PARAMS_PACKET,
        .size=sizeof(drive_params),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    //change this value (in cm)
    .distance=20,
    .radius_wheel=2
};

//dest_addr = 0 --> motor 1 
//dest_addr = 1 --> motor 2 
DifferentialDriveControlPacket drive_control = {
    {
        .id=DIFFERENTIAL_DRIVE_CONTROL_PACKET,
        .size=sizeof(drive_control),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .rotational_velocity=0,
    .translational_velocity=0
};

DifferentialDriveStatusPacket drive_status = {
    {
        .id=DIFFERENTIAL_DRIVE_STATUS_PACKET,
        .size=sizeof(drive_status),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .odom_x=0,
    .odom_y=0,
    .odom_theta=0,
    .rotational_velocity_desired=0,
    .translational_velocity_desired=0,
    .rotational_velocity_measured=0,
    .translational_velocity_measured=0
};
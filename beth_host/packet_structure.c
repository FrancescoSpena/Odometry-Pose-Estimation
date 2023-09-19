#include "packet_structure.h"
#include "beth_comm_host.h"

//PacketHeader* header;

MotorControlPacket motor1_control = {
    //header
    {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(motor1_control),
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
        .dest_addr=0,
        .checksum=0,
    },
    .dir_a_pin=0, 
    .dir_b_pin=0,
    .pwm_pin=0,
    //PID
    .kp=0,
    .ki=0,
    .kd=0,

    .max_i=0,
    .max_out=0,
    .dt=0,
    .idt=0
};

MotorControlPacket motor2_control = {
    //header
    {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(motor2_control),
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
        .dest_addr=0,
        .checksum=0,
    },
    .dir_a_pin=0, 
    .dir_b_pin=0,
    .pwm_pin=0,
 
    .kp=0,
    .ki=0,
    .kd=0,

    .max_i=0,
    .max_out=0,
    .dt=0,
    .idt=0
};

SystemStatusPacket system_status = {
    {
        .id=ID_SYSTEM_STATUS_PACKET,
        .size=sizeof(system_status),
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
        .dest_addr=0,
        .checksum=0,
    },
    //change this value (in cm)
    .distance=0,
    .radius_wheel=0
};

//dest_addr = 0 --> motor 1 
//dest_addr = 1 --> motor 2 
DifferentialDriveControlPacket drive_control = {
    {
        .id=DIFFERENTIAL_DRIVE_CONTROL_PACKET,
        .size=sizeof(drive_control),
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
        .dest_addr=0,
        .checksum=0,
    },
    .odom_x=0,
    .odom_y=0,
    .odom_theta=0,
};


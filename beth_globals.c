/**
 * beth_globals.c
*/

#include "beth_globals.h"

MotorControlPacket motor_control = {
    //header
    {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(motor_control),
        .seq=0,
        .dest_addr=0,
        .src_addr=0,
        .checksum=0,
    },
    .mode=0,
    .speed=0
};

MotorStatusPacket motor_status = {
    //header
    {
        .id=ID_MOTOR_STATUS_PACKET,
        .size=sizeof(motor_status),
        .seq=0,
        .dest_addr=0,
        .src_addr=0,
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
        .src_addr=0,
        .checksum=0,
    },
    //Cambiare rispetto arduino
    .dir_a_pin=5, 
    .dir_b_pin=6,
    .pwm_pin=4,
    //Fare un tuning
    .kp=5,
    .ki=30,
    .kd=0,

    .max_i=255,
    .max_out=255,
    .dt=0.01,
    .idt=100
};

MotorParamsPacket motor2_params = {
    //header
    {
        .id=ID_MOTOR_PARAMS_PACKET,
        .size=sizeof(motor2_params),
        .seq=0,
        .dest_addr=0,
        .src_addr=0,
        .checksum=0,
    },
    //Cambiare rispetto arduino
    .dir_a_pin=7, 
    .dir_b_pin=8,
    .pwm_pin=9,
    //Fare un tuning
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
        .src_addr=0,
        .checksum=0,
    },
    .rx_packets=0,
    .rx_errors=0,
    .tx_packets=0,
    .idle_cycles=0
};
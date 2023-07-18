/**
 * beth_packets.h 
*/

#pragma once 
#include <stdint.h>

#define MAX_PACKET_TYPE 16 
#define PACKET_SIZE_MAX 256 
#define ID_MOTOR_CONTROL_PACKET 0 
#define ID_MOTOR_STATUS_PACKET 1
#define ID_MOTOR_PARAMS_PACKET 2
#define ID_SYSTEM_STATUS_PACKET 3

typedef enum{
    Direct=0,
    Pid=1,
    Disable=2,
}JointMode;

#pragma pack(push,1)

typedef struct{
    uint8_t id;
    uint8_t size;
    uint16_t seq;
    uint8_t dest_addr;
    uint8_t src_addr;
    uint8_t checksum;
}PacketHeader;

typedef struct{
    PacketHeader h;
    uint8_t mode;
    uint8_t speed;
}MotorControlPacket;

typedef struct{
    PacketHeader h;
    int16_t encoder_ticks;
    int16_t desired_speed;
    int16_t measured_speed;
}MotorStatusPacket;

typedef struct{
    PacketHeader h;
    uint8_t dir_a_pin;
    uint8_t dir_b_pin;
    uint8_t pwm_pin;
    //PID parameters 
    double kp;
    double ki;
    double kd; 
    
    double max_i;
    double sum_i;
    int16_t max_out;
    double dt, idt;
}MotorParamsPacket;

typedef struct{
    PacketHeader h;
    uint16_t rx_packets;
    uint16_t rx_errors; 
    uint16_t tx_packets;
    uint16_t idle_cycles;
}SystemStatusPacket;

#pragma pack(pop)


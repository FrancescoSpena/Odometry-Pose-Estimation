/**
 * beth_packets.h 
*/

#pragma once 
#include <stdint.h>

#define MAX_PACKET_TYPE 7 
#define PACKET_SIZE_MAX 256 
#define ID_MOTOR_CONTROL_PACKET 0 
#define ID_MOTOR_STATUS_PACKET 1
#define ID_MOTOR_PARAMS_PACKET 2
#define ID_SYSTEM_STATUS_PACKET 3
#define DIFFERENTIAL_DRIVE_STATUS_PACKET 4
#define DIFFERENTIAL_DRIVE_PARAMS_PACKET 5
#define DIFFERENTIAL_DRIVE_CONTROL_PACKET 6

typedef enum{
    Direct=0,
    Pid=1,
    Disable=2,
}JointMode;

#pragma pack(push,1)

//Header
typedef struct{
    uint8_t id;
    uint8_t size;
    uint16_t seq;
    uint8_t dest_addr;
    uint8_t checksum;
}PacketHeader;

//Single motor 
typedef struct{
    PacketHeader h;
    uint8_t mode;
    int16_t speed;
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

//System
typedef struct{
    PacketHeader h;
    uint16_t rx_packets;
    uint16_t rx_errors; 
    uint16_t tx_packets;
    uint16_t idle_cycles;
}SystemStatusPacket;

//Robot
typedef struct{
    PacketHeader h;
    float odom_x, odom_y, odom_theta; //odometry encoder
    
    float translational_velocity_desired;
    float rotational_velocity_desired;
    
    uint8_t enabled;
}DifferentialDriveStatusPacket;

typedef struct{
    PacketHeader h;
    float radius_wheel;
    float distance; //distance wheel
}DifferentialDriveParamPacket;

//sent from the pc to the robot
typedef struct{
    PacketHeader h;
    float translational_velocity;
    float rotational_velocity;
}DifferentialDriveControlPacket;

#pragma pack(pop)


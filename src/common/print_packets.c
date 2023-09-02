#include "print_packets.h"
#include <stdio.h>

uint16_t printHeader(PacketHeader* h, char* buf){
    return sprintf(buf,"[id: %d size:%d seq:%d dest:%d cs:%d]",
    h->id,
    h->size,
    h->seq,
    h->dest_addr,
    h->checksum);
}

uint16_t MotorControlPacket_print(PacketHeader* h, char* buf){
    MotorControlPacket* p = (MotorControlPacket*)h;
    uint16_t h_chars = printHeader(h,buf);
    return sprintf(buf+h_chars, "[mode:%d speed:%d]",p->mode,p->speed);
}

uint16_t MotorStatusPacket_print(PacketHeader* h, char* buf){
    MotorStatusPacket* p = (MotorStatusPacket*)h; 
    uint16_t h_chars = printHeader(h,buf);
    return sprintf(buf+h_chars,"[eticks: %d desidered speed:%d measured speed:%d]", p->encoder_ticks, p->desired_speed,p->measured_speed);
}

uint16_t MotorParamsPacket_print(PacketHeader* h, char* buf){
    MotorParamsPacket* p = (MotorParamsPacket*)h;
    uint16_t h_chars = printHeader(h,buf);
    return sprintf(buf+h_chars, "[dirA: %d, dirB:%d pwm:%d kp:%f ki:%f kd:%f dt:%f idt:%f]",
                p->dir_a_pin,
                p->dir_b_pin,
                p->pwm_pin,
                p->kp,
                p->ki,
                p->kd,
                p->dt,
                p->idt);
}

uint16_t SystemStatusPacket_print(PacketHeader* h, char* buf){
    SystemStatusPacket* p = (SystemStatusPacket*)h; 
    uint16_t h_chars = printHeader(h,buf);
    return sprintf(buf+h_chars, "[rxp:%d rxe:%d txp:%d idle:%d]",
                p->rx_packets,
                p->rx_errors,
                p->tx_packets,
                p->idle_cycles);
}

uint16_t DifferentialDriveStatusPacket_print(PacketHeader* h, char* buf){
    DifferentialDriveStatusPacket* p = (DifferentialDriveStatusPacket*)h;
    uint16_t h_chars = printHeader(h,buf);
    return sprintf(buf+h_chars,"[odom_x:%f odom_y:%f odom_theta:%f tran_measured_vel:%f rotat_measured_vel:%f tran_desidered_vel:%f rotat_measured_vel:%f enabled:%d]",
                p->odom_x,
                p->odom_y,
                p->odom_theta,
                p->translational_velocity_measured,
                p->rotational_velocity_measured,
                p->rotational_velocity_desired,
                p->rotational_velocity_desired,
                p->enabled);
}

uint16_t DifferentialDriveParamPacket_print(PacketHeader* h, char* buf){
    DifferentialDriveParamPacket* p = (DifferentialDriveParamPacket*)h;
    uint16_t h_chars = printHeader(h,buf);
    return sprintf(buf+h_chars,"[radius:%f distance:%f]",p->radius_wheel,p->distance);
}

uint16_t DifferentialDriveControlPacket_print(PacketHeader* h, char* buf){
    DifferentialDriveControlPacket* p = (DifferentialDriveControlPacket*)h;
    uint16_t h_chars = printHeader(h,buf);
    return sprintf(buf+h_chars,"tran_velocity:%f rotat_velocity:%f",
                p->translational_velocity,p->rotational_velocity);
}

typedef uint16_t (*PrintPacketFn)(PacketHeader* h, char*);

typedef struct{
    PrintPacketFn print_fn;
}PrintPacketOps;

static PrintPacketOps print_packet_ops[MAX_PACKET_TYPE] = {
    { // 0 
        .print_fn = MotorControlPacket_print,
    },
    { // 1 
        .print_fn = MotorStatusPacket_print,
    },
    { // 2 
        .print_fn = MotorParamsPacket_print,
    },
    { // 3 
        .print_fn = SystemStatusPacket_print,
    },
    { //4 differential status 
        .print_fn = DifferentialDriveStatusPacket_print,
    },
    { //5 differential params
        .print_fn = DifferentialDriveParamPacket_print,
    },
    { //6 differential control
        .print_fn = DifferentialDriveControlPacket_print,
    }
};

uint16_t printPacket(PacketHeader* h, char* buf){
    PrintPacketFn fn = print_packet_ops[h->id].print_fn;
    return (*fn)(h,buf);
}
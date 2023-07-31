#include "print_packets.h"
#include <stdio.h>

uint16_t printHeader(PacketHeader* h, char* buf){
    return sprintf(buf,"[id: %d size:%d seq:%d dest:%d src:%d cs:%d]",
    h->id,
    h->size,
    h->seq,
    h->dest_addr,
    h->src_addr,
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
    return sprintf(buf+h_chars,"[eticks: %d ds:%d ms:%d]", p->encoder_ticks, p->desired_speed,p->measured_speed);
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
};

uint16_t printPacket(PacketHeader* h, char* buf){
    PrintPacketFn fn = print_packet_ops[h->id].print_fn;
    return (*fn)(h,buf);
}
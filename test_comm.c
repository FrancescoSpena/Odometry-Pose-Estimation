#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "beth_packets.h"
#include "packet_handler.h"


PacketHandler handler;

MotorStatusPacket packet_status; 

// 0x55 = U

int main(void){
    printf_init();
    PacketHandler_init(&handler);

    packet_status.h.size = sizeof(MotorStatusPacket);
    packet_status.h.id=ID_MOTOR1_STATUS_PACKET;

    packet_status.desired_speed=0;
    packet_status.encoder_ticks=100;
    packet_status.measured_speed=100;



    return 0;
}
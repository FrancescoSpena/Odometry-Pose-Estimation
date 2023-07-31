#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "beth_client.h"
#include "print_packets.h"
#include "../packet_operations.h"

BethClient client;

MotorControlPacket packet = {
        //header
        {
            .id=ID_MOTOR_CONTROL_PACKET,
            .size=sizeof(packet),
            .seq=0,
            .dest_addr=0,
            .src_addr=0,
            .checksum=0,
        },
        .mode=Pid,
        .speed=100
};

PacketOperation motor_control_op = {
    .id=ID_MOTOR_CONTROL_PACKET,
    .size=sizeof(packet),
    .on_receive_fn=recvFn,
    .args=(void*)&packet
};

PacketHandler handler;

int main(void){
    char buf[256];
    BethClient_init(&client,"/dev/ttyACM0",57600);
    PacketHandler_init(&handler);
    PacketHandler_addOperation(&handler,&motor_control_op);

    PacketHeader* h = &packet.h;
    
    while(1){
        BethClient_sendPacket(&client,&packet.h);
        printf("Sent:\n");
        printPacket(h,buf);
        printf("%s\n", buf);
        sleep(1);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "beth_client.h"
#include "print_packets.h"


BethClient client;
PacketHandler handler;

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



int main(void){
    BethClient_init(&client,"/dev/ttyACM0",19200);
    PacketHandler_init(&handler);

    if(client.serial_fd < 0){
        printf("Error opening\n");
        return 0;
    }

    char buf[256];
    PacketStatus status;
    PacketHandler_sendPacket(&handler,&packet.h);
    uint8_t bytes = PacketHandler_txSize(&handler);
    for(int i=0; i < bytes; i++){
        uint8_t c = PacketHandler_popByte(&handler);
        status = PacketHandler_readByte(&handler,c);
        if(status != Success){
            printf("i:%d\n",i);
            break;
        }
    }
    if(status != 1) printf("Status: %d --> parsing error\n",status);
    else printf("Parsing complete\n");
    PacketHeader* h = handler.current_packet;
    printPacket(h,buf);
    printf("Packet:\n");
    printf("%s\n",buf);
    
    return 0;
}
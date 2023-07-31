#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "beth_client.h"
#include "print_packets.h"
#include "../packet_operations.h"
#include <pthread.h>

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

PacketHandler handler;

void* statusRoutine(void* client){
    printf("status routine\n");
    char buf[256];
    BethClient* cli = (BethClient*)client;
    int fd = cli->serial_fd;
    uint8_t c;
    PacketStatus status = UnknownType;
    while(status != ChecksumSuccess){
        int n = read(fd,&c,1);
        if(n){
            status = PacketHandler_readByte(&handler,c);
        }
    }
    printf("Received:\n");
    PacketHeader* h = handler.current_packet;
    printPacket(h,buf);
    printf("%s\n",buf);
    pthread_exit(NULL);
}

int main(void){
    char buf[256];
    BethClient_init(&client,"/dev/ttyACM0",19200);
    PacketHandler_init(&handler);

    pthread_t thread;
    pthread_create(&thread,NULL,statusRoutine,&client);

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
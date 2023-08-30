#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "beth_host.h"
#include "../src/common/print_packets.h"
#include "../src/common/packet_operations.h"
#include <pthread.h>

#define NUM_THREAD 2
int flag_read;


BethHost host;

DifferentialDriveControlPacket packet = {
    {
        .id=DIFFERENTIAL_DRIVE_CONTROL_PACKET,
        .size=sizeof(DifferentialDriveControlPacket),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .translational_velocity=0,
    .rotational_velocity=100,  
};

PacketHandler handler;

void* statusRoutine(void* host){
    char buf[256];
    BethHost* hos = (BethHost*)host;
    int fd = hos->serial_fd;
    uint8_t c;
    PacketStatus status = UnknownType;
    int n;
    while(1){
        while(status != ChecksumSuccess){
            n = read(fd,&c,1);
            if(n){
                status = PacketHandler_readByte(&handler,c);
            }
        }
        status=UnknownType;
        printf("Received:\n");
        PacketHeader* recv = handler.current_packet;
        printPacket(recv,buf);
        printf("%s\n",buf);
    }
}

void* echoRoutine(void* host){
    BethHost* hos = (BethHost*)host;
    int fd = hos->serial_fd;
    uint8_t c;
    while(1){
        int n = read(fd,&c,1);
        if(n){
            printf("ricevuto:");
            printf("%c",c);
            printf("\n");
        }
    }
}

int main(void){
    BethHost_init(&host,"/dev/ttyACM0",19200);
    PacketHandler_init(&handler);


    pthread_t thread;
    pthread_create(&thread,NULL,statusRoutine,&host);

    char buf[256];

    while(1){
        BethHost_sendPacket(&host,&packet.h);
        printPacket(&packet.h,buf);
        printf("Sent:\n");
        printf("%s\n",buf);
        sleep(1);
    }
    return 0;
}
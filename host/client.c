#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "beth_client.h"
#include "print_packets.h"
#include "../packet_operations.h"
#include <pthread.h>

#define NUM_THREAD 2
int flag_read;

BethClient client;

MotorControlPacket packet = {
        //header
        {
            .id=ID_MOTOR_CONTROL_PACKET,
            .size=sizeof(packet),
            .seq=0,
            .dest_addr=0,
            .checksum=0,
        },
        .mode=Pid,
        .speed=100
};

PacketHandler handler;

void* statusRoutine(void* client){
    sleep(1);
    //printf("In status routine\n");
    char buf[256];
    BethClient* cli = (BethClient*)client;
    int fd = cli->serial_fd;
    uint8_t c;
    PacketStatus status = Success;
    int n;
    while(1){
        while(status != ChecksumSuccess){
            n = read(fd,&c,1);
            if(n){
                status = PacketHandler_readByte(&handler,c);
            }
        }
        sleep(1);
        printf("Received:\n");
        PacketHeader* recv = handler.current_packet;
        printPacket(recv,buf);
        printf("%s\n",buf);
    }
}

void* echoRoutine(void* client){
    BethClient* cli = (BethClient*)client;
    int fd = cli->serial_fd;
    uint8_t c;
    while(1){
        int n = read(fd,&c,1);
        if(n){
            printf("ricevuto:");
            /*for(int i=0; i < 8; i++){
                printf("%d", !!((c << i) & 0x80));
            }*/
            printf("%c",c);
            printf("\n");
        }
    }
}

/*
La trasmissione può essere fatta solo con singoli 
char, è possibile iterare su un buffer ma bisogna
sempre fare attenzione con la write 
*/

int main(void){
    BethClient_init(&client,"/dev/ttyACM0",19200);
    PacketHandler_init(&handler);

    if(client.serial_fd < 0){
        printf("Error opening\n");
        return 0;
    }

    pthread_t thread;
    pthread_create(&thread,NULL,echoRoutine,&client);

    char buf[256];

    while(1){
        BethClient_sendPacket(&client,&packet.h);
        printPacket(&packet.h,buf);
        printf("Sent:\n");
        printf("%s\n",buf);
        sleep(1);
    }
    

    return 0;
}
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

MotorStatusPacket packet = {
    {
        .id=ID_MOTOR_STATUS_PACKET,
        .size=sizeof(MotorStatusPacket),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .desired_speed=100,
    .encoder_ticks=0,
    .measured_speed=50,
};

PacketHandler handler;

void* statusRoutine(void* client){
    //printf("In status routine\n");
    char buf[256];
    BethClient* cli = (BethClient*)client;
    int fd = cli->serial_fd;
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
    //PacketHandler_addOperation(&handler,&motor_status_op);


    //pthread_t thread;
    //pthread_create(&thread,NULL,statusRoutine,&client);

    //char buf[256];

    while(1){
        BethClient_sendPacket(&client,&packet.h);
        //printPacket(&packet.h,buf);
        //printf("Sent:\n");
        //printf("%s\n",buf);
        sleep(1);
    }



    

    return 0;
}
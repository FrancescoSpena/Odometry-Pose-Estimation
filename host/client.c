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
            .src_addr=0,
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
    int i = 0;
    while(status != ChecksumSuccess){
        n = read(fd,&c,1);
        if(n){
            status = PacketHandler_readByte(&handler,c);
            i++;
            printf("c:%c\n",c);
        }
        //if(status != Success || status != ChecksumSuccess) break;
    }
    if(status != Success){
        printf("Parsing error --> status:%d\n",status);
        pthread_exit(NULL);
    }
    printf("Received:\n");
    PacketHeader* h = handler.current_packet;
    printPacket(h,buf);
    printf("%s\n",buf);
    flag_read=0;
    pthread_exit(NULL);
}

void* echoRoutine(void* client){
    BethClient* cli = (BethClient*)client;
    int fd = cli->serial_fd;
    uint8_t c;
    while(1){
        int n = read(fd,&c,1);
        if(n){
            printf("ricevuto:\n");
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
    pthread_create(&thread,NULL,statusRoutine,&client);

    char c;
    while(1){
        scanf("%c",&c);
        write(client.serial_fd,&c,1);
    }
    

    return 0;
}
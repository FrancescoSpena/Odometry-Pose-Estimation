#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "beth_client.h"
#include "print_packets.h"
#include "../packet_operations.h"
#include <pthread.h>

#define NUM_THREAD 2

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
    char buf[256];
    BethClient* cli = (BethClient*)client;
    int fd = cli->serial_fd;
    uint8_t c;
    PacketStatus status = UnknownType;
    while(status != ChecksumSuccess){
        int n = read(fd,&c,1);
        //printf("c:%c\n", c);
        if(n){
            status = PacketHandler_readByte(&handler,c);
        }
    }
    //printf("Received:\n");
    PacketHeader* h = handler.current_packet;
    printPacket(h,buf);
    printf("%s\n",buf);
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
            for(int i=0; i < 8; i++){
                printf("%d", !!((c << i) & 0x80));
            }
            printf("\n");
        }
    }
}

int main(void){
    BethClient_init(&client,"/dev/ttyACM0",57600);

    if(client.serial_fd < 0){
        //printf("Error opening\n");
        return 0;
    }

    pthread_t threads[NUM_THREAD];
    pthread_create(&threads[0],NULL,echoRoutine,&client);
    
    
    uint8_t c = 'a';
    int fd = client.serial_fd;
    ssize_t size = -1;
    while(1){
        printf("inviato:\n");
        for(int i=0; i < 8; i++){
            printf("%d", !!((c << i) & 0x80));
        }
        printf("\n");
        size = write(fd,&c,1);
        if(size == -1) break;
        sleep(1);
    }
    return 0;
}
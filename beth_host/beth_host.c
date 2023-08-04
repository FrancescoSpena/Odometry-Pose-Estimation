#include "beth_host.h"
#include "../src/common/packet_handler.h"
#include "serial.h"
#include "../src/common/print_packets.h"
#include <stdio.h>
#include <unistd.h>

#define PACKET_BUFFER_SIZE 8

char buf[256];

//Inizialize the packet handler
void BethHost_init(BethHost* h, char* dev, int speed){
    h->serial_fd=setupSerial(dev,speed);
    if(h->serial_fd < 0){
        return;
    }
    PacketHandler_init(&h->ph);
}

PacketStatus BethHost_sendPacket(BethHost* host, PacketHeader* h){
    PacketHandler handler = host->ph;
    PacketStatus ret = PacketHandler_sendPacket(&host->ph,h);
    int fd = host->serial_fd;
    ssize_t size = 0;
    uint8_t bytes = PacketHandler_txSize(&handler);
    for(int i=0; i < bytes; i++){
        uint8_t c = PacketHandler_popByte(&handler);
        size = write(fd,&c,1);
        if(size == -1){
            return UnknownType;
        }
    }
    return ret;
}

void recvFn(PacketHeader* recvp, void* _args){
    PacketHeader* dest = (PacketHeader*)_args;
    memcpy(dest,recvp,dest->size);
    printPacket(recvp,buf);
    printf("\33[2K");
    printf("%s\r", buf);
    fflush(stdout);
}
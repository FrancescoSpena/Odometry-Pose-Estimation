#pragma once 
#include "../beth_packets.h"
#include "../packet_status.h"
#include "../packet_handler.h"

typedef struct{
    PacketHandler ph;  //packet handler
    int serial_fd; //uart fd
}BethClient;

void BethClient_init(BethClient*, char* dev, int speed);
PacketStatus BethClient_sendPacket(BethClient*, PacketHeader*);
void recvFn(PacketHeader*, void* _args);
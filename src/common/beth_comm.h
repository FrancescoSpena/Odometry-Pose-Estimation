#pragma once 
#include "../beth_firmware/beth_globals.h"
#include "../common/beth_packets.h"
#include "../arch/include/comm_uart.h"
#include "packet_handler.h"
#include <stdint.h>

void BethComm_init(void);
void BethComm_receiveFn(PacketHeader* _h, void* _null);
PacketStatus BethComm_sendPacket(PacketHeader* _h);
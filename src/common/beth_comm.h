#pragma once 
#include "../beth_firmware/beth_globals.h"
#include "../common/beth_packets.h"
#include <stdint.h>

void BethComm_receiveFn(PacketHeader* _h, void*);
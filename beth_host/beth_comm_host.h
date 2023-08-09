#pragma once 
#include "packet_structure.h"
#include "../src/common/beth_packets.h"
#include <stdint.h>

void BethComm_receiveFn(PacketHeader* _h, void* _null);
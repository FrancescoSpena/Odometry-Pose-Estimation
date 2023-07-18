/**
 * beth_joint_internal.h
*/

#pragma once 
#include <stdint.h>
#include "beth_packets.h"


typedef struct{
    MotorControlPacket* control; 
    MotorStatusPacket* status; 
    MotorParamsPacket* params;

    uint8_t enc_idx;
    uint8_t dir; 
    uint8_t output;
}BethJoint;

void BethJoint_init(BethJoint*, MotorControlPacket*,
                    MotorStatusPacket*,
                    MotorParamsPacket*,
                    uint8_t _eidx);
void BethJoint_handle(BethJoint*);
/**
 * beth_joint_internal.h
*/

#pragma once 
#include <stdint.h>
#include "../common/beth_packets.h"


typedef struct{
    MotorControlPacket* control; 
    MotorStatusPacket* status; 
    MotorParamsPacket* params;

    int enc_idx;
    int dir; 
    int output;
}BethJoint;

void BethJoint_init(BethJoint*, MotorControlPacket*,
                    MotorStatusPacket*,
                    MotorParamsPacket*,
                    uint8_t _eidx);
void BethJoint_handle(BethJoint*);
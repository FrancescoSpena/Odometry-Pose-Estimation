/**
 * beth_joint.h
**/


#pragma once 
#include <stdint.h>
#include "beth_joint_internal.h"

#define NUM_JOINTS 2

BethJoint joints[NUM_JOINTS];

void BethJoints_init(void);
void BethJoints_handle(void);
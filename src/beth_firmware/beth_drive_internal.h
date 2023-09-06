/**
 * beth_drive_internal.h
*/

#pragma once
#include "beth_joint_internal.h"
#include "../common/beth_packets.h"

// computes the control from the desired velocities in ctr->control,
// flushes them to the left and right joints ctr->control_left and ctr->control_right
void DifferentialDriveController_control(void);
                        
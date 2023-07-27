/**
 * beth_drive_internal.h
*/

#pragma once
#include "beth_packets.h"
#include "beth_joint_internal.h"

typedef struct DiffDrive{
    DifferentialDriveStatusPacket* status;
    const DifferentialDriveParamPacket* params;
    const DifferentialDriveControlPacket* control;

    MotorStatusPacket* status_left, *status_right;
    MotorControlPacket* control_left, *control_right;
}DifferentialDriveController;

// initializes ctr by filling fields and computing cached quantities
void DifferentialDriveController_init(DifferentialDriveController* ctr,
                                      DifferentialDriveParamPacket* params,
                                      DifferentialDriveControlPacket* control,
                                      DifferentialDriveStatusPacket* status,
                                      BethJoint* motor_left,
                                      BethJoint* motor_right
                                      );
// computes the control from the desired velocities in ctr->control,
// flushes them to the left and right joints ctr->control_left and ctr->control_right
void DifferentialDriveController_control(DifferentialDriveController* ctr);
// resets the platform controller to x,y,theta, and the velocities to 0
void DifferentialDriveController_reset(DifferentialDriveController* ctr,
                                        float x, float y, float theta);
                        
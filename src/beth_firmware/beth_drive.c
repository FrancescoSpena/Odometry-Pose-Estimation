/**
 * beth_drive.c
*/

#include "beth_drive.h"
#include "beth_globals.h"
#include "beth_drive_internal.h"
#include "beth_joint.h"

DifferentialDriveController drive_controller;

void BethDrive_init(void){
    DifferentialDriveController_init(&drive_controller,
                                    &drive_params,
                                    &drive_control,
                                    &drive_status,
                                    &joints[0],
                                    &joints[1]);
    return;
}

void BethDrive_handle(void){
    DifferentialDriveController_control(&drive_controller);
}
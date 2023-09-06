/**
 * beth_drive.c
*/

#include "beth_drive.h"
#include "beth_globals.h"
#include "beth_drive_internal.h"
#include "beth_joint.h"


void BethDrive_handle(void){
    DifferentialDriveController_control();
    BethJoints_handle();
}
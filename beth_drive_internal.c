/**
 * beth_drive_internal.c
*/

#include "beth_drive_internal.h"
#include "uart.h"
#include "stdio.h"

void DifferentialDriveController_init(DifferentialDriveController* ctr,
                                      DifferentialDriveParamPacket* params,
                                      DifferentialDriveControlPacket* control,
                                      DifferentialDriveStatusPacket* status,
                                      BethJoint* motor_left,
                                      BethJoint* motor_right
                                      ){
    ctr->params=params;
    ctr->status=status;
    ctr->control=control;
    ctr->status_left=motor_left->status;
    ctr->control_left=motor_left->control;
    ctr->status_right=motor_right->status;
    ctr->control_right=motor_right->control;
    DifferentialDriveController_reset(ctr,0.,0.,0.);
}

void DifferentialDriveController_control(DifferentialDriveController* ctr){
    float tv_des=ctr->control->translational_velocity; //trans speed
    float rv_des=ctr->control->rotational_velocity;    //rotational speed

    /**
     * Compute this:
     * 
     * omega_r = v/r + (d*omega) / 2*r 
     * omega_l = v/r - (d*omega) / 2*r
    */
    
    float first_term = tv_des / ctr->params->radius_wheel;
    float second_term_num = ctr->params->distance * rv_des;
    float second_term_den = 2*ctr->params->radius_wheel;
    float second_term = second_term_num / second_term_den;

    float total_right = first_term + second_term;
    float total_left = first_term - second_term;
    
    //Write to the single motor 
    ctr->control_right->speed=total_right;
    ctr->control_left->speed=total_left;

    
}

void DifferentialDriveController_reset(DifferentialDriveController* ctr,
                                        float x, float y, float theta){
    ctr->status->odom_x=x;
    ctr->status->odom_y=y;
    ctr->status->odom_theta=theta;
    ctr->status->translational_velocity_measured=0.;
    ctr->status->rotational_velocity_measured=0;
    ctr->status->translational_velocity_desired=0.;
    ctr->status->rotational_velocity_desired=0.;
}
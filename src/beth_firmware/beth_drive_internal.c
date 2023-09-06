/**
 * beth_drive_internal.c
*/

#include "beth_drive_internal.h"
#include "../arch/include/uart.h"
#include "beth_globals.h"
#include <stdio.h>

void DifferentialDriveController_control(void){
    float tv_des=drive_control.translational_velocity; //trans speed
    float rv_des=drive_control.rotational_velocity;    //rotational speed

    /**
     * Compute this:
     * 
     * omega_r = v/r + (d*omega) / 2*r 
     * omega_l = v/r - (d*omega) / 2*r
    */
    
    float first_term = tv_des / drive_params.radius_wheel;  // v / r  
    float second_term_num = drive_params.distance * rv_des;  // d * omega  
    float second_term_den = 2*drive_params.radius_wheel;     // 2 * r = 4 
    float second_term = second_term_num / second_term_den;   // d * omega / 2 * r  


    float total_right = first_term + second_term;  // v/r + d*omega / 2*r  2 + 25 = 27
    float total_left = first_term - second_term;   // v/r - d*omega / 2*r  2 - 25 = -23 
    
    //Write to the single motor 
    motor1_control.speed=total_right;
    motor2_control.speed=total_left;
    return;
}

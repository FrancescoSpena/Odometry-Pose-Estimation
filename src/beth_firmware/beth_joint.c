/**
 * beth_joint.c
*/

#include "beth_joint.h"
#include "beth_joint_internal.h"
#include "beth_globals.h"
#include "../arch/include/encoder.h"
#include "../arch/include/pwm.h"


//Encoder_init, PWM_init and BethJoint_init
void BethJoints_init(void){
    Encoder_init();
    PWM_init();
    BethJoint_init(&joints[0],&motor1_control, &motor1_status, &motor1_params, 2);
    BethJoint_init(&joints[1],&motor2_control, &motor2_status, &motor2_params, 3);
    return;
}

//Send commands to joints
void BethJoints_handle(void){
    Encoder_sample();
    for(int i=0; i < NUM_JOINTS; ++i){
        BethJoint_handle(&joints[i]);
    }
    return;
}
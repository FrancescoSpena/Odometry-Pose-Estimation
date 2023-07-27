/**
 * beth_joint.c
*/

#include "beth_joint.h"
#include "beth_joint_internal.h"
#include "beth_globals.h"
#include "encoder.h"


void BethJoints_init(void){
    Encoder_init();
    BethJoint_init(&joints[0],&motor1_control, &motor1_status, &motor1_params, 3);
    BethJoint_init(&joints[1],&motor2_control, &motor2_status, &motor2_params, 2);
    return;
}

void BethJoints_handle(void){
    Encoder_sample();
    for(int i=0; i < NUM_JOINTS; ++i){
        BethJoint_handle(&joints[i]);
    }
    return;
}
/*
Odometry Internal API 
*/

#include <stdint.h>

#pragma once

typedef struct Odometry{
    float T_s;
    float v_k;
    float omega_k;
    float delta_s;
    float delta_theta;

    //internal 
    uint16_t encoder_left_prev;
    uint16_t encoder_right_prev;
}Odometry;

//Inizialize the structure
void OdometryInit_int(Odometry* o, float Ts);
//Update status' variable for odometry
void OdometryHandle_int(Odometry* o);
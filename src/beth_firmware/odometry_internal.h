/*
Odometry Internal API 
*/

#pragma once

typedef struct Odometry{
    float x_k;
    float y_k;
    float theta_k;
    int T_s;
    float v_k;
    float omega_k;
    float delta_s;
    float delta_theta;
}Odometry;

//Inizialize the structure
void OdometryInit_int(Odometry* o);
//Update status' variable for odometry
void OdometryHandle_int(Odometry* o);
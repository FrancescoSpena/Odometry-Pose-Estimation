#include "odometry_internal.h"
#include "beth_globals.h"
#include "math.h"
#include "../arch/include/encoder.h"

#include "../arch/include/uart.h"
#include <stdio.h>

void OdometryInit_int(Odometry* o, float Ts){
    o->omega_k = 0;
    if(Ts != 0) o->T_s = Ts;
    else o->T_s = 1;
    o->v_k = 0;
    o->delta_s = 0;
    o->delta_theta = 0;

    drive_status.odom_x = 0;
    drive_status.odom_y = 0;
    drive_status.odom_theta = 0;
    return;
}

void OdometryPredict(Odometry* o){
    int idx_r = 3;  //motor dx --> encoder 3
    int idx_l = 2;  //motor sx --> encoder 2 
    
    float r = drive_params.radius_wheel;
    float d = drive_params.distance;

    uint16_t enc_r = Encoder_getValue(idx_r);
    uint16_t enc_l = Encoder_getValue(idx_l);

    //Convert PPR in rad/s
    //rad/s = (PPR * 2 * π * Velocità di Rotazione in Giri al Secondo) / 1,000
    float speed_rot = 0;
    float speed_rot_right = 0;

    const float radiant = 2 * 3.14;

    //rad/s --> giri/sec
    speed_rot = motor1_control.speed / radiant;
    speed_rot_right = motor2_control.speed / radiant;

    enc_l = ((enc_r * radiant * speed_rot) / 1000);
    enc_r = ((enc_r * radiant * speed_rot_right) / 1000);

    uint16_t summ_enc = enc_r + enc_l;
    uint16_t diff_enc = enc_r - enc_l;

    o->delta_s = (r / 2) * summ_enc;
    o->delta_theta = (r / d) * diff_enc;

    o->v_k = (o->delta_s / o->T_s);
    o->omega_k = (o->delta_theta / o->T_s);
    return;
}

void OdometryHandle_int(Odometry* o){
    //cos(k) sin(k)  prec 
    double cos_k = cos(drive_status.odom_theta);
    double sin_k = sin(drive_status.odom_theta);

    //compute theta(k+1)
    drive_status.odom_theta = drive_status.odom_theta + (o->omega_k*o->T_s);
        
    //cos(k+1) sin(k+1)
    double cos_k1 = cos(drive_status.odom_theta);
    double sin_k1 = sin(drive_status.odom_theta);

    //Predict value of v_k and omega_k
    OdometryPredict(o);

    float sin_diff = (sin_k1 - sin_k);
    float cos_diff = (cos_k1 - cos_k);
    float first_term = o->v_k / o->omega_k;

    drive_status.odom_x = (drive_status.odom_x + (first_term * sin_diff));
    drive_status.odom_y = (drive_status.odom_y - (first_term * cos_diff));
    
    if(isnan(drive_status.odom_x) || isnan(drive_status.odom_y) || isnan(drive_status.odom_theta)){
        drive_status.odom_x = 0;
        drive_status.odom_y = 0;
        drive_status.odom_theta = 0;
    }
    
    return;
}
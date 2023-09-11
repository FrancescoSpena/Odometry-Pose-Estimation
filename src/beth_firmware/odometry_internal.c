#include "odometry_internal.h"
#include "beth_globals.h"
#include "math.h"
#include "../arch/include/encoder.h"

void OdometryInit_int(Odometry* o, float Ts){
    o->omega_k = 0;
    if(Ts != 0) o->T_s = Ts;
    else o->T_s = 1;
    o->theta_k = 0;
    o->v_k = 0;
    o->x_k = 0;
    o->y_k = 0;
    o->delta_s = 0;
    o->delta_theta = 0;
    return;
}

void OdometryPredict(Odometry* o){
    int idx_r = 3;  //motor dx --> encoder 3
    int idx_l = 2;  //motor sx --> encoder 2 
    
    float r = drive_params.radius_wheel;
    float d = drive_params.distance;

    const float radian = 0.0174;
    
    uint16_t enc_r = Encoder_getValue(idx_r);
    uint16_t enc_l = Encoder_getValue(idx_l);
    
    //convert degree to radian
    enc_r = enc_r * radian;
    enc_l = enc_l * radian;
    
    uint16_t summ_enc = enc_r + enc_l;
    uint16_t diff_enc = enc_r - enc_l;
    
    
    o->delta_s = (r / 2) * summ_enc;
    o->delta_theta = (r / d) * diff_enc;
    
    o->v_k = (o->delta_s / o->T_s);
    o->omega_k = (o->delta_theta / o->T_s);
    return;
}

void OdometryHandle_int(Odometry* o){
    float T_s = o->T_s;
    float omega_k = o->omega_k;
    float v_k = o->v_k;

    //cos(k) sin(k)
    double cos_k = cos((double)o->theta_k);
    double sin_k = sin((double)o->theta_k);

    drive_status.odom_theta = o->theta_k + (omega_k*T_s);
    
    //cos(k+1) sin(k+1)
    double cos_k1 = cos((double)drive_status.odom_theta);
    double sin_k1 = sin((double)drive_status.odom_theta);

    //Predict value of v_k and omega_k
    OdometryPredict(o);

    float sin_diff = (float)(sin_k1 - sin_k);
    float cos_diff = (float)(cos_k1 - cos_k);
    float first_term = v_k / omega_k;

    drive_status.odom_x = o->x_k + (first_term * sin_diff);
    drive_status.odom_y = o->y_k - (first_term * cos_diff);
    return;
}
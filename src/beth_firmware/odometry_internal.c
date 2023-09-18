#include "odometry_internal.h"
#include "beth_globals.h"
#include "math.h"
#include "../arch/include/encoder.h"

void OdometryInit_int(Odometry* o, float Ts){
    o->omega_k = 0;
    if(Ts != 0) o->T_s = Ts;
    else o->T_s = 1;
    o->v_k = 0;
    o->delta_s = 0;
    o->delta_theta = 0;
    o->val_left_prec_enc = 0;
    o->val_right_prec_enc = 0;

    drive_status.odom_x = 0;
    drive_status.odom_y = 0;
    drive_status.odom_theta = 0;

    return;
}

void OdometryPredict(Odometry* o){
    //value in m 
    float r = drive_params.radius_wheel / 100;
    float d = drive_params.distance / 100;
    float k = 0.021;

    //ticks/s 
    int16_t delta_r = motor2_status.measured_speed;
    int16_t delta_l = motor1_status.measured_speed;

    //ticks / s
    float delta_puro_r = (float)(delta_r / o->T_s);
    float delta_puro_l = (float)(delta_l / o->T_s);
    
    //convert to m/s
    delta_puro_r *= k;
    delta_puro_l *= k;

    //equation
    float summ_enc = delta_puro_r + delta_puro_l;
    float diff_enc = delta_puro_r - delta_puro_l;

    o->delta_s = (r / 2) * summ_enc;
    o->delta_theta = (r / d) * diff_enc;

    o->v_k = (o->delta_s / o->T_s);
    o->omega_k = (o->delta_theta / o->T_s);

    if(o->omega_k < 0.1) o->omega_k = 0;

    return;
}

void OdometryHandle_int(Odometry* o){
    //Predict value of v_k and omega_k
    OdometryPredict(o);

    //Eulero
    if(o->omega_k == 0){
        float sin_k = sin(drive_status.odom_theta);
        float cos_k = cos(drive_status.odom_theta);
        drive_status.odom_x = drive_status.odom_x + (o->v_k * o->T_s * cos_k);
        drive_status.odom_y = drive_status.odom_y + (o->v_k * o->T_s * sin_k);
        drive_status.odom_theta = drive_status.odom_theta + (o->omega_k * o->T_s);
    }else{
        //cos(k) sin(k)
        float sin_k = sin(drive_status.odom_theta);
        float cos_k = cos(drive_status.odom_theta);

        //theta(k+1)
        drive_status.odom_theta = drive_status.odom_theta + o->omega_k*o->T_s;

        //cos(k+1) sin(k+1)
        float sin_k1 = sin(drive_status.odom_theta);
        float cos_k1 = cos(drive_status.odom_theta);

        /*
        sin(k+1) - sin(k)
        cos(k+1) - cos(k)
        */
        float diff_sin = sin_k1 - sin_k;
        float diff_cos = cos_k1 - cos_k;

        //v_k / omega_k
        float fract = o->v_k / o->omega_k;

        //update X and Y 
        drive_status.odom_x = drive_status.odom_x + (fract * diff_sin);
        drive_status.odom_y = drive_status.odom_y - (fract * diff_cos);
    }
    return;
}
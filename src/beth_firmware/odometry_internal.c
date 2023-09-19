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
    o->encoder_left_prev = 0;
    o->encoder_right_prev = 0;
    return;
}

static const float cos_coeffs[]={0., 0.5 ,  0.    ,   -1.0/24.0,   0.    , 1.0/720.0};
static const float sin_coeffs[]={1., 0.  , -1./6. ,      0.    ,   1./120, 0.   };


// computes the terms of sin(theta)/thera and (1-cos(theta))/theta
// using taylor expansion
static void _computeThetaTerms(float* sin_theta_over_theta,
                               float* one_minus_cos_theta_over_theta,
                               float theta) {
  // evaluates the taylor expansion of sin(x)/x and (1-cos(x))/x,
  // where the linearization point is x=0, and the functions are evaluated
  // in x=theta
  *sin_theta_over_theta=0;
  *one_minus_cos_theta_over_theta=0;
  float theta_acc=1;
  for (uint8_t i=0; i<6; i++) {
    if (i&0x1)
      *one_minus_cos_theta_over_theta+=theta_acc*cos_coeffs[i];
    else 
      *sin_theta_over_theta+=theta_acc*sin_coeffs[i];
    theta_acc*=theta;
  }
}


void OdometryHandle_int(Odometry* o){
    //baseline
    float b = drive_params.distance;
    
    //factor wheel 
    float k = 0.0004491656163220702;
    
    
    int16_t left_ticks=motor1_status.measured_speed;
    int16_t right_ticks=motor2_status.measured_speed;
    
    

    
    if(left_ticks != 0 || right_ticks != 0){
        //left and right motion
        float delta_l = k * left_ticks;
        float delta_r = k * right_ticks;

        float delta_plus=delta_r+delta_l;
        float delta_minus=delta_r-delta_l;
        float dth= delta_minus * b;
        float one_minus_cos_theta_over_theta, sin_theta_over_theta;
        _computeThetaTerms(&sin_theta_over_theta, &one_minus_cos_theta_over_theta, dth);
        float dx=.5*delta_plus*sin_theta_over_theta;
        float dy=.5*delta_plus*one_minus_cos_theta_over_theta;

        //apply the increment to the previous estimate
        float s=sin(drive_status.odom_theta);
        float c=cos(drive_status.odom_theta);
        drive_status.odom_x+=c*dx-s*dy;
        drive_status.odom_y+=s*dx+c*dy;
        drive_status.odom_theta+=dth;
        // normallize theta;
        if (drive_status.odom_theta>M_PI){
            drive_status.odom_theta-=2*M_PI;
        }
        else if (drive_status.odom_theta<-M_PI){
            drive_status.odom_theta+=2*M_PI;
        }
    }
    
    
    return;
}
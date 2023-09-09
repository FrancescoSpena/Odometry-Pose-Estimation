#include "odometry.h"

Odometry odom;


//Init Odometry
void Odometry_init(int Ts){
    return OdometryInit_int(&odom,Ts);
}


//Side Effect to status packet
void Odometry_handle(void){
    return OdometryHandle_int(&odom);
}
#include "odometry.h"

Odometry odom;


//Init Odometry
void Odometry_init(void){
    OdometryInit_int(&odom);
    return;
}


//Side Effect to status packet
void Odometry_handle(void){
    OdometryHandle_int(&odom);
    return;
}
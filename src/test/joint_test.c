#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

#include "../beth_firmware/beth_drive.h"
#include "../beth_firmware/beth_joint.h"
#include "../common/beth_comm.h"
#include "../arch/include/timer.h"


/**
 * Encoder --> pin 13,12 
 * Motor1 --> dir_a=5, dir_b=6, pwm=4
 * Motor2 --> dir_a=7, dir_b=8, pwm=9
*/

volatile uint8_t comm_flag=0;

void timerCommFn(void) {
  comm_flag=1;
}

void commFn(void){
    BethComm_sendPacket(&motor1_status.h);
    comm_flag = 0;
}

int main(void){
    BethComm_init();
    BethJoints_init();

    struct Timer* timer_comm=Timer_create(1000, (void*)&timerCommFn, 0);
    Timer_start(timer_comm);

    while(1){
        if(comm_flag)
            commFn();
        BethJoints_handle();
    }
}
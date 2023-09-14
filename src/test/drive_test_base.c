#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "../arch/include/uart.h"
#include "../beth_firmware/beth_drive.h"
#include "../beth_firmware/beth_joint.h"
#include "../arch/include/comm_uart.h"
#include "../arch/include/timer.h"
#include "../arch/include/pins.h"
#include "../arch/include/digio.h"
#include "../arch/include/pwm.h"
#include "../common/packet_handler.h"
#include "../beth_firmware/beth_globals.h"
#include "../common/beth_comm.h"
#include "../beth_firmware/odometry.h"

volatile uint8_t comm_flag=0;

void timerCommFn(void) {
    comm_flag=1;
}

void commFn(void){
    //common
    BethComm_handle();
    //BethComm_sendPacket(&drive_status.h);
    //platform
    BethDrive_handle();
    comm_flag = 0;
}

int main(void){
    Odometry_init(0.01);
    BethJoints_init();
    BethComm_init();
    Timer_init();

    struct Timer* timer_comm=Timer_create(10, (void*)&timerCommFn, 0);
    Timer_start(timer_comm);

    while(1){
        if(comm_flag){
            commFn();
        }
    }

}
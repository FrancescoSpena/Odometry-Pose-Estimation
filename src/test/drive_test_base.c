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

#define COMMON

volatile uint8_t comm_flag=0;

#ifdef COMMON
void timerCommFn(void) {
  comm_flag=1;
}

void commFn(void){
    BethComm_handle();
    //BethComm_sendPacket(&drive_control.h);
    BethComm_sendPacket(&drive_status.h);
    //BethComm_sendPacket(&drive_params.h);
    comm_flag = 0;
}
#endif


int main(void){
    BethJoints_init();
    BethComm_init();
    #ifdef COMMON
    Timer_init();

    struct Timer* timer_comm=Timer_create(10, (void*)&timerCommFn, 0);
    Timer_start(timer_comm);
    #endif

    while(1){
        #ifdef COMMON
        if(comm_flag)
            commFn();
        #endif
        BethDrive_handle();
    }

}
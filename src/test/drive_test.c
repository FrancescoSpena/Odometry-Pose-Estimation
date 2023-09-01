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

#define BAUND1 19200
#define BAUND2 115200

#define PIN 13

volatile uint8_t comm_flag=0;

PacketHandler handler;
struct Uart* uart;

void timerCommFn(void) {
  comm_flag=1;
}

void statusRoutine(struct Uart* uart){
    PacketStatus status = UnknownType;
    uint8_t c;
    if(Uart_available(uart)){
        while(status != ChecksumSuccess){
            c = Uart_read(uart);
            status = PacketHandler_readByte(&handler,c);
        }
        system_status.rx_packets++;
        status = UnknownType;
    }
}

void commFn(void){
    statusRoutine(uart);
    BethDrive_handle();
    BethJoints_handle();
    comm_flag = 0;
}

int main(void){
    uart = Uart_init(BAUND2);
    PacketHandler_init(&handler);
    PacketHandler_addOperation(&handler,&diff_drive_control_op);
    //BethJoints_init();
    //BethDrive_init();
    BethComm_init();
    Timer_init();

    struct Timer* timer_comm=Timer_create(10, (void*)&timerCommFn, 0);

    Timer_start(timer_comm);

    while(1){
        BethComm_sendPacket(&system_status.h);
        if(comm_flag){
            commFn();
        }
    }

}
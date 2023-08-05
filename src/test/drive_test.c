#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "../arch/include/uart.h"
#include "../beth_firmware/beth_drive.h"
#include "../beth_firmware/beth_joint.h"
#include "../arch/include/comm_uart.h"
#include "../common/packet_handler.h"
#include "../beth_firmware/beth_globals.h"
#include "../common/beth_comm.h"

PacketHandler handler;

void echoRoutine(struct Uart* uart){
    uint8_t c;
    while(1){
        if(Uart_available(uart)){
            c = Uart_read(uart);
            Uart_write(uart,c);
        }
    } 
}

void statusRoutine(struct Uart* uart){
    PacketStatus status = UnknownType;
    uint8_t c;
    if(Uart_available(uart)){
        while(status != ChecksumSuccess){
            c = Uart_read(uart);
            status = PacketHandler_readByte(&handler,c);
            Uart_write(uart,c);
        }
        status = UnknownType;
    }
    _delay_ms(1000);
}

int main(void){
    struct Uart* uart = Uart_init(19200);
    PacketHandler_init(&handler);
    PacketHandler_addOperation(&handler,&diff_drive_control_op);
    BethJoints_init();
    BethDrive_init();

    while(1){
        statusRoutine(uart);
        BethDrive_handle();
        BethJoints_handle();
    }
}
#include "comm_uart.h"
#include "packet_handler.h"
#include <stdio.h>
#include "print_packets.h"
#include "beth_packets.h"
#include "packet_operations.h"
#include <string.h>
#include <util/delay.h>
#include "uart.h"
#include "beth_comm.h"
#include <stdlib.h>

/*Digitare Enter viene letto come un carattere*/

PacketHandler handler;

PacketHeader* header;

PacketOperation motor_contro_op = {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(MotorControlPacket),
        .on_receive_fn=BethComm_receiveFn,
        .args=&header,
};

int main(void){
    struct Uart* uart = Uart_init(19200);
    PacketHandler_init(&handler);
    //PacketHandler_addOperation(&handler,&motor_contro_op);
    PacketStatus status = UnknownType;
    uint8_t c;
    while(1){
        if(Uart_available(uart)){
            while(status != ChecksumSuccess){
                c = Uart_read(uart);
                status = PacketHandler_readByte(&handler,c);
                //Uart_write(uart,c);
            }
            status = UnknownType;
            BethComm_receiveFn(handler.current_packet,0);
            uint8_t mode = motor1_control.mode;
            uint8_t speed = motor1_control.speed;
            char str_mode;
            char str_speed;
            Uart_write(uart,'m');
            itoa(mode,&str_mode,10);
            itoa(speed,&str_speed,10);
            Uart_write(uart,str_mode);
            Uart_write(uart,'s');
            Uart_write(uart,str_speed);  
        }
        _delay_ms(500);
    }
}
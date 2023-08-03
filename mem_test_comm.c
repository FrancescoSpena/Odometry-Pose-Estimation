#include "comm_uart.h"
#include "packet_handler.h"
#include "beth_comm.h"
#include <util/delay.h>
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>


PacketHandler handler;
PacketHeader* header;

PacketOperation motor_contro_op = {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(MotorControlPacket),
        .on_receive_fn=BethComm_receiveFn,
        .args=&header,
};

MotorControlPacket packet = {
        //header
        {
            .id=ID_MOTOR_CONTROL_PACKET,
            .size=sizeof(packet),
            .seq=0,
            .dest_addr=0,
            .checksum=0,
        },
        .mode=Pid,
        .speed=1
};

void parsingPacket(struct Uart* uart){
    PacketStatus status = UnknownType;
    uint8_t c;
    while(1){
        if(Uart_available(uart)){
            while(status != ChecksumSuccess){
                c = Uart_read(uart);
                status = PacketHandler_readByte(&handler,c);
                Uart_write(uart,c);
            }
            status = UnknownType;  
        }
    }
}

int main(void){
    //printf_init();
    PacketHandler_init(&handler);
    struct Uart* uart = Uart_init(19200);

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

    }


    return 0; 
}
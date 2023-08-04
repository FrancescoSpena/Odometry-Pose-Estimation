#include "comm_uart.h"
#include "packet_handler.h"
#include "beth_comm.h"
#include <util/delay.h>
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>


PacketHandler handler;
PacketHeader* header;

PacketOperation motor_control_op = {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(MotorControlPacket),
        .on_receive_fn=BethComm_receiveFn,
        .args=&header,
};

PacketOperation motor_status_op = {
    .id=ID_MOTOR_STATUS_PACKET,
    .size=sizeof(MotorStatusPacket),
    .on_receive_fn=BethComm_receiveFn,
    .args=&header,
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
            //BethComm_receiveFn(handler.current_packet,0);
            //printf("des speed:%d\t measured speed:%d\n",motor1_status.desired_speed,motor1_status.measured_speed);
        }
        _delay_ms(500);
    }
}

int main(void){
    printf_init();
    struct Uart* uart = Uart_init(19200);
    PacketHandler_init(&handler);
    //PacketHandler_addOperation(&handler,&motor_status_op);
    while(1){
        parsingPacket(uart);
    }

    return 0; 
}
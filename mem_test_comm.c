#include "comm_uart.h"
#include "packet_handler.h"
#include "beth_comm.h"
#include <util/delay.h>
#include "uart.h"
#include <stdio.h>

PacketHandler handler;
PacketHeader* header;

PacketOperation motor_contro_op = {
        .id=ID_MOTOR_CONTROL_PACKET,
        .size=sizeof(MotorControlPacket),
        .on_receive_fn=BethComm_receiveFn,
        .args=&header,
};

int main(void){
    printf_init();
    header = handler.current_packet;
    struct Uart* uart = Uart_init(19200);
    PacketHandler_init(&handler);
    PacketHandler_addOperation(&handler,&motor_contro_op);
    PacketStatus status = UnknownType;
    uint8_t c;

    return 0; 
}
#include "comm_uart.h"
#include "packet_handler.h"
#include "uart.h"
#include <stdio.h>
#include "print_packets.h"
#include "beth_packets.h"
#include "packet_operations.h"
#include <string.h>


Uart uart;
PacketHandler handler;


int main(void){
    printf_init();
    Uart_init(uart);
    PacketHandler_init(&handler);

    while(1){
        if(Uart_available(&uart) > 1){
            uint8_t c = Uart_read(&uart);
            PacketHandler_readByte(&handler,c);
        }
    }
}
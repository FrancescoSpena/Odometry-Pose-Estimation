#include "comm_uart.h"
#include "packet_handler.h"
#include "uart.h"
#include <stdio.h>
#include "print_packets.h"
#include "beth_packets.h"
#include "packet_operations.h"
#include <string.h>
#include <util/delay.h>


Uart uart;
PacketHandler handler;


int main(void){
    printf_init();
    Uart_init(uart);
    PacketHandler_init(&handler);

    PacketStatus status = UnknownType;

    while(1){
        if(Uart_available(&uart) >= 1){
            while(status != ChecksumSuccess){
                uint8_t c = Uart_read(&uart);
                status = PacketHandler_readByte(&handler,c);
                if(status != Success) break;
            }
            while(handler.rx_buffer){
                Uart_write(&uart,handler.rx_buffer);
                //increment buffer (?) no idea 
            }
        }
    } 
    return 0;
}
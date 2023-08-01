#include "comm_uart.h"
#include "packet_handler.h"
#include <stdio.h>
#include "print_packets.h"
#include "beth_packets.h"
#include "packet_operations.h"
#include <string.h>
#include <util/delay.h>


int main(void){
    struct Uart* uart = Uart_init();

    while(1){
        if(Uart_available(uart) > 1){
            Uart_write(uart,Uart_read(uart));
        }
    }
}
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
        uint8_t ava = Uart_available(uart);
        if(ava != 0){
            Uart_write(uart,ava);
        }else Uart_write(uart,'0');
        _delay_ms(1000);
    }
}
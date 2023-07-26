#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#include "pins.h"
#include "digio.h"
#include "timer.h"

volatile uint8_t flag=0;

#define DURATION_MS 3000
#define PIN_LED 13


void fn(void*f){
    flag = 1;
}


int main(void){
    printf_init();
    Timer_init();

    struct Timer* up = Timer_create(DURATION_MS,&fn,0);

    Timer_start(up);

    digio_configurePin(PIN_LED,Output);


    while(1){
        if(flag == 1){
            printf("Led high\n");
            digio_setPin(PIN_LED,High);
            _delay_ms(500);
            flag = 0;
        }
        digio_setPin(PIN_LED,Low);
    }

}

     
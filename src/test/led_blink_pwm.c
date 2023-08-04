#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "pwm.h"
#include "pins.h"
#include "digio.h"

#define PIN 4

//9 ok, 13 ok (con mio setting), 10 ok, 4 ok

//12 no ok, 11 no ok, 8 no ok, 7 no ok
//6 no ok, 5 no ok, 3 no ok, 2 no ok

int main(void){
    printf_init();
    PWM_init();

    digio_configurePin(PIN,Output);
    PWM_enablePin(PIN);

    uint8_t intensity = 150;
    while(1){
        PWM_setOutput(PIN,intensity);
        //printf("intensity led = %u\n",intensity);
        //_delay_ms(50);
        //intensity+=20;
    }
}
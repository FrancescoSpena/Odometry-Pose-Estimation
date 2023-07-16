#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "pwm.h"
#include "pins.h"
#include "digio.h"

int main(void){
    printf_init();
    PWM_init();

    digio_configurePin(13,Output);
    PWM_enablePin(13);

    uint8_t intensity = 0;
    while(1){
        PWM_setOutput(13,intensity);
        printf("intensity led = %u\n",intensity);
        _delay_ms(500);
        intensity+=10;
    }
}
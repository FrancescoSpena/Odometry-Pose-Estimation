#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "pwm.h"
#include "pins.h"
#include "digio.h"
#include "encoder.h"

//Pin 13,12 --> encoder 3   
//Channel B pin 13, Channel A pin 12 
//Max val encoder 65500


void routine(int16_t prev, int16_t curr){
    if(prev == curr)
        return;
    if(prev > curr)
        printf("Avanti\n");
    else 
        printf("Indietro\n");
}


int main(void){
    printf_init();
    printf("Serial init\n");
    PWM_init();
    printf("PWM init\n");
    Encoder_init();
    printf("Encoder init\n");

    int16_t prev = 0;
    int16_t curr = 0;

    printf("Encoder system = %d\n",Encoder_numEncoders());
    while(1){
        Encoder_sample();
        printf("Encoder value = %u\n",Encoder_getValue(3));
        curr = Encoder_getValue(3);
        routine(prev,curr);
        prev = curr;
        _delay_ms(500);
    }
}
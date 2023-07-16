#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "pwm.h"
#include "pins.h"
#include "digio.h"
#include "encoder.h"

void routine_diff(uint16_t prev, uint16_t curr){
    if(prev == curr)
        return;
    printf("diff = %u\n", (int16_t)(curr - prev));
}

int main(void){

    printf_init();
    Encoder_init();

    uint16_t prev = 0;
    uint16_t curr = 0;


    while(1){
        Encoder_sample();
        printf("Encoder value = %d\n", Encoder_getValue(3));
        curr = Encoder_getValue(3);
        routine_diff(prev,curr);
        prev = curr;
        _delay_ms(500);
    }
}
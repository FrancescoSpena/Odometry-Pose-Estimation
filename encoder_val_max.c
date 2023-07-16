#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "pwm.h"
#include "pins.h"
#include "digio.h"
#include "encoder.h"


int main(void){
    printf_init();
    printf("Serial init\n");
    Encoder_init();
    printf("Encoder init\n");

    uint16_t curr = 0;
    uint16_t max = 0;
    while(1){
        Encoder_sample();
        printf("Encoder value = %u\n", Encoder_getValue(3));
        curr = Encoder_getValue(3);
        if(curr > max){
            max = curr;
        }
        printf("Max value = %u\n",max);
        _delay_ms(500);
    }
}
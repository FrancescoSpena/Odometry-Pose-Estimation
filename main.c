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

int main(void){
    printf_init();
    printf("Serial init\n");
    PWM_init();
    printf("PWM init\n");
    Encoder_init();
    printf("Encoder init\n");

    

    printf("Encoder system = %d\n",Encoder_numEncoders());
    while(1){
        Encoder_sample();
        
        printf("Encoder value = %u\n",Encoder_getValue(3));
        
        _delay_ms(500);
    }
}
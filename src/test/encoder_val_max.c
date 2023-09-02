#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "../arch/include/uart.h"
#include "../arch/include/pins.h"
#include "../arch/include/digio.h"
#include "../arch/include/pwm.h"
#include "../arch/include/encoder.h"


//index 3 = 13,12 (pin)
//index 2 = 11,10 (pin)

int main(void){
    printf_init();
    printf("Serial init\n");
    Encoder_init();
    printf("Encoder init\n");

    while(1){
        Encoder_sample();
        printf("Encoder value = %u\n", Encoder_getValue(2));
        _delay_ms(500);
    }
}
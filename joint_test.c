#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "pwm.h"
#include "pins.h"
#include "digio.h"
#include "encoder.h"
#include "beth_joint.h"

int main(void){

    printf_init();
    printf("Print init\n");
    Encoder_init();
    printf("Encoder init\n");
    BethJoints_init();
    //PWM_init();

    digio_configurePin(6,Output);
    digio_configurePin(5,Output);
    digio_configurePin(4,Output);

    printf("Go\n");
    while(1){
        Encoder_sample();
        printf("Val encoder = %u\n", Encoder_getValue(3));
        digio_setPin(6,1);
        digio_setPin(5,0);
        digio_setPin(3,1);
    }
}
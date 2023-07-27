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

#define PIN 6

int main(void){
    printf_init();

    digio_configurePin(5,Output);
    digio_configurePin(6,Output);
    digio_configurePin(7,Output);
    digio_configurePin(8,Output);

    while(1){
        digio_setPin(5,High);
        digio_setPin(6,High);
        digio_setPin(7,High);
        digio_setPin(8,High);
    }
}
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

#define PIN 13

int main(void){
    printf_init();

    digio_configurePin(PIN,Output);

    while(1){
        digio_setPin(PIN,High);
    }
}
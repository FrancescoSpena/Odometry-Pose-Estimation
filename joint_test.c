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
    Encoder_init();
    BethJoints_init();

    while(1){
        BethJoints_handle();
    }
}
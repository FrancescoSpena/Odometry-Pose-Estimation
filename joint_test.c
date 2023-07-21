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

#define PWM1 4
#define PWM2 9

/**
 * Encoder --> pin 13,12 
 * Motor1 --> dir_a=5, dir_b=6, pwm=4
 * Motor2 --> dir_a=7, dir_b=8, pwm=9
*/

int main(void){

    printf_init();
    printf("Print init\n");
    Encoder_init();
    printf("Encoder init\n");
    BethJoints_init();
    PWM_init();

    PWM_enablePin(PWM1);
    PWM_enablePin(PWM2);
    printf("Enable pin for PWM\n");


    printf("Go\n");
    while(1){
        printf("Val encoder = %d\n", Encoder_getValue(3));
        BethJoints_handle();
    }
}
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
#include "beth_drive.h"


/**
 * Encoder --> pin 13,12 
 * Motor1 --> dir_a=5, dir_b=6, pwm=4
 * Motor2 --> dir_a=7, dir_b=8, pwm=9
*/

int main(void){
    printf_init();
    BethJoints_init();
    //BethDrive_init();

    //BethDrive_handle();

    while(1){
        //Encoder_sample();
        BethJoints_handle();
        //printf("Encoder 1 = %d\t Encoder 2 = %d\n", Encoder_getValue(3),Encoder_getValue(2));
        
    }
}
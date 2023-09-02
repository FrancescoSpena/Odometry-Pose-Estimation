#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

#include "../beth_firmware/beth_drive.h"
#include "../beth_firmware/beth_joint.h"
#include "../common/beth_comm.h"


/**
 * Encoder --> pin 13,12 
 * Motor1 --> dir_a=5, dir_b=6, pwm=4
 * Motor2 --> dir_a=7, dir_b=8, pwm=9
*/

int main(void){
    BethJoints_init();

    while(1){
        BethJoints_handle();
    }
}
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "beth_drive.h"
#include "beth_joint.h"

int main(void){
    printf_init();
    BethJoints_init();
    BethDrive_init();

    printf("Go test drive\n");

    while(1){
        BethDrive_handle();
        BethJoints_handle();
    }
}
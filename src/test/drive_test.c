#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

#include "beth_drive.h"
#include "beth_joint.h"

DifferentialDriveControlPacket packet = {
    {
        .id=DIFFERENTIAL_DRIVE_CONTROL_PACKET,
        .size=sizeof(DifferentialDriveControlPacket),
        .seq=0,
        .dest_addr=0,
        .checksum=0,
    },
    .translational_velocity=0,
    .rotational_velocity=0,
};

void* readJoystick(void* _fd){
    struct js_event e;
    int fd = *(int*)_fd;
    while(1){
        while(read(fd,&e,sizeof(e)) > 0){
            if(e.number == AXYS0 && e.type == TYPE2){
                packet.translational_velocity=abs(e.value%255);
            }
            if(e.number == AXYS3 && e.type == TYPE2){
                packet.rotational_velocity=abs(e.value%255);
            }
        }
    }
}

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
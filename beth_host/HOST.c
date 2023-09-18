//Standard
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/joystick.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Platform
#include "beth_host.h"
#include "../src/common/print_packets.h"
#include "../src/common/packet_operations.h"
#include "../src/common/joystick.h"
#include "packet_structure.h"
#include "beth_comm_host.h"

//Math
#include "math.h"

//Speed Serial
#define BAUND1 19200

//Global variable 
volatile int flag_comm = 0;
BethHost host;

//Packet control
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

//Function to read a serial
void* statusRoutine(void* host){
    BethHost* hos = (BethHost*)host;
    int fd = hos->serial_fd;
    uint8_t c;
    PacketStatus status = UnknownType;
    int n;
    while(1){
        while(status != ChecksumSuccess){
            n = read(fd,&c,1);
            if(n){
                status = PacketHandler_readByte(&hos->ph,c);
            }
        }
        status=UnknownType;
        char buf[256];
        printPacket(hos->ph.current_packet,buf);
        printf("Received:\n %s\n",buf);
        
        FILE* file;
        file = fopen("data.txt","a");
        DifferentialDriveStatusPacket* status = (DifferentialDriveStatusPacket*)hos->ph.current_packet;
        if(!isnan(status->odom_x) && !isnan(status->odom_y)){
            float x = status->odom_x;
            float y = status->odom_y;
            fprintf(file,"%f %f\n",x,y);
        }
        fclose(file);
        usleep(10000);
    }
}


//Function to read joystick and make a packet
void* readJoystickRoutine(void* _fd){
    int fd = *(int*)_fd;
    int v_x = 0;
    int v_y = 0;
    int v_c = 0;
    float v_x_norm;
    float v_y_norm;
    while(1){
        int left = readJoystick(fd,GYROSCOPE_AXYSX_LEFT,&v_x);
        int right = readJoystick(fd,GYROSCOPE_AXYSY_RIGHT,&v_y);
        int center = readJoystick(fd,BUTTON_X,&v_c);
        if(left == 0){
            v_x_norm = -((float)v_x) / 32767;
            packet.translational_velocity=v_x_norm*255; 
        }
        if(right == 0){
            v_y_norm = ((float)v_y) / 32767;
            packet.rotational_velocity=v_y_norm*7; 
        }
        if(center == 0){
            packet.translational_velocity = 0;
            packet.rotational_velocity = 0;
        }
        flag_comm = 1;
        usleep(10000);
    }
}

//Function to print a packet
void printPacketSend(PacketHeader* h){
    char buf[256];
    printPacket(h,buf);
    printf("Sent:\n %s\n",buf);
    return;
}

//MAIN
int main(void){
    //Init Host and Handler
    BethHost_init(&host,"/dev/ttyACM0",BAUND1);
    //Open Joystick
    int fd_joy = openJoystick("/dev/input/js1");

    if(fd_joy < 0){
        printf("Joystick non disponibile\n");
        return 0;
    }
    
    //Create thread for read serial and joystick
    pthread_t leggo;
    pthread_create(&leggo,NULL,&statusRoutine,&host);
    pthread_t thread_read_joystick;
    pthread_create(&thread_read_joystick,NULL,&readJoystickRoutine,&fd_joy);

    //Send packet at 100Hz
    while(1){
        if(flag_comm){
            BethHost_sendPacket(&host,&packet.h);
            flag_comm = 0;  
        }
    }
    return 0;
}
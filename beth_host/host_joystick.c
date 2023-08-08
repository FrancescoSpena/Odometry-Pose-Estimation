#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "beth_host.h"
#include "../src/common/print_packets.h"
#include "../src/common/packet_operations.h"
#include "../src/common/joystick.h"
#include <pthread.h>
#include <linux/joystick.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <ncurses.h>
#include <string.h>

#define U_SECOND 500000

int flag_read = 0;
BethHost host;
sem_t sem;

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

PacketHandler handler;

void* statusRoutine(void* host){
    char buf[256];
    BethHost* hos = (BethHost*)host;
    int fd = hos->serial_fd;
    uint8_t c;
    PacketStatus status = UnknownType;
    int n;
    while(1){
        while(status != ChecksumSuccess){
            n = read(fd,&c,1);
            if(n){
                status = PacketHandler_readByte(&handler,c);
            }
        }
        status=UnknownType;
        printf("Received:\n");
        PacketHeader* recv = handler.current_packet;
        printPacket(recv,buf);
        printf("%s\n",buf);
    }
}

void* echoRoutine(void* host){
    BethHost* hos = (BethHost*)host;
    int fd = hos->serial_fd;
    uint8_t c;
    while(1){
        int n = read(fd,&c,1);
        if(n){
            printf("ricevuto:");
            printf("%c",c);
            printf("\n");
        }
    }
}

void* readJoystickRoutine(void* _fd){
    int fd = *(int*)_fd;
    while(1){
        sem_wait(&sem);
        int left = readJoystick(fd,GYROSCOPE_AXYSY_LEFT);
        int right = readJoystick(fd,GYROSCOPE_AXYSY_RIGHT);
        if(left != -1){
            packet.translational_velocity=abs(left%255);
            flag_read = 1;
        }
        if(right != -1){
            packet.rotational_velocity=abs(right%255);
            flag_read = 1;
        }
        sem_post(&sem);
    }
}

void _mainRoutine(int fd_joy, char buf){
    int flag = 0;
    int x = -1;
    int circle = -1;
    printf("Select mode\n");
    printf("X for giroscope\n");
    printf("○ for constant\n");
    while(flag == 0){
        if(x == 0 || circle == 1){
            flag = 1;
        }
        x = readJoystick(fd_joy,BUTTON_X);
        circle = readJoystick(fd_joy,BUTTON_CIRCLE);
    }
    
    if(x == 0){
        printf("Move giroscope to command robot\n");
        while(1){
            BethHost_sendPacket(&host,&packet.h);
            printf("Tran Speed:%f\t Rotational speed:%f\n",packet.translational_velocity,packet.rotational_velocity);
            #ifdef DEBUG
            printf("Sent:\n");
            printPacket(&packet.h,buf);
            printf("%s\n",buf);
            #endif
            sleep(1);
        }
    }
    if(circle == 1){
        int speed = 0;
        printf("Speed:%d\n",speed);
        printf("Press Up/Down button to modify the speed, △ to select\n");
        int triangle = readJoystick(fd_joy,BUTTON_TRIANGLE);
        while(triangle == -1){
            if(readJoystick(fd_joy,BUTTON_UP) != -1){
                speed+=50;
                if(speed >= 255) speed = 255;
                printf("Speed:%d\n",speed);
            }
            if(readJoystick(fd_joy,BUTTON_DOWN) != -1){
                speed-=50;
                if(speed <= 0) speed = 0;
                printf("Speed:%d\n",speed);
            }
            triangle = readJoystick(fd_joy,BUTTON_TRIANGLE);
        }
        triangle = -1;
        printf("Now press Up/Down button to modify rotational speed, △ to select\n");
        packet.translational_velocity=speed;
        speed = 0;
        while(triangle == -1){
            if(readJoystick(fd_joy,BUTTON_UP) != -1){
                speed+=50;
                if(speed >= 255) speed = 255;
                printf("Speed:%d\n",speed);
            }
            if(readJoystick(fd_joy,BUTTON_DOWN) != -1){
                speed-=50;
                if(speed <= 0) speed = 0;
                printf("Speed:%d\n",speed);
            }
            triangle = readJoystick(fd_joy,BUTTON_TRIANGLE); 
        }
        packet.rotational_velocity=speed;
    }
}

void mainRoutineNcourses(int fd_joy){
    //ncurses start
    initscr();			
	noecho();
    cbreak();
    
    //code

    int height, width, start_y, start_x; 
    height = 20; 
    width = 80; 
    start_y = 0;
    start_x = 0;

    WINDOW* win = newwin(height,width,start_y,start_x);
    refresh();

    box(win,0,0);
    mvwprintw(win,8,27,"Welcome to beth firmware");
    mvwprintw(win,9,27,"Press any key to continue");
    mvwprintw(win,18,1,"Create by Francesco Spena");
    mvwprintw(win,18,69,"2023/2024");
    wrefresh(win);
    getch();
    wclear(win);
    
    mvwprintw(win,0,1,"Select a mode:");
    mvwprintw(win,1,1,"1. Gyroscope");
    mvwprintw(win,2,1,"2. Constant speed");
    mvwprintw(win,3,1,"3. One motor");
    mvwprintw(win,4,1,"Enter: ");
    wrefresh(win);
    char c = getch();
    wclear(win);
    switch(c){
        case '1':
            mvwprintw(win,0,32,"Gyroscope mode");
            mvwprintw(win,2,1,"Rotate left/right gyroscope for command robot, triangle to exit");
            wrefresh(win);
            while(1){
                int left = readJoystick(fd_joy,GYROSCOPE_AXYSY_LEFT);
                int right = readJoystick(fd_joy,GYROSCOPE_AXYSY_RIGHT);
                if(left != -1 || right != -1){
                    mvwprintw(win,3,1,"Speed left:%d, Speed right:%d",left,right);
                }
                if(readJoystick(fd_joy,BUTTON_TRIANGLE) != -1){
                    break;
                } 
            }
            break;
        case '2':
            int speed = 0;
            mvwprintw(win,0,32,"Constant speed mode");
            mvwprintw(win,2,1,"Select with Up/Down button the speed, triangle to select:");
            wrefresh(win);
            while(1){
                if(readJoystick(fd_joy,BUTTON_UP) != -1){
                    speed+=50;
                    if(speed >= 255) speed = 255;
                }
                if(readJoystick(fd_joy,BUTTON_DOWN) != -1){
                    speed-=50;
                    if(speed <= 0) speed = 0;
                }
                if(readJoystick(fd_joy,BUTTON_TRIANGLE) != -1){
                    packet.translational_velocity=speed;
                    break;
                }
                mvwprintw(win,3,1,"Speed: %d",speed);
                wrefresh(win);
            }
            //Fare velocità rotazionale
            mvwprintw(win,4,1,"You are selected speed:%d, rotational speed:%d",
                                (int)packet.translational_velocity,
                                (int)packet.rotational_velocity);
            wrefresh(win);
            break;
        case '3':
            mvwprintw(win,0,32,"One motor mode");
            wrefresh(win);
            break;
        default:
            break;
    }


    //code
    
    //ncurses end
    getch();
	endwin();
    return;
}

int main(void){
    /*sem_init(&sem,0,1);
    BethHost_init(&host,"/dev/ttyACM0",19200);
    int fd_joy = openJoystick("/dev/input/js1");
    PacketHandler_init(&handler);
    pthread_t thread_status_routine;
    pthread_create(&thread_status_routine,NULL,statusRoutine,&host);
    pthread_t thread_read_joystick;
    pthread_create(&thread_read_joystick,NULL,readJoystickRoutine,&fd_joy);

    char buf[256];
    while(1){
        if(flag_read == 1){
            sem_wait(&sem);
            BethHost_sendPacket(&host,&packet.h);
            printf("Sent:\n");
            printPacket(&packet.h,buf);
            printf("%s\n",buf);
            flag_read = 0;
            sem_post(&sem);
        }
        usleep(U_SECOND);
    }*/
    int fd_joy = openJoystick("/dev/input/js1");
    mainRoutineNcourses(fd_joy);

    return 0;
}
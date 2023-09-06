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
#include "packet_structure.h"
#include "beth_comm_host.h"
#define U_SECOND 500000

#define BAUND1 19200
#define BAUND2 115200
BethHost host;
sem_t sem;

volatile int flag_comm = 0;


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
        char buf[256];
        printPacket(handler.current_packet,buf);
        printf("Ricevuto:\n");
        printf("%s\n",buf);
    }
}

void* commFn(void* f){
    while(1){
        flag_comm = 1;
        usleep(10000);
    }
}

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
            packet.translational_velocity=v_x_norm*80; 
        }
        if(right == 0){
            v_y_norm = ((float)v_y) / 32767;
            packet.rotational_velocity=v_y_norm*80; 
        }
        if(center == 0){
            packet.translational_velocity = 0;
            packet.rotational_velocity = 0;
        }
        flag_comm = 1;
        usleep(10000);
    }
}

void write_str(WINDOW *win, int y, int x, char *str) {
    while (*str) {
        switch(*str) {
        case '\n':
            break;
        default:
            mvwaddch(win,y,x,*str); 
        }
        ++str; 
    }
}

void mainRoutineNcourses(int fd_joy){
    if(fd_joy < 0){
        printf("no joystick\n");
        return;
    }
    
    //ncurses start
    initscr();			
	noecho();
    cbreak();
    
    if(has_colors() == FALSE){
        printf("Your terminal does not support color\n");
    }

    //code

    start_color();

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
    
    mvwprintw(win,7,20,"Select a mode:");
    mvwprintw(win,8,20,"1. Gyroscope");
    mvwprintw(win,9,20,"2. Constant speed");
    mvwprintw(win,10,20,"3. One motor");
    mvwprintw(win,9,50,"Enter: ");
    wrefresh(win);
    char c = getch();
    wclear(win);
    switch(c){
        case '1':
            mvwprintw(win,0,32,"Gyroscope mode");
            mvwprintw(win,2,1,"Rotate left/right gyroscope for command robot, triangle to exit");
            wrefresh(win);
            //TODO: mettere il joystick, leggere i dati da li e creare un pacchetto di tipo diff drive control 
            //e mandare tutto ad arduino ogni volta, in questo caso la comm info dirà solo le velocità che si 
            //stanno impostando con il joystick + dati con odometria (dentro una box tutti e due)
            break;
        case '2':
            int speed = 0;
            mvwprintw(win,0,32,"Constant speed mode");
            mvwprintw(win,2,1,"Set translational speed:");
            mvwprintw(win,3,1,"Enter:");
            mvwscanw(win,3,10,"%d",&speed);
            mvwprintw(win,3,10,"%d",speed);
            drive_control.translational_velocity=speed;
            //rotational speed
            speed = 0;
            mvwprintw(win,4,1,"Set rotational speed:");
            mvwprintw(win,5,1,"Enter:");
            mvwscanw(win,5,10,"%d",&speed);
            mvwprintw(win,5,10,"%d",speed);
            drive_control.rotational_velocity=speed;
            wrefresh(win);
            //Send packet with information
            wclear(win);
            int i = 0;
            while(i != 8){
                BethHost_sendPacket(&host,&drive_control.h);
                i++;
                usleep(U_SECOND);
            }
            //Info 
            mvwprintw(win,0,32,"Common information");
            WINDOW* win_status;
            //status box
            int height_status,widht_status,start_y_status,start_x_status;
            height_status = 15;
            widht_status = 32;
            start_y_status = 2;
            start_x_status = 5;
            win_status = newwin(height_status,widht_status,start_y_status,start_x_status);
            box(win_status,0,0);
            wprintw(win_status,"Status Box");
            //info print packet
            mvwprintw(win_status,10,1,"Odometry x,y,theta:");
            mvwprintw(win_status,11,1,"%f",drive_status.odom_x);
            mvwprintw(win_status,12,1,"%f",drive_status.odom_y);
            mvwprintw(win_status,13,1,"%f",drive_status.odom_theta);

            
            //refresh
            wrefresh(win);
            wrefresh(win_status);
            //control box
            WINDOW* win_control;
            int height_control,widht_control,start_y_control,start_x_control;
            height_control = 8;
            widht_control = 30;
            start_y_control = 2;
            start_x_control = 42;
            win_control = newwin(height_control,widht_control,start_y_control,start_x_control);
            box(win_control,0,0);
            wprintw(win_control,"Control Box");
            mvwprintw(win_control,2,1,"Translational Speed:");
            mvwprintw(win_control,3,1,"%f",drive_control.translational_velocity);
            mvwprintw(win_control,4,1,"Rotational Speed:");
            mvwprintw(win_control,5,1,"%f",drive_control.rotational_velocity);
            //refresh
            wrefresh(win);
            wrefresh(win_control);

            //param box
            WINDOW* win_params;
            int height_params,widht_params,start_y_params,start_x_params;
            height_params = 8;
            widht_params = 30;
            start_y_params = 10;
            start_x_params = 42;
            win_params = newwin(height_params,widht_params,start_y_params,start_x_params);
            box(win_params,0,0);
            wprintw(win_params,"Params Box");
            mvwprintw(win_params,2,1,"Radius Wheel:");
            mvwprintw(win_params,3,1,"%f",drive_params.radius_wheel);
            mvwprintw(win_params,4,1,"Distance:");
            mvwprintw(win_params,5,1,"%f",drive_params.distance);

            //refresh
            wrefresh(win);
            wrefresh(win_params);
            
            break;
        case '3':
            mvwprintw(win,0,32,"One motor mode");
            wrefresh(win);

            
            //TODO: chiedere se motore 1 o motore 2 e poi attraverso quello si modifica il dest_addr
            //chiedere velocità desiderata per motore + parametri PID (occhio perchè il PID sta su un'altro pacchetto)
            //creare comm info anche per questo dove faccio vedere control,status e params del motore
            
            //motor
            mvwprintw(win,1,1,"Which motor do you want to drive?");
            mvwprintw(win,2,2,"- 0 for motor 1");
            mvwprintw(win,3,2,"- 1 for motor 2");
            mvwprintw(win,4,1,"Enter:");
            int num_motor = -1;
            speed = 0;
            mvwscanw(win,4,8,"%d",&num_motor);
            mvwprintw(win,4,1,"Enter:%d",num_motor);
            motor1_control.h.dest_addr=num_motor;
            //speed motor 
            mvwprintw(win,5,1,"Select a speed");
            mvwprintw(win,6,1,"Enter:");
            mvwscanw(win,6,8,"%d",&speed);
            mvwprintw(win,6,1,"Enter:%d",speed);
            motor1_control.speed=speed;
            int mode = Pid;
            //mode motor
            mvwprintw(win,7,1,"Select a mode [Pid = 1, Direct = 0]");
            mvwprintw(win,8,1,"Enter:");
            mvwscanw(win,8,8,"%d",&speed);
            mvwprintw(win,8,1,"Enter:%d",speed);
            motor1_control.mode=mode;
            
            wrefresh(win);

            //comm info 
            
            
            break;
        default:
            break;
    }


    //code
    
    //ncurses end
    getch();
    wclear(win);
	endwin();
    return;
}

int main(void){
    /*BethHost_init(&host,"/dev/ttyACM0",BAUND1);
    int fd_joy = openJoystick("/dev/input/js1");

    if(fd_joy < 0){
        printf("joystick non disponibile, collegare un dispositivo funzionante\n");
        return 0;
    }
    PacketHandler_init(&handler);

    pthread_t leggo;
    pthread_create(&leggo,NULL,&statusRoutine,&host);
    pthread_t thread_read_joystick;
    pthread_create(&thread_read_joystick,NULL,&readJoystickRoutine,&fd_joy);

    //char buf[256];
    while(1){
        if(flag_comm){
            BethHost_sendPacket(&host,&packet.h);
            printf("Sent:\n");
            printPacket(&packet.h,buf);
            printf("%s\n",buf);
            flag_comm = 0;   
        }
    }*/


    //GUI

    BethHost_init(&host,"/dev/ttyACM0",19200);
    int fd_joy = openJoystick("/dev/input/js1");
    if(fd_joy < 0){
        printf("joystick non disponibile, collegare un dispositivo funzionante\n");
        return 0;
    }
    PacketHandler_init(&handler);

    pthread_t leggo;
    pthread_create(&leggo,NULL,&statusRoutine,&host);
    pthread_t thread_read_joystick;
    pthread_create(&thread_read_joystick,NULL,&readJoystickRoutine,&fd_joy);
    
    mainRoutineNcourses(fd_joy);


    return 0;
}
#pragma once
#include <linux/joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Name button
#define BUTTON_X 0x00
#define BUTTON_CIRCLE 0x01
#define BUTTON_TRIANGLE 0x02
#define BUTTON_SQUARE 0x03
#define BUTTON_UP 0x04
#define BUTTON_DOWN 0x05
#define BUTTON_LEFT 0x06
#define BUTTON_RIGHT 0x07
#define BUTTON_PLAYSTATION 0x08
#define BUTTON_START 0x09
#define BUTTON_SELECT 0x10
#define BUTTON_L1 0x15
#define BUTTON_L2 0x16
#define BUTTON_R1 0x17
#define BUTTON_R2 0x18
//Name gyroscope joystick
#define GYROSCOPE_AXYSX_LEFT 0x11
#define GYROSCOPE_AXYSY_LEFT 0x12
#define GYROSCOPE_AXYSX_RIGHT 0x13
#define GYROSCOPE_AXYSY_RIGHT 0x14


//Open device in read only and return fd
int openJoystick(const char* _device);
//Return button value if success, else -1 
int readJoystick(int _fd, int _name_button, int*);
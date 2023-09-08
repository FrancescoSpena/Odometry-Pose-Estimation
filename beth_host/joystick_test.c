#include "../src/common/joystick.h"


int main(void){
    int value = 0;
    int fd = openJoystick("/dev/input/js1");
    
    
    while(1){
        int left = readJoystick(fd,GYROSCOPE_AXYSX_LEFT,&value);
        printf("value:%d\n", value);
    }
    return 0;
}
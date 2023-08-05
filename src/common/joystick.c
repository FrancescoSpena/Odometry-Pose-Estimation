#include "joystick.h"

//Define axys joystick
#define AXYS0 0  
#define AXYS1 1
#define AXYS2 2
#define AXYS3 3
#define AXYS4 4
#define AXYS5 5
#define AXYS6 6
#define AXYS7 7
#define AXYS8 8
#define AXYS9 9
#define AXYS10 10
#define AXYS13 13
#define AXYS14 14
#define AXYS15 15
#define AXYS16 16
//Define type axys
#define TYPE1 1
#define TYPE2 2 

//internal function
void makeNameButton(int _name_button);
int _readJoystick(int _fd, int _axys, int _type);

struct js_event e;

typedef struct CoordinateButton{
    int _axys;
    int _type;
}CoordinateButton;

CoordinateButton cb;


//Open device in read only and return fd
int openJoystick(const char* _device){
    return open(_device,O_RDONLY);
}

//Return 0 if success, else -1 
int readJoystick(int _fd, int _name_button){
    makeNameButton(_name_button);
    return _readJoystick(_fd,cb._axys,cb._type);
}

//Return value if success, else -1 
int _readJoystick(int _fd, int _axys, int _type){
    int fd = _fd;
    while(read(fd,&e,sizeof(e)) > 0){
        if(e.number == _axys && e.type == _type){
            return e.value;
        }
        return -1;
    }
    return -1;
}

void makeNameButton(int _name_button){
    switch(_name_button){
        case BUTTON_X:
            cb._axys=AXYS0;
            cb._type=TYPE1;
            break;
        case BUTTON_CIRCLE:
            cb._axys=AXYS1;
            cb._type=TYPE1;
            break;
        case BUTTON_TRIANGLE:
            cb._axys=AXYS2;
            cb._type=TYPE1;
            break;
        case BUTTON_SQUARE: 
            cb._axys=AXYS3;
            cb._type=TYPE1;
            break;
        case BUTTON_UP: 
            cb._axys=AXYS13;
            cb._type=TYPE1;
            break;
        case BUTTON_DOWN: 
            cb._axys=AXYS14;
            cb._type=TYPE1;
            break;
        case BUTTON_LEFT: 
            cb._axys=AXYS15;
            cb._type=TYPE1;
            break;
        case BUTTON_RIGHT: 
            cb._axys=AXYS16;
            cb._type=TYPE1;
            break;
        case BUTTON_PLAYSTATION:
            cb._axys=AXYS10;
            cb._type=TYPE1;
            break;
        case BUTTON_START:
            cb._axys=AXYS9;
            cb._type=TYPE1;
            break;
        case BUTTON_SELECT:
            cb._axys=AXYS8;
            cb._type=TYPE1;
            break;
        case GYROSCOPE_AXYSX_LEFT:
            cb._axys=AXYS1;
            cb._type=TYPE2;
            break;
        case GYROSCOPE_AXYSY_LEFT:
            cb._axys=AXYS0;
            cb._type=TYPE2;
            break;
        case GYROSCOPE_AXYSX_RIGHT:
            cb._axys=AXYS4;
            cb._type=TYPE2;
            break;
        case GYROSCOPE_AXYSY_RIGHT:
            cb._axys=AXYS3;
            cb._type=TYPE2;
            break;
        case BUTTON_L1:
            cb._axys=AXYS4;
            cb._type=TYPE1;
            break;
        case BUTTON_L2:
            cb._axys=AXYS2;
            cb._type=TYPE2;
            break;
        case BUTTON_R1:
            cb._axys=AXYS5;
            cb._type=TYPE1;
            break;
        case BUTTON_R2:
            cb._axys=AXYS5;
            cb._type=TYPE2;
            break;
    }
}
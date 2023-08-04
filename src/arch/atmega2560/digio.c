/**
 * digio.c
 * @version 2023
*/

#include "../include/digio.h"
#include "../include/pins.h"
#include <avr/io.h>

#define PIN_NUMBER 14 //0 --> 13 digital pin

/**
 * Configure p as Input or Output
*/
void digio_configurePin(uint8_t pin, PinMode mode){
    if(pin > PIN_NUMBER)
        return;
    if(mode == Output){
        *(_pins[pin].r_ddr)|=(1 << _pins[pin].pin);  
    }else{
        *(_pins[pin].r_ddr)&=~(1 << _pins[pin].pin);
    }
}

/**
 * Set the PORT register of the pin: 
 * is pin an OUTPUT 
 *      then modify the output of the pin
 * is pin an INPUT 
 *      then modify the pullup status of the pin 
*/
void digio_setPin(uint8_t pin, PinStatus status){
    if(pin > PIN_NUMBER)
        return;
    if(status == High)
        *(_pins[pin].r_port)|=(1 << _pins[pin].pin);
    else
        *(_pins[pin].r_port)&=~(1 << _pins[pin].pin);
}

/**
 * Reads the PIN register of the pin
*/
uint8_t digio_readPin(uint8_t pin){
    if(pin > PIN_NUMBER)
        return 0;
    return *(_pins[pin].r_pin)&(1 << _pins[pin].pin);
}
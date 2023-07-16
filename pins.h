/**
 * pins.h
 * @version 2023
**/

#include <stdint.h>

typedef struct{
    //digio
    volatile uint8_t* r_ddr; //register address
    volatile uint8_t* r_port; //register port 
    volatile uint8_t* r_pin;  //register pin
    uint8_t pin;
    //pwm
    volatile uint8_t* tccr; //TCC register 
    volatile uint8_t* ocr;  //OC  register
    uint8_t mask;           //mask 
}Pin;

extern Pin _pins[];
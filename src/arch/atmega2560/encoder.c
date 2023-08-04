/**
 * encoder.c 
*/

#include "../include/encoder.h"
#include <avr/io.h> 
#include <avr/interrupt.h> 

#define NUM_ENCODERS 4 //num pin

//all port B is used to handle 4 encoders --> pin 13,12,11,10  
#define ENCODER_MASK 0xFF


typedef uint16_t EncoderValueType;

uint8_t _encoder_status; 
EncoderValueType _encoder_current_value[NUM_ENCODERS];
EncoderValueType _encoder_sampled_value[NUM_ENCODERS];

/**
 * Initializes the encoders by activating PCI (Pin Change Interrupt) on encoder
 * pins.
 **/
void Encoder_init(void){
    cli();
    for(int i=0; i < NUM_ENCODERS; i++){
        _encoder_current_value[i] = 0;
        _encoder_sampled_value[i] = 0;
    }

    _encoder_status = 0; 
    //configure port and interrupts 
    DDRB &=~ENCODER_MASK;           //set encoder pins as input
    PORTB |=ENCODER_MASK;           //enable pull up resistors
    PCICR |= (1 << PCIE0);          //set interrupt on change, looking up PCMSK0
    PCMSK0 |= ENCODER_MASK;         //set PCINT0 to trigger an interrupt on state change
    sei();
}

/**
 * sample the encoders (saving the respective values in a temporary storage)
 **/
void Encoder_sample(void){
    cli();
    //put value in a register for fast access
    register EncoderValueType* s=_encoder_sampled_value;
    register EncoderValueType* c=_encoder_current_value;
    for(uint8_t i=0; i < NUM_ENCODERS; ++i, ++s, ++c)
        *s=*c;
    sei();
}

/**
 * Get sampled values from encoders
 **/
uint16_t Encoder_getValue(uint8_t idx){
    return _encoder_sampled_value[idx];
}

/**
 * Get the number of encoders attached to the MCU
 **/
uint8_t Encoder_numEncoders(void){
    return NUM_ENCODERS;
}


static const int8_t _transition_table[] = {
    0,  //0000
    1,  //0001
    -1, //0010
    0,  //0011
    -1, //0100
    0,  //0101
    0,  //0110
    1,  //0111
    1,  //1000
    0,  //1001
    0,  //1010
    -1, //1011
    0,  //1100
    -1, //1101
    1,  //1110
    0   //1111
};

ISR(PCINT0_vect){
    cli();
    uint8_t port_value=PINB&ENCODER_MASK;
    register uint8_t c_value=port_value;                    //valore che stai leggendo
    register uint8_t prev_value=_encoder_status;            //valore precedente
    register const int8_t* ttable =_transition_table;
    register uint16_t* curr_enc=_encoder_current_value;     //valore print encoder
    for(uint8_t i=0; i < NUM_ENCODERS; ++i){
        //build the following data:
        // first 2 bits of prev_value | first 2 bits of c_value 
        uint8_t table_idx =(c_value&0x03)|(prev_value&0x03)<<2;
        *curr_enc+=ttable[table_idx];
        ++curr_enc;
        c_value>>=2;
        prev_value>>=2;
    }
    _encoder_status=port_value;
    sei();
}

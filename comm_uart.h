#pragma once 
#include <stdint.h>

#define BUF_SIZE 256 

typedef struct Uart{
    uint8_t rx_buffer[BUF_SIZE];
    uint8_t rx_start;
    uint8_t rx_end; 
    uint8_t rx_size;

    uint8_t tx_buffer[BUF_SIZE];
    uint8_t tx_start;
    uint8_t tx_end;
    uint8_t tx_size;
}Uart;

Uart uart;

/**
 * Initialize the Uart module
 **/
void Uart_init(Uart uart);

/**
 * Insert c in the uart buffer, ready to be sent
 **/
void Uart_write(struct Uart*, uint8_t);

/**
 * Return the number of available bytes to read from the uart
 **/
uint8_t Uart_available(struct Uart*);

/**
 * reads the first character from the uart buffer
 **/
uint8_t Uart_read(struct Uart*);

/**
 * Returns the number of free cells inside the tx buffer
 **/
uint16_t Uart_txFree(struct Uart*);
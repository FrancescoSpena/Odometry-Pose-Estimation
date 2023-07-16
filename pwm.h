/**
 * pwm.h
 * @version 2023
*/

#pragma once 
#include <stdint.h>

void PWM_init(void);
void PWM_enablePin(uint8_t pin);
void PWM_setOutput(uint8_t pin, uint8_t pwm);

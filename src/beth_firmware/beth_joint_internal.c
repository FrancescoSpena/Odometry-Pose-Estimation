/**
 * beth_joint_internal.c
*/

#include "beth_joint_internal.h"
#include "../arch/include/digio.h"
#include "../arch/include/pwm.h"
#include "../arch/include/encoder.h"
#include "../arch/include/uart.h"
#include <stdio.h>


void BethJoint_init(BethJoint* j, MotorControlPacket* _control,
                    MotorStatusPacket* _status,
                    MotorParamsPacket* _params,
                    uint8_t _eidx){

    j->control=_control;
    j->status=_status;
    j->params=_params;
    j->enc_idx=_eidx;


    digio_configurePin(j->params->pwm_pin, Output);
    digio_setPin(j->params->pwm_pin,0);
    PWM_enablePin(j->params->pwm_pin);
    PWM_setOutput(j->params->pwm_pin,0);

    digio_configurePin(j->params->dir_a_pin,Output);
    digio_setPin(j->params->dir_a_pin,0);
    digio_configurePin(j->params->dir_b_pin,Output);
    digio_setPin(j->params->dir_b_pin,0);

    return;
}

int16_t clamp(int16_t v, int16_t max){
    if(v > max)
        return max;
    if(v < -max)
        return -max;
    return v;
}

void BethJoint_handle(BethJoint* j){
    //Encoder section
    uint8_t enc_idx=j->enc_idx;
    uint16_t prev_ticks=j->status->encoder_ticks;
    j->status->encoder_ticks=Encoder_getValue(enc_idx);
    j->status->measured_speed=j->status->encoder_ticks-prev_ticks;

    if(j->control->mode == Disable)
        return;
    if(j->control->mode == Pid){
        static int16_t perror;
        j->status->desired_speed=j->control->speed;
        //printf("Desidered speed = %d\n", j->status->desired_speed);
        int16_t error = j->status->desired_speed - j->status->measured_speed;
        int16_t output = 0;

        printf("Speed to enc[%d]= %d\n", j->enc_idx,j->status->measured_speed);

        j->params->sum_i += j->params->ki * error * j->params->dt;
        j->params->sum_i = clamp(j->params->sum_i,j->params->max_i);

        double derror = (error - perror)*j->params->idt;
        output = j->params->kp*error + j->params->sum_i + derror*j->params->kd;
        output = clamp(output,j->params->max_out);
        int16_t speed = output;
        uint8_t dir = 0;
        if(speed < 0){
            speed = -speed;
            dir = 1;
        }
        j->dir = dir;
        j->output = speed;

        //printf("Dir = %d\n", j->dir);
        //printf("Output = %d\n", j->output);

        //printf("Pin A = %d\t Pin B = %d\t Pin PWM = %d\n", j->params->dir_a_pin, j->params->dir_b_pin, j->params->pwm_pin);


        digio_setPin(j->params->dir_a_pin, j->dir);
        digio_setPin(j->params->dir_b_pin,!j->dir);
        PWM_setOutput(j->params->pwm_pin,j->output);

        perror=error;
        return;
    }
    if(j->control->mode == Direct){
        //Direct Mode
        int16_t speed=j->control->speed;
        uint8_t dir=0;
        speed = clamp(speed,255);
        j->status->desired_speed=speed;
        if(speed < 0){
            dir = 1;
            speed = -speed;
        }
        j->dir=dir;
        j->output=speed;
        digio_setPin(j->params->dir_a_pin,j->dir);
        digio_setPin(j->params->dir_b_pin,!j->dir);
        PWM_setOutput(j->params->pwm_pin,j->output);
        return;
    }
    return;
}
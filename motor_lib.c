#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <cp0defs.h>
#include <sys/attribs.h>
#include "tim_lib.h"
#include "motor_lib.h"

// sets all the PWM and IN pins to the driver IC as outputs
// and initialize pins to output 0
void motor_init(void)
{
    // motor 1
    TRISBbits.TRISB15;      // set RB15 to output mode
    M1PWMA = 0;             // initialize RB15 to output 0

    TRISBbits.TRISB0 = 0;
    M1PWMB = 0;

    TRISGbits.TRISG8 = 0;
    M1AIN1 = 0;

    TRISGbits.TRISG6 = 0;
    M1AIN2 = 0;

    TRISAbits.TRISA12 = 0;
    M1BIN1 = 0;

    TRISAbits.TRISA11 = 0;
    M1BIN2 = 0;
    
    // motor 2
    TRISBbits.TRISB4 = 0;
    M2PWMA = 0;

    TRISAbits.TRISA8 = 0;
    M2PWMB = 0;

    TRISEbits.TRISE12 = 0;
    M2AIN1 = 0;

    TRISEbits.TRISE13 = 0;
    M2AIN2 = 0;

    TRISCbits.TRISC2 = 0;
    M2BIN1 = 0;

    TRISCbits.TRISC1 = 0;
    M2BIN2 = 0;
    
    // motor 3
    TRISBbits.TRISB7 = 0;
    M3PWMA = 0;

    TRISCbits.TRISC15 = 0;
    M3PWMB = 0;

    TRISCbits.TRISC7 = 0;
    M3AIN1 = 0;

    TRISCbits.TRISC8 = 0;
    M3AIN2 = 0;

    TRISCbits.TRISC10 = 0;
    M3BIN1 = 0;
    
    TRISDbits.TRISD8 = 0;
    M3BIN2 = 0;
    
    // motor 4
    TRISBbits.TRISB13 = 0;
    M4PWMA = 0;
    
    TRISBbits.TRISB9 = 0;
    M4PWMB = 0;
    
    TRISFbits.TRISF1 = 0;
    M4AIN1 = 0;
    
    TRISBbits.TRISB12 = 0;
    M4AIN2 = 0;
    
    TRISCbits.TRISC9 = 0;
    M4BIN1 = 0;
    
    TRISDbits.TRISD6 = 0;
    M4BIN2 = 0;
}


// Function will step the motor 1/50 its full revolution
// Inputs: motor number, direction (CW or CCW), and delay
// NOTE - actual delay of the function is 4 times your provided delay
void motor_step50(int motor_num, int dir, int delay)
{
    switch(dir)
    {
        case CW:
            motor_set_phase(motor_num, M_PHASE1);
            tim_delay_ms(delay);
            motor_set_phase(motor_num, M_PHASE2);
            tim_delay_ms(delay);
            motor_set_phase(motor_num, M_PHASE3);
            tim_delay_ms(delay);
            motor_set_phase(motor_num, M_PHASE4);
            tim_delay_ms(delay);
            break;
        case CCW:
            motor_set_phase(motor_num, M_PHASE3);
            tim_delay_ms(delay);
            motor_set_phase(motor_num, M_PHASE2);
            tim_delay_ms(delay);
            motor_set_phase(motor_num, M_PHASE1);
            tim_delay_ms(delay);
            motor_set_phase(motor_num, M_PHASE4);
            tim_delay_ms(delay);
            break;
        default:
            break;
    }
}


// set specified stepper motor to a certain phase
void motor_set_phase(int motor_num, int phase)
{
    switch(motor_num)
    {
        case MOTOR1:
            switch(phase)
            {
                case M_PHASE1:
                    M1PWMA = 1;
                    M1PWMB = 0;
                    M1AIN1 = 1;
                    M1AIN2 = 0;
                    break;
                case M_PHASE2:
                    M1PWMA = 0;
                    M1PWMB = 1;
                    M1BIN1 = 1;
                    M1BIN2 = 0;
                    break;
                case M_PHASE3:
                    M1PWMA = 1;
                    M1PWMB = 0;
                    M1AIN1 = 0;
                    M1AIN2 = 1;
                    break;
                case M_PHASE4:
                    M1PWMA = 0;
                    M1PWMB = 1;
                    M1BIN1 = 0;
                    M1BIN2 = 1;
                    break;
                default:
                    break;
            }
            break;
        case MOTOR2:
            switch(phase)
            {
                case M_PHASE1:
                    M2PWMA = 1;
                    M2PWMB = 0;
                    M2AIN1 = 1;
                    M2AIN2 = 0;
                    break;
                case M_PHASE2:
                    M2PWMA = 0;
                    M2PWMB = 1;
                    M2BIN1 = 1;
                    M2BIN2 = 0;
                    break;
                case M_PHASE3:
                    M2PWMA = 1;
                    M2PWMB = 0;
                    M2AIN1 = 0;
                    M2AIN2 = 1;
                    break;
                case M_PHASE4:
                    M2PWMA = 0;
                    M2PWMB = 1;
                    M2BIN1 = 0;
                    M2BIN2 = 1;
                    break;
                default:
                    break;
            }
            break;
        case MOTOR3:
            switch(phase)
            {
                case M_PHASE1:
                    M3PWMA = 1;
                    M3PWMB = 0;
                    M3AIN1 = 1;
                    M3AIN2 = 0;
                    break;
                case M_PHASE2:
                    M3PWMA = 0;
                    M3PWMB = 1;
                    M3BIN1 = 1;
                    M3BIN2 = 0;
                    break;
                case M_PHASE3:
                    M3PWMA = 1;
                    M3PWMB = 0;
                    M3AIN1 = 0;
                    M3AIN2 = 1;
                    break;
                case M_PHASE4:
                    M3PWMA = 0;
                    M3PWMB = 1;
                    M3BIN1 = 0;
                    M3BIN2 = 1;
                    break;
                default:
                    break;
            }
            break;
        case MOTOR4:
            switch(phase)
            {
                case M_PHASE1:
                    M4PWMA = 1;
                    M4PWMB = 0;
                    M4AIN1 = 1;
                    M4AIN2 = 0;
                    break;
                case M_PHASE2:
                    M4PWMA = 0;
                    M4PWMB = 1;
                    M4BIN1 = 1;
                    M4BIN2 = 0;
                    break;
                case M_PHASE3:
                    M4PWMA = 1;
                    M4PWMB = 0;
                    M4AIN1 = 0;
                    M4AIN2 = 1;
                    break;
                case M_PHASE4:
                    M4PWMA = 0;
                    M4PWMB = 1;
                    M4BIN1 = 0;
                    M4BIN2 = 1;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

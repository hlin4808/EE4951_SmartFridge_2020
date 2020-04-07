#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <cp0defs.h>
#include <sys/attribs.h>
#include "tim_lib.h"
#include "motor_lib.h"
#include "uart_lib.h"

/*** DEVCFG0 ***/

#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1            // use PGEC1 and PGED1
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_2X
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_LEVEL_3
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      SPLL            // choose PLL for SYSCLK
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    ON
#pragma config IESO =       ON
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF                 // watchdog off
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF
/*** DEVCFG2 ***/

#pragma config FPLLRNG =    RANGE_8_16_MHZ
#pragma config FPLLICLK =   PLL_FRC         // PLL source is 8MHz FRC
#pragma config FPLLIDIV =   DIV_1           // PLL divide by 1
#pragma config FPLLMULT =   MUL_16          // PLL multiply by 16
#pragma config FPLLODIV =   DIV_2           // PLL divide by 2
                                            // PLL - SYSCLK is 64MHz
#pragma config VBATBOREN =  ON
#pragma config DSBOREN =    ON
#pragma config DSWDTPS =    DSPS32
#pragma config DSWDTOSC =   LPRC
#pragma config DSWDTEN =    OFF
#pragma config FDSEN =      ON
#pragma config BORSEL =     HIGH
#pragma config UPLLEN =     OFF
/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config FUSBIDIO2 =   ON
#pragma config FVBUSIO2 =  ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    OFF
#pragma config FUSBIDIO1 =   ON
#pragma config FVBUSIO1 =  ON
#pragma config PWMLOCK =  OFF

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff

void setup();
void test_motor(void);
void test_position(void);


int main(void) {
    setup();
    tx_data("\rhello world!\r");
    tx_data("abcdefghijklmnopqrstuvwxyz1234567890");
    //tim_delay_ms(2000);
    while(1)
    {
        app_command();          // this function needs to be called periodically
                                // to check if there is a new command from the app
                                // and if there is this function will do something
        
//        test_motor();
        test_position();
    }
    
    
    while(1);   // never reach here, but have forever loop just in case
    return 0;
}

void setup()
{
    INTCONbits.MVEC = 1;
    __builtin_mtc0(12,0,(__builtin_mfc0(12,0) | 0x0001)); // Global Interrupt Enable
    
    // by default analog capable pins are set to analog
    // all the below ANSELx registers are set to 0 to set all pins to digital mode
    // ANSELD and ANSELF are only available on 100 pin devices so they are not needed
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELE = 0;
    ANSELG = 0;
    
    tim_init();
    motor_init();
    uart_init();
}

void test_motor(void)
{
    // NOTE - only MOTOR2 is working on the hardware side
    // the below will spin motor 2 clockwise one revolution (in 200ms)
    // then wait 500ms
    // spin counterclockwise one full revolution (in 200ms)
    // then wait 500ms
    int i = 0;
    for(i=0; i<50; i++)                 // step 50 times clockwise (one full revolution)
    {
        motor_step50(MOTOR2, CW, 1);    // motor 2 take one step CW (1/50 of full revolution), takes 1*4 ms (change 1 to x to slow down to x*4 ms)
    }
    tim_delay_ms(500);                  // wait 500ms
    
    for(i=0; i<50; i++)                 // step 50 times counter clockwise (one full revolution)
    {
        motor_step50(MOTOR2, CCW, 1);   // motor 2 take one step CCW (1/50 of full revolution), takes 1*4 ms (change 1 to x to slow down to x*4 ms)
    }
    tim_delay_ms(500);                  // wait 500ms
}

void test_position(void)
{
    // NOTE - only MOTOR2 is working on the hardware side
    
//    //LEAD SCREW - Z
//    motor_position(93,'Z');   //move 93 mm away from previous initial (CW)
//    tim_delay_ms(500);
//    motor_position(31,'Z');   //move 93-31 mm away from previous position (CCW))
//    tim_delay_ms(500);
//    motor_position(1,'Z');    //return to power on initial position
//    tim_delay_ms(500);
    
    //PULLEY - X
    motor_position(114,'X');
    tim_delay_ms(5000);
    motor_position(0,'X');
    tim_delay_ms(5000);
    
}

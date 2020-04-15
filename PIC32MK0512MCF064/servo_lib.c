#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <cp0defs.h>
#include <sys/attribs.h>
#include <proc/PIC32MK-MC/p32mk0512mcf064.h>
#include "tim_lib.h"
#include "uart_lib.h"   // contains the unlock and lock macros

void servo_init(void)
{
    TRISAbits.TRISA1 = 0;   // RA1 output
    
    //Peripheral Pin Select (PPS) Settings for Output Compare 2 (OC2))
    PPSUnLock();
    RPA1R = 0b00101;        // OC2 to RPA1
    PPSLock();
    
    OC2CON = 0;             // deinit everything for OC2
    
    OC2CONbits.SIDL = 0;    // continue operation in idle mode
    
    OC2CONbits.OC32 = 0;    // 16 bit timer used for comparisons with OC2R and OC2RS
    OC2CONbits.OCTSEL = 1;  // timer y is the clock source
    CFGCONbits.OCACLK = 0;  // this in combination with the OC32 and OCTSEL settings
                            // results in choosing 16 bit Timer 3 as timer source
                            // (refer to Table 19-1 in datasheet)
    
    OC2CONbits.OCM = 0b110; // PWM mode, fault pin disabled
    OC2R = 2000;            // start servo all the way to the left (1ms)
    OC2RS = 2000;           // being over cautious here...
    OC2CONbits.ON = 1;      // OC2 enabled
}

void servo_setCompare(int val)
{
    // How the servo works is the position is based on the pwm signal
    // Timer 3 is used to create a 20ms period
    // This function is used to set the PWM high signal time by changing val
    // TMR3 counts up to 40000 for 20ms
    // so val equal to 1 is 500ns, val equal to 2 is 1us, etc...
    
    // NOTE - having trouble finding specs on datasheet of our servo
    // so we will need to test different val to see how our servo moves
    // but we have a guess based on a different servo's operation
    // 2000 (1ms) is all the way to the left?
    // 4000 (2ms) is all the way to the right?
    
    // TENTATIVE range of servo is 2000 < val < 4000
    
    OC2RS = val;
}

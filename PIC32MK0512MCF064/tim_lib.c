#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <cp0defs.h>
#include <sys/attribs.h>
#include "tim_lib.h"


void tim_init(void)
{
    /********** timer 2 for 1us delays **********/
    T2CON = 0;              // deinit everything to defaults
    
    T2CONbits.TCS = 0;      // internal pheripheral clock is source (64MHz)
    T2CONbits.T32 = 0;      // 16 bit timer
    T2CONbits.TCKPS = 0;    // prescaler of 1
    TMR2 = 0;               // reset timer count to 0
    PR2 = 64 - 1;           // timer counts to 1us
    IFS0bits.T2IF = 0;      // reset timer2 interrupt flag
    
    T2CONbits.ON = 1;       // timer2 on
    
    
    /********** timer 3 for OC2 **********/
    T3CON = 0;                  // deinit everything to defaults
    
    T3CONbits.TCS = 0;          // internal pheripheral clock is source (64MHz)
    T3CONbits.T32 = 0;          // 16 bit timer
    T3CONbits.TCKPS = 0b101;    // prescaler of 32
    TMR3 = 0;                   // reset timer count to 0
    PR3 = 40000 - 1;            // timer counts to 20ms
    
    T3CONbits.ON = 1;           // timer3 on
}


void tim_delay_ms(int t)
{
    int i;
    for(i=0; i<t; i++)
    {
        tim_delay_us(1000);     // wait 1000us (which is 1ms)
    }
}


void tim_delay_us(int t)
{
    int i;
    TMR2 = 0;                       // reset timer2 count to 0
    IFS0bits.T2IF = 0;              // reset flag
    for(i=0; i<t; i++)
    {
        while(IFS0bits.T2IF == 0);  // wait for 1us to occur
        IFS0bits.T2IF = 0;          // reset flag
    }
}

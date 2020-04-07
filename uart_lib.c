#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <cp0defs.h>
#include <sys/attribs.h>
#include "uart_lib.h"

// initialize UART1 and UART2
// UART2 is a backup that is turned off right now
// to turn on UART2 set ...IE, ...EN, and ON
void uart_init()
{
    TRISAbits.TRISA7 = 1;   // RA7 (U1RX) input
    TRISAbits.TRISA0 = 0;   // RA0 (U1TX) output
    TRISGbits.TRISG7 = 1;   // RG7 (U2RX) input
    TRISBbits.TRISB14 = 0;  // RB14 (U2TX) output
    
    // iolock (cfgcon<13>) do we need this?
    
    U1RXR = 0b0111;         // U1RX to RPA7
    RPA0R = 0b00001;        // U1TX to RPA0
    U2RXR = 0b1010;         // U2RX to RPG7
    RPB14R = 0b00010;       // U2TX to RPB14
    
    // UART off
    // BRG clock is PBCLK2 (64MHz)
    // 8 bit data, no parity, 1 stop bit
    U1MODE = 0;
    U2MODE = 0;
    
    // UxBRG = (CLKSEL freq / (16 * desired baud rate)) - 1
    // for baud rate = 115200 -> UxBRG = 33.72
    U1BRG = 34;
    U2BRG = 34;
    
    // reset interrupt flags
    IFS1bits.U1RXIF = 0;
    IFS1bits.U1RXIF = 0;
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2RXIF = 0;
    
    // enable interrupts
    IEC1bits.U1RXIE = 1;    // U1RX interrupts enabled
    IEC1bits.U1TXIE = 1;    // U1TX interrupts enabled
    IEC1bits.U2RXIE = 0;    // U2RX interrupts disabled
    IEC1bits.U2TXIE = 0;    // U2TX interrupts disabled
    
    // tx interrupt when transmit buffer has at least one empty space
    // rx interrupt when receive buffer is not empty
    U1STA = 0;
    U1STAbits.URXEN = 1;    // U1RX enabled
    U1STAbits.UTXEN = 1;    // U1TX enabled
    U2STA = 0;
    U2STAbits.URXEN = 0;    // U2RX disabled
    U2STAbits.UTXEN = 0;    // U2TX disabled
    
    // turn on UART
    U1MODEbits.ON = 1;      // UART1 on
    U2MODEbits.ON = 0;      // UART2 off
}

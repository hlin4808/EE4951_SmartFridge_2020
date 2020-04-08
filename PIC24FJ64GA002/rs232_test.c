#include "xc.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#define RX_SIZE 50
#define TX_SIZE 50

volatile int tx_tail = 0;
volatile int tx_head = 0;
volatile int rx_i = 0;
volatile char rx_buff[RX_SIZE];
volatile char tx_buff[TX_SIZE];
int tx_flag = 0;

void __attribute__((interrupt, auto_psv)) _U1TXInterrupt()
{
    _U1TXIF = 0;

    if(++tx_tail < tx_head)
    {
        U1TXREG = tx_buff[tx_tail];
    }
    else
    {
        tx_flag = 0;
    }
}

void __attribute__((interrupt, auto_psv)) _U1RXInterrupt()
{
    _U1RXIF = 0;
    
    rx_buff[rx_i] = U1RXREG;    // read UxRXREG
    
    if(rx_buff[rx_i] == '@')    // check for end character from app
        rx_i = 0;
    else if(++rx_i >= RX_SIZE)  // stay at last index if exceeding buffer size
        rx_i = RX_SIZE - 1;
}

void tx_data(const char data[])
{
    int i = 0;
    
    while(tx_flag);              // wait for previous transmit to complete
    
    tx_head = 0;
    while(data[i] != 0)         // copy data until end of string (NULL char)
    {
        if(i >= TX_SIZE)
            break;
        
        tx_buff[i] = data[i];
        
        tx_head++;
        i++;
    }
    
    tx_flag = 1;                // transmitting
    tx_tail = 0;
    U1TXREG = tx_buff[tx_tail];
}

void setup(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M

    AD1PCFG = 0x9fff;            //sets all pins to digital I/O
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR18bits.U1RXR = 6; // RP6 - U1RX (bot right) (orange)
    RPOR3bits.RP7R = 3; // RP7 - U1TX (2nd bot right) (yellow)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    U1MODE = 0; // reset to defaults
    U1STA = 0;
    U1MODEbits.PDSEL = 0;   // 8 bit data, no parity
    U1MODEbits.STSEL = 0;   // one stop bit
    U1BRG = 8;  // baud rate = 111,111 (desired baud rate = 115,200 -> 3.5% error)
    U1STAbits.UTXISEL1 = 0; // interrupt when a character is shifted to the transmit shift register
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.URXISEL = 0;  // interrupt is set when any char is received
                            // transferred from the RSR to the receive buffer
                            // receive buffer has one or more characters
    
    // reset interrupt flags
    _U1TXIF = 0;
    _U1RXIF = 0;
    
    // enable interrupts
    _U1TXIE = 1;
    _U1RXIE = 1;
    
    U1MODEbits.UARTEN = 1;  // enable UART1
    U1STAbits.UTXEN = 1;    // transmit enabled
}

int main(void)
{
    setup();
    tx_data("\rhello world!\r");
    tx_data("abcdefghijklmnopqrstuvwxyz1234567890");
    while(1)
    {
        
    }
    
    return 0;
}

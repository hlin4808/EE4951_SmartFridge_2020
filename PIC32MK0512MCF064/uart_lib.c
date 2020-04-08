#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <cp0defs.h>
#include <sys/attribs.h>
#include "uart_lib.h"

// strings from app formats
// R for retrieve from fridge
// E for enter new data to fridge
// @ is end character

volatile int tx_tail = 0;
volatile int tx_head = 0;
volatile int rx_i = 0;
volatile char rx_buff[RX_SIZE];
volatile char command_buff[RX_SIZE];
volatile char tx_buff[TX_SIZE];
int tx_flag = 0;                        // for in progress transmissions
int command_flag = 0;                   // did we receive a new command?

// initialize UART1 and UART2
// UART1 is a backup that is turned off right now
// to turn on UART1 set ...IE, ...EN, and ON
void uart_init()
{    
    TRISAbits.TRISA7 = 1;   // RA7 (U1RX) input
    TRISAbits.TRISA0 = 0;   // RA0 (U1TX) output
    TRISGbits.TRISG7 = 1;   // RG7 (U2RX) input
    TRISBbits.TRISB14 = 0;  // RB14 (U2TX) output
    
    // iolock (cfgcon<13>) do we need this?
    
    //Peripheral Pin Select (PPS) Settings for UART
    PPSUnLock();
    U1RXR = 0b0111;         // U1RX to RPA7
    RPA0R = 0b00001;        // U1TX to RPA0
    U2RXR = 0b1010;         // U2RX to RPG7
    RPB14R = 0b00010;       // U2TX to RPB14
    PPSLock();
    
    
    // UART off
    // 8 bit data, no parity, 1 stop bit
    U1MODE = 0;
    U2MODE = 0;
    U1MODEbits.CLKSEL = 0b10;   // BRG clock is FRC (8MHz)
    U2MODEbits.CLKSEL = 0b10;   // BRG clock is FRC (8MHz)
    
    // UxBRG = (CLKSEL freq / (16 * desired baud rate)) - 1
    // for baud rate = 9600 -> UxBRG = 51.08
    U1BRG = 51;
    U2BRG = 51;
    
    //Interrupt priorities
//    IPC9bits.U1RXIP = 4;
//    IPC9bits.U1RXIS = 1;
//    IPC10bits.U1TXIP = 3;
//    IPC10bits.U1TXIS = 1;
    IPC14bits.U2RXIP = 3;
    IPC14bits.U2RXIS = 1;
//    IPC14bits.U2TXIP = 4;
//    IPC14bits.U2TXIS = 1;
    
    // reset interrupt flags
    IFS1bits.U1RXIF = 0;
    IFS1bits.U1TXIF = 0;
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    
    // enable interrupts
    IEC1bits.U1RXIE = 0;    // U1RX interrupts disabled
    IEC1bits.U1TXIE = 0;    // U1TX interrupts disabled
    IEC1bits.U2RXIE = 1;    // U2RX interrupts disabled
    IEC1bits.U2TXIE = 0;    // U2TX interrupts disabled
    
    // tx interrupt when transmit buffer has at least one empty space
    // rx interrupt when receive buffer is not empty
    U1STA = 0;
    U1STAbits.URXEN = 0;    // U1RX disabled
    U1STAbits.UTXEN = 0;    // U1TX disabled
    U2STA = 0;
    U2STAbits.URXEN = 1;    // U2RX enabled
    U2STAbits.UTXEN = 1;    // U2TX enabled
    
    // turn on UART
    U1MODEbits.ON = 0;      // UART1 off
    U2MODEbits.ON = 1;      // UART2 on
}


// send "string" to app
void tx_data(const char data[])
{
    int i = 0;
    
    U2STAbits.UTXEN = 1;                // Make sure transmitter is enabled
    
    while(data[i] != 0)         // send data until end of string (NULL char)
    {
        if(i >= TX_SIZE)        // dont send too big of a string
            break;
        
        while(U2STAbits.UTXBF);             // Wait while buffer is full
        U2TXREG = data[i];                  // Transmit character
        
        i++;
    }
}


// interrupt for when we receive data
void __ISR(_UART2_RX_VECTOR, ipl4auto)U2RX_Interrupt(void)
{
    int i;
    
    IFS1bits.U2RXIF = 0;
    
    rx_buff[rx_i] = U2RXREG;    // read UxRXREG

    if(rx_buff[rx_i] == '@')    // check for end character from app
    {
        for(i = 0; i <= rx_i; i++)
        {
            command_buff[i] = rx_buff[i];   // copy received data into command buffer
                                            // we do this so that we always have a complete command
                                            // the receive buffer will have an incomplete command in the middle of receiving
                                            // so not a good idea to look at rx_buff for commands
        }
        
        command_flag = 1;       // new command received
        rx_i = 0;               // reset receive buffer index for next command
    }
    else if(++rx_i >= RX_SIZE)  // stay at last index if exceeding buffer size
        rx_i = RX_SIZE - 1;
}


// checks if new command is received from app
// performs command if there is one
// command is stored in command_buff where the first char is the command identifier, followed by the data, and the last char is always '@'
// this function should be called at a set interval in main to periodically check for new commands
void app_command(void)
{
    int month, day, year;
    
    if(command_flag)
    {
        // new command has been received
        switch(command_buff[0])         // look at first char of received string for command identifier
        {
            case 'E':   // enter item into directory
                // DO SOMETHING HERE
                break;
            
            case 'R':   // request item from directory
                // DO SOMETHING HERE
                break;
            
            case 'V':   // view a list of fridge contents
                // DO SOMETHING
                break;
            
            case 'D':   // view a list of expiration dates for each food
                // DO SOMETHING
                break;
            
            case 'Z':   // app sending date
                // since command_buff is type char we need to subtract '0' to get the correct int number due to ascii format
                month = 10 * (command_buff[1] - '0');   // adds 10's digit
                month +=  command_buff[2] - '0';        // adds 1's digit
                
                day = 10 * (command_buff[3] - '0');     // adds 10's digit
                day +=  command_buff[4] - '0';          // adds 1's digit
                
                year = 1000 * (command_buff[5] - '0');  // adds 1000's digit
                year +=  100 * (command_buff[6] - '0'); // adds 100's digit
                year +=  10 * (command_buff[7] - '0');  // adds 10's digit
                year +=  command_buff[8] - '0';         // adds 1's digit
                
                break;
            
            default:    // invalid command identifier
                // do something? error message? or do nothing?
                break;
        }
        
        command_flag = 0;       // reset flag until new command received
    }
}


/*
// transmit data to app
// interrupt not working so sending data using tx_data() function instead
void __ISR(_UART2_TX_VECTOR, ipl3auto)U2TX_Interrupt(void)
{
    IFS1bits.U2TXIF = 0;

    if(++tx_tail < tx_head)
    {
        U2TXREG = tx_buff[tx_tail];
    }
    else
    {
        tx_flag = 0;
    }
}
*/



// below are example functions from https://www.digikey.com/eewiki/display/microcontroller/Getting+Started+with+the+PIC32+and+MPLAB+X#GettingStartedwiththePIC32andMPLABX-UARTConfiguration
void SendChar(char c)
{
    U2STAbits.UTXEN = 1;                // Make sure transmitter is enabled
    // while(CTS)                       // Optional CTS use
    while(U2STAbits.UTXBF);             // Wait while buffer is full
    U2TXREG = c;                        // Transmit character
}

void SendString(char *string)
{
     
   int i = 0;
     
   U1STAbits.UTXEN = 1;                // Make sure transmitter is enabled
     
   while(*string)
    {
        while(U1STAbits.UTXBF);         // Wait while buffer is full
        U1TXREG = *string;              // Transmit one character
        string++;                       // Go to next character in string
    }
}

char ReadChar(void)
{
    char c;
    //RTS = 0                           // Optional RTS use
    while(!U2STAbits.URXDA);            // Wait for information to be received
    //RTS = 1
    c = U2RXREG;
    return c;                     // Return received character
}

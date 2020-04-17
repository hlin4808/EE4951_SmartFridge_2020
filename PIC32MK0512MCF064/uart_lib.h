#ifndef UART_LIB_H
#define	UART_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define RX_SIZE 50
    #define TX_SIZE 50
    
    //Convenient macrso to do IOUNLOCK
    #define PPSUnLock() {SYSKEY=0x0;SYSKEY=0xAA996655;SYSKEY=0x556699AA;CFGCONbits.IOLOCK=0;} 
    #define PPSLock() {SYSKEY=0x0;SYSKEY=0xAA996655;SYSKEY=0x556699AA;CFGCONbits.IOLOCK=1;}
    
    void uart_init();
    void tx_data(const char data[]);
    void app_command(void);
    void app_move(void);
    
    // below are example functions from https://www.digikey.com/eewiki/display/microcontroller/Getting+Started+with+the+PIC32+and+MPLAB+X#GettingStartedwiththePIC32andMPLABX-UARTConfiguration
    void SendChar(char c);
    void SendString(char *string);
    char ReadChar(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_LIB_H */


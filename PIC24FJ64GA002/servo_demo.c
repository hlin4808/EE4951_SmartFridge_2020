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

void setup(void);
void initServo(void);
void setServo(int Val);
void initButton(void);
void __attribute__((interrupt, auto_psv)) _T2Interrupt();
void __attribute__((interrupt, auto_psv)) _IC1Interrupt();

volatile long int overflow;
volatile int period_index = 0;

volatile int servo_pos;     // 1 = extend, 0 = retract

int main(void) {
    setup();
    servo_pos = 0;      // retract
    setServo(2000);     // retract
    
    while(1)
    {
        if(servo_pos)
            setServo(4000);     // extend
        else
            setServo(2000);     // retract
    }
    return 0;
}

void setup(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xffff; //0x000E All inputs digital
    
    initServo();
    initButton();
}

void initServo(void)
{
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 8;  // Use Pin RP8 = "8", for Input Capture 1 (Table 10-2)
    RPOR3bits.RP6R = 18;  // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    TRISBbits.TRISB6 = 0; //Set RB6 to output
    
    //Setup Timer 3 - for Output Compare
    T3CON = 0;
    T3CONbits.TON = 0; //Stops 16-bit Timer3
    T3CONbits.TSIDL = 0; //Continue module operation in Idle mode
    T3CONbits.TCS = 0; //Use Internal clock (32MHz/2)
    T3CONbits.TGATE = 0; //Gated time accumulation disabled
    T3CONbits.TCKPS = 0b01; //divide by 8, period = 500ns
    TMR3 = 0;     // Initialize to zero
    PR3 = 40000-1; // Set 20ms duration
    
    //Setup Output Compare 1 (using Timer 3)
    OC1CON = 0;
    IFS0bits.OC1IF = 0; // Clear the OC1 interrupt status flag
    OC1CONbits.OCTSEL = 1; // Use Timer 3 for capture source
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults
    //OC1CONbits.OCM = 0b000; // Output compare off
    OC1R = 2000;  // Start servo all the way to the left (1ms) (-90 degree)
    OC1RS = 2000; // Being over cautious here...
    
    T3CONbits.TON = 1; //Starts 16-bit Timer3
}

void setServo(int Val) {
    OC1RS = Val;
}

void initButton(void){
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPINR7bits.IC1R = 8;
    __builtin_write_OSCCONL(OSCCON | 0x40);
    
    TRISBbits.TRISB8 = 1;
    CNPU2bits.CN22PUE = 1;

    
    T2CON = 0;
    T2CONbits.TCKPS = 0b11; //prescaler 256
    PR2 = 62499;        // 1 sec
    TMR2 = 0;
    _T2IF = 0;
    _T2IE = 1;
    
    IC1CON = 0; // Turn off and reset internal state of IC1
    IC1CONbits.ICTMR = 1; // Use Timer 2 for capture source
    IC1CONbits.ICM = 0b011; // Turn on and capture every rising edge
    _IC1IF = 0;
    _IC1IE = 1;

    
    T2CONbits.TON = 1;
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt()
{
    _T2IF = 0;
    overflow++;
}

void __attribute__((interrupt, auto_psv)) _IC1Interrupt()
{
    _IC1IF = 0;
    long int newPeriod = IC1BUF + (overflow * PR2);
    if(newPeriod > 125) //longer than 2ms
    {
        servo_pos++;        // change between 1 (extend) and 0 (retract)
        servo_pos %= 2;
        
        TMR2 = 0;
        overflow = 0;
    }
}

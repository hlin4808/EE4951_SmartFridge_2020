#ifndef MOTOR_LIB_H
#define	MOTOR_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif
    #define M1PWMA  LATBbits.LATB15  //OC7
    #define M1PWMB  LATBbits.LATB0   //OC3
    #define M1AIN1  LATGbits.LATG8
    #define M1AIN2  LATGbits.LATG6
    #define M1BIN1  LATAbits.LATA12
    #define M1BIN2  LATAbits.LATA11

    #define M2PWMA  LATBbits.LATB4   //OC1
    #define M2PWMB  LATAbits.LATA8   //OC9
    #define M2AIN1  LATEbits.LATE12
    #define M2AIN2  LATEbits.LATE13
    #define M2BIN1  LATCbits.LATC2
    #define M2BIN2  LATCbits.LATC1

    #define M3PWMA  LATBbits.LATB7   //OC13
    #define M3PWMB  LATCbits.LATC15  //OC5
    #define M3AIN1  LATCbits.LATC7
    #define M3AIN2  LATCbits.LATC8
    #define M3BIN1  LATCbits.LATC10
    #define M3BIN2  LATDbits.LATD8

    #define M4PWMA  LATBbits.LATB13  //OC14
    #define M4PWMB  LATBbits.LATB9   //OC11
    #define M4AIN1  LATFbits.LATF1
    #define M4AIN2  LATBbits.LATB12
    #define M4BIN1  LATCbits.LATC9
    #define M4BIN2  LATDbits.LATD6

    #define MOTOR1  1
    #define MOTOR2  2
    #define MOTOR3  3
    #define MOTOR4  4

    #define CW      1
    #define CCW     2

    #define M_PHASE1  1
    #define M_PHASE2  2
    #define M_PHASE3  3
    #define M_PHASE4  4
    
    void motor_init(void);
    void motor_step50(int motor_num, int dir, int delay);
    void motor_set_phase(int motor_num, int phase);

#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_LIB_H */

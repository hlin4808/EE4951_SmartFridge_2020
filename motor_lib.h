#ifndef MOTOR_LIB_H
#define	MOTOR_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif

    void motor_init(void);
    void motor_step50(int motor_num, int dir, int delay);
    void motor_set_phase(int motor_num, int phase);

#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_LIB_H */

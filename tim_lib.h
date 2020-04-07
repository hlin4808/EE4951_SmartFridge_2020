#ifndef TIM_LIB_H
#define	TIM_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif

    void tim_init(void);
    void tim_delay_ms(int t);
    void tim_delay_us(int t);

#ifdef	__cplusplus
}
#endif

#endif	/* TIM_LIB_H */

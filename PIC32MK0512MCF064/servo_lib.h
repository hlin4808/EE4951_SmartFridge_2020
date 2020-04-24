#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H

#ifdef __cplusplus
extern "C" {
#endif

    #define SERVO_EXT   1
    #define SERVO_RET   0
    
    void servo_init(void);
    void servo_setCompare(int val);
    void servo_ext_ret(int dir);

#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

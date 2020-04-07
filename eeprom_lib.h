/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EEPROM_LIB_H    /* Guard against multiple inclusion */
#define _EEPROM_LIB_H


#ifdef __cplusplus
extern "C" {
#endif
    
    void EEWriteExecute();
    void eeprom_init();
    void eeprom_write();
    void eeprom_read();


#ifdef __cplusplus
}
#endif

#endif _EEPROM_LIB_H

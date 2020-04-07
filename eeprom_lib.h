eeprom_lib.h
K

B

H
L
Type
C
Size
673 bytes
Storage used
0 bytesOwned by umn.edu
Location
eeprom and flash libs
Owner
Kelli Schmidt
Modified
Apr 2, 2020 by Kelli Schmidt
Opened
5:26 PM by me
Created
Apr 2, 2020
Add a description
Viewers can download
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

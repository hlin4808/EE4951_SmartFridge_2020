/* 
 * File:   flash_lib.h
 * Author: Kelli Schmidt
 *
 * Created on March 30, 2020, 11:10 PM
 */

#ifndef FLASH_LIB_H
#define	FLASH_LIB_H

void NVMInitiateOperation();
void flash_write();
void flash_read();

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FLASH_LIB_H */

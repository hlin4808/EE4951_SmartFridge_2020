#include <stdio.h>
#include <xc.h>

#define EEPROM_NOT_READY 8

void EEWriteExecute(int waitForDone)
{
    /* Disable interrupts prior to unlock sequence */
    EEKEY = 0xEDB7; // Unlock the EEPROM to enable writing
    EEKEY = 0x1248;
    EECONbits.RW = 1; // Execute the write
    /* Re-enable interrupts */
    if (waitForDone) // Wait for the write to finish if
    while (EECONbits.RW); // desired
}
void eeprom_init(void) // Basic EEPROM enable and initialization
{
    EECONbits.ON = 1;// Turn on the EEPROM
    while (EECONbits.RDY == 0);// Wait until EEPROM is ready (~125 us)
    EECONbits.WREN = 1;// Enable writing to the EEPROM
    EECONbits.CMD = 0b100;// Set the command to Configuration Write
    EEADDR = 0x00;// Addr 0x00 = DEVEE0;
    EEDATA = DEVEE0;
    EEWriteExecute(1); // Execute write and wait for finish
    EEADDR = 0x04;// Addr 0x04 = DEVEE1;
    EEDATA = DEVEE1;
    EEWriteExecute(1); // Execute write and wait for finish
    EEADDR = 0x08;// Addr 0x08 = DEVEE2;
    EEDATA = DEVEE2;
    EEWriteExecute(1); // Execute write and wait for finish
    EEADDR = 0x0C;// Addr 0x0C = DEVEE3;
    EEDATA = DEVEE3;
    EEWriteExecute(1); // Execute write and wait for finish
    EECONbits.WREN = 0; // Turn off writes.
}


/*
* Function returns ERR<1:0> bits or EEPROM_NOT_READY.
* Requested data is stored in ee_data
* Returns zero = no error occurred during the read cycle
* non-zero = error occurred during the read cycle
* or Data EEPROM is not ready
*/
int eeprom_read(unsigned int ee_addr, int *ee_data)
{
    if (EECONbits.RDY==1)               // If Data EEPROM to be ready
    {
        if (EECONbits.RW==0)            // If no operation underway
        {                               // Execute Read Command
            EEADDR = ee_addr & 0xFFC;   // Set address on 32-bit boundary
            EECONbits.CMD = 1;          // Load CMD<2:0> with
                                        // Data EEPROM read command
            EECONbits.WREN = 0;         // Access for read
            EECONbits.RW = 1;           // Start the operation
            while (EECONbits.RW==1);    // Wait until read is complete
            *ee_data = EEDATA;          // Read the data
            return EECONbits.ERR;
        }
    }
    return EEPROM_NOT_READY;
}


/*
* Function returns ERR<1:0> bits
* Returns zero = no error occurred during the write cycle
* non-zero = error occurred during the write cycle
* or Data EEPROM is not ready
*/
int eeprom_write(unsigned int ee_addr, int ee_data)
{
    if (EECONbits.RDY==1)               // If Data EEPROM to be ready
    {
        if (EECONbits.RW==0)            // If no operation underway
        {                               // Execute Write Command
            EEADDR = ee_addr & 0xFFC;   // Load address on a 32-bit boundary
            EECONbits.CMD = 0;          // Load CMD<2:0> with write command
            EECONbits.WREN = 1;         // Access for write
            EEDATA = ee_data;
            EEKEY = 0xED87;             // Write unlock sequence
            EEKEY = 0x1248;
            EECONbits.RW = 1;           // Start the write cycle
            while (EECONbits.RW==1);
                                        //Wait for write cycle to complete
                                        // Return ERR<1:0> bits. Zero if no error.
            return EECONbits.ERR;
        }
    }
    return EEPROM_NOT_READY;
}

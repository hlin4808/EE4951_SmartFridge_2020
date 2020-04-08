#include <stdio.h>
#include "xc.h"

void NVMInitiateOperation(void)
{ 
    int int_status;  // storage for current Interrupt Enable state 
    int dma_susp;   // storage for current DMA state 
    // Disable Interrupts 
    asm volatile('di%0' : '=r'(int_status));    
    // Disable DMA 
    if(!(dma_susp=DMACONbits.SUSPEND)) 
    { 
        DMACONSET=_DMACON_SUSPEND_MASK; // suspend 
        while((DMACONbits.DMABUSY));    // wait to be actually suspended} 
        NVMKEY = 0x0;NVMKEY = 0xAA996655; 
        NVMKEY = 0x556699AA; 
        NVMCONSET = 1 << 15;    // must be an atomic instruction 
        // Restore DMA 
        if(!dma_susp) 
        { 
            DMACONCLR=_DMACON_SUSPEND_MASK; // resume DMA activity
        }
        // Restore Interrupts
        if(int_status & 0x00000001) 
        {
            asm volatile('ei');
        }
    }
}

int flash_write(int flash_addr, int flash_data)
{
// Set up Address and Data Registers
NVMADDR = flash_addr & 0xFFC; // physical address 
NVMDATA0 = flash_data; // value
// set the operation, assumes WREN = 0
NVMCONbits.NVMOP = 0x1;// NVMOP for Word programming
// Enable Flash for write operation and set the NVMOP 
NVMCONbits.WREN = 1;// Start programming
NVMInitiateOperation();// see Example 52-1
// Wait for WR bit to clear
while(NVMCONbits.WR);// Disable future Flash Write/Erase operations
NVMCONbits.WREN = 0;// Check Error Status
if(NVMCON & 0x3000)// mask for WRERR and LVDERR
{// process errors}
}

int flash_read(int flash_addr)
{
    int *addrToRead = flash_addr;
    int data = *addrToRead;
}










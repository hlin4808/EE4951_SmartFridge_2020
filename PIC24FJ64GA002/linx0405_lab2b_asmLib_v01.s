.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _write_0, _write_1, _wait_100us, _wait_1ms, _wait_1us, _wait_10us
    
_wait_1us:	    ; 16 cycles * 62.5ns = 1000ns = 1us
		    ; 16 - 3 - 2 - 1 -1 = 9
		    ; 2 cycles for a function call
    REPEAT #9     ; l cycle to read repeat
    NOP		    ;(9+1)times repeat nop = 10 cycles
    RETURN	    ; 3 cycles for a return

_wait_10us:	    ; 160 cycles * 62.5ns = 10us
		    ; 160 - 3 - 2 - 1 -1 = 153
		    ; 2 cycles for a function call
    REPEAT #153   ; l cycle to read repeat
    NOP		    ;(153+1)times repeat nop = 154 cycles
    RETURN	    ; 3 cycles for a return

_wait_100us:    ; 1600 cycles * 62.5ns = 100us   
		    ; 1600 - 3 - 2 - 1-1 = 1593
		    ; 2 cycles for a function call
    REPEAT #1593  ; l cycle to read repeat
    NOP		    ; (1593+1)times repeat nop = 1594 cycles
    RETURN	    ; 3 cycles for a return

_wait_1ms:	    ; 16000 cycles * 62.5ns = 1ms
		    ; 16000 = 2(func call) + (1+15995) + 3 (return)
    		    ; 16000 - 3 - 2 - 1 - 1 = 15993 
		    ; 2 cycles for a function call
    REPEAT #15993  ; l cycle to read repeat	    
    NOP		    ; (15993+1)times repeat nop = 15994 cycles
		    
    RETURN	    ; 3 cycles for a return

_write_0: ; .35us high (5.6 cycles --> 6), 0.85us low (13.6 cycles --> 14)
    bset	LATA, #0
    repeat #3    	; 6 cycles including repeat itself and 4 nops and bclr
    nop
    bclr	LATA, #0
    repeat #6 ; ret takes 3, next call takes 2, bset takes 1
    nop
    return
    
_write_1: ; .7us high (11.2 cycles --> 11), 0.5us low (8 cycles)
    bset	LATA, #0
    repeat #8    	; 11 cycles including repeat itself, 9 nops and 1 bclr
    nop
    bclr	LATA, #0
    nop	; ret takes 3, next call takes 2, bset takes 1
    nop
    return

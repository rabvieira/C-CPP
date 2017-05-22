#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
	name isr_asm
	section .text:CODE
	extern gSecCounter
	public isr_asm_start
isr_asm_start:
	;PUSH 	{R7, LR} ;
        
        ; Timer Int Clear (Timer1_base, timer_tima_timout)
	;LDR.N	R2, [PC, #0x270]
	; LDR.N	R0, [PC, #0x278] ;TIMER1_BASE
        ; R0 <- Timer1_base
        MOV32   R0, TIMER1_BASE
	;LDR	R2, [R2]
	;LDR	R2, [R2]
	;MOV	R1, #1 ;TIMER_TIMA_TIMEOUT
	;BLX	R2
        ; STR R1, [0x40031000, #0x24]
        MOVS R1, #1
        STR R1, [R0, #TIMER_O_ICR]
        
        
        ; gSecCounter++
        ;MOV32   R5, 0x2000002C
        MOV32   R5, gSecCounter ;address
        LDR	R4, [R5]
        ADDS	R4, R4, #1
        STR	R4, [R5]
        
        
	;LDR.N	R0, [PC, #0x25c] ; COUNTER
	;LDR	R1, [R0, #0x2c]
	;ADDS	R1, R1, #1
	;STR	R1, [R0, #0x2c]
        
        ; Return
	;POP 	{R0, PC} ; return
        BX LR
	end
        
;    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
;    gSecCounter ++;

;STR R1, [R0, #0x24]

;STR R1, [TIMER1_BASE, #0x24]

;counter: 0x2000002C
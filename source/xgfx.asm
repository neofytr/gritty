bits   16
%include "xgfx.h"

; using call instruction pushes the return address on the stack
; before jmping to the specified location
; the called function can then clear all the stack it used
; and then issue the ret instruction which 
; will pop the return address from the stack and jump to it

global exit

exit:
    ; function prologue
    push bp     ; save the base ptr
    mov  bp, sp ; make the current stack ptr the current base ptr

    xor ax, ax
    int 0x21   ; exit the program (int 0x21 with ax = 0 returns the control back the OS (MS-DOS)

.halt:
    cli
    hlt 
    jmp .halt

    ; function epilogue
    mov sp, bp ; restore the stack ptr
    pop bp     ; restore the base ptr
    ret

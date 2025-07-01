bits   16
%include "xgfx.h"

; using call instruction pushes the return address on the stack
; before jmping to the specified location
; the called function can then clear all the stack it used
; and then issue the ret instruction which 
; will pop the return address from the stack and jump to it

; GCC expects it's own 32-bit ABI even in 16-bit real mode
; we need to follow this ABI in the functions that are called from the C code

global exit
global xwrite
global xputchr

exit:
    ; function prologue
    push ebp      ; save the base ptr
    mov  ebp, esp ; make the current stack ptr the current base ptr

    xor ah, ah
    int 0x21   ; exit the program (int 0x21 with ah = 0 returns the control back the OS (MS-DOS)

.halt:
    cli
    hlt 
    jmp .halt

    ; function epilogue
    mov esp, ebp ; restore the stack ptr
    pop ebp      ; restore the base ptr
    ret

    ; all stack pushes are 16-bit regardless

    ; arg (high order byte)
    ; arg (low order byte)
    ; return address (high order byte)
    ; return address (low order byte)
    ; prev base ptr (high order byte)
    ; prev base ptr (low order byte) <- sp, bp (just after prologue)

    ; 0x61
    ; 0x00 
    ; 0x33
    ; 

xwrite:
    push ebp      ; save the base ptr
    mov  ebp, esp ; make the current stack ptr the current base ptr

    mov dl, BYTE [ebp + 8]
    mov ah, 0x02           ; int 0x21 with ah = 0x02 prints the character in dl
    int 0x21

    mov esp, ebp
    pop ebp
    ret 

xputchar:
    push ebp 
    mov ebp, esp 

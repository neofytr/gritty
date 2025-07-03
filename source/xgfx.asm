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
global xvideo_mode
global xdraw_point_scg
global xdraw_point_bwt
global xget_more_err_info
global xopen_file
global xclose_file
global xread_file
global xwrite_file

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

    ; arg_byte (high order byte)
    ; arg_byte (low order byte)
    ; return address (high order byte)
    ; return address (low order byte)
    ; prev base ptr (high order byte)
    ; prev base ptr (low order byte) <- sp, bp (just after prologue)

    ; 0x61
    ; 0x00 
    ; 0x33
    ; 

xvideo_mode: 
    push ebp
    mov  ebp, esp
    save

    arg_byte al, 0  ; put the mode into al 
    xor      ah, ah
    int      0x10

    restore
    mov esp, ebp
    pop ebp
    ret

    ; y (row)
    ; x (column)

xdraw_point_bwt:
    push ebp
    mov  ebp, esp
    save

    ; change cursor position (int 0x10, 0x02)
    arg_byte dl, 0
    arg_byte dh, 1
    xor      bh, bh
    mov      ah, 0x02
    int      0x10

    ; write character at current cursor position (int 0x10, 0x0a)
    mov ah, 0x0a
    mov al, '+'
    xor bh, bh
    xor bl, bl   ; ignored in text mode (only needed in graphics mode)
    mov cx, 0x01
    int 0x10

    mov eax, 0x01 ; true return value

    restore
    mov esp, ebp
    pop ebp
    ret

xdraw_point_scg:
    push ebp
    mov  ebp, esp
    save

    arg_word cx, 0
    arg_word dx, 1
    arg_byte al, 2 ; 16 colors supported (from 0 to 15)

    ; write graphics pixel at coordinate (int 0x10, 0xc)
    mov ah, 0x0c
    xor bh, bh   ; page number 0
    int 0x10

    mov eax, 0x01

    restore
    mov esp, ebp
    pop ebp
    ret

xget_more_err_info:
    push ebp
    mov  ebp, esp
    save

    arg_word si, 0 ; err
    arg_word di, 1 ; act

    ; get extended error information  (int 0x21, 0x59)
    mov ah, 0x59
    xor bx, bx
    int 0x21

    ; AX contains the returned extended error code 
    ; These can fit into a byte, so, we use the lower byte of AX
    mov BYTE [si], al
    mov BYTE [di], bl ; BL contains the suggested action

    restore
    mov esp, ebp
    pop ebp
    ret

xopen_file:
    push ebp
    mov  ebp, esp
    save 

    arg_word dx, 0 ; filename
    arg_byte al, 1 ; mode 

    ; open file using handle (int 0x21, 0x3D) 
    mov ah, 0x3d
    int 0x21

    jnc .open_file_leave ; if the CF is not set, there is no error and the file handle is in AX
    neg ax               ; return negative of the error code
    
.open_file_leave:
    restore 
    mov esp, ebp
    pop ebp
    ret

xclose_file:
    push ebp
    mov  ebp, esp
    save 

    ; close file using handle (int 0x21, 0x3e)
    arg_word bx, 0    ; file handle to close
    mov      ah, 0x3e
    int      0x21

    jnc .close_file_leave
    neg ax                ; return negative of the error code

.close_file_leave:
    restore 
    mov ax,  0x00 ; success 
    mov esp, ebp
    pop ebp
    ret

xread_file:
    push ebp
    mov  ebp, esp
    save 

    ; read from file/device using file handle (int 0x21, 0x3f)
    mov      ah, 0x3f
    arg_word bx, 0    ; file handle 
    arg_word cx, 1    ; number of bytes
    arg_word dx, 2    ; buffer ptr
    int      0x21

    jnc .read_file_leave
    ; error 
    neg ax               ; return negative of the error code


.read_file_leave:
    ; if CF is not set, AX contains the number of bytes read 
    ; if AX != CX, then a partial read occured due to end of file
    ; if AX = 0, no data was read, and EOF occured before read 
    restore 
    mov esp, ebp
    pop ebp
    ret

xwrite_file:
    push ebp
    mov  ebp, esp
    save 

    ; write to file/device using handle  (int 0x21, 0x40)
    mov      ah, 0x40
    arg_word bx, 0    ; file handle
    arg_word cx, 1    ; number of bytes to write (a zero value truncates/extends the file to the current file position)
    arg_word dx, 2    ; ptr to buf 
    int      0x21

    jnc .write_file_leave
    ; error 
    neg ax                ; return negated error code

.write_file_leave:
    ; if AX != CX, a partial write occured
    restore 
    mov esp, ebp
    pop ebp
    ret


    

    
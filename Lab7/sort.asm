TITLE Sort

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
str_len DW 0
str_buf DB 255 DUP ("$")

.CODE
.STARTUP
    ; read string
    lea ax, str_len
    push ax
    call read_string
    add sp, 2

    ; set str len
    mov al, BYTE PTR str_len+1
    mov ah, 0
    mov str_len, ax

    ; sort
    lea bx, str_buf
    
    mov cx, str_len
    dec cx
    mov str_len, cx

out_loop_str:

in_loop_str:
    mov si, cx

    mov al, BYTE PTR [bx+0+si]
    cmp al, BYTE PTR [bx+-1+si]
    jg in_loop_str_end
    
    xchg al, BYTE PTR [bx+-1+si]
    xchg BYTE PTR [bx+0+si], al

in_loop_str_end:
    loop in_loop_str

out_loop_str_end:
    inc bx

    mov cx, str_len
    dec str_len
    loop out_loop_str
    
    ; print
    mov ah, 09h
    lea dx, str_buf
    int 21h

    ; exit
    mov ah, 4Ch
    xor al, al
    int 21h

INCLUDE readstr.inc

END
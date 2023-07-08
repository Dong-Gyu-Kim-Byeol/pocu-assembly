

BUFSIZE EQU 256
NEWLINE EQU 13,10
INPUT_NEWLINE_CHAR EQU 13
STREND EQU '$'

FILE_ACCESS_MODE_READ_ONLY EQU 0

.DATA
origin_iv_segment_int3 DW ?
origin_iv_offset_int3 DW ?

iv_segment_int3 DW ?
iv_offset_int3 DW ?

newline_msg DB NEWLINE,STREND
file_name_input_msg_newline DB 'input file name: ',NEWLINE,STREND
newline_typing_start_msg_newline DB NEWLINE,'input start typing: ',NEWLINE,STREND
wrong_character_msg DB '[wrong character intput]',STREND

input_buf_size DB BUFSIZE-1
input_buf_len DB ?
input_buf DB BUFSIZE DUP(?)

file_buf_size DB BUFSIZE-1
file_buf_len DB ?
file_buf DB BUFSIZE DUP(?)

; macro

print_str MACRO msg_offset
    mov ah, 09h
    xor al, al
    mov dx, msg_offset
    int 21h
ENDM

; ------------------------------------------------------------------------------------------------------------------------------
; in
; - buf_start
;   offset  size    description
;   00    	1   	maximum characters buffer can hold
;   01	    1	    number of chars from last input which may be recalled OR number of characters actually read, excluding CR
;   02	    n	    actual characters read, including the final carriage return, size n == buf_start[0]+1
; - end_char
; ------------------------------------------------------------------------------------------------------------------------------
input_str MACRO buf_start, end_char
    xor ax, ax
    mov buf_start[1], al
    mov ah, 0Ah
    mov dx, OFFSET buf_start
    int 21h

    ; set end_char
    xor ax, ax
    mov al, buf_start[1]
    mov si, ax
    mov buf_start[si+2], end_char
ENDM

; -----------------
; -
; -----------------
print_char MACRO char
    mov dl, char
    mov ah, 02h
    xor al, al
    int 21h
ENDM

; -----------------
; out
; - al: ascii char
; -----------------
input_char MACRO
    mov ah, 01h
    xor al, al
    int 21h
ENDM

; -----------------
; -
; -----------------
store_iv MACRO int_num, store_iv_segment, store_iv_offset
    push es

    mov ah, 35h
    mov al, int_num
    int 21h
    
    mov WORD PTR store_iv_segment, es
    mov WORD PTR store_iv_offset, bx

    pop es
ENDM

set_iv MACRO int_num, iv_segment, iv_offset
    push ds

    mov ah, 25h
    mov al, int_num
    mov dx, WORD PTR iv_offset
    mov ds, WORD PTR iv_segment
    int 21h

    pop ds
ENDM

.CODE
.STARTUP
    ; get int 3 interupt vector
    store_iv 3, origin_iv_segment_int3, origin_iv_offset_int3

    ; set int 3 interupt vector
    mov iv_segment_int3, SEG int3_handler
    mov iv_offset_int3, OFFSET int3_handler
    set_iv 3, iv_segment_int3, iv_offset_int3

    ; print file name input
    print_str OFFSET file_name_input_msg_newline
    input_str input_buf_size, 0

    ; file open
    mov ah, 3Dh
    mov al, FILE_ACCESS_MODE_READ_ONLY
    mov dx, OFFSET input_buf
    int 21h

    jc exit

    ; file read
    mov si, ax

    mov bx, ax
    xor cx, cx
    mov cl, file_buf_size
    mov dx, OFFSET file_buf
    mov ah, 3Fh
    mov al, FILE_ACCESS_MODE_READ_ONLY
    int 21h

    jc exit
    
    mov file_buf_len, al
    mov di, ax
    mov file_buf[di], STREND

    ; file close
    mov bx, si
    mov ah, 3Eh
    xor al, al
    int 21h

    jc exit
    
    ; print file str
    print_str OFFSET newline_typing_start_msg_newline
    print_str OFFSET file_buf
    print_str OFFSET newline_msg

    ; check_char
    xor si, si
    xor cx, cx
    mov cl, file_buf_len
    cmp cx, 0
    jz exit

check_char_loop:
    input_char
    
    cmp al, INPUT_NEWLINE_CHAR
    jz exit

    xor ah, ah
    xor dx, dx
    mov dl, file_buf[si]
    assert ax, dx, wrong_character_msg

    inc si

    loop check_char_loop

    ; exit
exit:
    ; set int 3 interupt vector
    set_iv 3, origin_iv_segment_int3, origin_iv_offset_int3

    mov ah, 4Ch
    xor al, al
    int 21h



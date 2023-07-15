TITLE Stat Printer

.DOSSEG
.8086
.8087
.MODEL TINY

INPUT_BUF_MAX_LEN EQU 254

STR_END_CHAR_ASCIZ EQU 0
STR_END_CHAR_CONSOL EQU '$'
NEWLINE EQU 13,10

BUF_MAX_LEN EQU 31*8 ; 248
TIME_FORMAT_MILLI EQU 'm'

FILE_ACCESS_MODE_READ_ONLY EQU 0

.DATA
input_buf DB INPUT_BUF_MAX_LEN+2 DUP (?)
file_handle DW ?

; file: time_format_chracter DB 1개(m: 밀리초, u: 마이크로초) + IEEE 754 64 비트 부동소수점 [1, 30000]개
buf DB BUF_MAX_LEN DUP (?)
convert_milli_div DW ?
float64_count DW ?

; [0, 60999]
avg_milli_l DW ?
avg_milli_h DW ?
newline_avg_str DB NEWLINE
avg_str DB '00.000',STR_END_CHAR_CONSOL

; macro

read_file MACRO file_handle, buf, max_len
    mov ah, 3Fh
    mov al, FILE_ACCESS_MODE_READ_ONLY
    mov bx, file_handle
    mov cx, max_len
    mov dx, OFFSET buf
    int 21h
ENDM

.CODE
.STARTUP
    ; read input
    ; set max len
    mov input_buf[0], INPUT_BUF_MAX_LEN
    ; set now len
    mov input_buf[1], 0
    ; set buf offset
    mov dx, OFFSET input_buf
    ; read
    mov ax, 0A00h
    int 21h

    ; set str end char
    xor ax, ax
    mov al, input_buf[1]
    mov di, ax
    mov input_buf[2+di], STR_END_CHAR_ASCIZ

    ; open file
    mov ah, 3Dh
    mov al, FILE_ACCESS_MODE_READ_ONLY
    mov dx, OFFSET input_buf[2]
    int 21h
    jc exit
    mov file_handle, ax

    ; calc sum
    finit
    ; init sum
    fldz
    fwait
    ; init float64_count
    mov float64_count, 0
    ; init convert_milli_div
    mov convert_milli_div, 1
    read_file file_handle, buf, 1    
    jc exit
    cmp ax, 0
    je print_avg
    cmp buf[0], TIME_FORMAT_MILLI
    je calc_sum_loop
    mov convert_milli_div, 1000

calc_sum_loop:
    read_file file_handle, buf, BUF_MAX_LEN
    jc exit
    cmp ax, 0
    jz print_avg

    mov cl, 3
    shr ax, cl
    mov cx, ax
    add float64_count, ax
    xor si, si
    lea bx, buf

calc_sum_loop_in:
    fadd QWORD PTR [bx+si]
    fwait
    add si, 8
    loop calc_sum_loop_in
    
    jmp calc_sum_loop

print_avg:
    ; check float64_count
    cmp float64_count, 0
    jne calc_avg
    mov avg_milli_l, 0
    jmp make_avg_str

    ; calc avg
calc_avg:
    fidiv float64_count
    ; convert milli
    fidiv convert_milli_div
    frndint
    ; save avg_milli, [0, 60999].
    ; WORD면 signed 범위 초과 시 음수 최대값 비트 패턴(1000 0000 0000 0000)이 저장된다.
    fistp DWORD PTR avg_milli_l
    fwait

    ; make avg str
make_avg_str:
    mov ax, avg_milli_l
    mov cx, 10

    xor dx, dx
    div cx
    add dx, '0'
    mov avg_str[5], dl

    xor dx, dx
    div cx
    add dx, '0'
    mov avg_str[4], dl

    xor dx, dx
    div cx
    add dx, '0'
    mov avg_str[3], dl

    xor dx, dx
    div cx
    add dx, '0'
    mov avg_str[1], dl

    xor dx, dx
    div cx
    add dx, '0'
    mov avg_str[0], dl

    ; print avg str
    mov ah, 09h
    xor al, al
    mov dx, OFFSET newline_avg_str
    int 21h

    ; exit
exit:
    ; file close
    mov bx, file_handle
    mov ah, 3Eh
    xor al, al
    int 21h

    ; exit
    mov ah, 4Ch
    xor al, al
    int 21h

END
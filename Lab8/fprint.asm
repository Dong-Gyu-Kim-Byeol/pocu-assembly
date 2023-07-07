TITLE Formatted Print

.DOSSEG
.8086
.NO87
.MODEL TINY

BUFSIZE EQU 255
NEWLINE EQU 13,10

.DATA
format_msg DB 'put format string: $'

newline_number_msg DB NEWLINE
number_msg DB 'put one number, number is 0~9: $'

input_buf_size DB BUFSIZE
input_buf_len DB ?
input_buf DB BUFSIZE+1 DUP('a')

input_num1 DB ?
input_num2 DB ?

newline_output_buf DB NEWLINE
output_buf DB BUFSIZE+1 DUP('a')

.CODE
.STARTUP
    ; print input format msg
    mov ah, 09h
    xor al, al
    mov dx, OFFSET format_msg
    int 21h

    ; input format string
    xor ax, ax
    mov input_buf_len, al
    mov ah, 0Ah
    mov dx, OFFSET input_buf_size
    int 21h

    xor ax, ax
    mov al, input_buf_len
    mov si, ax

    mov input_buf[si], '$'

    ; print input number msg
    mov ah, 09h
    xor al, al
    mov dx, OFFSET newline_number_msg
    int 21h

    ; input one num
    mov ah, 01h
    xor al, al
    int 21h
    mov input_num1, al

    ; print input number msg
    mov ah, 09h
    xor al, al
    mov dx, OFFSET newline_number_msg
    int 21h

    ; input one num
    mov ah, 01h
    xor al, al
    int 21h
    mov input_num2, al

    ; make output
    xor cx, cx
    mov cl, input_buf_len
    cmp cx, 0
    jz print_output
    
    xor si, si
    xor di, di

    ; is pre character is '%'
    xor dl, dl

make_output_loop:
    mov al, input_buf[si]

    cmp al, '%'
    jne check_format_string
    
    ; set pre character is '%'
    mov dl, 1
    jmp write_output

check_format_string:
    cmp dl, 1
    jne write_output

    ; min:
    cmp al, 'm'
    jne max

    dec di
    mov al, input_num1
    cmp al, input_num2
    jg min_num2

    jmp write_output

min_num2:
    mov al, input_num2
    jmp write_output

    ; max
max:
    cmp al, 'M'
    jne abs_diff

    dec di
    mov al, input_num1
    cmp al, input_num2
    jl max_num2

    jmp write_output

max_num2:
    mov al, input_num2
    jmp write_output

    ; abs diff
abs_diff:
    cmp al, 'd'
    jne write_output

    dec di
    mov al, input_num1
    sub al, input_num2
    jnc abs_diff_convert_char

    not al
    add al, 1
    jnc abs_diff_convert_char

abs_diff_convert_char:
    or al, 00110000b

    ; write
write_output:
    mov output_buf[di], al

    inc si
    inc di
    loop make_output_loop

    ; print output buf
print_output:
    mov output_buf[di], '$'

    mov ah, 09h
    xor al, al
    mov dx, OFFSET newline_output_buf
    int 21h

    ; exit
    mov ah, 4Ch
    xor al, al
    int 21h

END
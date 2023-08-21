TITLE Formatted Print

.DOSSEG
.8086
.NO87
.MODEL TINY

BUFSIZE EQU 255

.DATA
format_prompt DB 'enter format specifiers: $'
number_prompt DB 'enter a single digit number: $'
newline DB 0Ah, 0Dh, '$'

; 버퍼크기, 읽은 글자수, 문자열
buffer_in   DB BUFSIZE
            DB BUFSIZE+1 DUP(?)
buffer_out  DB BUFSIZE DUP(?)
minimum DB ?
        ; DB '$'
maximum DB ?
        ; DB '$'
diff    DB ?
        ; DB '$'

.CODE
.STARTUP
    ; print str to the console
    printstr MACRO str ;(str) <ah,dx>
        lea dx, str
        mov ah, 9h
        int 21h
    ENDM

    read_char MACRO
        mov ah, 01h
        int 21h
    ENDM

    ; 반환값 0과 함께 프로그램 종료
    EXIT0 MACRO ;() <al, ah, flags>
        mov ah, 4Ch
        xor al, al
        int 21h
    ENDM

    printstr format_prompt

    ; 문자열 읽기
    mov ah, 0Ah
    lea dx, buffer_in
    int 21h

    printstr newline
    
    printstr number_prompt

    read_char

    ; 첫번째 숫자 minimum에 저장
    sub al, '0'
    mov minimum, al

    printstr newline
    
    printstr number_prompt

    read_char

    ; 두번째 숫자 minimum에 저장
    sub al, '0'
    mov maximum, al

    printstr newline

    ; find minimum, maximum, diff
    ; 분기 없는 min/max/abs (참조: Game Engine Gems 2. https://wikidocs.net/142101)
    mov bl, minimum
    mov bh, bl
    sub bh, maximum
    mov al, bl

    mov dl, bh
    mov cl, 07h
    sar dl, cl

    mov cl, dl
    not cl
    and cl, bh
    sub minimum, cl

    mov cl, dl
    and cl, bh
    sub bl, cl
    mov maximum, bl

    mov cl, dl
    xor cl, bh
    sub cl, dl
    mov diff, cl

    ; 문자 형태로 저장
    add minimum, '0'
    add maximum, '0'
    add diff, '0'
    
    cld
    xor ax, ax
    xor ch, ch
    mov cl, buffer_in[1]
    lea si, buffer_in + 2
    lea di, buffer_out
convert_loop:
    jcxz print_result
    lodsb

    ; 남은 글자 수가 2개 미만이면 format 확인 필요 없이 출력
    cmp cx, 2h
    jc write

    ; check if format specifier
    cmp al, '%'
    jne write

is_specifier:
    ; check which specifier
    lodsb 
    dec si

    ; minimum?
    cmp al, 'm'
    jne check_max
    push si
    lea si, minimum
    movsb
    pop si
    inc si
    dec cx
    jmp continue

check_max:
    cmp al, 'M'
    jne check_diff
    push si
    lea si, maximum
    movsb
    pop si

    inc si
    dec cx
    jmp continue

check_diff:
    cmp al, 'd'
    jne check_percentage
    push si
    lea si, diff
    movsb
    pop si
    inc si
    dec cx
    jmp continue

check_percentage:
    cmp al, '%'
    jne write_percentage
    jmp write

write_percentage:
    mov bl, '%'
    mov [di], bl
    inc di
    inc si
    dec cx
write:
    mov [di], al
    inc di

continue:
    dec cx
    jmp convert_loop

print_result:
    mov BYTE PTR [di], '$'

    printstr buffer_out

    mov ah, 4Ch
    xor al, al
    int 21h

    EXIT0

END
TITLE Hello x86

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
msg DB "Hello, x86!$"

.CODE
.STARTUP
    ; 화면 출력
    mov ah, 09h
    lea dx, msg
    int 21h

    ; 프로그램 종료
    mov ah, 4Ch
    xor al, al  ; 리턴값 0
    int 21h
    ; DosBox-X 안에서 아래 명령어를 실행하여 어셈블
    ; ML /W3 hello.asm
END
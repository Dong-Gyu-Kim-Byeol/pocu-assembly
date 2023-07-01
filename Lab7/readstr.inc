read_string PROC NEAR
    push bp
    mov bp, sp

    mov bx, [bp+4]
    mov BYTE PTR [bx+1], 5
    mov BYTE PTR [bx+2], 'z'
    mov BYTE PTR [bx+3], 'a'
    mov BYTE PTR [bx+4], 'A'
    mov BYTE PTR [bx+5], 'D'
    mov BYTE PTR [bx+6], 'k'

    pop bp
    ret
read_string ENDP
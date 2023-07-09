int3_handler PROC
    print_str di
    
    ;; DOSBox-X의 cv는 int3의 주소를 복구하므로 매번 설정해 주어야 한다.
    ;set_iv 3, origin_iv_segment_int3, origin_iv_offset_int3
    ;int 3
    ;set_iv 3, iv_segment_int3, iv_offset_int3
    ;iret

    ; DOSBox-X의 cv가 int3의 주소를 복구하지 않으면 jmp로 더 빠르게 가능하다.
    mov bx, OFFSET origin_iv_offset_int3
    jmp FAR PTR [bx] ; jmp DWORD PTR [bx] 도 가능하다.

int3_handler ENDP
int3_handler PROC
    print_str di
    
    set_iv 3, origin_iv_segment_int3, origin_iv_offset_int3
    int 3
    set_iv 3, iv_segment_int3, iv_offset_int3
    iret

int3_handler ENDP
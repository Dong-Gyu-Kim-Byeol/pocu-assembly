g_board=$80 ; 5x5 board
G_MAX_BOARD_INDEX=24
G_MAX_BOARD_LINE_INDEX=4
G_CALL_NUM_MASK=%10000000

g_num=$00

g_ret=$01

g_temp=$10 ; [$0010, $001F]

callnum: ; (num) -> -
;==================================
; summary: 빙고판에서 num을 찾아 불린 번호로 표시 합니다.
;            
; arguments: num : g_num
;
; returns: -
;
; modifies: A, P, g_board
;==================================
    .SUBROUTINE

    callnum_n_if_call_rts 5

    rts
;==================================

    .MACRO callnum_n_if_call_rts ; (n) -> -
    ;==================================
    ; summary: n x n 빙고판에서 num을 찾아 불린 번호로 표시 합니다.
    ;
    ; arguments: n
    ;
    ; returns: -
    ;
    ; modifies: A, P, g_board
    ;==================================
        lda g_num

N .SET {1}
I .SET 0
    .REPEAT N*N
        callnum_entry_if_call_rts I
I .SET I+1
    .REPEND
    .ENDM
    ;==================================


    .MACRO callnum_entry_if_call_rts ; (g_board_index) -> -
    ;==================================
    ; summary: g_board+g_board_index가 g_num과 동일하면 불린 번호로 표시 합니다.
    ;            
    ; arguments: g_board_index
    ;
    ; returns: -
    ;
    ; modifies: A, P, g_board+g_board_index
    ;==================================
        sec
        cmp g_board+{1}
        bne .end
        ora #G_CALL_NUM_MASK
        sta g_board+{1}
        rts

.end:
    .ENDM
    ;==================================



won: ; (num) -> is_won
;==================================
; summary: 현 플레이어가 이겼다면 $01을 아니라면 $00을 반환합니다.
;            
; arguments: -
;
; returns: g_ret
;
; modifies: A, P, g_ret
;==================================
    .SUBROUTINE
.WON=$01
.NOT_WON=$00

won_check_horizontal_1:
    won_entry 0+0,won_check_horizontal_2
    won_entry 0+1,won_check_horizontal_2
    won_entry 0+2,won_check_horizontal_2
    won_entry 0+3,won_check_horizontal_2
    won_entry 0+4,won_check_horizontal_2

    jmp .won_end

won_check_horizontal_2:
    won_entry 5+0,won_check_horizontal_3
    won_entry 5+1,won_check_horizontal_3
    won_entry 5+2,won_check_horizontal_3
    won_entry 5+3,won_check_horizontal_3
    won_entry 5+4,won_check_horizontal_3

    jmp .won_end

won_check_horizontal_3:
    won_entry 10+0,won_check_horizontal_4
    won_entry 10+1,won_check_horizontal_4
    won_entry 10+2,won_check_horizontal_4
    won_entry 10+3,won_check_horizontal_4
    won_entry 10+4,won_check_horizontal_4

    jmp .won_end

won_check_horizontal_4:
    won_entry 15+0,won_check_horizontal_5
    won_entry 15+1,won_check_horizontal_5
    won_entry 15+2,won_check_horizontal_5
    won_entry 15+3,won_check_horizontal_5
    won_entry 15+4,won_check_horizontal_5

    jmp .won_end

won_check_horizontal_5:
    won_entry 20+0,won_check_vertical_1
    won_entry 20+1,won_check_vertical_1
    won_entry 20+2,won_check_vertical_1
    won_entry 20+3,won_check_vertical_1
    won_entry 20+4,won_check_vertical_1

    jmp .won_end

won_check_vertical_1:
    won_entry 0+0,won_check_vertical_2
    won_entry 5+0,won_check_vertical_2
    won_entry 10+0,won_check_vertical_2
    won_entry 15+0,won_check_vertical_2
    won_entry 20+0,won_check_vertical_2

    jmp .won_end

won_check_vertical_2:
    won_entry 0+1,won_check_vertical_3
    won_entry 5+1,won_check_vertical_3
    won_entry 10+1,won_check_vertical_3
    won_entry 15+1,won_check_vertical_3
    won_entry 20+1,won_check_vertical_3

    jmp .won_end

won_check_vertical_3:
    won_entry 0+2,won_check_vertical_4
    won_entry 5+2,won_check_vertical_4
    won_entry 10+2,won_check_vertical_4
    won_entry 15+2,won_check_vertical_4
    won_entry 20+2,won_check_vertical_4

    jmp .won_end

won_check_vertical_4:
    won_entry 0+3,won_check_vertical_5
    won_entry 5+3,won_check_vertical_5
    won_entry 10+3,won_check_vertical_5
    won_entry 15+3,won_check_vertical_5
    won_entry 20+3,won_check_vertical_5

    jmp .won_end

won_check_vertical_5:
    won_entry 0+4,won_check_cross_1
    won_entry 5+4,won_check_cross_1
    won_entry 10+4,won_check_cross_1
    won_entry 15+4,won_check_cross_1
    won_entry 20+4,won_check_cross_1

    jmp .won_end

won_check_cross_1:
    won_entry 10+2,won_not_won_end

    won_entry 0,won_check_cross_2
    won_entry 5+1,won_check_cross_2
    won_entry 15+3,won_check_cross_2
    won_entry 20+4,won_check_cross_2

    jmp .won_end
    
won_check_cross_2:
    won_entry 10+2,won_not_won_end
    
    won_entry 0+4,won_not_won_end
    won_entry 5+3,won_not_won_end
    won_entry 15+1,won_not_won_end
    won_entry 20+0,won_not_won_end

    jmp .won_end

.won_end:
    lda #.WON
    sta g_ret
    rts
    
won_not_won_end:
    lda #.NOT_WON
    sta g_ret
    rts
;==================================

    
    .MACRO won_entry ; (g_board_index, if_not_called_jmp) -> -
    ;==================================
    ; summary: g_board+g_board_index가 불리지 않은 숫자라면 if_not_called_jmp로 점프합니다.
    ;            
    ; arguments: g_board_index
    ;
    ; returns: -
    ;
    ; modifies: A, P
    ;==================================        
        lda g_board+{1}
        bpl {2}
    .ENDM
    ;==================================
g_temp=$10 ; [$0010, $001F]

g_board=$80 ; 5x5 board
G_MAX_BOARD_INDEX=24
G_MAX_BOARD_LINE_INDEX=4
G_CALL_NUM_MASK=%10000000

g_num=$00

g_ret=$01

callnum: ; (num) -> -
;==================================
; summary: 빙고 판에서 num을 찾아 불린 번호로 표시 합니다.
;            
; arguments: num : g_num
;
; returns: -
;
; modifies: A, P, g_board
;==================================
    .SUBROUTINE

    lda g_num
    
    callnum_entry 0
    callnum_entry 1
    callnum_entry 2
    callnum_entry 3
    callnum_entry 4
    callnum_entry 5
    callnum_entry 6
    callnum_entry 7
    callnum_entry 8
    callnum_entry 9
    callnum_entry 10
    callnum_entry 11
    callnum_entry 12
    callnum_entry 13
    callnum_entry 14
    callnum_entry 15
    callnum_entry 16
    callnum_entry 17
    callnum_entry 18
    callnum_entry 19
    callnum_entry 20
    callnum_entry 21
    callnum_entry 22
    callnum_entry 23
    callnum_entry 24

    rts
;==================================

    .MACRO callnum_entry ; (g_board_index) -> -
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
        bne callnum_entry_{1}_end
        ora #G_CALL_NUM_MASK
        sta g_board+{1}
        eor #G_CALL_NUM_MASK

callnum_entry_{1}_end:
    .ENDM
    ;==================================



won: ; (num) -> -
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
    won_entry 0+0,won_check_vertical_1
    won_entry 0+1,won_check_vertical_2
    won_entry 0+2,won_check_vertical_3
    won_entry 0+3,won_check_vertical_4
    won_entry 0+4,won_check_vertical_5

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
    won_entry 0+4,won_check_horizontal_2
    won_entry 5+4,won_check_horizontal_2
    won_entry 10+4,won_check_horizontal_2
    won_entry 15+4,won_check_horizontal_2
    won_entry 20+4,won_check_horizontal_2

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
    won_entry 20+0,won_check_cross_1
    won_entry 20+1,won_check_cross_1
    won_entry 20+2,won_check_cross_1
    won_entry 20+3,won_check_cross_1
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
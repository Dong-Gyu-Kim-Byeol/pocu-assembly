g_temp=$10 ; [$0010, $001F]

PG1=$0100
G_CALL_NUM_MASK=%10000000

g_num=$00

g_ret=$01

callnum: ; (btbl, n, num) -> -
;==================================
; summary: 빙고 판에서 num을 찾아 불린 번호로 표시를 합니다.
;          이 함수를 사용할 때 스택 정리는 **함수(callee)**가 합니다.
;
; arguments: btbl: S[0], S[1]: n x n 빙고판의 시작 주소
;            n: S[2]: 빙고판의 크기. n의 범위: 0 < n <= 10
;            num: S[3]: 7비트 숫자
;
; returns: -
;
; modifies: A, X, Y, P, S, btbl
;==================================
    .SUBROUTINE
.ret_addr_l=g_temp
.ret_addr_h=g_temp+1
.btbl_addr_l=g_temp+2
.btbl_addr_h=g_temp+3
.n=g_temp+4
.num=g_temp+5

    pla
    sta .ret_addr_l
    pla
    sta .ret_addr_h

    pla
    sta .btbl_addr_l
    pla
    sta .btbl_addr_h

    pla
    sta .n

    pla
    sta .num

    ; first_set_loop
    ldx .n

    ldy .n
    dey

    jmp .loop

.set_loop:
    dex
    beq .end

    ldy .n
    dey

    lda .btbl_addr_l
    clc
    adc .n
    sta .btbl_addr_l
    lda .btbl_addr_h
    adc #0
    sta .btbl_addr_h
    
    lda .num

.loop:
    sec
    cmp (.btbl_addr_l),y
    bne .loop_2
    ora #G_CALL_NUM_MASK
    sta (.btbl_addr_l),y
    jmp .end

.loop_2
    dey
    bpl .loop

    jmp .set_loop

.end:
    lda .ret_addr_h
    pha
    lda .ret_addr_l
    pha

    rts
;==================================


won: ; (btbl, n) -> -
;==================================
; summary: 현 플레이어가 이겼다면 $01을 아니라면 $00을 반환합니다.
;          이 함수를 사용할 때 스택 정리는 **호출자(caller)**가 합니다.
;
; arguments: btbl: S[0], S[1]: n x n 빙고판의 시작 주소
;            n: S[2]: 빙고판의 크기. n의 범위: 0 < n <= 10
;
; returns: g_ret: 이겼다면 $01을 아니라면 $00을 반환합니다.
;
; modifies: A, X, Y, P, g_ret
;==================================
    .SUBROUTINE
.WON=$01
.NOT_WON=$00
.btbl_addr_l=g_temp
.btbl_addr_h=g_temp+1
.n=g_temp+2
.t_btbl_addr_l=g_temp+3
.t_btbl_addr_h=g_temp+4

    tsx

    lda PG1+3,x
    sta .btbl_addr_l
    lda PG1+4,x
    sta .btbl_addr_h

    lda PG1+5,x
    sta .n

    ; .first_set_horizontal_loop
    lda .btbl_addr_l
    sta .t_btbl_addr_l
    lda .btbl_addr_h
    sta .t_btbl_addr_h

    ldx .n

    ldy .n
    dey

    jmp .horizontal_loop

.set_horizontal_loop:
    dex
    beq .first_set_vertical_loop

    ldy .n
    dey

    lda .t_btbl_addr_l
    clc
    adc .n
    sta .t_btbl_addr_l
    bcc .horizontal_loop
    lda .t_btbl_addr_h
    adc #0
    sta .t_btbl_addr_h

.horizontal_loop:
    lda (.t_btbl_addr_l),y
    bpl .set_horizontal_loop

    dey
    bpl .horizontal_loop

    jmp .won_end

.first_set_vertical_loop:
    lda .n
    sec
    sbc #1
    clc
    adc .btbl_addr_l
    sta .t_btbl_addr_l
    lda .btbl_addr_h
    adc #0
    sta .t_btbl_addr_h

    ldx .n
    
    ldy #0

    jmp .vertical_loop

.set_vertical_loop:
    lda .t_btbl_addr_l
    sec
    cmp .btbl_addr_l
    beq .first_set_cross_left_top_to_right_bottom_loop

    ldx .n

    ldy #0

    sec
    sbc #1
    sta .t_btbl_addr_l
    bcs .vertical_loop
    lda .t_btbl_addr_h
    sbc #0
    sta .t_btbl_addr_h

.vertical_loop:
    lda (.t_btbl_addr_l),y
    bpl .set_vertical_loop

    tya
    clc
    adc .n
    tay

    dex
    bne .vertical_loop

    jmp .won_end

.first_set_cross_left_top_to_right_bottom_loop:
    lda .btbl_addr_l
    sta .t_btbl_addr_l
    lda .btbl_addr_h
    sta .t_btbl_addr_h

    ldx .n

    ldy #0

.cross_left_top_to_right_bottom_loop:
    lda (.t_btbl_addr_l),y
    bpl .first_set_cross_left_bottom_to_right_top_loop

    lda .t_btbl_addr_l
    clc
    adc .n
    sta .t_btbl_addr_l
    lda .t_btbl_addr_h
    adc #0
    sta .t_btbl_addr_h

    iny

    dex
    bne .cross_left_top_to_right_bottom_loop

    jmp .won_end

.first_set_cross_left_bottom_to_right_top_loop:
    lda .btbl_addr_l
    sta .t_btbl_addr_l
    lda .btbl_addr_h
    sta .t_btbl_addr_h

    ldy .n
    dey

.cross_left_bottom_to_right_top_loop:
    lda (.t_btbl_addr_l),y
    bpl .not_won_end

    lda .t_btbl_addr_l
    clc
    adc .n
    sta .t_btbl_addr_l
    bcc .cross_left_bottom_to_right_top_loop_2
    lda .t_btbl_addr_h
    adc #0
    sta .t_btbl_addr_h

.cross_left_bottom_to_right_top_loop_2:
    dey
    bpl .cross_left_bottom_to_right_top_loop

.won_end:
    lda #.WON
    sta g_ret
    rts

.not_won_end:
    lda #.NOT_WON
    sta g_ret
    rts


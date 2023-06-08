PG1=$0100

g_ret=$20

g_num0=$10
g_num1=$11

g_temp=$00 ; [$0000, $000F]

min: ; (num0, num1) -> min | <P, g_temp, g_temp+1>
; =========================================================
; in
; num0 : S[0]
; num1 : S[1]
;
; out
; min : g_ret
;
; unsigned num0와 num1 중 더 작은 숫자를 반환합니다
; 이 함수를 사용할 때 스택 정리는 호출자(caller)가 합니다.
; =========================================================
    .SUBROUTINE
.a=g_temp
.x=g_temp+1
    sta .a
    stx .x

    tsx

    lda PG1+4,x ; S[1]
    sec
    cmp PG1+3,x ; S[1] - S[0]

    bcs .ret_num0 ; S[1] >= S[0]

    sta g_ret ; S[1]
    jmp .end

.ret_num0:
    lda PG1+3,x ; S[0]
    sta g_ret

.end:
    lda .a
    ldx .x

    rts
; =========================================================


max: ; (num0, num1) -> max | <X, P, g_temp>
; =========================================================
; in
; num0 : g_num0
; num1 : g_num1
;
; out
; max : X
;
; num0와 num1 중 더 큰 숫자를 반환합니다.
; =========================================================
    .SUBROUTINE
.a=g_temp
    sta .a

    lda g_num0
    cmp g_num1 ; g_num0 - g_num1

    bcc .ret_num1 ; g_num0 < g_num1
    tax
    jmp .end

.ret_num1:
    lda g_num1
    tax

.end:
    lda .a

    rts
; =========================================================


minmax: ; (num0, num1) -> min, max | <P, , g_temp, , g_temp+1, g_temp+2, g_temp+3, g_temp+4, g_temp+5>
; =========================================================
; in
; num0 : S[1]
; num1 : S[0]
;
; out
; min : S[0]
; max : S[1]
;
; unsigned num0와 num1의 min 값을 S[0]에 max 값을 S[1]에 반환합니다.
; 이 함수를 사용할 때 스택 정리는 함수(callee)가 합니다.
; =========================================================
    .SUBROUTINE
.a=g_temp
.x=g_temp+1
.ret_addr_l=g_temp+2
.ret_addr_h=g_temp+3
.num0=g_temp+4
.num1=g_temp+5

    sta .a
    stx .x

    pla
    sta .ret_addr_l
    pla
    sta .ret_addr_h
    pla
    sta .num1
    pla
    sta .num0
    tsx

.min:
    sec
    cmp .num1 ; .num0 - .num1
    bcs .min_num1 ; .num0 >= .num1

    ; .num0 < .num1
    sta PG1+1,x
    lda .num1
    sta PG1+2,x
    jmp .end

.min_num1:
    ; .num0 >= .num1
    sta PG1+2,x
    lda .num1
    sta PG1+1,x

.end:
    lda .ret_addr_h
    pha
    lda .ret_addr_l
    pha

    lda .a
    ldx .x

    rts
; =========================================================


mmref: ; (num0, num1, out_min, out_max) | <P, g_temp, g_temp+1 g_temp+2, g_temp+3, g_temp+4, g_temp+5, g_temp+6, g_temp+7, g_temp+8>
; =========================================================
; in
; num0 : g_num0
; num1 : S[0]
; out_min : S[1](addr l), S[2](addr h)
; out_max : S[3](addr l), S[4](addr h)
;
; unsigned num0와 num1의 min 값을 out_min 주소에 max 값을 out_max 주소에 반환합니다.
; 이 함수를 사용할 때 스택 정리는 함수(callee)가 합니다.
; =========================================================
    .SUBROUTINE
.a=g_temp
.x=g_temp+1
.ret_addr_l=g_temp+2
.ret_addr_h=g_temp+3
.out_min_l=g_temp+4
.out_min_h=g_temp+5
.out_max_l=g_temp+6
.out_max_h=g_temp+7
.num1=g_temp+8

    sta .a
    stx .x

    pla
    sta .ret_addr_l
    pla
    sta .ret_addr_h
    
    pla
    sta .num1

    pla
    sta .out_min_l
    pla
    sta .out_min_h
    pla
    sta .out_max_l
    pla
    sta .out_max_h
    ldx #0

.min:
    lda g_num0
    sec
    cmp .num1 ; g_num0 - .num1
    bcs .min_num1 ; g_num0 >= .num1

    ; g_num0 < .num1
    sta (.out_min_l,x)
    lda .num1
    sta (.out_max_l,x)
    jmp .end

.min_num1:
    ; g_num0 >= .num1
    sta (.out_max_l,x)
    lda .num1
    sta (.out_min_l,x)

.end:
    lda .ret_addr_h
    pha
    lda .ret_addr_l
    pha

    lda .a
    ldx .x

    rts
; =========================================================

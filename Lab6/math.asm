PG1=$0100

ret=$20

num0=$10
num1=$11

temp=$00 ; [$0000, $000F]

min: ; (num0, num1) -> min | <P, temp, temp+1>
; =========================================================
; in
; num0 : S[0]
; num1 : S[1]
;
; out
; min : ret
;
; unsigned num0와 num1 중 더 작은 숫자를 반환합니다
; 이 함수를 사용할 때 스택 정리는 호출자(caller)가 합니다.
; =========================================================
    .SUBROUTINE
.a=temp
.x=temp+1
    sta .a
    stx .x

    tsx

    lda PG1+4,x ; S[1]
    sec
    cmp PG1+3,x ; S[1] - S[0]

    bcs .ret_num0 ; S[1] >= S[0]

    sta ret ; S[1]
    jmp .end

.ret_num0:
    lda PG1+3,x ; S[0]
    sta ret

.end:
    lda .a
    ldx .x

    rts
; =========================================================


max: ; (num0, num1) -> max | <X, P, temp>
; =========================================================
; in
; num0 : num0
; num1 : num1
;
; out
; max : X
;
; num0와 num1 중 더 큰 숫자를 반환합니다.
; =========================================================
    .SUBROUTINE
.a=temp
    sta .a

    lda num0
    cmp num1 ; num0 - num1

    bcc .ret_num1 ; num0 < num1
    tax
    jmp .end

.ret_num1:
    lda num1
    tax

.end:
    lda .a

    rts
; =========================================================


minmax: ; (num0, num1) -> min, max | <P, temp+2, temp+3, temp+4, temp+5>
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
.a=temp+2
.x=temp+3
.ret_addr_l=temp+4
.ret_addr_h=temp+5

    sta .a
    stx .x

    pla
    sta .ret_addr_l
    pla
    sta .ret_addr_h

    ; min
    jsr min

    ; max
    pla
    sta num1
    pla
    sta num0
    jsr max

    pla
    pla

    ; max ret
    txa
    pha

    ; min ret
    lda ret
    pha

.end:
    lda .ret_addr_h
    pha
    lda .ret_addr_l
    pha

    lda .a
    ldx .x

    rts
; =========================================================


mmref: ; (num0, num1, out_min, out_max) | <P, temp+2, temp+3, temp+4, temp+5, temp+6, temp+7, temp+8, temp+9>
; =========================================================
; in
; num0 : num0
; num1 : S[0]
; out_min : S[1](addr l), S[2](addr h)
; out_max : S[3](addr l), S[4](addr h)
;
; unsigned num0와 num1의 min 값을 out_min 주소에 max 값을 out_max 주소에 반환합니다.
; 이 함수를 사용할 때 스택 정리는 함수(callee)가 합니다.
; =========================================================
    .SUBROUTINE
.a=temp+2
.x=temp+3
.ret_addr_l=temp+4
.ret_addr_h=temp+5
.out_min_l=temp+6
.out_min_h=temp+7
.out_max_l=temp+8
.out_max_h=temp+9

    sta .a
    stx .x

    pla
    sta .ret_addr_l
    pla
    sta .ret_addr_h
    
    pla
    sta num1

    pla
    sta .out_min_l
    pla
    sta .out_min_h
    pla
    sta .out_max_l
    pla
    sta .out_max_h

    ; min
    lda num1
    pha
    lda num0
    pha
    jsr min
    pla
    pla

    ; max
    jsr max

    ; max ret
    txa
    ldx #0
    sta (.out_max_l,x)

    ; min ret
    lda ret
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

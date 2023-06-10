    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils/utils.asm"

    ldx #$FF
    txs

t_X = $25
t_Y = $12
test_addr = $0020

;=========min==========
    ; Num0LtNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    lda #4
    pha

    lda #2
    pha

    jsr min

    assert $0020, $02

    sta test_addr
    assert test_addr, $02

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FD

    pla
    pla

    ; Num0GtNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    lda #2
    pha

    lda #4
    pha

    jsr min

    assert test_addr, $02

    sta test_addr
    assert test_addr, $04

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FD

    pla
    pla

    ; Num0EqNum1


    lda #0
    ldx #t_X
    ldy #t_Y

    lda #2
    pha

    lda #2
    pha

    jsr min

    assert test_addr, $02

    sta test_addr
    assert test_addr, $02

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FD

    pla
    pla
;=========min==========

;=========max==========

    ldx #$FF
    txs

t_num0 = $10
t_num1 = $11

    ; Num0LtNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    lda #2
    sta t_num0

    lda #4
    sta t_num1

    jsr max

    stx test_addr
    assert test_addr, $04

    sta test_addr
    assert test_addr, $04

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FF

    ; Num0GtNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    lda #4
    sta t_num0

    lda #2
    sta t_num1

    jsr max

    stx test_addr
    assert test_addr, $04

    sta test_addr
    assert test_addr, $02

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FF

    ; Num0EqNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    lda #2
    sta t_num0

    lda #2
    sta t_num1

    jsr max

    stx test_addr
    assert test_addr, $02

    sta test_addr
    assert test_addr, $02

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FF

;=========max==========

;========minmax========

    ldx #$FF
    txs

    ; Num0LtNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    pha
    pha

    lda #2
    pha

    lda #4
    pha

    jsr minmax

    sta test_addr
    assert test_addr, $04

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    pla
    sta test_addr
    assert test_addr, $02

    pla
    sta test_addr
    assert test_addr, $04

    tsx
    stx test_addr
    assert test_addr, $FF

    ; Num0GtNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    pha
    pha

    lda #4
    pha

    lda #2
    pha

    jsr minmax

    sta test_addr
    assert test_addr, $02

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    pla
    sta test_addr
    assert test_addr, $02

    pla
    sta test_addr
    assert test_addr, $04

    tsx
    stx test_addr
    assert test_addr, $FF

    ; Num0EqNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    pha
    pha

    lda #2
    pha

    lda #2
    pha

    jsr minmax

    sta test_addr
    assert test_addr, $02

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    pla
    sta test_addr
    assert test_addr, $02

    pla
    sta test_addr
    assert test_addr, $02

    tsx
    stx test_addr
    assert test_addr, $FF

;========minmax========

;========mmref=========

    ldx #$FF
    txs

t_out_min = $0080
t_out_max = $0081
t_ref_num0 = $10

    ; Num0LtNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    lda #>t_out_max
    pha

    lda #<t_out_max
    pha

    lda #>t_out_min
    pha

    lda #<t_out_min
    pha

    lda #4
    pha

    lda #2
    sta t_ref_num0

    jsr mmref

    sta test_addr
    assert test_addr, $02

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FF

    assert t_out_min, $02
    assert t_out_max, $04

    ; Num0GtNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    lda #>t_out_max
    pha

    lda #<t_out_max
    pha

    lda #>t_out_min
    pha

    lda #<t_out_min
    pha

    lda #2
    pha

    lda #4
    sta t_ref_num0

    jsr mmref

    sta test_addr
    assert test_addr, $04

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FF

    assert t_out_min, $02
    assert t_out_max, $04

    ; Num0EqNum1

    lda #0
    ldx #t_X
    ldy #t_Y

    lda #>t_out_max
    pha

    lda #<t_out_max
    pha

    lda #>t_out_min
    pha

    lda #<t_out_min
    pha

    lda #2
    pha

    lda #2
    sta t_ref_num0

    jsr mmref

    sta test_addr
    assert test_addr, $02

    stx test_addr
    assert test_addr, t_X

    sty test_addr
    assert test_addr, t_Y

    tsx
    stx test_addr
    assert test_addr, $FF

    assert t_out_min, $02
    assert t_out_max, $02

;========mmref=========

    termin
    
    .INCLUDE "math.asm"

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
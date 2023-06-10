    .PROCESSOR 6502
    .ORG $8000

table = $80
    .INCLUDE "utils/utils.asm"

    ldx #$FF
    txs

    ;===DEFINE BINGO TABLE HERE===
    .SUBROUTINE

    lda #0
    sta $00
    sta $01
    sta $10
    sta $11
    sta $12

    ldy #25
    ldx #24
.loop
    sty table,x
    dey
    dex
    bpl .loop
    ;=============================

    ;============row==============

    lda #$0B
    sta $00
    jsr callnum
    
    lda #$0C
    sta $00
    jsr callnum

    lda #$0D
    sta $00
    jsr callnum

    lda #$0E
    sta $00
    jsr callnum

    lda #$0F
    sta $00
    jsr callnum

    jsr won

    assert $0001, $01

    lda #0
    sta $01

    lda #$0B
    sta $8A

    lda #$0C
    sta $8B

    lda #$0D
    sta $8C

    lda #$0E
    sta $8D

    lda #$0F
    sta $8E

    ;============row==============

    ;============col==============

    lda #$03
    sta $00
    jsr callnum
    
    lda #$08
    sta $00
    jsr callnum

    lda #$0D
    sta $00
    jsr callnum

    lda #$12
    sta $00
    jsr callnum

    lda #$17
    sta $00
    jsr callnum

    jsr won

    assert $0001, $01

    lda #0
    sta $01

    lda #$03
    sta $82

    lda #$08
    sta $87

    lda #$0D
    sta $8C

    lda #$12
    sta $91

    lda #$17
    sta $96

    ;============col==============

    ;==========up-right===========

    lda #$15
    sta $00
    jsr callnum
    
    lda #$11
    sta $00
    jsr callnum

    lda #$0D
    sta $00
    jsr callnum

    lda #$09
    sta $00
    jsr callnum

    lda #$05
    sta $00
    jsr callnum

    jsr won

    assert $0001, $01

    lda #0
    sta $01

    lda #$15
    sta $94

    lda #$11
    sta $90

    lda #$0D
    sta $8C

    lda #$09
    sta $88

    lda #$05
    sta $84
    ;==========up-right===========

    ;==========up-left============

    lda #$19
    sta $00
    jsr callnum
    
    lda #$13
    sta $00
    jsr callnum

    lda #$0D
    sta $00
    jsr callnum

    lda #$07
    sta $00
    jsr callnum

    lda #$01
    sta $00
    jsr callnum

    jsr won

    assert $0001, $01

    lda #0
    sta $01

    lda #$01
    sta $80

    lda #$07
    sta $86

    lda #$0D
    sta $8C

    lda #$13
    sta $92

    lda #$19
    sta $98

    ;==========up-left============

    termin

    .INCLUDE "5bingo.asm"

    .ORG $FFFC, 0
    .WORD $8000
    .WORD $0000
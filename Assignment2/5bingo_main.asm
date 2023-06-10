    .PROCESSOR 6502
    .ORG $8000

LEN=25
table=$80
    .INCLUDE "utils/utils.asm"

    ldx #$FF
    txs

    ;===DEFINE BINGO TABLE HERE===
    .SUBROUTINE
    ldx #LEN
    ldy #LEN-1
.loop:
    stx table,y
    dex
    dey
    bpl .loop
    ;=============================

    lda #$12
    sta $00
    jsr callnum

    assert $0091, $92

    lda #$13
    sta $00
    jsr callnum

    assert $0092, $93

    jsr won

    assert $0001, $00

    lda #$10
    sta $00
    jsr callnum

    lda #$11
    sta $00
    jsr callnum

    lda #$14
    sta $00
    jsr callnum

    jsr won

    assert $0001, $01

    termin
    
    .INCLUDE "5bingo.asm"

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
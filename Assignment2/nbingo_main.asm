    .PROCESSOR 6502
    .ORG $8000
DIM=5
table=$1100

    .INCLUDE "utils/utils.asm"

    ldx #$FF
    txs

    ;===DEFINE BINGO TABLE HERE===
    .SUBROUTINE
    ldx #DIM*DIM
    ldy #DIM*DIM-1
.loop:
    txa
    sta table,y
    dex
    dey
    bpl .loop

    lda table+2
    ora #$80
    sta table+2
    
    lda table+4
    ora #$80
    sta table+4

    lda table+8
    ora #$80
    sta table+8

    lda table+16
    ora #$80
    sta table+16

    lda table+20
    ora #$80
    sta table+20
    ;=============================

    lda #$10
    pha
    lda #DIM
    pha
    lda #$11
    pha
    lda #$00
    pha

    jsr callnum

    assert $110F, $90

    lda #$11
    pha
    lda #DIM
    pha
    lda #$11
    pha
    lda #$00
    pha

    jsr callnum

    assert $1110, $91

    lda #$12
    pha
    lda #DIM
    pha
    lda #$11
    pha
    lda #$00
    pha

    jsr callnum

    assert $1111, $92

    lda #$13
    pha
    lda #DIM
    pha
    lda #$11
    pha
    lda #$00
    pha

    jsr callnum

    assert $1112, $93

    lda #$14
    pha
    lda #DIM
    pha
    lda #$11
    pha
    lda #$00
    pha

    jsr callnum

    assert $1113, $94

    lda #DIM
    pha
    lda #$11
    pha
    lda #$00
    pha

    jsr won

    ; stack cleanup
    tsx
    inx
    inx
    inx
    txs

    assert $0001, $01

    termin

    .INCLUDE "nbingo.asm"

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
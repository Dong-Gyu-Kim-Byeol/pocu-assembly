    .PROCESSOR 6502
    .ORG $8000

LEN=25
table=$80
    .INCLUDE "utils/utils.asm"

    ldx #$FF
    txs

    lda #2
    pha
    lda #4
    pha

    jsr min

    pla
    pla

    assert $0020, $02

    lda #4
    sta $10
    lda #2
    sta $11

    jsr max

    stx $20
    assert $0020, $04

    pha
    pha
    lda #4
    pha
    lda #2
    pha

    jsr minmax

    pla
    sta $20
    assert $0020, $02

    pla
    sta $20
    assert $0020, $04

    lda #>$2301
    pha
    lda #<$2301
    pha
    lda #>$2300
    pha
    lda #<$2300
    pha
    lda #4
    pha
    lda #2
    sta $10

    jsr mmref

    assert $2300, $02
    assert $2301, $04

    termin

    .INCLUDE "math.asm"

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
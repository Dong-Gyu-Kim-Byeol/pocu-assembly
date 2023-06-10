    .PROCESSOR 6502
    .ORG $8000

DIM = 5
table = $C000

    .INCLUDE "utils/utils.asm"
    
    ldx #$FF
    txs

    lda #DIM
    pha
    lda #$C0
    pha
    lda #$00
    pha

    jsr won

    assert $0001, $00
    termin

    .INCLUDE "nbingo.asm"

    .ORG $C000, 0
    .BYTE $00, $00, $00, $00, $00
    .BYTE $00, $00, $00, $00, $00
    .BYTE $00, $00, $00, $00, $00
    .BYTE $00, $00, $00, $00, $00
    .BYTE $00, $00, $00, $00, $00

    .ORG $FFFC, 0
    .WORD $8000
    .WORD $0000

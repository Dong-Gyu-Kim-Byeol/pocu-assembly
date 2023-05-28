    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils/utils.asm"

    ;===DEFINE INPUTS HERE===
    ; 0xFFFFFB2E (-1234)
    lda #$2E
    sta $4000
    lda #$FB
    sta $4001
    lda #$FF
    sta $4002
    lda #$FF
    sta $4003
    ;========================

    .INCLUDE "divby2.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $97
    assert $2301, $FD
    assert $2302, $FF
    assert $2303, $FF
    ;============================

    termin

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
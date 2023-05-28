    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils/utils.asm"

    ;===DEFINE INPUTS HERE===
    ; 0x000000BA (186)
    lda #$BA
    sta $4000
    lda #$00
    sta $4001
    sta $4002
    sta $4003

    ; 0xFFFFFB2F (-1233)
    lda #$2F
    sta $4004
    lda #$FB
    sta $4005
    lda #$FF
    sta $4006
    sta $4007
    ;========================

    .INCLUDE "addle.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $E9
    assert $2301, $FB
    assert $2302, $FF
    assert $2303, $FF
    ;============================

    termin

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
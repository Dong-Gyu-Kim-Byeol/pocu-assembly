    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils/utils.asm"

    ;===DEFINE INPUTS HERE===
    ; 0x000000BA (186)
    lda #$00
    sta $4000
    sta $4001
    sta $4002
    lda #$BA
    sta $4003

    ; 0xFFFFFB2F (-1233)
    lda #$FF
    sta $4004
    sta $4005
    lda #$FB
    sta $4006
    lda #$2F
    sta $4007
    ;========================

    .INCLUDE "addbe.asm"

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
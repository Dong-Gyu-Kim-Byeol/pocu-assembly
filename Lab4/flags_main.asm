    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils/utils.asm"

    ;===DEFINE RESET OUT MEM HERE===
    ; 0xFFFFFFFF
    lda #$FF
    sta $2300
    sta $2301
    sta $2302
    sta $2303
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $01
    assert $2302, $01
    assert $2303, $01
    ;============================

    termin

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
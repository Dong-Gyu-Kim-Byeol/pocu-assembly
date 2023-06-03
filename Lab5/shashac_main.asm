    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils/utils.asm"

    .INCLUDE "shashac.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $5D
    assert $2301, $1D
    assert $2302, $8E
    assert $2303, $F6
    ;============================

    termin

    .ORG $C000, 0
    .BYTE $04,$81
    .BYTE $21,$00,$21,$31
    .BYTE $42,$A2,$FF,$61
    .BYTE $00,$00,$00,$00
    .BYTE $11,$10,$31,$77

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
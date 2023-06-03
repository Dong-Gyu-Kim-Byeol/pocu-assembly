    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils/utils.asm"

    ;----------AllClear----------
    ;===DEFINE INPUT HERE===
    clc
    lda #$00
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $00
    assert $2301, $00
    assert $2302, $00
    assert $2303, $00
    ;============================
    ;----------AllClear----------

    ;---------NegativeSet--------
    ;===DEFINE INPUT HERE===
    clc
    lda #$80
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $00
    assert $2302, $00
    assert $2303, $00
    ;============================
    ;---------NegativeSet--------

    ;---------OverflowSet--------
    ;===DEFINE INPUT HERE===
    clc ; OverflowSet
    lda #$40
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $00
    assert $2301, $01
    assert $2302, $00
    assert $2303, $00
    ;============================
    ;---------OverflowSet--------

    ;-----------ZeroSet----------
    ;===DEFINE INPUT HERE===
    clc ; ZeroSet
    lda #$02
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $00
    assert $2301, $00
    assert $2302, $01
    assert $2303, $00
    ;============================
    ;-----------ZeroSet----------

    ;---------CarrySet-----------
    ;===DEFINE INPUT HERE===
    clc ; CarrySet
    lda #$01
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $00
    assert $2301, $00
    assert $2302, $00
    assert $2303, $01
    ;============================
    ;---------CarrySet-----------

    ;--------TwoFlagsSet---------
    ;===DEFINE INPUT HERE===
    clc
    lda #%00000011
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $00
    assert $2301, $00
    assert $2302, $01
    assert $2303, $01
    ;============================

    ;===DEFINE INPUT HERE===
    clc
    lda #%10000001
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $00
    assert $2302, $00
    assert $2303, $01
    ;============================

    ;===DEFINE INPUT HERE===
    clc
    lda #%10000010
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $00
    assert $2302, $01
    assert $2303, $00
    ;============================

    ;===DEFINE INPUT HERE===
    clc
    lda #%11000000
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $01
    assert $2302, $00
    assert $2303, $00
    ;============================

    ;===DEFINE INPUT HERE===
    clc
    lda #%01000001
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $00
    assert $2301, $01
    assert $2302, $00
    assert $2303, $01
    ;============================

    ;===DEFINE INPUT HERE===
    clc
    lda #%01000010
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $00
    assert $2301, $01
    assert $2302, $01
    assert $2303, $00
    ;============================
    ;--------TwoFlagsSet---------

    ;-------ThreeFlagsSet--------
    ;===DEFINE INPUT HERE===
    clc
    lda #%10000011
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $00
    assert $2302, $01
    assert $2303, $01
    ;============================

    ;===DEFINE INPUT HERE===
    clc
    lda #%01000011
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $00
    assert $2301, $01
    assert $2302, $01
    assert $2303, $01
    ;============================

    ;===DEFINE INPUT HERE===
    clc
    lda #%11000010
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $01
    assert $2302, $01
    assert $2303, $00
    ;============================

    ;===DEFINE INPUT HERE===
    clc
    lda #%11000001
    pha
    plp
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $01
    assert $2302, $00
    assert $2303, $01
    ;============================
    ;-------ThreeFlagsSet--------

    termin

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000
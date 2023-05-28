out_n = $2300
out_v = $2301
out_z = $2302
out_c = $2303

    ldx #$FF
    txs

    php
    pla
    tax
    
    ; n flag
    txa
    and #%10000000
    rol
    rol
    sta out_n

    ; v flag
    txa
    and #%01000000
    rol
    rol
    rol
    sta out_v
    
    ; z flag
    txa
    and #%00000010
    lsr
    sta out_z

    ; c flag
    txa
    and #%00000001
    sta out_c
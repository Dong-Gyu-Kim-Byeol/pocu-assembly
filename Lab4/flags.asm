out_n = $2300
out_v = $2301
out_z = $2302
out_c = $2303

    php
    pla
    tax
    
    ; n flag
    txa
    and #$80
    clc
    rol
    rol
    sta out_n

    ; v flag
    txa
    and #$40
    clc
    rol
    rol
    rol
    sta out_v
    
    ; z flag
    txa
    and #$02
    lsr
    sta out_z

    ; c flag
    txa
    and #$01
    sta out_c
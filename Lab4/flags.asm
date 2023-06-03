out_n = $2300
out_v = $2301
out_z = $2302
out_c = $2303

    php
    pla
    tax
    
    ; n flag
    txa
    rol
    rol
    and #$01
    sta out_n

    ; v flag
    txa
    rol
    rol
    rol
    and #$01
    sta out_v
    
    ; z flag
    txa
    lsr
    and #$01
    sta out_z

    ; c flag
    txa
    and #$01
    sta out_c
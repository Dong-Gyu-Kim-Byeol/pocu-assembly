buffer=$C000
out=$2300

accum_last_index=$00
temp=$01 ; $01 ~ $03

    ldx #$FF
    txs

    lda #0
    sta temp
    sta temp+1
    sta temp+2
    tay
    
    lda buffer
    asl
    asl
    sec
    sbc #4
    sta accum_last_index

round:
    ldx accum_last_index

accum:
    lda temp
    eor buffer+2,x
    sta temp

    lda temp+1
    eor buffer+3,x
    sta temp+1

    lda temp+2
    eor buffer+4,x
    sta temp+2

    tya
    eor buffer+5,x
    tay

    dex
    dex
    dex
    dex
    bpl accum

check:
    lda buffer+1

    bit temp
    beq continue
    
    bit temp+1
    beq continue

    bit temp+2
    beq continue

    tya
    bit buffer+1
    bne end

continue:
    lda temp
    asl
    ora #1
    sta temp

    lda temp+1
    rol
    ora #1
    sta temp+1

    lda temp+2
    rol
    ora #1
    sta temp+2

    tya
    rol
    ora #1
    tay

    jmp round

end:
    lda temp
    sta out
    lda temp+1
    sta out+1
    lda temp+2
    sta out+2
    tya
    sta out+3
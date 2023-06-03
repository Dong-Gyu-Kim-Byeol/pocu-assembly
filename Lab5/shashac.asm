buffer=$C000
out=$2300

temp=$01
count=$00

    ldx #$FF
    txs

    lda #0
    sta temp
    sta temp+1
    sta temp+2
    sta temp+3
    
    lda buffer
    asl
    asl
    sec
    sbc #4
    sta count

round:
    ldx count

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

    lda temp+3
    eor buffer+5,x
    sta temp+3

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

    bit temp+3
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

    lda temp+3
    rol
    ora #1
    sta temp+3

    jmp round

end:
    lda temp
    sta out
    lda temp+1
    sta out+1
    lda temp+2
    sta out+2
    lda temp+3
    sta out+3
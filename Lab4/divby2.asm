num_0 = $4000
num_1 = $4001
num_2 = $4002
num_3 = $4003

out_0 = $2300
out_1 = $2301
out_2 = $2302
out_3 = $2303

    ldx #$FF
    txs

    lda num_3
    rol
    
    lda num_3
    ror
    sta out_3

    lda num_2
    ror
    sta out_2

    lda num_1
    ror
    sta out_1

    lda num_0
    ror
    sta out_0
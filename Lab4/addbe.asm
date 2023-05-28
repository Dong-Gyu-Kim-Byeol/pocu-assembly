num_a_0 = $4000
num_a_1 = $4001
num_a_2 = $4002
num_a_3 = $4003

num_b_0 = $4004
num_b_1 = $4005
num_b_2 = $4006
num_b_3 = $4007

out_0 = $2300
out_1 = $2301
out_2 = $2302
out_3 = $2303

    ldx #$FF
    txs

    lda num_a_3
    clc
    adc num_b_3
    sta out_0

    lda num_a_2
    adc num_b_2
    sta out_1

    lda num_a_1
    adc num_b_1
    sta out_2

    lda num_a_0
    adc num_b_0
    sta out_3
#include "disassembler.h"

#include <assert.h>
#include <string.h>

void test1(void)
{
    const unsigned char mem[] = { 0x18, 0xA9, 0x05, 0x6D, 0x10, 0x20, 0x8D, 0x00, 0x10, 0x02 };
    char buffer[64];

    const unsigned char* next = disassemble(buffer, mem);

    assert(next == mem + 1);
    assert(strcmp("OPCODE=18[clc i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);

    assert(next == mem + 3);
    assert(strcmp("OPCODE=A9[lda #] OPERAND=.. 05", buffer) == 0);

    next = disassemble(buffer, next);

    assert(next == mem + 6);
    assert(strcmp("OPCODE=6D[adc a] OPERAND=20 10", buffer) == 0);

    next = disassemble(buffer, next);

    assert(next == mem + 9);
    assert(strcmp("OPCODE=8D[sta a] OPERAND=10 00", buffer) == 0);

    next = disassemble(buffer, next);

    assert(next == mem + 9);
    assert(strcmp("", buffer) == 0);
}

void homecoder_test(void)
{
    // 모든 명령어를 테스트 합니다. 단, operand 부분은 무작위로 만든 숫자이므로 올바른 테스트 값은 아닐 수도 있습니다.

    enum {
        BUFFER_SIZE = 64
    };

    const unsigned char mem[] = { 0x00, 0x01, 0x29, 0x05, 0x23, 0x06, 0xBE, 0x08, 0x09, 0x84, 0x0A, 0x0D, 0x6C, 0xE1, 0x0E, 0xAE, 0xD6, 0x10, 0x52, 0x11, 0x90, 0x15, 0x49, 0x16, 0xF1, 0x18, 0x19, 0xBB, 0xF1, 0x1D, 0xEB, 0xE9, 0x1E, 0xA6, 0xB3, 0x20, 0x3C, 0xDB,
        0x21, 0x87, 0x24, 0x0C, 0x25, 0x3E, 0x26, 0x99, 0x28, 0x29, 0x24, 0x2A, 0x2C, 0x0D, 0x5E, 0x2D, 0x06, 0x1C, 0x2E, 0x47, 0xB7, 0x30, 0xDE, 0x31, 0xB3, 0x35, 0x12, 0x36, 0x4D, 0x38, 0x39, 0x43, 0xC8, 0x3D, 0x8B, 0xBB, 0x3E, 0x1F, 0xA6, 0x40,
        0x41, 0x03, 0x45, 0x5A, 0x46, 0x7D, 0x48, 0x49, 0x09, 0x4A, 0x4C, 0x25, 0x38, 0x4D, 0x5D, 0x1F, 0x4E, 0xCB, 0xD4, 0x50, 0xFC, 0x51, 0x96, 0x55, 0xF5, 0x56, 0x45, 0x58, 0x59, 0x13, 0x3B, 0x5D, 0x89, 0x0D, 0x5E, 0x1C, 0x0A, 0x60,
        0x61, 0xDB, 0x65, 0xAE, 0x66, 0x32, 0x68, 0x69, 0x20, 0x6A, 0x6C, 0x50, 0x9A, 0x6D, 0x40, 0xEE, 0x6E, 0x36, 0x78, 0x70, 0xFD, 0x71, 0x12, 0x75, 0x49, 0x76, 0x32, 0x78, 0x79, 0x9E, 0xF6, 0x7D, 0x49, 0x7D, 0x7E, 0xAD, 0xDC, 0x81, 0x4F, 0x84, 0x14, 0x85, 0xF2, 0x86, 0x44, 0x88, 0x8A, 0x8C, 0x66, 0x40, 0x8D, 0x6B, 0xD0, 0x8E, 0x30, 0xC4, 0x90, 0xB7, 0x91, 0x32, 0x94, 0x3B, 0x95, 0xA1, 0x96, 0x22, 0x98, 0x99, 0x22, 0xF6, 0x9A, 0x9D, 0x9D, 0x91, 0xA0, 0xE1,
        0xA1, 0x8B, 0xA2, 0x1F, 0xA4, 0xDA, 0xA5, 0xB0, 0xA6, 0xCA, 0xA8, 0xA9, 0x99, 0xAA, 0xAC, 0xB9, 0x02, 0xAD, 0x9D, 0x72, 0xAE, 0x2C, 0x49, 0xB0, 0x80, 0xB1, 0x7E, 0xB4, 0xC5, 0xB5, 0x99, 0xB6, 0xD5, 0xB8, 0xB9, 0x80, 0xE9, 0xBA, 0xBC, 0xEA, 0xB2, 0xBD, 0xCC, 0xC9, 0xBE, 0xBF, 0x53, 0xC0, 0x67,
        0xC1, 0xD6, 0xC4, 0xBF, 0xC5, 0x14, 0xC6, 0xD6, 0xC8, 0xC9, 0x7E, 0xCA, 0xCC, 0xDC, 0x2D, 0xCD, 0x66, 0x8E, 0xCE, 0xEF, 0x83, 0xD0, 0x57, 0xD1, 0x49, 0xD5, 0x61, 0xD6, 0xFF, 0xD8, 0xD9, 0x8F, 0x69, 0xDD, 0xCD, 0x61, 0xDE, 0x1E, 0xD1, 0xE0, 0x9D,
        0xE1, 0x9C, 0xE4, 0x16, 0xE5, 0x72, 0xE6, 0x72, 0xE8, 0xE9, 0xE6, 0xEA, 0xEC, 0xF0, 0x1D, 0xED, 0x4F, 0x84, 0xEE, 0x77, 0x4A, 0xF0, 0x02, 0xF1, 0xD7, 0xF5, 0xE8, 0xF6, 0x39, 0xF8, 0xF9, 0x53, 0x2C, 0xFD, 0xC9, 0xCB, 0xFE, 0x1E, 0x12, 0xFF
    };

    unsigned char buffer[BUFFER_SIZE];

    const unsigned char* next = mem;

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=00[brk i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=01[ora (zp,x)] OPERAND=.. 29", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=05[ora zp] OPERAND=.. 23", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=06[asl zp] OPERAND=.. BE", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=08[php i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=09[ora #] OPERAND=.. 84", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=0A[asl A] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=0D[ora a] OPERAND=E1 6C", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=0E[asl a] OPERAND=D6 AE", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=10[bpl r] OPERAND=.. 52", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=11[ora (zp),y] OPERAND=.. 90", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=15[ora zp,x] OPERAND=.. 49", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=16[asl zp,x] OPERAND=.. F1", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=18[clc i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=19[ora a,y] OPERAND=F1 BB", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=1D[ora a,x] OPERAND=E9 EB", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=1E[asl a,x] OPERAND=B3 A6", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=20[jsr a] OPERAND=DB 3C", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=21[and (zp,x)] OPERAND=.. 87", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=24[bit zp] OPERAND=.. 0C", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=25[and zp] OPERAND=.. 3E", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=26[rol zp] OPERAND=.. 99", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=28[plp i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=29[and #] OPERAND=.. 24", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=2A[rol A] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=2C[bit a] OPERAND=5E 0D", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=2D[and a] OPERAND=1C 06", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=2E[rol a] OPERAND=B7 47", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=30[bmi r] OPERAND=.. DE", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=31[and (zp),y] OPERAND=.. B3", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=35[and zp,x] OPERAND=.. 12", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=36[rol zp,x] OPERAND=.. 4D", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=38[sec i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=39[and a,y] OPERAND=C8 43", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=3D[and a,x] OPERAND=BB 8B", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=3E[rol a,x] OPERAND=A6 1F", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=40[rti i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=41[eor (zp,x)] OPERAND=.. 03", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=45[eor zp] OPERAND=.. 5A", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=46[lsr zp] OPERAND=.. 7D", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=48[pha i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=49[eor #] OPERAND=.. 09", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=4A[lsr A] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=4C[jmp a] OPERAND=38 25", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=4D[eor a] OPERAND=1F 5D", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=4E[lsr a] OPERAND=D4 CB", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=50[bvc r] OPERAND=.. FC", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=51[eor (zp),y] OPERAND=.. 96", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=55[eor zp,x] OPERAND=.. F5", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=56[lsr zp,x] OPERAND=.. 45", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=58[cli i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=59[eor a,y] OPERAND=3B 13", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=5D[eor a,x] OPERAND=0D 89", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=5E[lsr a,x] OPERAND=0A 1C", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=60[rts i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=61[adc (zp,x)] OPERAND=.. DB", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=65[adc zp] OPERAND=.. AE", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=66[ror zp] OPERAND=.. 32", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=68[pla i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=69[adc #] OPERAND=.. 20", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=6A[ror A] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=6C[jmp (a)] OPERAND=9A 50", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=6D[adc a] OPERAND=EE 40", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=6E[ror a] OPERAND=78 36", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=70[bvs r] OPERAND=.. FD", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=71[adc (zp),y] OPERAND=.. 12", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=75[adc zp,x] OPERAND=.. 49", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=76[ror zp,x] OPERAND=.. 32", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=78[sei i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=79[adc a,y] OPERAND=F6 9E", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=7D[adc a,x] OPERAND=7D 49", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=7E[ror a,x] OPERAND=DC AD", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=81[sta (zp,x)] OPERAND=.. 4F", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=84[sty zp] OPERAND=.. 14", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=85[sta zp] OPERAND=.. F2", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=86[stx zp] OPERAND=.. 44", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=88[dey i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=8A[txa i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=8C[sty a] OPERAND=40 66", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=8D[sta a] OPERAND=D0 6B", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=8E[stx a] OPERAND=C4 30", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=90[bcc r] OPERAND=.. B7", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=91[sta (zp),y] OPERAND=.. 32", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=94[sty zp,x] OPERAND=.. 3B", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=95[sta zp,x] OPERAND=.. A1", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=96[stx zp,y] OPERAND=.. 22", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=98[tya i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=99[sta a,y] OPERAND=F6 22", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=9A[txs i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=9D[sta a,x] OPERAND=91 9D", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=A0[ldy #] OPERAND=.. E1", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=A1[lda (zp,x)] OPERAND=.. 8B", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=A2[ldx #] OPERAND=.. 1F", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=A4[ldy zp] OPERAND=.. DA", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=A5[lda zp] OPERAND=.. B0", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=A6[ldx zp] OPERAND=.. CA", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=A8[tay i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=A9[lda #] OPERAND=.. 99", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=AA[tax i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=AC[ldy a] OPERAND=02 B9", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=AD[lda a] OPERAND=72 9D", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=AE[ldx a] OPERAND=49 2C", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=B0[bcs r] OPERAND=.. 80", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=B1[lda (zp),y] OPERAND=.. 7E", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=B4[ldy zp,x] OPERAND=.. C5", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=B5[lda zp,x] OPERAND=.. 99", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=B6[ldx zp,y] OPERAND=.. D5", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=B8[clv i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=B9[lda a,y] OPERAND=E9 80", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=BA[tsx i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=BC[ldy a,x] OPERAND=B2 EA", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=BD[lda a,x] OPERAND=C9 CC", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=BE[ldx a,y] OPERAND=53 BF", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=C0[cpy #] OPERAND=.. 67", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=C1[cmp (zp,x)] OPERAND=.. D6", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=C4[cpy zp] OPERAND=.. BF", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=C5[cmp zp] OPERAND=.. 14", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=C6[dec zp] OPERAND=.. D6", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=C8[iny i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=C9[cmp #] OPERAND=.. 7E", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=CA[dex i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=CC[cpy a] OPERAND=2D DC", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=CD[cmp a] OPERAND=8E 66", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=CE[dec a] OPERAND=83 EF", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=D0[bne r] OPERAND=.. 57", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=D1[cmp (zp),y] OPERAND=.. 49", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=D5[cmp zp,x] OPERAND=.. 61", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=D6[dec zp,x] OPERAND=.. FF", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=D8[cld i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=D9[cmp a,y] OPERAND=69 8F", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=DD[cmp a,x] OPERAND=61 CD", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=DE[dec a,x] OPERAND=D1 1E", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=E0[cpx #] OPERAND=.. 9D", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=E1[sbc (zp,x)] OPERAND=.. 9C", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=E4[cpx zp] OPERAND=.. 16", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=E5[sbc zp] OPERAND=.. 72", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=E6[inc zp] OPERAND=.. 72", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=E8[inx i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=E9[sbc #] OPERAND=.. E6", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=EA[nop i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=EC[cpx a] OPERAND=1D F0", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=ED[sbc a] OPERAND=84 4F", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=EE[inc a] OPERAND=4A 77", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=F0[beq r] OPERAND=.. 02", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=F1[sbc (zp),y] OPERAND=.. D7", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=F5[sbc zp,x] OPERAND=.. E8", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=F6[inc zp,x] OPERAND=.. 39", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=F8[sed i] OPERAND=.. ..", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=F9[sbc a,y] OPERAND=2C 53", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=FD[sbc a,x] OPERAND=CB C9", buffer) == 0);

    next = disassemble(buffer, next);
    assert(strcmp("OPCODE=FE[inc a,x] OPERAND=12 1E", buffer) == 0);
}

int main(void)
{
    test1();

    homecoder_test();

    return 0;
}
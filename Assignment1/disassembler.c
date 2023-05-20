#include "disassembler.h"

#include <assert.h>
#include <stdio.h>

// instruction
const char* const BRK = "brk";
const char* const PHP = "php";
const char* const BPL = "bpl";
const char* const CLC = "clc";
const char* const JSR = "jsr";
const char* const BIT = "bit";
const char* const PLP = "plp";
const char* const BMI = "bmi";
const char* const SEC = "sec";
const char* const RTI = "rti";
const char* const PHA = "pha";
const char* const JMP = "jmp";
const char* const BVC = "bvc";
const char* const CLI = "cli";
const char* const RTS = "rts";
const char* const PLA = "pla";
const char* const BVS = "bvs";
const char* const SEI = "sei";
const char* const STY = "sty";
const char* const DEY = "dey";
const char* const BCC = "bcc";
const char* const TYA = "tya";
const char* const LDY = "ldy";
const char* const TAY = "tay";
const char* const BCS = "bcs";
const char* const CLV = "clv";
const char* const CPY = "cpy";
const char* const INY = "iny";
const char* const BNE = "bne";
const char* const CLD = "cld";
const char* const CPX = "cpx";
const char* const INX = "inx";
const char* const BEQ = "beq";
const char* const SED = "sed";
const char* const ORA = "ora";
const char* const AND = "and";
const char* const EOR = "eor";
const char* const ADC = "adc";
const char* const STA = "sta";
const char* const LDA = "lda";
const char* const ASL = "asl";
const char* const ROL = "rol";
const char* const LSR = "lsr";
const char* const ROR = "ror";
const char* const STX = "stx";
const char* const TXA = "txa";
const char* const TAX = "tax";
const char* const LDX = "ldx";
const char* const TXS = "txs";
const char* const TSX = "tsx";
const char* const CMP = "cmp";
const char* const DEC = "dec";
const char* const DEX = "dex";
const char* const SBC = "sbc";
const char* const INC = "inc";
const char* const NOP = "nop";

// mode
const char* const ACCUMULATOR = "A";                            // 누산기(accumulator, A 레지스터)
const char* const IMMEDIATE = "#";                              // 즉시(immediate)
const char* const ABSOLUTE = "a";                               // 절대(absolute) 주소
const char* const ZERO_PAGE = "zp";                             // 제로 페이지(zero page) 주소
const char* const IMPLIED = "i";                                // 묵시적(implied)
const char* const RELATIVE = "r";                               // 상대(relative) 주소
const char* const ABSOLUTE_INDIRECT = "(a)";                    // 절대 간접(absolute indirect) 주소
const char* const ABSOLUTE_INDEXED_WITH_X = "a,x";              // 절대 주소 + x
const char* const ABSOLUTE_INDEXED_WITH_Y = "a,y";              // 절대 주소 + y
const char* const ZERO_PAGE_INDEXED_WITH_X = "zp,x";            // 제로 페이지 주소 + x
const char* const ZERO_PAGE_INDEXED_WITH_Y = "zp,y";            // 제로 페이지 주소 + y
const char* const ZERO_PAGE_INDEXED_INDIRECT = "(zp,x)";        // 색인 후 간접 참조
const char* const ZERO_PAGE_INDIRECT_INDEXED_WITH_Y = "(zp),y"; // 간접 참조 후 색인

typedef enum opcode_mode {
    OPCODE_MODE_UNSPECIFIED,
    OPCODE_MODE_ACCUMULATOR,                            // 누산기(accumulator, A 레지스터)
    OPCODE_MODE_IMMEDIATE,                              // 즉시(immediate)
    OPCODE_MODE_ABSOLUTE,                               // 절대(absolute) 주소
    OPCODE_MODE_ZERO_PAGE,                              // 제로 페이지(zero page) 주소
    OPCODE_MODE_IMPLIED,                                // 묵시적(implied)
    OPCODE_MODE_RELATIVE,                               // 상대(relative) 주소
    OPCODE_MODE_ABSOLUTE_INDIRECT,                      // 절대 간접(absolute indirect) 주소
    OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X,                // 절대 주소 + x
    OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y,                // 절대 주소 + y
    OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X,               // 제로 페이지 주소 + x
    OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_Y,               // 제로 페이지 주소 + y
    OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT,             // 색인 후 간접 참조
    OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y       // 간접 참조 후 색인
} opcode_mode_t;

typedef struct opcode_info {
    char* opcode_name;
    char* mode_name;
    opcode_mode_t mode;
} opcode_info_t;

void ptint_disassemble(char* out_buffer64, const unsigned char opcode, const opcode_info_t opcode_info, const unsigned char* const operand1_or_null, const unsigned char* const operand2_or_null);

const unsigned char* disassemble(char* out_buffer64, const unsigned char* mem)
{
    assert(out_buffer64 != NULL);
    assert(mem != NULL);

    const opcode_info_t OPCODE_TABLE[16][16] = {
        { { BRK, IMPLIED, OPCODE_MODE_IMPLIED }, { ORA, ZERO_PAGE_INDEXED_INDIRECT, OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { ORA, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { ASL, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { PHP, IMPLIED, OPCODE_MODE_IMPLIED }, { ORA, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { ASL, ACCUMULATOR, OPCODE_MODE_ACCUMULATOR }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { ORA, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { ASL, ABSOLUTE, OPCODE_MODE_ABSOLUTE } },
        { { BPL, RELATIVE, OPCODE_MODE_RELATIVE }, { ORA, ZERO_PAGE_INDIRECT_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { ORA, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { ASL, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CLC, IMPLIED, OPCODE_MODE_IMPLIED }, { ORA, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { ORA, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { ASL, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X } },
        { { JSR, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { AND, ZERO_PAGE_INDEXED_INDIRECT, OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, {BIT, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { AND, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { ROL, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { PLP, IMPLIED, OPCODE_MODE_IMPLIED }, { AND, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { ROL, ACCUMULATOR, OPCODE_MODE_ACCUMULATOR }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { BIT, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { AND, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { ROL, ABSOLUTE, OPCODE_MODE_ABSOLUTE } },
        { { BMI, RELATIVE, OPCODE_MODE_RELATIVE }, { AND, ZERO_PAGE_INDIRECT_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { AND, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { ROL, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { SEC, IMPLIED, OPCODE_MODE_IMPLIED }, { AND, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { AND, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { ROL, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X } },
        { { RTI, IMPLIED, OPCODE_MODE_IMPLIED }, { EOR, ZERO_PAGE_INDEXED_INDIRECT, OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { EOR, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { LSR, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { PHA, IMPLIED, OPCODE_MODE_IMPLIED }, { EOR, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { LSR, ACCUMULATOR, OPCODE_MODE_ACCUMULATOR }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { JMP, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { EOR, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { LSR, ABSOLUTE, OPCODE_MODE_ABSOLUTE } },
        { { BVC, RELATIVE, OPCODE_MODE_RELATIVE }, { EOR, ZERO_PAGE_INDIRECT_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { EOR, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { LSR, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CLI, IMPLIED, OPCODE_MODE_IMPLIED }, { EOR, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { EOR, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { LSR, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X } },
        { { RTS, IMPLIED, OPCODE_MODE_IMPLIED }, { ADC, ZERO_PAGE_INDEXED_INDIRECT, OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { ADC, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { ROR, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { PLA, IMPLIED, OPCODE_MODE_IMPLIED }, { ADC, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { ROR, ACCUMULATOR, OPCODE_MODE_ACCUMULATOR }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { JMP, ABSOLUTE_INDIRECT, OPCODE_MODE_ABSOLUTE_INDIRECT }, { ADC, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { ROR, ABSOLUTE, OPCODE_MODE_ABSOLUTE } },
        { { BVS, RELATIVE, OPCODE_MODE_RELATIVE }, { ADC, ZERO_PAGE_INDIRECT_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { ADC, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { ROR, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { SEI, IMPLIED, OPCODE_MODE_IMPLIED }, { ADC, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { ADC, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { ROR, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X } },
        { { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { STA, ZERO_PAGE_INDEXED_INDIRECT, OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { STY, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { STA, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { STX, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { DEY, IMPLIED, OPCODE_MODE_IMPLIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { TXA, IMPLIED, OPCODE_MODE_IMPLIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { STY, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { STA, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { STX, ABSOLUTE, OPCODE_MODE_ABSOLUTE } },
        { { BCC, RELATIVE, OPCODE_MODE_RELATIVE }, { STA, ZERO_PAGE_INDIRECT_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { STY, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { STA, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { STX, ZERO_PAGE_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { TYA, IMPLIED, OPCODE_MODE_IMPLIED }, { STA, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y }, { TXS, IMPLIED, OPCODE_MODE_IMPLIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { STA, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED } },
        { { LDY, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { LDA, ZERO_PAGE_INDEXED_INDIRECT, OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT }, {LDX, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { LDY, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { LDA, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { LDX, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { TAY, IMPLIED, OPCODE_MODE_IMPLIED }, { LDA, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { TAX, IMPLIED, OPCODE_MODE_IMPLIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { LDY, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { LDA, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { LDX, ABSOLUTE, OPCODE_MODE_ABSOLUTE } },
        { { BCS, RELATIVE, OPCODE_MODE_RELATIVE }, { LDA, ZERO_PAGE_INDIRECT_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { LDY, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { LDA, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { LDX, ZERO_PAGE_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CLV, IMPLIED, OPCODE_MODE_IMPLIED }, { LDA, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y }, { TSX, IMPLIED, OPCODE_MODE_IMPLIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { LDY, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { LDA, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { LDX, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y } },
        { { CPY, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { CMP, ZERO_PAGE_INDEXED_INDIRECT, OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CPY, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { CMP, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { DEC, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { INY, IMPLIED, OPCODE_MODE_IMPLIED }, { CMP, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { DEX, IMPLIED, OPCODE_MODE_IMPLIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CPY, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { CMP, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { DEC, ABSOLUTE, OPCODE_MODE_ABSOLUTE } },
        { { BNE, RELATIVE, OPCODE_MODE_RELATIVE }, { CMP, ZERO_PAGE_INDIRECT_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CMP, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { DEC, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CLD, IMPLIED, OPCODE_MODE_IMPLIED }, { CMP, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CMP, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { DEC, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X } },
        { { CPX, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { SBC, ZERO_PAGE_INDEXED_INDIRECT, OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CPX, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { SBC, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { INC, ZERO_PAGE, OPCODE_MODE_ZERO_PAGE }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { INX, IMPLIED, OPCODE_MODE_IMPLIED }, { SBC, IMMEDIATE, OPCODE_MODE_IMMEDIATE }, { NOP, IMPLIED, OPCODE_MODE_IMPLIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { CPX, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { SBC, ABSOLUTE, OPCODE_MODE_ABSOLUTE }, { INC, ABSOLUTE, OPCODE_MODE_ABSOLUTE } },
        { { BEQ, RELATIVE, OPCODE_MODE_RELATIVE }, { SBC, ZERO_PAGE_INDIRECT_INDEXED_WITH_Y, OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { SBC, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { INC, ZERO_PAGE_INDEXED_WITH_X, OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { SED, IMPLIED, OPCODE_MODE_IMPLIED }, { SBC, ABSOLUTE_INDEXED_WITH_Y, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { NULL, NULL, OPCODE_MODE_UNSPECIFIED }, { SBC, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X }, { INC, ABSOLUTE_INDEXED_WITH_X, OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X } }
    };

    const unsigned char HIGH_NIBBLE_MASK = 0b11110000;
    const unsigned char LOW_NIBBLE_MASK = 0b00001111;

    const unsigned char* p_next_mem = mem;
    const unsigned char opcode = *p_next_mem;
    p_next_mem++;

    const unsigned char high_nibble = (opcode & HIGH_NIBBLE_MASK) >> 4;
    const unsigned char low_nibble = opcode & LOW_NIBBLE_MASK;

    const opcode_info_t opcode_info = OPCODE_TABLE[high_nibble][low_nibble];

    switch (opcode_info.mode) {
    case OPCODE_MODE_ACCUMULATOR:
        /* intentional fallthrough */
    case OPCODE_MODE_IMPLIED: {
        ptint_disassemble(out_buffer64, opcode, opcode_info, NULL, NULL);
        break;
    }
    case OPCODE_MODE_IMMEDIATE:
        /* intentional fallthrough */
    case OPCODE_MODE_ZERO_PAGE:
        /* intentional fallthrough */
    case OPCODE_MODE_RELATIVE:
        /* intentional fallthrough */
    case OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_X:
        /* intentional fallthrough */
    case OPCODE_MODE_ZERO_PAGE_INDEXED_WITH_Y:
        /* intentional fallthrough */
    case OPCODE_MODE_ZERO_PAGE_INDEXED_INDIRECT:
        /* intentional fallthrough */
    case OPCODE_MODE_ZERO_PAGE_INDIRECT_INDEXED_WITH_Y: {
        const unsigned char* const p_operand2 = p_next_mem;
        p_next_mem++;
        ptint_disassemble(out_buffer64, opcode, opcode_info, NULL, p_operand2);
        break;
    }
    case OPCODE_MODE_ABSOLUTE:
        /* intentional fallthrough */
    case OPCODE_MODE_ABSOLUTE_INDIRECT:
        /* intentional fallthrough */
    case OPCODE_MODE_ABSOLUTE_INDEXED_WITH_X:
        /* intentional fallthrough */
    case OPCODE_MODE_ABSOLUTE_INDEXED_WITH_Y: {
        const unsigned char* const p_operand2 = p_next_mem;
        p_next_mem++;
        const unsigned char* const p_operand1 = p_next_mem;
        p_next_mem++;
        ptint_disassemble(out_buffer64, opcode, opcode_info, p_operand1, p_operand2);
        break;
    }
    case OPCODE_MODE_UNSPECIFIED: {
        *out_buffer64 = '\0';
        p_next_mem = mem;
        break;
    }
    default:
        assert(0);
        break;
    }

    return p_next_mem;
}

// out_buffer64
// OPCODE=<옵코드>[<니모닉> <주소지정 모드 표기>] OPERAND=<상위 바이트> <하위 바이트>
// 상위 바이트나 하위 바이트가 없다면 그 대신 ..를 사용합니다.
void ptint_disassemble(char* out_buffer64, const unsigned char opcode, const opcode_info_t opcode_info, const unsigned char* const operand1_or_null, const unsigned char* const operand2_or_null)
{
    assert(out_buffer64 != NULL);

    const char* const NONE_OPERAND_STRING = "..";

    if (operand1_or_null == NULL && operand2_or_null == NULL) {
        sprintf(out_buffer64, "OPCODE=%02X[%s %s] OPERAND=%s %s", opcode, opcode_info.opcode_name, opcode_info.mode_name, NONE_OPERAND_STRING, NONE_OPERAND_STRING);
    } else if (operand1_or_null != NULL && operand2_or_null == NULL) {
        sprintf(out_buffer64, "OPCODE=%02X[%s %s] OPERAND=%02X %s", opcode, opcode_info.opcode_name, opcode_info.mode_name, *operand1_or_null, NONE_OPERAND_STRING);
    } else if (operand1_or_null == NULL && operand2_or_null != NULL) {
        sprintf(out_buffer64, "OPCODE=%02X[%s %s] OPERAND=%s %02X", opcode, opcode_info.opcode_name, opcode_info.mode_name, NONE_OPERAND_STRING, *operand2_or_null);
    } else {
        assert(operand1_or_null != NULL);
        assert(operand2_or_null != NULL);
        sprintf(out_buffer64, "OPCODE=%02X[%s %s] OPERAND=%02X %02X", opcode, opcode_info.opcode_name, opcode_info.mode_name, *operand1_or_null, *operand2_or_null);
    }
}
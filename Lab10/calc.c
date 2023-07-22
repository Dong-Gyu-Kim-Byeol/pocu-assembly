#include "calc.h"

#include <assert.h>
#include <math.h>

#include "eprpc.h"

#define TRUE ((char)0xFF)
#define FALSE (0)

void parse_rpn(const char* line, op_t* ops, double* operands, const size_t count)
{
    // ' ' // 구분
    // '-' // 부호
    // '.' // 가수부 '.' 이하
    // 'e' // 지수부 // e+ // e-

    // 연산
    // '+'
    // '-'
    // '*'
    // '/'

    char* p_line = line;
    op_t* p_ops = ops;
    double* p_operands = operands;

    char is_new = TRUE;
    char is_num = TRUE;
    char is_mantissa_minus = FALSE;
    char is_mantissa_dot = FALSE;
    char is_exponent = FALSE;
    char is_exponent_minus = FALSE;
    double mantissa_dot_h = 0;
    double mantissa_dot_l = 0;
    double mantissa_dot_l_mul = 0.1;
    double exponent = 0;

    while (TRUE) {
        char c = *p_line++;

        if (c == ' ' || c == NULL) {
            if (is_num) {
                mantissa_dot_h += mantissa_dot_l;

                if (is_mantissa_minus) {
                    mantissa_dot_h *= -1;
                }

                if (is_exponent) {
                    if (is_exponent_minus) {
                        mantissa_dot_h *= powl(10, -1 * exponent);
                    } else {
                        mantissa_dot_h *= powl(10, exponent);
                    }
                }

                *p_operands++ = mantissa_dot_h;
                *p_ops++ = OP_LOAD;
            } else {
                *p_operands++ = 0;
            }
        }

        if (c == NULL) {
            break;
        }

        switch (c) {
        case ' ':
            is_new = TRUE;
            is_num = TRUE;
            is_mantissa_minus = FALSE;
            is_mantissa_dot = FALSE;
            is_exponent = FALSE;
            is_exponent_minus = FALSE;
            mantissa_dot_h = 0;
            mantissa_dot_l = 0;
            mantissa_dot_l_mul = 0.1;
            exponent = 0;
            break;

        case '-':
            if (is_new) {
                is_new = FALSE;

                if (*p_line == NULL || *p_line == ' ') {
                    *p_ops++ = OP_SUB;
                    is_num = FALSE;
                    break;
                }

                is_mantissa_minus = TRUE;
                break;
            }

            if (is_exponent) {
                is_exponent_minus = TRUE;
                break;
            }

            assert(0);
            break;

        case '.':
            is_mantissa_dot = TRUE;
            break;

        case 'e':
            is_exponent = TRUE;
            is_mantissa_dot = FALSE;
            break;

        case '+':
            if (is_new) {
                is_new = FALSE;
                *p_ops++ = OP_ADD;
                is_num = FALSE;
                break;
            }
            /*if (is_exponent) {
                is_exponent_minus = FALSE;
            }*/
            break;

        case '*':
            is_new = FALSE;
            *p_ops++ = OP_MUL;
            is_num = FALSE;
            break;

        case '/':
            is_new = FALSE;
            *p_ops++ = OP_DIV;
            is_num = FALSE;
            break;

        default:
            assert('0' <= c);
            assert(c <= '9');

            is_new = FALSE;

            if (!is_exponent && !is_mantissa_dot) {
                mantissa_dot_h *= 10;
                mantissa_dot_h += c - '0';
                break;
            }
            if (!is_exponent && is_mantissa_dot) {
                mantissa_dot_l += (c - '0') * mantissa_dot_l_mul;
                mantissa_dot_l_mul /= 10;
                break;
            }

            assert(is_exponent == TRUE);

            exponent *= 10;
            exponent += c - '0';
            break;
        }
    }

    *p_ops++ = OP_END;
}

void calculate(const op_t* op, const double* operands)
{
    op_t* p_op = op;
    double* p_operands = operands;

    eprpc_init();

    while (*p_op != OP_END) {
        switch (*p_op) {
        case OP_LOAD:
            eprpc_load(*p_operands);
            break;

        case OP_ADD:
            eprpc_add();
            break;

        case OP_SUB:
            eprpc_sub();
            break;

        case OP_MUL:
            eprpc_mul();
            break;

        case OP_DIV:
            eprpc_div();
            break;

        case OP_END:
            break;

        default:
            assert(0);
            break;
        }

        ++p_op;
        ++p_operands;
    }
}

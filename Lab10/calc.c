#include "calc.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "eprpc.h"

#define TRUE ((char)0xFF)
#define FALSE (0)

void parse_rpn(const char* line, op_t* ops, double* operands, const size_t count)
{
    const char* p_line = line;
    const char* p_word = line;
    op_t* p_ops = ops;
    double* p_operands = operands;

    char is_new = TRUE;

    while (TRUE) {
        const char c = *p_line++;

        if (c == ' ' || c == '\0') {
            double num = 0;
            const int result = sscanf(p_word, "%lf", &num);

            if (result == 1) {
                *p_operands++ = num;
                *p_ops++ = OP_LOAD;
            } else {
                *p_operands++ = 0;
            }

            p_word = p_line;
            is_new = TRUE;
        }

        if (c == '\0') {
            break;
        }

        switch (c) {
        case ' ':
            break;

        case '-':
            if (is_new && (*p_line == '\0' || *p_line == ' ')) {
                is_new = FALSE;
                *p_ops++ = OP_SUB;
                break;
            }
            break;

        case '+':
            if (is_new && (*p_line == '\0' || *p_line == ' ')) {
                is_new = FALSE;
                *p_ops++ = OP_ADD;
                break;
            }
            break;

        case '*':
            if (is_new && (*p_line == '\0' || *p_line == ' ')) {
                is_new = FALSE;
                *p_ops++ = OP_MUL;
                break;
            }
            break;

        case '/':
            if (is_new && (*p_line == '\0' || *p_line == ' ')) {
                is_new = FALSE;
                *p_ops++ = OP_DIV;
                break;
            }
            break;

        default:
            is_new = FALSE;
            break;
        }
    }

    *p_ops++ = OP_END;
}

void calculate(const op_t* op, const double* operands)
{
    const op_t* p_op = op;
    const double* p_operands = operands;

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

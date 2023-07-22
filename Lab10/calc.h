/***************************************************************************/
/* WARNING: DO NOT CHANGE. This file will be overwritten by Buildbot       */
/***************************************************************************/

#ifndef CALC_H
#define CALC_H

#include <stddef.h>

typedef enum op {
    /* add an operand to calculator */
    OP_LOAD,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    /* signals the end of operation buffer */
    OP_END
} op_t;

void parse_rpn(const char* line, op_t* ops, double* operands, const size_t count);
void calculate(const op_t* op, const double* operands);

#endif /* CALC_H */
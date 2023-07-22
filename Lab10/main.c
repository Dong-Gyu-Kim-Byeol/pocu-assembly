#include <assert.h>
#include <math.h>

#include "calc.h"
#include "eprpc.h"

enum { NUM_DATA = 16 };

//#define EPSILON32 (1.401298E-45)
#define EPSILON64 (4.94065645841247E-324)

int main(void)
{
    {
        double result;
        double cmp_abs;

        eprpc_init();
        eprpc_load(2.2);
        eprpc_load(3.2);

        eprpc_add();

        eprpc_get_result(&result); /* result: 5.4 */
        cmp_abs = fabs(5.4 - result);
        assert(cmp_abs <= EPSILON64);
    }

    {
        double result;
        double cmp_abs;
        double operands[NUM_DATA];
        op_t ops[NUM_DATA];

        parse_rpn("1.2 3.4 5.2 + * 3.4 / -2.7 +", ops, operands, NUM_DATA);

        calculate(ops, operands);
        eprpc_get_result(&result);

        cmp_abs = abs(0.33529411764705882352941176470588 - result);
        assert(cmp_abs <= EPSILON64);

        parse_rpn("1.2 3.4 5.2 + * 3.4 / 2.7 +", ops, operands, NUM_DATA);

        calculate(ops, operands);
        eprpc_get_result(&result);

        assert(5.7352941176470589 == result);

        eprpc_get_result(&result);

        assert(5.7352941176470589 == result);

        parse_rpn("2.34e-4 4.523e-3 3.2 0.002 * + /", ops, operands, NUM_DATA);

        calculate(ops, operands);
        eprpc_get_result(&result);

        assert(0.021422686075254049 == result);

        parse_rpn("3.2 5.7 8.1 9.0 - + 1.1 / *", ops, operands, NUM_DATA);

        calculate(ops, operands);
        eprpc_get_result(&result);

        assert(13.963636363636363 == result);
    }

    return 0;
}
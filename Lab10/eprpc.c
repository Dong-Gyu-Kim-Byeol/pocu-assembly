#include "eprpc.h"

void __declspec(naked) eprpc_init(void)
{
    __asm {
        finit
        fwait
        ret
    }
}

void eprpc_load(const double num)
{
    __asm {
        fld num
        fwait
    }
}

void eprpc_get_result(const double* out_result)
{
    double result;

    __asm {
        fst result
        fwait

        lea ecx, result
        mov edx, [ecx]
        mov eax, dword ptr[ecx + 4]

        mov ecx, out_result
        mov dword ptr[ecx], edx
        mov dword ptr[ecx + 4], eax
    }
}

void __declspec(naked) eprpc_add(void)
{
    __asm {
        fadd
        fwait
        ret
    }
}
void __declspec(naked) eprpc_sub(void)
{
    __asm {
        fsub
        fwait
        ret
    }
}
void __declspec(naked) eprpc_mul(void)
{
    __asm {
        fmul
        fwait
        ret
    }
}
void __declspec(naked) eprpc_div(void)
{
    __asm {
        fdiv
        fwait
        ret
    }
}
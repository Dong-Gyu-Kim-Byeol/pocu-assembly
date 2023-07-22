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

void eprpc_get_result(double* out_result)
{
    __asm {
        sub esp, 8

        fst qword ptr[esp]
        fwait

        mov edx, dword ptr[esp]
        mov eax, dword ptr[esp + 4]

        mov ecx, out_result
        mov dword ptr[ecx], edx
        mov dword ptr[ecx + 4], eax

        add esp, 8
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
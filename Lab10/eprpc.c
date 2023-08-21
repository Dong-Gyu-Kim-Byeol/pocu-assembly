#include "eprpc.h"

void __declspec(naked) eprpc_init(void)
{
    __asm {
        finit
        fwait
        ret
    }
}

// 인자가 있는 함수에는 __declspec(naked)를 적용하지 말 것
void eprpc_load(const double num)
{
    __asm {
        fld num
        fwait
    }
}

// 인자가 있는 함수에는 __declspec(naked)를 적용하지 말 것
void eprpc_get_result(double* out_result)
{
    __asm {
        /*
        sub esp, 8

        fst qword ptr[esp]
        fwait

        mov edx, dword ptr[esp]
        mov eax, dword ptr[esp + 4]

        mov ecx, out_result
        mov dword ptr[ecx], edx
        mov dword ptr[ecx + 4], eax

        add esp, 8
        */

        mov eax, out_result
        fst qword ptr[eax]
        fwait
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
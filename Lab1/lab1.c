#include "lab1.h"

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    __asm
    {
        mov eax, dword ptr[ebp + 8]
        mov ebx, dword ptr[ebp + 0Ch]
        sub eax, ebx
    }
}
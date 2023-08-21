void transpose(mat4_t* mat)
{
    __asm {
        mov             eax, mat

        vmovups         ymm2, [eax];                    ymm2 = { x0, y0, z0, w0, x1, y1, z1, w1 }
        vmovups         ymm3, [eax + 32];               ymm3 = { x2, y2, z2, w2, x3, y3, z3, w3 }
        vperm2i128      ymm0, ymm2, ymm3, 00100000b;    ymm0 = { x0, y0, z0, w0, x2, y2, z2, w2 }
        vperm2i128      ymm1, ymm2, ymm3, 00110001b;    ymm1 = { x1, y1, z1, w1, x3, y3, z3, w3 }

        ; shuffle to each ymm contain the x,z or y,w elements of every row
        vshufps         ymm2, ymm0, ymm1, 10001000b;    ymm2 = { x0, z0, x1, z1, x2, z2, x3, z3 }
        vshufps         ymm3, ymm0, ymm1, 11011101b;    ymm3 = { y0, w0, y1, w1, y2, w2, y3, w3 }

        vinsertf128     ymm4, ymm2, xmm3, 1;            ymm4 = { x0, z0, x1, z1, y0, w0, y1, w1 }
        vperm2f128      ymm5, ymm2, ymm3, 00110001b;    ymm5 = { x2, z2, x3, z3, y2, w2, y3, w3 }

        vshufps         ymm6, ymm4, ymm5, 10001000b;    ymm6 = { x0, x1, x2, x3, y0, y1, y2, y3 }
        vshufps         ymm7, ymm4, ymm5, 11011101b;    ymm7 = { z0, z1, z2, z3, w0, w1, w2, w3 }

        vmovups         [eax], ymm6
        vmovups         [eax + 32], ymm7
    }
}
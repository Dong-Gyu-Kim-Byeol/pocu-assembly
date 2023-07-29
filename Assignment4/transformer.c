#include "transformer.h"

#include <stddef.h>
#include <stdio.h>

#include "matrix.h"

size_t read_points(vec4_t* points, const size_t count)
{
    size_t index = 0;
    vec4_t temp = { 0.f, 0.f, 0.f, 1.f };
    int ret;

    while (index < count) {
        ret = scanf("%f %f %f", &temp.x, &temp.y, &temp.z);
        if (ret != 3) {
            break;
        }

        points[index++] = temp;
    }

    return index;
}

void print_csv(const vec4_t* points, const size_t count)
{
    const size_t last_i = count - 1;

    for (size_t i = 0; i < count; ++i) {
        printf("%.4f, %.4f, %.4f\n", points[i].x, points[i].y, points[i].z);
    }
}

void transpose(mat4_t* mat)
{
    __asm {
        mov eax, mat;

        movaps xmm0, dword ptr[eax];
        movaps xmm1, dword ptr[eax + 16];
        movaps xmm2, dword ptr[eax + 32];
        movaps xmm3, dword ptr[eax + 48];

        movaps xmm4, xmm0;
        shufps xmm4, xmm1, 01000100b;
        shufps xmm0, xmm1, 11101110b;

        movaps xmm5, xmm2;
        shufps xmm5, xmm3, 01000100b;
        shufps xmm2, xmm3, 11101110b;

        movaps xmm1, xmm4;
        shufps xmm1, xmm5, 10001000b;
        movaps dword ptr[eax], xmm1;

        shufps xmm4, xmm5, 11011101b;
        movaps dword ptr[eax + 16], xmm4;

        movaps xmm1, xmm0;
        shufps xmm1, xmm2, 10001000b;
        movaps dword ptr[eax + 32], xmm1;

        shufps xmm0, xmm2, 11011101b;
        movaps dword ptr[eax + 48], xmm0;
    }
}

void transform(vec4_t* dst, const vec4_t* src, const mat4_t* mat_tr)
{
    __asm {
        mov eax, src;
        mov edx, mat_tr;
        movaps xmm0, dword ptr[eax];

        xorps xmm6, xmm6;

        movaps xmm7, xmm0;
        dpps xmm7, dword ptr[edx], 11110001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm0;
        dpps xmm7, dword ptr[edx + 16], 11110001b;
        shufps xmm7, xmm7, 01010001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm0;
        dpps xmm7, dword ptr[edx + 32], 11110001b;
        shufps xmm7, xmm7, 01000101b;
        addps xmm6, xmm7;

        movaps xmm7, xmm0;
        dpps xmm7, dword ptr[edx + 48], 11110001b;
        shufps xmm7, xmm7, 00010101b;
        addps xmm6, xmm7;

        mov eax, dst;
        movaps dword ptr[eax], xmm6;
    }
}

void concatenate(mat4_t* dst, const mat4_t* m0, const mat4_t* m1_tr)
{
    __asm {
        mov eax, m0;
        mov edx, m1_tr;
        mov edi, dst;

        movaps xmm0, dword ptr[edx];
        movaps xmm1, dword ptr[edx + 16];
        movaps xmm2, dword ptr[edx + 32];
        movaps xmm3, dword ptr[edx + 48];

        // 1
        movaps xmm4, dword ptr[eax];
        xorps xmm6, xmm6;

        movaps xmm7, xmm4;
        dpps xmm7, xmm0, 11110001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm1, 11110001b;
        shufps xmm7, xmm7, 01010001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm2, 11110001b;
        shufps xmm7, xmm7, 01000101b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm3, 11110001b;
        shufps xmm7, xmm7, 00010101b;
        addps xmm6, xmm7;

        movaps dword ptr[edi], xmm6;

        // 2
        movaps xmm4, dword ptr[eax + 16];
        xorps xmm6, xmm6;

        movaps xmm7, xmm4;
        dpps xmm7, xmm0, 11110001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm1, 11110001b;
        shufps xmm7, xmm7, 01010001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm2, 11110001b;
        shufps xmm7, xmm7, 01000101b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm3, 11110001b;
        shufps xmm7, xmm7, 00010101b;
        addps xmm6, xmm7;

        movaps dword ptr[edi + 16], xmm6;

        // 3
        movaps xmm4, dword ptr[eax + 32];
        xorps xmm6, xmm6;

        movaps xmm7, xmm4;
        dpps xmm7, xmm0, 11110001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm1, 11110001b;
        shufps xmm7, xmm7, 01010001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm2, 11110001b;
        shufps xmm7, xmm7, 01000101b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm3, 11110001b;
        shufps xmm7, xmm7, 00010101b;
        addps xmm6, xmm7;

        movaps dword ptr[edi + 32], xmm6;

        // 4
        movaps xmm4, dword ptr[eax + 48];
        xorps xmm6, xmm6;

        movaps xmm7, xmm4;
        dpps xmm7, xmm0, 11110001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm1, 11110001b;
        shufps xmm7, xmm7, 01010001b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm2, 11110001b;
        shufps xmm7, xmm7, 01000101b;
        addps xmm6, xmm7;

        movaps xmm7, xmm4;
        dpps xmm7, xmm3, 11110001b;
        shufps xmm7, xmm7, 00010101b;
        addps xmm6, xmm7;

        movaps dword ptr[edi + 48], xmm6;
    }
}

void run(vec4_t* world_pts, const vec4_t* local_pts, const size_t count, const vec3_t* scale, const vec3_t* rotation, const vec3_t* translation)
{
    mat4_t world_mat_tr;
    {
        mat4_t scratch_mat;

        mat4_t scale_mat;
        mat_scale(&scale_mat, scale->x, scale->y, scale->z);

        mat4_t rotation_x_mat_tr;
        mat_rotation_x(&rotation_x_mat_tr, rotation->x);
        transpose(&rotation_x_mat_tr);
        mat4_t rotation_y_mat_tr;
        mat_rotation_y(&rotation_y_mat_tr, rotation->y);
        transpose(&rotation_y_mat_tr);
        mat4_t rotation_z_mat_tr;
        mat_rotation_z(&rotation_z_mat_tr, rotation->z);
        transpose(&rotation_z_mat_tr);

        mat4_t translation_mat_tr;
        mat_translation(&translation_mat_tr, translation->x, translation->y, translation->z);
        transpose(&translation_mat_tr);

        // world_mat = scale_mat * rotation_x_mat * rotation_y_mat * rotation_z_mat * translation_mat
        concatenate(&world_mat_tr, &scale_mat, &rotation_x_mat_tr);

        scratch_mat = world_mat_tr;
        concatenate(&world_mat_tr, &scratch_mat, &rotation_y_mat_tr);

        scratch_mat = world_mat_tr;
        concatenate(&world_mat_tr, &scratch_mat, &rotation_z_mat_tr);

        scratch_mat = world_mat_tr;
        concatenate(&world_mat_tr, &scratch_mat, &translation_mat_tr);

        transpose(&world_mat_tr);
    }

    for (size_t i = 0; i < count; ++i) {
        transform(&world_pts[i], &local_pts[i], &world_mat_tr);
    }
}

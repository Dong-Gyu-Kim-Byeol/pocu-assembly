#include "transformer.h"
#include "matrix.h"

#include <stddef.h>
#include <stdio.h>
#define TRUE 1
size_t read_points(vec4_t* points, const size_t count)
{
    int result = 1;
    size_t red_point = 0;
    while (TRUE) {
        result = scanf_s("%f %f %f", &points->x, &points->y, &points->z);
        if (result <= 0) {
            break;
        }

        points->w = 1.0f;
        ++points;
        ++red_point;
        if (red_point == count) {
            break;
        }
    }
    return red_point;
}

void print_csv(const vec4_t* points, const size_t count)
{
    size_t current = 0;
    const vec4_t* curr_pt = points;
    while (current < count) {
        printf_s("%9.4f,%9.4f,%9.4f\n", curr_pt->x, curr_pt->y, curr_pt->z);
        ++curr_pt;
        ++current;
    }
}

void transpose(mat4_t* mat)
{
    __asm {
        mov eax, mat;
        vmovaps xmm0, [eax];
        vmovaps xmm1, [eax + 16];
        vmovaps xmm2, [eax + 32];
        vmovaps xmm3, [eax + 48];
        vmovaps xmm4, xmm0;
        shufps xmm4, xmm1, 0x44;
        shufps xmm0, xmm1, 0xEE;
        vmovaps xmm5, xmm2;
        shufps xmm5, xmm3, 0x44;
        shufps xmm2, xmm3, 0xEE;
        vmovaps xmm1, xmm4;
        shufps xmm1, xmm5, 0x88;
        vmovaps[eax], xmm1;
        shufps xmm4, xmm5, 0xDD;
        vmovaps[eax + 16], xmm4;
        vmovaps xmm1, xmm0;
        shufps xmm1, xmm2, 0x88;
        vmovaps[eax + 32], xmm1;
        shufps xmm0, xmm2, 0xDD;
        vmovaps[eax + 48], xmm0;
    }
}

void transform(vec4_t* dst, const vec4_t* src, const mat4_t* mat_tr)
{
    __asm {
        mov eax, src
        mov ebx, dst
        mov edx, mat_tr

        vmovaps xmm0, [eax];
        vmovaps xmm1, xmm0;
        vmovaps xmm2, xmm0;
        vmovaps xmm3, xmm0;

        vmovaps xmm4, [edx];
        vmovaps xmm5, [edx + 16];
        vmovaps xmm6, [edx + 32];
        vmovaps xmm7, [edx + 48];

        dpps xmm0, xmm4, 11110001b;
        dpps xmm1, xmm5, 11110001b;
        dpps xmm2, xmm6, 11110001b;
        dpps xmm3, xmm7, 11110001b;
        vunpcklps xmm0, xmm0, xmm1;
        vunpcklps xmm2, xmm2, xmm3;
        vunpcklpd xmm0, xmm0, xmm2;

        vmovaps[ebx], xmm0;
    }
}

void concatenate(mat4_t* dst, const mat4_t* m0, const mat4_t* m1_tr)
{
    __asm {
        mov eax, m0;
        mov ebx, dst;
        mov ecx, 4;
        mov edx, m1_tr;
        vmovaps xmm4, [edx];
        vmovaps xmm5, [edx + 16];
        vmovaps xmm6, [edx + 32];
        vmovaps xmm7, [edx + 48];

    convertLow:
        vmovaps xmm0, [eax];
        vmovaps xmm1, xmm0;
        vmovaps xmm2, xmm0;
        vmovaps xmm3, xmm0;
        dpps xmm0, xmm4, 11110001b;
        dpps xmm1, xmm5, 11110001b;
        dpps xmm2, xmm6, 11110001b;
        dpps xmm3, xmm7, 11110001b;
        vunpcklps xmm0, xmm0, xmm1;
        vunpcklps xmm2, xmm2, xmm3;
        vunpcklpd xmm0, xmm0, xmm2;
        vmovaps[ebx], xmm0;
        add eax, 16;
        add ebx, 16;
        loop convertLow;
    }
}

void run(vec4_t* world_pts, const vec4_t* local_pts, const size_t count, const vec3_t* scale, const vec3_t* rotation, const vec3_t* translation)
{
    mat4_t buffer_matrix;
    mat4_t matrix;
    mat_scale(&matrix, scale->x, scale->y, scale->z);

    mat_rotation_x(&buffer_matrix, rotation->x);
    transpose(&buffer_matrix);
    concatenate(&matrix, &matrix, &buffer_matrix);

    mat_rotation_y(&buffer_matrix, rotation->y);
    transpose(&buffer_matrix);
    concatenate(&matrix, &matrix, &buffer_matrix);

    mat_rotation_z(&buffer_matrix, rotation->z);
    transpose(&buffer_matrix);
    concatenate(&matrix, &matrix, &buffer_matrix);

    mat_translation(&buffer_matrix, translation->x, translation->y, translation->z);
    transpose(&buffer_matrix);
    concatenate(&matrix, &matrix, &buffer_matrix);

    transpose(&matrix);
    size_t curr_count = 0;
    while (curr_count < count) {
        transform(world_pts, local_pts, &matrix);
        ++local_pts;
        ++world_pts;
        ++curr_count;
    }
}
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "transformer.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t read_points(vec4_t* points, const size_t count)
{
    assert(points != NULL);

    size_t i_read;

    memset(points, 0, count * sizeof(vec4_t));

    vec4_t vec4 = { 0.0f, 0.0f, 0.0f, 1.0f };

    size_t num_input = 0u;

    
    for (i_read = 0; i_read < count; ++i_read) {
        num_input = scanf("%f %f %f", &vec4.x, &vec4.y, &vec4.z);

        if (num_input != 3) {
            break;
        }

        points[i_read] = vec4;
    }

    return i_read;
}

void print_csv(const vec4_t* points, const size_t count)
{
    assert(points != NULL);

    size_t i;

    for (i = 0; i < count; ++i) {
        printf("%9.4f,%9.4f,%9.4f\n", points[i].x, points[i].y, points[i].z);
    }
}

/*
x0 y0 z0 w0
x1 y1 z1 w1
x2 y2 z2 w2
x3 y3 z3 w3
*/

void transpose(mat4_t* mat)
{
    __asm {
        mov eax, mat

        movaps xmm0, [eax]
        movaps xmm1, [eax+16]
        movaps xmm2, [eax+32]
        movaps xmm3, [eax+48]

        movaps xmm4, xmm0
        shufps xmm4, xmm1, 01000100b
        shufps xmm0, xmm1, 11101110b

        movaps xmm5, xmm2
        shufps xmm5, xmm3, 01000100b
        shufps xmm2, xmm3, 11101110b

        movaps xmm1, xmm4
        shufps xmm1, xmm5, 10001000b
        movaps [eax], xmm1

        shufps xmm4, xmm5, 11011101b
        movaps [eax+16], xmm4

        movaps xmm1, xmm0
        shufps xmm1, xmm2, 10001000b
        movaps [eax+32], xmm1

        shufps xmm0, xmm2, 11011101b
        movaps [eax+48], xmm0
    }
}

void transform(vec4_t* dst, const vec4_t* src, const mat4_t* mat_tr)
{
    __asm {
        mov eax, src
        mov ebx, mat_tr

        movaps xmm0, [eax]
        movaps xmm1, [ebx]
        movaps xmm2, [ebx+16]
        movaps xmm3, [ebx+32]
        movaps xmm4, [ebx+48]

        dpps   xmm1, xmm0, 11110001b
        dpps   xmm2, xmm0, 11110010b
        dpps   xmm3, xmm0, 11110100b
        dpps   xmm4, xmm0, 11111000b

        orps   xmm1, xmm2
        orps   xmm1, xmm3
        orps   xmm1, xmm4

        mov    eax, dst
        movaps [eax], xmm1
    }
}

void concatenate(mat4_t* dst, const mat4_t* m0, const mat4_t* m1_tr)
{
    __asm {
        mov edi, dst
        mov esi, m0
        mov eax, m1_tr
        mov ecx, 4

    CONCAT_LOOP:
        movaps xmm0, [esi]
        movaps xmm1, [eax]
        movaps xmm2, [eax+16]
        movaps xmm3, [eax+32]
        movaps xmm4, [eax+48]

        dpps   xmm1, xmm0, 11110001b
        dpps   xmm2, xmm0, 11110010b
        dpps   xmm3, xmm0, 11110100b
        dpps   xmm4, xmm0, 11111000b

        orps   xmm1, xmm2
        orps   xmm1, xmm3
        orps   xmm1, xmm4

        movaps [edi], xmm1

        add    esi, 16
        add    edi, 16

        loop CONCAT_LOOP
    }
}

void run(vec4_t* world_pts, const vec4_t* local_pts, const size_t count, const vec3_t* scale, const vec3_t* rotation, const vec3_t* translation)
{
    assert(world_pts != NULL && local_pts != NULL && scale != NULL && translation != NULL);

    size_t i;
    mat4_t mt_transform;
    mat4_t mt_scale;
    mat4_t mt_rotation_x;
    mat4_t mt_rotation_y;
    mat4_t mt_rotation_z;
    mat4_t mt_translation;

    mat_scale(&mt_scale, scale->x, scale->y, scale->z);
    mat_rotation_x(&mt_rotation_x, rotation->x);
    mat_rotation_y(&mt_rotation_y, rotation->y);
    mat_rotation_z(&mt_rotation_z, rotation->z);
    mat_translation(&mt_translation, translation->x, translation->y, translation->z);

    transpose(&mt_rotation_x);
    transpose(&mt_rotation_y);
    transpose(&mt_rotation_z);
    transpose(&mt_translation);

    concatenate(&mt_transform, &mt_scale, &mt_rotation_x);
    concatenate(&mt_transform, &mt_transform, &mt_rotation_y);
    concatenate(&mt_transform, &mt_transform, &mt_rotation_z);
    concatenate(&mt_transform, &mt_transform, &mt_translation);

    transpose(&mt_transform);

    for (i = 0; i < count; ++i) {
        transform(&world_pts[i], &local_pts[i], &mt_transform);
    }
}

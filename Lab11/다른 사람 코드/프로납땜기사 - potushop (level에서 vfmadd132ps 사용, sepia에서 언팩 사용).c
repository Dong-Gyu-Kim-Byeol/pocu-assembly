#include <assert.h>

#include "potushop.h"
#include "image.h"

#define BROADCAST_VECTOR(vector, num) vector.x = num;\
vector.y = num;\
vector.z = num;\
vector.w = num;

ALIGN16 typedef struct vector4 {
    float x;
    float y;
    float z;
    float w;
} vector4_t;

const static vector4_t GRAYSCALE = { 0.3f, 0.59f, 0.11f, 0.f };

const static vector4_t SEPIA_R = { 0.393f, 0.769f, 0.189f, 0.f };
const static vector4_t SEPIA_G = { 0.349f, 0.686f, 0.168f, 0.f };
const static vector4_t SEPIA_B = { 0.272f, 0.534f, 0.131f, 0.f };
const static vector4_t WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
const static vector4_t WHITE_255 = { 255.0f, 255.0f, 255.0f, 1.0f };
static vector4_t s_bright_adder;
static vector4_t s_imin_neg;
static vector4_t s_od_over_id_mul_normalizer;
static vector4_t s_omin_over_255;
static vector4_t s_clamp_max;

void set_brightness_arg(int brightness)
{
    assert(brightness >= -100 && brightness <= 100);
    assert(sizeof(vector4_t) == 16);

    float adder = (float)brightness / 255.0f;
    s_bright_adder.x = adder;
    s_bright_adder.y = adder;
    s_bright_adder.z = adder;
    s_bright_adder.w = adder;

}

void set_level_args(int in_min, int in_max, int out_min, int out_max)
{
    assert(in_min >= 0 && in_min <= 253);
    assert(in_max >= 2 && in_max <= 255);
    assert(in_min < in_max);

    assert(out_min >= 0 && out_min <= 255);
    assert(out_max >= 0 && out_max <= 255);
    int id = in_max - in_min;

    BROADCAST_VECTOR(s_clamp_max, (float)id / 255.0f);

    int od = out_max - out_min;
    //id *= 255;
    float od_over_id_mul_normalizer = (float)od / (float)id;

    float in_min_negf = -(float)in_min / 255.0f;
    float out_min_f = (float)out_min / 255.0f;
    BROADCAST_VECTOR(s_od_over_id_mul_normalizer, od_over_id_mul_normalizer);
    BROADCAST_VECTOR(s_imin_neg, in_min_negf);
    BROADCAST_VECTOR(s_omin_over_255, out_min_f);


}
//ALIGN16 float g_pixels[MAX_PIXELS][4];
//int g_num_pixels;
void to_grayscale(void)
{
    __asm {
        mov ebx, OFFSET g_pixels;
        mov ecx, g_num_pixels;
        vmovaps xmm1, GRAYSCALE;
    convert:
        vmovaps xmm0, [ebx];
        dpps xmm0, xmm1, 0FFh;
        vmovaps[ebx], xmm0;
        add ebx, 16;
        loop convert;
    }
}

void to_sepia(void)
{
    __asm {
        mov ebx, OFFSET g_pixels;
        mov ecx, g_num_pixels;
        vmovaps xmm3, SEPIA_R;
        vmovaps xmm4, SEPIA_G;
        vmovaps xmm5, SEPIA_B;
        vmovaps xmm6, WHITE;
    convert:
        vmovaps xmm0, [ebx];
        vmovaps xmm1, xmm0;
        vmovaps xmm2, xmm0;
        dpps xmm0, xmm3, 01110001b;
        dpps xmm1, xmm4, 01110001b;
        dpps xmm2, xmm5, 01110001b;
        vunpcklps xmm0, xmm0, xmm1;
        vunpcklpd xmm0, xmm0, xmm2;
        vminps xmm0, xmm0, xmm6;
        vmovaps[ebx], xmm0;
        add ebx, 16;
        loop convert;
    }
}

void change_brightness(void)
{
    __asm {
        mov ebx, OFFSET g_pixels;
        mov ecx, g_num_pixels;
        vmovaps xmm1, s_bright_adder;
        vmovaps xmm2, WHITE;
        vxorps xmm3, xmm3, xmm3;
    convert:
        vmovaps xmm0, [ebx];
        vaddps xmm0, xmm0, xmm1;
        vminps xmm0, xmm0, xmm2;
        vmaxps xmm0, xmm0, xmm3;
        vmovaps[ebx], xmm0;
        add ebx, 16;
        loop convert;
    }
}
/*
const static vector4_t WHITE_255 = { 255.0f, 255.0f, 255.0f, 1.0f };
static vector4_t s_bright_adder;
static vector4_t s_imin_neg;
static vector4_t s_od_mul_normalizer_over_id;
static vector4_t s_omin_over_255;
static vector4_t s_clamp_max;
*/
void change_levels(void)
{
    __asm {
        mov ebx, OFFSET g_pixels;
        mov ecx, g_num_pixels;
        vmovaps xmm1, s_imin_neg;
        vmovaps xmm2, WHITE_255;
        vxorps xmm3, xmm3, xmm3;
        vmovaps xmm4, s_clamp_max;
        vmovaps xmm5, s_omin_over_255;
        vmovaps xmm6, s_od_over_id_mul_normalizer;

    convert:
        vmovaps xmm0, [ebx];
        vaddps xmm0, xmm0, xmm1;
        vmaxps xmm0, xmm0, xmm3;
        vminps xmm0, xmm0, xmm4;
        vfmadd132ps xmm0, xmm5, xmm6;
        vmovaps[ebx], xmm0;
        add ebx, 16;
        loop convert;
    }

}

#include <assert.h>
#include <math.h>

#include "potushop.h"
#include "image.h"

ALIGN16 typedef struct vector4 {
    float x;
    float y;
    float z;
    float w;
} vector4_t;

const static vector4_t ONE = { 1.f, 1.f, 1.f, 1.f };

const static vector4_t GRAYSCALE = { 0.3f, 0.59f, 0.11f, 0.f };

const static vector4_t SEPIA_R = { 0.393f, 0.769f, 0.189f, 0.f };
const static vector4_t SEPIA_G = { 0.349f, 0.686f, 0.168f, 0.f };
const static vector4_t SEPIA_B = { 0.272f, 0.534f, 0.131f, 0.f };

const static double MAX_COLOR_VALUE = 255.0;

static vector4_t s_brightness = { 0.f, 0.f, 0.f, 0.f };

static vector4_t s_level_in_min = { 0.f, 0.f, 0.f, 0.f };
static vector4_t s_level_in_max = { 0.f, 0.f, 0.f, 0.f };
static vector4_t s_level_out_min = { 0.f, 0.f, 0.f, 0.f };
static vector4_t s_level_out_cvt = { 0.f, 0.f, 0.f, 0.f };

void set_brightness_arg(int brightness)
{
    assert(brightness >= -100 && brightness <= 100);
    assert(sizeof(vector4_t) == 16);

    const float brightness_f = (float)((double)brightness / MAX_COLOR_VALUE);
    s_brightness.x = brightness_f;
    s_brightness.y = brightness_f;
    s_brightness.z = brightness_f;
}

void set_level_args(int in_min, int in_max, int out_min, int out_max)
{
    assert(in_min >= 0 && in_min <= 253);
    assert(in_max >= 2 && in_max <= 255);
    assert(in_min < in_max);

    assert(out_min >= 0 && out_min <= 255);
    assert(out_max >= 0 && out_max <= 255);

    const float in_min_f = (float)((double)in_min / MAX_COLOR_VALUE);
    s_level_in_min.x = in_min_f;
    s_level_in_min.y = in_min_f;
    s_level_in_min.z = in_min_f;

    const float in_max_f = (float)((double)in_max / MAX_COLOR_VALUE);
    s_level_in_max.x = in_max_f;
    s_level_in_max.y = in_max_f;
    s_level_in_max.z = in_max_f;

    /*
    v∈[in_min, in_max]를  v∈[out_min, out_max]로 변환
    { (v - in_min) / (in_max - in_min) } * (out_max - out_min) + out_min
    = (v - in_min) * ((out_max - out_min) / (in_max - in_min)) + out_min
    = (v - in_min) * out_cvt + out_min
    */
    const float out_min_f = (float)((double)out_min / MAX_COLOR_VALUE);
    s_level_out_min.x = out_min_f;
    s_level_out_min.y = out_min_f;
    s_level_out_min.z = out_min_f;

    const float out_cvt_f = (float)((double)(out_max - out_min) / (double)(in_max - in_min));
    s_level_out_cvt.x = out_cvt_f;
    s_level_out_cvt.y = out_cvt_f;
    s_level_out_cvt.z = out_cvt_f;
}

void to_grayscale(void)
{
    /*
    r = r * 0.3 + g * 0.59 + b * 0.11
    g = r * 0.3 + g * 0.59 + b * 0.11
    b = r * 0.3 + g * 0.59 + b * 0.11
    */

    __asm {
        mov eax, 0;
        mov ecx, g_num_pixels;
        movaps xmm0, GRAYSCALE;
        movaps xmm1, ONE;
        xorps xmm2, xmm2;

    pixel_loop:
        movaps xmm7, g_pixels[eax];
        mulps xmm7, xmm0;
        haddps xmm7, xmm2;
        haddps xmm7, xmm2;
        shufps xmm7, xmm7, 00000000b;
        minps xmm7, xmm1;
        movaps g_pixels[eax], xmm7;

        add eax, 16;
        loop pixel_loop;
    }
}

void to_sepia(void)
{
    /*
    r = r * 0.393 + g * 0.769 + b * 0.189
    g = r * 0.349 + g * 0.686 + b * 0.168
    b = r * 0.272 + g * 0.534 + b * 0.131
    */

    __asm {
        mov eax, 0;
        mov ecx, g_num_pixels;
        movaps xmm0, SEPIA_R;
        movaps xmm1, SEPIA_G;
        movaps xmm2, SEPIA_B;
        movaps xmm3, ONE;
        xorps xmm4, xmm4;

    pixel_loop:
        movaps xmm7, g_pixels[eax];
        xorps xmm6, xmm6;

        movaps xmm5, xmm7;
        mulps xmm5, xmm0;
        haddps xmm5, xmm4;
        haddps xmm5, xmm4;
        shufps xmm5, xmm5, 01010100b;
        addps xmm6, xmm5;

        movaps xmm5, xmm7;
        mulps xmm5, xmm1;
        haddps xmm5, xmm4;
        haddps xmm5, xmm4;
        shufps xmm5, xmm5, 01010001b;
        addps xmm6, xmm5;

        movaps xmm5, xmm7;
        mulps xmm5, xmm2;
        haddps xmm5, xmm4;
        haddps xmm5, xmm4;
        shufps xmm5, xmm5, 01000101b;
        addps xmm6, xmm5;

        minps xmm6, xmm3;
        movaps g_pixels[eax], xmm6;

        add eax, 16;
        loop pixel_loop;
    }
}

void change_brightness(void)
{
    /*
    r = r + brightness
    g = g + brightness
    b = b + brightness
    */

    __asm {
        mov eax, 0;
        mov ecx, g_num_pixels;
        movaps xmm0, s_brightness;
        movaps xmm1, ONE;
        xorps xmm2, xmm2;

    pixel_loop:
        movaps xmm7, g_pixels[eax];
        addps xmm7, xmm0;
        minps xmm7, xmm1;
        maxps xmm7, xmm2;
        movaps g_pixels[eax], xmm7;

        add eax, 16;
        loop pixel_loop;
    }
}

void change_levels(void)
{
    __asm {
        mov eax, 0;
        mov ecx, g_num_pixels;
        movaps xmm0, s_level_in_min;
        movaps xmm1, s_level_in_max;
        movaps xmm2, s_level_out_cvt;
        movaps xmm3, s_level_out_min;

    pixel_loop:
        movaps xmm7, g_pixels[eax];
        maxps xmm7, xmm0;
        minps xmm7, xmm1;

        /*
        v∈[in_min, in_max]를  v∈[out_min, out_max]로 변환
        { (v - in_min) / (in_max - in_min) } * (out_max - out_min) + out_min
        = (v - in_min) * ((out_max - out_min) / (in_max - in_min)) + out_min
        = (v - in_min) * out_cvt + out_min
        */
        subps xmm7, xmm0;
        mulps xmm7, xmm2;
        addps xmm7, xmm3;

        movaps g_pixels[eax], xmm7;

        add eax, 16;
        loop pixel_loop;
    }
}
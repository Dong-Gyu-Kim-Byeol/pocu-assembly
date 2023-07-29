/***************************************************************************/
/* WARNING: DO NOT CHANGE. This file will be overwritten by Buildbot       */
/***************************************************************************/
#include "image.h"

#include <assert.h>
#include <stdio.h>

ALIGN16 float g_pixels[MAX_PIXELS][4];
int g_num_pixels;

#pragma pack(push, 1)
typedef struct bitmap_header {
    /* bitmap file header (14 bytes) */
    unsigned short signature; /* must be 0x4D42 */
    unsigned int filesize;
    unsigned short reserved0;
    unsigned short reserved1;
    unsigned int data_offset;

    /* only support BITMAPINFOHEADER (40 bytes)*/
    unsigned int info_size; /* must be 40, 52(v2), 56(v3), 108(v4), 124(v5) */
    int width;
    int height;
    unsigned short num_planes; /* must be 1 */
    unsigned short bpp; /* only support 32 bits */
    unsigned int compression; /* only supports 0: BI_RGB */
    unsigned int bitmapsize;
    int h_res;
    int v_res;
    unsigned int num_colors;
    unsigned int num_important_colors; /* ignored */
    unsigned char extra[84];
} bitmap_header_t;
#pragma pack(pop)

static bitmap_header_t s_header;
static unsigned int s_argb8_pixels[MAX_PIXELS];

void normalize_pixels(void)
{
    int i;

    assert(g_num_pixels > 0);

    for (i = 0; i < g_num_pixels; ++i) {
        const unsigned int color = s_argb8_pixels[i];

        g_pixels[i][R] = ((color >> 16) & 0xFF) / 255.f;
        g_pixels[i][G] = ((color >> 8) & 0xFF) / 255.f;
        g_pixels[i][B] = (color & 0xFF) / 255.f;
        g_pixels[i][A] = ((color >> 24) & 0xFF) / 255.f;
    }
}

static inline unsigned int get_xrgb8(size_t i)
{
    unsigned char r = (unsigned char)(g_pixels[i][R] * 255.f + 0.5f);
    unsigned char g = (unsigned char)(g_pixels[i][G] * 255.f + 0.5f);
    unsigned char b = (unsigned char)(g_pixels[i][B] * 255.f + 0.5f);
    unsigned char a = 0xFF;

    return a << 24 | r << 16 | g << 8 | b;
}

static void to_argb8(void)
{
    int i;

    for (i = 0; i < g_num_pixels; ++i) {
        s_argb8_pixels[i] = get_xrgb8(i);
    }
}

enum {
    FILE_HEADER_SIZE = 14,
    DIB_SIZE_V1 = 40,
    DIB_SIZE_V2 = 52,
    DIB_SIZE_V3 = 56,
    DIB_SIZE_V4 = 108,
    DIB_SIZE_V5 = 124,
};

int load_bitmap(const char* filename)
{
    FILE* fp;
    int num_bytes;
    size_t num_read;

    assert(sizeof(bitmap_header_t) == FILE_HEADER_SIZE + DIB_SIZE_V5);

    /* open file */
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "error: couldn't open %s\n", filename);

        return 0;
    }

    /* read header */
    num_read = fread(&s_header, sizeof(bitmap_header_t), 1, fp);

    if (num_read != 1
        || s_header.signature != 0x4D42
        || (s_header.info_size != DIB_SIZE_V1 && s_header.info_size != DIB_SIZE_V2 && s_header.info_size != DIB_SIZE_V3 && s_header.info_size != DIB_SIZE_V4 && s_header.info_size != DIB_SIZE_V5)
        || s_header.num_planes != 1
        || s_header.bpp != 32
        || (s_header.compression != 0 && s_header.compression != 3)) {

        fprintf(stderr, "unsupported bitmap file format: %s\n", filename);

        fclose(fp);

        return 0;
    }

    num_bytes = s_header.width * s_header.height * 4;

    if (num_bytes > sizeof(s_argb8_pixels)) {

        fprintf(stderr, "image too big: %s\n", filename);

        fclose(fp);

        return 0;
    }

    assert(s_header.data_offset == FILE_HEADER_SIZE + s_header.info_size);

    /* copy pixel data*/
    fseek(fp, s_header.data_offset, SEEK_SET);
    num_read = fread(s_argb8_pixels, num_bytes, 1, fp);

    if (num_read != 1) {

        fprintf(stderr, "failed to read enough pixel data: %s\n", filename);

        fclose(fp);

        return 0;
    }

    fclose(fp);

    g_num_pixels = s_header.width * s_header.height;

    return 1;
}

int save_bitmap(const char* filename)
{
    /* open file */
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "error: couldn't open %s\n", filename);

        return 0;
    }

    /* write header */
    fwrite(&s_header, FILE_HEADER_SIZE + s_header.info_size, 1, fp);

    to_argb8();

    /* write data */
    fwrite(s_argb8_pixels, s_header.width * s_header.height * 4, 1, fp);

    fclose(fp);

    return 1;
}

int compare_bitmap(const char* filename)
{
    int i;
    int ret = 0;
    const bitmap_header_t prev_header = s_header;
    const int prev_num_pixels = g_num_pixels;

    if (!load_bitmap(filename)) {
        ret = -1;
        goto exit;
    }

    if (s_header.width != prev_header.width || s_header.height != prev_header.height) {
        fprintf(stderr, "image size mismatch: %s\n", filename);

        ret = -2;
        goto exit;
    }

    for (i = 0; i < g_num_pixels; ++i) {
        unsigned int actual = get_xrgb8(i);
        unsigned int expected = s_argb8_pixels[i];

        if (expected != actual) {
            ret = i + 1;
            goto exit;
        }
    }

exit:
    g_num_pixels = prev_num_pixels;
    s_header = prev_header;
    return ret;
}
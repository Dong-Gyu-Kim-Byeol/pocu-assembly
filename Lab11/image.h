/***************************************************************************/
/* WARNING: DO NOT CHANGE. This file will be overwritten by Buildbot       */
/***************************************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#define ALIGN16 __declspec(align(16))

enum { MAX_PIXELS = 1024 * 1024 };
enum {
    R,
    G,
    B,
    A
};

/* number of pixels in currently loaded image. 16-byte aligned */
extern int g_num_pixels;
/* normalized pixel data (0~100%). 16-byte aligned */
extern float g_pixels[MAX_PIXELS][4];

/***************************************************************************/
/* Loads image data from a .bmp file to a temporary buffer.                */
/* Before manipulating pixels in g_pixels, call normalize_pixels() first   */
/*                                                                         */
/* returns                                                                 */
/*   1: success                                                            */
/*   0: fail                                                               */
/***************************************************************************/
int load_bitmap(const char* filename);

/***************************************************************************/
/* Copies the RGBA8 pixel data to g_pixels                                 */
/*                                                                         */
/***************************************************************************/
void normalize_pixels(void);

/***************************************************************************/
/* Saves current working copy in g_pixels to a .bmp file                   */
/*                                                                         */
/* returns                                                                 */
/*   1: success                                                            */
/*   0: fail                                                               */
/***************************************************************************/
int save_bitmap(const char* filename);

/***************************************************************************/
/* Compares current working copy with bitmap file specified in filename    */
/*                                                                         */
/* returns                                                                 */
/*  -2: dimension mismatch                                                 */
/*  -1: file open error                                                    */
/*   0: content same                                                       */
/*  +n: content different at offset n - 1                                  */
/***************************************************************************/
int compare_bitmap(const char* filename);

#endif /* IMAGE_H */
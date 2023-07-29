#include "image.h"
#include "potushop.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    int ret;

    {
        ret = load_bitmap("data/sneakers_1024.bmp");
        assert(ret);

        normalize_pixels();

        to_grayscale();

        ret = save_bitmap("grayscale.bmp");
        assert(ret);

        assert(compare_bitmap("data/grayscale_expected.bmp") == 0);
    }

    {
        ret = load_bitmap("data/sneakers_1024.bmp");
        assert(ret);

        normalize_pixels();

        to_sepia();

        ret = save_bitmap("sepia.bmp");
        assert(ret);

        assert(compare_bitmap("data/sepia_expected.bmp") == 0);
    }

    {
        ret = load_bitmap("data/sneakers_1024.bmp");
        assert(ret);

        normalize_pixels();

        set_brightness_arg(-74);
        change_brightness();

        ret = save_bitmap("darken.bmp");
        assert(ret);

        assert(compare_bitmap("data/darken_expected.bmp") == 0);
    }

    {
        ret = load_bitmap("data/sneakers_1024.bmp");
        assert(ret);

        normalize_pixels();

        set_level_args(122, 211, 89, 147);
        change_levels();

        ret = save_bitmap("inout.bmp");
        assert(ret);

        assert(compare_bitmap("data/inout_expected.bmp") == 0);
    }

    return 0;
}
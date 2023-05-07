#include <assert.h>

#include "lab1.h"

int main(void)
{
    {
        int num = 0;

        num = add(1, 2);
        assert(num == 3);

        num = add(1, -2);
        assert(num == -1);
    }

    {
        int num = 0;

        num = sub(2, 1);
        assert(num == 1);

        num = sub(2, -1);
        assert(num == 3);

        num = sub(1, 2);
        assert(num == -1);
    }

    return 0;
}

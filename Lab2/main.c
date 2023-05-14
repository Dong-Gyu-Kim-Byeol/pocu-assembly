#include "canvas.h"
#include "palette.h"

#include <assert.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>

#define MAX_SIZE (32)

#define GET_INSTRUCTION(opcode, operand) (((opcode) << 5) | (operand))

void test(void)
{
    unsigned char canvas[MAX_SIZE * MAX_SIZE];

    set_canvas(canvas);

    unsigned char palette_index = 0;
    unsigned char color_index = 20;

    const unsigned char* palette = get_palette(palette_index);
    unsigned char color = palette[color_index];

    execute(GET_INSTRUCTION(0b000, color_index));

    for (int i = 0; i < MAX_SIZE; ++i)
    {
        for (int j = 0; j < MAX_SIZE; ++j)
        {
            assert(canvas[j * MAX_SIZE + i] == color);
        }
    }

    palette_index = 27;
    color_index = 19;

    palette = get_palette(palette_index);
    color = palette[color_index];

    unsigned char x = 7;
    unsigned char y = 12;

    execute(GET_INSTRUCTION(0b001, palette_index));
    execute(GET_INSTRUCTION(0b010, x));
    execute(GET_INSTRUCTION(0b011, y));

    execute(GET_INSTRUCTION(0b100, color_index));

    assert(canvas[y * MAX_SIZE + x] == color);

    unsigned char xy = 0b01001;

    execute(GET_INSTRUCTION(0b110, xy));

    color_index = 3;
    color = palette[color_index];

    execute(GET_INSTRUCTION(0b100, color_index));

    x = 0;
    y = 31;

    unsigned int index = y * MAX_SIZE + x;
    unsigned char color_canvas = canvas[index];
    assert(canvas[y * MAX_SIZE + x] == color);

    color_index = 11;
    color = palette[color_index];

    execute(GET_INSTRUCTION(0b101, color_index));
    execute(GET_INSTRUCTION(0b111, 0b10010));

    y = 0;

    assert(canvas[y * MAX_SIZE + x] == color);

    execute(GET_INSTRUCTION(0b111, 0b00110));

    ++x;
    ++y;

    assert(canvas[y * MAX_SIZE + x] != color);

    execute(GET_INSTRUCTION(0b111, 0b10110));

    assert(canvas[y * MAX_SIZE + x] == color);
    assert(canvas[(y + 1) * MAX_SIZE + (x + 1) == color]);
}

int main(void)
{
    test();

    return 0;
}
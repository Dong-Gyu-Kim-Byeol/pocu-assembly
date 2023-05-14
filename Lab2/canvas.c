#include "canvas.h"
#include "palette.h"

#include <assert.h>
#include <memory.h>

#define CANVAS_SIZE (32)

unsigned char* g_canvas32x32 = NULL;

// registor

const unsigned char* g_p_palette = NULL;

void set_palette(unsigned char id)
{
    g_p_palette = get_palette(id);
}

unsigned char get_palette_color(const unsigned char index)
{
    assert(g_p_palette != NULL);

    unsigned char color = g_p_palette[index];
    return color;
}

unsigned int g_cursor_x = 0;

void set_cursor_x(const unsigned int x)
{
    g_cursor_x = x;
}

unsigned int g_cursor_y = 0;

void set_cursor_y(const unsigned int y)
{
    g_cursor_y = y;
}

unsigned int get_cursor_index(void)
{
    const unsigned int index = g_cursor_x + CANVAS_SIZE * g_cursor_y;
    return index;
}

unsigned char g_brush_color_index = 0;

void set_brush_color_index(const unsigned char index)
{
    g_brush_color_index = index;
}

unsigned char get_brush_color(void)
{
    assert(g_p_palette != NULL);

    const unsigned char color = g_p_palette[g_brush_color_index];
    return color;
}

void set_x_y(const unsigned char quad, const unsigned char corner)
{
#define QUAD_1 (0b00000000)
#define QUAD_2 (0b00000001)
#define QUAD_3 (0b00000010)
#define QUAD_4 (0b00000011)

#define CORNER_1 (0b00000000)
#define CORNER_2 (0b00000001)
#define CORNER_3 (0b00000010)
#define CORNER_4 (0b00000011)

    unsigned int x = 0;
    unsigned int y = 0;

    switch (quad) {
    case QUAD_1: {
        switch (corner) {
        case CORNER_1: {
            x = 16;
            y = 0;
            break;
        }
        case CORNER_2: {
            x = 16;
            y = 15;
            break;
        }
        case CORNER_3: {
            x = 31;
            y = 0;
            break;
        }
        case CORNER_4: {
            x = 31;
            y = 15;
            break;
        }
        default: {
            assert(0);
            break;
        }
        }

        break;
    }
    case QUAD_2: {
        switch (corner) {
        case CORNER_1: {
            x = 0;
            y = 0;
            break;
        }
        case CORNER_2: {
            x = 0;
            y = 15;
            break;
        }
        case CORNER_3: {
            x = 15;
            y = 0;
            break;
        }
        case CORNER_4: {
            x = 15;
            y = 15;
            break;
        }
        default: {
            assert(0);
            break;
        }
        }

        break;
    }
    case QUAD_3: {
        switch (corner) {
        case CORNER_1: {
            x = 0;
            y = 16;
            break;
        }
        case CORNER_2: {
            x = 0;
            y = 31;
            break;
        }
        case CORNER_3: {
            x = 15;
            y = 16;
            break;
        }
        case CORNER_4: {
            x = 15;
            y = 31;
            break;
        }
        default: {
            assert(0);
            break;
        }
        }

        break;
    }
    case QUAD_4: {
        switch (corner) {
        case CORNER_1: {
            x = 16;
            y = 16;
            break;
        }
        case CORNER_2: {
            x = 16;
            y = 31;
            break;
        }
        case CORNER_3: {
            x = 31;
            y = 16;
            break;
        }
        case CORNER_4: {
            x = 31;
            y = 31;
            break;
        }
        default: {
            assert(0);
            break;
        }
        }

        break;
    }
    default: {
        assert(0);
        break;
    }
    }

    set_cursor_x(x);
    set_cursor_y(y);
}

void brush_color(const unsigned char pen_mode)
{
#define PEN_MODE_UP     (0b00000000)
#define PEN_MODE_DOWN   (0b00000001)

    switch (pen_mode) {
    case PEN_MODE_UP: {
        // empty
        break;
    }
    case PEN_MODE_DOWN: {
        const unsigned int cursor_index = get_cursor_index();
        const unsigned char color = get_brush_color();

        g_canvas32x32[cursor_index] = color;
        break;
    }
    default: {
        assert(0);
        break;
    }
    }
}

// public --------------------------------------------------------------

void set_canvas(unsigned char* canvas32x32)
{
    assert(canvas32x32 != NULL);

    if (g_canvas32x32 == NULL) {
        set_palette(0);
        set_cursor_x(0);
        set_cursor_y(0);
    }

    g_canvas32x32 = canvas32x32;
}

void execute(unsigned char instruction)
{
    // instruction
    //  - <opcode><argument(s)>
    //     3 - bit      5 - bit

    // opcode
#define OPCODE_CLEAR            (0b00000000)
#define OPCODE_SET_PALETTE      (0b00100000)
#define OPCODE_SET_X            (0b01000000)
#define OPCODE_SET_Y            (0b01100000)
#define OPCODE_DRAW_COLOR       (0b10000000)
#define OPCODE_SET_BRUSH        (0b10100000)
#define OPCODE_SET_X_Y          (0b11000000)
#define OPCODE_MOV              (0b11100000)

    const unsigned char OPCODE_MASK = 0b11100000;
    const unsigned char ARGUMENT_MASK = 0b00011111;

    const unsigned char opcode = instruction & OPCODE_MASK;
    const unsigned char argument = instruction & ARGUMENT_MASK;
    switch (opcode) {
    case OPCODE_CLEAR: {
        assert(g_canvas32x32 != NULL);

        const unsigned char color = get_palette_color(argument);
        memset(g_canvas32x32, color, CANVAS_SIZE * CANVAS_SIZE);
        break;
    }
    case OPCODE_SET_PALETTE: {
        set_palette(argument);
        break;
    }
    case OPCODE_SET_X: {
        set_cursor_x(argument);
        break;
    }
    case OPCODE_SET_Y: {
        set_cursor_y(argument);
        break;
    }
    case OPCODE_DRAW_COLOR: {
        assert(g_canvas32x32 != NULL);

        const unsigned int cursor_index = get_cursor_index();
        const unsigned char color = get_palette_color(argument);

        g_canvas32x32[cursor_index] = color;
        break;
    }
    case OPCODE_SET_BRUSH: {
        set_brush_color_index(argument);
        break;
    }
    case OPCODE_SET_X_Y: {
        const unsigned char CORNER_MASK = 0b00000011;
        const unsigned char QUAD_MASK = 0b00001100;

        const unsigned char corner = argument & CORNER_MASK;
        const unsigned char quad = (argument & QUAD_MASK) >> 2;

        set_x_y(quad, corner);
        break;
    }
    case OPCODE_MOV: {
#define X_DIR_DONT_MOVE     (0b00000000)
#define X_DIR_RIGHT_MOVE    (0b00000001)
#define X_DIR_LEFT_MOVE     (0b00000010)

#define Y_DIR_DONT_MOVE     (0b00000000)
#define Y_DIR_UP_MOVE       (0b00000001)
#define Y_DIR_DOWN_MOVE     (0b00000010)

        const unsigned char Y_DIR_MASK = 0b00000011;
        const unsigned char X_DIR_MASK = 0b00001100;
        const unsigned char PEN_MODE_MASK = 0b00010000;

        const unsigned char y_dir = argument & Y_DIR_MASK;
        const unsigned char x_dir = (argument & X_DIR_MASK) >> 2;
        const unsigned char pen_mode = (argument & PEN_MODE_MASK) >> 4;

        brush_color(pen_mode);

        switch (y_dir) {
        case Y_DIR_DONT_MOVE: {
            // empty
            break;
        }
        case Y_DIR_UP_MOVE: {
            g_cursor_y -= 1;
            g_cursor_y &= ARGUMENT_MASK;
            break;
        }
        case Y_DIR_DOWN_MOVE: {
            g_cursor_y += 1;
            g_cursor_y &= ARGUMENT_MASK;
            break;
        }
        default: {
            assert(0);
            break;
        }
        }

        switch (x_dir) {
        case X_DIR_DONT_MOVE: {
            // empty
            break;
        }
        case X_DIR_RIGHT_MOVE: {
            g_cursor_x += 1;
            g_cursor_x &= ARGUMENT_MASK;
            break;
        }
        case X_DIR_LEFT_MOVE: {
            g_cursor_x -= 1;
            g_cursor_x &= ARGUMENT_MASK;
            break;
        }
        default: {
            assert(0);
            break;
        }
        }

        brush_color(pen_mode);

        break;
    }
    default: {
        assert(0);
        break;
    }
    }
}
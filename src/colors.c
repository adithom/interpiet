#include "colors.h"

static const PietOp instruction_table[6][3] = {
    { OP_NONE,      OP_PUSH,      OP_POP      },
    { OP_ADD,       OP_SUBTRACT,  OP_MULTIPLY },
    { OP_DIVIDE,    OP_MOD,       OP_NOT      },
    { OP_GREATER,   OP_POINTER,   OP_SWITCH   },
    { OP_DUPLICATE, OP_ROLL,      OP_IN_NUM   },
    { OP_IN_CHAR,   OP_OUT_NUM,   OP_OUT_CHAR }
};

int color_hue(PietColor c) {
    return c / 3;
}

int color_lightness(PietColor c) {
    return c % 3;
}

PietOp decode_instruction(PietColor old_color, PietColor new_color) {
    if (old_color >= COLOR_WHITE || new_color >= COLOR_WHITE) return OP_NONE;

    int hue_delta   = (color_hue(new_color)        - color_hue(old_color)        + 6) % 6;
    int light_delta = (color_lightness(new_color)   - color_lightness(old_color)  + 3) % 3;

    return instruction_table[hue_delta][light_delta];
}

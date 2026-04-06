#ifndef PIETVM_COLORS_H
#define PIETVM_COLORS_H

#include "types.h"

typedef enum {
    OP_NONE = 0,
    OP_PUSH,
    OP_POP,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MOD,
    OP_NOT,
    OP_GREATER,
    OP_POINTER,
    OP_SWITCH,
    OP_DUPLICATE,
    OP_ROLL,
    OP_IN_NUM,
    OP_IN_CHAR,
    OP_OUT_NUM,
    OP_OUT_CHAR
} PietOp;

int     color_hue(PietColor c);
int     color_lightness(PietColor c);
PietOp  decode_instruction(PietColor old_color, PietColor new_color);

#endif

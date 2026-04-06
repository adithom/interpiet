#include "interp.h"
#include "colors.h"
#include "ops.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

#define STEP_LIMIT 100000

static int dp_dx[] = { 1,  0, -1,  0 };
static int dp_dy[] = { 0,  1,  0, -1 };

static int is_passable(CodelGrid *grid, int x, int y) {
    if (!grid_in_bounds(grid, x, y)) return 0;
    PietColor c = grid_get(grid, x, y);
    return c != COLOR_BLACK && c != COLOR_UNKNOWN;
}

static const char *op_name(PietOp op) {
    switch (op) {
        case OP_NONE:      return "none";
        case OP_PUSH:      return "push";
        case OP_POP:       return "pop";
        case OP_ADD:       return "add";
        case OP_SUBTRACT:  return "subtract";
        case OP_MULTIPLY:  return "multiply";
        case OP_DIVIDE:    return "divide";
        case OP_MOD:       return "mod";
        case OP_NOT:       return "not";
        case OP_GREATER:   return "greater";
        case OP_POINTER:   return "pointer";
        case OP_SWITCH:    return "switch";
        case OP_DUPLICATE: return "duplicate";
        case OP_ROLL:      return "roll";
        case OP_IN_NUM:    return "in_num";
        case OP_IN_CHAR:   return "in_char";
        case OP_OUT_NUM:   return "out_num";
        case OP_OUT_CHAR:  return "out_char";
        default:           return "?";
    }
}

void interp_run(VM *vm, CodelGrid *grid, BlockMap *bm) {
    int running = 1;
    int steps   = 0;
    static const char *dp_str[] = { "RIGHT", "DOWN", "LEFT", "UP" };
    static const char *cc_str[] = { "LEFT", "RIGHT" };

    while (running) {
        if (steps >= STEP_LIMIT) {
            fprintf(stderr, "[debug] STEP LIMIT (%d) reached — likely infinite loop. "
                            "Last position: (%d,%d) dp=%s cc=%s\n",
                    STEP_LIMIT, vm->x, vm->y,
                    dp_str[vm->dp], cc_str[vm->cc]);
            break;
        }
        steps++;
        ColorBlock *cur_block = blockmap_get(bm, grid, vm->x, vm->y);
        PietColor old_color   = cur_block->color;

        int found  = 0;
        int next_x = 0;
        int next_y = 0;

        for (int attempt = 0; attempt < 8; attempt++) {
            Point exit = find_exit_codel(cur_block, vm->dp, vm->cc);
            int tx = exit.x + dp_dx[vm->dp];
            int ty = exit.y + dp_dy[vm->dp];

            if (is_passable(grid, tx, ty)) {
                next_x = tx;
                next_y = ty;
                found  = 1;
                break;
            }

            if (attempt % 2 == 0)
                vm->cc = (vm->cc == CC_LEFT) ? CC_RIGHT : CC_LEFT;
            else
                vm->dp = (Direction)((vm->dp + 1) % 4);
        }

        if (!found) {
            running = 0;
            break;
        }

        PietColor new_color = grid_get(grid, next_x, next_y);

        if (new_color == COLOR_WHITE) {
            while (grid_in_bounds(grid, next_x, next_y) &&
                   grid_get(grid, next_x, next_y) == COLOR_WHITE) {
                next_x += dp_dx[vm->dp];
                next_y += dp_dy[vm->dp];
            }

            if (!is_passable(grid, next_x, next_y)) {
                running = 0;
                break;
            }

            vm->x = next_x;
            vm->y = next_y;
            continue;
        }

        PietOp op       = decode_instruction(old_color, new_color);
        int block_size  = cur_block->size;

        static const char *color_name[] = {
            "lred","red","dred","lyel","yel","dyel",
            "lgrn","grn","dgrn","lcyn","cyn","dcyn",
            "lblu","blu","dblu","lmag","mag","dmag",
            "wht","blk","???"
        };
        fprintf(stderr, "[step %5d] (%2d,%2d)->(%2d,%2d) dp=%-5s cc=%-5s  %s->%s  op=%s (block_size=%d)\n",
                steps, vm->x, vm->y, next_x, next_y,
                dp_str[vm->dp], cc_str[vm->cc],
                color_name[old_color], color_name[new_color],
                op_name(op), block_size);

        switch (op) {
            case OP_NONE:      break;
            case OP_PUSH:      op_push(vm, block_size); break;
            case OP_POP:       op_pop(vm);              break;
            case OP_ADD:       op_add(vm);              break;
            case OP_SUBTRACT:  op_subtract(vm);         break;
            case OP_MULTIPLY:  op_multiply(vm);         break;
            case OP_DIVIDE:    op_divide(vm);           break;
            case OP_MOD:       op_mod(vm);              break;
            case OP_NOT:       op_not(vm);              break;
            case OP_GREATER:   op_greater(vm);          break;
            case OP_POINTER:   op_pointer(vm);          break;
            case OP_SWITCH:    op_switch(vm);           break;
            case OP_DUPLICATE: op_duplicate(vm);        break;
            case OP_ROLL:      op_roll(vm);             break;
            case OP_IN_NUM:    op_in_num(vm);           break;
            case OP_IN_CHAR:   op_in_char(vm);          break;
            case OP_OUT_NUM:   op_out_num(vm);          break;
            case OP_OUT_CHAR:  op_out_char(vm);         break;
        }

        vm->x = next_x;
        vm->y = next_y;
    }
}

#ifndef PIETVM_OPS_H
#define PIETVM_OPS_H

#include "types.h"

void op_push(VM *vm, int block_size);
void op_pop(VM *vm);
void op_add(VM *vm);
void op_subtract(VM *vm);
void op_multiply(VM *vm);
void op_divide(VM *vm);
void op_mod(VM *vm);
void op_not(VM *vm);
void op_greater(VM *vm);
void op_pointer(VM *vm);
void op_switch(VM *vm);
void op_duplicate(VM *vm);
void op_roll(VM *vm);
void op_in_num(VM *vm);
void op_in_char(VM *vm);
void op_out_num(VM *vm);
void op_out_char(VM *vm);

#endif

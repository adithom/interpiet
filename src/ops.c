#include "ops.h"
#include "stack.h"
#include <stdio.h>

void op_add(VM *vm) {
    int b = stack_pop(&vm->stack);
    int a = stack_pop(&vm->stack);
    stack_push(&vm->stack, a + b);
}

void op_subtract(VM *vm) {
    int b = stack_pop(&vm->stack);
    int a = stack_pop(&vm->stack);
    stack_push(&vm->stack, a - b);
}

void op_multiply(VM *vm) {
    int b = stack_pop(&vm->stack);
    int a = stack_pop(&vm->stack);
    stack_push(&vm->stack, a * b);
}

void op_divide(VM *vm) {
    int b = stack_pop(&vm->stack);
    int a = stack_pop(&vm->stack);
    if (b == 0) return;
    stack_push(&vm->stack, a / b);
}

void op_mod(VM *vm) {
    int b = stack_pop(&vm->stack);
    int a = stack_pop(&vm->stack);
    if (b == 0) return;
    stack_push(&vm->stack, a % b);
}

void op_push(VM *vm, int block_size) {
    stack_push(&vm->stack, block_size);
}

void op_pop(VM *vm) {
    stack_pop(&vm->stack);
}

void op_not(VM *vm) {
    int a = stack_pop(&vm->stack);
    stack_push(&vm->stack, a == 0 ? 1 : 0);
}

void op_greater(VM *vm) {
    int b = stack_pop(&vm->stack);
    int a = stack_pop(&vm->stack);
    stack_push(&vm->stack, a > b ? 1 : 0);
}

void op_duplicate(VM *vm) {
    if (vm->stack.top == 0) return;
    stack_push(&vm->stack, stack_peek(&vm->stack));
}

void op_roll(VM *vm) {
    int count = stack_pop(&vm->stack);
    int depth = stack_pop(&vm->stack);
    stack_roll(&vm->stack, depth, count);
}

void op_pointer(VM *vm) {
    int steps = stack_pop(&vm->stack);
    steps = ((steps % 4) + 4) % 4;
    vm->dp = (Direction)((vm->dp + steps) % 4);
}

void op_switch(VM *vm) {
    int times = stack_pop(&vm->stack);
    if (times < 0) times = -times;  // absolute value
    if (times % 2 != 0)
        vm->cc = (vm->cc == CC_LEFT) ? CC_RIGHT : CC_LEFT;
}



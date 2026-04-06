#ifndef PIETVM_STACK_H
#define PIETVM_STACK_H

#include "types.h"

Stack stack_init(void);
void  stack_push(Stack *s, int val);
int   stack_pop(Stack *s);
int   stack_peek(Stack *s);
void  stack_roll(Stack *s, int depth, int count);
void  stack_free(Stack *s);

#endif

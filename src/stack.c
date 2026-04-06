#include "stack.h"
#include <stdlib.h>

Stack stack_init(void) {
    Stack s;
    s.top      = 0;
    s.capacity = 8;
    s.data     = malloc(s.capacity * sizeof(int));
    return s;
}

void stack_push(Stack *s, int val) {
    if (s->top == s->capacity) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * sizeof(int));
    }
    s->data[s->top++] = val;
}

int stack_pop(Stack *s) {
    if (s->top == 0) return 0;
    return s->data[--s->top];
}

int stack_peek(Stack *s) {
    if (s->top == 0) return 0;
    return s->data[s->top - 1];
}

static void reverse(int *arr, int len) {
    int lo = 0, hi = len - 1;
    while (lo < hi) {
        int tmp  = arr[lo];
        arr[lo]  = arr[hi];
        arr[hi]  = tmp;
        lo++;
        hi--;
    }
}

void stack_roll(Stack *s, int depth, int count) {
    if (depth <= 0 || depth > s->top) return;
    if (count == 0) return;

    count = count % depth;
    if (count < 0) count += depth;

    int base = s->top - depth;

    reverse(s->data + base, depth);
    reverse(s->data + base, count);
    reverse(s->data + base + count, depth - count);
}

void stack_free(Stack *s) {
    free(s->data);
    s->data     = NULL;
    s->top      = 0;
    s->capacity = 0;
}

#include "../include/collections/stack.h"
#include "../include/llv.h"

#define BIG_STRING "this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size."

int main(void) {
    Stack stack = stack_new("Too big");
    stack_push(stack, NEW_NODE(stack, BIG_STRING));
    update(1, stack);
    stack_free(stack);
}

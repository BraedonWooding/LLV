#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/ll.h"
#include "../../include/collections/stack.h"
#include "../../include/helper.h"
#include "../list_helper.h"

Stack stack_new(char *name) {
    return ll_new(name);
}

void stack_free(Stack stack) {
    ll_free(stack);
}

void stack_free_node(StackNode n) {
    ll_free_node(n);
}

void stack_clear(Stack stack) {
    ll_clear(stack);
}

<<<<<<< HEAD
int stack_length(Stack stack) {
    return ll_length(stack);
}

int stack_is_empty(Stack stack) {
=======
size_t stack_length(Stack stack) {
    return ll_length(stack);
}

bool stack_is_empty(Stack stack) {
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
    return ll_is_empty(stack);
}

StackNode stack_new_node(Data data, TypeTag type) {
    return ll_new_node(data, type);
}

void stack_push(Stack stack, StackNode node) {
    ll_push(stack, node);
}

StackNode stack_pop(Stack stack) {
    return ll_pop(stack);
}

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/ll.h"
#include "../../include/collections/Stack.h"
#include "../../include/helper.h"
#include "../list_helper.h"

Stack stack_new(char *name) {
    return ll_new(name);
}

void stack_free(Stack stack) {
    ll_free(stack);
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

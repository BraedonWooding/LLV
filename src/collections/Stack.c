#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/LL.h"
#include "../../include/collections/Stack.h"
#include "../../include/helper.h"
#include "../list_helper.h"

Stack stack_new(char *name) {
    return LL_new(name);
}

void stack_free(Stack stack) {
    LL_free(stack);
}

StackNode stack_new_node(Data data, TypeTag type) {
    return LL_new_node(data, type);
}

void stack_push(Stack stack, StackNode node) {
    LL_push(stack, node);
}

StackNode stack_pop(Stack stack) {
    return LL_pop(stack);
}

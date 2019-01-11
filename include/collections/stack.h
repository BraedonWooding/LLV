#ifndef LLV_STACK_H
#define LLV_STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _singly_linked_list_t *Stack;
typedef struct _LL_node_t *StackNode;

Stack stack_new(char *name);

void stack_free(Stack stack);

StackNode stack_new_node(Data data, TypeTag type);

void stack_push(Stack stack, StackNode node);

StackNode stack_pop(Stack stack);

#endif
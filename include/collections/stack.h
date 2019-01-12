#ifndef LLV_STACK_H
#define LLV_STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _singly_linked_list_t *Stack;
typedef struct _LL_node_t *StackNode;

/* Create a new stack with a given name */
Stack stack_new(char *name);

/* Free the stack */
void stack_free(Stack stack);

/* 
   Create a new node in the stack 
   Could use NEW_NODE(stack, data);
*/
StackNode stack_new_node(Data data, TypeTag type);

/* Pushes a new node onto the top of the stack */
void stack_push(Stack stack, StackNode node);

/* Pops a node form the top of the stack */
StackNode stack_pop(Stack stack);

#endif
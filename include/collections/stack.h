#ifndef LLV_STACK_H
#define LLV_STACK_H

#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"
#include "ll_structs.h"

typedef struct _singly_linked_list_t *Stack;
typedef struct _LL_node_t *StackNode;

/* Create a new stack with a given name */
Stack stack_new(char *name);

/* Free the stack */
void stack_free(Stack stack);

/* Frees the given node */
void stack_free_node(StackNode n);

/* Returns how many items are currently on the stack */
int stack_length(Stack stack);

/* Returns true if there are no items on the stack */
int stack_is_empty(Stack stack);
/* Pops a node form the top of the stack */
StackNode stack_pop(Stack stack);

#endif /* LLV_STACK_H */

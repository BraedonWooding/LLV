#ifndef LLV_LINKED_LIST_H
#define LLV_LINKED_LIST_H

#include <stdlib.h>
#include <stdbool.h>

#include "ll_structs.h"

typedef struct _LL_node_t *LL_Node;

/*
    Create a new list with a given name and a default type.
*/
LL ll_new(char *name);

/*
    Clears and frees list.
*/
void ll_free(LL list);

/*
    Frees the given node.
*/
void ll_free_node(LL_Node n);

/*
    Creates a new node with the data given.
*/
LL_Node ll_new_node(Data data, TypeTag type);

/*
    Prints out the node data.
*/
void ll_default_print_data(LL_Node n);

/*
    Free all nodes in collection.
*/
void ll_clear(LL list);

/*
    Inserts the given node after the LL_Node 'at'.
*/
void ll_insert_after(LL list, LL_Node node, LL_Node at);

/*
    Inserts the given node before the LL_Node 'at'.
*/
void ll_insert_before(LL list, LL_Node node, LL_Node at);

/*
    Removes the given node from the list, returning the node removed.
*/
LL_Node ll_remove_node(LL list, LL_Node node);

/*
    Returns true if the list is empty.
*/
bool ll_is_empty(LL list);

/*
    List can be null in some cases.  Will find the previous node to the given one.
*/
LL_Node ll_find_prev(LL list, LL_Node at);

/*
    List can be null in some cases.  Will find the next node to the given one.
*/
LL_Node ll_find_next(LL_Node n);

/*
    Returns the length of the list.
*/
int ll_length(LL list);

/*
    Pushes node to top of list.
*/
void ll_push(LL list, LL_Node n);

/*
    Pops top node from list.
*/
LL_Node ll_pop(LL list);

/*
    Adds node to end of list.
*/
void ll_append(LL list, LL_Node n);

#endif /* LLV_LINKED_LIST_H */

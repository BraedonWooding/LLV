#ifndef LLV_LINKED_LIST_H
#define LLV_LINKED_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _LL_node_t *LL_Node;

struct _LL_node_t {
    char *ptr;          // for display
    Data data;          // the data of this node
    TypeTag data_tag;   // the corresponding tag for the data ^^
    LL_Node next;       // the next one in the list
};

typedef struct _singly_linked_list_t {
    COLLECTION_CHILD;   // inherited members
    LL_Node head;       // the front of the list
    LL_Node tail;       // the back of the list
} *LL;

/*
    Create a new list with a given name and a default type.
*/
LL LL_new(char *name);

/*
    Clears and frees list.
*/
void LL_free(LL list);

/*
    Frees the given node.
*/
void LL_free_node(LL_Node n);

/*
    Creates a new node with the data given.
*/
LL_Node LL_new_node(Data data, TypeTag type);

/*
    Prints out the node data.
*/
void LL_default_print_data(LL_Node n);

/*
    Free all nodes in collection.
*/
void LL_clear_list(LL list);

/*
    Inserts the given node after the LL_Node 'at'.
*/
void LL_insert_after(LL list, LL_Node node, LL_Node at);

/*
    Inserts the given node before the LL_Node 'at'.
*/
void LL_insert_before(LL list, LL_Node node, LL_Node at);

/*
    Removes the given node from the list, returning the node removed.
*/
LL_Node LL_remove_node(LL list, LL_Node node);

/*
    Returns true if the list is empty.
*/
bool LL_is_empty(LL list);

/*
    List can be null in some cases.  Will find the previous node to the given one.
*/
LL_Node LL_find_prev(LL list, LL_Node at);

/*
    List can be null in some cases.  Will find the next node to the given one.
*/
LL_Node LL_find_next(LL_Node n);

/*
    Returns the length of the list.
*/
size_t LL_length(LL list);

/*
    Pushes node to top of list.
*/
void LL_push(LL list, LL_Node n);

/*
    Pops top node from list.
*/
LL_Node LL_pop(LL list);

/*
    Adds node to end of list.
*/
void LL_append(LL list, LL_Node n);

#endif
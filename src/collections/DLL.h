#ifndef LLV_DOUBLY_LINKED_LIST_H
#define LLV_DOUBLY_LINKED_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _dll_node_t *DLL_Node;

struct _dll_node_t {
    char *ptr; // pointer linked
    Data data; // the data type
    TypeTag data_tag; // the corresponding tag;
    DLL_Node next;
    DLL_Node prev;
};

typedef struct _doubly_linked_list_t {
    COLLECTION_CHILD;
    DLL_Node head;
    DLL_Node tail;
} *DLL;

/*
    Create a new list with a given name and a default type.
*/
DLL DLL_new(char *name);

/*
    Clears and frees list.
*/
void DLL_free(DLL list);

/*
    Frees the given node.
*/
void DLL_free_node(DLL_Node n);

/*
    Creates a new node with the data given.
*/
DLL_Node DLL_new_node(Data data, TypeTag type);

/*
    Prints out the node data.
*/
void DLL_default_print_data(DLL_Node n);

/*
    Free all nodes in collection.
*/
void DLL_clear_list(DLL list);

/*
    Inserts the given node after the DLL_Node 'at'.
*/
void DLL_insert_after(DLL list, DLL_Node node, DLL_Node at);

/*
    Inserts the given node before the DLL_Node 'at'.
*/
void DLL_insert_before(DLL list, DLL_Node node, DLL_Node at);

/*
    Removes the given node from the list, returning the node removed.
*/
DLL_Node DLL_remove_node(DLL list, DLL_Node node);

/*
    Returns true if the list is empty.
*/
bool DLL_is_empty(DLL list);

/*
    List can be null in some cases.  Will find the previous node to the given one.
*/
DLL_Node DLL_find_prev(DLL_Node at);

/*
    List can be null in some cases.  Will find the next node to the given one.
*/
DLL_Node DLL_find_next(DLL_Node n);

/*
    Returns length of list.
*/
size_t DLL_length(DLL list);

/*
    Pushes node to top of list.
*/
void DLL_push(DLL list, DLL_Node n);

/*
    Pops top node from list.
*/
DLL_Node DLL_pop(DLL list);

/*
    Adds node to end of list.
*/
void DLL_append(DLL list, DLL_Node n);

#endif
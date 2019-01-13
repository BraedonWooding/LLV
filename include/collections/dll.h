#ifndef LLV_DOUBLY_LINKED_LIST_H
#define LLV_DOUBLY_LINKED_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _dll_node_t *DLL_Node;

struct _dll_node_t {
    char *ptr;          // for display
    Data data;          // the data type
    TypeTag data_tag;   // the corresponding tag for the data ^^;
    DLL_Node next;      // the next item in the LL
    DLL_Node prev;      // the previous item
};

typedef struct _doubly_linked_list_t {
    struct _collection_t parent;   // all the inherited members
    DLL_Node head;      // the head of the list
    DLL_Node tail;      // the tail of the list
} *DLL;

/*
    Create a new list with a given name and a default type.
*/
DLL dll_new(char *name);

/*
    Clears and frees list.
*/
void dll_free(DLL list);

/*
    Frees the given node.
*/
void dll_free_node(DLL_Node n);

/*
    Creates a new node with the data given.
*/
DLL_Node dll_new_node(Data data, TypeTag type);

/*
    Prints out the node data.
*/
void dll_default_print_data(DLL_Node n);

/*
    Free all nodes in collection.
*/
void dll_clear(DLL list);

/*
    Inserts the given node after the DLL_Node 'at'.
*/
void dll_insert_after(DLL list, DLL_Node node, DLL_Node at);

/*
    Inserts the given node before the DLL_Node 'at'.
*/
void dll_insert_before(DLL list, DLL_Node node, DLL_Node at);

/*
    Removes the given node from the list, returning the node removed.
*/
DLL_Node dll_remove_node(DLL list, DLL_Node node);

/*
    Returns true if the list is empty.
*/
bool dll_is_empty(DLL list);

/*
    List can be null in some cases.  Will find the previous node to the given one.
*/
DLL_Node dll_find_prev(DLL_Node at);

/*
    List can be null in some cases.  Will find the next node to the given one.
*/
DLL_Node dll_find_next(DLL_Node n);

/*
    Returns length of list.
*/
size_t dll_length(DLL list);

/*
    Pushes node to top of list.
*/
void dll_push(DLL list, DLL_Node n);

/*
    Pops top node from list.
*/
DLL_Node dll_pop(DLL list);

/*
    Adds node to end of list.
*/
void dll_append(DLL list, DLL_Node n);

#endif
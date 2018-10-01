// Just a visualisation implementation of a linked list
// Implementation is exposed as to allow you to edit the list yourself.

// header guard
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdlib.h>

#ifdef _WIN32
    #define WINDOWS_COMPATIBILITY
    #define CLEAR_SCREEN "cls"
#else
    #define UNIX_COMPATIBILITY
    #define CLEAR_SCREEN "clear"
#endif

/*
    If true It'll clear whenever you call update.
*/
extern bool clear_on_update;

/*
    The time between each step, if 0 then it requires the user to press enter.
*/
extern size_t ms_step;

/*
    Each node is represented as doubly linked, however
    will only be doubly linked if their parent list is 'doubly_linked'
*/
typedef struct _node {
    struct _node *next;
    struct _node *prev;
    union {
        void *data;
        long long value;
    };
} node;

typedef node *Node;

/*
    The linked list parent, maintains a head and tail pointer.
    Shouldn't edit doubly linked after creation as it won't fix up the previous pointers.
*/
typedef struct _linked_list {
    Node head;
    Node tail;
    bool doubly_linked;
    char *name; // used for output
} linkedList;

typedef linkedList *LL;

/*
    Sleeps for the given amount of time.
*/
void sleep_ms(size_t ms);

/*
    Frees the linked list.
    If doubly linked is true, it'll print out the list as doubly linked
    and will utilise the 'previous' pointers.
*/
LL new_LL(bool doubly_linked, char *name);

/*
    Frees the given node.
*/
Node new_node(long long val);

/*
    Frees the list.
*/
void free_list(LL list);

/*
    Free all lists.
*/
void free_all_lists(void);

/*
    Clears the given list.
*/
void clear_list(LL list);

/*
    Pushes the given node to the end of the list.
*/
void push_node(LL list, Node node);

/*
    Creates a new node and pushes it to the end of the list.
*/
void push_value(LL list, long long value);

/*
    Inserts the given node after the Node 'at'.
*/
void insert_after_node(LL list, Node node, Node at);

/*
    Creates a new node and inserts it after the position given (the 'at' Node).
*/
void insert_after_node_value(LL list, long long value, Node at);

/*
    Inserts the given node before the Node 'at'.
*/
void insert_before_node(LL list, Node node, Node at);

/*
    Creates a new node and inserts it before the position given (the 'at' Node).
*/
void insert_before_node_value(LL list, long long value, Node at);

/*
    Removes the given node from the list, returning the node removed.
*/
Node remove_node(LL list, Node node);

/*
    Returns true if the list is empty.
*/
bool is_empty(LL list);

/*
    Prints out a single node.
*/
void print_single_node(Node n);

/*
    Prints out a linked list of nodes.
*/
void print_list(LL list);

/*
    Prints out all the lists to the terminal.
    Use as a visualisation.
*/
void update(void);

#endif
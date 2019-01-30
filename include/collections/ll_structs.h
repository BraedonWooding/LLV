#ifndef LLV_LINKED_LIST_STRUCTS_H
#define LLV_LINKED_LIST_STRUCTS_H

/*
  This is so we don't have to import all the LL functions if we are just using stacks/queues.
*/

#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

struct _LL_node_t {
    char *ptr;          // for display
    Data data;          // the data of this node
    TypeTag data_tag;   // the corresponding tag for the data ^^
    struct _LL_node_t *next;       // the next one in the list
};

typedef struct _singly_linked_list_t {
    struct _collection_t parent;   // inherited members
    struct _LL_node_t *head;       // the front of the list
    struct _LL_node_t *tail;       // the back of the list
} *LL;

#endif /* LLV_LINKED_LIST_STRUCTS_H */

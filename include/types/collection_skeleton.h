#ifndef LLV_COLLECTION_SKELETON_H
#define LLV_COLLECTION_SKELETON_H

#include <stdlib.h>

/* This is used in implementation class to utilise a collection. */

#define DEFAULT_PRINT_HEIGHT (5)
#define DEFAULT_PTR_HEIGHT (2)

typedef struct _collection_t *Collection;

typedef void(*print_node)(void *n, char **buf, size_t size, size_t len, size_t offset);
typedef size_t(*sizeof_node)(void *n);
typedef void(*print_list)(Collection collection);

/*
    This will make your life easier, it prevents the case where you either copy
    it wrong, or things get moved around.
*/
#define COLLECTION_CHILD        \
    char *name;                 \
    print_node node_printer;    \
    sizeof_node get_sizeof;     \
    print_list list_printer;    \
    size_t vert_len;            \

// This is the main linked list class; you get a subsect of the list.
struct _collection_t {
    COLLECTION_CHILD;
};

#endif
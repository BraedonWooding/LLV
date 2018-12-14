#ifndef LLV_COLLECTION_SKELETON_H
#define LLV_COLLECTION_SKELETON_H

#include <stdlib.h>

typedef struct _collection_t *Collection;

typedef void(*fn_print_node)(void *n, wchar_t **buf, size_t size, size_t len, size_t offset);
typedef size_t(*fn_sizeof_node)(void *n);
typedef void(*fn_print_list)(Collection collection);

/*
    This will make your life easier, it prevents the case where you either copy
    it wrong, or things get moved around.

    @REFACTOR: maybe we should use a sub-struct there are `__attribute__(inline)`
               if we want to use them like we currently use them.
               I however also think accessing them through the sub-struct is fine
               since they don't have a clear 'thing'
*/
#define COLLECTION_CHILD            \
    char *name;                     \
    fn_print_node node_printer;     \
    fn_sizeof_node get_sizeof;      \
    fn_print_list list_printer;     \

// Currently we expect everyone to fit in the COLLECTION_NODE struct properly
// Really we should do the same as above!  and have fake node defined as just that
// then have a fake linked list node that is defined with the next pointer.

// This is the main linked list class; you get a subsect of the list.
struct _collection_t {
    COLLECTION_CHILD;
};

#endif
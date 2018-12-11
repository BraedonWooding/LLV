#ifndef LLV_COLLECTION_HELPER
#define LLV_COLLECTION_HELPER

#include <string.h>
#include <math.h>

#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"

#define NULL_NODE "X"
#define NULL_NODE_LEN strlen(NULL_NODE)
#define SPACES_ON_SIDE (2)
#define EXTRA_WIDTH (4)

// make sure your struct can downcast to this
// don't malloc this!
typedef struct _fake_node_t {
    char *ptr;
    Data data; // the data type
    TypeTag data_tag; // the corresponding tag;
    struct _fake_node_t *next;
} *FakeNode;

size_t list_sizeof(void *n);

void list_print_node(void *n, char **buf, size_t size, size_t len, size_t offset);

void list_print_general(Collection list, size_t len, size_t count, FakeNode forwards,
                FakeNode backwards, int stop, size_t *node_sizes, char *after_node,
                char *start_of_list, char *end_of_list, char *ellipses, FakeNode head,
                char *collection_name);

#endif
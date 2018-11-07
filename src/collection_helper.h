#ifndef LLV_COLLECTION_HELPER
#define LLV_COLLECTION_HELPER

#include <string.h>
#include <math.h>

#include "types/collection_skeleton.h"
#include "types/shared_types.h"

#define NULL_NODE "X"

// make sure your struct can downcast to this
// don't malloc this!
typedef struct _fake_node_t {
    Data data; // the data type
    TypeTag data_tag; // the corresponding tag;
    struct _fake_node_t *next;
    char *ptr;
} *FakeNode;

size_t sizeof_uint(unsigned long long int n);

size_t sizeof_int(long long int n);

size_t general_sizeof_list(void *n);

void general_print_node_list(void *n, char **buf, size_t size, size_t len, size_t offset);

void general_print_list(Collection list, size_t len, size_t count, FakeNode forwards,
                FakeNode backwards, int stop, size_t *node_sizes, char *after_node,
                char *start_of_list, char *end_of_list, char *ellipses, FakeNode head);

int print_out_nodes(Collection list, FakeNode begin, FakeNode end, char **buf, size_t *node_sizes,
                         size_t *offset, char *after_node, int size_offset);

void write_str_center_incr(char **buf, size_t *offset, size_t len,
                           char *str, size_t str_len);

#endif
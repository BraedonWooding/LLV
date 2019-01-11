#ifndef LLV_ARRAY_H
#define LLV_ARRAY_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _array_t *Array;
typedef struct _array_data_t *ArrayNode;

struct _array_data_t {
    char *ptr;
    Data data; // the data type
    TypeTag data_tag; // the corresponding tag;
};

struct _array_t {
    COLLECTION_CHILD;
    ArrayNode data;
    size_t len;
};

Array array_new(char *name, size_t size);

void array_free(Array array);

ArrayNode array_at(Array array, size_t index);

struct _array_data_t array_new_node(Data data, TypeTag type);

void array_resize(Array array, size_t new_size);

void array_set(Array array, size_t index, struct _array_data_t node);

size_t array_length(Array array);

#endif
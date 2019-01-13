#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "../../include/collections/array.h"
#include "../../include/helper.h"
#include "../list_helper.h"
#include "../../lib/obsidian.h"
#include "../array_helper.h"

void array_print(Collection c);

Array array_new(char *name, size_t size) {
    Array array = malloc_with_oom(sizeof(struct _array_t), "Array");
    array->data = malloc_with_oom(sizeof(struct _array_data_t) * size, "Array Nodes");
    array->len = size;
    array->parent.get_sizeof = list_sizeof;
    array->parent.node_printer = list_print_node;
    array->parent.list_printer = array_print;
    array->parent.name = name;
    return array;
}

void array_free(Array array) {
    free(array->data);
    free(array);
}

ArrayNode array_at(Array array, size_t index) {
    if (array->len <= index) return NULL;
    return &array->data[index];
}

struct _array_data_t array_new_node(Data data, TypeTag type) {
    return (struct _array_data_t) {
        .data = data,
        .ptr = NULL,
        .data_tag = type,
    };
}

void array_resize(Array array, size_t new_size) {
    array->data = realloc(array->data, sizeof(struct _array_data_t) * new_size);
    array->len = new_size;
}

void array_set(Array array, size_t index, struct _array_data_t node) {
    obs_assert(array->len, >, index);
    array->data[index] = node;
}

size_t array_length(Array array) {
    return array->len;
}

void array_print(Collection c) {
    Array array = (Array)c;
    print_array_like(c, "Array", (FakeArrayNode)array->data, array->len);
}

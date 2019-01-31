#include "../../include/collections/array.h"
<<<<<<< HEAD
#include "../list_helper.h"
=======
#include "../../include/helper.h"
#include "../list_helper.h"
#include "../../lib/obsidian.h"
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
#include "../array_helper.h"

void array_print(Collection c);

Array array_new(char *name, int size) {
    Array array = (Array)malloc_with_oom(sizeof(struct _array_t), "Array");
    array->data = (ArrayNode)malloc_with_oom(sizeof(struct _array_data_t) * size, "Array Nodes");
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

ArrayNode array_at(Array array, int index) {
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

void array_resize(Array array, int new_size) {
    array->data = (ArrayNode)realloc(array->data, sizeof(struct _array_data_t) * new_size);
    array->len = new_size;
}

void array_set(Array array, int index, struct _array_data_t node) {
    assert_msg(array->len > index, "array:array_set %d is out of bounds max "
                                   "index is %d", index, array->len - 1);
    array->data[index] = node;
}

int array_length(Array array) {
    return array->len;
}

void array_print(Collection c) {
    Array array = (Array)c;
    print_array_like(c, "Array", (FakeArrayNode)array->data, array->len);
}

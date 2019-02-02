#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "../../include/collections/list.h"
#include "../../include/helper.h"
#include "../list_helper.h"
#include "../array_helper.h"

void list_print(Collection c);

int int_pow(int base, int exp) {
    int res = base;
    for (int i = 1; i < exp; i++) res *= base;
    return res;
}

List list_new(char *name) {
    List list = (List)malloc_with_oom(sizeof(struct _list_t), "List");
    list->cur_len = list->max_len = 0;
    list->data = NULL;
    list->grow_function = poly_grow_function;
    list->factor = 2.0;
    list->parent.get_sizeof = list_sizeof;
    list->parent.node_printer = list_print_node;
    list->parent.list_printer = list_print;
    list->parent.name = name;
    return list;
}

void list_free(List list) {
    if (list->data != NULL) free(list->data);
    free(list);
}

int linear_grow_function(int old_len, int min_new_len, double factor) {
    assert_msg(factor >= 0.0, "list:linear_grow_function factor (%d) must be "
                              "positive or 0", factor);
    if (factor == 0 || old_len == 0) return min_new_len;
    return (min_new_len + factor) * factor / factor;
}

int poly_grow_function(int old_len, int min_new_len, double factor) {
    assert_msg(factor >= 0.0, "list:poly_grow_function factor (%d) must be "
                              "positive or 0", factor);
    if (factor == 0 || old_len == 0) return min_new_len;
    int new_len = old_len;
    while (new_len <= min_new_len) new_len *= factor;
    return new_len;
}

int exponential_grow_function(int old_len, int min_new_len, double factor) {
    assert_msg(factor >= 0.0, "list:exponential_grow_function factor (%d) must "
                              "be positive or 0", factor);
    if (factor == 0 || old_len == 0) return min_new_len;
    int new_len = old_len;
    while (new_len <= min_new_len) new_len = int_pow(new_len, factor);
    return new_len;
}

ListNode list_at(List list, int index) {
    if (list->cur_len >= index) return NULL;
    return &list->data[index];
}

struct _list_data_t list_new_node(Data data, TypeTag type) {
    return (struct _list_data_t) {
        .data = data,
        .data_tag = type,
        .ptr = NULL,
    };
}

void list_clear(List list, int release_memory) {
    list->cur_len = 0;
    if (release_memory) {
        list->max_len = 0;
        free(list->data);
    }
}

void list_push_back(List list, struct _list_data_t node) {
    if (list->cur_len == list->max_len) list_reserve(list, list->cur_len + 1);
    list->data[list->cur_len++] = node;
}

/*
    @Refactor: These two are similar enough that we need a middleman func.
    Also don't like the early return probably an else would be better.
*/
void list_insert_after(List list, int index, struct _list_data_t node) {
    assert_msg(list->cur_len > index, "list:list_insert_after %d is out of "
                                      "bounds, the max index"
                                      "is %d\n", index, list->cur_len - 1);
    if (index == list->cur_len - 1) {
        list_push_back(list, node);
        return;
    }

    if (list->cur_len == list->max_len) list_reserve(list, list->cur_len + 1);
    memmove(list->data + index + 2, list->data + index + 1,
        sizeof(struct _list_data_t) * (list->cur_len - 1 - index));
    list->data[index + 1] = node;
    list->cur_len++;
}

void list_insert_before(List list, int index, struct _list_data_t node) {
    assert_msg(list->cur_len > index, "list:list_insert_after %d is out of "
                                      "bounds, the max index"
                                      "is %d\n", index, list->cur_len);
    if (index == list->cur_len) {
        list_push_back(list, node);
        return;
    }

    if (list->cur_len == list->max_len) list_reserve(list, list->cur_len + 1);
    memmove(list->data + index + 1, list->data + index,
        sizeof(struct _list_data_t) * (list->cur_len - 1 - index));
    list->data[index] = node;
    list->cur_len++;
}

void list_remove(List list, int index) {
    assert_msg(list->cur_len > index, "list:list_insert_after %d is out of "
                                      "bounds, the max index"
                                      "is %d\n", index, list->cur_len - 1);
    // easy remove
    if (index == list->cur_len - 1) {
        list->cur_len--;
    } else {
        // shuffle back one post index
        memmove(list->data + index, list->data + index + 1,
            sizeof(struct _list_data_t) * (list->cur_len - 1 - index));
        list->cur_len--;
    }
}

int list_is_empty(List list) {
    return list->cur_len == 0;
}

int list_length(List list) {
    return list->cur_len;
}

int list_capacity(List list) {
    return list->max_len;
}

void list_reserve(List list, int len) {
    if (list->max_len >= len) return;
    int new_len = list->grow_function(list->max_len, len, list->factor);
    list->max_len = new_len;
    list->data = (ListNode)realloc(list->data, sizeof(struct _list_data_t) * list->max_len);
}

void list_print(Collection c) {
    List list = (List)c;
    print_array_like(c, "List", (FakeArrayNode)list->data, list->cur_len);
}

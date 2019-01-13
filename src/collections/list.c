#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/list.h"
#include "../../include/helper.h"
#include "../list_helper.h"
#include "../../lib/obsidian.h"
#include "../array_helper.h"

void list_print(Collection c);

List list_new(char *name) {
    List list = malloc_with_oom(sizeof(struct _list_t), "List");
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

size_t linear_grow_function(size_t old_len, size_t min_new_len, double factor) {
    obs_assert(factor, >=, 0.0);
    if (factor == 0 || old_len == 0) return min_new_len;
    return (min_new_len + factor) * factor / factor;
}

size_t poly_grow_function(size_t old_len, size_t min_new_len, double factor) {
    obs_assert(factor, >=, 0.0);
    if (factor == 0 || old_len == 0) return min_new_len;
    size_t new_len = old_len;
    while (new_len <= min_new_len) new_len *= factor;
    return new_len;
}

size_t exponential_grow_function(size_t old_len, size_t min_new_len, double factor) {
    obs_assert(factor, >=, 0.0);
    if (factor == 0 || old_len == 0) return min_new_len;
    size_t new_len = old_len;
    while (new_len <= min_new_len) new_len = pow(new_len, factor);
    return new_len;
}

ListNode list_at(List list, size_t index) {
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

void list_clear(List list, bool release_memory) {
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
void list_insert_after(List list, size_t index, struct _list_data_t node) {
    obs_assert(list->cur_len, >, index);
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

void list_insert_before(List list, size_t index, struct _list_data_t node) {
    obs_assert(list->cur_len, >=, index);
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

void list_remove(List list, size_t index) {
    obs_assert(list->cur_len, >, index);
    // easy remove
    if (index == list->cur_len - 1) {
        list->cur_len--;
        return;
    } else {
        // shuffle back one post index
        memmove(list->data + index, list->data + index + 1,
            sizeof(struct _list_data_t) * (list->cur_len - 1 - index));
    }
}

bool list_is_empty(List list) {
    return list->cur_len == 0;
}

size_t list_length(List list) {
    return list->cur_len;
}

size_t list_capacity(List list) {
    return list->max_len;
}

void list_reserve(List list, size_t len) {
    if (list->max_len >= len) return;
    size_t new_len = list->grow_function(list->max_len, len, list->factor);
    list->max_len = new_len;
    list->data = realloc(list->data, sizeof(struct _list_t) * list->max_len);
}

void list_print(Collection c) {
    List list = (List)c;
    print_array_like(c, "List", (FakeArrayNode)list->data, list->cur_len);
}

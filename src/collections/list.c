#include <stdio.h>
<<<<<<< HEAD
#include <string.h>
=======
#include <math.h>
#include <string.h>
#include <stdbool.h>
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
#include <stdint.h>

#include "../../include/collections/list.h"
#include "../../include/helper.h"
#include "../list_helper.h"
<<<<<<< HEAD
=======
#include "../../lib/obsidian.h"
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
#include "../array_helper.h"

void list_print(Collection c);

<<<<<<< HEAD
int int_pow(int base, int exp) {
    int res = base;
    for (int i = 1; i < exp; i++) res *= base;
    return res;
}

List list_new(char *name) {
    List list = (List)malloc_with_oom(sizeof(struct _list_t), "List");
=======
List list_new(char *name) {
    List list = malloc_with_oom(sizeof(struct _list_t), "List");
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
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

<<<<<<< HEAD
int linear_grow_function(int old_len, int min_new_len, double factor) {
    assert_msg(factor >= 0.0, "list:linear_grow_function factor (%d) must be "
                              "positive or 0", factor);
=======
size_t linear_grow_function(size_t old_len, size_t min_new_len, double factor) {
    obs_assert(factor, >=, 0.0);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
    if (factor == 0 || old_len == 0) return min_new_len;
    return (min_new_len + factor) * factor / factor;
}

<<<<<<< HEAD
int poly_grow_function(int old_len, int min_new_len, double factor) {
    assert_msg(factor >= 0.0, "list:poly_grow_function factor (%d) must be "
                              "positive or 0", factor);
    if (factor == 0 || old_len == 0) return min_new_len;
    int new_len = old_len;
=======
size_t poly_grow_function(size_t old_len, size_t min_new_len, double factor) {
    obs_assert(factor, >=, 0.0);
    if (factor == 0 || old_len == 0) return min_new_len;
    size_t new_len = old_len;
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
    while (new_len <= min_new_len) new_len *= factor;
    return new_len;
}

<<<<<<< HEAD
int exponential_grow_function(int old_len, int min_new_len, double factor) {
    assert_msg(factor >= 0.0, "list:exponential_grow_function factor (%d) must "
                              "be positive or 0", factor);
    if (factor == 0 || old_len == 0) return min_new_len;
    int new_len = old_len;
    while (new_len <= min_new_len) new_len = int_pow(new_len, factor);
    return new_len;
}

ListNode list_at(List list, int index) {
=======
size_t exponential_grow_function(size_t old_len, size_t min_new_len, double factor) {
    obs_assert(factor, >=, 0.0);
    if (factor == 0 || old_len == 0) return min_new_len;
    size_t new_len = old_len;
    while (new_len <= min_new_len) new_len = pow(new_len, factor);
    return new_len;
}

ListNode list_at(List list, size_t index) {
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
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

<<<<<<< HEAD
void list_clear(List list, int release_memory) {
=======
void list_clear(List list, bool release_memory) {
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
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
<<<<<<< HEAD
void list_insert_after(List list, int index, struct _list_data_t node) {
    assert_msg(list->cur_len > index, "list:list_insert_after %d is out of "
                                      "bounds, the max index"
                                      "is %d\n", index, list->cur_len - 1);
=======
void list_insert_after(List list, size_t index, struct _list_data_t node) {
    obs_assert(list->cur_len, >, index);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
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

<<<<<<< HEAD
void list_insert_before(List list, int index, struct _list_data_t node) {
    assert_msg(list->cur_len > index, "list:list_insert_after %d is out of "
                                      "bounds, the max index"
                                      "is %d\n", index, list->cur_len);
=======
void list_insert_before(List list, size_t index, struct _list_data_t node) {
    obs_assert(list->cur_len, >=, index);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
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

<<<<<<< HEAD
void list_remove(List list, int index) {
    assert_msg(list->cur_len > index, "list:list_insert_after %d is out of "
                                      "bounds, the max index"
                                      "is %d\n", index, list->cur_len - 1);
=======
void list_remove(List list, size_t index) {
    obs_assert(list->cur_len, >, index);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
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

<<<<<<< HEAD
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
=======
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
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
}

void list_print(Collection c) {
    List list = (List)c;
    print_array_like(c, "List", (FakeArrayNode)list->data, list->cur_len);
}

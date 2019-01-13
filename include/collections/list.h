#ifndef LLV_VECTOR_H
#define LLV_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _list_t *List;
typedef struct _list_data_t *ListNode;

typedef size_t(*fn_growth_factor)(size_t old_len, size_t min_new_len, double factor);

struct _list_data_t {
    char *ptr;
    Data data; // the data type
    TypeTag data_tag; // the corresponding tag;
};

struct _list_t {
    struct _collection_t parent;
    ListNode data;
    size_t cur_len;
    size_t max_len;
    fn_growth_factor grow_function;
    double factor;
};

/*
    Create a new list with the given name.
    By default uses `poly_grow_function` with factor 2.
*/
List list_new(char *name);

/*
    Frees the list.
*/
void list_free(List list);

/*
    Grows the list linearly.
    i.e. increases by factor each time.
    if factor == 0 then just is always min_new_len i.e. no growth.
    for factor == 2 => 2 -> 4 -> 6 -> 8 -> 10...
*/
size_t linear_grow_function(size_t old_len, size_t min_new_len, double factor);

/*
    Grows the list polynomially.
    i.e. doubles each time if factor == 2.
    2 -> 4 -> 8...
*/
size_t poly_grow_function(size_t old_len, size_t min_new_len, double factor);

/*
    y = x^factor such that min_new_len <= y(old_len).
    Will effectively find the smallest multiple such that it'll fit the min_new_len.

    2 -> 4 -> 16... for a factor of 2
    2 -> 8 -> 512... for a factor of 3
*/
size_t exponential_grow_function(size_t old_len, size_t min_new_len, double factor);

/*
    Get the node at the index given.
*/
ListNode list_at(List list, size_t index);

/*
    Creates a new list node.
    Used for the other functions
*/
struct _list_data_t list_new_node(Data data, TypeTag type);

/*
    Makes sure the list can handle len amount of data.
*/
void list_reserve(List list, size_t len);

/*
    Clears list.  Will release memory if release_memory is true.
*/
void list_clear_list(List list, bool release_memory);

/*
    Pushes node to back of list, growing if needed.
*/
void list_push_back(List list, struct _list_data_t node);

/*
    Inserts node after index given.
*/
void list_insert_after(List list, size_t index, struct _list_data_t node);

/*
    Inserts node before index given.
*/
void list_insert_before(List list, size_t index, struct _list_data_t node);

/*
    Removes node at index.
*/
void list_remove(List list, size_t index);

/*
    Returns true if list is empty.
*/
bool list_is_empty(List list);

/*
    Returns length of list.
*/
size_t list_length(List list);

/*
    Returns capacity of list.
*/
size_t list_capacity(List list);

#endif
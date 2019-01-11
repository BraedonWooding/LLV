#ifndef LLV_VECTOR_H
#define LLV_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _vector_t *Vector;
typedef struct _vec_data_t *VecNode;

typedef size_t(*fn_growth_factor)(size_t old_len, size_t min_new_len, double factor);

struct _vec_data_t {
    char *ptr;
    Data data; // the data type
    TypeTag data_tag; // the corresponding tag;
};

struct _vector_t {
    COLLECTION_CHILD;
    VecNode data;
    size_t max_len;
    size_t cur_len;
    fn_growth_factor grow_function;
    double factor;
};

/*
    Create a new vector with the given name.
    By default uses `poly_grow_function` with factor 2.
*/
Vector vec_new(char *name);

/*
    Frees the vector.
*/
void vec_free(Vector vec);

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
VecNode vec_at(Vector vec, size_t index);

/*
    Creates a new vec node.
    Used for the other functions
*/
struct _vec_data_t vec_new_node(Data data, TypeTag type);

/*
    Makes sure the vector can handle len amount of data.
*/
void vec_reserve(Vector vec, size_t len);

/*
    Clears list.  Will release memory if release_memory is true.
*/
void vec_clear_list(Vector vec, bool release_memory);

/*
    Pushes node to back of list, growing if needed.
*/
void vec_push_back(Vector vec, struct _vec_data_t node);

/*
    Inserts node after index given.
*/
void vec_insert_after(Vector vec, size_t index, struct _vec_data_t node);

/*
    Inserts node before index given.
*/
void vec_insert_before(Vector vec, size_t index, struct _vec_data_t node);

/*
    Removes node at index.
*/
void vec_remove(Vector vec, size_t index);

/*
    Returns true if vector is empty.
*/
bool vec_is_empty(Vector vec);

/*
    Returns length of vector.
*/
size_t vec_length(Vector vec);

/*
    Returns capacity of vector.
*/
size_t vec_capacity(Vector vec);

#endif
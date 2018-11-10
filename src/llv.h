#ifndef LLV_H
#define LLV_H

#include <stdlib.h>
#include <stdbool.h>

#include "types/shared_types.h"
#include "types/collection_skeleton.h"

/* This is the main header to import, by importing this you get everything
   you need to build an example/demonstration of LLV except for the collections.
*/

#define GET_DATA(data)              \
    _Generic((data),                \
                  int: data_int,    \
                char*: data_str,    \
               double: data_flt,    \
              default: data_any     \
    )(data)

#define GET_DATA_TYPE(data)         \
    _Generic((data),                \
                  int: INTEGER,     \
                char*: STRING,      \
               double: FLOAT,       \
              default: ANY          \
    )

#define NEW_NODE(type, data) type##_new_node(GET_DATA(data), GET_DATA_TYPE(data))

#define SET_PTR(node, value) if (node != NULL) node->ptr = value;
#define UNSET_PTR(node) SET_PTR(node, NULL)

void setup(size_t time, bool clear, bool incl_ptrs_single);

void update(int number, ...);
void update_wait(void);
void fmt_update(char *fmt, ...);
void clear_screen(void);

void attach_ptr(void *node, char *ptr);
bool deattach_ptr(void *node, char *ptr);

void print_out_single_box(void *node, print_node printer, sizeof_node sizeof_n, int height);
void print_out_single_box_using_defaults(void *node, Collection c);

#endif
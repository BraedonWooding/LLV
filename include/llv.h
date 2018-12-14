#ifndef LLV_H
#define LLV_H

#include <stdlib.h>
#include <stdbool.h>

#include "helper.h"
#include "types/shared_types.h"
#include "types/collection_skeleton.h"

/* This is the main header to import, by importing this you get everything
   you need to build an example/demonstration of LLV except for the collections.
*/

#ifdef MODERN_C
#define GET_DATA(data)              \
    _Generic((data),                \
        long: data_int, \
        long long: data_int, \
        unsigned int: data_int, \
        unsigned long: data_int, \
        unsigned long long: data_int, \
        int: data_int, \
        float: data_flt, \
        double: data_flt, \
        long double: data_flt, \
        char: data_int, \
        short: data_int, \
        unsigned short: data_int, \
        char *: data_str, \
        bool: data_int, \
        default: data_any \
    )(data)

#define GET_DATA_TYPE(data)         \
    _Generic((data),                \
        long: INTEGER, \
        long long: INTEGER, \
        unsigned int: INTEGER, \
        unsigned long: INTEGER, \
        unsigned long long: INTEGER, \
        int: INTEGER, \
        float: FLOAT, \
        double: FLOAT, \
        long double: FLOAT, \
        char: INTEGER, \
        short: INTEGER, \
        unsigned short: INTEGER, \
        char *: STRING, \
        bool: INTEGER, \
        default: ANY \
    )

#define NEW_NODE(type, data) type##_new_node(GET_DATA(data), GET_DATA_TYPE(data))
#endif

#define SET_PTR(node, value) if ((node) != NULL) (node)->ptr = value;
#define UNSET_PTR(node) SET_PTR(node, NULL)

void update(int number, ...);
void update_wait(void);
void fmt_update(char *fmt, ...);
void clear_screen(void);

void attach_ptr(void *node, char *ptr);
bool deattach_ptr(void *node, char *ptr);

void print_out_single_box(void *node, fn_print_node printer, fn_sizeof_node sizeof_n, int height);
void print_out_single_box_using_defaults(void *node, Collection c);

#endif
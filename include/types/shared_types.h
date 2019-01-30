#ifndef LLV_SHARED_TYPES_H
#define LLV_SHARED_TYPES_H

#include <stdlib.h>

#if defined(__STDC__)
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)  /* C11 */
#       define MODERN_C
#   endif
#endif

#ifndef MODERN_C
#   warning "Not Modern C, behaviour is not guaranteed"
#endif

// This is used everywhere

// all the possible data types
typedef enum _type_tag {
    FLOAT,
    STRING,
    INTEGER,
    ANY,        // if set for a list its equivalent to 'none'
} TypeTag;

typedef union _data {
    long long int_data;
    void *any_data;
    char *str_data;
    double flt_data;
} Data;

typedef struct _data_node_t {
    char *visual_ptr;
    Data data;
    TypeTag tag;
} DataNode;

Data data_int(long long data);
Data data_flt(double data);
Data data_str(char *data);
Data data_any(void *data);

#endif /* LLV_SHARED_TYPES_H */

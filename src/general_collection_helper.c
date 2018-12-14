#include "general_collection_helper.h"

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <wchar.h>

#include "../include/helper.h"

size_t sizeof_uint(unsigned long long int n) {
    return (n == 0) ? 1 : log10(n) + 1;
}

size_t sizeof_int(long long int n) {
    if (n < 0) {
        return sizeof_uint(n * -1) + 1;
    } else {
        return sizeof_uint(n);
    }
}

size_t sizeof_flt(double flt) {
    // @TODO: make this better
    wchar_t buf[MAX_SIZE_FLT];
    return swprintf(buf, MAX_SIZE_FLT, L"%." MAX_PRECISION_FLT_PRINT_STR "g", flt);
}

size_t sizeof_data(Data data, TypeTag tag) {
    size_t node_size = 0;
    switch (tag) {
        case FLOAT: {
            node_size = sizeof_flt(data.flt_data);
        } break;
        case STRING: {
            node_size = strlen(data.str_data);
        } break;
        case INTEGER: {
            node_size = sizeof_int(data.int_data);
        } break;
        case ANY: {
            node_size = sizeof_uint((size_t)data.any_data);
        } break;
    }
    return node_size;
}

void write_str_center_incr(wchar_t **buf, size_t *offset, size_t len,
                           wchar_t *str, size_t str_len) {
    write_str_center_of_buf(buf, *offset, len, str, str_len);
    *offset += str_len;
}

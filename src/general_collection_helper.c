#include "general_collection_helper.h"

#include <string.h>
#include <stdio.h>
#include <wchar.h>

#include "../include/helper.h"

int log10_int(int num) {
    int log = 0;
    while (num >= 10) {
        num /= 10;
        log++;
    }
    return log;
}

int sizeof_uint(unsigned long long int n) {
    return (n == 0) ? 1 : log10_int(n) + 1;
}

int sizeof_int(long long int n) {
    if (n < 0) {
        return sizeof_uint(n * -1) + 1;
    } else {
        return sizeof_uint(n);
    }
}

int sizeof_flt(double flt) {
    wchar_t buf[MAX_SIZE_FLT];
    int res = swprintf(buf, MAX_SIZE_FLT, L"%.5g", flt);
    if (res < 0) {
        fprintf(stderr, "BUG IN FLT PRINTER\n");
        abort();
    }
    return (int)res;
}

int sizeof_data(Data data, TypeTag tag) {
    int node_size = 0;
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

void write_str_center_incr(wchar_t **buf, int *offset, int len,
                           wchar_t *str, int str_len) {
    write_str_center_of_buf(buf, *offset, len, str, str_len);
    *offset += str_len;
}

#include "../../include/types/shared_types.h"

Data data_int(long long data) {
    return (Data){.int_data = data};
}

Data data_flt(double data) {
    return (Data){.flt_data = data};
}

Data data_str(char *data) {
    return (Data){.str_data = data};
}

Data data_any(void *data) {
    return (Data){.any_data = data};
}
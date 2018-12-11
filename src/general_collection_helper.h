#ifndef LLV_GENERAL_COLLECTION_HELPER
#define LLV_GENERAL_COLLECTION_HELPER

#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"

#define MAX_SIZE_FLT                    (8)
#define MAX_PRECISION_FLT_PRINT         (6)
#define MAX_PRECISION_FLT_PRINT_STR     "6"

size_t sizeof_uint(unsigned long long int n);

size_t sizeof_int(long long int n);

void write_str_center_incr(char **buf, size_t *offset, size_t len,
                           char *str, size_t str_len);

size_t sizeof_data(Data data, TypeTag tag);

#endif
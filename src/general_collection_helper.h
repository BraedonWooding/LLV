#ifndef LLV_GENERAL_COLLECTION_HELPER
#define LLV_GENERAL_COLLECTION_HELPER

#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"

#define MAX_SIZE_FLT                    (15)

int sizeof_uint(unsigned long long int n);

int sizeof_int(long long int n);

void write_str_center_incr(wchar_t **buf, int *offset, int len,
                           wchar_t *str, int str_len);

int sizeof_data(Data data, TypeTag tag);

#endif /* LLV_GENERAL_COLLECTION_HELPER */

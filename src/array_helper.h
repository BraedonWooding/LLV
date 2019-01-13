#ifndef LLV_ARRAY_HELPER
#define LLV_ARRAY_HELPER

#include <string.h>
#include <math.h>

#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"
#include "../include/helper.h"

/*
    valid values for WIDTH
    - (0) no spaces i.e. `|a||b||c||d|`
    - (x > 0) spaces i.e. for x == 1 `|a| |b| |c| |d|`
    - (x == -1) no spaces and no double bars i.e. `|a|b|c|d|`
*/
#define WIDTH (1)
#define ELLIPSES (L" [ ... ]")
#define ELLIPSES_LEN (wcslen(ELLIPSES))

struct _fake_array_data_t {
   char *ptr;
   Data data; // the data type
   TypeTag data_tag; // the corresponding tag;
};

typedef struct _fake_array_data_t *FakeArrayNode;

void print_array_like(Collection c, char *collection_type, FakeArrayNode data, size_t len);

#endif

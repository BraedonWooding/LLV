#include "../include/collections/array.h"
#include "../include/llv.h"

#define BIG_STRING "this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size."

int main(void) {
    Array array = array_new("Too big", 1);
    array_set(array, 0, NEW_NODE(array, BIG_STRING));
    update(1, array); // should fail with error
    array_free(array);
}

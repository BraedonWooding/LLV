#include "../include/collections/array.h"
#include "../include/llv.h"

#define BIG_STRING "this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size."

int main(void) {
    Array array = array_new("Too big", 3);
    array_set(array, 0, NEW_NODE(array, "small"));
    array_set(array, 1, NEW_NODE(array, "small as well"));
    array_set(array, 2, NEW_NODE(array, BIG_STRING));
    update(1, array);
    array_free(array);
}

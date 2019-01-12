#include "../include/collections/dll.h"
#include "../include/llv.h"

#define BIG_STRING "this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size."

int main(void) {
    DLL list = dll_new("Too big");
    dll_append(list, NEW_NODE(dll, BIG_STRING));
    update(1, list);
    dll_free(list);
}

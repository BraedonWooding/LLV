#include "../include/collections/list.h"
#include "../include/llv.h"

#define BIG_STRING "this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size."

int main(void) {
    List list = list_new("Too big");
    list_push_back(list, NEW_NODE(list, BIG_STRING));
    update(1, list);
    list_free(list);
}
#include "../include/collections/ll.h"
#include "../include/llv.h"

#define BIG_STRING "this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size."

int main(void) {
    LL list = ll_new("Too big");
    ll_append(list, NEW_NODE(ll, 3));
    ll_append(list, NEW_NODE(ll, 1));
    ll_append(list, NEW_NODE(ll, BIG_STRING));
    update(1, list);
    ll_free(list);
}

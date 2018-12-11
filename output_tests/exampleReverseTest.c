/*
    This is just the example code
    This will be ran to generate the `.out`
*/

#define DEFAULT_TERMINAL_WIDTH (80)

#include "../include/collections/LL.h"
#include "../include/llv.h"
#include "../include/helper.h"

int main(void) {
    setup(1, false, false);
    LL to_reverse = LL_new("to_reverse");
    for (int i = 0; i < 100; i += 1) {
        LL_insert_after(to_reverse, NEW_NODE(LL, i), to_reverse->tail);
    }

    LL reversed = LL_new("reversed");
    attach_ptr(&to_reverse->head, "cur");

    while (!LL_is_empty(to_reverse)) {
        LL_Node n = to_reverse->head;
        fmt_update("%l %l %s %n", to_reverse, reversed, "\nRemoving the following\n", n);
        to_reverse->head = n->next;
        LL_push(reversed, n);
        fmt_update("%s %l %l", "Resulting in;", to_reverse, reversed);
    }

    deattach_ptr(&to_reverse->head, "cur");
    LL_free(to_reverse);
    LL_free(reversed);
}

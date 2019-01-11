#include <LLV/collections/ll.h>
#include <LLV/llv.h>
#include <LLV/helper.h>

int main(void) {
    LL to_reverse = ll_new("to_reverse");
    for (int i = 0; i < 100; i += 1) {
        ll_insert_after(to_reverse, NEW_NODE(ll, i), to_reverse->tail);
    }

    LL reversed = ll_new("reversed");
    attach_ptr(&to_reverse->head, "cur");

    while (!ll_is_empty(to_reverse)) {
        LL_Node n = to_reverse->head;
        fmt_update("%l %l %s %n", to_reverse, reversed, "\nRemoving the following\n", n);
        to_reverse->head = n->next;
        ll_push(reversed, n);
        fmt_update("%s %l %l", "Resulting in;", to_reverse, reversed);
    }

    deattach_ptr(&to_reverse->head, "cur");
    ll_free(to_reverse);
    ll_free(reversed);
}

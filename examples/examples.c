#include "../src/collections/LL.h"
#include "../src/llv.h"
#include "../src/helper.h"

#include <stdio.h>

#define TEST(x) \
    _Generic((x), int: "INT", float: "FLT")

void usage_exit(void) {
    printf("Use like; ./program <X> where X is a number and from the following list\n"
           "0: Reverse standard\n"
           "1: Even numbers in a linked list\n");
    exit(1);
}

void reverse_standard(void) {
    // This examples reverses a list
    // not in place though, by using another list
    // memory wise the same.
    // new_LL pushes it to a global LL for update to work on.
    LL to_reverse = LL_new("to_reverse", INTEGER);
    for (int i = 0; i < 10; i++) {
        // while you could do LL_new_node((Data){i}, INTEGER)
        // you can also just use this great macro, which is honestly beautiful
        LL_insert_after(to_reverse, NEW_NODE(LL, i), to_reverse->tail);
    }

    LL reversed = LL_new("reversed", INTEGER);
    SET_PTR(to_reverse->head, "cur");

    // tons of helper functions such as remove_node, insert_node and so on
    while (!LL_is_empty(to_reverse)) {
        // you can do whatever you want to the list
        LL_Node n = to_reverse->head;
        fmt_update("%l %l %s %n", to_reverse, reversed, "\nRemoving the following\n", n);
        to_reverse->head = n->next;
        UNSET_PTR(n);
        SET_PTR(to_reverse->head, "cur");
        LL_push(reversed, n);
        // a simpler update looks like;
        // `update(2, to_reverse, reversed);`
        // a more complex one is like
        fmt_update("%s %l %l", "Resulting in;", to_reverse, reversed);
    }
    LL_free(to_reverse);
    LL_free(reversed);
}

int main(int argc, char *argv[]) {
    if (argc != 2) usage_exit();

    // Cross platform clearing
    system(CLEAR_SCREEN);

    switch (atoi(argv[1])) {
        case 0: {
            reverse_standard();
            break;
        }
        default: usage_exit();
    }
}
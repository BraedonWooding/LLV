#include "../include/collections/LL.h"
#include "../include/llv.h"
#include "../include/helper.h"
#include "../include/collections/Array.h"

#include <stdio.h>
#include <stdlib.h>

void usage_exit(void) {
    printf("Use like; ./program <X> where X is a number and from the following list\n"
           "0: Reverse standard\n"
           "1: Bubble sort array\n");
    exit(1);
}

void bubble_sort(void) {
    Array array = array_new("to sort", 10);
    int *values = ((int[]){1000, 24, 12, 2, 9, 90, 110, 54, 1, 10});
    for (int i = 0; i < 10; i++) {
        array_set(array, i, NEW_NODE(array, values[i]));
    }

    fmt_update("%l", array);

    ArrayNode n1 = NULL;
    ArrayNode n2 = NULL;
    attach_ptr(&n1, "a");
    attach_ptr(&n2, "b");

    bool changes = true;
    while (changes) {
        changes = false;
        for (int i = 0; i < 9; i++) {
            n1 = array_at(array, i);
            n2 = array_at(array, i + 1);
            fmt_update("%l", array);

            // swap
            if (n1->data.int_data > n2->data.int_data) {
                struct _array_data_t tmp = *n1;
                array_set(array, i, *n2);
                array_set(array, i + 1, tmp);
                changes = true;

                n2 = n1;
                n1 = array_at(array, i + 1);
                fmt_update("%l %s", array, "b < a therefore swap");
            }
        }
    }

    deattach_ptr(&n1, "a");
    deattach_ptr(&n2, "b");
    fmt_update("%s %l", "All sorted", array);
    array_free(array);
}

void reverse_standard(void) {
    // This examples reverses a list
    // not in place though, by using another list
    // memory wise the same.
    // new_LL pushes it to a global LL for update to work on.
    LL to_reverse = LL_new("to_reverse");
    for (int i = 0; i < 1000; i += 10) {
        // while you could do LL_new_node((Data){i}, INTEGER)
        // you can also just use this great macro, which is honestly beautiful
        LL_insert_after(to_reverse, NEW_NODE(LL, (double)i * 10000000.0 + 0.5), to_reverse->tail);
    }

    LL reversed = LL_new("reversed");
    // i've included the longer way as well for your 'enjoyment'
    // SET_PTR(to_reverse->head, "cur");
    attach_ptr(&to_reverse->head, "cur");

    // tons of helper functions such as remove_node, insert_node and so on
    while (!LL_is_empty(to_reverse)) {
        // you can do whatever you want to the list
        LL_Node n = to_reverse->head;
        fmt_update("%l %l %s %n", to_reverse, reversed, "\nRemoving the following\n", n);
        to_reverse->head = n->next;
        // UNSET_PTR(n);
        // SET_PTR(to_reverse->head, "cur");
        LL_push(reversed, n);
        // a simpler update looks like;
        // `update(2, to_reverse, reversed);`
        // a more complex one is like
        fmt_update("%s %l %l", "Resulting in;", to_reverse, reversed);
    }

    // if you care about freeing
    deattach_ptr(&to_reverse->head, "cur");
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
        } break;
        case 1: {
            bubble_sort();
        } break;
        default: usage_exit();
    }
}
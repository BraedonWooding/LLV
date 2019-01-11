#include <LLV/collections/ll.h>
#include <LLV/collections/dll.h>
#include <LLV/collections/array.h>
#include <LLV/llv.h>

#include <locale.h>

#include <stdio.h>
#include <stdlib.h>

void usage_exit(void) {
    printf("Use like; ./program <X> where X is a number and from the following list\n"
           "0: Reverse standard\n"
           "1: Bubble sort array\n"
           "2: DLL add\n");
    exit(1);
}

void dll_add(void) {
    DLL list = dll_new("Adding elements to list");
    fmt_update("%l", list);
    int *values = ((int[]){100, 24, 345, 59, 393, 292, 12, 500, 800, 900, 40044040});
    for (int i = 0; i < 11; i++) {
        dll_append(list, NEW_NODE(dll, values[i]));
        fmt_update("%l", list);
    }
    fmt_update("%l", list);
}

void bubble_sort(void) {
    Array array = array_new("Sorting List", 7);
    int *values = ((int[]){1, 1000, 9, 90, 110, 54, 10});
    for (int i = 0; i < 7; i++) {
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
        for (int i = 0; i < 6; i++) {
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
    LL to_reverse = ll_new("Original List");
    for (int i = 0; i < 10; i++) {
        // while you could do ll_new_node((Data){i}, INTEGER)
        // you can also just use this great macro, which is honestly beautiful
        ll_insert_after(to_reverse, NEW_NODE(ll, (double)i * 10000000.0), to_reverse->tail);
    }

    LL reversed = ll_new("Reversed");
    // i've included the longer way as well for your 'enjoyment'
    LL_Node cur = to_reverse->head;
    // SET_PTR(cur, "cur");
    attach_ptr(&cur, "cur");

    // tons of helper functions such as remove_node, insert_node and so on
    while (cur != NULL) {
        // UNSET_PTR(cur);
        ll_push(reversed, ll_new_node(cur->data, cur->data_tag));
        // a simpler update looks like;
        // `update(2, to_reverse, reversed);`
        // a more complex one is like
        cur = cur->next;
        // SET_PTR(cur, "cur");
        fmt_update("%s %l %l", "Resulting in;", to_reverse, reversed);
    }

    // if you care about freeing
    deattach_ptr(&to_reverse->head, "cur");
    ll_free(to_reverse);
    ll_free(reversed);
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
        case 2: {
            dll_add();
        } break;
        default: usage_exit();
    }
}
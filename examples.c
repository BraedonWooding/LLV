#include "linked_list.h"

#include <stdio.h>

// for extern, if not here then it has default value of false
bool clear_on_update = true;
size_t ms_step = 1000; // if set to 0 manual stepping

int main(void) {
    // Cross platform clearing
    system(CLEAR_SCREEN);

    // This examples reverses a list
    // not in place though, by using another list
    // memory wise the same.
    // new_LL pushes it to a global LL for update to work on.
    LL to_reverse = new_LL(false, "to_reverse");
    for (int i = 0; i < 20; i++) push_value(to_reverse, i);
    LL reversed = new_LL(false, "reversed");
    // tons of helper functions such as remove_node, insert_node and so on
    while (!is_empty(to_reverse)) {
        // you can do whatever you want to the list
        Node n = to_reverse->head;
        to_reverse->head = n->next;
        // you can print out individual nodes to get a nicer graphic
        printf("Removing\n");
        print_single_node(n);

        // example of some of the helper functions
        if (is_empty(reversed)) push_node(reversed, n);
        else                    insert_before_node(reversed, n, reversed->head);
        update();
    }

    // you can change clear on update at any time
    printf("ENDED\n");
    clear_on_update = false;
    update();
    // Frees every new_LL created
    // just a nice lil helper can also call it individually on each
    // like free_list(to_reverse)
    free_all_lists();
}
// Support both ways (installed to system and local single source header)
#if defined(__has_include) && __has_include("llv.h")
#   include "llv.h"
#else
#   include <LLV/llv.h>
#   include <LLV/collections/array.h>
#   include <LLV/collections/dll.h>
#   include <LLV/collections/list.h>
#   include <LLV/collections/ll.h>
#   include <LLV/collections/queue.h>
#   include <LLV/collections/stack.h>
#endif

#include <locale.h>

#include <stdio.h>
#include <stdlib.h>

// Look at examples.incl for the code

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

void usage_exit(void) {
    printf("Use like; ./program <X> where X is a number and from the following list\n"
           "0" ": " "Reverse standard" "\n"
           "1" ": " "Sort List" "\n"
           "2" ": " "DLL insert from user prompt" "\n"
           "3" ": " "Removing odd elements" "\n"
           "4" ": " "User input then removing even nodes" "\n"
           "5" ": " "User input then removing out of bounds" "\n"
           );
    exit(1);
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
        ll_append(to_reverse, NEW_NODE(ll, (double)i * 10000000.0));
    }

    LL reversed = ll_new("Reversed");
    // i've included the longer way as well for your 'enjoyment'
    LL_Node cur = to_reverse->head;
    // SET_PTR(cur, "cur");
    attach_ptr(&cur, "cur");

    fmt_update("%l %l", to_reverse, reversed);

    // tons of helper functions such as remove_node, insert_node and so on
    while (cur != NULL) {
        // UNSET_PTR(cur);
        ll_push(reversed, ll_new_node(cur->data, cur->data_tag));
        // a simpler update looks like;
        // `update(2, to_reverse, reversed);`
        // a more complex one is like
        cur = cur->next;
        // SET_PTR(cur, "cur");
        fmt_update("%l %l", to_reverse, reversed);
    }

    // if you care about freeing
    deattach_ptr(&cur, "cur");
    ll_free(to_reverse);
    ll_free(reversed);
}

void bubble_sort(void) {
    int values[] = ((int[]){1, 1000, 9, 90, 110, 54, 10});

    Array array = array_new("Sorting List", ARRAY_LEN(values));
    for (int i = 0; i < ARRAY_LEN(values); i++) {
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
        for (int i = 0; i < ARRAY_LEN(values) - 1; i++) {
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

void dll_insert_user(void) {
    DLL list = dll_new("Adding elements to list from user prompt");
    int res;
    while(true) {
        // make sure to cast buffers like this - same reason as scanf
        // %i for input then `i` for int
        // valid ones are: %ii (int) %il (long) %ic (char) %is (char buf - no allocation) %if (double)
        // note: they must all be at the end!!
        fmt_update("%l %s %ii", list, "Ctrl+D to stop input", &res);
        if (feof(stdin)) break;
        dll_append(list, NEW_NODE(dll, res));
    }
    clearerr(stdin);

    fmt_update("%l", list);
    dll_free(list);
}

void wiki_eg1(void) {
    LL list = ll_new("Example List");
    update(1, list); // prints out the list (which will be empty)
    for (int i = 0; i < 10; i++) {
        // NEW_NODE is a special generic macro that will create data
        // correctly with the proper type tag
        // in this case it is ll_new_node((Data){i}, INTEGER)
        ll_append(list, NEW_NODE(ll, i));
        update(1, list);
    }

    // LL and DLL have `head` and `tail`
    LL_Node cur = list->head;
    // this means that as you update cur
    // It'll display what box it currently is pointing to
    // cur can be null as well in that case it'll point to nothing
    attach_ptr(&cur, "cur");
    update(1, list);
    // sidenote: you can also call SET_PTR(cur, "cur"); and UNSET_PTR(cur);
    // if for a specific reason you don't want it to track and update
    // also remember to deattach_ptr(&cur); at the end if you care about freeing
    while (cur != NULL) {
       // read data as int, can also do GET(cur, int)
       // note typesafe so know what you are doing!!
       if (cur->data.int_data % 2 != 0) {
           // remove node
           LL_Node tmp = cur;
           cur = cur->next;
           ll_remove_node(list, tmp); // this also returns the removed node
           // sidenote: you could write this out yourself
           // by carrying around a prev ptr (or calling ll_find_prev)
           // fmt_update is a fancy printf basically
           fmt_update("%l %s %n", list, "Removed Node:", tmp);
           ll_free_node(tmp); // if you care
       } else {
           cur = cur->next;
           update(1, list);
       }
    }

    fmt_update("%s %l", "Finished Removing Odd Nodes", list);
    deattach_ptr(&cur, "cur");
    ll_free(list);
}

void wiki_eg2(void) {
    LL list = ll_new("Example List");
    int res;
    while(true) {
        // make sure to cast buffers like this - same reason as scanf
        // %i for input then `i` for int
        // valid ones are: %ii (int) %il (long) %ic (char) %is (char buf - no allocation) %if (double)
        // note: they must all be at the end!!
        fmt_update("%l %s %ii", list, "Ctrl+D to stop entering in nodes", &res);
        if (feof(stdin)) break;
        ll_append(list, NEW_NODE(ll, res));
    }

    clearerr(stdin);

    // LL and DLL have `head` and `tail`
    LL_Node cur = list->head;
    // this means that as you update cur
    // It'll display what box it currently is pointing to
    // cur can be null as well in that case it'll point to nothing
    attach_ptr(&cur, "cur");
    update(1, list);
    // sidenote: you can also call SET_PTR(cur, "cur"); and UNSET_PTR(cur);
    // if for a specific reason you don't want it to track and update
    // also remember to deattach_ptr(&cur); at the end if you care about freeing
    while (cur != NULL) {
       // read data as int, can also do GET(cur, int)
       // note typesafe so know what you are doing!!
       if (cur->data.int_data % 2 == 0) {
           // remove node
           LL_Node tmp = cur;
           cur = cur->next;
           ll_remove_node(list, tmp); // this also returns the removed node
           // sidenote: you could write this out yourself
           // by carrying around a prev ptr (or calling ll_find_prev)
           // fmt_update is a fancy printf basically
           fmt_update("%l %s %n", list, "Removed Node:", tmp);
           ll_free_node(tmp); // if you care
       } else {
           cur = cur->next;
       }
    }

    fmt_update("%s %l", "Finished Removing Even Nodes", list);
    deattach_ptr(&cur, "cur");
    ll_free(list);
}

void wiki_eg3(void) {
    LL list = ll_new("Example List");
    double res;
    while(true) {
        fmt_update("%l %s %if", list, "Ctrl+D when finished", &res);
        // feof can be used to check if stdin is currently EOF
        // @NOTE: to support tests we also check res == 0
        if (feof(stdin)) break;
#ifdef TESTING
        if (res == 0) break;
#endif
        // NEW_NODE picks up on it being a double and doesn't care!!! yay
        ll_append(list, NEW_NODE(ll, res));
    }

    // if stdin is EOF then clear it so we can keep reading from it
    clearerr(stdin);

    // change from the wiki example due to commas being weird in macros
    double lower; double upper;
    // no need to have lists here we just want a lower and upper bound but why not
    fmt_update("%l %s %if %if", list, "Enter lower and then upper bound", &lower, &upper);

    LL_Node cur = list->head;
    attach_ptr(&cur, "cur");
    update(1, list);
    while (cur != NULL) {
       if (cur->data.flt_data < lower || cur->data.flt_data > upper) {
           LL_Node tmp = cur;
           cur = cur->next;
           ll_remove_node(list, tmp);
           fmt_update("%l %s %n", list, "Removed Node:", tmp);
           ll_free_node(tmp);
       } else {
           cur = cur->next;
       }
    }

    fmt_update("%s %l", "Finished Removing Nodes out of bounds", list);
    deattach_ptr(&cur, "cur");
    ll_free(list);
}

int main(int argc, char *argv[]) {
    if (argc != 2) usage_exit();
    int num = atoi(argv[1]);

    switch (num) {
        case 0: reverse_standard(); break;
        case 1: bubble_sort(); break;
        case 2: dll_insert_user(); break;
        case 3: wiki_eg1(); break;
        case 4: wiki_eg2(); break;
        case 5: wiki_eg3(); break;
        default: usage_exit(); break;
    }
}
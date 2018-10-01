#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "linked_list.h"

#ifdef WINDOWS_COMPATIBILITY
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>

    // https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
    #if _POSIX_C_SOURCE >= 199309L
        #include <time.h>
    #else
        #include <unistd.h>
        #define POSIX_LEGACY
    #endif
#endif

#define BUF_SIZE 1024

typedef struct _terminal_size {
    int width;
    int height;
} terminalSize;

linkedList created_lists = { 0, 0, true };

void *malloc_with_oom(size_t size, char *obj_name);
Node find_prev(LL list, Node at);

void sleep_ms(size_t ms) {
    #ifdef WINDOWS_COMPATIBILITY
        Sleep(ms);
    #elif defined(POSIX_LEGACY)
        // has timeout as opposed to usleep
        struct timeval tv = { .tv_sec = ms / 1000, .tv_usec = (ms % 1000) * 1000 };
        select(0, NULL, NULL, NULL, &tv);
    #else
        struct timespec ts = { .tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000 };
        nanosleep(&ts, NULL);
    #endif
}

bool is_empty(LL list) {
    return list->head == NULL;
}

LL new_LL(bool doubly_linked, char *name) {
    LL list = malloc_with_oom(sizeof(linkedList), "Linked List");
    list->doubly_linked = doubly_linked;
    list->head = list->tail = NULL;
    list->name = name;
    Node new_node = malloc_with_oom(sizeof(node), "Node");
    new_node->data = list;
    new_node->next = new_node->prev = NULL;
    push_node(&created_lists, new_node);
    return list;
}

Node new_node(long long val) {
    Node new_node = malloc_with_oom(sizeof(node), "Node");
    new_node->prev = new_node->next = NULL;
    new_node->value = val;
    return new_node;
}

void free_list(LL list) {
    for (Node cur = created_lists.head; cur != NULL;) {
        if (cur->data == list) {
            Node temp = remove_node(&created_lists, cur);
            free(temp);
            break;
        } else {
            cur = cur->next;
        }
    }

    for (Node cur = list->head; cur != NULL;) {
        Node temp = cur;
        cur = cur->next;
        free(temp);
    }
    free(list);
}

void free_all_lists(void) {
    // while it may seem O(N^2) it actually is just O(N)
    //  (where N is max of list/created_list)
    // as the free_list for loop will always break on the first iteration
    while (!is_empty(&created_lists)) {
        free_list(created_lists.head->data);
    }
}

void clear_list(LL list) {
    for (Node cur = list->head; cur != NULL;) {
        Node temp = cur;
        cur = cur->next;
        free(temp);
    }
    list->head = list->tail = NULL;
}

void push_node(LL list, Node node) {
    node->next = node->prev = NULL;
    if (is_empty(list)) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        if (list->doubly_linked) node->prev = list->tail;
        list->tail = node;
    }
}

void push_value(LL list, long long value) {
    push_node(list, new_node(value));
}

void insert_before_node(LL list, Node node, Node at) {
    node->next = node->prev = NULL;
    Node at_prev = NULL;
    if (at == list->head) {
        list->head = node;
    } else {
        Node at_prev = find_prev(list, at);
        // at_prev -> at => at_prev -> node -> at
        at_prev->next = node;
    }
    node->next = at;
    if (list->doubly_linked) {
        // at_prev <-> node <-> at
        node->prev = at_prev;
        at->prev = node;
    }
}

void insert_before_node_value(LL list, long long value, Node at) {
    insert_before_node(list, new_node(value), at);
}

void insert_after_node(LL list, Node node, Node at) {
    node->next = node->prev = NULL;
    Node post_at = at->next;
    // at -> post_at => at -> node -> post_at;
    node->next = post_at;
    at->next = node;
    if (list->doubly_linked) {
        // at <-> node <-> post_at;
        if (post_at != NULL) post_at->prev = node;
        node->prev = at;
    }
    if (post_at == NULL) list->tail = node;
}

void insert_after_node_value(LL list, long long value, Node at) {
    insert_after_node(list, new_node(value), at);
}

Node remove_node(LL list, Node node) {
    if (node == list->head) {
        list->head = node->next;
        if (list->head != NULL) list->head->prev = NULL;
    } else {
        Node at_prev = find_prev(list, node);
        at_prev->next = node->next;
        if (list->doubly_linked && list->tail != node) {
            node->next->prev = at_prev;
        }
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }
    return node;
}

void update(void) {
    // clear screen cross compatibility
    for (Node cur = created_lists.head; cur != NULL; cur = cur->next) {
        printf("\nLIST: %s\n\n", ((LL)cur->data)->name);
        print_list((LL)cur->data);
    }

    if (created_lists.head != NULL) {
        if (ms_step != 0) {
            sleep_ms(ms_step);
        } else {
            getchar();
        }
    }

    if (clear_on_update) system(CLEAR_SCREEN);
}

Node find_prev(LL list, Node at) {
    if (list->doubly_linked) {
        // O(1)
        return at->prev;
    } else {
        // O(n)
        bool found;
        Node cur;
        for (cur = list->head; cur != NULL; cur = cur->next) {
            if (cur->next == at) return cur;
        }
        printf("Couldn't find node %lld in list: %s\n", at->value, list->name);
        exit(1);
    }
}

terminalSize get_terminal_size() {
    int cols, rows;
    #ifdef WINDOWS_COMPATIBILITY
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        cols = w.ws_col;
        rows = w.ws_row;
    #endif
    if (cols == 0) {
        cols = 80;
        rows = 80;
    }

    terminalSize size = { cols, rows };
    return size;
}

/* Example output
    ==========    ====
    |12345678| -> |54| -> X
    ==========    ====
*/

size_t size_of_node(Node n) {
    // + 2 for the '|' on either side
    if (n->value == 0) return 3;
    return ceil(log10(n->value + 1)) + 2;
}

void print_repeating(char character, size_t count) {
    for (int i = 0; i < count; i++) putchar(character);
}

void print_single_node(Node n) {
    size_t node_size = size_of_node(n);
    // print top
    print_repeating('=', node_size);
    printf("     \n");
    // print bars and digit and pionter
    printf("|%lld| -> x\n", n->value);
    // print bottom
    print_repeating('=', node_size);
    printf("     \n");
}

void print_list(LL list) {
    if (is_empty(list)) {
        printf("x (Empty List)\n");
        return;
    }

    // Find out size of terminal
    terminalSize size = get_terminal_size();
    linkedList begin = {0, 0, true};
    linkedList end = {0, 0, true};
    begin.head = begin.tail = list->head;
    end.head = end.tail = list->tail;
    // case of a single node
    if (list->head->next == NULL) {
        print_single_node(list->head);
        return;
    }

    // confirm they have space for atleast the first and last
    // note: we can use a buffer here because if you have a terminal
    // that is more than 1024 on the left and on the right i.e. 2048 in total
    // then you are insane (and so just increase buf_size)...
    int first[BUF_SIZE] = {size_of_node(list->head)};
    int last[BUF_SIZE] = {size_of_node(list->tail)};
    int first_len = 1;
    int last_len = 1;
    // 4 for each arrow (only one, since we can't presume elipses) + 1 for 'x'
    int total = first[0] + 25;

    /* i.e.
        ===           ===
        |1| -> ... -> |2| -> x
        ===           ===
    */
    // +3 for elipses + 4 for arrow
    if (size.width < total + last[0] + 11) {
        // too much of a pain to automatically resize
        fprintf(stderr, "Terminal not big enough to fit the minimum, resize and rerun\n");
        exit(1);
    }

    // Now we can atleast print the minimum we want to push on the left side
    // Then we can push back on the right side
    Node head_cur = list->head->next;
    while (total < size.width && head_cur != NULL) {
        // just for those crazy enough to try, don't want any buffer overflows over here
        assert(first_len < BUF_SIZE);
        first[first_len++] = size_of_node(head_cur);
        total += first[first_len - 1] + 4; // for the arrow
        head_cur = head_cur->next;
    }
    if (head_cur != NULL || total > size.width) {
        // just remove the last one we did to go back into the green
        total -= first[--first_len];
        first[first_len] = 0;

        // @OPTIMISE, could use the back traversal of DLL
        // not a universal, since we also support just standard LL
        // so I don't have it suported here.

        // remove from the 'first' and add to the 'last' effectively
        Node cur_tail = list->tail;
        // noting we also have to increase last_count by the difference from first to last
        while (first_len > last_len) {
            first_len--;
            total -= first[first_len] + 4;
            first[first_len] = 0;
            cur_tail = find_prev(list, cur_tail);
            last[last_len++] = size_of_node(cur_tail);
            total += last[last_len - 1];
        }
    } else {
        // fit entire list so remove the total from last
        last_len = 0;
        last[0] = 0;
    }

    // time to actually print out list
    // print out the top
    for (int i = 0; i < first_len; i++) {
        print_repeating('=', first[i]);
        printf("    "); // for arrow
    }
    if (last_len > 0) {
        // print elipses + arrow (just the top)
        printf("       ");
        for (int i = last_len - 1; i >= 0; i--) {
            print_repeating('=', last[i]);
            printf("    "); // for arrow
        }
    }
    // print end (arrow + 'x')
    printf("     \n");
    // print numbers
    Node cur = list->head;
    for (int i = 0; i < first_len; i++, cur = cur->next) {
        printf("|%lld| -> ", cur->value);
    }
    if (last_len > 0) {
        printf("... -> ");
        Node cur = list->tail;
        for (int i = 0; i < last_len - 1; i++, cur = find_prev(list, cur));
        for (int i = 0; i < last_len; i++, cur = cur->next) {
            printf("|%lld| -> ", cur->value);
        }
    }
    printf("x\n");
    // print bottoms
    for (int i = 0; i < first_len; i++) {
        print_repeating('=', first[i]);
        printf("    "); // for arrow
    }
    if (last_len > 0) {
        // print elipses + arrow (just the top)
        printf("       ");
        for (int i = last_len - 1; i >= 0; i--) {
            print_repeating('=', last[i]);
            printf("    "); // for arrow
        }
    }
    printf("     \n");
}

void *malloc_with_oom(size_t size, char *obj_name) {
    void *obj = malloc(size);
    if (obj == NULL) {
        printf("Error: OOM, can't allocate %lu bytes for %s\n", size, obj_name);
        exit(1);
    }
    return obj;
}

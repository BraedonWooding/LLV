#include "llv.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "list_helper.h"
#include "helper.h"

typedef struct _ll_visual_t {
    void *node;
    char *ptr_name;
    struct _ll_visual_t *next;
} *VisualNode;

static VisualNode visual_ptrs = NULL;

void update_collection(Collection c);

static size_t sleep_time = 0;
static bool clear_on_update = true;
static bool include_ptrs_on_single = false;

void attach_ptr(void *node, char *ptr) {
    VisualNode new = malloc_with_oom(sizeof(struct _ll_visual_t), "FakeNode");
    new->node = node;
    new->ptr_name = ptr;
    new->next = visual_ptrs;
    visual_ptrs = new;
}

bool deattach_ptr(void *node, char *ptr) {
    VisualNode prev = NULL;
    for (VisualNode n = visual_ptrs; n != NULL; prev = n, n = n->next) {
        if (n->node == node && !strcmp(n->ptr_name, ptr)) {
            if (prev == NULL)   visual_ptrs = n->next;
            else                prev->next = n->next;
            free(n);
            return true;
        }
    }
    return false;
}

void update_wait(void) {
    // wait either a set amount of time or till key press
    if (sleep_time > 0) sleep_ms(sleep_time);
    else {
        printf("\nType enter to continue...\n");
        getchar();
    }
}

void update_ptrs(bool remove) {
    for (VisualNode n = visual_ptrs; n != NULL; n = n->next) {
        if (n->node != NULL) {
            // yeh... ouchy, basically all collection nodes
            // can implicitly downcast to just a char* so we can grab
            // a ptr to a node (which is a ptr to a ptr to a struct) and cast
            char **downcast = *(char***)n->node;
            if (downcast != NULL) *downcast = remove ? NULL : n->ptr_name;
        } else {
            // if that's NULL there is no chance it is going to go un-null
            deattach_ptr(n->node, n->ptr_name);
        }
    }
}

void fmt_update(char *fmt, ...) {
    if (clear_on_update) clear_screen();
    va_list list;
    va_start(list, fmt);
    update_ptrs(false);
    for (char *a = fmt; *a != '\0'; a++) {
        if (*a == '%') {
            switch (*(++a)) {
                case 'n': {
                    // single node
                    FakeNode n = va_arg(list, FakeNode);

                    print_out_single_box(n, list_print_node, list_sizeof,
                                         DEFAULT_PRINT_HEIGHT);
                } break;
                case 'l': {
                    Collection c = va_arg(list, Collection);
                    update_collection(c);
                } break;
                case 's': {
                    char *str = va_arg(list, char *);
                    printf("%s\n", str);
                } break;
            }
        }
    }
    update_ptrs(true);
    va_end(list);
    update_wait();
}

void clear_screen(void) {
    system(CLEAR_SCREEN);
}

void setup(size_t time, bool clear, bool incl_ptrs_single) {
    sleep_time = time;
    clear_on_update = clear;
    include_ptrs_on_single = incl_ptrs_single;
}

void update(int number, ...) {
    if (clear_on_update) clear_screen();
    va_list list;
    va_start(list, number);
    update_ptrs(false);
    for (int i = 0; i < number; i++) {
        Collection c = va_arg(list, Collection);
        update_collection(c);
    }
    va_end(list);
    update_ptrs(true);
    update_wait();
}

void print_out_single_box(void *node, print_node printer, sizeof_node sizeof_n, int height) {
    char **buf = malloc_with_oom(sizeof(char *) * (height + DEFAULT_PTR_HEIGHT), "Single");
    size_t count = sizeof_n(node);
    for (int i = 0; i < height + DEFAULT_PTR_HEIGHT; i++) {
        buf[i] = malloc_with_oom(sizeof(char) * (count + 1), "Single");
        for (int j = 0; j < count; j++) buf[i][j] = ' ';
        buf[i][count] = '\0';
    }
    printer(node, buf, count, height, 0);

    for (int i = 0; i < height; i++) {
        puts(buf[i]);
        free(buf[i]);
    }
    for (int i = height; i < DEFAULT_PTR_HEIGHT + height; i++) {
        if (include_ptrs_on_single) {
            bool found_non_space = false;
            for (int j = 0; j < count; j++) {
                if (buf[i][j] != ' ') {
                    found_non_space = true;
                    break;
                }
            }
            if (found_non_space) printf("%s\n", buf[i]);
        }
        free(buf[i]);
    }
    free(buf);
}

void print_out_single_box_using_defaults(void *node, Collection c) {
    print_out_single_box(node, c->node_printer, c->get_sizeof, c->vert_len);
}

void update_collection(Collection c) {
    c->list_printer(c);
}

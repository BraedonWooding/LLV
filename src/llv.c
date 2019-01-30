#include "../include/llv.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <assert.h>

#include "list_helper.h"
#include "env_var.h"

typedef struct _ll_visual_t {
    void *node;
    char *ptr_name;
    struct _ll_visual_t *next;
} *VisualNode;

VisualNode visual_ptrs = NULL;

void update_collection(Collection c);

void print_border(void) {
    terminalSize size = get_terminal_size();
    for (int i = 0; i < size.width; i++) printf("%lc", BOX_HORIZONTAL);
    printf("\n");
}

void attach_ptr(void *node, char *ptr) {
    VisualNode visual_node = (VisualNode)malloc_with_oom(sizeof(struct _ll_visual_t), "FakeNode");
    visual_node->node = node;
    visual_node->ptr_name = ptr;
    visual_node->next = visual_ptrs;
    visual_ptrs = visual_node;
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

void input_wait(char *fmt, va_list args) {
    // sleep time doesn't effect this
    for (char *a = fmt; *a != '\0'; a++) {
        if (*a == '%') {
            assert((*(++a) == 'i') && "%i must be the last of the fmt");
            switch (*(++a)) {
                case 'i': {
                    printf("Enter an integer: ");
                    int *i = va_arg(args, int*);
                    scanf("%d", i);
                } break;
                case 'l': {
                    printf("Enter a long integer: ");
                    long *i = va_arg(args, long*);
                    scanf("%ld", i);
                } break;
                case 'c': {
                    printf("Enter a character: ");
                    char *c = va_arg(args, char*);
                    scanf("%c", c);
                } break;
                case 's': {
                    printf("Enter a string: ");
                    char *str = va_arg(args, char*);
                    scanf("%s", str);
                } break;
                case 'f': {
                    printf("Enter a floating pointer number: ");
                    double *flt = va_arg(args, double*);
                    scanf("%lf", flt);
                } break;
                default: {
                    printf("Unknown %c", *a);
                }
            }
        }
    }
}

void update_wait(void) {
    // wait either a set amount of time or till key press
    if (get_sleep_time() > 0) sleep_ms(get_sleep_time());
    else {
        printf("\nType enter to continue...\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
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
    if (clear_on_update()) clear_screen();
    print_border();
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
                                         get_print_height());
                } break;
                case 'l': {
                    Collection c = va_arg(list, Collection);
                    update_collection(c);
                } break;
                case 's': {
                    char *str = va_arg(list, char *);
                    printf("%s\n", str);
                } break;
                case 'i': {
                    a--; // go back to `%`
                    update_ptrs(true);
                    print_border();
                    input_wait(a, list);
                    va_end(list);
                } return;
            }
        }
    }
    update_ptrs(true);
    va_end(list);
    print_border();
    update_wait();
}

void clear_screen(void) {
    system(CLEAR_SCREEN);
}

void update(int number, ...) {
    if (clear_on_update()) clear_screen();
    print_border();
    va_list list;
    va_start(list, number);
    update_ptrs(false);
    for (int i = 0; i < number; i++) {
        Collection c = va_arg(list, Collection);
        update_collection(c);
    }
    va_end(list);
    update_ptrs(true);
    print_border();
    update_wait();
}

void print_out_single_box(void *node, fn_print_node printer, fn_sizeof_node sizeof_n, int height) {
    wchar_t **buf = (wchar_t**)malloc_with_oom(sizeof(wchar_t *) * (height + get_ptr_height()), "Single");
    int count = sizeof_n(node);
    for (int i = 0; i < height + get_ptr_height(); i++) {
        buf[i] = (wchar_t*)malloc_with_oom(sizeof(wchar_t) * (count + 1), "Single");
        for (int j = 0; j < count; j++) buf[i][j] = ' ';
        buf[i][count] = '\0';
    }
    printer(node, buf, count, height, 0);

    for (int i = 0; i < height; i++) {
        printf("%ls\n", buf[i]);
        free(buf[i]);
    }
    for (int i = height; i < get_ptr_height() + height; i++) {
        if (include_ptrs_on_single()) {
            bool found_non_space = false;
            for (int j = 0; j < count; j++) {
                if (buf[i][j] != ' ') {
                    found_non_space = true;
                    break;
                }
            }
            if (found_non_space) printf("%ls\n", buf[i]);
        }
        free(buf[i]);
    }
    free(buf);
}

void print_out_single_box_using_defaults(void *node, Collection c) {
    print_out_single_box(node, c->node_printer, c->get_sizeof, get_print_height());
}

void update_collection(Collection c) {
    c->list_printer(c);
}

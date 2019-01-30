#include "list_helper.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>

#include "../include/helper.h"
#include "general_collection_helper.h"
#include "env_var.h"

#define PTR_SYMBOL select_char_unicode(L'⌃', L'^')

void print_bounding_box(wchar_t **buf, int offset, int len, int width);
void print_out_nodes(Collection list, FakeNode begin, FakeNode end, wchar_t **buf, int *node_sizes,
                         int *offset, wchar_t *after_node, int starting_size);

int list_sizeof(void *n) {
    FakeNode node = (FakeNode)n;
    return sizeof_data(node->data, node->data_tag) + EXTRA_WIDTH;
}

/*
    @REFACTORING NOTICE:
    - I'll attempt this soon, it isn't just magic numbers to fix I have a few crazy ideas
      to how to fit even more pointers by placing above/below as well this could show nicer pointers
      then having just below.
*/
void print_ptr(wchar_t **buf, int len, int count, char *ptr_name, int ptr_len, int offset) {
    // @REFACTORING
    // this is just a very early version
    // I'm going to update this once I figure out visual ptrs a bit more
    // a lot of this is hand waivey and magic variables
    int i = 0;
    while (i < get_ptr_height() && buf[len + i][offset] == PTR_SYMBOL) i++;

    if (i == get_ptr_height()) {
        // we can fit two if we are clever
        // we want to make sure though that we haven't done this before
        // and find a level that we can fit the second one on
        i = 0;
        while (i < get_ptr_height() && buf[len + i][offset + count] != ' ') i++;
        if (i == get_ptr_height()) {
            // oh no, we can't print this out :(
            // @TODO: figure out if we want to try something else
            return;
        }

        for (int j = offset + 3; j < offset + count; j++) buf[i + len][j] = ' ';
        // first truncate old ptr
        // next we want to fit ourselves in we will push ourselves one/two longer
        // then the actual minimum square, but that's okay, min is 5 and ptr is always
        // ` -> ` so we can fit it
        int max = ptr_len + 1 > 7 ? 7 : ptr_len + 1;
        for (int j = offset + 4; j < offset + max; j++) buf[i + len][j] = ptr_name[j - offset - 4];
        buf[i + len][offset + 3] = PTR_SYMBOL;
    } else {
        int max = ptr_len + 1 > 5 ? 5 : ptr_len + 1;
        for (int j = offset + 1; j < offset + max; j++) buf[i + len][j] = ptr_name[j - offset - 1];
        buf[i + len][offset] = PTR_SYMBOL;
    }
}

void list_print_node(void *n, wchar_t **buf, int size, int len, int offset) {
    FakeNode node = (FakeNode)n;
    print_bounding_box(buf, offset, len, size);
    wchar_t *text_to_print;
    switch (node->data_tag) {
        case FLOAT: {
            text_to_print = (wchar_t*)malloc_with_oom(sizeof(wchar_t) * (size - EXTRA_WIDTH / 2 + 1), "Text to print");
            swprintf(text_to_print, size - EXTRA_WIDTH / 2 + 1, L"%.5g", node->data.flt_data);
        } break;
        case STRING: {
            int len = strlen(node->data.str_data);
            text_to_print = (wchar_t*)malloc_with_oom(len * sizeof(wchar_t), "Text to print");
            for (int i = 0; i < len; i++) text_to_print[i] = node->data.str_data[i];
        } break;
        case INTEGER: {
            // note: we are including the '\0' room just to make nothing weird happen
            // with snprintf, but we will completely ignore it below in memcpy.
            text_to_print = (wchar_t*)malloc_with_oom(sizeof(wchar_t) * (size - EXTRA_WIDTH / 2 + 1), "Text to print");
            swprintf(text_to_print, size - EXTRA_WIDTH / 2, L"%lld", node->data.int_data);
        } break;
        case ANY: {
            text_to_print = (wchar_t*)malloc_with_oom(sizeof(wchar_t) * (size - EXTRA_WIDTH / 2 + 1), "Text to print");
            swprintf(text_to_print, size - EXTRA_WIDTH / 2, L"%p", node->data.any_data);
        } break;
    }
    if (node->ptr != NULL) print_ptr(buf, len, size, node->ptr, strlen(node->ptr), offset);

    // our sizes are always buffered by '4'
    // a sprintf or similar is just going to give us nasty '\0'
    memcpy(buf[len / 2] + EXTRA_WIDTH/2 + offset, text_to_print, sizeof(wchar_t) * (size - EXTRA_WIDTH));
    free(text_to_print);
}

void print_bounding_box(wchar_t **buf, int offset, int len, int width) {
    write_str_repeat_char_vert(buf, offset, BOX_VERT, len, 0);
    write_str_repeat_char_vert(buf, offset, BOX_VERT, len, width - 1);
    write_str_repeat_char(buf[0], offset + 1, BOX_HORIZONTAL, width - 2);
    buf[0][offset] = BOX_TOP_LEFT;
    buf[0][offset + width - 1] = BOX_TOP_RIGHT;
    write_str_repeat_char(buf[len - 1], offset, BOX_HORIZONTAL, width);
    buf[len - 1][offset] = BOX_BOTTOM_LEFT;
    buf[len - 1][offset + width - 1] = BOX_BOTTOM_RIGHT;
}

void print_out_nodes(Collection list, FakeNode begin, FakeNode end, wchar_t **buf, int *node_sizes,
                         int *offset, wchar_t *after_node, int starting_size) {
    int i = starting_size;
    FakeNode n;
    for (n = begin; n != end; n = n->next, i++) {
        list_print_node(n, buf, node_sizes[i], get_print_height(), *offset);
        *offset += node_sizes[i];
        if (n->next != end) {
            write_str_center_incr(buf, offset, get_print_height(), after_node, wcslen(after_node));
        }
    }
}

void list_print_general(Collection list, int len, int count, FakeNode forwards,
                FakeNode backwards, int stop, int *node_sizes, wchar_t *after_node,
                wchar_t *start_of_list, wchar_t *end_of_list, wchar_t *ellipses, FakeNode head,
                char *collection_name) {
    terminalSize size = get_terminal_size();
    assert_msg(size.width >= count, "list_helper:list_print_general, size.width (%d) must be >= count (%d)\n", size.width, count);

    // now we have sizes we can allocate buffer and prepare to print list
    // probably going to be a few characters bigger than we need but no harm no foul
    wchar_t **buf = (wchar_t**)malloc_with_oom(sizeof(wchar_t*) * (get_print_height() + get_ptr_height()), "Buffer");
    for (int i = 0; i < get_print_height() + get_ptr_height(); i++) {
        buf[i] = (wchar_t*)malloc_with_oom((count + 1) * sizeof(wchar_t), "Buffer");
        for (int j = 0; j < count; j++) buf[i][j] = ' ';
        buf[i][count] = '\0';
    }

    int offset = 0;
    FakeNode forward_stop = forwards;
    // if everything in the list fits on page
    bool everything_fits = forward_stop == NULL;

    if (head == NULL) {
        write_str_center_incr(buf, &offset, get_print_height(), NULL_NODE, wcslen(NULL_NODE));
    } else {
        write_str_center_incr(buf, &offset, get_print_height(), start_of_list, wcslen(start_of_list));
        print_out_nodes(list, head, forward_stop, buf, node_sizes, &offset, after_node, 0);

        if (!everything_fits) {
            write_str_center_incr(buf, &offset, get_print_height(), after_node, wcslen(after_node));
            write_str_center_incr(buf, &offset, get_print_height(), ellipses, wcslen(ellipses));
            int backwards_start = len;
            backwards = backwards->next;
            for (FakeNode n = backwards; n != NULL; n = n->next) backwards_start--;
            print_out_nodes(list, backwards, NULL, buf, node_sizes, &offset, after_node, backwards_start);
        }

        // print end character
        write_str_center_incr(buf, &offset, get_print_height(), end_of_list, wcslen(end_of_list));
    }

    printf("%s: %s\n", collection_name, list->name);
    for (int i = 0; i < get_print_height(); i++) {
        printf("%ls\n", buf[i]);
        free(buf[i]);
    }
    for (int i = get_print_height(); i < get_ptr_height() + get_print_height(); i++) {
        bool found_non_space = false;
        for (int j = 0; j < count; j++) {
            if (buf[i][j] != ' ') {
                found_non_space = true;
                break;
            }
        }
        if (found_non_space) printf("%ls\n", buf[i]);
        free(buf[i]);
    }

    assert_msg(offset == count, "list_helper:list_print_general, "
                                "we promised to print out %d characters and "
                                "printed out just %d\n", count, offset);
    printf("\n");

    free(buf);
    free(node_sizes);
}

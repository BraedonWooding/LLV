#include "list_helper.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/helper.h"
#include "general_collection_helper.h"
#include "../lib/obsidian.h"

void print_bounding_box(char **buf, size_t offset, size_t len, size_t width);
void print_out_nodes(Collection list, FakeNode begin, FakeNode end, char **buf, size_t *node_sizes,
                         size_t *offset, char *after_node, int starting_size);

size_t list_sizeof(void *n) {
    FakeNode node = (FakeNode)n;
    return sizeof_data(node->data, node->data_tag) + EXTRA_WIDTH;
}

void print_ptr(char **buf, size_t len, size_t count, char *ptr_name, size_t ptr_len, size_t offset) {
    // @REFACTORING
    // this is just a very early version
    // I'm going to update this once I figure out visual ptrs a bit more
    // a lot of this is hand waivey and magic variables
    int i = 0;
    while (i < DEFAULT_PTR_HEIGHT && buf[len + i][offset] == '^') i++;

    if (i == DEFAULT_PTR_HEIGHT) {
        // we can fit two if we are clever
        // we want to make sure though that we haven't done this before
        // and find a level that we can fit the second one on
        i = 0;
        while (i < DEFAULT_PTR_HEIGHT && buf[len + i][offset + count] != ' ') i++;
        if (i == DEFAULT_PTR_HEIGHT) {
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
        buf[i + len][offset + 3] = '^';
    } else {
        int max = ptr_len + 1 > 5 ? 5 : ptr_len + 1;
        for (int j = offset + 1; j < offset + max; j++) buf[i + len][j] = ptr_name[j - offset - 1];
        buf[i + len][offset] = '^';
    }
}

void list_print_node(void *n, char **buf, size_t size, size_t len, size_t offset) {
    FakeNode node = (FakeNode)n;
    print_bounding_box(buf, offset, len, size);
    char *text_to_print;
    switch (node->data_tag) {
        case FLOAT: {
            text_to_print = malloc_with_oom(size - EXTRA_WIDTH / 2 + 1, "Text to print");
            snprintf(text_to_print, size - EXTRA_WIDTH / 2, "%g", node->data.flt_data);
        } break;
        case STRING: {
            text_to_print = node->data.str_data;
        } break;
        case INTEGER: {
            // note: we are including the '\0' room just to make nothing weird happen
            // with snprintf, but we will completely ignore it below in memcpy.
            text_to_print = malloc_with_oom(size - EXTRA_WIDTH / 2 + 1, "Text to print");
            snprintf(text_to_print, size - EXTRA_WIDTH / 2, "%lld", node->data.int_data);
        } break;
        case ANY: {
            text_to_print = malloc_with_oom(size - EXTRA_WIDTH / 2 + 1, "Text to print");
            snprintf(text_to_print, size - EXTRA_WIDTH / 2, "%zu", (size_t)node->data.any_data);
        } break;
    }
    if (node->ptr != NULL) print_ptr(buf, len, size, node->ptr, strlen(node->ptr), offset);

    // our sizes are always buffered by '4'
    // a sprintf or similar is just going to give us nasty '\0'
    memcpy(buf[len / 2] + EXTRA_WIDTH/2 + offset, text_to_print, size - EXTRA_WIDTH);
    if (node->data_tag != STRING) free(text_to_print);
}

void print_bounding_box(char **buf, size_t offset, size_t len, size_t width) {
    write_str_repeat_char_vert(buf, offset, '|', len, 0);
    write_str_repeat_char_vert(buf, offset, '|', len, width - 1);
    write_str_repeat_char(buf[0], offset, '=', width);
    write_str_repeat_char(buf[len - 1], offset, '=', width);
}

void print_out_nodes(Collection list, FakeNode begin, FakeNode end, char **buf, size_t *node_sizes,
                         size_t *offset, char *after_node, int starting_size) {
    int i = starting_size;
    FakeNode n;
    for (n = begin; n != end; n = n->next, i++) {
        list_print_node(n, buf, node_sizes[i], list->vert_len, *offset);
        *offset += node_sizes[i];
        if (n->next != end) {
            write_str_center_incr(buf, offset, list->vert_len, after_node, strlen(after_node));
        }
    }
}

void list_print_general(Collection list, size_t len, size_t count, FakeNode forwards,
                FakeNode backwards, int stop, size_t *node_sizes, char *after_node,
                char *start_of_list, char *end_of_list, char *ellipses, FakeNode head) {
    terminalSize size = get_terminal_size();
    obs_assert((size_t)size.width, >=, count);

    // now we have sizes we can allocate buffer and prepare to print list
    // probably going to be a few characters bigger than we need but no harm no foul
    char **buf = malloc_with_oom(sizeof(char*) * (list->vert_len + DEFAULT_PTR_HEIGHT), "Buffer");
    for (int i = 0; i < list->vert_len + DEFAULT_PTR_HEIGHT; i++) {
        buf[i] = malloc_with_oom((count + 1) * sizeof(char), "Buffer");
        for (int j = 0; j < count; j++) buf[i][j] = ' ';
        buf[i][count] = '\0';
    }

    size_t offset = 0;
    FakeNode forward_stop = forwards;
    // if everything in the list fits on page
    bool everything_fits = forward_stop == NULL;

    if (head == NULL) {
        write_str_center_incr(buf, &offset, list->vert_len, NULL_NODE, strlen(NULL_NODE));
    } else {
        write_str_center_incr(buf, &offset, list->vert_len, start_of_list, strlen(start_of_list));
        print_out_nodes(list, head, forward_stop, buf, node_sizes, &offset, after_node, 0);

        if (!everything_fits) {
            write_str_center_incr(buf, &offset, list->vert_len, after_node, strlen(after_node));
            write_str_center_incr(buf, &offset, list->vert_len, ellipses, strlen(ellipses));
            size_t backwards_start = len;
            backwards = backwards->next;
            for (FakeNode n = backwards; n != NULL; n = n->next) backwards_start--;
            print_out_nodes(list, backwards, NULL, buf, node_sizes, &offset, after_node, backwards_start);
        }

        // print end character
        write_str_center_incr(buf, &offset, list->vert_len, end_of_list, strlen(end_of_list));
    }

    printf("List: %s\n", list->name);
    for (int i = 0; i < list->vert_len; i++) {
        printf("%s\n", buf[i]);
        free(buf[i]);
    }
    for (int i = list->vert_len; i < DEFAULT_PTR_HEIGHT + list->vert_len; i++) {
        bool found_non_space = false;
        for (int j = 0; j < count; j++) {
            if (buf[i][j] != ' ') {
                found_non_space = true;
                break;
            }
        }
        if (found_non_space) printf("%s\n", buf[i]);
        free(buf[i]);
    }

    obs_assert(offset, ==, count);
    printf("\n");

    free(buf);
    free(node_sizes);
}

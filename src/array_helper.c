#include "array_helper.h"

#include <stdio.h>
<<<<<<< HEAD
=======
#include <math.h>
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

<<<<<<< HEAD
=======
#include "../lib/obsidian.h"
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
#include "list_helper.h"
#include "general_collection_helper.h"
#include "env_var.h"

<<<<<<< HEAD
int array_get_sizes(Collection c, FakeArrayNode array, int len, int max, int **node_sizes, int *out_calculated_len) {
    int count = 0;
    *out_calculated_len = 0;
    if (len == 0) *node_sizes = NULL;
    else *node_sizes = (int*)malloc_with_oom(sizeof(int) * len, "Node Sizes");

    // same idea as DLL, go both ways
    int i;
=======
static size_t get_sizes(Collection c, FakeArrayNode array, size_t len, size_t max, size_t **node_sizes, size_t *out_actual_len) {
    size_t count = 0;
    *out_actual_len = 0;
    *node_sizes = len == 0 ? NULL : malloc_with_oom(sizeof(size_t) * len, "Node Sizes");

    // same idea as DLL, go both ways
    size_t i;
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
    int mod = -1;

    // this will handle the case where we have extra width at the end
    if (len > 0) count = -WIDTH;

    for (i = 0; i < (len + 1) / 2; i++) {
<<<<<<< HEAD
        int forward_size = c->get_sizeof(&array[i]);
=======
        size_t forward_size = c->get_sizeof(&array[i]);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
        (*node_sizes)[i] = forward_size;

        if (forward_size + count + WIDTH > max) {
            count += ELLIPSES_LEN;
            if (count > max) {
                mod = 1;
            }
            break;
        }
        count += forward_size + WIDTH;
<<<<<<< HEAD
        (*out_calculated_len)++;

        if (i == len / 2) break;

        int backward_size = c->get_sizeof(&array[len - 1 - i]);
=======
        (*out_actual_len)++;

        if (i == len / 2) break;

        size_t backward_size = c->get_sizeof(&array[len - 1 - i]);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
        (*node_sizes)[len - 1 - i] = backward_size;
        if (backward_size + count + WIDTH > max) {
            count += ELLIPSES_LEN;
            if (count > max) mod = 0;
            break;
        }
        count += backward_size + WIDTH;
<<<<<<< HEAD
        (*out_calculated_len)++;
=======
        (*out_actual_len)++;
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
    }

    // this presumes that we don't have one node bigger than the screen.
    // obviously this could be incorrect.
    if (mod != -1) {
        // we have to handle the size being over the limit
        while (count > max) {
            if (mod % 2 == 0) {
                // we want to undo front
                count -= (*node_sizes)[i] + WIDTH;
            } else {
                // undo back
                i--;
                count -= (*node_sizes)[len - 1 - i] + WIDTH;
            }
<<<<<<< HEAD
            (*out_calculated_len)--;
=======
            (*out_actual_len)--;
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            mod++;
        }
    }

<<<<<<< HEAD
    if ((*out_calculated_len == 1 && len > 1) ||
        (*out_calculated_len == 0 && len > 0)) {
=======
    if ((*out_actual_len == 1 && len > 1) ||
        (*out_actual_len == 0 && len > 0)) {
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
        printf("Error: No valid sizing constraint matches terminal size; i.e. increase your terminal size since on current size can't even fit the bare minimum\n");
        exit(1);
    }

    return count;
}

<<<<<<< HEAD
void print_array_like(Collection c, char *collection_type, FakeArrayNode data, int len) {
    terminalSize size = get_terminal_size();
    int *node_sizes;
    int calculated_len;
    int count = array_get_sizes(c, data, len, size.width, &node_sizes, &calculated_len);
    assert_msg(calculated_len <= len, "array_helper:print_array_like, calculated_len (%d) must be <= len (%d)\n", calculated_len, len);

    int total_height = get_print_height() + get_ptr_height();
    wchar_t **buf = (wchar_t**)malloc_with_oom(sizeof(wchar_t*) * total_height, "Buffer");
    for (int i = 0; i < total_height; i++) {
        buf[i] = (wchar_t*)malloc_with_oom((count + 1) * sizeof(wchar_t), "Buffer");
=======
void print_array_like(Collection c, char *collection_type, FakeArrayNode data, size_t len) {
    terminalSize size = get_terminal_size();
    size_t *node_sizes;
    size_t actual_len;
    size_t count = get_sizes(c, data, len, size.width, &node_sizes, &actual_len);
    obs_assert(actual_len, <=, len);

    size_t total_height = get_print_height() + get_ptr_height();
    wchar_t **buf = malloc_with_oom(sizeof(wchar_t*) * total_height, "Buffer");
    for (int i = 0; i < total_height; i++) {
        buf[i] = malloc_with_oom((count + 1) * sizeof(wchar_t), "Buffer");
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
        for (int j = 0; j < count; j++) buf[i][j] = ' ';
        buf[i][count] = '\0';
    }

<<<<<<< HEAD
    int front_len = calculated_len == len ? calculated_len : (calculated_len + 1) / 2;
    int offset = 0;
    for (int i = 0; i < front_len; i++) {
=======
    size_t front_len = actual_len == len ? actual_len : (actual_len + 1) / 2;
    size_t offset = 0;
    for (size_t i = 0; i < front_len; i++) {
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
        if (i != 0) {
            write_str_repeat_char_grid(buf, offset, ' ', get_print_height(), WIDTH, 0);
            offset += WIDTH;
        }
        list_print_node(&data[i], buf, node_sizes[i], get_print_height(), offset);
        offset += node_sizes[i];
    }

<<<<<<< HEAD
    if (front_len != calculated_len) {
=======
    if (front_len != actual_len) {
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
        // do middle/ellipses
        write_str_center_incr(buf, &offset, get_print_height(), ELLIPSES, ELLIPSES_LEN);

        // do back side
<<<<<<< HEAD
        for (int i = len - calculated_len / 2; i < len; i++) {
=======
        for (size_t i = len - actual_len / 2; i < len; i++) {
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            write_str_repeat_char_grid(buf, offset, ' ', get_print_height(), WIDTH, 0);
            offset += WIDTH;
            list_print_node(&data[i], buf, node_sizes[i], get_print_height(), offset);
            offset += node_sizes[i];
        }
    }

    printf("%s: %s\n", collection_type, c->name);
    for (int i = 0; i < get_print_height(); i++) {
        printf("%ls\n", buf[i]);
        free(buf[i]);
    }
    for (int i = get_print_height(); i < total_height; i++) {
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

<<<<<<< HEAD
    assert_msg(offset == count, "array_helper:print_array_like, "
                                "we promised to print out %d characters and "
                                "printed out just %d\n", count, offset);
=======
    obs_assert(offset, ==, count);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
    printf("\n");

    free(buf);
    free(node_sizes);
<<<<<<< HEAD
}
=======
}
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca

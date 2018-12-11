#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/LL.h"
#include "../../include/helper.h"
#include "../list_helper.h"

#define AFTER_NODE (" -> ")
#define AFTER_NODE_LEN (strlen(AFTER_NODE))

// This used to be `NULL_NODE " <- "` but I've made it empty (but still a definition)
// Since I felt that wasn't really what a linked list should look like as there is no
// previous pointer on the first member.  However the code will still work if reverted
// just incase it turns out we want this to look like it used to or some other way.
#define START_OF_LIST ("")
#define START_OF_LIST_LEN (strlen(START_OF_LIST))
#define END_OF_LIST (" -> " NULL_NODE)
#define END_OF_LIST_LEN (strlen(END_OF_LIST))
#define ELLIPSES ("... -> ")
#define ELLIPSES_LEN (strlen(ELLIPSES))

void LL_print_list(Collection list);

LL LL_new(char *name) {
    LL ll = malloc_with_oom(sizeof(struct _singly_linked_list_t), "LL");
    ll->name = name;
    ll->head = ll->tail = NULL;
    ll->list_printer = LL_print_list;
    ll->get_sizeof = list_sizeof;
    ll->node_printer = list_print_node;
    ll->vert_len = DEFAULT_PRINT_HEIGHT;
    return ll;
}

void LL_free(LL list) {
    LL_clear_list(list);
    free(list);
}

void LL_free_node(LL_Node n) {
    // should help catch any dereferencing memory that can't be accessed.
    n->next = NULL;
    free(n);
}

LL_Node LL_new_node(Data data, TypeTag type) {
    LL_Node new_node = malloc_with_oom(sizeof(struct _LL_node_t), "LL LL_Node");
    new_node->next = NULL;
    new_node->data = data;
    new_node->data_tag = type;
    new_node->ptr = NULL;
    return new_node;
}

void LL_push(LL list, LL_Node n) {
    LL_insert_before(list, n, list->head);
}

LL_Node LL_pop(LL list) {
    return LL_remove_node(list, list->head);
}

void LL_append(LL list, LL_Node n) {
    LL_insert_after(list, n, list->tail);
}

void LL_clear_list(LL list) {
    for (LL_Node cur = list->head; cur != NULL;) {
        LL_Node temp = cur;
        cur = cur->next;
        free(temp);
    }
    list->head = list->tail = NULL;
}

void LL_insert_after(LL list, LL_Node node, LL_Node at) {
    node->next = NULL;
    if (at == NULL) {
        list->tail = list->head = node;
    } else {
        LL_Node post_at = at->next;
        // at -> post_at => at -> node -> post_at;
        node->next = post_at;
        at->next = node;
        if (post_at == NULL) list->tail = node;
    }
}

void LL_insert_before(LL list, LL_Node node, LL_Node at) {
    node->next = NULL;
    if (at == NULL) {
        list->tail = list->head = node;
    } else {
        LL_Node at_prev = NULL;
        if (at == list->head) {
            list->head = node;
        } else {
            LL_Node at_prev = LL_find_prev(list, at);
            // at_prev -> at => at_prev -> node -> at
            at_prev->next = node;
        }
        node->next = at;
    }
}

LL_Node LL_remove_node(LL list, LL_Node node) {
    if (node == NULL) return NULL;

    LL_Node at_prev;
    if (node == list->head) {
        list->head = node->next;
        at_prev = NULL;
    } else {
        at_prev = LL_find_prev(list, node);
        if (at_prev == NULL) return NULL;
        at_prev->next = node->next;
    }
    if (node == list->tail) {
        list->tail = at_prev;
    }
    node->next = NULL;
    return node;
}

bool LL_is_empty(LL list) {
    return list->head == NULL;
}

LL_Node LL_find_prev(LL list, LL_Node at) {
    if (at != NULL) {
        bool found;
        LL_Node cur;
        for (cur = list->head; cur != NULL; cur = cur->next) {
            if (cur->next == at) return cur;
        }
    }
    return NULL;
}

LL_Node LL_find_next(LL_Node n) {
    if (n == NULL) return NULL;
    return n->next;
}

size_t *attempt_fit(LL list, size_t len, terminalSize size, size_t *out_count,
                    LL_Node *out_forwards, LL_Node *out_backwards, int *out_stop) {
    size_t *node_sizes = malloc_with_oom(sizeof(size_t) * len, "node_sizes");

    if (LL_is_empty(list)) {
        *out_stop = 0;
        *out_count = 1;
        return node_sizes;
    }

    *out_count = START_OF_LIST_LEN + NULL_NODE_LEN;
    *out_stop = 0;
    for (; *out_forwards != NULL; *out_forwards = (*out_forwards)->next, (*out_stop)++) {
        node_sizes[*out_stop] = list->get_sizeof(*out_forwards);
        *out_count += node_sizes[*out_stop] + AFTER_NODE_LEN;
    }

    // if we fit on screen then exit, we won't use out_backwards!
    if (*out_count <= size.width) {
        return node_sizes;
    }

    *out_count = NULL_NODE_LEN + ELLIPSES_LEN + START_OF_LIST_LEN;
    *out_stop = 0;
    *out_forwards = list->head;

    // how far backwards we can go
    int backwards_index = 0;
    // Account for odd lists by including the extra element on the left side
    for (; *out_stop < (len + 1) / 2; (*out_stop)++) {
        size_t forward_size = node_sizes[*out_stop] + AFTER_NODE_LEN;
        if (forward_size + *out_count > size.width) break;
        *out_forwards = (*out_forwards)->next;
        *out_count += forward_size;

        // @TODO: confirm that this is correct with a few tests
        // I'm like 110% sure this should work this way (otherwise it will double up on nodes)
        // but it is always good to make sure with a few tests that show this is needed.
        if (*out_stop == len / 2) break;

        size_t backward_size = node_sizes[len - 1 - *out_stop] + AFTER_NODE_LEN;
        if (backward_size + *out_count > size.width) break;
        *out_count += backward_size;
        backwards_index++;
    }

    // we need to actually traverse the list
    // find middle
    LL_Node middle = list->head;
    for (int i = 0; i < len / 2; i++) middle = middle->next;
    *out_backwards = middle;

    // now go forwards from backwards (len / 2) - backwards_index times
    for (int i = len / 2; i < len - 1 - backwards_index; i++) {
        *out_backwards = (*out_backwards)->next;
    }

    return node_sizes;
}

size_t LL_length(LL list) {
    int count = 0;
    for (LL_Node n = list->head; n != NULL; n = n->next) count++;
    return count;
}

void LL_print_list(Collection list) {
    LL ll = (LL)list;
    size_t len = LL_length(ll);
    size_t count;
    int stop;
    LL_Node forwards = ll->head;
    LL_Node backwards = ll->tail;
    terminalSize size = get_terminal_size();
    size_t *node_sizes = attempt_fit(ll, len, size, &count, &forwards, &backwards, &stop);
    list_print_general(list, len, count, (FakeNode)forwards, (FakeNode)backwards, stop, node_sizes,
                       AFTER_NODE, START_OF_LIST, END_OF_LIST, ELLIPSES, (FakeNode)ll->head, "Linked List");
}

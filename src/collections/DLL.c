#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/DLL.h"
#include "../../include/helper.h"
#include "../list_helper.h"

#define AFTER_NODE (" <-> ")
#define AFTER_NODE_LEN (strlen(AFTER_NODE))
#define START_OF_LIST (NULL_NODE " <-> ")
#define START_OF_LIST_LEN (strlen(START_OF_LIST))
#define END_OF_LIST (NULL_NODE)
#define END_OF_LIST_LEN (strlen(END_OF_LIST))
#define ELLIPSES (" ... <-> ")
#define ELLIPSES_LEN (strlen(ELLIPSES))

void DLL_print_list(Collection collection);

DLL DLL_new(char *name) {
    DLL dll = malloc_with_oom(sizeof(struct _doubly_linked_list_t), "DLL");
    dll->name = name;
    dll->head = dll->tail = NULL;
    dll->list_printer = DLL_print_list;
    dll->get_sizeof = list_sizeof;
    dll->node_printer = list_print_node;
    dll->vert_len = DEFAULT_PRINT_HEIGHT;
    return dll;
}

void DLL_free(DLL list) {
    DLL_clear_list(list);
    free(list);
}

void DLL_free_node(DLL_Node n) {
    // should help catch any dereferencing memory that can't be accessed.
    n->next = NULL;
    n->prev = NULL;
    free(n);
}

DLL_Node DLL_new_node(Data data, TypeTag type) {
    DLL_Node new_node = malloc_with_oom(sizeof(struct _dll_node_t), "DLL_Node");
    new_node->next = new_node->prev = NULL;
    new_node->data = data;
    new_node->data_tag = type;
    new_node->ptr = NULL;
    return new_node;
}

void DLL_push(DLL list, DLL_Node n) {
    DLL_insert_before(list, n, list->head);
}

DLL_Node DLL_pop(DLL list) {
    return DLL_remove_node(list, list->head);
}

void DLL_append(DLL list, DLL_Node n) {
    DLL_insert_after(list, n, list->tail);
}

void DLL_clear_list(DLL list) {
    for (DLL_Node cur = list->head; cur != NULL;) {
        DLL_Node temp = cur;
        cur = cur->next;
        free(temp);
    }
    list->head = list->tail = NULL;
}

void DLL_insert_after(DLL list, DLL_Node node, DLL_Node at) {
    node->next = NULL;
    DLL_Node post_at = at->next;
    // at -> post_at => at -> node -> post_at;
    node->next = post_at;
    at->next = node;
    if (post_at == NULL) list->tail = node;
}

void DLL_insert_before(DLL list, DLL_Node node, DLL_Node at) {
    node->next = NULL;
    DLL_Node at_prev = NULL;
    if (at == list->head) {
        list->head = node;
    } else {
        DLL_Node at_prev = DLL_find_prev(at);
        // at_prev -> at => at_prev -> node -> at
        at_prev->next = node;
    }
    node->next = at;
}

DLL_Node DLL_remove_node(DLL list, DLL_Node node) {
    if (node == list->head) {
        list->head = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }
    node->next = node->prev = NULL;
    return node;
}

bool DLL_is_empty(DLL list) {
    return list->head == NULL;
}

DLL_Node DLL_find_prev(DLL_Node at) {
    if (at == NULL) return NULL;
    return at->prev;
}

DLL_Node DLL_find_next(DLL_Node n) {
    if (n == NULL) return NULL;
    return n->next;
}

size_t DLL_length(DLL list) {
    int count = 0;
    for (DLL_Node n = list->head; n != NULL; n = n->next) count++;
    return count;
}

// @TODO: this is very wrong I'm sure!
// Should be based of LL but doesn't need to generate the entire list every time
size_t *attempt_fit(DLL list, size_t len, terminalSize size, size_t *out_count,
                    DLL_Node *out_forwards, DLL_Node *out_backwards, int *out_stop) {
    size_t *node_sizes = malloc_with_oom(sizeof(size_t) * len, "node_sizes");

    *out_count = START_OF_LIST_LEN;
    *out_stop = 0;
    // only go through half the list
    for (; *out_stop < (len + 1) / 2 && *out_count < size.width; (*out_stop)++) {
        node_sizes[*out_stop] = list->get_sizeof(*out_forwards);
        size_t forward_size = node_sizes[*out_stop] + AFTER_NODE_LEN;
        if (*out_count + forward_size > size.width) break;
        *out_count += forward_size;

        if (*out_stop == len / 2) break;

        node_sizes[len - 1 - *out_stop] = list->get_sizeof(*out_backwards);
        size_t backward_size = node_sizes[len - 1 - *out_stop] + AFTER_NODE_LEN;
        if (*out_count + backward_size > size.width) break;
        *out_count += node_sizes[len - 1 - *out_stop];

        (*out_forwards) = (*out_forwards)->next;
        (*out_backwards) = (*out_backwards)->prev;
    }

    if (*out_stop != len / 2) {
        
    } else {
        // go through entire list
        *out_forwards = list->head;
        *out_backwards = NULL;
    }

    return node_sizes;
}

void DLL_print_list(Collection list) {
    DLL dll = (DLL)list;
    size_t len = DLL_length(dll);
    size_t count;
    int stop;
    DLL_Node forwards = dll->head;
    DLL_Node backwards = dll->tail;
    terminalSize size = get_terminal_size();
    size_t *node_sizes = attempt_fit(dll, len, size, &count, &forwards, &backwards, &stop);
    list_print_general(list, len, count, (FakeNode)forwards, (FakeNode)backwards, stop, node_sizes, AFTER_NODE,
               START_OF_LIST, END_OF_LIST, ELLIPSES, (FakeNode)dll->head, "Doubly Linked List");
}

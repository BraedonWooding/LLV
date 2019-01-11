#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "../../include/collections/dll.h"
#include "../../include/helper.h"
#include "../list_helper.h"

#define AFTER_NODE (select_str_unicode(L" ⟺   ", L" <-> "))
#define AFTER_NODE_LEN (wcslen(AFTER_NODE))
#define START_OF_LIST (select_str_unicode(NULL_NODE L" ⟺  ", NULL_NODE L" <-> "))
#define START_OF_LIST_LEN (wcslen(START_OF_LIST))
#define END_OF_LIST (select_str_unicode(L" ⟺   " NULL_NODE, L" <-> " NULL_NODE))
#define END_OF_LIST_LEN (wcslen(END_OF_LIST))
#define ELLIPSES (select_str_unicode(L"... ⟺   ", L"... <-> "))
#define ELLIPSES_LEN (wcslen(ELLIPSES))

void dll_print_list(Collection collection);

DLL dll_new(char *name) {
    DLL dll = malloc_with_oom(sizeof(struct _doubly_linked_list_t), "DLL");
    dll->name = name;
    dll->head = dll->tail = NULL;
    dll->list_printer = dll_print_list;
    dll->get_sizeof = list_sizeof;
    dll->node_printer = list_print_node;
    return dll;
}

void dll_free(DLL list) {
    dll_clear_list(list);
    free(list);
}

void dll_free_node(DLL_Node n) {
    // should help catch any dereferencing memory that can't be accessed.
    n->next = NULL;
    n->prev = NULL;
    free(n);
}

DLL_Node dll_new_node(Data data, TypeTag type) {
    DLL_Node new_node = malloc_with_oom(sizeof(struct _dll_node_t), "DLL_Node");
    new_node->next = new_node->prev = NULL;
    new_node->data = data;
    new_node->data_tag = type;
    new_node->ptr = NULL;
    return new_node;
}

void dll_push(DLL list, DLL_Node n) {
    dll_insert_before(list, n, list->head);
}

DLL_Node dll_pop(DLL list) {
    return dll_remove_node(list, list->head);
}

void dll_append(DLL list, DLL_Node n) {
    dll_insert_after(list, n, list->tail);
}

void dll_clear_list(DLL list) {
    for (DLL_Node cur = list->head; cur != NULL;) {
        DLL_Node temp = cur;
        cur = cur->next;
        free(temp);
    }
    list->head = list->tail = NULL;
}

void dll_insert_after(DLL list, DLL_Node node, DLL_Node at) {
    node->next = NULL;
    if (at == NULL) {
        list->tail = list->head = node;
    } else {
        DLL_Node post_at = at->next;
        // at -> post_at => at -> node -> post_at;
        node->next = post_at;
        at->next = node;
        node->prev = at;
        if (post_at == NULL) list->tail = node;
    }
}

void dll_insert_before(DLL list, DLL_Node node, DLL_Node at) {
    if (at == list->head) {
        list->head = node;
        node->prev = NULL;
        node->next = at;
        if (at != NULL) {
            at->prev = node;
        }
    } else if (at != NULL) {
        at->prev->next = node;
        node->next = at;
        at->prev = node;
    }
}

DLL_Node dll_remove_node(DLL list, DLL_Node node) {
    if (node == NULL) return NULL;
    bool found = false;
    for (DLL_Node c = list->head; c != NULL && !found; c = c->next) found = c == node;
    if (!found) return NULL;

    if (list->head == node) {
        list->head = node->next;
    }
    if (list->tail == node) {
        list->tail = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    node->next = node->prev = NULL;
    return node;
}

bool dll_is_empty(DLL list) {
    return list->head == NULL;
}

DLL_Node dll_find_prev(DLL_Node at) {
    if (at == NULL) return NULL;
    return at->prev;
}

DLL_Node dll_find_next(DLL_Node n) {
    if (n == NULL) return NULL;
    return n->next;
}

size_t dll_length(DLL list) {
    int count = 0;
    for (DLL_Node n = list->head; n != NULL; n = n->next) count++;
    return count;
}

size_t *dll_attempt_fit(DLL list, size_t len, terminalSize size, size_t *out_count,
                    DLL_Node *out_forwards, DLL_Node *out_backwards, int *out_stop) {
    size_t *node_sizes = malloc_with_oom(sizeof(size_t) * len, "node_sizes");

    if (dll_is_empty(list)) {
        *out_stop = 0;
        *out_count = NULL_NODE_LEN;
        return node_sizes;
    }

    *out_count = START_OF_LIST_LEN + NULL_NODE_LEN + ELLIPSES_LEN;
    *out_stop = 0;
    *out_forwards = list->head;
    *out_backwards = list->tail;

    // only go through half the list
    bool broke_due_to_size = false;
    for (; *out_stop < (len + 1) / 2; (*out_stop)++) {
        node_sizes[*out_stop] = list->get_sizeof(*out_forwards);
        size_t forward_size = node_sizes[*out_stop] + AFTER_NODE_LEN;
        if (*out_count + forward_size > size.width) {
            broke_due_to_size = true;
            break;
        }
        *out_count += forward_size;
        (*out_forwards) = (*out_forwards)->next;

        if (*out_stop == len / 2) break;

        node_sizes[len - 1 - *out_stop] = list->get_sizeof(*out_backwards);
        size_t backward_size = node_sizes[len - 1 - *out_stop] + AFTER_NODE_LEN;
        if (*out_count + backward_size > size.width) {
            broke_due_to_size = true;
            break;
        }
        *out_count += backward_size;

        (*out_backwards) = (*out_backwards)->prev;
    }



    if (!broke_due_to_size) {
        // go through entire list
        *out_forwards = NULL;
        *out_backwards = NULL;
        *out_count -= ELLIPSES_LEN;
    } else {
        //*out_backwards = (*out_backwards)->prev;
    }

    return node_sizes;
}

void dll_print_list(Collection list) {
    DLL dll = (DLL)list;
    size_t len = dll_length(dll);
    size_t count;
    int stop;
    DLL_Node forwards = dll->head;
    DLL_Node backwards = dll->tail;
    terminalSize size = get_terminal_size();
    size_t *node_sizes = dll_attempt_fit(dll, len, size, &count, &forwards, &backwards, &stop);
    list_print_general(list, len, count, (FakeNode)forwards, (FakeNode)backwards, stop, node_sizes, AFTER_NODE,
               START_OF_LIST, END_OF_LIST, ELLIPSES, (FakeNode)dll->head, "Doubly Linked List");
}

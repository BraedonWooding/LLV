#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "../../include/collections/dll.h"
#include "../../include/helper.h"
#include "../list_helper.h"

#define DLL_AFTER_NODE (select_str_unicode(L" ⟺   ", L" <-> "))
#define DLL_AFTER_NODE_LEN (wcslen(DLL_AFTER_NODE))
#define DLL_START_OF_LIST (select_str_unicode(NULL_NODE L" ⟺  ", NULL_NODE L" <-> "))
#define DLL_START_OF_LIST_LEN (wcslen(DLL_START_OF_LIST))
#define DLL_END_OF_LIST (select_str_unicode(L" ⟺   " NULL_NODE, L" <-> " NULL_NODE))
#define DLL_END_OF_LIST_LEN (wcslen(DLL_END_OF_LIST))
#define DLL_ELLIPSES (select_str_unicode(L"... ⟺   ", L"... <-> "))
#define DLL_ELLIPSES_LEN (wcslen(DLL_ELLIPSES))

void dll_print_list(Collection collection);

DLL dll_new(char *name) {
    DLL dll = (DLL)malloc_with_oom(sizeof(struct _doubly_linked_list_t), "DLL");
    dll->parent.name = name;
    dll->head = dll->tail = NULL;
    dll->parent.list_printer = dll_print_list;
    dll->parent.get_sizeof = list_sizeof;
    dll->parent.node_printer = list_print_node;
    return dll;
}

void dll_free(DLL list) {
    dll_clear(list);
    free(list);
}

void dll_free_node(DLL_Node n) {
    // should help catch any dereferencing memory that can't be accessed.
    n->next = NULL;
    n->prev = NULL;
    free(n);
}

DLL_Node dll_new_node(Data data, TypeTag type) {
    DLL_Node new_node = (DLL_Node)malloc_with_oom(sizeof(struct _dll_node_t), "DLL_Node");
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

void dll_clear(DLL list) {
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
    for (DLL_Node c = list->head; c != NULL && !found; c = c->next) {
        found = (c == node);
    }
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

int dll_is_empty(DLL list) {
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

int dll_length(DLL list) {
    int count = 0;
    for (DLL_Node n = list->head; n != NULL; n = n->next) count++;
    return count;
}

int *dll_attempt_fit(DLL list, int len, terminalSize size, int *out_count,
                    DLL_Node *out_forwards, DLL_Node *out_backwards, int *out_stop) {
    int *node_sizes = (int*)malloc_with_oom(sizeof(int) * len, "node_sizes");

    if (dll_is_empty(list)) {
        *out_stop = 0;
        *out_count = NULL_NODE_LEN;
        return node_sizes;
    }

    *out_count = DLL_START_OF_LIST_LEN + NULL_NODE_LEN + DLL_ELLIPSES_LEN;
    *out_stop = 0;
    *out_forwards = list->head;
    *out_backwards = list->tail;

    // only go through half the list
    bool broke_due_to_size = false;
    for (; *out_stop < (len + 1) / 2; (*out_stop)++) {
        node_sizes[*out_stop] = list->parent.get_sizeof(*out_forwards);
        int forward_size = node_sizes[*out_stop] + DLL_AFTER_NODE_LEN;
        if (*out_count + forward_size > size.width) {
            broke_due_to_size = true;
            break;
        }

        *out_count += forward_size;
        *out_forwards = (*out_forwards)->next;

        if (*out_stop == len / 2) break;

        node_sizes[len - 1 - *out_stop] = list->parent.get_sizeof(*out_backwards);
        int backward_size = node_sizes[len - 1 - *out_stop] + DLL_AFTER_NODE_LEN;
        if (*out_count + backward_size > size.width) {
            broke_due_to_size = true;
            break;
        }

        *out_count += backward_size;
        *out_backwards = (*out_backwards)->prev;
    }

    if (*out_stop == 0 && broke_due_to_size) {
        printf("Error: No valid sizing constraint matches terminal size; i.e. "
               "increase your terminal size since on current size can't even "
               "fit the bare minimum\n");
        exit(1);
    }

    if (!broke_due_to_size) {
        // go through entire list
        *out_forwards = NULL;
        *out_backwards = NULL;
        *out_count -= DLL_ELLIPSES_LEN;
    }

    return node_sizes;
}

void dll_print_list(Collection list) {
    DLL dll = (DLL)list;
    int len = dll_length(dll);
    int count;
    int stop;
    DLL_Node forwards = dll->head;
    DLL_Node backwards = dll->tail;
    terminalSize size = get_terminal_size();
    int *node_sizes = dll_attempt_fit(dll, len, size, &count, &forwards,
                                      &backwards, &stop);
    list_print_general(list, len, count, (FakeNode)forwards,
                       (FakeNode)backwards, stop, node_sizes, DLL_AFTER_NODE,
                       DLL_START_OF_LIST, DLL_END_OF_LIST, DLL_ELLIPSES,
                       (FakeNode)dll->head, "Doubly Linked List");
}

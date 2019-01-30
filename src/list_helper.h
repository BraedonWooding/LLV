#ifndef LLV_COLLECTION_HELPER
#define LLV_COLLECTION_HELPER

#include <string.h>

#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"
#include "../include/helper.h"

#define NULL_NODE L"X"
#define NULL_NODE_LEN wcslen(NULL_NODE)
#define SPACES_ON_SIDE (2)
#define EXTRA_WIDTH (4)

#define BOX_HORIZONTAL select_char_unicode(L'═', L'=')
#define BOX_BOTTOM_RIGHT select_char_unicode(L'╝', L'=')
#define BOX_BOTTOM_LEFT select_char_unicode(L'╚', L'=')
#define BOX_VERT select_char_unicode(L'║', L'|')
#define BOX_TOP_LEFT select_char_unicode(L'╔', L'=')
#define BOX_TOP_RIGHT select_char_unicode(L'╗', L'=')

// make sure your struct can downcast to this
// don't malloc this!
typedef struct _fake_node_t {
    char *ptr;
    Data data; // the data type
    TypeTag data_tag; // the corresponding tag;
    struct _fake_node_t *next;
} *FakeNode;

int list_sizeof(void *n);

void list_print_node(void *n, wchar_t **buf, int size, int len, int offset);

void list_print_general(Collection list, int len, int count, FakeNode forwards,
                FakeNode backwards, int stop, int *node_sizes, wchar_t *after_node,
                wchar_t *start_of_list, wchar_t *end_of_list, wchar_t *ellipses, FakeNode head,
                char *collection_name);

#endif /* LLV_COLLECTION_HELPER */

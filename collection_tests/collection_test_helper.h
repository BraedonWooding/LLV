#ifndef LLV_COLLECTION_TEST_HELPER_H
#define LLV_COLLECTION_TEST_HELPER_H

#include <LLV/llv.h>

typedef struct _fake_node_t {
    char *ptr;
    Data data; // the data type
    TypeTag data_tag; // the corresponding tag;
    struct _fake_node_t *next;
} *FakeNode;

#define map_items(c, len, data, c_type, func) \
    for (int i = 0; i < len; i++) { \
        func(c, NEW_NODE(c_type, data[i])); \
    } \

#define get_data(data, list) \
    _Generic((list[0]), \
        long: data.int_data, \
        long long: data.int_data, \
        unsigned int: data.int_data, \
        unsigned long: data.int_data, \
        unsigned long long: data.int_data, \
        int: data.int_data, \
        float: data.flt_data, \
        double: data.flt_data, \
        long double: data.flt_data, \
        char: data.int_data, \
        short: data.int_data, \
        unsigned short: data.int_data, \
        char *: data.str_data, \
        bool: data.int_data, \
        default: data.any_data \
    )

#define test_strcmp_fakenode(list, exp) { \
    int i = 0; \
    for (FakeNode n = (FakeNode)list->head; n != NULL; n = n->next, i++) { \
        obs_test_strcmp(n->data.str_data, exp[i]) \
    } \
}

#define test_list(list, exp, type) { \
    int i = 0; \
    for (FakeNode n = (FakeNode)list->head; n != NULL; n = n->next, i++) { \
        obs_test(get_data(n->data, exp), ==, exp[i]) \
    } \
    obs_test(type##_length(list), ==, (size_t)i); \
}

#define test_empty_list(list, type) \
    obs_test(list->head, ==, NULL); \
    obs_test(list->tail, ==, NULL); \
    obs_test(type##_length(list), ==, (size_t)0); \
    obs_test_true(type##_is_empty(list)); \

#endif

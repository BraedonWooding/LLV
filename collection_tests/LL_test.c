#include "../include/collections/LL.h"
#include "../lib/obsidian.h"
#include "../include/llv.h"
#include <string.h>

// we use macros just because obs_test refers to variables
// that are defined in OBS_SETUP and so we need to be in scope

#define add_items_to_LL(list, len, data) \
    for (int i = 0; i < len; i++) { \
        ll_append(list, NEW_NODE(ll, data[i])); \
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

#define test_ll_strcmp(list, exp) { \
    int i = 0; \
    for (LL_Node n = list->head; n != NULL; n = n->next, i++) { \
        obs_test_strcmp(n->data.str_data, exp[i]) \
    } \
}

#define test_ll(list, exp) { \
    int i = 0; \
    for (LL_Node n = list->head; n != NULL; n = n->next, i++) { \
        obs_test(get_data(n->data, exp), ==, exp[i]) \
    } \
    obs_test(ll_length(list), ==, (size_t)i); \
}

#define test_empty(list) \
    obs_test(list->head, ==, NULL); \
    obs_test(list->tail, ==, NULL); \
    obs_test(ll_length(list), ==, (size_t)0); \
    obs_test_true(ll_is_empty(list)); \

int main(void) {
    OBS_SETUP("Linked List")

    OBS_TEST_GROUP("LL_new && ll_new_node", {
        OBS_TEST("Create list and test properties", {
            LL list = ll_new("1");
            obs_test_strcmp(list->name, "1");
            test_empty(list);
            ll_free(list);
        })

        OBS_TEST("Create node and test properties", {
            // NOTE: no point testing the other data members when selecting one
            // i.e. if we have set int_data = 4 we shouldn't test str_data
            // this is because it is classified as undefined behaviour and
            // we shouldn't expect undefined behaviour to behave in any way.

            // int node
            LL_Node node = ll_new_node((Data){.int_data = 4}, INTEGER);
            obs_test(node->data_tag, ==, INTEGER);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.int_data, ==, (long long)4);
            ll_free_node(node);

            // flt node
            node = ll_new_node((Data){.flt_data = 5.9}, FLOAT);
            obs_test(node->data_tag, ==, FLOAT);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.flt_data, ==, 5.9);
            ll_free_node(node);

            // str node
            node = ll_new_node((Data){.str_data = "Hello"}, STRING);
            obs_test(node->data_tag, ==, STRING);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            ll_free_node(node);

            // any node
            int x = 9;
            node = ll_new_node((Data){.any_data = &x}, ANY);
            obs_test(node->data_tag, ==, ANY);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test((int*)node->data.any_data, ==, &x);
            ll_free_node(node);
        })

        OBS_TEST("Create node using generic macros", {
            // int node
            LL_Node node = NEW_NODE(ll, 4);
            obs_test(node->data_tag, ==, INTEGER);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.int_data, ==, (long long)4);
            ll_free_node(node);

            // flt node
            node = NEW_NODE(ll, 5.9);
            obs_test(node->data_tag, ==, FLOAT);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.flt_data, ==, 5.9);
            ll_free_node(node);

            // str node
            node = NEW_NODE(ll, "Hello");
            obs_test(node->data_tag, ==, STRING);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            ll_free_node(node);

            // any node
            int x = 9;
            node = NEW_NODE(ll, &x);
            obs_test(node->data_tag, ==, ANY);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test((int*)node->data.any_data, ==, &x);
            ll_free_node(node);
        })
    });

    OBS_TEST_GROUP("LL_clear_list", {
        OBS_TEST("Clearing empty list", {
            LL list = ll_new("1");
            test_empty(list);
            ll_clear_list(list);
            test_empty(list);
            ll_free(list);
        })

        OBS_TEST("Clearing list with elements", {
            LL list = ll_new("2");
            long long *elements = ((long long[]){1, 2, 3, 4, 5});
            add_items_to_LL(list, 5, elements);
            test_ll(list, elements);
            ll_clear_list(list);
            test_empty(list);
            ll_free(list);
        })

        OBS_TEST("Clearing list then adding different elements", {
            LL list = ll_new("3");
            double *elements = ((double[]){5.0, 3.2, 100.5, 421.222, 1.022, 2.22, 0.2});
            add_items_to_LL(list, 7, elements);
            test_ll(list, elements);
            ll_clear_list(list);
            test_empty(list);
            char **new_elements = ((char*[]){"Hello", "World", "Wow"});
            add_items_to_LL(list, 3, new_elements);
            test_ll_strcmp(list, new_elements);
            ll_clear_list(list);
            test_empty(list);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("LL_insert_after/before", {
        OBS_TEST("Inserting after on empty list", {
            LL list = ll_new("1");
            test_empty(list);
            ll_insert_after(list, NEW_NODE(ll, 5), list->head);
            test_ll(list, (long long[]){5});
            ll_free(list);
        })

        OBS_TEST("Inserting before on empty list", {
            LL list = ll_new("2");
            test_empty(list);
            ll_insert_before(list, NEW_NODE(ll, "hello"), list->head);
            test_ll_strcmp(list, (char*[]){"hello"});
            ll_free(list);
        })

        OBS_TEST("Inserting after on single element list", {
            LL list = ll_new("3");
            ll_insert_after(list, NEW_NODE(ll, 5.4), list->head);
            test_ll(list, ((double[]){5.4}));
            ll_insert_after(list, NEW_NODE(ll, 2.2), list->head);
            test_ll(list, ((double[]){5.4, 2.2}));
            ll_free(list);
        })

        OBS_TEST("Inserting before on single element list", {
            LL list = ll_new("4");
            ll_insert_before(list, NEW_NODE(ll, 5.4), list->head);
            test_ll(list, ((double[]){5.4}));
            ll_insert_before(list, NEW_NODE(ll, 2.2), list->head);
            test_ll(list, ((double[]){2.2, 5.4}));
            ll_free(list);
        })

        OBS_TEST("Inserting after in middle of list", {
            LL list = ll_new("5");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            ll_insert_after(list, NEW_NODE(ll, 100), list->head->next->next->next);
            long long *result = ((long long[]){1, 2, 3, 4, 100, 5, 6, 7, 8, 9, 10});
            test_ll(list, result);
            ll_free(list);
        })

        OBS_TEST("Inserting after on last element", {
            LL list = ll_new("6");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            ll_insert_after(list, NEW_NODE(ll, 100), list->tail);
            long long *result = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100});
            test_ll(list, result);
            ll_free(list);
        })

        OBS_TEST("Inserting before in middle of list", {
            LL list = ll_new("7");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            ll_insert_before(list, NEW_NODE(ll, 100), list->head->next->next->next);
            long long *result = ((long long[]){1, 2, 3, 100, 4, 5, 6, 7, 8, 9, 10});
            test_ll(list, result);
            ll_free(list);
        })

        OBS_TEST("Inserting before on first element", {
            LL list = ll_new("8");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            ll_insert_before(list, NEW_NODE(ll, 100), list->head);
            long long *result = ((long long[]){100, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            test_ll(list, result);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("ll_remove_node", {
        OBS_TEST("Remove a NULL node from an empty list", {
            LL list = ll_new("1");
            obs_assert(ll_remove_node(list, NULL), ==, NULL);
            ll_free(list);
        })

        OBS_TEST("Remove a non null node from an empty list", {
            LL list = ll_new("1");
            LL_Node n = NEW_NODE(ll, 5);
            obs_assert(ll_remove_node(list, n), ==, NULL);
            ll_free_node(n);
            ll_free(list);
        })

        OBS_TEST("Remove a node from an non empty list", {
            LL list = ll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            ll_remove_node(list, list->head->next->next);
            long long *result = ((long long[]){1, 2, 4, 5, 6, 7, 8, 9, 10});
            test_ll(list, result);
            ll_free(list);
        })

        OBS_TEST("Remove the only node from an non empty list", {
            LL list = ll_new("4");
            LL_Node original = NEW_NODE(ll, 1);
            ll_insert_after(list, original, list->head);
            LL_Node n = ll_remove_node(list, list->head);
            test_empty(list);
            obs_test(original, ==, n);
            ll_free(list);
            ll_free_node(n);
        })

        OBS_TEST("Remove first and last then the rest (from forward to back)", {
            LL list = ll_new("5");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            // remove front
            ll_free_node(ll_remove_node(list, list->head));
            long long *result1 = ((long long[]){2, 3, 4, 5, 6, 7, 8, 9, 10});
            test_ll(list, result1);
            // remove back
            ll_free_node(ll_remove_node(list, list->tail));
            long long *result2 = ((long long[]){2, 3, 4, 5, 6, 7, 8, 9});
            test_ll(list, result2);
            // remove rest
            while (!ll_is_empty(list)) {
                ll_free_node(ll_remove_node(list, list->head));
            }
            test_empty(list);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("ll_is_empty", {
        OBS_TEST("Empty list", {
            LL list = ll_new("1");
            obs_test_true(ll_is_empty(list));
            ll_free(list);
        })

        OBS_TEST("Single element list", {
            LL list = ll_new("2");
            ll_append(list, NEW_NODE(ll, 2));
            obs_test_false(ll_is_empty(list));
            ll_free(list);
        })

        OBS_TEST("Multi element list", {
            LL list = ll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            obs_test_false(ll_is_empty(list));
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("LL_find_prev/next", {
        OBS_TEST("Previous/Next of first element", {
            LL list = ll_new("1");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            obs_test(ll_find_prev(list, list->head), ==, NULL);
            obs_test(ll_find_next(list->head)->data.int_data, ==, (long long)2);
            ll_free(list);
        })

        OBS_TEST("Previous/Next of last element", {
            LL list = ll_new("2");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            obs_test(ll_find_prev(list, list->tail)->data.int_data, ==, (long long)9);
            obs_test(ll_find_next(list->tail), ==, NULL);
            ll_free(list);
        })

        OBS_TEST("Previous/Next of null element", {
            LL list = ll_new("3");
            obs_test(ll_find_prev(list, NULL), ==, NULL);
            obs_test(ll_find_next(NULL), ==, NULL);
            ll_free(list);
        })

        OBS_TEST("Previous/Next of middle elements", {
            LL list = ll_new("4");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            obs_test(ll_find_prev(list, list->head->next->next), ==, list->head->next);
            obs_test(ll_find_next(list->head->next->next->next), ==, list->head->next->next->next->next);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("ll_length", {
        OBS_TEST("Empty list", {
            LL list = ll_new("1");
            obs_test(ll_length(list), ==, (size_t)0);
            ll_free(list);
        })

        OBS_TEST("Single element list", {
            LL list = ll_new("2");
            ll_push(list, NEW_NODE(ll, 2));
            obs_test(ll_length(list), ==, (size_t)1);
            ll_free(list);
        })

        OBS_TEST("Multi element list", {
            LL list = ll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            obs_test(ll_length(list), ==, (size_t)10);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("LL_push/pop", {
        OBS_TEST("Push onto empty list, then pop it", {
            LL list = ll_new("1");
            ll_push(list, NEW_NODE(ll, 4));
            obs_test(ll_length(list), ==, (size_t)1);
            obs_test(list->head->data.int_data, ==, (long long)4);
            LL_Node n = ll_pop(list);
            obs_test(n->data.int_data, ==, (long long)4);
            ll_free_node(n);
            obs_test(ll_length(list), ==, (size_t)0);
            ll_free(list);
        })

        OBS_TEST("Pop empty list", {
            LL list = ll_new("2");
            obs_test(ll_pop(list), ==, NULL);
            ll_free(list);
        })

        OBS_TEST("Push multiple then pop them", {
            long long *elements = ((long long[]){1, 2, 5, 9, 2, 4, 5});
            long long *result = ((long long[]){5, 4, 2, 9, 5, 2, 1});
            LL list = ll_new("3");
            for (int i = 0; i < 7; i++) {
                ll_push(list, NEW_NODE(ll, elements[i]));
            }
            test_ll(list, result);
            for (int i = 0; i < 7; i++) {
                LL_Node n = ll_pop(list);
                obs_test(n->data.int_data, ==, result[i]);
                ll_free_node(n);
            }
            obs_test(ll_length(list), ==, (size_t)0);
            ll_free(list);
        })
    })

    OBS_TEST_GROUP("LL_append", {
        OBS_TEST("Append to empty list", {
            LL list = ll_new("1");
            ll_append(list, NEW_NODE(ll, 10));
            obs_test(ll_length(list), ==, (size_t)1);
            obs_test(list->head->data.int_data, ==, (long long)10);
            ll_free(list);
        })

        OBS_TEST("Append to non-empty list", {
            LL list = ll_new("1");
            long long *elements = ((long long[]){1, 2, 5, 9, 2, 4, 5});
            add_items_to_LL(list, 7, elements);
            test_ll(list, elements);
            ll_append(list, NEW_NODE(ll, 10));
            obs_test(ll_length(list), ==, (size_t)8);
            obs_test(list->tail->data.int_data, ==, (long long)10);
            ll_free(list);
        })
    })

    OBS_REPORT
}
